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
	class SegmentedSelect;

	class SegmentedButton : public Button{
	public: 
		SegmentedButton( std::string title_, float x_ = 0, float y_ = 0, float width_ = 200, float height_ = 30 );
		
		bool selected;
		bool roundedLeft; 
		bool roundedRight; 
		
		virtual void initSegmentedButton(); 
		virtual void drawBackground();
	};
	
	class SegmentedSelect : public Container{
	public: 
		SegmentedSelect( float x_ = 0, float y_ = 0, float width_ = 200, float height_ = 20 );
		
		string selected;
		
		virtual void addLabel( string text );
		virtual void commit();

		// iterator?
		virtual size_t getNumSegments();
		virtual string getSegment( int num );
		
		virtual void update();
		virtual void draw();
		virtual void drawBackground();
		
		virtual void touchDown( ofTouchEventArgs &touch );
		virtual void touchMoved( ofTouchEventArgs &touch ); 
		virtual void touchUp( ofTouchEventArgs &touch ); 
		virtual void touchDoubleTap( ofTouchEventArgs &touch ); 

		ofEvent<string> onChange;
		
	private: 
		virtual void onButtonPress( const void* sender, ofTouchEventArgs &args );
	};
};

#endif