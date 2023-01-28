/*
 *  Created by hansi on 28.01.11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "Container.h"
#include "MuiConfig.h"
#include "Root.h"


mui::Container::Container( float x, float y, float width, float height ) : x(x), y(y), width(width), height(height), opaque(false), parent(nullptr), layoutHandler(nullptr), visible(true), ignoreEvents(false), singleTouch(true), name( "" ), singleTouchId( -1 ), focusTransferable(true),bg(0,0,0,0), allowSubpixelTranslations(true), drawDirty(false), userData(nullptr),needsLayout(false),cursor(mui::Cursor::Default){
};

//--------------------------------------------------------------
mui::Container::~Container(){
	MUI_ROOT->removeFromResponders( this );
}


//--------------------------------------------------------------
void mui::Container::add( Container * c, int index ){
	if(c->parent != nullptr ){
		c->remove();
	}
	
    if( index == -1 ){
        children.push_back( c );
        c->parent = this;
		c->afterAdd(this);
		c->onAfterAdd.notify(this);
    }
    else{
        vector<Container*>::iterator it = children.begin();
        it += index;
        children.insert( it, c );
        c->parent = this;
		c->afterAdd(this);
		c->onAfterAdd.notify(this);
    }
}

//--------------------------------------------------------------
void mui::Container::remove( Container * c ){
	vector<Container*>::iterator it = find( children.begin(), children.end(), c );
	MUI_ROOT->removeFromResponders( c );
    if( it != children.end() ){
		c->parent = nullptr;
        children.erase( it );
		c->onAfterRemove.notify(this);
		c->afterRemove(this);
    }
}

//--------------------------------------------------------------
void mui::Container::remove(){
	MUI_ROOT->removeFromResponders( this );
    if( parent != NULL ){
        parent->remove( this );
    }
}

//--------------------------------------------------------------
void mui::Container::setChildren( std::vector<Container*> new_children_list){
	std::set<Container*> old_children_set(children.begin(),children.end());
	std::set<Container*> new_children_set(new_children_list.begin(),new_children_list.end());
	std::vector<Container*> old_children_list = children;
	children = new_children_list;
	
	for(Container * c : old_children_list){
		// removed from previous children
		if(new_children_set.find(c)==new_children_set.end()){
			MUI_ROOT->removeFromResponders( c );
			c->parent = nullptr;
			c->onAfterRemove.notify(this);
			c->afterRemove(this);
		}
	}
	
	for(Container * c : new_children_list){
		// added, and was not previously added?
		if(old_children_set.find(c)==old_children_set.end()){
			c->parent = this;
			c->afterAdd(this);
			c->onAfterAdd.notify(this);
		}
	}
}


ofRectangle mui::Container::getGlobalBounds(){
	return ofRectangle( getGlobalPosition(), width, height );
}

ofRectangle mui::Container::getBounds(){
	return ofRectangle(x, y, width, height);
}

ofRectangle mui::Container::getChildBounds(){
	// figure out min/max values...
	std::vector<Container*>::iterator it = children.begin();
	float minX, minY, maxX, maxY;
	
	minX = 0;
	minY = 0;
	maxX = 0;
	maxY = 0;
	
	while( it != children.end() ) {
		if( (*it)->visible ){
			minX = fminf( (*it)->x, minX );
			minY = fminf( (*it)->y, minX );
			maxX = fmaxf( (*it)->x + (*it)->width, maxX );
			maxY = fmaxf( (*it)->y + (*it)->height, maxY );
		}
		++it;
	}
	
	return ofRectangle( minX, minY, maxX - minX, maxY - minY );
}

void mui::Container::setBounds( float x, float y, float w, float h ){
	if(this->x!=x || this->y != y || this->width != w || this->height != h){
		this->x = x;
		this->y = y;
		this->width = w;
		this->height = h;
		handleLayout();
	}
}

void mui::Container::setBounds( ofRectangle rect ){
	setBounds(rect.x, rect.y, rect.width, rect.height );
}

//--------------------------------------------------------------
ofVec2f mui::Container::getSize(){
	return ofVec2f(width,height);
}

//--------------------------------------------------------------
void mui::Container::setSize(ofVec2f size){
	width = size.x;
	height = size.y;
}

//--------------------------------------------------------------
void mui::Container::setSize(float w, float h){
	width = w;
	height = h;
}

//--------------------------------------------------------------
ofVec2f mui::Container::getPosition(){
	return ofVec2f(x,y);
}

//--------------------------------------------------------------
void mui::Container::setPosition(ofVec2f position){
	x = position.x;
	y = position.y;
}

//--------------------------------------------------------------
void mui::Container::setPosition(float x, float y){
	this->x = x;
	this->y = y;
}


//--------------------------------------------------------------
void mui::Container::layout(){
	// layout managers might have been a fucking stupid idea.
	// or maybe not. this is definitely gonna change!
	if( layoutHandler != NULL ){
		layoutHandler->layout( this );
	}
}

void mui::Container::handleLayout(){
	// does this order make sense?
	int numPasses = 0;
rerun_layout:
	needsLayout = false;
	
	if( layoutHandler == NULL ){
		layout();
	}
	else{
		layoutHandler->layout(this);
	}
	onLayout.notify(this); 

	vector<mui::Container*>::iterator it = children.begin();
	bool sizeChanged = false;
	float oldW, oldH;
	while( it != children.end() ){
		oldW = (*it)->width;
		oldH = (*it)->height;
		(*it)->handleLayout();
		if( oldW != (*it)->width || oldH != (*it)->height ){
			sizeChanged = true;
		}
		++it;
	}
	
	
	if( sizeChanged || needsLayout ){
		numPasses ++;
		if( numPasses <= 3 ){
			goto rerun_layout;
		}
		else{
			ofLogWarning() << "[ofxMighty] Num layout passes seems high, layout possibly unstable. " << endl;
		}
	}
	
	needsLayout = false;
}


//--------------------------------------------------------------
void mui::Container::drawBackground(){
    if( bg.a > 0 ){
		ofFill();
        ofSetColor( bg );
        ofDrawRectangle( 0, 0, width, height );
        ofSetColor( 255 );
    }
}


//--------------------------------------------------------------
void mui::Container::handleDraw(){
	if( !visible ) return;
	
	ofPushMatrix();
	if( allowSubpixelTranslations ) ofTranslate( x, y );
	else ofTranslate( (int)x, (int)y );
	
	if( opaque ){
		drawBackground();
		onDrawBackground.notify(this);
	}
	
	if (needsLayout) handleLayout(); 

	draw();
	onDraw.notify(this);
	
	if( MUI_DEBUG_DRAW ){
		ofNoFill();
		ofSetColor( 255, 0, 0 );
		ofDrawRectangle( 0, 0, width, height );
		ofSetColor( 255 );
		ofFill();
	}
	
	
	std::vector<Container*>::iterator it = children.begin();
	while( it != children.end() ) {
        (*it)->handleDraw();
		++it;
	}
	
	onDrawAbove.notify(this); 
	
	ofPopMatrix();
}


//--------------------------------------------------------------
void mui::Container::handleUpdate(){
	update();
	onUpdate.notify(this);
	
	std::vector<Container*>::iterator it = children.begin();
	while( it != children.end() ) {
        //Container * child = (*it); // just for debugging ...
		(*it)->handleUpdate();
		++it;
	}
	
	if(needsLayout){
		handleLayout();
		needsLayout = false;
	}
}


//--------------------------------------------------------------
mui::Container * mui::Container::handleTouchDown( ofTouchEventArgs &touch ){
	if( !visible ) return NULL;
    
	
	//string depth = "";
	//Container * p = this;
	//while( p->parent != NULL ) p = p->parent, depth = "  " + depth;
	
	if( touch.x >= 0 && touch.x <= width && touch.y >= 0 && touch.y <= height ){
		float x, y;
		Container * touched;
		
		std::vector<Container*>::reverse_iterator it = children.rbegin();
		while( it != children.rend() ){
			touch.x -= ( x = (*it)->x );
			touch.y -= ( y = (*it)->y );
			touched = (*it)->handleTouchDown( touch );
			touch.x += x;
			touch.y += y;
			
			if( touched != NULL ){
				return touched;
			}
			
			++it;
		}
		
		if( !ignoreEvents ){
			if( singleTouchId == -1 ){
				singleTouchId = touch.id;
			}
			
			if( !singleTouch || ( singleTouch && singleTouchId == touch.id ) ){
				touchDown( touch );
				onTouchDown.notify(touch); 
			}
			
			if( MUI_ROOT->touchResponder[touch.id] != NULL ){
				return MUI_ROOT->touchResponder[touch.id];
			}
			else{
				return this;
			}
		}
	}
	
	return NULL;
}


//--------------------------------------------------------------
mui::Container * mui::Container::handleTouchMoved( ofTouchEventArgs &touch ){
	if( !visible ) return NULL;
	
	
	if( touch.x >= 0 && touch.x <= width && touch.y >= 0 && touch.y <= height ){
		float x, y;
		Container * touched;
		
		std::vector<Container*>::reverse_iterator it = children.rbegin();
		while( it != children.rend() ) {
			touch.x -= ( x = (*it)->x );
			touch.y -= ( y = (*it)->y );
			touched = (*it)->handleTouchMoved( touch );
			touch.x += x;
			touch.y += y;
			
			if( touched != NULL ){
				// that container is touched!
				return touched;
			}
			
			++it;
		}
		
		if( !ignoreEvents ){
			if( !singleTouch || ( singleTouch && singleTouchId == touch.id ) ){
				touchMoved( touch );
				onTouchMoved.notify(touch);
			}
			
			return this;
		}
	}
	
	return NULL;
}


//--------------------------------------------------------------
mui::Container * mui::Container::handleTouchHover( ofTouchEventArgs &touch ){
	if( !visible ) return NULL;
	
	
	if( touch.x >= 0 && touch.x <= width && touch.y >= 0 && touch.y <= height ){
		float x, y;
		Container * touched;
		
		std::vector<Container*>::reverse_iterator it = children.rbegin();
		while( it != children.rend() ) {
			touch.x -= ( x = (*it)->x );
			touch.y -= ( y = (*it)->y );
			touched = (*it)->handleTouchHover( touch );
			touch.x += x;
			touch.y += y;
			
			if( touched != NULL ){
				// that container is touched!
				return touched;
			}
			
			++it;
		}
		
		if( !ignoreEvents ){
			if( !singleTouch || singleTouchId == -1 || ( singleTouch && singleTouchId == touch.id ) ){
				touchHover( touch );
				onTouchHover.notify(touch);
			}
			
			return this;
		}
	}
	
	return NULL;
}


//--------------------------------------------------------------
mui::Container * mui::Container::handleTouchDoubleTap( ofTouchEventArgs &touch ){
	if( !visible ) return NULL;
	
	
	if( touch.x >= 0 && touch.x <= width && touch.y >= 0 && touch.y <= height ){
		float posX, posY;
		Container * touched;
		
		std::vector<Container*>::reverse_iterator it = children.rbegin();
		while( it != children.rend() ) {
			touch.x -= ( posX = (*it)->x );
			touch.y -= ( posY = (*it)->y );
			touched = (*it)->handleTouchDoubleTap( touch );
			touch.x += posX;
			touch.y += posY;
			
			if( touched != NULL ){
				// that container is touched!
				return touched;
			}
			
			++it;
		}
		
		if( !ignoreEvents ){
			if( !singleTouch || ( singleTouch && singleTouchId == touch.id ) ){
				onTouchDoubleTap.notify(touch);
				touchDoubleTap( touch );
			}
			
			return this;
		}
	}
	
	return NULL;
}


//--------------------------------------------------------------
mui::Container * mui::Container::handleTouchUp( ofTouchEventArgs &touch ){
	if( !visible ) return NULL;
	
	
	if( touch.x >= 0 && touch.x <= width && touch.y >= 0 && touch.y <= height ){
		float posX, posY;
		Container * touched;
		
		std::vector<Container*>::reverse_iterator it = children.rbegin();
		while( it != children.rend() ) {
			touch.x -= ( posX = (*it)->x );
			touch.y -= ( posY = (*it)->y );
			touched = (*it)->handleTouchUp( touch );
			touch.x += posX;
			touch.y += posY;
            
			if( touched != NULL ){
				// that container is touched!
				return touched;
			}
			
			++it;
		}
		
		if( !ignoreEvents ){
			if( !singleTouch || ( singleTouch && singleTouchId == touch.id ) ){
				touchUp( touch );
				onTouchUp.notify(touch);
				singleTouchId = -1;
			}
			
			return this;
		}
	}
	
	return NULL;
}

void mui::Container::handleTouchCanceled( ofTouchEventArgs &touch ){
	touchCanceled(touch);
	onTouchCanceled.notify(touch);
	if( parent != NULL ){
		parent->handleTouchCanceled(touch);
	}
}

bool mui::Container::handleFileDragged(ofDragInfo & info ){
	if( !visible ) return NULL;
	
	auto & touch = info.position;
	if( touch.x >= 0 && touch.x <= width && touch.y >= 0 && touch.y <= height ){
		float posX, posY;
		
		std::vector<Container*>::reverse_iterator it = children.rbegin();
		while( it != children.rend() ) {
			touch.x -= ( posX = (*it)->x );
			touch.y -= ( posY = (*it)->y );
			if( (*it)->handleFileDragged( info ) ) return true;
			touch.x += posX;
			touch.y += posY;
			
			++it;
		}
		
		if( !ignoreEvents ){
			if(fileDragged(info)) return true;
			if(onFileDragged.notify(info)) return true;
			
		}
	}
	
	return false;
}



//--------------------------------------------------------------
void mui::Container::reloadTextures(){

}

//--------------------------------------------------------------
void mui::Container::handleReloadTextures(){
	reloadTextures();

	std::vector<Container*>::iterator it = children.begin();
	while( it != children.end() ) {
	    (*it)->handleReloadTextures();
		++it;
	}
}

//--------------------------------------------------------------
bool mui::Container::hasFocus(){
	for( int i = 0; i < OF_MAX_TOUCHES; i++ ){
		if( Root::INSTANCE->touchResponder[i] == this )
			return true;
	}
	
	return false;
}

//--------------------------------------------------------------
bool mui::Container::hasFocus( ofTouchEventArgs &touch ){
    return Root::INSTANCE->touchResponder[touch.id] == this;
}

bool mui::Container::requestFocus( ofTouchEventArgs &args ){
	return Root::INSTANCE->becomeTouchResponder(this, args);
}

bool mui::Container::hasKeyboardFocus(){
	return Root::INSTANCE->keyboardResponder == this;
}

bool mui::Container::requestKeyboardFocus(){
	return Root::INSTANCE->becomeKeyboardResponder(this);
}

//--------------------------------------------------------------
ofPoint mui::Container::getGlobalPosition(){
	float posX = 0;
	float posY = 0;
	
	Container * element = this;
	while( element != NULL ){
		posX += element->x;
		posY += element->y;
		element = element->parent;
	}
	
	return ofPoint(posX,posY);
}

mui::Container * mui::Container::byName( string name ){
	vector<mui::Container*>::iterator it = children.begin();
	while( it != children.end() ){
		if( (*it)->name == name ) return *it;
		++it;
	}
	
	return NULL;
}

void mui::Container::toFront(){
	if( parent != NULL ){
		auto it = std::find(parent->children.begin(), parent->children.end(), this);
		if( it != parent->children.end() ){
			//TODO: (maybe) use std::rotate
			parent->children.erase(it);
			parent->add(this, -1);
		}
	}
}

void mui::Container::toBack(){
	if( parent != NULL ){
		auto it = std::find(parent->children.begin(), parent->children.end(), this);
		if( it != parent->children.end() ){
			//TODO: (maybe) use std::rotate
			parent->children.erase(it);
			parent->add(this, 0);
		}
	}
}

mui::Container * mui::Container::findChildAt( float posX, float posY, bool onlyVisible, bool mustAcceptTouches ){
	if( posX >= 0 && posY >= 0 && posX <= width && posY <= height && (!onlyVisible || visible ) ){
		// do we have a child that fits there?
		vector<mui::Container*>::iterator it = children.begin();

		mui::Container * result = NULL;
		mui::Container * child = NULL;

		while( it != children.end() ){
			child = (*it)->findChildAt( posX - (*it)->x, posY - (*it)->y, onlyVisible, mustAcceptTouches );
			if( child != NULL ) result = child;
			++it;
		}
		
		return (result == NULL && (!mustAcceptTouches || !ignoreEvents))? this : result;
	}
	else{
		return NULL;
	}
}

bool mui::Container::isVisibleOnScreen( float border ){
	float posX = 0;
	float posY = 0;
	
	Container * element = this;
	while( element != NULL ){
		if( element->visible == false ) return false;
		if( element->parent != nullptr ){
			posX += element->x;
			posY += element->y;
		}
		else if( dynamic_cast<mui::Root*>(element) == nullptr ){
			return false;
		}
		element = element->parent;
	}
	
	
	ofRectangle me(posX-border,posY-border,width+2*border,height+2*border);
	ofRectangle root(0,0,MUI_ROOT->width, MUI_ROOT->height );
	
	return me.intersects(root);
}

bool mui::Container::isMouseOver(){
/*	ofRectangle rect = getGlobalBounds();
	float mouseX = ofGetMouseX()/mui::MuiConfig::scaleFactor;
	float mouseY = ofGetMouseY()/mui::MuiConfig::scaleFactor;
	return rect.inside(mouseX,mouseY);*/
	return MUI_ROOT->hoverResponder.find(this) != MUI_ROOT->hoverResponder.end();
}

//--------------------------------------------------------------
string mui::Container::toString(){
	return
    string("Container") +
    "[ name=" + name +
    ", x=" + ofToString( x, 0 ) +
    ", y=" + ofToString( y, 0 ) +
    ", y=" + ofToString( y, 0 ) +
    ", w=" + ofToString( width, 0 ) +
    ", h=" + ofToString( height, 0 ) +
    "]"; 
}


string mui::Container::getPropertyString(string key){
	auto it = properties.find(key);
	if (it == properties.end()) return "";
	
	mui::data::Attribute & val = (*it).second;
	try{
		return val.value<string>();
	}
	catch(exception&){}

	try{
		const char * ch = val.value<const char*>();
		return string(ch);
	}
	catch(exception &){}
	
	return "";
}
