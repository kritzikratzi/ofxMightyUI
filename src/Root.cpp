/*
 *  Root.cpp
 *  iPhoneEmptyExample
 *
 *  Created by hansi on 28.01.11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "Root.h"


void mui::Root::handleDraw(){
	ofEnableAlphaBlending(); 
	
	Container::handleDraw(); 
	
	ofDisableAlphaBlending(); 
}



/*bool mui::Root::handleTouchDown( ofTouchEventArgs &touch ){
}

bool mui::Root::handleTouchMoved( ofTouchEventArgs &touch ){
}

bool mui::Root::handleTouchUp( ofTouchEventArgs &touch ){
}

bool mui::Root::handleTouchDoubleTap( ofTouchEventArgs &touch ){
}

*/