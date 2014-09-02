/*
 *  BarButton.h
 *  iPhoneEmptyExample
 *  
 *  Use this as a starting point for a new element! 
 * 
 *  Created by hansi on 29.01.11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef MUI_BAR_BUTTON
#define MUI_BAR_BUTTON

namespace mui{
	class BarButton : public Button{
	public: 
		BarButton( std::string title_, float x_ = 0, float y_ = 0, float width_ = 200, float height_ = 30 ) : 
		Button( title_, x_, y_, width_, height_ ){ initBarButton(); }; 
		
		virtual void initBarButton(); 
		virtual void drawBackground();
	private: 
	};
};

#endif