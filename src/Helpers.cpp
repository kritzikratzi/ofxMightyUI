/*
 *  Images.cpp
 *  ofxMightyUI
 *
 *  Created by hansi on 29.01.11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "Helpers.h"
#include "TextureAtlas.h"
#include "ofxMightyUI.h"
#include <locale>


std::map<std::string, ofTexture*> mui::Helpers::textures;
std::map<std::string, ofImage*> mui::Helpers::images;
std::stack<ofRectangle> mui::Helpers::scissorStack;
mui::TextureAtlas mui::Helpers::atlas;
ofxFontStash2 mui::Helpers::fontStash;
short mui::Helpers::nextCursorId = 0;
std::map<short, GLFWcursor*> mui::Helpers::cursorIdToData;
std::map<string,short> mui::Helpers::cursorNameToId;



// TODO: clear caches/regen texs should take atlas into account?
void mui::Helpers::clearCaches(){
	for(std::map<std::string, ofImage*>::iterator iterator = images.begin(); iterator != images.end(); iterator++) {
		ofImage *img = iterator->second;
		img->clear();
		delete img;
	}
	for(std::map<std::string, ofTexture*>::iterator iterator = textures.begin(); iterator != textures.end(); iterator++) {
		ofTexture *tex = iterator->second;
		tex->clear();
		delete tex;
	}
	textures.clear();

	cerr << "CLEAR CACHE NOT FOR REALLY IMPLEMENTED FOR FONTS. GOING TO LEAK" << endl;
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
	
	if( !ofFile(outputPath.absolute().toString(), ofFile::Reference).exists() ){
		// maybe in the data dir?
		string dataFile = ofToDataPath(path, true);
		if( ofFile(dataFile,ofFile::Reference).exists() ){
			outputPath = Poco::Path(dataFile);
		}
	}
	
	if( mui::MuiConfig::logLevel <= OF_LOG_NOTICE ){
		cout << "loading path: " << outputPath.toString() << " || " << outputPath.absolute().toString() << " || " << path << endl;
	}
	return outputPath.absolute().toString();
}

void mui::Helpers::drawImage( string name, float x, float y, float w, float h ){
	// we have a retina version?
	const ofRectangle * rect = &mui::TextureAtlas::NOT_FOUND;
	
	if( mui::MuiConfig::useRetinaAssets ){
		rect = &atlas.getRectRef(name + "@2x.png");
	}
	
	if( rect == &mui::TextureAtlas::NOT_FOUND ){
		rect = &atlas.getRectRef(name+".png");
	}
	
	if( rect == &mui::TextureAtlas::NOT_FOUND ){
		cerr << "Image: " << name << " not available. this is bad!" << endl;
	}
	else{
		ofEnableTextureEdgeHack(); 
		atlas.addDraw(x, y, w, h, *rect);
	}
}

void mui::Helpers::beginImages(){
	if( !atlas.loaded ){
		string xml = mui::Helpers::muiPath("mui/atlas/ui.xml");
		string png = mui::Helpers::muiPath("mui/atlas/ui.png");
		atlas.load(xml, png);
	}
}

void mui::Helpers::endImages(){
	atlas.drawAdded();
}

bool mui::Helpers::loadFont(string fontName){
	if( !fontStash.isFontLoaded(fontName) ){
		//cout << "Font: [" << fontName << "] not loaded yet, doing this now!" << endl;
		fontStash.addFont(fontName, muiPath(fontName==""?MUI_FONT:fontName));
	}
	
	return true;
}

ofxFontStashStyle mui::Helpers::getStyle( int size ){
	loadFont("");
	ofxFontStashStyle style;
	style.fontSize = size;
	style.fontID="";
	return style;
}

ofxFontStashStyle mui::Helpers::getStyle( string customFont, int fontSize ){
	loadFont(customFont);
	ofxFontStashStyle style;
	style.fontID = customFont;
	style.fontSize = fontSize;
	return style;
}

ofxFontStash2 & mui::Helpers::getFontStash(){
	return fontStash;
}


void mui::Helpers::drawString( string s, float x, float y, ofColor color, int fontSize ){
	loadFont("");
	ofxFontStashStyle style;
	style.fontSize = fontSize;
	style.color = color;
	fontStash.draw(s, style, x, y);
}

mui::Cursor mui::Helpers::getCustomCursor(const string & fontName, const string & character, float xPct, float yPct) {
	return mui::Cursor(mui::Cursor::Custom, getCustomCursorId(fontName, character, xPct, yPct));
}

short mui::Helpers::getCustomCursorId(const string & fontName, const string & character, float xPct, float yPct) {
	string name = fontName + ":" + character;
	auto it = cursorNameToId.find(name);
	if (it == cursorNameToId.end()) {
		loadFont(fontName);
		float h = 16 * muiGetDefaultDisplayScaling();
		ofxFontStashStyle style = getStyle(fontName, h*0.8);
		style.alignment = static_cast<FONSalign>(FONSalign::FONS_ALIGN_CENTER | FONSalign::FONS_ALIGN_MIDDLE);


		cursorNameToId[name] = nextCursorId;

		// we leak intentionally here 

		ofFbo fbo;
		fbo.allocate(h, h, GL_RGBA, 4);
		fbo.begin(); 
		style.color = ofColor(0);
		style.blur = 4;
		getFontStash().draw(character, style, h / 2, h / 2);
		style.color = ofColor(255);
		style.blur = 0;
		getFontStash().draw(character, style, h / 2, h / 2);
		fbo.end();
		ofPixels pixels; 
		fbo.getTexture().readToPixels(pixels);
		fbo.clear();

		GLFWimage image;
		image.width = h;
		image.height = h; 
		image.pixels = pixels.getData();

		GLFWcursor * cursor = glfwCreateCursor(&image, h*xPct, h*yPct);
		cursorIdToData[nextCursorId] = cursor; 

		return nextCursorId++;
	}
	else {
		return it->second;
	}
}

GLFWcursor * mui::Helpers::getCustomCursorData(short customId){
	auto it = cursorIdToData.find(customId);
	return it == cursorIdToData.end() ? nullptr : it->second;
};

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
	float cp1x = prevX + 2.0f/3.0f*(cpx - prevX);
	float cp1y = prevY + 2.0f/3.0f*(cpy - prevY);
	float cp2x = cp1x + (x - prevX)/3.0f;
	float cp2y = cp1y + (y - prevY)/3.0f;
	
	// finally call cubic Bezier curve function
	ofBezierVertex(cp1x, cp1y, cp2x, cp2y, x, y);
};

// this only sets the scissor mask,
// you have to call glEnable( GL_SCISSOR_TEST yourself )
void mui::Helpers::orientedScissor( float x, float y, float w, float h ){
	if( mui::MuiConfig::scaleFactor != 1 ){ x*=mui::MuiConfig::scaleFactor; y*=mui::MuiConfig::scaleFactor; w*=mui::MuiConfig::scaleFactor; h*=mui::MuiConfig::scaleFactor; }
	// position 0
	glScissor( (GLint)x, (GLint)(ofGetHeight()-y-h), (GLint)w, (GLint)h );
}

void mui::Helpers::pushScissor( Container * c, ofRectangle rect ){
	pushScissor(c, rect.x, rect.y, rect.width, rect.height ); 
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

ofRectangle mui::Helpers::alignBox(Container * container, float width, float height, HorizontalAlign horizontalAlign, VerticalAlign verticalAlign) {
	return ofRectangle(
		horizontalAlign == Left ? 0 : (horizontalAlign == Right ? (container->width - width) : ((container->width - width) / 2)),
		verticalAlign == Top ? 0 : (verticalAlign == Bottom ? (container->height - height) : ((container->height - height) / 2)),
		width,
		height
	);
}

ofRectangle mui::Helpers::alignBox(Container * container, const Inset & inset, float width, float height, HorizontalAlign horizontalAlign, VerticalAlign verticalAlign) {
	return ofRectangle(
		horizontalAlign == Left ? inset.left : (horizontalAlign == Right ? (container->width - width - inset.right) : (inset.left + (container->width - inset.left - inset.right - width) / 2)),
		verticalAlign == Top ? inset.top : (verticalAlign == Bottom ? (container->height - height - inset.top - inset.bottom) : (inset.top + (container->height - inset.top - inset.bottom - height) / 2)),
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
    if( src != NULL ){
        p += src->getGlobalPosition(); 
    }
    if( dest != NULL ){
        p -= dest->getGlobalPosition(); 
    }
    
    return p; 
}

ofPoint mui::Helpers::translateCoords( ofPoint pt, Container * src, Container * dest ){
	return translateCoords(pt.x, pt.y, src, dest);
}

