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
	
	window = new mui::InternalWindow( "Adjustments", 0, 400, 300, 300 );
	window->view->layoutManager = new mui::RowLayout();
	root->add( window );
	
	barLeft = new mui::BarButton( "Left" );
	barRight = new mui::BarButton( "Right" ); 
	window->setLeftBarButton( barLeft ); 
	window->setRightBarButton( barRight ); 
	
	slider = new mui::Slider( -1, -1, 250, 20, 0, 1, 0.5 );
	slider2 = new mui::SliderWithLabel( -1, -1, 250, 20, 0, 1, 0.5, 2 );
	window->view->add( slider );
	window->view->add( slider2 );
	
	button = new mui::Button( "Button", 20, 200, 70, 30 ); 
	button->onPress += Poco::Delegate<testApp, ofTouchEventArgs>( this, &testApp::onButtonPress );
	window->view->add( button );
	
	window->view->layout();
	
	
	window2 = new mui::InternalWindow( "Scrolling", 300, 800, 300, 300 ); 
	window2->view->layoutManager = new mui::RowLayout();
	scrollPane = new mui::ScrollPane( -1, -1, 280, 240 );
	
	textField = new mui::TextField( "hey!", -1, -1, 270, 30 ); 
	scrollPane->view->add( textField );
	
	segmentedSelect = new mui::SegmentedSelect( -1, -1, 270, 30 );
	segmentedSelect->addLabel( "Sin" );
	segmentedSelect->addLabel( "Cos" );
	segmentedSelect->addLabel( "Tan" );
	scrollPane->view->add( segmentedSelect );
	scrollPane->view->layoutManager = new mui::RowLayout(); 
	ofColor fg; 
	fg.r = fg.g = fg.b = 0; 
	for( int i = 0; i < 10; i++ ){
		labels[i] = new mui::Label( "i'm a label:" + ofToString( i, 0 ), -1, -1, 270, 50 );
		labels[i]->fg = fg; 
		labels[i]->commit();
		scrollPane->view->add( labels[i] );
	}
	//scrollPane->canScrollX = false; 
	scrollPane->view->layout(); 
	scrollPane->commit();
	window2->view->add( scrollPane );
	window2->view->layout(); 
	
	root->add( window2 );
	 
	
	loadWindow = new LoadWindow(); 
	//root->add( loadWindow ); 
}

//--------------------------------------------------------------
void testApp::onButtonPress( const void* sender, ofTouchEventArgs &args ){
	bool small = slider->width == 250; 
	root->prepareAnimation(200);
    root->animate(slider->width, small? 150:250 ); 
    root->animate(slider2->width, small? 150:250 ); 
    root->animate(window->width, small? 200:300 ); 
    root->animate(window->height, small? 200:300 ); 
	root->animate(button->y, small?100:200 ); 
	root->commitAnimation(); 
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
		//cout << "avg time wasted: " << ( wastedTime/100 ) << " milliseconds per iteration" << endl; 
		avgWastedTime = wastedTime/100; 
		wastedTime = 0; 
	}
	ofSetColor( 255, 255, 255 ); 
	mui::Helpers::getFont( 12 )->drawString( ofToString( avgWastedTime, 0 ) + " ms/frame", 10, 1000 ); 
	
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

