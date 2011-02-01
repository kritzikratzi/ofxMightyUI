#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){	
	// register touch events
	ofRegisterTouchEvents(this);
	ofEnableSmoothing(); 
	
	// initialize the accelerometer
	ofxAccelerometer.setup();
	
	//iPhoneAlerts will be sent to this.
	ofxiPhoneAlerts.addListener(this);
	
	//If you want a landscape oreintation 
	//iPhoneSetOrientation(OFXIPHONE_ORIENTATION_LANDSCAPE_RIGHT);
	
	ofBackground(127,127,127);
	ofSetFrameRate( 60 ); 
	
	// do this first: 
	//mui::Helpers::enableRetinaHack(); 
	
	
	root = new mui::Root();
	
	window = new mui::InternalWindow( "Adjustments", 0, 0, 300, 300 ); 
	root->add( window ); 
	
	slider = new mui::Slider( 20, 20, 250, 100, 0, 1, 0.5 );
	window->add( slider ); 
	
	button = new mui::Button( "Button", 20, 240, 70, 30 ); 
	button->onPress += Poco::Delegate<testApp, ofTouchEventArgs>( this, &testApp::onButtonPress );
	window->add( button ); 
}

//--------------------------------------------------------------
void testApp::onButtonPress( const void* sender, ofTouchEventArgs &args ){
	tween::TweenerParam param(200, tween::QUAD, tween::EASE_OUT);
	bool small = slider->width == 250; 
    param.addProperty(&(slider->width), small? 150:250 ); 
    param.addProperty(&(window->width), small? 200:300 ); 
    param.addProperty(&(window->height), small? 200:300 ); 
    param.addProperty(&(button->y), small?140:240 ); 
    tweener.addTween(param); 
}



//--------------------------------------------------------------
void testApp::update(){
	long start = ofGetSystemTime(); 
	root->handleUpdate(); 
	wastedTime += ofGetSystemTime() - start; 
	tweener.step( ofGetSystemTime() );
}

//--------------------------------------------------------------
void testApp::draw(){
	long start = ofGetSystemTime(); 
	root->handleDraw(); 
	wastedTime += ofGetSystemTime() - start; 
	
	if( ofGetFrameNum() % 100 == 0 ){
		cout << "avg time wasted: " << ( wastedTime/100 ) << " milliseconds per iteration" << endl; 
		wastedTime = 0; 
	}
	
}

//--------------------------------------------------------------
void testApp::exit(){

}

//--------------------------------------------------------------
void testApp::touchDown(ofTouchEventArgs &touch){
	root->handleTouchDown( touch ); 
}

//--------------------------------------------------------------
void testApp::touchMoved(ofTouchEventArgs &touch){
	root->handleTouchMoved( touch ); 
}

//--------------------------------------------------------------
void testApp::touchUp(ofTouchEventArgs &touch){
	root->handleTouchUp( touch ); 
}

//--------------------------------------------------------------
void testApp::touchDoubleTap(ofTouchEventArgs &touch){
	root->handleTouchDoubleTap( touch ); 
}

//--------------------------------------------------------------
void testApp::lostFocus(){

}

//--------------------------------------------------------------
void testApp::gotFocus(){

}

//--------------------------------------------------------------
void testApp::gotMemoryWarning(){

}

//--------------------------------------------------------------
void testApp::deviceOrientationChanged(int newOrientation){

}

