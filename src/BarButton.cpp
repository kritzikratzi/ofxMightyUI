/*
 *  BarButton.cpp
 *  ofxMightyUI
 *
 *  Created by hansi on 29.01.11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 */

#include "BarButton.h"
#include "Label.h"

using namespace mui;

//--------------------------------------------------------------
mui::BarButton::BarButton( std::string title_, float x_, float y_, float width_, float height_ )
: Button( title_, x_, y_, width_, height_ ){
	label->fontSize = 10; 
	label->commit(); 
}


//--------------------------------------------------------------
void mui::BarButton::drawBackground(){
	ofSetColor( 255, 255, 255 );
	Helpers::beginImages();
	if( pressed ){
		Helpers::drawImage( "barbutton_left_active", 7, 7, 5, 30 );
		Helpers::drawImage( "barbutton_center_active", 7+5, 7, width-24, 30 );
		Helpers::drawImage( "barbutton_right_active", width-5-7, 7, 5, 30 );
	}
	else{
		Helpers::drawImage( "barbutton_left", 7, 7, 5, 30 );
		Helpers::drawImage( "barbutton_center", 7+5, 7, width-24, 30 );
		Helpers::drawImage( "barbutton_right", width-5-7, 7, 5, 30 );
	}
	Helpers::endImages();
}
