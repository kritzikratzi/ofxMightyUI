/*
 *  Root.h
 *  iPhoneEmptyExample
 *
 *  Created by hansi on 28.01.11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef MUI_ROOT
#define MUI_ROOT

namespace mui{
	class TextField; 
	
	class Root : public Container {
	public: 
		Root() : Container( 0, 0, ofGetWidth(), ofGetHeight() ){ INSTANCE = this; init(); };
		
		virtual void init(); 
		virtual void handleDraw(); 
		virtual bool handleTouchDown( ofTouchEventArgs &touch );
		virtual bool handleTouchMoved( ofTouchEventArgs &touch );
		virtual bool handleTouchUp( ofTouchEventArgs &touch );
		virtual bool handleTouchDoubleTap( ofTouchEventArgs &touch );
		
		virtual void showTextField( TextField * tf );
		
		static mui::Root * INSTANCE;
	};
}


#endif
