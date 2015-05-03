//
//  TextureAtlas.cpp
//  OsciStudio
//
//  Created by Hansi on 23.04.15.
//
//

#include "TextureAtlas.h"

const ofRectangle mui::TextureAtlas::NOT_FOUND(0,0,0,0);

mui::TextureAtlas::TextureAtlas(){
	quad.getVertices().resize(4);
	quad.getTexCoords().resize(4);
	quad.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
	add.setMode(OF_PRIMITIVE_TRIANGLES);
}

void mui::TextureAtlas::load( string absXmlPath, string absImgPath ){
	ofLoadImage(tex, absImgPath);
	tex.setTextureWrap(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
	tex.setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
	
	ofXml xml(absXmlPath);
//	xml.setTo("");
	
	xml.setTo( "//" );
	int numTiles = xml.getNumChildren();
	for( int i = 0; i < numTiles; i++ ){
		xml.setToChild(i);
		ofRectangle rect;
		rect.x = ofToInt(xml.getAttribute("x"));
		rect.y = ofToInt(xml.getAttribute("y"));
		rect.width = ofToInt(xml.getAttribute("w"));
		rect.height = ofToInt(xml.getAttribute("h"));
		rects[xml.getAttribute("n")] = rect;
		xml.setToParent();
	}
	
	loaded = true;
}

void mui::TextureAtlas::bind(){
	glActiveTexture(GL_TEXTURE0);
	tex.bind();
}

void mui::TextureAtlas::unbind(){
	tex.unbind();
}

void mui::TextureAtlas::draw( string name, float x, float y ){
	const ofRectangle & rect = getRectRef(name);
	drawSubsection(x,y,0,rect.width,rect.height,rect.x,rect.y,rect.width,rect.height);
}

void mui::TextureAtlas::draw( string name, float x, float y, float w, float h ){
	const ofRectangle & rect = getRectRef(name);
	drawSubsection(x,y,0,w,h,rect.x,rect.y,rect.width,rect.height);
}

void mui::TextureAtlas::draw( float x, float y, float w, float h, const ofRectangle & rect ){
	drawSubsection(x, y, 0, w, h, rect.x, rect.y, rect.width, rect.height);
}


const ofRectangle & mui::TextureAtlas::getRectRef( string name ){
	map<string,ofRectangle>::iterator it = rects.find(name);
	if( it != rects.end() ){
		return (*it).second;
	}
	else{
		return NOT_FOUND;
	}
}

void mui::TextureAtlas::addDraw(string name, float x, float y, float w, float h ){
	const ofRectangle & rect = getRectRef(name);
	addSubsection(x,y,0,w,h,rect.x,rect.y,rect.width,rect.height);
}

void mui::TextureAtlas::addDraw(float x, float y, float w, float h, const ofRectangle & rect ){
	addSubsection(x,y,0,w,h,rect.x,rect.y,rect.width,rect.height);
}

void mui::TextureAtlas::drawAdded(){
	bind();
	add.draw();
	add.clear();
	unbind();
}

// see drawSubsection
void mui::TextureAtlas::addSubsection(float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh){
	GLfloat px0 = x;		// up to you to get the aspect ratio right
	GLfloat py0 = y;
	GLfloat px1 = w+x;
	GLfloat py1 = h+y;
	
	if (tex.texData.bFlipTexture == ofIsVFlipped()){
		swap(py0,py1);
	}
	
	// for rect mode center, let's do this:
	if (ofGetRectMode() == OF_RECTMODE_CENTER){
		px0 -= w/2;
		py0 -= h/2;
		px1 -= w/2;
		py1 -= h/2;
	}
	
	//we translate our drawing points by our anchor point.
	//we still respect ofRectMode so if you have rect mode set to
	//OF_RECTMODE_CENTER your anchor will be relative to that.
	GLfloat anchorX;
	GLfloat anchorY;
	
	
	// -------------------------------------------------
	// complete hack to remove border artifacts.
	// slightly, slightly alters an image, scaling...
	// to remove the border.
	// we need a better solution for this, but
	// to constantly add a 2 pixel border on all uploaded images
	// is insane..
	
	GLfloat offsetw = 0.0f;
	GLfloat offseth = 0.0f;
	
	if (!ofGLSupportsNPOTTextures() && ofIsTextureEdgeHackEnabled()) {
		offsetw = 1.0f / (tex.texData.tex_w);
		offseth = 1.0f / (tex.texData.tex_h);
	}
	// -------------------------------------------------
	
	ofPoint topLeft = tex.getCoordFromPoint(sx, sy);
	ofPoint bottomRight = tex.getCoordFromPoint(sx + sw, sy + sh);
	
	GLfloat tx0 = topLeft.x + offsetw;
	GLfloat ty0 = topLeft.y + offseth;
	GLfloat tx1 = bottomRight.x - offsetw;
	GLfloat ty1 = bottomRight.y - offseth;
	
	/*if(z>0 || z<0){
		ofPushMatrix();
	 
		ofTranslate(0,0,z);
	 }*/

	add.addVertex(ofVec3f(px0,py0,z));
	add.addVertex(ofVec3f(px1,py0,z));
	add.addVertex(ofVec3f(px1,py1,z));

	add.addVertex(ofVec3f(px0,py0,z));
	add.addVertex(ofVec3f(px1,py1,z));
	add.addVertex(ofVec3f(px0,py1,z));
	
	add.addTexCoord(ofVec3f(tx0,ty0));
	add.addTexCoord(ofVec3f(tx1,ty0));
	add.addTexCoord(ofVec3f(tx1,ty1));
	
	add.addTexCoord(ofVec3f(tx0,ty0));
	add.addTexCoord(ofVec3f(tx1,ty1));
	add.addTexCoord(ofVec3f(tx0,ty1));
}

// copied almost directly from ofTexture.cpp
// changes:
// - no anchor point support (doesn't make sense)
// - removed bind/unbind/activetex
//----------------------------------------------------------
void mui::TextureAtlas::drawSubsection(float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh) {
	GLfloat px0 = x;		// up to you to get the aspect ratio right
	GLfloat py0 = y;
	GLfloat px1 = w+x;
	GLfloat py1 = h+y;
	
	if (tex.texData.bFlipTexture == ofIsVFlipped()){
		swap(py0,py1);
	}
	
	// for rect mode center, let's do this:
	if (ofGetRectMode() == OF_RECTMODE_CENTER){
		px0 -= w/2;
		py0 -= h/2;
		px1 -= w/2;
		py1 -= h/2;
	}
	
	//we translate our drawing points by our anchor point.
	//we still respect ofRectMode so if you have rect mode set to
	//OF_RECTMODE_CENTER your anchor will be relative to that.
	GLfloat anchorX;
	GLfloat anchorY;
	
	
	// -------------------------------------------------
	// complete hack to remove border artifacts.
	// slightly, slightly alters an image, scaling...
	// to remove the border.
	// we need a better solution for this, but
	// to constantly add a 2 pixel border on all uploaded images
	// is insane..
	
	GLfloat offsetw = 0.0f;
	GLfloat offseth = 0.0f;
	
	if (!ofGLSupportsNPOTTextures() && ofIsTextureEdgeHackEnabled()) {
		offsetw = 1.0f / (tex.texData.tex_w);
		offseth = 1.0f / (tex.texData.tex_h);
	}
	// -------------------------------------------------
	
	ofPoint topLeft = tex.getCoordFromPoint(sx, sy);
	ofPoint bottomRight = tex.getCoordFromPoint(sx + sw, sy + sh);
	
	GLfloat tx0 = topLeft.x + offsetw;
	GLfloat ty0 = topLeft.y + offseth;
	GLfloat tx1 = bottomRight.x - offsetw;
	GLfloat ty1 = bottomRight.y - offseth;
	
	/*if(z>0 || z<0){
		ofPushMatrix();
	 
		ofTranslate(0,0,z);
	 }*/
	quad.getVertices()[0].set(px0,py0,z);
	quad.getVertices()[1].set(px1,py0,z);
	quad.getVertices()[2].set(px1,py1,z);
	quad.getVertices()[3].set(px0,py1,z);
	
	quad.getTexCoords()[0].set(tx0,ty0);
	quad.getTexCoords()[1].set(tx1,ty0);
	quad.getTexCoords()[2].set(tx1,ty1);
	quad.getTexCoords()[3].set(tx0,ty1);
	
	quad.draw();
}
