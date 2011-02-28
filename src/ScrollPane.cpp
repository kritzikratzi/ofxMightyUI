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


void mui::ScrollPane::init(){
	view = new Container( 0, 0, width, height ); 
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
	
	cout << "commit scrollpane: " << minScrollX << ", " << maxScrollX << "," << maxX << endl; 
	view->width = fmaxf( width, maxScrollX ); 
	view->height = fmax( height, maxScrollY ); 
	cout << "view size = " << maxScrollX << ", " << maxScrollY << endl; 
}

//--------------------------------------------------------------
void mui::ScrollPane::update(){
	if( pressed ){
		// TODO: this needs to be improved! 
		scrollX = fminf( maxScrollX, fmaxf( scrollX, minScrollX ) ); 
		scrollY = fminf( maxScrollY+50, fmaxf( scrollY, minScrollY-50 ) ); 
	}
	else{
		scrollX = fminf( maxScrollX, fmaxf( scrollX, minScrollX ) ); 
		scrollY = fminf( maxScrollY, fmaxf( scrollY, minScrollY ) ); 
	}
	
	currentScrollX += ( scrollX - currentScrollX ) / 3; 
	currentScrollY += ( scrollY - currentScrollY ) / 3; 
	
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
bool mui::ScrollPane::touchDown( ofTouchEventArgs &touch ){
	if( !pressed ){
		pressedX = touch.x; 
		pressedY = touch.y; 
		pressed = true; 
		return true; 
	}
	
	return false;
}


//--------------------------------------------------------------
bool mui::ScrollPane::touchMoved( ofTouchEventArgs &touch ){
	cout << "touch moved!" << scrollY << endl; 
	if( pressed ){
		scrollX -= ( touch.x - pressedX ); 
		scrollY -= ( touch.y - pressedY ); 
		pressedX = touch.x; 
		pressedY = touch.y; 
		
		return true; 
	}
	
	return false; 
}


//--------------------------------------------------------------
bool mui::ScrollPane::touchUp( ofTouchEventArgs &touch ){
	if( pressed ){
		pressed = false; 
		return true; 
	}
	
	return false; 
}


//--------------------------------------------------------------
bool mui::ScrollPane::touchDoubleTap( ofTouchEventArgs &touch ){
	return false; 
}