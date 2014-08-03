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

#include "MUI.h"
#include <math.h>


void mui::ScrollPaneView::handleDraw(){
	if( !visible ) return;
	
	ofPushMatrix();
	ofTranslate( (int)x, (int)y );
	
	if( opaque ) drawBackground();
	draw();
	
	if( MUI_DEBUG_DRAW ){
		ofNoFill();
		ofSetColor( 255, 0, 0 );
		ofRect( 0, 0, width, height );
	}
	
	std::vector<Container*>::reverse_iterator it = children.rbegin();
	while( it != children.rend() ) {
		// TODO: GIANT rectangles are not account for
		Container * c = (*it);
		bool inside =
			Helpers::inside( parent, x+c->x, y+c->y ) ||
			Helpers::inside( parent, x+c->x+c->width, y+c->y ) ||
			Helpers::inside( parent, x+c->x, y+c->y+c->height ) ||
			Helpers::inside( parent, x+c->x+c->width, y+c->y+c->height );

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
	ofRectangle bounds = getViewBoundingBox(); 
	
	minScrollX = fminf( 0, bounds.x ); 
	minScrollY = fminf( 0, bounds.y ); 
	maxScrollX = fmaxf( 0, bounds.x + bounds.width - width ); 
	maxScrollY = fmaxf( 0, bounds.y + bounds.height - height );
	
	wantsToScrollX = maxScrollX != 0 || minScrollX != 0; 
	wantsToScrollY = maxScrollY != 0 || minScrollY != 0; 
	
	view->width = fmaxf( width, maxScrollX + width ); 
	view->height = fmax( height, maxScrollY + height ); 
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
		minX = fminf( (*it)->x, minX ); 
		minY = fminf( (*it)->y, minX ); 
		maxX = fmaxf( (*it)->x + (*it)->width, maxX );
		maxY = fmaxf( (*it)->y + (*it)->height, maxY ); 
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

//--------------------------------------------------------------
void mui::ScrollPane::update(){
	if( pressed ){
/*		scrollX = ofClamp( scrollX, minScrollX - MUI_SCROLLPANE_BLEED*2, maxScrollX + MUI_SCROLLPANE_BLEED*2 );
		scrollY = ofClamp( scrollY, minScrollY - MUI_SCROLLPANE_BLEED*2, maxScrollY + MUI_SCROLLPANE_BLEED*2 );
		currentScrollX += ( getScrollTarget( scrollX, minScrollX, maxScrollX ) - currentScrollX )/3;
		currentScrollY += ( getScrollTarget( scrollY, minScrollY, maxScrollY ) - currentScrollY )/3;
		cout << "pressed, scrollX = " << scrollX << " // " << currentScrollX << endl; */
	}
	else if( animating ){
		long t = ofGetElapsedTimeMicros(); 
		float totalT = ( t - animationStartTime  ) / 1000.0; 
		float dt = ( t - lastAnimationTime ) / 1000000.0; 
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
				if( currentScrollX > maxScrollX || currentScrollY > maxScrollY || currentScrollX < minScrollX || currentScrollY < minScrollY )
					beginBaseAnimation( ofClamp( currentScrollX, minScrollX, maxScrollX ), ofClamp( currentScrollY, minScrollY, maxScrollY ) ); 
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
	mui::Helpers::pushScissor( this );
	Container::handleDraw(); 
	mui::Helpers::popScissor();
}


//--------------------------------------------------------------
void mui::ScrollPane::updateTouchVelocity( ofTouchEventArgs &touch ){
	float dx = - touch.x + pressedX; 
	float dy = - touch.y + pressedY;
	
	long dt = ofGetElapsedTimeMicros() - lastTouchTime; 
	lastTouchTime = ofGetElapsedTimeMicros(); 
	float pct = 0.01 + MIN( 0.99, dt/100000.0 ); 
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
	pressed = true; 
	animating = animatingToBase = animatingMomentum = false; 
	updateTouchVelocity( touch ); 
}


//--------------------------------------------------------------
void mui::ScrollPane::touchMoved( ofTouchEventArgs &touch ){
	if( pressed ){
//		if( canScrollX && wantsToScrollX ) scrollX -= ( touch.x - pressedX ); 
//		if( canScrollY && wantsToScrollY ) scrollY -= ( touch.y - pressedY ); 
		
		float wantX = initialX - touch.x + touchStartX; 
		float wantY = initialY - touch.y + touchStartY; 
		
		if( wantX > maxScrollX ) wantX = ( wantX + maxScrollX ) / 2; 
		else if( wantX < minScrollX ) wantX = ( wantX + minScrollX ) / 2; 
		else scrollX = wantX; 
		
		if( wantY > maxScrollY ) wantY = ( wantY + maxScrollY ) / 2; 
		else if( wantY < minScrollY ) wantY = ( wantY + minScrollY ) / 2; 
		else wantY = wantY; 
		
		if( canScrollX ) currentScrollX = wantX; 
		if( canScrollY ) currentScrollY = wantY; 
		
		updateTouchVelocity( touch ); 
		pressedX = touch.x; 
		pressedY = touch.y; 
	}
}


//--------------------------------------------------------------
void mui::ScrollPane::touchMovedOutside( ofTouchEventArgs &touch ){
	touchMoved( touch ); 
}


//--------------------------------------------------------------
void mui::ScrollPane::touchUp( ofTouchEventArgs &touch ){
	if( pressed ){
		updateTouchVelocity( touch );
		if( ( canScrollX && ABS( velX ) > 50 ) || ( canScrollY && ABS( velY ) > 50 ) ){
			beginMomentumAnimation();
		}
		else if( currentScrollX > maxScrollX || currentScrollY > maxScrollY || currentScrollX < minScrollX || currentScrollY < minScrollY )
			beginBaseAnimation( ofClamp( currentScrollX, minScrollX, maxScrollX ), ofClamp( currentScrollY, minScrollY, maxScrollY ) ); 
		else if( ( canScrollX && ABS( velX ) > 30 ) || ( canScrollY && ABS( velY ) ) > 30 ){
			beginMomentumAnimation();
		}
	}

	pressed = false;
	watchingTouch[touch.id] = false;

}


//--------------------------------------------------------------
void mui::ScrollPane::touchUpOutside( ofTouchEventArgs &touch ){
	touchUp( touch );
}


//--------------------------------------------------------------
void mui::ScrollPane::touchDoubleTap( ofTouchEventArgs &touch ){
}



//--------------------------------------------------------------
mui::Container * mui::ScrollPane::handleTouchDown( ofTouchEventArgs &touch ){
	if( !pressed ){
		if( touch.x >= 0 && touch.x <= width && touch.y >= 0 && touch.y <= height ){
			if( animating && ( !animatingMomentum || ofDist(0,0,animateX/1000,animateY/1000) >= 0.1 ) && !animatingToBase /*&& ( !animatingToBase || ofDist(animateToX, animateToY,currentScrollX,currentScrollY ) >= 5 )*/){
				// you want something? just take it, it's yours!
				touchDown( touch );
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
	else if( pressed && singleTouchId == touch.id ){
		return this;
	}
	
	Container *c = Container::handleTouchDown( touch );
	return c;
}


//--------------------------------------------------------------
mui::Container * mui::ScrollPane::handleTouchMoved( ofTouchEventArgs &touch ){
	if( !pressed && watchingTouch[touch.id] ){
		// we don't care about "wantsToScrollX" anymore, 
		// because on touch devices you can drag around a bit anyways
		if(( canScrollX && /*wantsToScrollX && */fabsf( touchStart[touch.id].x - touch.x ) > 20 ) || 
		   ( canScrollY && /*wantsToScrollY && */fabsf( touchStart[touch.id].y - touch.y ) > 20 )
		){
			// steal focus! 
			if( Root::INSTANCE->becomeResponder( this, touch ) ){ 
				touchDown( touch ); // fake a touchdown
				watchingTouch[touch.id] = false;
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
		pressed = false;
		return this;
	}
	else{
		return Container::handleTouchUp( touch );
	}
}