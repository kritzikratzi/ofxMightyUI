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
//	glBlendFunc( GL_ONE, GL_ONE_MINUS_SRC_ALPHA );
//	fbo.draw( (int)size.x, (int)size.y, size.width, size.height );
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
ofRectangle mui::Label::box( float t, float r, float b, float l ){
	ofRectangle size = Helpers::alignBox( this, boundingBox.width, boundingBox.height, horizontalAlign, verticalAlign );
	return ofRectangle( size.x - boundingBox.x - l, size.y - boundingBox.y - t, boundingBox.width + l + r, boundingBox.height + t + b );
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
    

	/*
	int w = floorf(boundingBox.width) + 1;
	int h = floorf(boundingBox.height);
 
	// magic trick #1:
	if( mui::MuiConfig::scaleFactor != 1 ) w *= mui::MuiConfig::scaleFactor, h *= mui::MuiConfig::scaleFactor;
	
	ofPushMatrix();
	if( fbo.getWidth() != w || fbo.getHeight() != h ) fbo.allocate( w, h, GL_RGBA ); 
	ofClear( 0, 0, 0, 0 );
	
	//////////////////////
	// RENDER! 
	//////////////////////
	fbo.begin();
	
	#ifdef TARGET_OPENGLES
	glBlendFuncSeparateOES(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA,GL_ONE,GL_ONE_MINUS_SRC_ALPHA);  //oriol added to get rid of halos
	#else
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA,GL_ONE,GL_ONE_MINUS_SRC_ALPHA); //oriol added to get rid of halos
	#endif
	
	if( mui::MuiConfig::scaleFactor != 1 ){
		ofPushMatrix(); 
		ofScale( 1/mui::MuiConfig::scaleFactor, 1/mui::MuiConfig::scaleFactor, 1 );
	}
	
	ofSetColor( fg.r, fg.g, fg.b ); 
	font->drawString( displayText, -(int)boundingBox.x, -(int)boundingBox.y );
	
	if( mui::MuiConfig::scaleFactor != 1 ){
		ofPopMatrix(); 
	}
	
	
	
	fbo.end();
	ofPopMatrix();*/
}