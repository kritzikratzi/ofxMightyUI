/*
 *  Root.cpp
 *  ofxMightyUI
 *
 *  Created by hansi on 28.01.11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "Root.h"
#include "ofEventUtils.h"
#include "ofEvents.h"
#include "Container.h"
#include "ScrollPane.h"

using namespace mui;

// TODO: the handleXX functions might return null, even if touchMovedOutside and touchUpOutside 
//       delegated to containers. this shouldn't be the case. 


mui::Root * mui::Root::INSTANCE = NULL;

//--------------------------------------------------------------
mui::Root::Root() : Container( 0, 0, -1, -1 ){
	INSTANCE = this;
	ignoreEvents = true;
	keyboardResponder = nullptr;
	popupMenu = nullptr;
	init();
};

//--------------------------------------------------------------
void mui::Root::init(){
	#if TARGET_OS_IPHONE
	NativeIOS::init();
//	#elif TARGET_OS_MAC
//	NativeOSX::init();
	#endif
	
	name = "Root"; 
	width = ofGetWidth()/mui::MuiConfig::scaleFactor;
	height = ofGetHeight()/mui::MuiConfig::scaleFactor;
	
	ofAddListener( ofEvents().setup, this, &mui::Root::of_setup, OF_EVENT_ORDER_AFTER_APP );
	ofAddListener( ofEvents().update, this, &mui::Root::of_update, OF_EVENT_ORDER_AFTER_APP );
	ofAddListener( ofEvents().draw, this, &mui::Root::of_draw, OF_EVENT_ORDER_AFTER_APP );
	//ofAddListener( ofEvents().exit, this, &mui::Root::of_exit );
	ofAddListener( ofEvents().windowResized, this, &mui::Root::of_windowResized, OF_EVENT_ORDER_BEFORE_APP );
	ofAddListener( ofEvents().keyPressed, this, &mui::Root::of_keyPressed, OF_EVENT_ORDER_BEFORE_APP );
	ofAddListener( ofEvents().keyReleased, this, &mui::Root::of_keyReleased, OF_EVENT_ORDER_BEFORE_APP );
	ofAddListener( ofEvents().mouseMoved, this, &mui::Root::of_mouseMoved, OF_EVENT_ORDER_BEFORE_APP );
	ofAddListener( ofEvents().mouseDragged, this, &mui::Root::of_mouseDragged, OF_EVENT_ORDER_BEFORE_APP );
	ofAddListener( ofEvents().mousePressed, this, &mui::Root::of_mousePressed, OF_EVENT_ORDER_BEFORE_APP );
	ofAddListener( ofEvents().mouseReleased, this, &mui::Root::of_mouseReleased, OF_EVENT_ORDER_BEFORE_APP );
	ofAddListener( ofEvents().mouseScrolled, this, &mui::Root::of_mouseScrolled, OF_EVENT_ORDER_BEFORE_APP );
	ofAddListener( ofEvents().touchDown, this, &mui::Root::of_touchDown, OF_EVENT_ORDER_BEFORE_APP );
	ofAddListener( ofEvents().touchUp, this, &mui::Root::of_touchUp, OF_EVENT_ORDER_BEFORE_APP );
	ofAddListener( ofEvents().touchMoved, this, &mui::Root::of_touchMoved, OF_EVENT_ORDER_BEFORE_APP );
	ofAddListener( ofEvents().touchDoubleTap, this, &mui::Root::of_touchDoubleTap, OF_EVENT_ORDER_BEFORE_APP );
	ofAddListener( ofEvents().touchCancelled, this, &mui::Root::of_touchCancelled, OF_EVENT_ORDER_BEFORE_APP );
	//ofAddListener( ofEvents().messageEvent, this, &mui::Root::of_messageEvent );
	ofAddListener( ofEvents().fileDragEvent, this, &mui::Root::of_fileDragEvent, OF_EVENT_ORDER_BEFORE_APP );
	
	
	// this seems unclear ... let's better put this in place!
	for( int i = 0; i < OF_MAX_TOUCHES; i++ ){
		touchResponder[i] = NULL;
	}
}

void mui::Root::handleUpdate(){
	int _width = ofGetWidth()/mui::MuiConfig::scaleFactor;
	int _height = ofGetHeight()/mui::MuiConfig::scaleFactor;

	if( width != _width || height != _height ){
		width = _width;
		height = _height;
		handleLayout();
		cout << "updating width width = " << width << ", height = " << height << endl;
	}
	else if( numLayoutFrames > 0 ){
		numLayoutFrames--; 
		handleLayout();
		cout << "updating width width = " << width << ", height = " << height << endl;
	}
	
	tweener.step( ofGetSystemTime() );
	
	// figure out where we are hovering
	
	set<Container*> newHoverResponder;
	Container * c = findChildAt(ofGetMouseX()/mui::MuiConfig::scaleFactor, ofGetMouseY()/mui::MuiConfig::scaleFactor,true,true);
	if(c){
		ofMouseEventArgs args; //TODO: fix up coords
		while(c!=nullptr){
			if(!c->ignoreEvents){
				newHoverResponder.insert(c);
				if(hoverResponder.find(c) == hoverResponder.end() ){
					c->mouseEnter(args);
					c->onMouseEnter.notify(args);
				}
			}
			
			c = c->parent;
		}
	}
	
	for( Container * c : hoverResponder ){
		ofMouseEventArgs args; //TODO: fix up coords
		if(newHoverResponder.find(c) == newHoverResponder.end()){
			c->mouseExit(args);
			c->onMouseExit.notify(args);
		}
	}
	
	hoverResponder = move(newHoverResponder); 
	
	#if TARGET_OS_IPHONE
	NativeIOS::update();
//	#elif TARGET_OS_MAC
//	NativeOSX::init();
	#endif
	Container::handleUpdate();
    
    handleRemovals();
}

//--------------------------------------------------------------
void mui::Root::handleDraw(){
	ofSetupScreenOrtho(); 
	ofPushStyle();
	ofScale( mui::MuiConfig::scaleFactor, mui::MuiConfig::scaleFactor, mui::MuiConfig::scaleFactor );
	ofFill(); 
	ofSetLineWidth( 1 ); 
	ofSetColor( 255, 255, 255 ); 
	ofEnableAlphaBlending();
	
	Container::handleDraw();
	
    handleRemovals();
	
	if( mui::MuiConfig::debugDraw ){
		mui::Container * active = this->findChildAt( ofGetMouseX()/mui::MuiConfig::scaleFactor - this->x, ofGetMouseY()/mui::MuiConfig::scaleFactor-this->y, true );
		if( active != NULL ){
			ofPoint p = active->getGlobalPosition();
			ofPushMatrix();
			ofFill();
			string name;
			mui::Container * c = active;
			string size;
			while( c != NULL  ){
				bool empty = c->name == ""; 
				auto n = empty ? (typeid(*c).name()) : c->name;
				name = (empty?"::":"") + n + (name==""?"":">") + name;
				c = c->parent;
			}

			name = name + " [" + typeid(*active).name() + "]"; 
			
			ofRectangle b = active->getGlobalBounds();
			stringstream info; 
			info << "Pos:" << b.x << "," << b.y << "  " << b.width << " x " << b.height << " / ";
			info << "Rel:" << active->x << "," << active->y;
			size = info.str();
			
			
			ofxFontStashStyle style = mui::Helpers::getStyle(10);
			ofNoFill();
			ofSetColor( 255,255,0 );
			ofDrawRectangle( p.x, p.y, active->width, active->height );
			ofSetColor(255);
			ofFill();
			style.color = ofColor(0);
			style.blur = 5;
			for(int i = 0; i < 10; i++){
				mui::Helpers::getFontStash().draw(name, style, p.x, p.y+10);
				mui::Helpers::getFontStash().draw(size, style, p.x, p.y+20);
			}
			style.color = ofColor(255);
			style.blur = 0;
			mui::Helpers::getFontStash().draw(name, style, p.x, p.y+10);
			mui::Helpers::getFontStash().draw(size, style, p.x, p.y+20);
			ofPopMatrix();
		}
	}
	
	ofPopStyle();
}



//--------------------------------------------------------------
mui::Container * mui::Root::handleTouchDown( ofTouchEventArgs &touch ){
	#if TARGET_OS_IPHONE
	NativeIOS::hide();
//	#elif TARGET_OS_MAC
//	NativeOSX::hide();
	#endif
	
	ofTouchEventArgs copy = touch; 
	fixTouchPosition( touch, copy, NULL ); 
	
	//return ( touchResponder[touch.id] = Container::handleTouchDown( copy ) );
	Container * lastPopup = popupMenu;
	touchResponder[touch.id] = nullptr;
	touchResponder[touch.id] = Container::handleTouchDown( copy );
	if( touchResponder[touch.id] != keyboardResponder ) keyboardResponder = NULL;
	if( popupMenu == lastPopup ) removePopupIfNecessary(touchResponder[touch.id]);
	
	return touchResponder[touch.id]; 
}


//--------------------------------------------------------------
mui::Container * mui::Root::handleTouchMoved( ofTouchEventArgs &touch ){
	ofTouchEventArgs copy = touch;
	fixTouchPosition( touch, copy, NULL );
	Container * touched = Container::handleTouchMoved( copy );
	
	if( touched != touchResponder[touch.id] && touchResponder[touch.id] != NULL ){
		copy = touch;
		fixTouchPosition( touch, copy, NULL );
		copy = Helpers::translateTouch( copy, this, touchResponder[touch.id] );
		touchResponder[touch.id]->touchMovedOutside( copy );
		touchResponder[touch.id]->onTouchMovedOutside.notify(copy);
		
		return touchResponder[touch.id];
	}
	
	return touched;
}



//--------------------------------------------------------------
mui::Container * mui::Root::handleTouchHover( ofTouchEventArgs &touch ){
	ofTouchEventArgs copy = touch;
	fixTouchPosition( touch, copy, NULL );
	Container * touched = Container::handleTouchHover( copy );
	
	if( touched != touchResponder[touch.id] && touchResponder[touch.id] != NULL ){
		copy = touch;
		fixTouchPosition( touch, copy, NULL );
		copy = Helpers::translateTouch( copy, this, touchResponder[touch.id] );
		touchResponder[touch.id]->touchMovedOutside( copy );
		touchResponder[touch.id]->onTouchMovedOutside.notify(copy);
		return touchResponder[touch.id];
	}
	
	return touched;
}


//--------------------------------------------------------------
mui::Container * mui::Root::handleTouchUp( ofTouchEventArgs &touch ){
	ofTouchEventArgs copy = touch; 
	fixTouchPosition( touch, copy, NULL ); 
	Container * touched = Container::handleTouchUp( copy ); 
	
	if( touched != touchResponder[touch.id] && touchResponder[touch.id] != NULL ){
		fixTouchPosition( touch, copy, touchResponder[touch.id] );
		Container *c = touchResponder[touch.id];
		touchResponder[touch.id]->touchUpOutside( copy );
		touchResponder[touch.id]->onTouchUpOutside.notify( copy );
		c->singleTouchId = -1;
	}
	
    touchResponder[touch.id] = NULL; 
    
	return touched; 	
}


//--------------------------------------------------------------
mui::Container * mui::Root::handleTouchDoubleTap( ofTouchEventArgs &touch ){
	ofTouchEventArgs copy = touch; 
	fixTouchPosition( touch, copy, NULL ); 

	return Container::handleTouchDoubleTap( copy ); 
}

//--------------------------------------------------------------
mui::Container * mui::Root::handleTouchCancelled( ofTouchEventArgs &touch ){
	if( touchResponder[touch.id] != NULL ){
		touchResponder[touch.id]->touchCanceled( touch );
		touchResponder[touch.id]->singleTouchId = -1;
		mui::Container * c = touchResponder[touch.id];
		touchResponder[touch.id] = NULL;
		return c;
	}
	else{
		return NULL;
	}
}



//--------------------------------------------------------------
void mui::Root::fixTouchPosition( ofVec2f &touch, ofVec2f &copy, Container * container ){
	copy.x = touch.x/mui::MuiConfig::scaleFactor;
	copy.y = touch.y/mui::MuiConfig::scaleFactor;
	
	if( container != NULL ){
		ofPoint pos = container->getGlobalPosition();
		copy.x -= pos.x;
		copy.y -= pos.y;
	}
}




//--------------------------------------------------------------
void mui::Root::showTextField( TextField * tf ){
	#if TARGET_OS_IPHONE
	NativeIOS::showTextField( tf );
//	#elif TARGET_OS_MAC
//	NativeOSX::showTextField( tf );
	#endif
}

void mui::Root::hideTextFields(){
    #if TARGET_OS_IPHONE
    NativeIOS::hide();
//	#elif TARGET_OS_MAC
//	NativeOSX::hide();
    #endif
}

//--------------------------------------------------------------
ofRectangle mui::Root::convertNativeToMui( const ofRectangle rect ){
	const float s = mui::MuiConfig::scaleFactor;
	return ofRectangle(rect.x/s, rect.y/s, rect.width/s, rect.height/s);
}
//--------------------------------------------------------------
ofPoint mui::Root::convertNativeToMui( const ofPoint pt ){
	const float s = mui::MuiConfig::scaleFactor;
	return ofPoint(pt.x/s, pt.y/s, pt.z/s);
}
//--------------------------------------------------------------
ofRectangle mui::Root::convertMuiToNative( const ofRectangle rect ){
	const float s = mui::MuiConfig::scaleFactor;
	return ofRectangle(rect.x*s, rect.y*s, rect.width*s, rect.height*s);
}

//--------------------------------------------------------------
ofPoint mui::Root::convertMuiToNative( const ofPoint pt ){
	const float s = mui::MuiConfig::scaleFactor;
	return ofPoint(pt.x*s, pt.y*s, pt.z*s);
}



//--------------------------------------------------------------
bool mui::Root::becomeTouchResponder( Container * c, ofTouchEventArgs &touch ){
	// the trivial case ...
	if( c != NULL && c == touchResponder[touch.id] )
		return true;
	
	// notify previous owner,
	// cancel if it doesn't allow transfering focus
	if( touchResponder[touch.id] != NULL ){
        if( touchResponder[touch.id]->focusTransferable == false )
            return false; 
        
		touchResponder[touch.id]->handleTouchCanceled( touch );
		touchResponder[touch.id]->singleTouchId = -1; 
	}
	
	// alright, install new owner
	touchResponder[touch.id] = c;
	if( touchResponder[touch.id] != NULL ){
		touchResponder[touch.id]->singleTouchId = touch.id;
	}

	
	return true; 
}

bool mui::Root::becomeKeyboardResponder( Container * c ){
	this->keyboardResponder = c;
	return true;
}


//--------------------------------------------------------------
void mui::Root::safeRemove( Container * c ){
    safeRemoveList.push_back( c ); 
}

//--------------------------------------------------------------
void mui::Root::safeRemoveAndDelete( mui::Container *c ){
    safeRemoveAndDeleteList.push_back( c ); 
}

//--------------------------------------------------------------
void mui::Root::removeFromResponders( Container * c ){
	if(c == nullptr ) return;
	
	for( int i = 0; i < OF_MAX_TOUCHES; i++ ){
		if( touchResponder[i] == c ){
			touchResponder[i] = NULL;
		}
	}
	
	if( keyboardResponder == c ){
		keyboardResponder = NULL;
	}
	
	hoverResponder.erase(c);
	
	if(c == popupMenu ) popupMenu = nullptr;
	// recurse
	//for(const auto & child : c->children){
	//	removeFromResponders(child);
	//}
}

void mui::Root::reloadTextures(){
	mui::Helpers::clearCaches();
}



//--------------------------------------------------------------
void mui::Root::prepareAnimation( int milliseconds, int type, int direction ){
	param = tween::TweenerParam( milliseconds, (short)type, (short)direction );
}

//--------------------------------------------------------------
void mui::Root::runOnUiThread(function<void()> func){
	//TBD
}

//--------------------------------------------------------------
bool mui::Root::getKeyPressed( int key ){
	return ofGetKeyPressed(key);
}

//--------------------------------------------------------------
void mui::Root::animate( float &variable, float targetValue ){
    param.addProperty( &variable, targetValue ); 
}


//--------------------------------------------------------------
void mui::Root::commitAnimation(){
    tweener.addTween( param );
}

void mui::Root::handleRemovals(){
    vector<Container*>::iterator it = safeRemoveList.begin(); 
    while( it != safeRemoveList.end() ){
        (*it)->remove();
        ++it; 
    }
    
    it = safeRemoveAndDeleteList.begin(); 
    while( it != safeRemoveAndDeleteList.end() ){
        (*it)->remove();
        delete (*it); 
        ++it; 
    }
    
    safeRemoveList.clear(); 
    safeRemoveAndDeleteList.clear(); 
}

//--------------------------------------------------------------
mui::Container * mui::Root::handleKeyPressed( ofKeyEventArgs &event ){
	// this is a bit awkward, there seems to be a bug in
	// glfw that re-sends modifier key when they are released.
	// for now, i'm not fixing this. i really shouldn't be, not here.

	if( getKeyPressed(OF_KEY_ESC) && popupMenu != nullptr){
		if (keyboardResponder == nullptr || !keyboardResponder->onKeyPressed.notify(event) || !keyboardResponder->keyPressed(event)) {
			safeRemove(popupMenu);
			popupMenu = nullptr; 
		}
		return this;
	}
	
	if( mui::MuiConfig::enableDebuggingShortcuts && getKeyPressed(MUI_KEY_ACTION) && event.keycode == GLFW_KEY_D ){
		mui::MuiConfig::debugDraw ^= true;
		return this;
	}
	
	if( mui::MuiConfig::enableDebuggingShortcuts && getKeyPressed(OF_KEY_ALT) && event.keycode == OF_KEY_RETURN){
		// dump the view hierachy!
		mui::Container * active = this->findChildAt( ofGetMouseX()/mui::MuiConfig::scaleFactor - this->x, ofGetMouseY()/mui::MuiConfig::scaleFactor-this->y, true );
		
		cout << "------------------------------------";
		cout << "DUMPING VIEW HIERACHY" << endl;
		while( active != NULL ){
			ofRectangle b = active->getBounds();
			stringstream info; 
			info << "Pos:" << b.x << "," << b.y << "  " << b.width << " x " << b.height;
			string size = info.str(); 
			
			cout << "> " << active->name << size << endl;
			active = active->parent;
		}
		cout << "------------------------------------";
		
		return this;
	}

	if( mui::MuiConfig::enableDebuggingShortcuts && getKeyPressed(OF_KEY_ALT) && event.keycode == 'I' ){
		cout << "------------------------------------" << endl;
		mui::Container * active = this->findChildAt( ofGetMouseX()/mui::MuiConfig::scaleFactor - this->x, ofGetMouseY()/mui::MuiConfig::scaleFactor-this->y, true );
		cout << "Set a debug point in " << __FILE__ << ":" << __LINE__ << " to inspect this element" << endl;
		cout << "------------------------------------" << endl;
		
		return this;
	}
	
	if( mui::MuiConfig::debugDraw && getKeyPressed(OF_KEY_ALT) && event.keycode == 'L' ){
		handleLayout();
		
		return this; 
	}
	
	if( keyboardResponder != NULL ){
		if( !keyboardResponder->isVisibleOnScreen()){
			keyboardResponder = NULL;
		}
		else{
			mui::Container * c = keyboardResponder;
			while(c != nullptr && !c->onKeyPressed.notify(event) && !c->keyPressed(event)){
				c = c->parent;
			}
			return c;
		}
	}
	
	return keyboardResponder;
}

//--------------------------------------------------------------
mui::Container * mui::Root::handleKeyReleased( ofKeyEventArgs &event ){
	if( keyboardResponder != NULL ){
		if(keyboardResponder->onKeyReleased.notify(event)) return keyboardResponder;
		keyboardResponder->keyReleased(event);
	}
	
	return keyboardResponder;
}

//--------------------------------------------------------------
mui::Container * mui::Root::handleMouseMoved( float x, float y ){
	ofTouchEventArgs args;
	args.x = x;
	args.y = y;
	args.id = 0;
	return handleTouchHover(args);
}

//--------------------------------------------------------------
mui::Container * mui::Root::handleMouseDragged( float x, float y, int button ){
	ofTouchEventArgs args;
	args.x = x;
	args.y = y;
	args.id = 0;
	return handleTouchMoved(args);
}

//--------------------------------------------------------------
mui::Container * mui::Root::handleMousePressed( float x, float y, int button ){
	ofTouchEventArgs args;
	args.x = x;
	args.y = y;
	args.id = 0;
	return handleTouchDown(args);
}

//--------------------------------------------------------------
mui::Container * mui::Root::handleMouseReleased( float x, float y, int button ){
	ofTouchEventArgs args;
	args.x = x;
	args.y = y;
	args.id = 0;
	return handleTouchUp(args);
}

void mui::Root::showPopupMenu( mui::Container * c, mui::Container * source, ofVec2f pos, mui::HorizontalAlign horizontalAlign, mui::VerticalAlign verticalAlign ){
	mui::Root::showPopupMenu(c, source, pos.x, pos.y, horizontalAlign, verticalAlign );
}

void mui::Root::showPopupMenu( mui::Container * c, mui::Container * source, float x, float y, mui::HorizontalAlign horizontalAlign, mui::VerticalAlign verticalAlign ){
	if(popupMenu != nullptr){
		popupMenu->visible = false;
		popupMenu->remove(); 
		popupMenu = nullptr;
	}
	
	if(c == nullptr) return;
	
	c->visible = true; 
	add(c);
	c->handleLayout();
	popupMenu = c;
	
	if( source == nullptr ){
		popupMenu->x = x;
		popupMenu->y = y;
	}
	else{
		ofPoint p = source->getGlobalPosition();
		popupMenu->x = p.x + x;
		popupMenu->y = p.y + y;
	}
	
	switch(horizontalAlign){
		case mui::Left: break;
		case mui::Right: popupMenu->x -= popupMenu->width; break;
		case mui::Center: popupMenu->x -= popupMenu->width/2; break;
	}
	switch(verticalAlign){
		case mui::Top: break;
		case mui::Middle: popupMenu->y -= popupMenu->height; break;
		case mui::Bottom: popupMenu->y -= popupMenu->height/2; break;
	}
	
	popupMenu->x = ofClamp(popupMenu->x, 1, width - popupMenu->width );
	popupMenu->y = ofClamp(popupMenu->y, 1, height - popupMenu->height );
}



void mui::Root::of_setup( ofEventArgs &args ){
	//handleSetup();
	handleLayout(); 
}
void mui::Root::of_update( ofEventArgs &args ){
	if(mui::MuiConfig::detectRetina){
		auto ptr = ofGetWindowPtr();
		auto glfw = dynamic_cast<ofAppGLFWWindow*>(ptr);
		if(glfw && mui::MuiConfig::scaleFactor != glfw->getPixelScreenCoordScale() ){
			mui::MuiConfig::scaleFactor = glfw->getPixelScreenCoordScale();
			mui::Helpers::getFontStash().pixelDensity = mui::MuiConfig::scaleFactor;
			cout << "[ofxMightyUI] updated pixel scaling factor to " << mui::MuiConfig::scaleFactor << endl;
		}
	}
	handleUpdate();
}
void mui::Root::of_draw( ofEventArgs &args ){
	handleDraw();
}
void mui::Root::of_exit( ofEventArgs &args ){
	//handleExit(args);
}
void mui::Root::of_windowResized( ofResizeEventArgs &args ){
	//handleWindowResized(args);
	if (args.width <= 0 || args.height <= 0) return; 
	width = args.width/mui::MuiConfig::scaleFactor;
	height = args.height/mui::MuiConfig::scaleFactor;
	numLayoutFrames = 1;
	handleLayout();
}
bool mui::Root::of_keyPressed( ofKeyEventArgs &args ){
	return handleKeyPressed(args) != NULL;
}
bool mui::Root::of_keyReleased( ofKeyEventArgs &args ){
	return handleKeyReleased(args) != NULL;
}
bool mui::Root::of_mouseMoved( ofMouseEventArgs &args ){
	return handleMouseMoved(args.x, args.y) != NULL;
}
bool mui::Root::of_mouseDragged( ofMouseEventArgs &args ){
	return handleMouseDragged(args.x, args.y, args.button) != NULL;
}
bool mui::Root::of_mousePressed( ofMouseEventArgs &args ){
	return handleMousePressed(args.x, args.y, args.button) != NULL;
}
bool mui::Root::of_mouseReleased( ofMouseEventArgs &args ){
	return handleMouseReleased(args.x, args.y, args.button) != NULL;
}
bool mui::Root::of_mouseScrolled( ofMouseEventArgs &args ){
	ofVec2f pos;
	fixTouchPosition(args, pos, NULL);
	mui::Container * container = (mui::Container*)findChildOfType<mui::ScrollPane>(pos.x, pos.y, true, true);
	if( container != NULL ){
		container->mouseScroll(args);
		container->onMouseScroll.notify(args);
		return true;
	}
	else{
		mui::Container * container = findChildAt(pos.x, pos.y, true, true );
		if( container != nullptr && container != this ){
			container->mouseScroll(args);
			container->onMouseScroll.notify(args);
			return true;
		}
		else{
			return false;
		}
	}
}
bool mui::Root::of_touchDown( ofTouchEventArgs &args ){
	return handleTouchDown(args) != NULL;
}
bool mui::Root::of_touchUp( ofTouchEventArgs &args ){
	return handleTouchUp(args) != NULL;
}
bool mui::Root::of_touchMoved( ofTouchEventArgs &args ){
	return handleTouchMoved(args) != NULL;
}
bool mui::Root::of_touchDoubleTap( ofTouchEventArgs &args ){
	return handleTouchDoubleTap(args) != NULL;
}
bool mui::Root::of_touchCancelled( ofTouchEventArgs &args ){
	return handleTouchCancelled(args) != NULL;
}
void mui::Root::of_messageEvent( ofMessage &args ){
	//handleMessageEvent(args);
}
bool mui::Root::of_fileDragEvent( ofDragInfo &args ){
	ofDragInfo copy = args;
	copy.position.x = args.position.x/mui::MuiConfig::scaleFactor;
	copy.position.y = args.position.y/mui::MuiConfig::scaleFactor;
	
	return handleFileDragged(copy);
}

void mui::Root::removePopupIfNecessary( mui::Container * target ){
	if(popupMenu != nullptr ){
		if(target != nullptr ){
			// is the popup somehow a parent of what was clicked?
			while(target != nullptr){
				if( target == popupMenu ){
					return;
				}
				target = target->parent;
			}
		}
		
		popupMenu->visible = false;
		safeRemove(popupMenu);
		popupMenu = nullptr;
	}
}
