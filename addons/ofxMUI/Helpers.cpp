/*
 *  Images.cpp
 *  iPhoneEmptyExample
 *
 *  Created by hansi on 29.01.11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "MUI.h"
#include <locale>

std::map<std::string, ofImage*> mui::Helpers::images; 
std::map<int, MUI_FONT_TYPE*> mui::Helpers::fonts;
std::map<string, MUI_FONT_TYPE*> mui::Helpers::customFonts;
std::stack<ofRectangle> mui::Helpers::scissorStack;

void mui::Helpers::clearCaches(){
	for(std::map<std::string, ofImage*>::iterator iterator = images.begin(); iterator != images.end(); iterator++) {
		ofImage *img = iterator->second;
		img->clear();
		delete img;
	}
	images.clear();


	#if MUI_FONT_TYPE_ACTIVE == MUI_FONT_TYPE_FONTSTASH
		ofxTrueTypeFontFS::clearCaches();
		fonts.clear();
		customFonts.clear();
	#elif MUI_FONT_TYPE_ACTIVE == MUI_FONT_TYPE_OPENFRAMEWORKS
		for(std::map<int, ofTrueTypeFont*>::iterator iterator = fonts.begin(); iterator != fonts.end(); iterator++) {
			ofTrueTypeFont *font = iterator->second;
			delete font;
		}
		for(std::map<std::string, ofTrueTypeFont*>::iterator iterator = customFonts.begin(); iterator != customFonts.end(); iterator++) {
			ofTrueTypeFont *font = iterator->second;
			delete font;
		}
		fonts.clear();
		customFonts.clear();
	#else
		#error Unsupported Font Rendering Implemtation selected.
	#endif

}

std::string mui::Helpers::muiPath( std::string path ){
	// pretty much copy&pasted from OF
	Poco::Path outputPath;
	Poco::Path inputPath(path);
	string strippedDataPath = mui::MuiConfig::dataPath.toString();
	strippedDataPath = ofFilePath::removeTrailingSlash(strippedDataPath);
	if (inputPath.toString().find(strippedDataPath) != 0) {
		outputPath = mui::MuiConfig::dataPath;
		outputPath.resolve(inputPath);
	} else {
		outputPath = inputPath;
	}
	
	cout << "loading path: " << outputPath.toString() << " || " << outputPath.absolute().toString() << " || " << path << endl;
	return outputPath.absolute().toString();
}


ofImage * mui::Helpers::getImage( std::string name ){

	std::map<std::string, ofImage*>::iterator iter = mui::Helpers::images.find( name ); 

	if( iter == images.end() ){
		cout << "Image: " << name << " not loaded yet, doing this now!" << endl; 
		ofImage * img = new ofImage(); 
		if( mui::MuiConfig::useRetinaAssets ){
			img->loadImage( muiPath("mui/retina/" + name + ".png") );
			if( img->width == 0 ){
				cerr << "Image: " << name << " not available in retina folder. trying normal!" << endl;
				img->loadImage( muiPath("mui/normal/" + name + ".png") );
			}
		}
		else{
			img->loadImage( muiPath("mui/normal/" + name + ".png") );
		}
		images[name] = img; 
		return img; 
	}
	
	return iter->second;
}

MUI_FONT_TYPE* mui::Helpers::getFont( int fontSize ){
	std::map<int, MUI_FONT_TYPE*>::iterator iter = mui::Helpers::fonts.find( fontSize );
	
	if( iter == fonts.end() ){
		cout << "Font: " << fontSize << " not loaded yet, doing this now!" << endl;
		MUI_FONT_TYPE * font = new MUI_FONT_TYPE();
		font->loadFont( muiPath(MUI_FONT), fontSize, true );
		fonts[fontSize] = font;
		return font;
	}
	
	return iter->second;
}

MUI_FONT_TYPE* mui::Helpers::getFont( string customFont, int fontSize ){
	string id = customFont + ofToString(fontSize);
	std::map<string, MUI_FONT_TYPE*>::iterator iter = mui::Helpers::customFonts.find( id );
	
	if( iter == customFonts.end() ){
		cout << "Font: " << fontSize << " not loaded yet, doing this now!" << endl;
		MUI_FONT_TYPE * font = new MUI_FONT_TYPE();
		font->loadFont( muiPath(customFont), fontSize, true );
		customFonts[id] = font;
		return font;
	}
	
	return iter->second;
}

void mui::Helpers::drawString( string s, float x, float y, int fontSize ){
	if( mui::MuiConfig::useRetinaAssets ){
		MUI_FONT_TYPE * font = Helpers::getFont( 2*fontSize );
		ofPushMatrix();
		ofTranslate( x, y );
		ofScale( 0.5, 0.5 );
		font->drawString( s, 0, 0 );
		ofPopMatrix();
	}
	else{
		MUI_FONT_TYPE * font = Helpers::getFont( fontSize );
		font->drawString( s, x, y );
	}
}


void mui::Helpers::drawStringWithShadow( std::string s, int x, int y, int fontSize, int r, int g, int b ){
	MUI_FONT_TYPE * font = mui::Helpers::getFont( fontSize ); 
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

ofColor mui::Helpers::rgb( int r, int g, int b, int a ){
	ofColor color; 
	color.r = r; 
	color.g = g; 
	color.b = b; 
	color.a = a; 
	
	return color; 
}

// this only sets the scissor mask,
// you have to call glEnable( GL_SCISSOR_TEST yourself )
void mui::Helpers::orientedScissor( float x, float y, float w, float h ){
	if( mui::MuiConfig::scaleFactor != 1 ){ x*=mui::MuiConfig::scaleFactor; y*=mui::MuiConfig::scaleFactor; w*=mui::MuiConfig::scaleFactor; h*=mui::MuiConfig::scaleFactor; }
	// position 0
	glScissor( x, ofGetHeight()-y-h, w, h );
    
	// position 1
	//glScissor( y, x, h, w );
    
	//TODO: clarify next line!
	//maybe this is an ios-setup specific solution
	// in portrait mode:
	//    glScissor( pos.x, pos.y, width, height );
}

void mui::Helpers::pushScissor( Container * c, float x, float y, float w, float h ){
    glEnable( GL_SCISSOR_TEST );
    if( c != NULL && w == -9999 ) w = c->width+1;
    if( c != NULL && h == -9999 ) h = c->height+1;
    
    ofPoint pos(x, y);
    
    if( c != NULL ){
        ofPoint trans = c->getGlobalPosition();
        pos.x += trans.x;
        pos.y += trans.y;
    }
	
    
	ofRectangle r( pos.x, pos.y, w, h );
	
	if( scissorStack.size() > 0 ){
		ofRectangle & m = scissorStack.top();
		r.x = fmaxf( r.x, m.x );
		r.y = fmaxf( r.y, m.y );
		r.width = fminf( pos.x + r.width, m.x + m.width ) - pos.x;
		r.height = fminf( pos.y + r.height, m.y + m.height ) - pos.y;
	}
	
	scissorStack.push( r );
	orientedScissor( r.x, r.y, r.width, r.height );
}

void mui::Helpers::popScissor(){
	scissorStack.pop();
	
    if( scissorStack.size() == 0 ){
		glDisable( GL_SCISSOR_TEST );
	}
	else{
		ofRectangle &r = scissorStack.top();
		orientedScissor( r.x, r.y, r.width, r.height );
	}
}


ofColor mui::Helpers::rgb( int rgbVal, int a ){
	ofColor color; 
	color.r = (rgbVal&0xFF0000)>>16; 
	color.g = (rgbVal&0x00FF00)>>8; 
	color.b = rgbVal&0xFF; 
	color.a = a; 
	
	return color; 
}

ofColor mui::Helpers::rgba( int rgbVal ){
	ofColor color; 
	color.a = (rgbVal&0xFF000000)>>24; 
	color.r = (rgbVal&0xFF0000)>>16; 
	color.g = (rgbVal&0xFF00)>>8; 
	color.b = rgbVal&0xFF; 
	
	return color; 
}


ofColor mui::Helpers::grey( int g, int a ){
	ofColor color; 
	color.r = g; 
	color.g = g; 
	color.b = g; 
	color.a = a; 
	
	return color; 
}

// compares two strings with a strict weak order,
// so stringLtString(a,a) will return false (obviously)
bool mui::Helpers::stringLtString(string strA, string strB){
	static std::locale loc;
    int N = MIN( strA.length(), strB.length() );
    for( int i = 0; i < N; i++ ){
        char chA = std::toupper( strA[i], loc );
        char chB = std::toupper( strB[i], loc );
        bool isLetterA = chA >= 'A' && chA <= 'Z';
        bool isLetterB = chB >= 'A' && chB <= 'Z';
        bool isDigitA = chA >= '0' && chA <= '9';
        bool isDigitB = chB >= '0' && chB <= '9';
        bool isSpecialA = !isLetterA && !isDigitA;
        bool isSpecialB = !isLetterB && !isDigitB;
        
        
        // first the cases where we have two chars of the same type,
        // in all these we sort naturally
        if( chA == chB ){
            continue;
        }
        else if( ( isLetterA && isLetterB ) || ( isDigitA && isDigitB ) || ( isSpecialA && isSpecialB ) ){
            return chA < chB;
        }
        // then digits
        else if( isDigitA || isDigitB ){
            return isDigitB;
        }
        // special chars after everything ...
        else if( isSpecialA || isSpecialB ){
            return isSpecialB;
        }
        // rofl, they're not both letters,
        // so you can't really ever end up here...
        else{
            cout << "odd, but whatever..." << endl;
            return isLetterB;
        }
    }
    
	if( strA.length() == 0 ) return false;
	else if( strB.length() == 0 ) return true;
    else return strA.length() < strB.length();
}
ofRectangle mui::Helpers::alignBox( Container * container, float width, float height, HorizontalAlign horizontalAlign, VerticalAlign verticalAlign ){
	return ofRectangle( 
		horizontalAlign == Left? 0 : ( horizontalAlign == Right? (container->width - width) : ((container->width-width)/2)), 
		verticalAlign == Top? 0 : ( verticalAlign == Bottom? (container->height - height) : ((container->height-height)/2)), 
		width, 
		height
	); 
}

bool mui::Helpers::inside( Container * container, float x, float y ){
	return x >= 0 && y >= 0 && x <= container->width && y <= container->height; 
}

//--------------------------------------------------------------
// translate touch coordinates from one container's coordinates into another's. 
// if the *src container is global already you can point it to NULL, 
// or if the destination container is the global coordinate system use NULL for *dest
ofTouchEventArgs mui::Helpers::translateTouch( ofTouchEventArgs &touch, Container * src, Container * dest ){
    ofTouchEventArgs copy = touch; 
    
	/*if( Helpers::retinaMode ){
		copy.x = touch.x/2; 
		copy.y = touch.y/2;
        copy.xspeed = touch.xspeed/2;
        copy.yspeed = touch.yspeed/2; 
	}
	else{
		copy.x = touch.x; 
		copy.y = touch.y; 
        copy.xspeed = touch.xspeed; 
        copy.yspeed = touch.yspeed; 
	}*/

	if( src != NULL ){
		ofPoint pos = src->getGlobalPosition();
		copy.x += pos.x;
		copy.y += pos.y;
	}
    if( dest != NULL ){
        ofPoint pos = dest->getGlobalPosition(); 
        copy.x -= pos.x; 
        copy.y -= pos.y; 
    }
    
    return copy; 
}


ofPoint mui::Helpers::translateCoords(float x, float y, mui::Container *src, mui::Container *dest){
    ofPoint p(x, y ); 
    cout << "[p=" << p.x << "," << p.y << endl; 
    if( src != NULL ){
        p += src->getGlobalPosition(); 
        cout << "[sp=" << p.x << "," << p.y << endl; 
    }
    if( dest != NULL ){
        p -= dest->getGlobalPosition(); 
        cout << "[dp=" << p.x << "," << p.y << endl; 
    }
    
    return p; 
}
