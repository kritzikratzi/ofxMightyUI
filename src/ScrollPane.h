/*
 *  ScrollPane.h
 *  iPhoneEmptyExample
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
    
    class ScrollPaneView : public Container{
    public:
        ScrollPaneView( float x, float y, float w, float h ) : Container( x, y, w, h ){
			focusTransferable = false;
		};
        virtual void handleDraw();
    };
    
	class ScrollPane : public Container{
	public: 
		ScrollPane( float x_ = 0, float y_ = 0, float width_ = 200, float height_ = 20 ) 
			: Container( x_, y_, width_, height_ ), 
			wantsToScrollX(false), wantsToScrollY(false), 
			canScrollX(true), canScrollY(true), scrollX(0), scrollY(0), 
			maxScrollX(0), maxScrollY(0), minScrollX(0), minScrollY(0), 
			currentScrollX(0), currentScrollY(0), 
			pressed(false), 
			view( NULL ), 
            animating(false), animatingToBase(false), animatingMomentum(false),
			usePagingH(false), numPagesAdded(0)
			{ init(); };
		~ScrollPane(); 
		float scrollX, scrollY; // intended
		float currentScrollX, currentScrollY; // actually

		float minScrollX, minScrollY; 
		float maxScrollX, maxScrollY; 
		bool canScrollX, canScrollY;
		
		ScrollPaneView * view;
        
		virtual void init();
		
		virtual void commit(); 
		virtual ofRectangle getViewBoundingBox();
		virtual void update();
		virtual void draw();
		virtual void drawBackground();
		virtual void handleDraw();

		void beginBaseAnimation( float toX, float toY );
		void beginMomentumAnimation();
		
		// use horizontal paging?
		bool usePagingH;
		
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

		virtual Container * handleTouchDown( ofTouchEventArgs &touch );
		virtual Container * handleTouchMoved( ofTouchEventArgs &touch );
		virtual Container * handleTouchUp( ofTouchEventArgs &touch );
		
		
	private: 
//		virtual inline float getScrollTarget( float value, float min, float max ); 
		
		bool pressed; 
		float pressedX, pressedY; 
		bool wantsToScrollX, wantsToScrollY; 
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