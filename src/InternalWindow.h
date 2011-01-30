/*
 *  InternalWindow.h
 *  iPhoneEmptyExample
 *  
 *  Use this as a starting point for a new element! 
 * 
 *  Created by hansi on 29.01.11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 * 
 */

#ifndef MUI_INTERNAL_WINDOW
#define MUI_INTERNAL_WINDOW

#include "MUI.h"

namespace mui{
	class InternalWindow : public Container{
	public: 
		InternalWindow( float x_ = 0, float y_ = 0, float width_ = 300, float height_ = 300, float radius_ = 10 ) 
		: Container( x_, y_, width_, height_ ), radius(radius_) {};  
		
		virtual void update();
		virtual void draw();
		virtual void drawBackground();
		
		virtual bool touchDown( ofTouchEventArgs &touch );
		virtual bool touchMoved( ofTouchEventArgs &touch );
		virtual bool touchUp( ofTouchEventArgs &touch );
		virtual bool touchDoubleTap( ofTouchEventArgs &touch );
		
		float radius;
		int dragX, dragY; 
	private: 
	};
};

#endif