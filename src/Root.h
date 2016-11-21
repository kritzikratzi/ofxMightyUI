/*
 *  Root.h
 *  ofxMightyUI
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
		
		Container * touchResponder[OF_MAX_TOUCHES]; // think of the responder as the elements received touchDown events.
		Container * keyboardResponder;
		set<Container*> hoverResponder;
		Container * popupMenu; 
		
		virtual void init(); 
		virtual void handleUpdate();
		virtual void handleDraw();
		
		// touch events
		virtual Container * handleTouchDown( ofTouchEventArgs &touch );
		virtual Container * handleTouchMoved( ofTouchEventArgs &touch );
		virtual Container * handleTouchHover( ofTouchEventArgs &touch );
		virtual Container * handleTouchUp( ofTouchEventArgs &touch );
		virtual Container * handleTouchDoubleTap( ofTouchEventArgs &touch );
		virtual Container * handleTouchCancelled( ofTouchEventArgs &touch );
		
		// keyboard events
		virtual Container * handleKeyPressed( ofKeyEventArgs &event );
		virtual Container * handleKeyReleased( ofKeyEventArgs &event );
		
		// mouse event forwards (mouse events get converted to touch events)
		virtual Container * handleMouseMoved( float x, float y );
		virtual Container * handleMouseDragged( float x, float y, int button );
		virtual Container * handleMousePressed( float x, float y, int button );
		virtual Container * handleMouseReleased( float x, float y, int button );
		
		// shows a container as a popup menu
		virtual void showPopupMenu( mui::Container * popupMenu, mui::Container * source, float x, float y, mui::HorizontalAlign horizontalAlign = mui::Left, mui::VerticalAlign verticalAlign = mui::Top );
		
		
		ofRectangle convertNativeToMui( const ofRectangle rect );
		ofPoint convertNativeToMui( const ofPoint pt );
		ofRectangle convertMuiToNative( const ofRectangle rect );
		ofPoint convertMuiToNative( const ofPoint pt );
		
		virtual bool becomeTouchResponder( Container * container, ofTouchEventArgs &touch );
		virtual bool becomeKeyboardResponder( Container * container );
		virtual void showTextField( TextField * tf );
		virtual void hideTextFields();
		virtual void safeRemoveAndDelete( Container * c ); 
		virtual void safeRemove( Container * c );
		virtual void removeFromResponders( Container * c ); 
		
		virtual void reloadTextures();

		virtual void prepareAnimation( int milliseconds, int type = tween::QUAD, int direction = tween::EASE_OUT ); 
		virtual void animate( float &variable, float targetValue ); 
		virtual void commitAnimation();
		
		virtual void runOnUiThread(function<void()> func);
		
		virtual bool getKeyPressed( int key ); 
		
		static mui::Root * INSTANCE;
		
	private:
		void fixTouchPosition( ofVec2f &touch, ofVec2f &copy, Container * c );
        void handleRemovals(); 
        
		tween::TweenerParam param;
		tween::Tweener tweener;
        
        vector<Container*> safeRemoveAndDeleteList; 
        vector<Container*> safeRemoveList;
		
		virtual void of_setup( ofEventArgs &args );
		virtual void of_update( ofEventArgs &args );
		virtual void of_draw( ofEventArgs &args );
		virtual void of_exit( ofEventArgs &args );
		virtual void of_windowResized( ofResizeEventArgs &args );
		virtual bool of_keyPressed( ofKeyEventArgs &args );
		virtual bool of_keyReleased( ofKeyEventArgs &args );
		virtual bool of_mouseMoved( ofMouseEventArgs &args );
		virtual bool of_mouseDragged( ofMouseEventArgs &args );
		virtual bool of_mousePressed( ofMouseEventArgs &args );
		virtual bool of_mouseReleased( ofMouseEventArgs &args );
		virtual bool of_mouseScrolled( ofMouseEventArgs &args );
		virtual bool of_touchDown( ofTouchEventArgs &args );
		virtual bool of_touchUp( ofTouchEventArgs &args );
		virtual bool of_touchMoved( ofTouchEventArgs &args );
		virtual bool of_touchDoubleTap( ofTouchEventArgs &args );
		virtual bool of_touchCancelled( ofTouchEventArgs &args );
		virtual void of_messageEvent( ofMessage &args );
		virtual bool of_fileDragEvent( ofDragInfo &args );
		
		void removePopupIfNecessary( mui::Container * target ); 
	};
}

