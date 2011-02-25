/*
 *  Root.cpp
 *  iPhoneEmptyExample
 *
 *  Created by hansi on 28.01.11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "MUI.h"

mui::Root * mui::Root::INSTANCE = NULL;

void mui::Root::init(){
	cout << "init root!" << endl; 
	#ifdef TARGET_OS_IPHONE
	cout << "target ios!" << endl; 
	NativeIOS::init(); 
	#endif
}


void mui::Root::handleDraw(){
	ofFill(); 
	ofSetLineWidth( 1 ); 
	ofSetColor( 255, 255, 255 ); 
	ofEnableAlphaBlending(); 
	if( Helpers::retinaMode ){
		ofPushMatrix();
		ofScale( 2, 2, 1 );
	}
	
	Container::handleDraw();
	
	if( Helpers::retinaMode ){
		ofPopMatrix(); 
	}
	
	ofDisableAlphaBlending(); 
}



bool mui::Root::handleTouchDown( ofTouchEventArgs &touch ){
	#ifdef TARGET_OS_IPHONE
	NativeIOS::hide(); 
	#endif
	
	// really? this creates a copy? 
	if( Helpers::retinaMode ){
		ofTouchEventArgs copyToMessWith = touch; 
		copyToMessWith.x /= 2; 
		copyToMessWith.y /= 2; 
		
		return Container::handleTouchDown( copyToMessWith ); 
	}
	else{
		return Container::handleTouchDown( touch ); 
	}
}

bool mui::Root::handleTouchMoved( ofTouchEventArgs &touch ){
	// really? this creates a copy? 
	if( Helpers::retinaMode ){
		ofTouchEventArgs copyToMessWith = touch; 
		copyToMessWith.x /= 2; 
		copyToMessWith.y /= 2; 
		
		return Container::handleTouchMoved( copyToMessWith ); 
	}
	else{
		return Container::handleTouchMoved( touch ); 
	}
}

bool mui::Root::handleTouchUp( ofTouchEventArgs &touch ){
	// really? this creates a copy? 
	if( Helpers::retinaMode ){
		ofTouchEventArgs copyToMessWith = touch; 
		copyToMessWith.x /= 2; 
		copyToMessWith.y /= 2; 
		
		return Container::handleTouchUp( copyToMessWith ); 
	}
	else{
		return Container::handleTouchUp( touch ); 
	}
}

bool mui::Root::handleTouchDoubleTap( ofTouchEventArgs &touch ){
	// really? this creates a copy? 
	if( Helpers::retinaMode ){
		ofTouchEventArgs copyToMessWith = touch; 
		copyToMessWith.x /= 2; 
		copyToMessWith.y /= 2; 
		
		return Container::handleTouchDoubleTap( copyToMessWith ); 
	}
	else{
		return Container::handleTouchDoubleTap( touch ); 
	}
}

void mui::Root::showTextField( TextField * tf ){
	#ifdef TARGET_OS_IPHONE
	NativeIOS::showTextField( tf ); 
	#endif
}


