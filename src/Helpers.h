/*
 *  Images.h
 *  iPhoneEmptyExample
 *
 *  Created by hansi on 29.01.11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
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
		static void clearCaches();
		static string muiPath( string path );

		static void beginImages();
		static void endImages();
		static void drawImage( const string name, float x, float y );
		static void drawImage( const string name, float x, float y, float w, float h );

		static bool loadFont( const string customFont );
		static ofxFontStashStyle getStyle( const int size = mui::MuiConfig::fontSize );
		static ofxFontStashStyle getStyle( const string customFont, const int size = mui::MuiConfig::fontSize );
		static ofxFontStash2 & getFontStash();
		static void drawString( const string s, const float x = 0, const float y = 0, const ofColor color = ofColor(255), const int size = mui::MuiConfig::fontSize );

		static void roundedRect(float x, float y, float w, float h, float r);
		static void quadraticBezierVertex( float cpx, float cpy, float x, float y, float prevX, float prevY); 

		static void orientedScissor( float x, float y, float w, float h );
		static void pushScissor( Container * c = NULL, float x = 0, float y = 0, float w = -9999, float h = -9999  );
		static void pushScissor( Container * c, ofRectangle rect );
        static void popScissor();
		
        static bool stringLtString( string a, string b );
		
		static ofRectangle alignBox( Container * container, float width, float height, HorizontalAlign horizontalAlign, VerticalAlign verticalAlign );
		static bool inside( Container * container, float x, float y ); 
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
