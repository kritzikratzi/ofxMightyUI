/*
 *  Button.h
 *  
 *  Hurray, it's a button. 
 *  Default behavior: 
 *    - button has gray background with white text
 *    - during clicking the background becomes semi transparent
 *    - a left click will trigger onPress()
 *    - a right click, or (osx) ctrl+click will trigger onContextMenu()
 *    - releasing the mouse outside the button will no trigger any event
 * 
 */

#ifndef MUI_BUTTON
#define MUI_BUTTON

#include "MuiCore.h"

namespace mui{
	class Button : public Container{
	public: 
		Button( std::string title, float x = 0, float y = 0, float width = 200, float height = 20 );
		
		Label * label; 
		bool pressed;
		ofEvent<ofTouchEventArgs> onPress; //todo: replace with mui::EventHandler
		ofEvent<ofTouchEventArgs> onContextMenu; //todo: replace with mui::EventHandler
		
		virtual void update();
		virtual void draw();
		virtual void drawBackground();
		virtual void fitWidthToLabel(int paddingLR = 5); // deprecated
		virtual void fitWidthToLabel(int paddingL, int paddingR ); // deprecated 
		virtual void sizeToFitWidth(int paddingLR = 5);
		virtual void sizeToFitWidth(int paddingL, int paddingR );
		virtual void layout();
		virtual bool keyPressed(ofKeyEventArgs &args);
		
		// simulate a click
		virtual void clickAndNotify(); 

		virtual void touchDown( ofTouchEventArgs &touch ); 
		virtual void touchMoved( ofTouchEventArgs &touch ); 
		virtual void touchMovedOutside( ofTouchEventArgs &touch ); 
		virtual void touchUp( ofTouchEventArgs &touch ); 
		virtual void touchUpOutside( ofTouchEventArgs &touch ); 
		virtual void touchDoubleTap( ofTouchEventArgs &touch ); 
		virtual void touchCanceled( ofTouchEventArgs &touch ); 
	private:
		bool isRightClick{false};
	};
};

#endif