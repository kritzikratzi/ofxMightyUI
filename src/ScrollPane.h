/*
 *  ScrollPane.h
 *  iPhoneEmptyExample
 *  
 *  Use this as a starting point for a new element! 
 * 
 *  Created by hansi on 29.01.11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef MUI_SCROLL_PANE
#define MUI_SCROLL_PANE

namespace mui{
	class ScrollPane : public Container{
	public: 
		ScrollPane( float x_ = 0, float y_ = 0, float width_ = 200, float height_ = 20 ) 
			: Container( x_, y_, width_, height_ ), 
			canScrollX(true), canScrollY(true), scrollX(0), scrollY(0), 
			maxScrollX(0), maxScrollY(0), minScrollX(0), minScrollY(0), 
			currentScrollX(0), currentScrollY(0), 
			pressed(false), 
			view( NULL )
			{ init(); };
		
		float scrollX, scrollY; 
		float minScrollX, minScrollY; 
		float maxScrollX, maxScrollY; 
		bool canScrollX, canScrollY; 
		
		Container * view;
		
		virtual void init(); 
		
		virtual void commit(); 
		virtual void update();
		virtual void draw();
		virtual void drawBackground();
		
		virtual void handleDraw();
		
		virtual void touchDown( ofTouchEventArgs &touch ); 
		virtual void touchMoved( ofTouchEventArgs &touch ); 
		virtual void touchMovedOutside( ofTouchEventArgs &touch );
		virtual void touchUp( ofTouchEventArgs &touch ); 
		virtual void touchUpOutside( ofTouchEventArgs &touch ); 
		virtual void touchDoubleTap( ofTouchEventArgs &touch ); 

		virtual Container * handleTouchDown( ofTouchEventArgs &touch );
		virtual Container * handleTouchMoved( ofTouchEventArgs &touch );
		virtual Container * handleTouchUp( ofTouchEventArgs &touch );
		
	private: 
		float currentScrollX, currentScrollY; 
		bool pressed; 
		float pressedX, pressedY; 
		
		bool watchingTouch[OF_MAX_TOUCHES]; 
		ofPoint touchStart[OF_MAX_TOUCHES]; 
	};
};

#endif