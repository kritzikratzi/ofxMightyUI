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
	
	root = new mui::Root(); 
	
	mui::Slider * slider = new mui::Slider( 20, 20, 100, 100, 0, 1, 0.5 );
	
	mui::InternalWindow * window = new mui::InternalWindow( 100, 300, 300, 300 ); 
	window->add( slider ); 
	root->add( window ); 
}

//--------------------------------------------------------------
void testApp::update(){
	root->handleUpdate(); 
}

//--------------------------------------------------------------
void testApp::draw(){
	root->handleDraw(); 
	popup.draw(); 
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

