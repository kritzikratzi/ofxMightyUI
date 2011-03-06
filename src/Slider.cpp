/*
 *  Slider.cpp
 *  iPhoneEmptyExample
 *
 *  Created by hansi on 28.01.11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "MUI.h"


int mui::Slider::paddingLR = 11; 


//--------------------------------------------------------------
void mui::Slider::draw(){
	ofSetColor( 255, 255, 255 ); 
	float x = valueToScreen( value ); 
	float startY = (int)( height/2 - 9.0/2 );
	// draw bg... 
	// Helpers::getImage( "slider_inactive_left" )->draw( 0, startY ); // not needed! 
	ofEnableTextureEdgeHack(); 
	Helpers::getImage( "slider_inactive_center" )->draw( 5, startY, width - 10, 9 ); 
	ofDisableTextureEdgeHack(); 
	Helpers::getImage( "slider_inactive_right")->draw( width - 5, startY, 5, 9 ); 
	Helpers::getImage( "slider_active_left" )->draw( 0, startY, 5, 9 ); 
	if( x > 5 ) 
		Helpers::getImage( "slider_active_center" )->draw( 5, startY, x - 5, 9 ); 
	
	Helpers::getImage( "slider_handle" )->draw( x-13, startY-10, 26, 27 ); 
}



//--------------------------------------------------------------
void mui::Slider::touchDown( ofTouchEventArgs &touch ){
	cout << "touch down on slider!!!" << endl; 
	if( fabsf( height/2 - touch.y ) < 2*paddingLR ){
		touchMoved( touch ); 
	}
}


//--------------------------------------------------------------
void mui::Slider::touchMoved( ofTouchEventArgs &touch ){
	cout << "accepting touch moved, txy=" << touch.x << ", " << touch.y << endl; 
	value = fminf( max, fmaxf( min, screenToValue( touch.x ) ) ); 
}


//--------------------------------------------------------------
void mui::Slider::touchMovedOutside( ofTouchEventArgs &touch ){
	touchMoved( touch ); 
}


//--------------------------------------------------------------
void mui::Slider::touchUp( ofTouchEventArgs &touch ){
	cout << "accepting touch up, txy=" << touch.x << ", " << touch.y << endl; 
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