/*
 *  Root.h
 *  iPhoneEmptyExample
 *
 *  Created by hansi on 28.01.11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "MuiCore.h"

#define MUI_ROOT (mui::Root::INSTANCE)

namespace mui{
	class TextField; 
	class Root : public Container {
	public: 
		Root();
		
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
		void fixTouchPosition( ofTouchEventArgs &touch, ofTouchEventArgs &copy, Container * c );
        void handleRemovals(); 
        
		tween::TweenerParam param;
		tween::Tweener tweener;
        
        vector<Container*> safeRemoveAndDeleteList; 
        vector<Container*> safeRemoveList;
		
		virtual void of_setup( ofEventArgs &args );
		virtual void of_update( ofEventArgs &args );
		virtual void of_draw( ofEventArgs &args );
		virtual void of_exit( ofEventArgs &args );
		virtual void of_windowEntered( ofEntryEventArgs &args );
		virtual void of_windowResized( ofResizeEventArgs &args );
		virtual bool of_keyPressed( ofKeyEventArgs &args );
		virtual bool of_keyReleased( ofKeyEventArgs &args );
		virtual bool of_mouseMoved( ofMouseEventArgs &args );
		virtual bool of_mouseDragged( ofMouseEventArgs &args );
		virtual bool of_mousePressed( ofMouseEventArgs &args );
		virtual bool of_mouseReleased( ofMouseEventArgs &args );
		virtual void of_audioReceived( ofAudioEventArgs &args );
		virtual void of_audioRequested( ofAudioEventArgs &args );
		virtual bool of_touchDown( ofTouchEventArgs &args );
		virtual bool of_touchUp( ofTouchEventArgs &args );
		virtual bool of_touchMoved( ofTouchEventArgs &args );
		virtual bool of_touchDoubleTap( ofTouchEventArgs &args );
		virtual bool of_touchCancelled( ofTouchEventArgs &args );
		virtual void of_messageEvent( ofMessage &args );
		virtual void of_fileDragEvent( ofDragInfo &args );
	};
}

