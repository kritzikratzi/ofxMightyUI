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


#include "Button.h"
#include "MuiCore.h"
#include "Label.h"

using namespace mui; 

//--------------------------------------------------------------
mui::Button::Button( std::string title_, float x_, float y_, float width_, float height_ )
: Container( x_, y_, width_, height_ ), pressed(false) { init( title_ ); };

//--------------------------------------------------------------
void mui::Button::init( std::string title ){
	label = new Label( title, 0, 0, width, height );
	label->horizontalAlign = Center; 
	label->verticalAlign = Middle; 
	label->fg.r = label->fg.g = label->fg.b = 255; 
	label->fontSize = mui::MuiConfig::fontSize;
	label->commit(); 
	bg = ofColor( 128, 50 );
	add( label ); 
	opaque = true; 
	name = "button-" + title; 
}

//--------------------------------------------------------------
void mui::Button::update(){
	label->width = width; 
	label->height = height; 
}


//--------------------------------------------------------------
void mui::Button::draw(){
	if( hasKeyboardFocus() ){
		ofSetColor(fg);
		ofNoFill();
		ofDrawRectangle(0, 0, width, height);
		ofFill();
		ofSetColor(255);
	}
}


//--------------------------------------------------------------
void mui::Button::drawBackground(){
	if( pressed ){
		ofSetColor( bg.r/2, bg.g/2, bg.b/2, bg.a );
	}
	else{
		ofSetColor( bg.r, bg.g, bg.b, bg.a );
	}
	ofDrawRectangle( 0, 0, width, height );
	ofSetColor(255);
}

void mui::Button::fitWidthToLabel( int paddingLR ){
	fitWidthToLabel(paddingLR, paddingLR );
}

void mui::Button::fitWidthToLabel( int paddingL, int paddingR ){
	label->ellipsisMode = false; 
	label->commit();
	ofRectangle bounds = label->box(0, paddingR, 0, paddingL );
	width = bounds.width;
}

void mui::Button::layout(){
	label->width = width;
	label->height = height;
}

//--------------------------------------------------------------
void mui::Button::touchDown( ofTouchEventArgs &touch ){
	pressed = true;
}


//--------------------------------------------------------------
void mui::Button::touchMoved( ofTouchEventArgs &touch ){
	pressed = true;
}


//--------------------------------------------------------------
void mui::Button::touchMovedOutside( ofTouchEventArgs &touch ){
	pressed = false; 
}


//--------------------------------------------------------------
void mui::Button::touchUp( ofTouchEventArgs &touch ){
	pressed = false;
	ofNotifyEvent( onPress, touch, this );
}


//--------------------------------------------------------------
void mui::Button::touchUpOutside( ofTouchEventArgs &touch ){
	pressed = false; 
}


//--------------------------------------------------------------
void mui::Button::touchDoubleTap( ofTouchEventArgs &touch ){
}


//--------------------------------------------------------------
void mui::Button::touchCanceled( ofTouchEventArgs &touch ){
	pressed = false; 
}

void mui::Button::clickAndNotify(){
	ofTouchEventArgs args;
	args.x = width/2;
	args.y = height/2;
	ofNotifyEvent(onPress, args, this );
}

bool mui::Button::keyPressed(ofKeyEventArgs &args){
	if( args.key == ' ' || args.key == OF_KEY_RETURN ){
		clickAndNotify();
		return true;
	}
	else{
		return false;
	}
}
