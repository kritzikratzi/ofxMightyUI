/*
 *  Label.cpp
 *  iPhoneEmptyExample
 *
 *  Created by hansi on 29.01.11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 * 
 * ------------------------------
 *  PLEASE READ BEFORE YOU CODE:
 * 
 *  This is a simple container thingie. 
 *  You can use it exactly like an OF main app, there are only small differences in how touch events work. 
 *  1. If you don't want an event to propagate to other containers just do a "return true;" at then end. 
 *  2. A container will only be notified of events that happen within it's bounds
 *  3. If you do a "return true" at the end of touchDown() your container will be remembered and will then 
 *     receive all moved(), doubleTap() and touchUp() events, even if they happen outside the container. 
 *  4. The coordinates touch.x and touch.y are modified to match the drawing coordinate system, so 0/0 is at the 
 *     left top corner of your container, not top left corner of your application. 
 */

#include "MUI.h"

//--------------------------------------------------------------
void mui::Label::update(){
}


//--------------------------------------------------------------
void mui::Label::draw(){
//	// magic trick #1:	
//	int w, h; 
//	if( Helpers::retinaMode ) w = 2*width, h = 2*height; 
//	else w = width, h=height; 
//	

    //	ofRectangle size = Helpers::alignBox( this, fbo.getWidth(), fbo.getHeight(), horizontalAlign, verticalAlign ); 
	ofRectangle size = Helpers::alignBox( this, boundingBox.width, boundingBox.height, horizontalAlign, verticalAlign ); 
	
	ofSetColor( 255, 255, 255 ); 
	glBlendFunc( GL_ONE, GL_ONE_MINUS_SRC_ALPHA ); 
//	fbo.draw( (int)size.x, (int)size.y, size.width, size.height );
    ofSetColor( fg.r, fg.g, fg.b );
	if( mui::MuiConfig::scaleFactor != 1 ){
		MUI_FONT_TYPE * font;
		if( fontName == "" ) font = Helpers::getFont( mui::MuiConfig::scaleFactor*fontSize );
		else  font = Helpers::getFont( fontName, mui::MuiConfig::scaleFactor*fontSize );
		ofPushMatrix();
		ofTranslate( (int)(size.x-boundingBox.x), (int)(size.y-(int)boundingBox.y) );
		ofScale( 0.5, 0.5 );
		font->drawString( displayText, 0, 0 );
		ofPopMatrix();
	}
	else{
		MUI_FONT_TYPE * font;
		if( fontName == "" ) font = Helpers::getFont( fontSize );
		else font = Helpers::getFont( fontName, fontSize );
		font->drawString( displayText, (int)(size.x-boundingBox.x), (int)(size.y-(int)boundingBox.y) );
	}
	

	ofEnableAlphaBlending(); 
}


//--------------------------------------------------------------
void mui::Label::render(){
	// old, maybe new again one day. 
	//Helpers::drawStringWithShadow( text, renderX, renderY, fontSize, fg.r, fg.g, fg.b );
	
	
	if( horizontalAlign == Left ) renderX = -boundingBox.x;
	if( verticalAlign == Top ) renderY = - boundingBox.y; 
}


//--------------------------------------------------------------
void mui::Label::drawBackground(){
    Container::drawBackground(); 
}


ofRectangle mui::Label::box( float t, float r, float b, float l ){
	ofRectangle size = Helpers::alignBox( this, boundingBox.width, boundingBox.height, horizontalAlign, verticalAlign );
	return ofRectangle( size.x - boundingBox.x - l, size.y - boundingBox.y - t, boundingBox.width + l + r, boundingBox.height + t + b );
}
//--------------------------------------------------------------
void mui::Label::commit(){
	// magic trick #2
	// MUI_FONT_TYPE * font = Helpers::getFont( Helpers::retinaMode?(fontSize*2):fontSize );
	// magic trick #2.2: fuck retina, we compute the bounding box at normal size!
	MUI_FONT_TYPE * font = Helpers::getFont( fontSize );
	boundingBox = font->getStringBoundingBox( text, 0, 0 );
	
	
	// NASTY HACK#158
	boundingBox.x = 0;

    if( ellipsisMode ){
        if( boundingBox.width > width && text.length() > 3 ){
            int len = text.length() - 3;
            while( boundingBox.width > width && len >= 0 ){
                displayText = text.substr(0, len ) + "...";
                boundingBox = font->getStringBoundingBox( displayText, 0, 0 ); 
                len --; 
            }
        }
        else{
            displayText = text;
        }
    }
    else{
        displayText = text;
    }
    
/*
	int w = floorf(boundingBox.width) + 1;
	int h = floorf(boundingBox.height);
 
	// magic trick #1:
	if( Helpers::retinaMode ) w *= 2, h *= 2; 
	
	ofPushMatrix(); 
	if( fbo.getWidth() != w || fbo.getHeight() != h ) fbo.allocate( w, h, GL_RGBA ); 
	fbo.clear( 0, 0, 0, 0 ); 
	
	//////////////////////
	// RENDER! 
	//////////////////////
	fbo.begin(); 
	
	#ifdef TARGET_OPENGLES
	glBlendFuncSeparateOES(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA,GL_ONE,GL_ONE_MINUS_SRC_ALPHA);  //oriol added to get rid of halos
	#else
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA,GL_ONE,GL_ONE_MINUS_SRC_ALPHA); //oriol added to get rid of halos
	#endif
	
	if( Helpers::retinaMode ){
		ofPushMatrix(); 
		ofScale( 0.5, 0.5, 1 );
	}
	
	ofSetColor( fg.r, fg.g, fg.b ); 
	font->drawString( text, -(int)boundingBox.x, -(int)boundingBox.y ); 
	
	if( Helpers::retinaMode ){
		ofPopMatrix(); 
	}
	
	
	
	fbo.end();
	ofPopMatrix(); */
}