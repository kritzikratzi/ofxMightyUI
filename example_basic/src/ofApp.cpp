#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	// try to get rid of this?
	mui_init();
	
	// set up the root panel. its very fast scene graph
	// that sits on top of your application.
	root = new mui::Root();
	
	// lets create a button and a listener
	hello = new mui::Button( "Hello. Press me!", 10, 10, 300, 30 );
	hello->bg = ofColor::orange;
	ofAddListener( hello->onPress, this, &ofApp::buttonPressed);
	root->add( hello );
}

void ofApp::buttonPressed(ofTouchEventArgs & args){
	hello->bg.setHue(ofRandom(360));
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
