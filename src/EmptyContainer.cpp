/*
 *  EmptyContainer.cpp
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

//--------------------------------------------------------------
void mui::EmptyContainer::update(){
}


//--------------------------------------------------------------
void mui::EmptyContainer::draw(){
	ofSetColor( 255, 0, 0 ); 
	ofRect( 0, 0, width, height ); 
}


//--------------------------------------------------------------
void mui::EmptyContainer::drawBackground(){
}


//--------------------------------------------------------------
void mui::EmptyContainer::touchDown( ofTouchEventArgs &touch ){
}


//--------------------------------------------------------------
void mui::EmptyContainer::touchMoved( ofTouchEventArgs &touch ){
}


//--------------------------------------------------------------
void mui::EmptyContainer::touchUp( ofTouchEventArgs &touch ){
}


//--------------------------------------------------------------
void mui::EmptyContainer::touchDoubleTap( ofTouchEventArgs &touch ){
}