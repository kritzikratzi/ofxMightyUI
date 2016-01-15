/*
 *  Label.cpp
 *  iPhoneEmptyExample
 *
 *  Text label
 *  - Supports vertical and horizontal alignments
 *  - Call commit() after changing text, fontSize, fontName, ellipsis mode or any of the other variables (except bounds!).
 *  - fg variable affects text color (no commit needed)
 *  - Doesn't support multiline
 */

#include "Label.h"


mui::Label::Label( std::string text_, float x_, float y_, float width_, float height_ ) :
	Container( x_, y_, width_, height_ ),
	ellipsisMode(true), text( text_), fontSize( MUI_FONT_SIZE ), horizontalAlign(Left), verticalAlign(Middle){
		ignoreEvents = true;
		commit();
};
//--------------------------------------------------------------
void mui::Label::update(){
}


//--------------------------------------------------------------
void mui::Label::draw(){
	ofRectangle size = Helpers::alignBox( this, boundingBox.width, boundingBox.height, horizontalAlign, verticalAlign );
	
	ofSetColor( 255, 255, 255 ); 
    ofSetColor( fg.r, fg.g, fg.b, fg.a );
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
}


//--------------------------------------------------------------
void mui::Label::drawBackground(){
    Container::drawBackground(); 
}

//--------------------------------------------------------------
void mui::Label::layout(){
	// we might have to recalculate the text if things change ...
	if( ellipsisMode ){
		commit();
	}
}

void mui::Label::sizeToFit( float padX, float padY ){
	commit(); // update bounding box
	width = boundingBox.width + padX;
	height = boundingBox.height + padY;
	layout(); // tell ourselves about the size change
}

void mui::Label::sizeToFitWidth( float padX ){
	commit();
	width = boundingBox.width + padX;
	layout();
}

void mui::Label::sizeToFitHeight( float padY ){
	commit();
	height = boundingBox.height + padY;
	layout();
}

//--------------------------------------------------------------
//deprecated
ofRectangle mui::Label::box( float t, float r, float b, float l ){
	ofRectangle size = Helpers::alignBox( this, boundingBox.width, boundingBox.height, horizontalAlign, verticalAlign );
	return ofRectangle( size.x - boundingBox.x - l, size.y - boundingBox.y + t, boundingBox.width + l + r, boundingBox.height + t + b );
}

//--------------------------------------------------------------
void mui::Label::commit(){
	// magic trick #2
	// MUI_FONT_TYPE * font = Helpers::getFont( Helpers::retinaMode?(fontSize*2):fontSize );
	// magic trick #2.2: fuck retina, we compute the bounding box at normal size!
	MUI_FONT_TYPE * font;
	if( fontName == "" ) font = Helpers::getFont( fontSize );
	else  font = Helpers::getFont( fontName, fontSize );
	boundingBox = font->getStringBoundingBox( text, 0, 0 );
	
	// NASTY HACK#158
	boundingBox.x = 0;

    if( ellipsisMode ){
        if( boundingBox.width > width && text.length() > 3 ){
			// don't mess with the original boundingBox object. it's publicly readable!
			ofRectangle box = boundingBox;
            int len = text.length() - 3;
            while( box.width > width && len >= 0 ){
                displayText = text.substr(0, len ) + "...";
                box = font->getStringBoundingBox( displayText, 0, 0 );
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
	
	// Orient y on a simple uppercase character
	// Otherwise things go up and down unexpectedly
	ofRectangle baselineSize = font->getStringBoundingBox("M", 0, 0);
	boundingBox.height = baselineSize.height;
	boundingBox.y = baselineSize.y;
}