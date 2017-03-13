/*
 *  Button.cpp
 *  ofxMightyUI
 *
 *  Created by hansi on 29.01.11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */


#include "ToggleButton.h"
#include "Label.h"

mui::ToggleButton::ToggleButton( std::string title, float x, float y, float width, float height )
: Container( x, y, width, height ), pressed(false), selected(false), checkbox(false),checkboxAlign(mui::Left){
	label = new Label( title, 0, 0, width, height );
	label->horizontalAlign = Center;
	label->verticalAlign = Middle;
	label->fg = ofColor(255);
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
}


//--------------------------------------------------------------
void mui::ToggleButton::draw(){
	if( checkbox ){
		ofSetColor(fg);
		float x = checkboxAlign == Right?label->width:0;
		float h = label->boundingBox.height;
		if( selected ){
			float baseX = x+2+h*0.1;
			ofDrawRectangle(baseX,(height-h)/2,h,h);
			ofSetColor(fg.getInverted());
			ofDrawLine(baseX+2,(height-h)/2+2,baseX+h-2,(height+h)/2-2);
			ofDrawLine(baseX+2,(height+h)/2-2,baseX+h-2,(height-h)/2+2);
		}
		else{
			ofNoFill();
			ofDrawRectangle(x+2+h*0.1,(height-h)/2,h,h);
			ofFill();
		}
		ofSetColor(255);
	}
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
	sizeToFitWidth(paddingLR,paddingLR);
}

void mui::ToggleButton::fitWidthToLabel( int paddingL, int paddingR ){
	sizeToFitWidth(paddingL,paddingR);
}

void mui::ToggleButton::sizeToFitWidth( int paddingLR ){
	sizeToFitWidth(paddingLR, paddingLR );
}

void mui::ToggleButton::sizeToFitWidth( int paddingL, int paddingR ){
	label->ellipsisMode = false;
	label->commit();
	ofRectangle bounds = label->box(0, paddingR, 0, paddingL );
	width = bounds.width + (checkbox?(4+bounds.height*1.2):0);
}

//--------------------------------------------------------------
void mui::ToggleButton::layout(){
	if( checkbox ){
		float checkboxWidth = (4+label->boundingBox.height*1.2);
		switch(checkboxAlign){
			case Left:
			case Center:
				label->x = checkboxWidth;
				label->width = width - checkboxWidth;
				break;
			case Right:
				label->x = 0;
				label->width = width - checkboxWidth;
				break;
		}
	}
	else{
		label->x = 0;
		label->width = width; 
	}
	label->height = height;
	label->fg = selected?selectedFg:fg;
}

//--------------------------------------------------------------
void mui::ToggleButton::touchDown( ofTouchEventArgs &touch ){
	isRightClick = ofGetMousePressed(OF_MOUSE_BUTTON_3);
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
	pressed = false;
	if(isRightClick){
		ofNotifyEvent( onContextMenu, touch, this);
	}
	else{
		selected = !selected;
		ofNotifyEvent(onPress, touch, this );
		ofNotifyEvent(onChange, selected, this );
	}
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
	ofNotifyEvent(onChange, selected, this);
}

void mui::ToggleButton::clickAndNotify(bool state){
	ofTouchEventArgs args;
	args.x = width/2; // click in the center
	args.y = height/2;
	selected = state;
	ofNotifyEvent(onPress, args, this);
	ofNotifyEvent(onChange, selected, this);
}

