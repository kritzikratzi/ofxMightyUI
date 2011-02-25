/*
 *  Images.cpp
 *  iPhoneEmptyExample
 *
 *  Created by hansi on 29.01.11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "MUI.h"

std::map<std::string, ofImage*> mui::Helpers::images; 
std::map<int, ofTrueTypeFont*> mui::Helpers::fonts; 
bool mui::Helpers::retinaMode = false; 

/**
 * not much magic need for that retina hack. 
 * look inside Label.cpp and and Root.cpp, nothing else should be affected.  
 * btw - if you do decide to enable retina mode you can consider this mui-library 
 * to also calculate in points, not in pixels. 
 */
void mui::Helpers::enableRetinaHack(){
	if( images.size() > 0 || fonts.size() > 0 ){
		cout << "oooooooo . retina mode is most likely gonna be all fucked up!" << endl; 
	}
	
	retinaMode = true; 
}

ofImage * mui::Helpers::getImage( std::string name ){

	std::map<std::string, ofImage*>::iterator iter = mui::Helpers::images.find( name ); 

	if( iter == images.end() ){
		cout << "Image: " << name << " not loaded yet, doing this now!" << endl; 
		ofImage * img = new ofImage(); 
		if( retinaMode ) img->loadImage( "mui/retina/" + name + ".png" ); 
		else img->loadImage( "mui/normal/" + name + ".png" ); 
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


void mui::Helpers::roundedRect(float x, float y, float w, float h, float r){
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

ofColor rgb( int r, int g, int b ){
	ofColor color; 
	color.r = r; 
	color.g = g; 
	color.b = b; 
	
	return color; 
}

ofColor rgb( int rgbVal ){
	ofColor color; 
	color.r = (rgbVal&0xFF0000)>>16; 
	color.g = (rgbVal&0x00FF00)>>8; 
	color.b = rgbVal&0xFF; 
	
	return color; 
}
