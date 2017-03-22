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


mui::Slider::Slider( float x_, float y_, float width_, float height_, float min_, float max_, float value_ )
: Container( x_, y_, width_, height_ ), min(min_), max(max_), value(value_),valueMapper(make_shared<MapperLinear>()) {
};

//--------------------------------------------------------------
void mui::Slider::draw(){
	ofSetColor( 255, 255, 255 ); 
	float x = ofMap(valueMapper->toScreen( this, value ), 0, 1, paddingLR, width-paddingLR,true);
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
void mui::Slider::setValueAndNotify(float val){
	value = ofClamp(val,min,max);
	onChange.notify(value);
}


//--------------------------------------------------------------
void mui::Slider::touchDown( ofTouchEventArgs &touch ){
	touchMoved( touch ); 
}


//--------------------------------------------------------------
void mui::Slider::touchMoved( ofTouchEventArgs &touch ){
	value = ofClamp( valueMapper->toValue( this, ofMap(touch.x,paddingLR,width-paddingLR,0,1,true) ), min, max );
	onChange.notify(value);
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
float mui::Slider::MapperLinear::toValue( mui::Slider * s, float x ){
	return ofMap(x,0,1,s->min,s->max);
}


//--------------------------------------------------------------
float mui::Slider::MapperLinear::toScreen( mui::Slider * s, float val ){
	return ofMap(val,s->min,s->max,0,1);
}

//--------------------------------------------------------------
mui::Slider::MapperLog::MapperLog(float strength):strength(strength){
	
}

//--------------------------------------------------------------
float mui::Slider::MapperLog::toValue( mui::Slider * s, float x ){
	return (expf(x*logf(strength + 1)) - 1)/strength*(s->max - s->min) + s->min;
}


//--------------------------------------------------------------
float mui::Slider::MapperLog::toScreen( mui::Slider * s, float val ){
	return logf((val - s->min)/(s->max - s->min)*strength + 1)/logf(strength + 1);
}

