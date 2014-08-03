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

void mui::Root::handleUpdate(){
	tweener.step( ofGetSystemTime() );
	#ifdef TARGET_OS_IPHONE
	NativeIOS::update(); 
	#endif
	Container::handleUpdate();
    
    handleRemovals();
}

//--------------------------------------------------------------
void mui::Root::handleDraw(){
	ofFill(); 
	ofSetLineWidth( 1 ); 
	ofSetColor( 255, 255, 255 ); 
	ofEnableAlphaBlending();
//TMP
//	if( Helpers::retinaMode ){
//		ofPushMatrix();
//		ofScale( 2, 2, 1 );
//	}
	
	Container::handleDraw();
	
//	if( Helpers::retinaMode ){
//		ofPopMatrix();
//	}
	
	ofDisableAlphaBlending(); 
    
    handleRemovals(); 
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
        copy = Helpers::translateTouch( touch, this, respondingContainer[touch.id] );
        copy = Helpers::translateTouch( touch, this, respondingContainer[touch.id] );
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
		Container *c = respondingContainer[touch.id];
		respondingContainer[touch.id]->touchUpOutside( copy );
		c->singleTouchId = -1;
	}
	
    respondingContainer[touch.id] = NULL; 
    
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
// TMP
// DISABLE RETINA STUFF, ALREADY HANDLED BY OF
	if( Helpers::retinaMode && false ){
		copy.x = touch.x/2; 
		copy.y = touch.y/2;
        copy.xspeed = touch.xspeed/2;
        copy.yspeed = touch.yspeed/2; 
	}
	else{
		copy.x = touch.x; 
		copy.y = touch.y; 
        copy.xspeed = touch.xspeed; 
        copy.yspeed = touch.yspeed; 
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

void mui::Root::hideTextFields(){
    #ifdef TARGET_OS_IPHONE
    NativeIOS::hide(); 
    #endif
}


//--------------------------------------------------------------
bool mui::Root::becomeResponder( Container * c, ofTouchEventArgs &touch ){
	// the trivial case ...
	if( c != NULL && c == respondingContainer[touch.id] )
		return true;
	
	// notify previous owner,
	// cancel if it doesn't allow transfering focus
	if( respondingContainer[touch.id] != NULL ){
        if( respondingContainer[touch.id]->focusTransferable == false )
            return false; 
        
		respondingContainer[touch.id]->touchCanceled( touch ); 
		respondingContainer[touch.id]->singleTouchId = -1; 
	}
	
	// alright, install new owner
	respondingContainer[touch.id] = c;
	if( respondingContainer[touch.id] != NULL ){
		respondingContainer[touch.id]->singleTouchId = touch.id;
	}

	
	return true; 
}


//--------------------------------------------------------------
void mui::Root::safeRemove( Container * c ){
    safeRemoveList.push_back( c ); 
}

//--------------------------------------------------------------
void mui::Root::safeRemoveAndDelete( mui::Container *c ){
    safeRemoveAndDeleteList.push_back( c ); 
}

//--------------------------------------------------------------
void mui::Root::removeFromResponders( Container * c ){
	for( int i = 0; i < OF_MAX_TOUCHES; i++ ){
		if( respondingContainer[i] == c ){
			respondingContainer[i] = NULL;
		}
	}
}



//--------------------------------------------------------------
void mui::Root::prepareAnimation( int milliseconds, int type, int direction ){
	param = tween::TweenerParam( milliseconds, type, direction );
}


//--------------------------------------------------------------
void mui::Root::animate( float &variable, float targetValue ){
    param.addProperty( &variable, targetValue ); 
}


//--------------------------------------------------------------
void mui::Root::commitAnimation(){
    tweener.addTween( param );
}

void mui::Root::handleRemovals(){
    vector<Container*>::iterator it = safeRemoveList.begin(); 
    while( it != safeRemoveList.end() ){
        (*it)->remove();
        ++it; 
    }
    
    it = safeRemoveAndDeleteList.begin(); 
    while( it != safeRemoveAndDeleteList.end() ){
        (*it)->remove();
        delete (*it); 
        ++it; 
    }
    
    safeRemoveList.clear(); 
    safeRemoveAndDeleteList.clear(); 
}

