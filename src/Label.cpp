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
	ellipsisMode(true), text( text_), fontSize(-1), horizontalAlign(Left), verticalAlign(Middle),fontName(""),displayTextNumTris(0){
		ignoreEvents = true;
		if( fontSize < 0 ) fontSize = mui::MuiConfig::fontSize;
		commit();
};
//--------------------------------------------------------------
void mui::Label::update(){
	fontStyle.fontSize = fontSize;
	fontStyle.color = fg;
	fontStyle.fontID = fontName;
}


//--------------------------------------------------------------
void mui::Label::draw(){
	ofRectangle size = Helpers::alignBox( this, boundingBox.width, boundingBox.height, horizontalAlign, verticalAlign );
	mui::Helpers::getFontStash().draw(displayText, fontStyle, size.x-boundingBox.x, size.y-boundingBox.y);
	//cout << "tris = " << displayTextNumTris << endl;
	//displayTextVbo.draw( GL_TRIANGLES, 0, displayTextNumTris );
	ofSetColor( 255, 255, 255 );
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
	ellipsisMode = false;
	commit(); // update bounding box
	width = boundingBox.width + padX;
	height = boundingBox.height + padY;
	layout(); // tell ourselves about the size change
}

void mui::Label::sizeToFitWidth( float padX ){
	ellipsisMode = false;
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
	update();
	// magic trick #2
	// MUI_FONT_TYPE * font = Helpers::getFont( Helpers::retinaMode?(fontSize*2):fontSize );
	// magic trick #2.2: fuck retina, we compute the bounding box at normal size!
	mui::Helpers::loadFont(fontName);
	boundingBox = Helpers::getFontStash().getTextBounds(text, fontStyle, 0, 0);
	// NASTY HACK#158
	boundingBox.x = 0;

    if( ellipsisMode ){
        if( boundingBox.width > width && text.length() > 3 ){
			// don't mess with the original boundingBox object. it's publicly readable!
			ofRectangle box = boundingBox;
            int len = text.length() - 3;
            while( box.width > width && len >= 0 ){
                displayText = text.substr(0, len ) + "...";
				box = Helpers::getFontStash().getTextBounds(displayText, fontStyle, 0, 0 );
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
	
	//vector<StyledText> styledText;
	//styledText.push_back(StyledText{displayText,fontStyle});
	//vector<StyledLine> lines = Helpers::getFontStash().layoutLines(styledText, 999999);
	//Helpers::getFontStash().drawLinesToVbo(displayTextVbo, displayTextNumTris, lines, 0, 0);
	
	// Orient y on a simple uppercase character
	// Otherwise things go up and down unexpectedly
	ofRectangle baselineSize = Helpers::getFontStash().getTextBounds("M", fontStyle, 0, 0);
	boundingBox.height = baselineSize.height;
	boundingBox.y = baselineSize.y;
}