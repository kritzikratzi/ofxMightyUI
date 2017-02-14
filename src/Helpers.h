/*
 *  Helpers.h
 *  ofxMightyUI
 *
 *  Created by hansi on 29.01.11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 *  A collection of happy little utilities.
 */

#ifndef MUI_HELPERS
#define MUI_HELPERS

#include "MuiCore.h"
#include "ofxFontStash2.h"

namespace mui{
	class TextureAtlas;
	class Container;
	
	class Helpers{
	public:
		// delete all texture and font caches
		static void clearCaches();
		
		// look for files in different places (ie inside the data folder and inside the application itself)
		static string muiPath( string path );

		// draw multiple images from the built-in texture atlas.
		// wrap all drawImage calls inside beginImages/endImages!
		static void beginImages();
		static void endImages();
		static void drawImage( const string name, float x, float y );
		static void drawImage( const string name, float x, float y, float w, float h );

		// load a font (e.g. "myfont.ttf"), funnily, the filename is also the font name you'll use later
		// to access the font again.
		static bool loadFont( const string customFont );
		
		// methods to draw fonts, and quick access into fontstash2
		static ofxFontStashStyle getStyle( const int size = mui::MuiConfig::fontSize );
		static ofxFontStashStyle getStyle( const string customFont, const int size = mui::MuiConfig::fontSize );
		static ofxFontStash2 & getFontStash();
		static void drawString( const string s, const float x = 0, const float y = 0, const ofColor color = ofColor(255), const int size = mui::MuiConfig::fontSize );

		// draw a round rect. this is so old, i haven't touched this code in years.
		static void roundedRect(float x, float y, float w, float h, float r);
		static void quadraticBezierVertex( float cpx, float cpy, float x, float y, float prevX, float prevY); 

		// do a glScissors that takes hardware rotation into account.
		// don't use directly, unless you know what you're doing.
		static void orientedScissor( float x, float y, float w, float h );
		
		// push/pop an element onto the scissor stack.
		// easiest is to just do: void draw(){ Helpers::pushScissor(this); doMessyDrawing(); popScissor(this); }
		static void pushScissor( Container * c = NULL, float x = 0, float y = 0, float w = -9999, float h = -9999  );
		static void pushScissor( Container * c, ofRectangle rect );
        static void popScissor();
		
		// Align a box sized width x height inside a container.
		static ofRectangle alignBox(Container * container, float width, float height, HorizontalAlign horizontalAlign, VerticalAlign verticalAlign);
		static ofRectangle alignBox(Container * container, const Inset & inset, float width, float height, HorizontalAlign horizontalAlign, VerticalAlign verticalAlign);

		// check if a point is inside a container.
		// move to container?
		static bool inside( Container * container, float x, float y );
		
		// convert between coordinate systems.
		// you will get odd results if the two containers don't have a common parent.
        static ofTouchEventArgs translateTouch( ofTouchEventArgs &touch, Container * src, Container * dest );
		static ofPoint translateCoords( float x, float y, Container * src, Container * dest );
		static ofPoint translateCoords( ofPoint pt, Container * src, Container * dest );
		
	private:
		static std::map<std::string, ofTexture*> textures;
		static std::map<std::string, ofImage*> images;
        static std::stack<ofRectangle> scissorStack;
		static mui::TextureAtlas atlas;
		static ofxFontStash2 fontStash;
		
		friend class MuiFont;
	};
}


#endif
