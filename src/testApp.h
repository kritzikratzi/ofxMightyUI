#pragma once

#include "ofMain.h"
#include "ofxiPhone.h"
#include "ofxiPhoneExtras.h"

#include "ButtonPopup.h"
#include "MUI.h"
#include "poco/Delegate.h"

#include "CppTweener.h" 

class testApp : public ofxiPhoneApp {
	
public:
	void setup();
	void update();
	void draw();
	void exit();
	
	void touchDown(ofTouchEventArgs &touch);
	void touchMoved(ofTouchEventArgs &touch);
	void touchUp(ofTouchEventArgs &touch);
	void touchDoubleTap(ofTouchEventArgs &touch);

	void lostFocus();
	void gotFocus();
	void gotMemoryWarning();
	void deviceOrientationChanged(int newOrientation);

	void onButtonPress( const void* sender, ofTouchEventArgs &args ); 
	
	//ButtonPopup popup;
	tween::Tweener tweener; 
	
	mui::Root *root;
	mui::InternalWindow * window; 
	mui::Slider * slider; 
	mui::SliderWithLabel * slider2; 
	mui::Button * button;
	mui::TextField * textField; 
	mui::InternalWindow * window2; 
	mui::ScrollPane * scrollPane; 
	mui::Label * labels[10]; 
	mui::BarButton * barLeft; 
	mui::BarButton * barRight;
	mui::SegmentedSelect * segmentedSelect; 
	long wastedTime; 
};


