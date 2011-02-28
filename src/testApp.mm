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
	
	barLeft = new mui::BarButton( "Left" ); 
	barRight = new mui::BarButton( "Right" ); 
	window->setLeftBarButton( barLeft ); 
	window->setRightBarButton( barRight ); 
	
	slider = new mui::Slider( 20, 60, 250, 20, 0, 1, 0.5 );
	slider2 = new mui::SliderWithLabel( 20, 100, 250, 20, 0, 1, 0.5, 2 );
	window->add( slider ); 
	window->add( slider2 ); 
	
	button = new mui::Button( "Button", 20, 240, 70, 30 ); 
	button->onPress += Poco::Delegate<testApp, ofTouchEventArgs>( this, &testApp::onButtonPress );
	window->add( button );
	
	
	window2 = new mui::InternalWindow( "Scrolling", 300, 0, 300, 300 ); 
	scrollPane = new mui::ScrollPane( 10, 50, 280, 240 );
	
	textField = new mui::TextField( "hey!", 0, 0, 270, 30 ); 
	scrollPane->view->add( textField ); 
	
	segmentedSelect = new mui::SegmentedSelect( 0, 30, 270, 30 ); 
	segmentedSelect->addLabel( "Sin" ); 
	segmentedSelect->addLabel( "Cos" ); 
	segmentedSelect->addLabel( "Tan" ); 
	scrollPane->view->add( segmentedSelect ); 
	
	ofColor fg; 
	fg.r = fg.g = fg.b = 0; 
	for( int i = 0; i < 10; i++ ){
		labels[i] = new mui::Label( "i'm a label:" + ofToString( i, 0 ), 0, i*50 + 60, 270, 50 ); 
		labels[i]->fg = fg; 
		labels[i]->commit();
		scrollPane->view->add( labels[i] );
	}
	scrollPane->commit();
	window2->add( scrollPane );
	
	root->add( window2 );
	 
	
	loadWindow = new LoadWindow(); 
	root->add( loadWindow ); 
}

//--------------------------------------------------------------
void testApp::onButtonPress( const void* sender, ofTouchEventArgs &args ){
	/*tween::TweenerParam param(200, tween::QUAD, tween::EASE_OUT);
	bool small = slider->width == 250; 
    param.addProperty(&(slider->width), small? 150:250 ); 
    param.addProperty(&(slider2->width), small? 150:250 ); 
    param.addProperty(&(window->width), small? 200:300 ); 
    param.addProperty(&(window->height), small? 200:300 ); 
    param.addProperty(&(button->y), small?140:240 ); 
    tweener.addTween(param); */
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

