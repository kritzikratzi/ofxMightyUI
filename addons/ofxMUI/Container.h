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
	class Layout; 
	class Container{
	public: 
		float x;
		float y;
		float width;
		float height;
		
		ofColor fg;
		ofColor bg;
		
		bool opaque; 
		bool visible; 
		bool ignoreEvents; // false by default. if set to true this thing will never receive events, children of this container however will still receive events. 
		bool singleTouch; // true by default. if set to true a container will remember the first finger touching down and then discard all other events.
		int singleTouchId; // either -1 or the id of the active touch. take this into account for single-touch containers before injecting events (i.e. you shouldn't ever really need this)
		bool focusTransferable; // can the focus travel on to another parent?
		bool allowSubpixelTranslations; // translate by subpixels also? (default)
		bool drawDirty; // force drawing clipped objects in scrollpanes. 
		string name;
		
		vector<mui::Container*> children;
		Container * parent; 
		Layout * layoutManager; 
		
		//bool startedInside[OF_MAX_TOUCHES]; // don't use this. unless you're you really want to. 
		
		Container( float x_, float y_, float width_ = 10, float height_ = 10 ) : 
		x(x_), y(y_), width(width_), height(height_), opaque(false), parent(NULL), layoutManager(NULL), visible(true), ignoreEvents(false), singleTouch(true), name( "" ), singleTouchId( -1 ), focusTransferable(true),bg(0,0,0,0), allowSubpixelTranslations(true), drawDirty(false){
			//for( int i = 0; i < OF_MAX_TOUCHES; i++ ){
			//	startedInside[i] = false; 
			//}
			static int num = 0;
			this->name = "Container-" + ofToString(++num);
		};
		~Container();
		
		void add( Container * c, int index = -1 ); 
		void remove( Container * c ); 
		void remove(); 
        
		virtual void update(){};
		virtual void draw(){};
		virtual void drawBackground(); 
		
		virtual void layout(); 
		virtual void handleDraw(); 
		virtual void handleUpdate();
		
		virtual void touchDown( ofTouchEventArgs &touch ){};
		virtual void touchMoved( ofTouchEventArgs &touch ){};
		virtual void touchMovedOutside( ofTouchEventArgs &touch ){}
		virtual void touchUp( ofTouchEventArgs &touch ){}; 
		virtual void touchUpOutside( ofTouchEventArgs &touch ){}
		virtual void touchDoubleTap( ofTouchEventArgs &touch ){};
		virtual void touchCanceled( ofTouchEventArgs &touch ){}; // when some other component "stole" the responder status. 
		
		virtual Container * handleTouchDown( ofTouchEventArgs &touch );
		virtual Container * handleTouchMoved( ofTouchEventArgs &touch );
		virtual Container * handleTouchUp( ofTouchEventArgs &touch );
		virtual Container * handleTouchDoubleTap( ofTouchEventArgs &touch );

		virtual Container * getContainer( float x, float y );
		
		virtual bool hasFocus();
		virtual bool hasFocus( ofTouchEventArgs &touch );
		virtual ofPoint getGlobalPosition();
		virtual string toString(); 
	private: 
	};
};

#endif