/*
 *  Label.cpp
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

#include "Label.h"

//--------------------------------------------------------------
void mui::Label::update(){
}


//--------------------------------------------------------------
void mui::Label::draw(){
	// old, maybe new again one day. 
	//Helpers::drawStringWithShadow( text, renderX, renderY, fontSize, fg.r, fg.g, fg.b );
	
	// magic trick #1: 
	int w, h; 
	if( Helpers::retinaMode ) w = 2*width, h = 2*height; 
	else w = width, h=height; 
	
	if( horizontalAlign == Left ) renderX = -boundingBox.x;
	else if( horizontalAlign == Center ) renderX = -boundingBox.x + w/2 - boundingBox.width/2;
	else if( horizontalAlign == Right ) renderX = -boundingBox.x + w - boundingBox.width; 
	if( verticalAlign == Top ) renderY = - boundingBox.y; 
	else if( verticalAlign == Middle ) renderY = -boundingBox.y + h/2 - boundingBox.height/2; 
	else if( verticalAlign == Bottom ) renderY = -boundingBox.y + h - boundingBox.height; 
	
	
	ofTrueTypeFont * font; 
	
	if( Helpers::retinaMode ){
		ofPushMatrix(); 
		ofScale( 0.5, 0.5, 1 );
		font = Helpers::getFont( fontSize*2 );
	}
	else{
		font = Helpers::getFont( fontSize ); 
	}
	
	ofSetColor( fg.r, fg.g, fg.b ); 
	font->drawString( text, renderX, renderY ); 
	
	if( Helpers::retinaMode ){
		ofPopMatrix(); 
	}
}


//--------------------------------------------------------------
void mui::Label::drawBackground(){
}


//--------------------------------------------------------------
bool mui::Label::touchDown( ofTouchEventArgs &touch ){
	return false; 
}


//--------------------------------------------------------------
bool mui::Label::touchMoved( ofTouchEventArgs &touch ){
	return false; 
}


//--------------------------------------------------------------
bool mui::Label::touchUp( ofTouchEventArgs &touch ){
	return false; 
}


//--------------------------------------------------------------
bool mui::Label::touchDoubleTap( ofTouchEventArgs &touch ){
	return false; 
}

//--------------------------------------------------------------
void mui::Label::commit(){
	// magic trick #2
	ofTrueTypeFont * font = Helpers::getFont( Helpers::retinaMode?(fontSize*2):fontSize );
	cout << "font=" << font << endl; 
	boundingBox = font->getStringBoundingBox( text, 0, 0 );
}