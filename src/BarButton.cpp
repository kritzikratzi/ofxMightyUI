/*
 *  BarButton.cpp
 *  iPhoneEmptyExample
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
		initBarButton();
};

//--------------------------------------------------------------
void mui::BarButton::initBarButton(){
	label->fontSize = 10; 
	label->commit(); 
}


//--------------------------------------------------------------
void mui::BarButton::drawBackground(){
	ofSetColor( 255, 255, 255 );
	Helpers::beginImages();
	if( pressed ){
		Helpers::drawImage( "barbutton_left_active", 0, 0, 5, 30 );
		Helpers::drawImage( "barbutton_center_active", 5, 0, width-10, 30 );
		Helpers::drawImage( "barbutton_right_active", width-5, 0, 5, 30 );
	}
	else{
		Helpers::drawImage( "barbutton_left", 0, 0, 5, 30 );
		Helpers::drawImage( "barbutton_center", 5, 0, width-10, 30 );
		Helpers::drawImage( "barbutton_right", width-5, 0, 5, 30 );
	}
	Helpers::endImages();
}
