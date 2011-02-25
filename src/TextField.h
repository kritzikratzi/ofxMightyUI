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
		: Label( text_, x_, y_, width_, height_ ){};
		
		virtual void update();
		virtual void draw();
		virtual void drawBackground();
		
		virtual bool touchDown( ofTouchEventArgs &touch ); 
		virtual bool touchMoved( ofTouchEventArgs &touch ); 
		virtual bool touchUp( ofTouchEventArgs &touch ); 
		virtual bool touchDoubleTap( ofTouchEventArgs &touch ); 
		
	private: 
	};
};

#endif