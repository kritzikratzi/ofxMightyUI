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

#include <map>
#include <string>
#include "MUI.h"

namespace mui{
	class Container; 
	
	class Helpers{
	public: 
		static void clearCaches();
		static string muiPath( string path );
		static ofImage * getImage( string what );
		static MUI_FONT_TYPE * getFont( int size ); // get a font for a specific pixel size
		static MUI_FONT_TYPE * getFont( string customFont, int size ); // get a font for a specific pixel size
		static void drawString( string s, float x = 0, float y = 0, int size = MUI_FONT_SIZE ); // draw retina friendly string
		static void roundedRect(float x, float y, float w, float h, float r); 
		static void quadraticBezierVertex( float cpx, float cpy, float x, float y, float prevX, float prevY); 
		static void drawStringWithShadow( std::string s, int x, int y, int fontSize, int r, int g, int b ); 
        static void orientedScissor( float x, float y, float w, float h );
        static void pushScissor( Container * c = NULL, float x = 0, float y = 0, float w = -9999, float h = -9999  );
        static void popScissor();
        static void disableMask();
		static ofColor rgb( int r, int g, int b, int a = 255 ); 
		static ofColor rgb( int rgb, int a = 255 ); 
		static ofColor rgba( int rgba ); 
		static ofColor grey( int g, int a = 255 );
        static bool stringLtString( string a, string b ); 
		static ofRectangle alignBox( Container * container, float width, float height, HorizontalAlign horizontalAlign, VerticalAlign verticalAlign );
		static bool inside( Container * container, float x, float y ); 
        static ofTouchEventArgs translateTouch( ofTouchEventArgs &touch, Container * src, Container * dest );
        static ofPoint translateCoords( float x, float y, Container * src, Container * dest ); 
	private:
		static std::map<std::string, ofImage*> images;
		static std::map<int, MUI_FONT_TYPE*> fonts;
		static std::map<string, MUI_FONT_TYPE*> customFonts;
        static std::stack<ofRectangle> scissorStack;
		
	};
}


#endif
