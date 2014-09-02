#include "ofxTrueTypeFontFS.h"


std::map<string,ofxFontStash*> ofxTrueTypeFontFS::fonts;
#define FONTSTASH_SIZE fontSize*1.5

//------------------------------------------------------------------------
ofxTrueTypeFontFS::ofxTrueTypeFontFS(){
	bLoadedOk		= false;
	bMakeContours	= false;
	letterSpacing = 1;
	spaceSize = 1;
	border			= 3;
	binded = false;
	font = NULL; 
}

//------------------------------------------------------------------------
ofxTrueTypeFontFS::~ofxTrueTypeFontFS(){
	// not needed :D 
}

//------------------------------------------------------------------------
// 			-- default (without dpi), non-full char set, anti aliased, 96 dpi:
bool ofxTrueTypeFontFS::loadFont(string filename, int fontsize, bool _bAntiAliased, bool _bFullCharacterSet, bool makeContours, float simplifyAmt, int dpi){
	
	std::map<string, ofxFontStash*>::iterator iter = ofxTrueTypeFontFS::fonts.find( filename );
	
	// see what i did there? 
	fontSize = fontsize;
	
	
	if( iter == fonts.end() ){
		cout << "Font: " << fontSize << " not loaded yet, doing this now!" << endl;
		font = new ofxFontStash();
		bLoadedOk = font->setup( filename, 1.0 );
		if( bLoadedOk ){
			fonts[filename] = font;
			return true;
		}
		else{
			return false;
		}
	}
	
	font = iter->second;
	return true;
}

//------------------------------------------------------------------------
bool ofxTrueTypeFontFS::isLoaded(){
	return bLoadedOk;
}

//------------------------------------------------------------------------
bool ofxTrueTypeFontFS::isAntiAliased(){
	return true;
}

//------------------------------------------------------------------------
bool ofxTrueTypeFontFS::hasFullCharacterSet(){
	return true;
}

//------------------------------------------------------------------------
int ofxTrueTypeFontFS::getSize(){
	return fontSize;
}

//------------------------------------------------------------------------
float ofxTrueTypeFontFS::getLineHeight(){
	return lineHeight;
}

//------------------------------------------------------------------------
void ofxTrueTypeFontFS::setLineHeight(float height){
	lineHeight = height;
};

//------------------------------------------------------------------------
float ofxTrueTypeFontFS::getLetterSpacing(){
	return -1;
}

//------------------------------------------------------------------------
void ofxTrueTypeFontFS::setLetterSpacing(float spacing){ ;
};

//------------------------------------------------------------------------
float ofxTrueTypeFontFS::getSpaceSize(){
	return -1;
};

//------------------------------------------------------------------------
void ofxTrueTypeFontFS::setSpaceSize(float size){
	;
};

//------------------------------------------------------------------------
float ofxTrueTypeFontFS::stringWidth(string s){
	ofRectangle bb = getStringBoundingBox( s, 0, 0 );
	return -bb.x + bb.width;
}

//------------------------------------------------------------------------
float ofxTrueTypeFontFS::stringHeight(string s){
	ofRectangle bb = getStringBoundingBox( s, 0, 0);
	return -bb.y + bb.height;
}

//------------------------------------------------------------------------
ofRectangle ofxTrueTypeFontFS::getStringBoundingBox(string s, float x, float y){
	return font->getBoundingBoxSize( s, FONTSTASH_SIZE, x, y );
}

//------------------------------------------------------------------------
void ofxTrueTypeFontFS::drawString(string s, float x, float y){
	ofEnableAlphaBlending(); 
	font->draw( s, FONTSTASH_SIZE, x, y );
}

//------------------------------------------------------------------------
void ofxTrueTypeFontFS::drawStringAsShapes(string s, float x, float y){
	// can't do. whatever!
	font->draw( s, FONTSTASH_SIZE, x, y );
}

//			get the num chars in the loaded char set
//------------------------------------------------------------------------
int ofxTrueTypeFontFS::getNumCharacters(){
	return 17; // who cares?
}

//------------------------------------------------------------------------
ofTTFCharacter ofxTrueTypeFontFS::getCharacterAsPoints(int character){
	// how this is not an error is beyond me
	return ofTTFCharacter(); 
}

//------------------------------------------------------------------------
vector<ofTTFCharacter> getStringAsPoints(string str){
	// same
	return vector<ofTTFCharacter>(); 
}

//------------------------------------------------------------------------
void ofxTrueTypeFontFS::bind(){
	// whatever
}

//------------------------------------------------------------------------
void ofxTrueTypeFontFS::unbind(){
	// i don't really care
}


