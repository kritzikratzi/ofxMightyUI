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
	typedef  Container* (Container::*PrimaryHandler)( ofTouchEventArgs& );
	typedef  void (Container::*SecondaryHandler)( ofTouchEventArgs& );

	
	class TextField; 
	class Root : public Container {
	public: 
		Root() : Container( 0, 0, ofGetWidth(), ofGetHeight() ){ INSTANCE = this; ignoreEvents = true; init(); };
		
		Container * respondingContainer[OF_MAX_TOUCHES]; // think of the responder as the elements received touchDown events.
		
		virtual void init(); 
		virtual void handleUpdate();
		virtual void handleDraw();
		virtual Container * handleTouchDown( ofTouchEventArgs &touch );
		virtual Container * handleTouchMoved( ofTouchEventArgs &touch );
		virtual Container * handleTouchUp( ofTouchEventArgs &touch );
		virtual Container * handleTouchDoubleTap( ofTouchEventArgs &touch );
		
		virtual bool becomeResponder( Container * container, ofTouchEventArgs &touch );
		virtual void showTextField( TextField * tf );
		
		
		virtual void prepareAnimation( int milliseconds, int type = tween::QUAD, int direction = tween::EASE_OUT ); 
		virtual void animate( float &variable, float targetValue ); 
		virtual void commitAnimation();
		
		static mui::Root * INSTANCE;
		
	private: 
		Container * handle( ofTouchEventArgs &touch, PrimaryHandler handler, SecondaryHandler secondaryHandler ); 
		void fixTouchPosition( ofTouchEventArgs &touch, ofTouchEventArgs &copy, Container * c ); 

		tween::TweenerParam param;
		tween::Tweener tweener;
	};
}


#endif
