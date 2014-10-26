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
		SliderWithLabel( float x_ = 0, float y_ = 0, float width_ = 200, float height_ = 20, float min_ = 0, float max_ = 1, float value_ = 0, int decimalPlaces_ = 0 );
		
		int decimalPlaces;
		float defaultValue;
		Slider * slider;
		Label * label;

		virtual void update();
		virtual void draw();
		virtual void drawBackground();

	private: 
		float oldValue; 
	};
};

#endif