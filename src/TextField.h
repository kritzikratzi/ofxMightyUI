/*
 *  TextField.h
 *  iPhoneEmptyExample
 *  
 *  Use this as a starting point for a new element! 
 * 
 *  Created by hansi on 29.01.11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef MUI_TEXTFIELD
#define MUI_TEXTFIELD

namespace mui{
	class TextField : public Label{
	public: 
		TextField( std::string text_ = "Label", float x_ = 0, float y_ = 0, float width_ = 200, float height_ = 20 )
		: Label( text_, x_, y_, width_, height_ ){ ignoreEvents = false; };
		
		virtual void update();
		virtual void draw();
		virtual void drawBackground();
		
		virtual void touchDown( ofTouchEventArgs &touch ); 
		virtual void touchMoved( ofTouchEventArgs &touch ); 
		virtual void touchUp( ofTouchEventArgs &touch ); 
		virtual void touchDoubleTap( ofTouchEventArgs &touch ); 
		
	private: 
	};
};

#endif