/*
 *  Text.h
 *  iPhoneEmptyExample
 *  
 *  Use this as a starting point for a new element! 
 * 
 *  Created by hansi on 29.01.11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 *  Allrighty, label does some magic to properly support the retina display. 
 *  It's not complicated, but i'm too tired to write documentation, sorry :(
 */

#ifndef MUI_LABEL
#define MUI_LABEL

namespace mui{
	class Label : public Container{
	public: 
		Label( std::string text_ = "Label", float x_ = 0, float y_ = 0, float width_ = 200, float height_ = 20 ) : 
		Container( x_, y_, width_, height_ ), 
		text( text_), fontSize( 12 ), horizontalAlign(Left), verticalAlign(Middle)
		{ ignoreEvents = true; commit(); };  
		
		
		HorizontalAlign horizontalAlign; 
		VerticalAlign verticalAlign; 
		std::string text; 
		int fontSize; 
		ofRectangle boundingBox; 
		int renderX; 
		int renderY; 
		
		virtual void update();
		virtual void draw();
		virtual void drawBackground();
		
		virtual void commit(); // call this after you made changes to any variables (changing x/y is okay without doing a commit() ). doing text-maths too insane to do on every frame! 
		
	private: 
	};
};

#endif