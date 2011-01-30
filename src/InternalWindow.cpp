/*
 *  InternalWindow.cpp
 *  iPhoneEmptyExample
 *
 *  Created by hansi on 29.01.11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 */

#include "InternalWindow.h"

//--------------------------------------------------------------
void mui::InternalWindow::update(){
}


//--------------------------------------------------------------
void mui::InternalWindow::draw(){	
	Helpers::drawStringWithShadow( "Adjustments", 100, 30, 16, 255, 255, 255 ); 
}


//--------------------------------------------------------------
void mui::InternalWindow::drawBackground(){
	ofSetColor( 255, 255, 255, 200 ); 
	ofFill(); 
	Helpers::roundedRect( 0, 0, width, height, radius ); 
	
	ofSetColor( 255, 255, 255 ); 
	Helpers::getImage( "titlebar_left" )->draw( 0, 0, 5, 44 ); 
	Helpers::getImage( "titlebar_center" )->draw( 5, 0, width - 9, 44 ); 
	Helpers::getImage( "titlebar_right" )->draw( width-5, 0, 5, 44 ); 
}


//--------------------------------------------------------------
bool mui::InternalWindow::touchDown( ofTouchEventArgs &touch ){
	if( touch.y <= 44 ){
		dragX = touch.x; 
		dragY = touch.y; 
		return true; 
	}
	
	return false; 
}


//--------------------------------------------------------------
bool mui::InternalWindow::touchMoved( ofTouchEventArgs &touch ){
	int deltaX = touch.x - dragX; 
	int deltaY = touch.y - dragY; 
	x += deltaX; 
	y += deltaY; 
	//dragX += deltaX; 
	//dragY += deltaY; 
	return true; 
}


//--------------------------------------------------------------
bool mui::InternalWindow::touchUp( ofTouchEventArgs &touch ){
	return false; 
}


//--------------------------------------------------------------
bool mui::InternalWindow::touchDoubleTap( ofTouchEventArgs &touch ){
	return false; 
}