/*
 *  EmptyContainer.h
 *  ofxMightyUI
 *  
 *  Use this as a starting point for a new element! 
 * 
 *  Created by hansi on 29.01.11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef MUI_EMPTY_CONTAINER
#define MUI_EMPTY_CONTAINER

#include "MuiCore.h"

namespace mui{
	class EmptyContainer : public Container{
	public: 
		EmptyContainer( float x_ = 0, float y_ = 0, float width_ = 200, float height_ = 20 );
		
		virtual void update();
		virtual void draw();
		virtual void drawBackground();
		
		virtual void touchDown( ofTouchEventArgs &touch ); 
		virtual void touchMoved( ofTouchEventArgs &touch ); 
		virtual void touchUp( ofTouchEventArgs &touch ); 
		virtual void touchDoubleTap( ofTouchEventArgs &touch ); 
		
	private: 
	};
};

#endif