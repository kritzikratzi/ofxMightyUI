//
//  ofxiOSEAGLView+retinaPatch.m
//  emptyExample
//
//  Created by Oriol Ferrer Mesià on 21/01/13.
//
//

#import "ofxiOSEAGLView.h"

#import "ofMain.h"
#import "ofxiPhoneExtras.h"
#import "ofxiOSEAGLView+retinaPatch.h"
#import "ofxiOSExtensions.h"


@interface ofxiOSEAGLView() {
    BOOL bInit;
}
- (void)updateDimensions;
@end


@implementation ofxiOSEAGLView (retinaPatch)



- (void) drawView {

    ofNotifyUpdate();

    //------------------------------------------

    [self lockGL];
    [self startRender];

	ofGLProgrammableRenderer * es2Renderer = NULL;
    if(ofIsGLProgrammableRenderer()) {
        es2Renderer = (ofGLProgrammableRenderer *)(ofGetCurrentRenderer().get());
        es2Renderer->startRender();
    }


    glViewport(0, 0, windowSize->x * scaleFactor, windowSize->y * scaleFactor);

    float * bgPtr = ofBgColorPtr();
    bool bClearAuto = ofbClearBg();
    if ( bClearAuto == true){
        glClearColor(bgPtr[0],bgPtr[1],bgPtr[2], bgPtr[3]);
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    if(ofAppiPhoneWindow::getInstance()->isSetupScreenEnabled()) {
        ofSetupScreen();
    }

    //------------------------------------------ draw.

    ofNotifyDraw();

    //------------------------------------------

    if(es2Renderer != NULL) {
        es2Renderer->finishRender();
    }

    [self finishRender];
    [self unlockGL];

    [super notifyDraw];   // alerts delegate that a new frame has been drawn.
}


- (void)updateDimensions {

	float pscaleFactor = scaleFactor; //store real scaleFactor
	scaleFactor = 1; // pretend the screen is no retina

    windowPos->set(self.frame.origin.x * scaleFactor, self.frame.origin.y * scaleFactor, 0);
    windowSize->set(self.frame.size.width * scaleFactor, self.frame.size.height * scaleFactor, 0);

    UIScreen * currentScreen = self.window.screen;  // current screen is the screen that GLView is attached to.
    if(!currentScreen) {                            // if GLView is not attached, assume to be main device screen.
        currentScreen = [UIScreen mainScreen];
    }
    screenSize->set(currentScreen.bounds.size.width * scaleFactor, currentScreen.bounds.size.height * scaleFactor, 0);

	scaleFactor = pscaleFactor; // set back to retina scalefactor
}


//------------------------------------------------------
- (void)touchesBegan:(NSSet *)touches
           withEvent:(UIEvent *)event{

    if(!bInit) {
        // if the glView is destroyed which also includes the OF app,
        // we no longer need to pass on these touch events.
        return;
    }

	for(UITouch *touch in touches) {
		int touchIndex = 0;
		while([[activeTouches allValues] containsObject:[NSNumber numberWithInt:touchIndex]]){
			touchIndex++;
		}

		[activeTouches setObject:[NSNumber numberWithInt:touchIndex] forKey:[NSValue valueWithPointer:(void*)touch]];

		CGPoint touchPoint = [touch locationInView:self];

		touchPoint.x *= 1; // this has to be done because retina still returns points in 320x240 but with high percision
		touchPoint.y *= 1; //uri changed  scaleFactor

		if( touchIndex==0 ){
			ofNotifyMousePressed(touchPoint.x, touchPoint.y, 0);
		}

		ofTouchEventArgs touchArgs;
        touchArgs.numTouches = [[event touchesForView:self] count];
		touchArgs.x = touchPoint.x;
		touchArgs.y = touchPoint.y;
		touchArgs.id = touchIndex;
		if([touch tapCount] == 2) ofNotifyEvent(ofEvents().touchDoubleTap,touchArgs);	// send doubletap
		ofNotifyEvent(ofEvents().touchDown,touchArgs);	// but also send tap (upto app programmer to ignore this if doubletap came that frame)
	}
}

//------------------------------------------------------
- (void)touchesMoved:(NSSet *)touches
           withEvent:(UIEvent *)event{

    if(!bInit) {
        // if the glView is destroyed which also includes the OF app,
        // we no longer need to pass on these touch events.
        return;
    }

	for(UITouch *touch in touches){
		int touchIndex = [[activeTouches objectForKey:[NSValue valueWithPointer:(void*)touch]] intValue];

		CGPoint touchPoint = [touch locationInView:self];

		touchPoint.x *= 1; // this has to be done because retina still returns points in 320x240 but with high percision
		touchPoint.y *= 1;

		if( touchIndex==0 ){
			ofNotifyMouseDragged(touchPoint.x, touchPoint.y, 0);
		}
		ofTouchEventArgs touchArgs;
		touchArgs.numTouches = [[event touchesForView:self] count];
		touchArgs.x = touchPoint.x;
		touchArgs.y = touchPoint.y;
		touchArgs.id = touchIndex;
		ofNotifyEvent(ofEvents().touchMoved, touchArgs);
	}
}

//------------------------------------------------------
- (void)touchesEnded:(NSSet *)touches
           withEvent:(UIEvent *)event{

    if(!bInit) {
        // if the glView is destroyed which also includes the OF app,
        // we no longer need to pass on these touch events.
        return;
    }

	for(UITouch *touch in touches){
		int touchIndex = [[activeTouches objectForKey:[NSValue valueWithPointer:(void*)touch]] intValue];

		[activeTouches removeObjectForKey:[NSValue valueWithPointer:(void*)touch]];

		CGPoint touchPoint = [touch locationInView:self];

		touchPoint.x *= 1; // this has to be done because retina still returns points in 320x240 but with high percision
		touchPoint.y *= 1;

		if( touchIndex==0 ){
			ofNotifyMouseReleased(touchPoint.x, touchPoint.y, 0);
		}

		ofTouchEventArgs touchArgs;
		touchArgs.numTouches = [[event touchesForView:self] count] - [touches count];
		touchArgs.x = touchPoint.x;
		touchArgs.y = touchPoint.y;
		touchArgs.id = touchIndex;
		ofNotifyEvent(ofEvents().touchUp, touchArgs);
	}
}

//------------------------------------------------------
- (void)touchesCancelled:(NSSet *)touches
               withEvent:(UIEvent *)event{

    if(!bInit) {
        // if the glView is destroyed which also includes the OF app,
        // we no longer need to pass on these touch events.
        return;
    }

	for(UITouch *touch in touches){
		int touchIndex = [[activeTouches objectForKey:[NSValue valueWithPointer:(void*)touch]] intValue];

		CGPoint touchPoint = [touch locationInView:self];

		touchPoint.x *= scaleFactor; // this has to be done because retina still returns points in 320x240 but with high percision
		touchPoint.y *= scaleFactor;

		ofTouchEventArgs touchArgs;
		touchArgs.numTouches = [[event touchesForView:self] count];
		touchArgs.x = touchPoint.x;
		touchArgs.y = touchPoint.y;
		touchArgs.id = touchIndex;
		ofNotifyEvent(ofEvents().touchCancelled, touchArgs);
	}

	[self touchesEnded:touches withEvent:event];
}

@end
