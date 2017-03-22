/*
 *  Slider.h
 *  ofxMightyUI
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
		
		class Mapper;
		float min;
		float max; 
		float value;
		
		Slider( float x_ = 0, float y_ = 0, float width_ = 200, float height_ = 20, float min_ = 0, float max_ = 1, float value_ = 0 ); 
		
		virtual void draw() override;
		
		void setValueAndNotify(float val);
		
		virtual void touchDown( ofTouchEventArgs &touch ) override;
		virtual void touchMoved( ofTouchEventArgs &touch ) override;
		virtual void touchMovedOutside( ofTouchEventArgs &touch ) override;
		virtual void touchUp( ofTouchEventArgs &touch ) override;
		virtual void touchDoubleTap( ofTouchEventArgs &touch ) override;
		
		
		shared_ptr<Mapper> valueMapper;
		EventHandler<float> onChange{this};
		

		
	private: 
		static int paddingLR;
	
		
	public:
		class Mapper{
		public: 
			virtual float toValue( Slider * slider, float x ) = 0;
			virtual float toScreen( Slider * slider, float val ) = 0;
		};
		
		class MapperLinear : public Mapper{
		public:
			float toValue( Slider * slider, float x );
			float toScreen( Slider * slider, float val );
		};
		
		class MapperLog : public Mapper{
		public:
			MapperLog(float strength);
			float strength{10}; 
			float toValue( Slider * slider, float x );
			float toScreen( Slider * slider, float val );
		};
	};
}

#endif
