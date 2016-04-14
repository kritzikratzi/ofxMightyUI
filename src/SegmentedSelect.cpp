/*
 *  SegmentedSelect.cpp
 *  iPhoneEmptyExample
 *
 *  Created by hansi on 29.01.11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 */
#include "SegmentedSelect.h"
#include "Label.h"

template<typename T>
mui::SegmentedButton<T>::SegmentedButton( std::string title_, T value, float x_, float y_, float width_, float height_) :
Button( title_, x_, y_, width_, height_ ), selected(false), value(value) {
	initSegmentedButton();
};

//--------------------------------------------------------------
template<typename T>
void mui::SegmentedButton<T>::initSegmentedButton(){
	label->fontSize -= 2;
	label->commit(); 
}

//--------------------------------------------------------------
template<typename T>
void mui::SegmentedButton<T>::drawBackground(){
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
template<typename T>
mui::SegmentedSelect<T>::SegmentedSelect( float x_, float y_, float width_, float height_ )
: Container( x_, y_, width_, height_ ), selected(NULL){
}


//--------------------------------------------------------------
template<typename T>
void mui::SegmentedSelect<T>::addSegment( string text, T value ){
	SegmentedButton<T> * button = new SegmentedButton<T>( text, value );
	button->width = button->label->boundingBox.width + 10; 
	ofAddListener(button->onPress, this, &SegmentedSelect::onButtonPress );

	add( button );
	commit(); 
}

//--------------------------------------------------------------
template<typename T>
size_t mui::SegmentedSelect<T>::getNumSegments(){
	return children.size();
}

//--------------------------------------------------------------
template<typename T>
mui::SegmentedButton<T> * mui::SegmentedSelect<T>::getSegment( int num ){
	mui::SegmentedButton<T> * button = (mui::SegmentedButton<T>*)children[num];
	return button;
}

//--------------------------------------------------------------
template<typename T>
void mui::SegmentedSelect<T>::commit(){
	vector<Container*>::iterator it = children.begin(); 
	int x = 0; 
	bool first = true; 
	SegmentedButton<T> * last = NULL;
	
	while( it != children.end() ){
		SegmentedButton<T> * button = (SegmentedButton<T>*) *it;
		button->x = x; 
		x += button->width; 
		button->selected = button == selected;
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
template<typename T>
void mui::SegmentedSelect<T>::update(){
}


//--------------------------------------------------------------
template<typename T>
void mui::SegmentedSelect<T>::draw(){
}


//--------------------------------------------------------------
template<typename T>
void mui::SegmentedSelect<T>::drawBackground(){
}


//--------------------------------------------------------------
template<typename T>
void mui::SegmentedSelect<T>::onButtonPress( const void* sender, ofTouchEventArgs &args ){
	SegmentedButton<T> * button = (SegmentedButton<T>*) sender;
	selected = button;
	ofNotifyEvent(onChange, selected, this);
	commit(); 
}


//--------------------------------------------------------------
template<typename T>
void mui::SegmentedSelect<T>::touchDown( ofTouchEventArgs &touch ){
}


//--------------------------------------------------------------
template<typename T>
void mui::SegmentedSelect<T>::touchMoved( ofTouchEventArgs &touch ){
}


//--------------------------------------------------------------
template<typename T>
void mui::SegmentedSelect<T>::touchUp( ofTouchEventArgs &touch ){
}


//--------------------------------------------------------------
template<typename T>
void mui::SegmentedSelect<T>::touchDoubleTap( ofTouchEventArgs &touch ){
}


template class mui::SegmentedSelect<int>;
template class mui::SegmentedSelect<float>;
template class mui::SegmentedSelect<double>;
template class mui::SegmentedSelect<string>;
