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


#include "ToggleButton.h"
#include "Label.h"

mui::ToggleButton::ToggleButton( std::string title_, float x_, float y_, float width_, float height_ )
: Container( x_, y_, width_, height_ ), pressed(false), selected(false){
	init( title_ );
};

//--------------------------------------------------------------
void mui::ToggleButton::init( std::string title ){
	label = new Label( title, 0, 0, width, height );
	label->horizontalAlign = Center;
	label->verticalAlign = Middle;
	label->fg.r = label->fg.g = label->fg.b = 255;
	label->fontSize = mui::MuiConfig::fontSize;
	label->commit();
	add( label );
	opaque = true;
	name = "togglebutton-" + title;
	
	bg = ofColor(125,50);
	fg = ofColor( 200 );
	
	selectedBg = ofColor(255);
	selectedFg = ofColor(0);
}

//--------------------------------------------------------------
void mui::ToggleButton::update(){
	label->width = width;
	label->height = height;
	label->fg = selected?selectedFg:fg;
}


//--------------------------------------------------------------
void mui::ToggleButton::draw(){
}


//--------------------------------------------------------------
void mui::ToggleButton::drawBackground(){
	ofColor bg = selected?selectedBg:this->bg;
	if( bg.a > 0 ){
		ofFill();
		ofSetColor( bg );
		ofDrawRectangle( 0, 0, width, height );
	}
}

void mui::ToggleButton::fitWidthToLabel( int paddingLR ){
	fitWidthToLabel(paddingLR, paddingLR );
}

void mui::ToggleButton::fitWidthToLabel( int paddingL, int paddingR ){
	label->ellipsisMode = false; 
	label->commit();
	ofRectangle bounds = label->box(0, paddingR, 0, paddingL );
	width = bounds.width;
}

//--------------------------------------------------------------
void mui::ToggleButton::layout(){
	label->width = width;
	label->height = height;
	label->fg = selected?selectedFg:fg;
}

//--------------------------------------------------------------
void mui::ToggleButton::touchDown( ofTouchEventArgs &touch ){
	pressed = true;
}


//--------------------------------------------------------------
void mui::ToggleButton::touchMoved( ofTouchEventArgs &touch ){
	pressed = true;
}


//--------------------------------------------------------------
void mui::ToggleButton::touchMovedOutside( ofTouchEventArgs &touch ){
	pressed = false;
}


//--------------------------------------------------------------
void mui::ToggleButton::touchUp( ofTouchEventArgs &touch ){
	selected = !selected;
	pressed = false;
	ofNotifyEvent(onPress, touch, this );
}


//--------------------------------------------------------------
void mui::ToggleButton::touchUpOutside( ofTouchEventArgs &touch ){
	pressed = false;
}


//--------------------------------------------------------------
void mui::ToggleButton::touchDoubleTap( ofTouchEventArgs &touch ){
}


//--------------------------------------------------------------
void mui::ToggleButton::touchCanceled( ofTouchEventArgs &touch ){
	pressed = false;
}


void mui::ToggleButton::clickAndNotify(){
	ofTouchEventArgs args;
	args.x = width/2; // click in the center
	args.y = height/2;
	selected ^= true;
	ofNotifyEvent(onPress, args, this);
}

void mui::ToggleButton::clickAndNotify(bool state){
	ofTouchEventArgs args;
	args.x = width/2; // click in the center
	args.y = height/2;
	selected = state;
	ofNotifyEvent(onPress, args, this);
}

