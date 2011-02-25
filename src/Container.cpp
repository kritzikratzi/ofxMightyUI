/*
 *  Node.cpp
 *  iPhoneEmptyExample
 *
 *  Created by hansi on 28.01.11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "MUI.h"

void mui::Container::add( Container * c ){
	children.push_back( c ); 
	c->parent = this; 
}

void mui::Container::drawBackground(){
}


void mui::Container::handleDraw(){
	if( !visible ) return; 
	
	ofPushMatrix(); 
	ofTranslate( x, y ); 
	
	if( !opaque ) drawBackground(); 
	draw(); 
	
	std::vector<Container*>::iterator it = children.begin();
	while( it != children.end() ) {
		(*it)->handleDraw(); 
		++it;
	}

	ofPopMatrix(); 
}


void mui::Container::handleUpdate(){
	update(); 
	
	std::vector<Container*>::iterator it = children.begin();
	while( it != children.end() ) {
		(*it)->handleUpdate(); 
		++it;
	}
}

bool mui::Container::handleTouchDown( ofTouchEventArgs &touch ){
	if( !visible ) return false; 

	touch.x -= x;
	touch.y -= y;
	
	//cout << "do kids care?" << touch.x << ", " << touch.y << "--" << endl; 
	if( touch.x >= 0 && touch.x <= width && touch.y >= 0 && touch.y <= height ){
		//cout << "maybe" << endl; 
		std::vector<Container*>::iterator it = children.begin();
		while( it != children.end() ) {
			if( (*it)->handleTouchDown( touch ) ){
				//cout << "one kid did care!" << endl; 
				return true; 
			}
			
			++it;
		}
		//cout << "kids don't care!" << endl; 

		if( touchDown( touch ) ){
			startedInside[touch.id] = true;
			return true; 
		}
	}
	
	touch.x += x;
	touch.y += y;
	return false; 
}

bool mui::Container::handleTouchMoved( ofTouchEventArgs &touch ){
	if( !visible ) return false; 

	touch.x -= x;
	touch.y -= y;
	
	if( startedInside[touch.id] ){
		if( touchMoved( touch ) ){
			return true; 
		}
	}
	else{
		std::vector<Container*>::iterator it = children.begin();
		while( it != children.end() ) {
			if( (*it)->handleTouchMoved( touch ) ){
				return true; 
			}
			
			++it;
		}
	}
	
	touch.x += x; 
	touch.y += y; 
	
	return false; 
}

bool mui::Container::handleTouchDoubleTap( ofTouchEventArgs &touch ){
	if( !visible ) return false; 

	touch.x -= x;
	touch.y -= y;
	
	if( startedInside[touch.id] ){
		if( touchDoubleTap( touch ) ){
			return true; 
		}
	}
	else{
		std::vector<Container*>::iterator it = children.begin();
		while( it != children.end() ) {
			if( (*it)->handleTouchDoubleTap( touch ) ){
				return true; 
			}
			
			++it;
		}
	}
	
	touch.x += x; 
	touch.y += y; 
	
	return false; 
}

bool mui::Container::handleTouchUp( ofTouchEventArgs &touch ){
	if( !visible ) return false; 
	
	touch.x -= x;
	touch.y -= y;
	
	if( startedInside[touch.id] ){
		startedInside[touch.id] = false;
		
		if( touchUp( touch ) ){
			return true; 
		}
	}
	else{
		std::vector<Container*>::iterator it = children.begin();
		while( it != children.end() ) {
			if( (*it)->handleTouchUp( touch ) ){
				return true; 
			}
			
			++it;
		}
	}
	
	touch.x += x; 
	touch.y += y; 
	
	return false; 
}

ofPoint mui::Container::getGlobalPosition(){
	ofPoint result; 
	
	float x = 0; 
	float y = 0; 
	
	Container * parent = this; 
	while( parent != NULL ){
		x += parent->x; 
		y += parent->y; 
		parent = parent->parent; 
	}
	
	result.x = x; 
	result.y = y; 
	
	return result; 
}

