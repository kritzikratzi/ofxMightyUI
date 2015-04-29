//
//  ofxTrueTypeFontFS.h
//  Music Path
//
//  Created by Hansi on 04.10.12.
//
//

#pragma once

#pragma once

#include "ofxFontStash.h"
#include <map>

class ofTrueTypeFont;

class ofxTrueTypeFontFS : public ofTrueTypeFont{
	
public:
	
	
	ofxTrueTypeFontFS();
	virtual ~ofxTrueTypeFontFS();
	
	//set the default dpi for all typefaces.
	static void setGlobalDpi(int newDpi);
	
	// 			-- default (without dpi), non-full char set, anti aliased, 96 dpi:
	bool 		loadFont(string filename, int fontsize, bool _bAntiAliased=true, bool _bFullCharacterSet=false, bool makeContours=false, float simplifyAmt=0.3, int dpi=0);
	
	bool		isLoaded();
	bool		isAntiAliased();
	bool		hasFullCharacterSet();
	
    int         getSize();
    float       getLineHeight();
  	void 		setLineHeight(float height);
	float 		getLetterSpacing();
	void 		setLetterSpacing(float spacing);
	float 		getSpaceSize();
	void 		setSpaceSize(float size);
	float 		stringWidth(string s);
	float 		stringHeight(string s);
	
	ofRectangle    getStringBoundingBox(string s, float x, float y);
	
	void 		drawString(string s, float x, float y);
	void		drawStringAsShapes(string s, float x, float y);
	
	//			get the num chars in the loaded char set
	int			getNumCharacters();
	
	ofTTFCharacter getCharacterAsPoints(int character);
	vector<ofTTFCharacter> getStringAsPoints(string str);
	
	void bind();
	void unbind();

	static void clearCaches();

protected:
	bool			bLoadedOk;
	bool 			bAntiAliased;
	bool 			bFullCharacterSet;
	int 			nCharacters;
	
	vector <ofTTFCharacter> charOutlines;
	
	float 			lineHeight;
	float			letterSpacing;
	float			spaceSize;
	
	vector<charProps> 	cps;			// properties for each character
	
	int				fontSize;
	bool			bMakeContours;
	float 			simplifyAmt;
	int 			dpi;
	
	void 			drawChar(int c, float x, float y);
	void			drawCharAsShape(int c, float x, float y);
	
	int				border;//, visibleBorder;
	string			filename;
	
	ofTexture texAtlas;
	bool binded;
	ofMesh stringQuads;

private:
	static std::map<string, ofxFontStash*> fonts;
	ofxFontStash * font;

};


