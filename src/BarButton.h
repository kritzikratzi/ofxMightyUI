/*
 *  BarButton.h
 *  iPhoneEmptyExample
 *  
 *  TODO:move inside internal window
 * 
 *  Created by hansi on 29.01.11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef MUI_BAR_BUTTON
#define MUI_BAR_BUTTON

#include "MuiCore.h"
#include "Button.h"

namespace mui{
	class BarButton : public Button{
	public: 
		BarButton( std::string title_, float x_ = 0, float y_ = 0, float width_ = 200, float height_ = 30 );
		
		virtual void initBarButton(); 
		virtual void drawBackground();
	private: 
	};
};

#endif