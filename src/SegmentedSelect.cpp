/*
 *  SegmentedSelect.cpp
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

#include "MUI.h"


//--------------------------------------------------------------
void mui::SegmentedButton::initSegmentedButton(){
	label->fontSize = 10; 
	label->commit(); 
}

//--------------------------------------------------------------
void mui::SegmentedButton::drawBackground(){
	ofSetColor( 255, 255, 255 ); 
	if( selected || pressed ){
		if( roundedLeft ) Helpers::getImage( "segment_left_active" )->draw( 0, 0, 5, 29 ); 
		Helpers::getImage( "segment_center_active" )->draw( roundedLeft?5:0, 0, width-((roundedLeft?5:0)+(roundedRight?5:0)), 29 ); 
		if( roundedRight ) Helpers::getImage( "segment_right_active" )->draw( width-5, 0, 5, 29 ); 
	}
	else{
		if( roundedLeft ) Helpers::getImage( "segment_left" )->draw( 0, 0, 5, 29 ); 
		Helpers::getImage( "segment_center" )->draw( roundedLeft?5:0, 0, width-((roundedLeft?5:0)+(roundedRight?5:0)), 29 ); 
		if( roundedRight ) Helpers::getImage( "segment_right" )->draw( width-5, 0, 5, 29 ); 
	}
	
	if( !roundedRight ){
		Helpers::getImage( "segment_separator" )->draw( width-1, 0, 1, 29 ); 
	}
}




//--------------------------------------------------------------
//--------------------------------------------------------------




//--------------------------------------------------------------
void mui::SegmentedSelect::addLabel( string text ){
	SegmentedButton * button = new SegmentedButton( text );
	button->width = button->label->boundingBox.width + 10; 
	button->onPress += Poco::Delegate<SegmentedSelect, ofTouchEventArgs>( this, &SegmentedSelect::onButtonPress );

	add( button );
	commit(); 
}


//--------------------------------------------------------------
void mui::SegmentedSelect::commit(){
	vector<Container*>::iterator it = children.begin(); 
	int x = 0; 
	bool first = true; 
	SegmentedButton * last = NULL; 
	
	while( it != children.end() ){
		SegmentedButton * button = (SegmentedButton*) *it; 
		button->x = x; 
		x += button->width; 
		button->selected = button->label->text == selected;
		button->roundedLeft = first; 
		button->roundedRight = false;
		
		first = false; 
		last = button; 
		
		++it;
	}
	
	if( last != NULL ){
		last->roundedRight = true; 
	}
}


//--------------------------------------------------------------
void mui::SegmentedSelect::update(){
}


//--------------------------------------------------------------
void mui::SegmentedSelect::draw(){
}


//--------------------------------------------------------------
void mui::SegmentedSelect::drawBackground(){
}


//--------------------------------------------------------------
void mui::SegmentedSelect::onButtonPress( const void* sender, ofTouchEventArgs &args ){
	SegmentedButton * button = (SegmentedButton*) sender; 
	selected = button->label->text; 
	commit(); 
}


//--------------------------------------------------------------
bool mui::SegmentedSelect::touchDown( ofTouchEventArgs &touch ){
	return false; 
}


//--------------------------------------------------------------
bool mui::SegmentedSelect::touchMoved( ofTouchEventArgs &touch ){
	return false; 
}


//--------------------------------------------------------------
bool mui::SegmentedSelect::touchUp( ofTouchEventArgs &touch ){
	return false; 
}


//--------------------------------------------------------------
bool mui::SegmentedSelect::touchDoubleTap( ofTouchEventArgs &touch ){
	return false; 
}