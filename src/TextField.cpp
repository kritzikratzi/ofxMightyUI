/*
 *  TextField.cpp
 *  iPhoneEmptyExample
 *
 *  Created by hansi on 29.01.11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.

 */

#include "TextField.h"
#include "Label.h"
#include "Root.h"

//--------------------------------------------------------------
mui::TextField::TextField( std::string text_, float x_, float y_, float width_, float height_ )
: Label( text_, x_, y_, width_, height_ ){
	ignoreEvents = false;
};

//--------------------------------------------------------------
void mui::TextField::setText( string value ){
    if( text.compare( value ) != 0 ){
        text = value; 
        commit(); 
		ofNotifyEvent(onChange, text, this );
    }
}


//--------------------------------------------------------------
void mui::TextField::update(){
	Label::update(); 
}


//--------------------------------------------------------------
void mui::TextField::draw(){
	Label::draw(); 
}


//--------------------------------------------------------------
void mui::TextField::drawBackground(){
	Label::drawBackground(); 
}


//--------------------------------------------------------------
void mui::TextField::touchDown( ofTouchEventArgs &touch ){
}


//--------------------------------------------------------------
void mui::TextField::touchMoved( ofTouchEventArgs &touch ){
}


//--------------------------------------------------------------
void mui::TextField::touchUp( ofTouchEventArgs &touch ){
	Root::INSTANCE->showTextField( this );
	ofNotifyEvent(onPress, touch, this );
}


//--------------------------------------------------------------
void mui::TextField::touchDoubleTap( ofTouchEventArgs &touch ){
}