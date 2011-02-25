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
		bool visible; 
		vector<mui::Container*> children;
		Container * parent; 
		
		Container( float x_, float y_, float width_, float height_ ) : x(x_), y(y_), width(width_), height(height_), opaque(false), parent(NULL), visible(true) {
			for( int i = 0; i < OF_MAX_TOUCHES; i++ ){
				startedInside[i] = false; 
			}
		};
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
		
		virtual ofPoint getGlobalPosition(); 
		
	private: 
		bool startedInside[OF_MAX_TOUCHES];
	};
};

#endif