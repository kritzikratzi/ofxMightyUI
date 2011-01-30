/*
 *  Images.cpp
 *  iPhoneEmptyExample
 *
 *  Created by hansi on 29.01.11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "Helpers.h"

std::map<std::string, ofImage*> mui::Helpers::images; 
std::map<int, ofTrueTypeFont*> mui::Helpers::fonts; 

ofImage * mui::Helpers::getImage( std::string name ){

	std::map<std::string, ofImage*>::iterator iter = mui::Helpers::images.find( name ); 

	if( iter == images.end() ){
		cout << "Image: " << name << " not loaded yet, doing this now!" << endl; 
		ofImage * img = new ofImage(); 
		img->loadImage( "mui/normal/" + name + ".png" ); 
		images[name] = img; 
		return img; 
	}
	
	return iter->second;
	return NULL; 
}

ofTrueTypeFont* mui::Helpers::getFont( int fontSize ){
	
	std::map<int, ofTrueTypeFont*>::iterator iter = mui::Helpers::fonts.find( fontSize ); 
	
	if( iter == fonts.end() ){
		cout << "Font: " << fontSize << " not loaded yet, doing this now!" << endl; 
		ofTrueTypeFont * font = new ofTrueTypeFont(); 
		font->loadFont( MUI_FONT, fontSize ); 
		fonts[fontSize] = font; 
		return font; 
	}
	
	return iter->second;
	return NULL; 
}


void mui::Helpers::drawStringWithShadow( std::string s, int x, int y, int fontSize, int r, int g, int b ){
	ofTrueTypeFont * font = mui::Helpers::getFont( fontSize ); 
	ofSetColor( r/2, g/2, b/2, 150 ); 
	font->drawString( s, x, y+1 ); 
	ofSetColor( r, g, b ); 
	font->drawString( s, x, y ); 
}


void mui::Helpers::roundedRect(float x, float y, float w, float h, float r) {
    ofBeginShape();
	ofVertex(x+r, y);
	ofVertex(x+w-r, y);
	quadraticBezierVertex(x+w, y, x+w, y+r, x+w-r, y);
	ofVertex(x+w, y+h-r);
	quadraticBezierVertex(x+w, y+h, x+w-r, y+h, x+w, y+h-r);
	ofVertex(x+r, y+h);
	quadraticBezierVertex(x, y+h, x, y+h-r, x+r, y+h);
	ofVertex(x, y+r);
	quadraticBezierVertex(x, y, x+r, y, x, y+r);
    ofEndShape();
}

void mui::Helpers::quadraticBezierVertex(float cpx, float cpy, float x, float y, float prevX, float prevY) {
	float cp1x = prevX + 2.0/3.0*(cpx - prevX);
	float cp1y = prevY + 2.0/3.0*(cpy - prevY);
	float cp2x = cp1x + (x - prevX)/3.0;
	float cp2y = cp1y + (y - prevY)/3.0;
	
	// finally call cubic Bezier curve function
	ofBezierVertex(cp1x, cp1y, cp2x, cp2y, x, y);
};