#include "MUI.h"
#include "Helpers.h"

mui::Image::Image( string filename, float x_, float y_, float width_, float height_ )
: Container( x_, y_, width_, height_ ), texRef(NULL),horizontalAlign(Center),verticalAlign(Middle),scaleMethod(ScaleMethodFitInside) {
	ofLoadImage(tex, filename);
}

//--------------------------------------------------------------
void mui::Image::update(){
}


//--------------------------------------------------------------
void mui::Image::draw(){
	ofTexture * tex = NULL;
	
	if( texRef != NULL ){
		tex = texRef;
	}
	else if( this->tex.isAllocated() ){
		tex = &this->tex;
	}
	
	if( tex != NULL && tex->getWidth() > 0 && tex->getHeight() > 0 ){
		float imgW = tex->getWidth(), imgH = tex->getHeight();
		float imgRatio = imgW/imgH;
		float ratio = width/height;
		float scale = 1;
		
		switch( scaleMethod ){
			case ScaleMethodFitInside:
				if( imgRatio>ratio ) scale = width/imgW;
				else scale = height/imgH;
				break;
			case ScaleMethodCropToFill:
				if( imgRatio>ratio ) scale = height/imgH;
				else scale = width/imgW;
				break;
		}
		
		imgW *= scale;
		imgH *= scale;
		ofRectangle rect = Helpers::alignBox(this, imgW, imgH, horizontalAlign, verticalAlign);
		ofSetColor( fg );
		ofEnableAlphaBlending();
		tex->draw(rect);
		ofDisableAlphaBlending();
	}
}


//--------------------------------------------------------------
void mui::Image::touchDown( ofTouchEventArgs &touch ){
}


//--------------------------------------------------------------
void mui::Image::touchMoved( ofTouchEventArgs &touch ){
}


//--------------------------------------------------------------
void mui::Image::touchUp( ofTouchEventArgs &touch ){
}


//--------------------------------------------------------------
void mui::Image::touchDoubleTap( ofTouchEventArgs &touch ){
}

//--------------------------------------------------------------
void mui::Image::autoHeight(){
	ofRectangle rect = getDrawRect();
	if( rect.width < width ){
		height = rect.height*width/rect.width;
	}
    //	height = -rect.y + rect.height;
}

//--------------------------------------------------------------
ofRectangle mui::Image::getDrawRect(){
	ofTexture * tex = NULL;
	
	if( texRef != NULL ){
		tex = texRef;
	}
	else if( this->tex.isAllocated() ){
		tex = &this->tex;
	}
	
	if( tex != NULL && tex->getWidth() > 0 && tex->getHeight() > 0 ){
		float imgW = tex->getWidth(), imgH = tex->getHeight();
		float imgRatio = imgW/imgH;
		float ratio = width/height;
		float scale = 1;
		
		switch( scaleMethod ){
			case ScaleMethodFitInside:
				if( imgRatio>ratio ) scale = width/imgW;
				else scale = height/imgH;
				break;
			case ScaleMethodCropToFill:
				if( imgRatio>ratio ) scale = height/imgH;
				else scale = width/imgW;
				break;
		}
		
		imgW *= scale;
		imgH *= scale;
		ofRectangle rect = Helpers::alignBox(this, imgW, imgH, horizontalAlign, verticalAlign);
		return rect;
	}
	else{
		return ofRectangle();
	}
}