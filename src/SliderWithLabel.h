/*
 *  SliderWithLabel.h
 *  iPhoneEmptyExample
 *  
 *  Use this as a starting point for a new element! 
 * 
 *  Created by hansi on 29.01.11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef MUI_SLIDER_WITH_LABEL
#define MUI_SLIDER_WITH_LABEL

namespace mui{
	class SliderWithLabel : public Container{
	public: 
		SliderWithLabel( float x_ = 0, float y_ = 0, float width_ = 200, float height_ = 20, float min_ = 0, float max_ = 1, float value_ = 0, int decimalPlaces_ = 0 ) 
		: Container( x_, y_, width_, height_ ), decimalPlaces(decimalPlaces_), oldValue(-9999) {
			slider = new Slider( 0, 0, width_ - 40, height_, min_, max_, value_ ); 
			label = new Label( "", width_ - 35, 0, 35, height_ ); 
			ofColor col; 
			col.r = col.g = col.b = 0; 
			label->fg = col; 
			label->horizontalAlign = Right; 
			defaultValue = value_;
			
			add( slider ); 
			add( label ); 
		};
		
		int decimalPlaces;
		float defaultValue;
		Slider * slider;
		Label * label;
		
		virtual void update();
		virtual void draw();
		virtual void drawBackground();
		
		virtual bool touchDown( ofTouchEventArgs &touch ); 
		virtual bool touchMoved( ofTouchEventArgs &touch ); 
		virtual bool touchUp( ofTouchEventArgs &touch ); 
		virtual bool touchDoubleTap( ofTouchEventArgs &touch ); 
		
	private: 
		float oldValue; 
	};
};

#endif