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


namespace mui{
	class Slider : public Container{
	public: 
		float min; 
		float max; 
		float value;
		ofTouchEventArgs touch; 
		
		Slider( float x_ = 0, float y_ = 0, float width_ = 200, float height_ = 20, float min_ = 0, float max_ = 1, float value_ = 0 ) 
			: Container( x_, y_, width_, height_ ), min(min_), max(max_), value(value_) {};  
		
		virtual void update(){}; 
		virtual void draw(); 
		
		virtual bool touchDown( ofTouchEventArgs &touch ); 
		virtual bool touchMoved( ofTouchEventArgs &touch ); 
		virtual bool touchUp( ofTouchEventArgs &touch ); 
		virtual bool touchDoubleTap( ofTouchEventArgs &touch ); 
		
		
		virtual float screenToValue( float x ); 
		virtual float valueToScreen( float val ); 
		
	private: 
		static int paddingLR;
		
	}; 
}

#endif
