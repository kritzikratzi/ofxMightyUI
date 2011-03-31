/*
 *  InternalWindow.h
 *  iPhoneEmptyExample
 *  
 *  Use this as a starting point for a new element! 
 * 
 *  Created by hansi on 29.01.11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 *  TODO: Destructor for *label; 
 */

#ifndef MUI_INTERNAL_WINDOW
#define MUI_INTERNAL_WINDOW

namespace mui{
	class Label; 
	
	class InternalWindow : public Container{
	public: 
		InternalWindow( std::string title_ = "Window", float x_ = 0, float y_ = 0, float width_ = 300, float height_ = 300, float radius_ = 10 )
		: Container( x_, y_, width_, height_ ), radius(radius_), leftBarButton( NULL ), rightBarButton( NULL ), view( NULL ), padding(10) { init( title_ ); };
		
		Label * label; 
		BarButton * leftBarButton; 
		BarButton * rightBarButton; 
		Container * view; 
		int padding; 
		
		virtual void init( std::string title );
		virtual void update();
		virtual void draw();
		virtual void drawBackground();
		virtual void setLeftBarButton( BarButton * leftBarButton ); 
		virtual void setRightBarButton( BarButton * rightBarButton ); 
		
		virtual void touchDown( ofTouchEventArgs &touch );
		virtual void touchMoved( ofTouchEventArgs &touch );
		virtual void touchMovedOutside( ofTouchEventArgs &touch );
		virtual void touchUp( ofTouchEventArgs &touch );
		virtual void touchDoubleTap( ofTouchEventArgs &touch );
		
	private: 
		float radius;
		int dragX, dragY; 
	};
};

#endif