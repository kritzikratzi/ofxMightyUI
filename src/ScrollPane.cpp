/*
 *  ScrollPane.cpp
 *  iPhoneEmptyExample
 *
 *  Created by hansi on 29.01.11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 * 
 * ------------------------------
 *  PLEASE READ BEFORE YOU CODE:
 * 
 *  This is a simple container thingie. 
 *  You can use it exactly like an OF main app, there are only small differences in how touch events work. 
 *  1. If you don't want an event to propagate to other containers just do a "return true;" at then end. 
 *  2. A container will only be notified of events that happen within it's bounds
 *  3. If you do a "return true" at the end of touchDown() your container will be remembered and will then 
 *	 receive all moved(), doubleTap() and touchUp() events, even if they happen outside the container. 
 *  4. The coordinates touch.x and touch.y are modified to match the drawing coordinate system, so 0/0 is at the 
 *	 left top corner of your container, not top left corner of your application. 
 */

#include "ScrollPane.h"
#include "Root.h"

mui::ScrollPane::ScrollPane( float x_, float y_, float width_, float height_ )
	: Container( x_, y_, width_, height_ ),
	wantsToScrollX(false), wantsToScrollY(false),
	canScrollX(true), canScrollY(true), scrollX(0), scrollY(0),
	maxScrollX(0), maxScrollY(0), minScrollX(0), minScrollY(0),
	autoLockToBottom(false),isAutoLockingToBottom(true),
	currentScrollX(0), currentScrollY(0),
	trackingState(INACTIVE),
	view( NULL ),
	animating(false), animatingToBase(false), animatingMomentum(false),
	usePagingH(false), numPagesAdded(0){
		init();
};

void mui::ScrollPaneView::handleDraw(){
	if( !visible ) return;
	
	ofPushMatrix();
	ofTranslate( (int)x, (int)y );
	
	if( opaque ) drawBackground();
	draw();
	
	if( MUI_DEBUG_DRAW ){
		ofNoFill();
		ofSetColor( 255, 0, 0 );
		ofDrawRectangle( 0, 0, width, height );
	}
	
	std::vector<Container*>::iterator it = children.begin();
	while( it != children.end() ) {
		// TODO: GIANT rectangles are not account for
		Container * c = (*it);
		bool inside =
			Helpers::inside( parent, x+c->x, y+c->y ) ||
			Helpers::inside( parent, x+c->x+c->width, y+c->y ) ||
			Helpers::inside( parent, x+c->x, y+c->y+c->height ) ||
			Helpers::inside( parent, x+c->x+c->width, y+c->y+c->height ) ||
			Helpers::inside( parent, x+c->x+c->width/2, y+c->y+c->height/2 );

		if( inside || c->drawDirty ){
			c->handleDraw();
		}
		++it;
	}
	
	ofPopMatrix();
}


void mui::ScrollPane::init(){
	singleTouch = true;
	ignoreEvents = true; 
	
	for( int i = 0; i < OF_MAX_TOUCHES; i++ ){
		watchingTouch[i] = false; 
	}
	
	view = new ScrollPaneView( 0, 0, width, height );
	view->ignoreEvents = true;
	view->name = "scroll-view"; 
	add( view );
}

//--------------------------------------------------------------
mui::ScrollPane::~ScrollPane(){
	delete view;
}

//--------------------------------------------------------------
void mui::ScrollPane::commit(){
	view->handleLayout();
	ofRectangle bounds = getViewBoundingBox();
	
	minScrollX = fminf( 0, bounds.x ); 
	minScrollY = fminf( 0, bounds.y ); 
	maxScrollX = fmaxf( 0, bounds.x + bounds.width - width );
	maxScrollY = fmaxf( 0, bounds.y + bounds.height - height );
	
	wantsToScrollX = maxScrollX != 0 || minScrollX != 0; 
	wantsToScrollY = maxScrollY != 0 || minScrollY != 0; 
	
	view->width = fmaxf( width, maxScrollX + width );
	view->height = fmaxf( height, maxScrollY + height );
	
	// todo: trigger layout (again!) when size changed?
	
	if( isAutoLockingToBottom && autoLockToBottom && trackingState == INACTIVE ){
		beginBaseAnimation( ofClamp( currentScrollX, minScrollX, maxScrollX ), maxScrollY );
	}
}

ofRectangle mui::ScrollPane::getViewBoundingBox(){
	// figure out min/max values... 
	std::vector<Container*>::iterator it = view->children.begin(); 
	float minX, minY, maxX, maxY; 
	
	minX = 0; 
	minY = 0; 
	maxX = 0; 
	maxY = 0; 
	
	while( it != view->children.end() ) {
		if( (*it)->visible ){
			minX = fminf( (*it)->x, minX );
			minY = fminf( (*it)->y, minX ); 
			maxX = fmaxf( (*it)->x + (*it)->width, maxX );
			maxY = fmaxf( (*it)->y + (*it)->height, maxY );
		}
		++it;
	}
	
	return ofRectangle( minX, minY, maxX - minX, maxY - minY ); 
}

void mui::ScrollPane::beginBaseAnimation( float toX, float toY ){
	animateX = toX - currentScrollX; 
	animateY = toY - currentScrollY; 
	animateToX = toX; 
	animateToY = toY; 
	animationStartTime = ofGetElapsedTimeMicros(); 
	lastAnimationTime = animationStartTime; 
	animating = true; 
	animatingToBase = true; 
	animatingMomentum = false; 
}

void mui::ScrollPane::beginMomentumAnimation(){
	animateX = velX; 
	animateY = velY; 
	animationStartTime = ofGetElapsedTimeMicros(); 
	lastAnimationTime = animationStartTime; 
	animating = true; 
	animatingToBase = false; 
	animatingMomentum = true; 
}

mui::Container * mui::ScrollPane::createPage(){
	static int pageCount = 0;
	mui::Container * container = new mui::Container( width*numPagesAdded, 0, width, height );
	container->name = "page-" + ofToString(pageCount++);
	view->add(container);
	container->ignoreEvents = true;
	numPagesAdded ++;
	commit();
	return container;
}

//--------------------------------------------------------------
mui::ScrollPane * mui::ScrollPane::createPageWithScrollPane(){
	static int scrollPageCount = 0;
	mui::ScrollPane * container = new mui::ScrollPane( width*numPagesAdded, 0, width, height );
	container->name = "scrollpage-" + ofToString(scrollPageCount++);
	view->add(container);
	container->ignoreEvents = true;
	container->canScrollX = false; 
	numPagesAdded ++;
	commit();
	return container;
}

//--------------------------------------------------------------
void mui::ScrollPane::nextPage(int inc){
	cout << getPageNum() << endl; 
	gotoPage( getPageNum() + inc);
}

//--------------------------------------------------------------
void mui::ScrollPane::prevPage(int dec){
	gotoPage( getPageNum() - dec);
}

//--------------------------------------------------------------
void mui::ScrollPane::gotoPage( int page ){
	page = (int) ofClamp(page, 0, MAX(numPages()-1,0));
	beginBaseAnimation(page*width, currentScrollY);
}

//--------------------------------------------------------------
int mui::ScrollPane::getPageNum(){
	if( animating && animatingToBase ){
		return (int) roundf(animateToX/width-0.499999f);
	}
	else{
		return (int) roundf(currentScrollX/width-0.499999f);
	}
}

//--------------------------------------------------------------
int mui::ScrollPane::numPages(){
	return numPagesAdded;
}


//--------------------------------------------------------------
void mui::ScrollPane::update(){
	if( trackingState != INACTIVE ){
/*		scrollX = ofClamp( scrollX, minScrollX - MUI_SCROLLPANE_BLEED*2, maxScrollX + MUI_SCROLLPANE_BLEED*2 );
		scrollY = ofClamp( scrollY, minScrollY - MUI_SCROLLPANE_BLEED*2, maxScrollY + MUI_SCROLLPANE_BLEED*2 );
		currentScrollX += ( getScrollTarget( scrollX, minScrollX, maxScrollX ) - currentScrollX )/3;
		currentScrollY += ( getScrollTarget( scrollY, minScrollY, maxScrollY ) - currentScrollY )/3;
		cout << "pressed, scrollX = " << scrollX << " // " << currentScrollX << endl; */
	}
	else if( animating ){
		long t = ofGetElapsedTimeMicros(); 
		float totalT = ( t - animationStartTime  ) / 1000.0f;
		float dt = ( t - lastAnimationTime ) / 1000000.0f;
		lastAnimationTime = t; 
		
		if( animatingToBase ){
			currentScrollX = animateToX - animateX*expf(-6*totalT/MUI_SCROLL_TO_BASE_DURATION);
			currentScrollY = animateToY - animateY*expf(-6*totalT/MUI_SCROLL_TO_BASE_DURATION);
			if( totalT > MUI_SCROLL_TO_BASE_DURATION ){
				currentScrollX = animateToX;
				currentScrollY = animateToY;
				animatingToBase = false;
				animating = false; 
			}
		}
		else if( animatingMomentum ){
			float factorX = ( currentScrollX > minScrollX && currentScrollX < maxScrollX )? 1 : ( MIN( ABS( currentScrollX - minScrollX ), ABS( currentScrollX - maxScrollX ) ) );
			float factorY = ( currentScrollY > minScrollY && currentScrollY < maxScrollY )? 1 : ( MIN( ABS( currentScrollY - minScrollY ), ABS( currentScrollY - maxScrollY ) ) );
			
#define SIGN(x) (x<0?-1:(x==0?0:1))
			float newAniX = animateX - SIGN(animateX)*MUI_SCROLL_VELOCITY_DECREASE*dt*factorX;
			float newAniY = animateY - SIGN(animateY)*MUI_SCROLL_VELOCITY_DECREASE*dt*factorY;

			if( SIGN( newAniX ) != SIGN( animateX ) ) newAniX = 0; 
			if( SIGN( newAniY ) != SIGN( animateY ) ) newAniY = 0; 
			
			if( canScrollX ) currentScrollX += ( animateX = newAniX )*dt;
			if( canScrollY ) currentScrollY += ( animateY = newAniY )*dt;
			
			if( ( !canScrollX || animateX == 0 ) && ( !canScrollY || animateY == 0 ) ){
				animateX = animateY = 0; 
				animating = false; 
				animatingMomentum = false; 
				if( currentScrollX > maxScrollX || currentScrollY > maxScrollY || currentScrollX < minScrollX || currentScrollY < minScrollY ){
					if( currentScrollY > maxScrollY ) isAutoLockingToBottom = true;
					beginBaseAnimation( ofClamp( currentScrollX, minScrollX, maxScrollX ), ofClamp( currentScrollY, minScrollY, maxScrollY ) );
				}
			}
#undef SIGN
			
		}
		else{
			animating = false; 
		}
	}

	if( canScrollX ) view->x = -currentScrollX;
	if( canScrollY ) view->y = -currentScrollY;
}


//--------------------------------------------------------------
void mui::ScrollPane::draw(){
}


//--------------------------------------------------------------
void mui::ScrollPane::drawBackground(){
	Container::drawBackground(); 
}


//--------------------------------------------------------------
// mostly a copy of Container::handleDraw
void mui::ScrollPane::handleDraw(){
	ofRectangle intersection = ofRectangle(0,0,width-7,height).getIntersection(view->getBounds());
	mui::Helpers::pushScissor( this, intersection );
	Container::handleDraw(); 
	mui::Helpers::popScissor();
	
	if( visible && minScrollY != maxScrollY ){
		ofDrawLine(x+width-4, y+2, x+width-4, y+height-2 );
		float scrubberHeight = ofClamp(height*height/(maxScrollY - minScrollY), 10, height/2);
		float scrubberPos = ofMap(currentScrollY, minScrollY, maxScrollY, 2, height-2-scrubberHeight);
		ofDrawRectangle(x+width-6, y+scrubberPos, 5, scrubberHeight );
	}
}


//--------------------------------------------------------------
void mui::ScrollPane::updateTouchVelocity( ofTouchEventArgs &touch ){
	float dx = - touch.x + pressedX; 
	float dy = - touch.y + pressedY;
	
	long dt = ofGetElapsedTimeMicros() - lastTouchTime; 
	lastTouchTime = ofGetElapsedTimeMicros(); 
	float pct = 0.01f + MIN( 0.99f, dt/100000.0f );
	velX = pct*(dx*1000000/dt) + (1-pct)*velX;
	velY = pct*(dy*1000000/dt) + (1-pct)*velY;
}


//--------------------------------------------------------------
// The next three functions handle the neat scrolling process. 
// they are mostly taken from my "scrolly.js"

//--------------------------------------------------------------
void mui::ScrollPane::touchDown( ofTouchEventArgs &touch ){
	pressedX = touch.x;
	pressedY = touch.y; 
	touchStartX = touch.x; 
	touchStartY = touch.y; 
	initialX = currentScrollX; 
	initialY = currentScrollY; 
	velX = 0; velY = 0; 
	trackingState = (canScrollY && touch.x > width - 13)?DRAG_SCROLLBAR:DRAG_CONTENT;
	animating = animatingToBase = animatingMomentum = false; 
	updateTouchVelocity( touch ); 
}


//--------------------------------------------------------------
void mui::ScrollPane::touchMoved( ofTouchEventArgs &touch ){
	if( trackingState == DRAG_CONTENT ){
//		if( canScrollX && wantsToScrollX ) scrollX -= ( touch.x - pressedX ); 
//		if( canScrollY && wantsToScrollY ) scrollY -= ( touch.y - pressedY ); 
		
		float wantX = initialX - touch.x + touchStartX; 
		float wantY = initialY - touch.y + touchStartY; 
		
		if( wantX > maxScrollX ) wantX = ( wantX + 2*maxScrollX ) / 3;
		else if( wantX < minScrollX ) wantX = ( wantX + 2*minScrollX ) / 3;
		else scrollX = wantX; 
		
		if( wantY > maxScrollY ) wantY = ( wantY + 2*maxScrollY ) / 3;
		else if( wantY < minScrollY ) wantY = ( wantY + 2*minScrollY ) / 3;
		else wantY = wantY; 
		
		if( canScrollX ) currentScrollX = wantX; 
		if( canScrollY ) currentScrollY = wantY;
		
		updateTouchVelocity( touch ); 
		pressedX = touch.x; 
		pressedY = touch.y; 
	}
	else if( trackingState == DRAG_SCROLLBAR ){
		float scrubberHeight = ofClamp((maxScrollY - minScrollY)/height, 10, height/2);
		//float scrubberPos = ofMap(currentScrollY, minScrollY, maxScrollY, 2, height-2-scrubberHeight);
		currentScrollY = ofMap( touch.y, 2+scrubberHeight/2, height-2-scrubberHeight/2, minScrollY, maxScrollY, true);
	}
}


//--------------------------------------------------------------
void mui::ScrollPane::touchMovedOutside( ofTouchEventArgs &touch ){
	touchMoved( touch ); 
}


//--------------------------------------------------------------
void mui::ScrollPane::touchUp( ofTouchEventArgs &touch ){
	if( trackingState == DRAG_CONTENT ){
		updateTouchVelocity( touch );
		isAutoLockingToBottom = false;
		if( usePagingH ){
			int page = 0;
			if( fabsf(velX ) < 5 ){
				page = (int)ofClamp( roundf(currentScrollX/width), 0, numPagesAdded-1 );
			}
			else{
				page = (int)ofClamp( roundf(currentScrollX/width+(velX<0?-.5f:+.5f)), 0, numPagesAdded-1 );
			}
			beginBaseAnimation(page*width, currentScrollY);
		}
		else if( ( canScrollX && ABS( velX ) > 50 ) || ( canScrollY && ABS( velY ) > 50 ) ){
			beginMomentumAnimation();
		}
		else if( currentScrollX > maxScrollX || currentScrollY > maxScrollY || currentScrollX < minScrollX || currentScrollY < minScrollY ){
			isAutoLockingToBottom = currentScrollY > maxScrollY;
			beginBaseAnimation( ofClamp( currentScrollX, minScrollX, maxScrollX ), ofClamp( currentScrollY, minScrollY, maxScrollY ) );
		}
		else if( ( canScrollX && ABS( velX ) > 30 ) || ( canScrollY && ABS( velY ) > 30 ) ){
			beginMomentumAnimation();
		}
	}

	trackingState = INACTIVE;
	watchingTouch[touch.id] = false;
	focusTransferable = true;

}


//--------------------------------------------------------------
void mui::ScrollPane::touchUpOutside( ofTouchEventArgs &touch ){
	touchUp( touch );
}


//--------------------------------------------------------------
void mui::ScrollPane::touchDoubleTap( ofTouchEventArgs &touch ){
}

//--------------------------------------------------------------
void mui::ScrollPane::touchCanceled( ofTouchEventArgs &touch ){
	watchingTouch[touch.id] = false;
	cout << "stop watching touch #" << touch.id << endl;
	trackingState = INACTIVE;
	focusTransferable = true;
}

void mui::ScrollPane::mouseScroll( ofMouseEventArgs &args){
	view->x = -(currentScrollX = ofClamp(currentScrollX-args.scrollX, minScrollX, maxScrollX));
	view->y = -(currentScrollY = ofClamp(currentScrollY-args.scrollY, minScrollY, maxScrollY));
}


//--------------------------------------------------------------
mui::Container * mui::ScrollPane::handleTouchDown( ofTouchEventArgs &touch ){
	if( !visible ){
		watchingTouch[touch.id] = false;
		return NULL;
	}
	if( trackingState == INACTIVE ){
		if( touch.x >= 0 && touch.x <= width && touch.y >= 0 && touch.y <= height ){
			if( animating && ( !animatingMomentum || ofDist(0,0,animateX/1000,animateY/1000) >= 0.1 ) && !animatingToBase /*&& ( !animatingToBase || ofDist(animateToX, animateToY,currentScrollX,currentScrollY ) >= 5 )*/){
				// you want something? just take it, it's yours!
				touchDown( touch );
				return this;
			}
			else if( canScrollY && wantsToScrollY && touch.x > width - 10 ){
				touchDown(touch);
				return this;
			}
			else{
				Container *c = Container::handleTouchDown( touch );
				if( c != NULL && !c->focusTransferable ){
					return c;
				}
				else{
					watchingTouch[touch.id] = true;
					touchStart[touch.id].x = touch.x;
					touchStart[touch.id].y = touch.y;
					
					return c == NULL? this : c;
				}
			}
		}
	}
	else if( trackingState != INACTIVE && singleTouchId == touch.id ){
		return this;
	}
	
	Container *c = Container::handleTouchDown( touch );
	return c;
}


//--------------------------------------------------------------
mui::Container * mui::ScrollPane::handleTouchMoved( ofTouchEventArgs &touch ){
	if( trackingState == INACTIVE && watchingTouch[touch.id] ){
		// we don't care about "wantsToScrollX" anymore, 
		// because on touch devices you can drag around a bit anyways
		if(( canScrollX && /*wantsToScrollX && */fabsf( touchStart[touch.id].x - touch.x ) > 20 ) ||
		   ( canScrollY && /*wantsToScrollY && */fabsf( touchStart[touch.id].y - touch.y ) > 20 )
		){
			cout << "steal focus for touch #" << touch.id << endl;
			// steal focus!
			if( Root::INSTANCE->becomeTouchResponder( this, touch ) ){
				touchDown( touch ); // fake a touchdown
				watchingTouch[touch.id] = false;
				focusTransferable = false;
			}
		}
	}

	return Container::handleTouchMoved( touch );
}


//--------------------------------------------------------------
mui::Container * mui::ScrollPane::handleTouchUp( ofTouchEventArgs &touch ){
	watchingTouch[touch.id] = false;
	if( singleTouchId == touch.id ){
		touchUp( touch );
		singleTouchId = -1;
		trackingState = INACTIVE;
		return this;
	}
	else{
		return Container::handleTouchUp( touch );
	}
}