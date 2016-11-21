/*
 *  BarButton.h
 *	A special button that is only used inside the internal window.
 *  
 *  Deprecated, do not use, will be removed!
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
		
		virtual void drawBackground();
	private: 
	};
};

#endif