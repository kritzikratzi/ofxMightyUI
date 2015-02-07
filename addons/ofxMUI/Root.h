/*
 *  Root.h
 *  iPhoneEmptyExample
 *
 *  Created by hansi on 28.01.11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#define MUI_ROOT (mui::Root::INSTANCE)

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
		
		// touch events
		virtual Container * handleTouchDown( ofTouchEventArgs &touch );
		virtual Container * handleTouchMoved( ofTouchEventArgs &touch );
		virtual Container * handleTouchUp( ofTouchEventArgs &touch );
		virtual Container * handleTouchDoubleTap( ofTouchEventArgs &touch );
		virtual Container * handleTouchCancelled( ofTouchEventArgs &touch );
		
		// keyboard events
		virtual Container * handleKeyPressed( int key );
		virtual Container * handleKeyReleased( int key );
		
		// mouse event forwards (mouse events get converted to touch events)
		virtual Container * handleMouseMoved( int x, int y );
		virtual Container * handleMouseDragged( int x, int y, int button );
		virtual Container * handleMousePressed( int x, int y, int button );
		virtual Container * handleMouseReleased( int x, int y, int button );
		
		virtual bool becomeResponder( Container * container, ofTouchEventArgs &touch );
		virtual void showTextField( TextField * tf );
		virtual void hideTextFields();
		virtual void safeRemoveAndDelete( Container * c ); 
		virtual void safeRemove( Container * c );
		virtual void removeFromResponders( Container * c ); 
		
		virtual void reloadTextures();

		virtual void prepareAnimation( int milliseconds, int type = tween::QUAD, int direction = tween::EASE_OUT ); 
		virtual void animate( float &variable, float targetValue ); 
		virtual void commitAnimation();
		
		static mui::Root * INSTANCE;
		
	private: 
		Container * handle( ofTouchEventArgs &touch, PrimaryHandler handler, SecondaryHandler secondaryHandler ); 
		void fixTouchPosition( ofTouchEventArgs &touch, ofTouchEventArgs &copy, Container * c ); 
        void handleRemovals(); 
        
		tween::TweenerParam param;
		tween::Tweener tweener;
        
        vector<Container*> safeRemoveAndDeleteList; 
        vector<Container*> safeRemoveList; 
	};
}

