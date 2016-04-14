/*
 *  SegmentedSelect.h
 *  iPhoneEmptyExample
 *  
 *  Use this as a starting point for a new element! 
 * 
 *  Created by hansi on 29.01.11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef MUI_SEGMENTED_SELECTED
#define MUI_SEGMENTED_SELECTED

#include "MuiCore.h"
#include "Button.h"

namespace mui{
	template<typename T>
	class SegmentedButton : public Button{
	public: 
		SegmentedButton( std::string title_, T value, float x_ = 0, float y_ = 0, float width_ = 200, float height_ = 30 );
		
		bool selected;
		bool roundedLeft; 
		bool roundedRight;
		T value;
		
		
		virtual void initSegmentedButton(); 
		virtual void drawBackground();
	};
	
	template<typename T>
	class SegmentedSelect : public Container{
	public: 
		SegmentedSelect( float x_ = 0, float y_ = 0, float width_ = 200, float height_ = 20 );
		
		SegmentedButton<T> * selected;
		
		virtual void addSegment( string text, T value );
		virtual void commit();

		// iterator?
		virtual size_t getNumSegments();
		virtual SegmentedButton<T> * getSegment( int num );
		
		virtual void update();
		virtual void draw();
		virtual void drawBackground();
		
		virtual void touchDown( ofTouchEventArgs &touch );
		virtual void touchMoved( ofTouchEventArgs &touch ); 
		virtual void touchUp( ofTouchEventArgs &touch ); 
		virtual void touchDoubleTap( ofTouchEventArgs &touch ); 

		ofEvent<SegmentedButton<T>*> onChange;
		
	private: 
		virtual void onButtonPress( const void* sender, ofTouchEventArgs &args );
	};
};

#endif