/*
 *  Button.cpp
 */


#include "Button.h"
#include "ofxMightyUI.h"
#include "Label.h"

using namespace mui; 

//--------------------------------------------------------------
mui::Button::Button( std::string title, float x, float y, float width, float height )
: Container( x, y, width, height ), pressed(false) {
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

mui::Button::~Button(){
	delete label;
}

//--------------------------------------------------------------
void mui::Button::update(){
//	label->width = width; 
//	label->height = height; 
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
	sizeToFitWidth(paddingLR,paddingLR);
}

void mui::Button::fitWidthToLabel( int paddingL, int paddingR ){
	sizeToFitWidth(paddingL,paddingR);
}


void mui::Button::sizeToFitWidth( int paddingLR ){
	fitWidthToLabel(paddingLR, paddingLR );
}

void mui::Button::sizeToFitWidth( int paddingL, int paddingR ){
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
	isRightClick = muiIsContextClick();
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
	if(!pressed) return;
	pressed = false;
	if(isRightClick){
		ofNotifyEvent( onContextMenu, touch, this);
	}
	else{
		ofNotifyEvent( onPress, touch, this );
	}
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
