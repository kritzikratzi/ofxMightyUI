/*
 *  Slider.cpp
 *  ofxMightyUI
 *
 *  Created by hansi on 28.01.11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "Slider.h"


int mui::Slider::paddingLR = 11; 


//--------------------------------------------------------------
void mui::Slider::draw(){
	ofSetColor( 255, 255, 255 ); 
	float x = valueToScreen( value ); 
	float startY = (int)( height/2 - 9.0/2 );
	// draw bg... 
	// Helpers::getImage( "slider_inactive_left", 0, startY ); // not needed!
	Helpers::beginImages();
	Helpers::drawImage( "slider_inactive_center", 4, startY, width - 9, 9 );
	Helpers::drawImage( "slider_inactive_right", width - 5, startY, 5, 9 );
	Helpers::drawImage( "slider_active_left", 0, startY, 5, 9 );
	if( x > 5 ) 
		Helpers::drawImage( "slider_active_center", 5, startY, x - 5, 9 );
	
	Helpers::drawImage( "slider_handle", x-13, startY-10, 26, 27 );
	Helpers::endImages();
}



//--------------------------------------------------------------
void mui::Slider::touchDown( ofTouchEventArgs &touch ){
	touchMoved( touch ); 
}


//--------------------------------------------------------------
void mui::Slider::touchMoved( ofTouchEventArgs &touch ){
	value = fminf( max, fmaxf( min, screenToValue( touch.x ) ) );
	ofNotifyEvent(onChange, value, this);
}


//--------------------------------------------------------------
void mui::Slider::touchMovedOutside( ofTouchEventArgs &touch ){
	touchMoved( touch ); 
}


//--------------------------------------------------------------
void mui::Slider::touchUp( ofTouchEventArgs &touch ){
}


//--------------------------------------------------------------
void mui::Slider::touchDoubleTap( ofTouchEventArgs &touch ){
}

//--------------------------------------------------------------
float mui::Slider::screenToValue( float x ){
	return min + ( max - min ) * ( x - paddingLR )/( width - 2*paddingLR ); 
}


//--------------------------------------------------------------
float mui::Slider::valueToScreen( float val ){
	return paddingLR + ( width - 2*paddingLR ) * ( val - min ) / ( max - min ); 
}