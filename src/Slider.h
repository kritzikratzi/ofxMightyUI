/*
 *  Slider.h
 *  iPhoneEmptyExample
 *
 *  Created by hansi on 28.01.11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef MUI_SLIDER
#define MUI_SLIDER

#include "MuiCore.h"

namespace mui{
	class Slider : public Container{
	public: 
		float min; 
		float max; 
		float value;
		
		Slider( float x_ = 0, float y_ = 0, float width_ = 200, float height_ = 20, float min_ = 0, float max_ = 1, float value_ = 0 ) 
			: Container( x_, y_, width_, height_ ), min(min_), max(max_), value(value_) {};  
		
		virtual void update(){}; 
		virtual void draw(); 
		
		virtual void touchDown( ofTouchEventArgs &touch ); 
		virtual void touchMoved( ofTouchEventArgs &touch ); 
		virtual void touchMovedOutside( ofTouchEventArgs &touch ); 
		virtual void touchUp( ofTouchEventArgs &touch ); 
		virtual void touchDoubleTap( ofTouchEventArgs &touch ); 
		
		
		virtual float screenToValue( float x ); 
		virtual float valueToScreen( float val ); 
		
		ofEvent<float> onChange;
        
		
	private: 
		static int paddingLR;
	}; 
}

#endif
