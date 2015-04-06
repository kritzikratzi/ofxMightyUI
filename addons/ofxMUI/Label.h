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
		Container( x_, y_, width_, height_ ), ellipsisMode(false),
		text( text_), fontSize( MUI_FONT_SIZE ), horizontalAlign(Left), verticalAlign(Middle)
		{ ignoreEvents = true; commit(); };  
		
		
		HorizontalAlign horizontalAlign; 
		VerticalAlign verticalAlign; 
		std::string text; 
		int fontSize;
		string fontName; // subject to change!
		ofRectangle boundingBox;
		int renderX; 
		int renderY; 
		//ofxFBOTexture fbo;
		bool ellipsisMode;
        
		virtual void update();
		virtual void draw();
		virtual void render();
		virtual void drawBackground();
		
		virtual void commit(); // call this after you made changes to any variables (changing x/y is okay without doing a commit() ). doing text-maths too insane to do on every frame!
		
		virtual ofRectangle box( float tblr = 0 ){ return box( tblr, tblr, tblr, tblr ); }
		virtual ofRectangle box( float tb, float lr ){ return box( tb, lr, tb, lr ); }
		virtual ofRectangle box( float t, float lr, float b ){ return box( t, lr, b, lr ); }
		virtual ofRectangle box( float t, float r, float b, float l );

	private: 
        std::string displayText; 
	};
};

#endif