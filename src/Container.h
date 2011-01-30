/*
 *  Node.h
 *  iPhoneEmptyExample
 *
 *  Created by hansi on 28.01.11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef MUI_NODE
#define MUI_NODE

#include "ofMain.h"
#include "ofxiPhone.h"
#include <vector>


namespace mui{
	class Container : public ofxMultiTouchListener{
	public: 
		float x;
		float y;
		float width;
		float height;
		
		ofColor fg;
		ofColor bg;
		
		bool opaque; 
		vector<mui::Container*> aaa;
		
		Container( float x_, float y_, float width_, float height_ ) : x(x_), y(y_), width(width_), height(height_), opaque(false){};
		~Container(){}
		
		void add( Container * c ); 
		
		virtual void update(){};
		virtual void draw(){};
		virtual void drawBackground(); 
		
		virtual void handleDraw(); 
		virtual void handleUpdate();
		
		virtual bool touchDown( ofTouchEventArgs &touch ){ return false; }; 
		virtual bool touchMoved( ofTouchEventArgs &touch ){ return false; }; 
		virtual bool touchUp( ofTouchEventArgs &touch ){ return false; }; 
		virtual bool touchDoubleTap( ofTouchEventArgs &touch ){ return false; };
		
		virtual bool handleTouchDown( ofTouchEventArgs &touch );
		virtual bool handleTouchMoved( ofTouchEventArgs &touch );
		virtual bool handleTouchUp( ofTouchEventArgs &touch );
		virtual bool handleTouchDoubleTap( ofTouchEventArgs &touch );
		
	private: 
		bool startedInside[OF_MAX_TOUCHES];
	};
};

#endif