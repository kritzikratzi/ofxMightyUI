/*
 *  SliderWithLabel.cpp
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
void mui::SliderWithLabel::update(){
	slider->width = width - 40; 
	slider->height = height; 
	label->x = width - 35; 
	if( oldValue != slider->value ){
		label->text = ofToString( slider->value, decimalPlaces ); 
		label->commit(); 
		oldValue = slider->value; 
	}
}


//--------------------------------------------------------------
void mui::SliderWithLabel::draw(){
}


//--------------------------------------------------------------
void mui::SliderWithLabel::drawBackground(){
}