/*
 *  Button.cpp
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
void mui::Button::init( std::string title ){
	label = new Label( title, 0, 0, width, height );
	label->horizontalAlign = Center; 
	label->verticalAlign = Middle; 
	label->fg.r = label->fg.g = label->fg.b = 255; 
	label->fontSize = 12; 
	label->commit(); 
	bg.r = bg.g = bg.b = 125; 
	add( label ); 
}

//--------------------------------------------------------------
void mui::Button::update(){
	label->width = width; 
	label->height = height; 
}


//--------------------------------------------------------------
void mui::Button::draw(){
}


//--------------------------------------------------------------
void mui::Button::drawBackground(){
	ofFill(); 
	ofSetColor( bg.r, bg.g, bg.b ); 
	Helpers::roundedRect( 0, 0, width, height, 5 ); 
	if( pressed ){
		ofSetColor( bg.r/2, bg.g/2, bg.b/2 ); 
	}
	
	ofNoFill(); 
	Helpers::roundedRect( 0, 0, width, height, 5 ); 
}


//--------------------------------------------------------------
bool mui::Button::touchDown( ofTouchEventArgs &touch ){
	cout << "touch down on button!" << label->text << endl; 
	if( pressed ) return false; 
	
	pressed = true; 
	return true; 
}


//--------------------------------------------------------------
bool mui::Button::touchMoved( ofTouchEventArgs &touch ){
	return false; 
}


//--------------------------------------------------------------
bool mui::Button::touchUp( ofTouchEventArgs &touch ){
	if( pressed && touch.x >= 0 && touch.x <= width && touch.y >= 0 && touch.y <= height ){
		onPress( this, touch );
	}

	pressed = false; 
	return true; 
}


//--------------------------------------------------------------
bool mui::Button::touchDoubleTap( ofTouchEventArgs &touch ){
	return true; 
}