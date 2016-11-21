/*
 *  SliderWithLabel.cpp
 *  ofxMightyUI
 *
 *  Created by hansi on 29.01.11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 */

#include "SliderWithLabel.h"
#include "Label.h"

mui::SliderWithLabel::SliderWithLabel( float x_, float y_, float width_, float height_, float min_, float max_, float value_, int decimalPlaces_ ) : Container( x_, y_, width_, height_ ), decimalPlaces(decimalPlaces_), oldValue(-9999) {
	slider = new Slider( 0, 0, width_ - 40, height_, min_, max_, value_ );
	float longestVal = -min_>max_?min_:max_;
	label = new Label( ofToString(longestVal,decimalPlaces), width_ - 35, 0, 35, height_ );
	label->sizeToFitWidth();
	label->width += 5;
	if( longestVal < 0 ) label->width += 20;
	ofColor col;
	col.r = col.g = col.b = 0;
	label->fg = col;
	label->horizontalAlign = Right;
	defaultValue = value_;
	label->ignoreEvents = false;
	
	add( slider );
	add( label );
}

//--------------------------------------------------------------
void mui::SliderWithLabel::update(){
	if( oldValue != slider->value ){
		label->text = ofToString( slider->value, decimalPlaces );
		label->commit();
		oldValue = slider->value;
		// do we really need to do this on every frame?
		layout();
	}
}

//--------------------------------------------------------------
void mui::SliderWithLabel::layout(){
	slider->width = width - 5 - label->width;
	slider->height = height; 
	label->x = width - label->width;
}

mui::Container * mui::SliderWithLabel::handleTouchDown( ofTouchEventArgs &args ){
	mui::Container * res = mui::Container::handleTouchDown(args);
	if( res == label ){
		slider->value = defaultValue;
		ofNotifyEvent(slider->onChange, defaultValue, slider );
	}
	return res; 
}
