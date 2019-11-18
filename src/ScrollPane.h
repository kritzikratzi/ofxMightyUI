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
        ScrollPaneView( float x, float y, float w, float h ) : Container( x, y, w, h ){
			focusTransferable = false;
		};
        virtual void handleDraw();
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
		
		virtual void commit(); 
		virtual ofRectangle getViewBoundingBox();
		virtual void update();
		virtual void draw();
		virtual void drawBackground();
		virtual void handleDraw();
		virtual void handleLayout();

		// scroll the least amount necessary so that the element becomes visible
		void scrollIntoView(mui::Container * container);
		// scroll the least amount necessary so that the target rectangle become visible.
		void scrollIntoView(const ofRectangle & rect);
		// scrolls to a position
		void scrollTo( float x, float y );
		void scrollBy(float dx, float dy); 
		
		// internal stuff ^^
		void beginBaseAnimation( float toX, float toY );
		void beginMomentumAnimation();
		
		// use horizontal paging?
		bool usePagingH;
		bool autoLockToBottom;
		
		// create a new page.
		Container * createPage();
		ScrollPane * createPageWithScrollPane();
		virtual void nextPage(int inc = 1);
		virtual void prevPage(int dec = 1);
		virtual void gotoPage( int page );
		virtual int getPageNum();
		virtual int numPages();
		
		
		
		
		virtual void updateTouchVelocity( ofTouchEventArgs &touch );
		virtual void touchDown( ofTouchEventArgs &touch );
		virtual void touchMoved( ofTouchEventArgs &touch );
		virtual void touchMovedOutside( ofTouchEventArgs &touch );
		virtual void touchUp( ofTouchEventArgs &touch ); 
		virtual void touchUpOutside( ofTouchEventArgs &touch ); 
		virtual void touchDoubleTap( ofTouchEventArgs &touch ); 
		virtual void touchCanceled( ofTouchEventArgs &touch ); 
		virtual void mouseScroll( ofMouseEventArgs &args);

		virtual Container * handleTouchDown( ofTouchEventArgs &touch );
		virtual Container * handleTouchMoved( ofTouchEventArgs &touch );
		virtual Container * handleTouchUp( ofTouchEventArgs &touch );
		
		
	private:
		enum TrackingState{
			DRAG_CONTENT,
			DRAG_SCROLLBAR_X,
			DRAG_SCROLLBAR_Y,
			INACTIVE
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
	};
	
};

#endif
