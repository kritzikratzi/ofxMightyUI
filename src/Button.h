/*
 *  Button.h
 *  iPhoneEmptyExample
 *  
 *  Use this as a starting point for a new element! 
 * 
 *  Created by hansi on 29.01.11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef MUI_BUTTON
#define MUI_BUTTON

#include "poco/BasicEvent.h"

namespace mui{
	class Label; 
	
	class Button : public Container{
	public: 
		Button( std::string title_, float x_ = 0, float y_ = 0, float width_ = 200, float height_ = 20 ) 
		: Container( x_, y_, width_, height_ ), pressed(false) { init( title_ ); };
		
		Label * label; 
		bool pressed; 
		Poco::BasicEvent<ofTouchEventArgs> onPress;
		
		virtual void init( std::string title );
		virtual void update();
		virtual void draw();
		virtual void drawBackground();
		
		virtual bool touchDown( ofTouchEventArgs &touch ); 
		virtual bool touchMoved( ofTouchEventArgs &touch ); 
		virtual bool touchUp( ofTouchEventArgs &touch ); 
		virtual bool touchDoubleTap( ofTouchEventArgs &touch ); 
		
	
	};
};

#endif