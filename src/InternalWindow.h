/*
 *  InternalWindow.h
 *
 *  It's an internal window, but it's deprecated :(
 *
 */

#ifndef MUI_INTERNAL_WINDOW
#define MUI_INTERNAL_WINDOW

#include "MuiCore.h"

namespace mui{
	class InternalWindow : public Container{
	public: 
		InternalWindow( std::string title = "Window", float x = 0, float y = 0, float width = 300, float height = 300, float radius = 10 );
		
		Label * label; 
		BarButton * leftBarButton; 
		BarButton * rightBarButton; 
		Container * view; 
		int padding; 
		
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
		float dragX, dragY;
	};
};

#endif