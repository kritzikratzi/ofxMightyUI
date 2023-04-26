/*
 *  ScrollPane.h
 *  ofxMightyUI
 *  
 * um.. the scrol pane :) 
 * use scrollpane->view->add to add elements. 
 * after that call scrollpane->commit() to 
 * compute the new boundaries. this will not happen 
 * automatically. 
 */

#ifndef MUI_SCROLL_PANE
#define MUI_SCROLL_PANE

#include "MuiCore.h"

namespace mui{
    class ScrollPane;
    
	enum BarStyle{
		IF_NEEDED,
		ALWAYS
	};
	
    class ScrollPaneView : public Container{
    public:
		enum class Type{main,top,left};
		ScrollPaneView( mui::ScrollPane * owner, ScrollPaneView::Type viewType, float x, float y, float w, float h );
		virtual ~ScrollPaneView(); 
        virtual void handleDraw();
	private:
		mui::ScrollPane * owner;
		Type viewType;
    };
    
	class ScrollPane : public Container{
	public: 
		ScrollPane( float x_ = 0, float y_ = 0, float width_ = 200, float height_ = 20 );
		~ScrollPane();
		float scrollX, scrollY; // intended
		float currentScrollX, currentScrollY; // actually

		float minScrollX, minScrollY; 
		float maxScrollX, maxScrollY;
		float viewportWidth, viewportHeight;
		
		BarStyle barStyleX, barStyleY; 
		
		bool canScrollX, canScrollY;
		
		ScrollPaneView * view;
		float padRight = 0;
		float padBottom = 0; 
        
		virtual void init();
		
		virtual void commit(bool relayoutViews=true); 
		virtual ofRectangle getViewBoundingBox();
		virtual void update();
		virtual void draw();
		virtual void drawBackground();
		virtual void handleDraw();
		virtual void handleLayout();

		// scroll the least amount necessary so that the element becomes visible
		void scrollIntoView(mui::Container * container, bool animate = true);
		// scroll the least amount necessary so that the target rectangle become visible.
		void scrollIntoView(const ofRectangle & rect, bool animate = true);
		// scrolls to a position
		void scrollTo( float x, float y, bool animate = true );
		void scrollBy(float dx, float dy, bool animate = true); 
		
		// internal stuff ^^
		void beginBaseAnimation( float toX, float toY, bool animate = true );
		void beginMomentumAnimation();
		
		// use horizontal paging?
		bool usePagingH;
		bool autoLockToBottom;
		
		// create a new page.
		Container * createPage(); // memory belongs to you, release accordingly
		ScrollPane * createPageWithScrollPane(); // memory belongs to you, release accordingly
		virtual void nextPage(int inc = 1);
		virtual void prevPage(int dec = 1);
		virtual void gotoPage( int page );
		virtual int getPageNum();
		virtual int numPages();
		
		mui::Container * getLeftMenu(float height = -1);
		mui::Container * getTopMenu(float width = -1);
		
		virtual void updateTouchVelocity( ofTouchEventArgs &touch );
		virtual void touchDown( ofTouchEventArgs &touch );
		virtual void touchMoved( ofTouchEventArgs &touch );
		virtual void touchMovedOutside( ofTouchEventArgs &touch );
		virtual void touchUp( ofTouchEventArgs &touch ); 
		virtual void touchUpOutside( ofTouchEventArgs &touch ); 
		virtual void touchDoubleTap( ofTouchEventArgs &touch ); 
		virtual void touchCanceled( ofTouchEventArgs &touch ); 
		virtual bool mouseScroll( ofMouseEventArgs &args);

		virtual Container * handleTouchDown( ofTouchEventArgs &touch );
		virtual Container * handleTouchMoved( ofTouchEventArgs &touch );
		virtual Container * handleTouchUp( ofTouchEventArgs &touch );
		
		
	private:
		enum TrackingState{
			DRAG_CONTENT,
			DRAG_SCROLLBAR_X,
			DRAG_SCROLLBAR_Y,
			INACTIVE,
			FOLLOW_DURING_DRAG
		};
//		virtual inline float getScrollTarget( float value, float min, float max ); 
		void beginTracking(ofTouchEventArgs &touch, TrackingState state);
		
		TrackingState trackingState;
		float pressedX, pressedY;
		bool wantsToScrollX, wantsToScrollY;
		bool isAutoLockingToBottom;
        float initialX, initialY; // positions before drag start
		
		bool watchingTouch[OF_MAX_TOUCHES]; 
		ofPoint touchStart[OF_MAX_TOUCHES]; 
        
        long lastTouchTime; // time last touched in us
        float velX, velY; // current velocity of touch 
        float touchStartX, touchStartY; 
        
        // are we animating and if yes, in which way? 
        bool animating, animatingToBase, animatingMomentum; 
        long animationStartTime; 
        long lastAnimationTime; 
        float animateX, animateY; // variables used during animation
        float animateToX, animateToY;
		
		int numPagesAdded;
		
		Inset inset; // how much to inset the scrollview from the scrollpane itself.
		mui::ScrollPaneView * topMenu = nullptr;
		mui::ScrollPaneView * leftMenu = nullptr;
		friend class ScrollPaneView; 
	};
	
};

#endif
