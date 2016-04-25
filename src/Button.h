/*
 *  Button.h
 *  
 *  yyyy, a button! 
 
 void onEvent(const void* pSender, ofTouchEventArgs& arg)
 {
 std::cout << "onEvent: " << arg << std::endl;
 }
 
 then do button.onPress += Delegate<YourClass,ofTouchEventArgs>( this, %YourClass::yourEventFunc ); 
 */

#ifndef MUI_BUTTON
#define MUI_BUTTON

#include "MuiCore.h"

namespace mui{
	class Button : public Container{
	public: 
		Button( std::string title_, float x_ = 0, float y_ = 0, float width_ = 200, float height_ = 20 );
		
		Label * label; 
		bool pressed;
		ofEvent<ofTouchEventArgs> onPress;
		
		virtual void init( std::string title );
		virtual void update();
		virtual void draw();
		virtual void drawBackground();
		virtual void fitWidthToLabel(int paddingLR = 5);
		virtual void fitWidthToLabel(int paddingL, int paddingR );
		virtual void layout();
		
		// simulate a click
		virtual void clickAndNotify(); 

		virtual void touchDown( ofTouchEventArgs &touch ); 
		virtual void touchMoved( ofTouchEventArgs &touch ); 
		virtual void touchMovedOutside( ofTouchEventArgs &touch ); 
		virtual void touchUp( ofTouchEventArgs &touch ); 
		virtual void touchUpOutside( ofTouchEventArgs &touch ); 
		virtual void touchDoubleTap( ofTouchEventArgs &touch ); 
		virtual void touchCanceled( ofTouchEventArgs &touch ); 
	};
};

#endif