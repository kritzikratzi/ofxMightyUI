/*
 *  InternalWindow.cpp
 *  iPhoneEmptyExample
 *
 *  Created by hansi on 29.01.11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 */

#include "InternalWindow.h"


//--------------------------------------------------------------
void mui::InternalWindow::init( std::string title ){
	label = new Label( title, 0, 0, width, 44 ); 
	label->horizontalAlign = Center; 
	label->verticalAlign = Middle; 
	label->fg.r = label->fg.g = label->fg.b = 255; 
	label->fontSize = 16; 
	label->commit(); 
	
	add( label ); 
}

//--------------------------------------------------------------
void mui::InternalWindow::update(){
	label->width = width; 
}


//--------------------------------------------------------------
void mui::InternalWindow::draw(){	
}


//--------------------------------------------------------------
void mui::InternalWindow::drawBackground(){
	ofSetColor( 255, 255, 255, 200 ); 
	ofFill(); 
	Helpers::roundedRect( 0, 0, width, height, radius );
	ofNoFill(); 
	//Helpers::roundedRect( 1, 22, width-2, height-22, radius ); 
	
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