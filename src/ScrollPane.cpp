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
 *     receive all moved(), doubleTap() and touchUp() events, even if they happen outside the container. 
 *  4. The coordinates touch.x and touch.y are modified to match the drawing coordinate system, so 0/0 is at the 
 *     left top corner of your container, not top left corner of your application. 
 */

#include "MUI.h"
#include <math.h>

void mui::ScrollPane::init(){
	singleTouch = false; 
	
	for( int i = 0; i < OF_MAX_TOUCHES; i++ ){
		watchingTouch[i] = false; 
	}
	
	view = new Container( 0, 0, width, height ); 
	view->ignoreEvents = true; 
	add( view ); 
	
}


//--------------------------------------------------------------
void mui::ScrollPane::commit(){
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
		maxY = fmaxf( (*it)->x + (*it)->width, maxX );
		maxY = fmaxf( (*it)->y + (*it)->height, maxY ); 
		
		++it; 
	}
	
	
	minScrollX = fminf( 0, minX ); 
	minScrollY = fminf( 0, minY ); 
	maxScrollX = fmaxf( 0, maxX - width ); 
	maxScrollY = fmaxf( 0, maxY - height );
	
	wantsToScrollX = maxScrollX != 0 || minScrollX != 0; 
	wantsToScrollY = maxScrollY != 0 || minScrollY != 0; 
	
	view->width = fmaxf( width, maxScrollX ); 
	view->height = fmax( height, maxScrollY ); 
}

//--------------------------------------------------------------
void mui::ScrollPane::update(){
	if( pressed ){
		scrollX = ofClamp( scrollX, minScrollX - MUI_SCROLLPANE_BLEED*2, maxScrollX + MUI_SCROLLPANE_BLEED*2 );
		scrollY = ofClamp( scrollY, minScrollY - MUI_SCROLLPANE_BLEED*2, maxScrollY + MUI_SCROLLPANE_BLEED*2 );
		
		currentScrollX += ( getScrollTarget( scrollX, minScrollX, maxScrollX ) - currentScrollX )/3;
		currentScrollY += ( getScrollTarget( scrollY, minScrollY, maxScrollY ) - currentScrollY )/3;
	}
	else{
		scrollX = ofClamp( scrollX, minScrollX, maxScrollX ); 
		scrollY = ofClamp( scrollY, minScrollX, maxScrollY ); 
		
		currentScrollX += ( scrollX - currentScrollX )/3; 
		currentScrollY += ( scrollY - currentScrollY )/3;
	}

	view->x = -currentScrollX;
	view->y = -currentScrollY;
}


//--------------------------------------------------------------
void mui::ScrollPane::draw(){
}


//--------------------------------------------------------------
void mui::ScrollPane::drawBackground(){
}


//--------------------------------------------------------------
// mostly a copy of Container::handleDraw
void mui::ScrollPane::handleDraw(){
	glEnable( GL_SCISSOR_TEST ); 
	ofPoint pos = getGlobalPosition(); 
	//TODO: clarify next line! 
	//maybe this is an ios-setup specific solution
	pos.y = ofGetHeight()-pos.y-height; 
	glScissor( pos.x, pos.y, width, height ); 
	
	Container::handleDraw(); 
	glDisable( GL_SCISSOR_TEST );
}


//--------------------------------------------------------------
void mui::ScrollPane::touchDown( ofTouchEventArgs &touch ){
	pressedX = touch.x;
	pressedY = touch.y; 
	pressed = true; 
}


//--------------------------------------------------------------
void mui::ScrollPane::touchMoved( ofTouchEventArgs &touch ){
	if( pressed ){
		if( canScrollX && wantsToScrollX ) scrollX -= ( touch.x - pressedX ); 
		if( canScrollY && wantsToScrollY ) scrollY -= ( touch.y - pressedY ); 
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
	pressed = false;
}


//--------------------------------------------------------------
void mui::ScrollPane::touchUpOutside( ofTouchEventArgs &touch ){
	pressed = false;
	watchingTouch[touch.id] = false;	
}


//--------------------------------------------------------------
void mui::ScrollPane::touchDoubleTap( ofTouchEventArgs &touch ){
}



//--------------------------------------------------------------
mui::Container * mui::ScrollPane::handleTouchDown( ofTouchEventArgs &touch ){
	if( !pressed ){
		watchingTouch[touch.id] = true;
		touchStart[touch.id].x = touch.x;
		touchStart[touch.id].y = touch.y;
	}
	
	return Container::handleTouchDown( touch ); 
}


//--------------------------------------------------------------
mui::Container * mui::ScrollPane::handleTouchMoved( ofTouchEventArgs &touch ){
	if( watchingTouch[touch.id] ){
		if(( canScrollX && wantsToScrollX && fabsf( touchStart[touch.id].x - touch.x ) > 20 ) || 
		   ( canScrollY && wantsToScrollY && fabsf( touchStart[touch.id].y - touch.y ) > 20 )
		){
			// steal focus! 
			touchDown( touch ); // fake a touchdown
			Root::INSTANCE->becomeResponder( this, touch );
			watchingTouch[touch.id] = false;
		}
	}

	return Container::handleTouchMoved( touch );
}


//--------------------------------------------------------------
mui::Container * mui::ScrollPane::handleTouchUp( ofTouchEventArgs &touch ){
	watchingTouch[touch.id] = false;
	
	return Container::handleTouchUp( touch ); 
}


//--------------------------------------------------------------
float mui::ScrollPane::getScrollTarget( float value, float min, float max ){
	if( value < min ){
		return min - MUI_SCROLLPANE_BLEED * powf(sinf( PI/2*fminf( 2*MUI_SCROLLPANE_BLEED, min - value )/MUI_SCROLLPANE_BLEED/2 ), 1);
	}
	else if( value < max ){
		return value; 
	}
	else{
		return max + MUI_SCROLLPANE_BLEED * powf(sinf( PI/2*fminf( 2*MUI_SCROLLPANE_BLEED, value - max )/MUI_SCROLLPANE_BLEED/2 ), 1);
	}
}