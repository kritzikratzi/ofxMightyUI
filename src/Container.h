/**
 * Container.h
 *
 * Container is the base class of everything and is meant to be override to be useful. 
 *
 * Basic features: 
 * - handling of touch, mouse and key events
 * - a layout callback. layout is performed: 1.)manually by calling handleLayout() 2.)after any window resize 3.)by setting needsLayout=true
 * - update is called all the time as long as the container is reachable from the root
 * - draw is called only when visible=true
 *
 * Hansi Raber, 2010-2016
 */

#ifndef MUI_NODE
#define MUI_NODE

#include "MuiCore.h"

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
		bool needsLayout; 
		
		vector<mui::Container*> children;
		Container * parent; 
		Layout * layoutHandler;
		
		// put anything you like here...
		void * userData; // deprecated

		//bool startedInside[OF_MAX_TOUCHES]; // don't use this. unless you're you really want to.
		
		Container( float x = 0, float y = 0, float width = 10, float height = 10 );
		
		~Container();
		
		void add( Container * c, int index = -1 ); 
		void remove( Container * c ); 
		void remove();
		
		ofRectangle getGlobalBounds();
		ofRectangle getBounds();
		ofRectangle getChildBounds(); 
		void setBounds( float x, float y, float width, float height );
		void setBounds( const ofRectangle bounds );

		virtual void update(){};
		virtual void draw(){};
		virtual void drawBackground(); 
		
		virtual void layout();
		virtual void handleLayout();
		
		virtual void handleDraw();
		virtual void handleUpdate();
		
		// override points for touch/mouse events
		virtual void touchDown( ofTouchEventArgs &touch ){};
		virtual void touchMoved( ofTouchEventArgs &touch ){};
		virtual void touchMovedOutside( ofTouchEventArgs &touch ){}
		virtual void touchHover( ofTouchEventArgs &touch ){};
		virtual void touchUp( ofTouchEventArgs &touch ){};
		virtual void touchUpOutside( ofTouchEventArgs &touch ){}
		virtual void touchDoubleTap( ofTouchEventArgs &touch ){};
		virtual void touchCanceled( ofTouchEventArgs &touch ){}; // when some other component "stole" the responder status.
		virtual void mouseScroll( ofMouseEventArgs &args){};
		virtual void mouseEnter( ofMouseEventArgs &args){}; 
		virtual void mouseExit( ofMouseEventArgs &args){};
		
		// override points for key events
		// return true if you took care of the event, false if it wasn't handled.
		virtual bool keyPressed( ofKeyEventArgs &touch){ return false; };
		virtual bool keyReleased( ofKeyEventArgs &touch){ return true; };
		
		// override points for hierachy events
		virtual void afterAdd( mui::Container * newParent ){}; // called when added to a parent
		virtual void afterRemove( mui::Container * oldParent ){}; // called when reomved from a parent
		
		// handler events. these efficiently perform the event handling internally.
		// only override if necessary and always call the super method!
		virtual Container * handleTouchDown( ofTouchEventArgs &touch );
		virtual Container * handleTouchMoved( ofTouchEventArgs &touch );
		virtual Container * handleTouchHover( ofTouchEventArgs &touch );
		virtual Container * handleTouchUp( ofTouchEventArgs &touch );
		virtual Container * handleTouchDoubleTap( ofTouchEventArgs &touch );
		virtual void handleTouchCanceled( ofTouchEventArgs &touch );
		virtual bool handleFileDragged( ofDragInfo & args );
		
		// override point + handler when the gl state was reset
		virtual void reloadTextures();
		virtual void handleReloadTextures();
		
		// check for focus / request touch focus
		// this is often important during touch events to follow a finger outside the container.
		virtual bool hasFocus();
		virtual bool hasFocus( ofTouchEventArgs &touch );
		virtual bool requestFocus( ofTouchEventArgs &args );
		
		// check for keyboard focus / request keyboard focus
		virtual bool hasKeyboardFocus();
		virtual bool requestKeyboardFocus();
		
		// sum up all element transformation until the parent is null
		virtual ofPoint getGlobalPosition();
		
		// make this the front most child in the parent container
		virtual void toFront();
		// make this the last child in the parent container
		virtual void toBack();
		
		// finds a direct child by it's name.
		mui::Container * byName( string name );
		
		// recursively find children of a certain type at a position
		// implemented directly in the header because templates ... have some issues
		template <typename T>
		T * findChildOfType( float posX, float posY, bool onlyVisible = true, bool mustAcceptTouches = false ){
			mui::Container * thing = findChildAt(posX,posY,onlyVisible,mustAcceptTouches);
			while(thing!=NULL){
				T * result = dynamic_cast<T* const>(thing);
				if( result != NULL ){
					return result;
				}
				else{
					thing = thing->parent;
				}
			}
			return NULL;
		}
		
		// recursively find children at position.
		virtual Container * findChildAt( float x, float y, bool onlyVisible = true, bool mustAcceptTouches = false );
		
		// recursively ascend to parents until a certain type is found.
		template <typename T>
		T * findParentOfType(){
			mui::Container * mum = parent;
			while(mum!=NULL){
				T * result = dynamic_cast<T* const>(mum);
				if( result != NULL ){
					return result;
				}
				else{
					mum = mum->parent;
				}
			}
			return NULL;
		}
		
		// in progress?
		template <typename T>
		T * findChildOfType( const function<bool(Container * container, bool & checkChildren )> walker, const function<bool(T * result)> decider ){
			bool checkChildren = true;
			T * result = nullptr;
			if(walker(this,checkChildren) && dynamic_cast<T>(this) != nullptr && decider((T*)this) ){
				result = (T*)this;
			}
			
			if(checkChildren){
				T * childResult = nullptr;
				for( Container * c : children ){
					childResult = c->findChildOfType<T>(walker, decider);
					if(childResult != nullptr){
						result = childResult;
					}
				}
			}
			
			return result;
		}
		
		// in progress?
		template <typename T>
		T * findChildOfTypeAt( const function<bool(Container * container, bool & checkChildren )> walker, const function<bool(T * result)> decider, float x, float y ){
			bool checkChildren = true;
			T * result = nullptr;
			if( x < 0 || y < 0 || x > width || y > height ) return nullptr;
			if(walker(this,checkChildren) && dynamic_cast<T>(this) != nullptr && decider((T*)this) ){
				result = (T*)this;
			}
			
			if(checkChildren){
				T * childResult = nullptr;
				for( Container * c : children ){
					childResult = c->findChildOfType<T>(walker, decider, x - c->x, y - c->y);
					if(childResult != nullptr){
						result = childResult;
					}
				}
			}
			
			return result;
		}
		
		// in progress?
		template <typename T>
		T * findChildAt( const function<bool(Container * container, bool & checkChildren )> walker, const function<bool(T * result)> decider, float x, float y ){
			bool checkChildren = true;
			T * result = nullptr;
			if( x < 0 || y < 0 || x > width || y > height ) return nullptr;
			if(walker(this,checkChildren) && dynamic_cast<T>(this) != nullptr && decider((T*)this) ){
				result = (T*)this;
			}
			
			if(checkChildren){
				T * childResult = nullptr;
				for( Container * c : children ){
					childResult = c->findChildOfType<T>(walker, decider, x - c->x, y - c->y);
					if(childResult != nullptr){
						result = childResult;
					}
				}
			}
			
			return result;
		}
		
		// tests if the element and all of it's parent are visible.
		// the border parameter slightly enlargers the element, so that visible seems true even when the container is slightly outside
		virtual bool isVisibleOnScreen(float border=0);
		
		 // check if any cursor is over the component
		virtual bool isMouseOver();
		virtual string toString();
	private:
	};
};

#endif
