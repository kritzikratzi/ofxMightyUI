/*
 *  Root.cpp
 *  iPhoneEmptyExample
 *
 *  Created by hansi on 28.01.11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "MUI.h"


// TODO: the handleXX functions might return null, even if touchMovedOutside and touchUpOutside 
//       delegated to containers. this shouldn't be the case. 


mui::Root * mui::Root::INSTANCE = NULL;


//--------------------------------------------------------------
void mui::Root::init(){
	#ifdef TARGET_OS_IPHONE
	NativeIOS::init(); 
	#endif
}


//--------------------------------------------------------------
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



//--------------------------------------------------------------
mui::Container * mui::Root::handleTouchDown( ofTouchEventArgs &touch ){
	#ifdef TARGET_OS_IPHONE
	NativeIOS::hide(); 
	#endif
	
	ofTouchEventArgs copy = touch; 
	fixTouchPosition( touch, copy, NULL ); 
	
	//return ( respondingContainer[touch.id] = Container::handleTouchDown( copy ) ); 
	respondingContainer[touch.id] = Container::handleTouchDown( copy ); 
	return respondingContainer[touch.id]; 
}


//--------------------------------------------------------------
mui::Container * mui::Root::handleTouchMoved( ofTouchEventArgs &touch ){
	ofTouchEventArgs copy = touch; 
	fixTouchPosition( touch, copy, NULL ); 
	Container * touched = Container::handleTouchMoved( copy ); 
	
	if( touched != respondingContainer[touch.id] && respondingContainer[touch.id] != NULL ){
		fixTouchPosition( touch, copy, respondingContainer[touch.id] ); 
		respondingContainer[touch.id]->touchMovedOutside( copy ); 
	}
	
	return touched;
}


//--------------------------------------------------------------
mui::Container * mui::Root::handleTouchUp( ofTouchEventArgs &touch ){
	ofTouchEventArgs copy = touch; 
	fixTouchPosition( touch, copy, NULL ); 
	Container * touched = Container::handleTouchUp( copy ); 
	
	if( touched != respondingContainer[touch.id] && respondingContainer[touch.id] != NULL ){
		fixTouchPosition( touch, copy, respondingContainer[touch.id] ); 
		respondingContainer[touch.id]->touchUpOutside( copy ); 
		respondingContainer[touch.id]->singleTouchId = -1; 
	}
	
	return touched; 	
}


//--------------------------------------------------------------
mui::Container * mui::Root::handleTouchDoubleTap( ofTouchEventArgs &touch ){
	ofTouchEventArgs copy = touch; 
	fixTouchPosition( touch, copy, NULL ); 

	return Container::handleTouchDoubleTap( copy ); 
}


//--------------------------------------------------------------
void mui::Root::fixTouchPosition( ofTouchEventArgs &touch, ofTouchEventArgs &copy, Container * container ){
	if( Helpers::retinaMode ){
		copy.x = touch.x/2; 
		copy.y = touch.y/2;
	}
	else{
		copy.x = touch.x; 
		copy.y = touch.y; 
	}
	
	if( container != NULL ){
		ofPoint pos = container->getGlobalPosition();
		copy.x -= pos.x;
		copy.y -= pos.y;
	}
}




//--------------------------------------------------------------
void mui::Root::showTextField( TextField * tf ){
	#ifdef TARGET_OS_IPHONE
	NativeIOS::showTextField( tf ); 
	#endif
}


//--------------------------------------------------------------
bool mui::Root::becomeResponder( Container * c, ofTouchEventArgs &touch ){
	if( respondingContainer[touch.id] != NULL ){
		respondingContainer[touch.id]->touchCanceled( touch ); 
		respondingContainer[touch.id]->singleTouchId = -1; 
	}
	
	respondingContainer[touch.id] = c; 
	respondingContainer[touch.id]->singleTouchId = touch.id; 
	
	
	return true; 
}


