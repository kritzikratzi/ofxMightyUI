/*
 *  SegmentedSelect.cpp
 *  iPhoneEmptyExample
 *
 *  Created by hansi on 29.01.11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 */
#include "SegmentedSelect.h"
#include "Label.h"

mui::SegmentedButton::SegmentedButton( std::string title_, float x_, float y_, float width_, float height_) :
Button( title_, x_, y_, width_, height_ ), selected(false) {
	initSegmentedButton();
};

//--------------------------------------------------------------
void mui::SegmentedButton::initSegmentedButton(){
	label->fontSize = 10; 
	label->commit(); 
}

//--------------------------------------------------------------
void mui::SegmentedButton::drawBackground(){
	ofSetColor( 255, 255, 255 );
	Helpers::beginImages();
	if( selected || pressed ){
		if( roundedLeft ) Helpers::drawImage( "segment_left_active", 0, 0, 5, 29 );
		Helpers::drawImage( "segment_center_active", roundedLeft?5:0, 0, width-((roundedLeft?5:0)+(roundedRight?5:0)), 29 );
		if( roundedRight ) Helpers::drawImage( "segment_right_active", width-5, 0, 5, 29 );
	}
	else{
		if( roundedLeft ) Helpers::drawImage( "segment_left", 0, 0, 5, 29 );
		Helpers::drawImage( "segment_center", roundedLeft?5:0, 0, width-((roundedLeft?5:0)+(roundedRight?5:0)), 29 );
		if( roundedRight ) Helpers::drawImage( "segment_right", width-5, 0, 5, 29 );
	}
	
	if( !roundedRight ){
		Helpers::drawImage( "segment_separator", width-1, 0, 1, 29 );
	}
	Helpers::endImages();
}




//--------------------------------------------------------------
//--------------------------------------------------------------


//--------------------------------------------------------------
mui::SegmentedSelect::	SegmentedSelect( float x_, float y_, float width_, float height_ )
: Container( x_, y_, width_, height_ ), selected(""){
}


//--------------------------------------------------------------
void mui::SegmentedSelect::addLabel( string text ){
	SegmentedButton * button = new SegmentedButton( text );
	button->width = button->label->boundingBox.width + 10; 
	ofAddListener(button->onPress, this, &SegmentedSelect::onButtonPress );

	add( button );
	commit(); 
}

//--------------------------------------------------------------
size_t mui::SegmentedSelect::getNumSegments(){
	return children.size();
}

//--------------------------------------------------------------
string mui::SegmentedSelect::getSegment( int num ){
	mui::SegmentedButton * button = (mui::SegmentedButton*)children[num];
	return button->label->text;
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
	ofNotifyEvent(onChange, selected); 
	commit(); 
}


//--------------------------------------------------------------
void mui::SegmentedSelect::touchDown( ofTouchEventArgs &touch ){
}


//--------------------------------------------------------------
void mui::SegmentedSelect::touchMoved( ofTouchEventArgs &touch ){
}


//--------------------------------------------------------------
void mui::SegmentedSelect::touchUp( ofTouchEventArgs &touch ){
}


//--------------------------------------------------------------
void mui::SegmentedSelect::touchDoubleTap( ofTouchEventArgs &touch ){
}