

#include "ofxEasyRetina.h"


ofxEasyRetina::ofxEasyRetina(){
	ofDisableSetupScreen();
	scaleFactor = ofxiOSGetOFWindow()->isRetinaEnabled() ? 2 : 1;
}

void ofxEasyRetina::setDeviceScaleFactor(float factor_){
	scaleFactor = factor_;
}

float ofxEasyRetina::getScaleFactor(){
	return  scaleFactor;
}

void ofxEasyRetina::setNearestMagnification(){
	//make retina devices upscale with nearest (not default linear), just to clearly see the difference
	EAGLView * view = ofxiOSGetGLView();
	view.layer.magnificationFilter = kCAFilterNearest;
}


void ofxEasyRetina::setLinearMagnification(){
	//make retina devices upscale with nearest (not default linear), just to clearly see the difference
	EAGLView * view = ofxiOSGetGLView();
	view.layer.magnificationFilter = kCAFilterLinear;
}


//----------------------------------------------------------
void ofxEasyRetina::setupScreenPerspective(float width, float height, float fov, float nearDist, float farDist) {
	ofRectangle currentViewport = ofGetCurrentRenderer()->getCurrentViewport();

	// oriol added scale (retina)
	float viewW = currentViewport.width / scaleFactor;
	float viewH = currentViewport.height / scaleFactor;

	float eyeX = viewW / 2;
	float eyeY = viewH / 2;
	float halfFov = PI * fov / 360;
	float theTan = tanf(halfFov);
	float dist = eyeY / theTan;
	float aspect = (float) viewW / viewH;

	if(nearDist == 0) nearDist = dist / 10.0f;
	if(farDist == 0) farDist = dist * 10.0f;


	ofGetCurrentRenderer()->matrixMode(OF_MATRIX_PROJECTION);
	ofMatrix4x4 persp;
	persp.makePerspectiveMatrix(fov, aspect, nearDist, farDist);
	ofGetCurrentRenderer()->loadMatrix( persp );

	ofGetCurrentRenderer()->matrixMode(OF_MATRIX_MODELVIEW);
	ofMatrix4x4 lookAt;
	lookAt.makeLookAtViewMatrix( ofVec3f(eyeX, eyeY, dist),  ofVec3f(eyeX, eyeY, 0),  ofVec3f(0, 1, 0) );
	ofGetCurrentRenderer()->loadMatrix(lookAt);

}

//----------------------------------------------------------
void ofxEasyRetina::setupScreenOrtho(float nearDist, float farDist) {

	ofRectangle currentViewport = ofGetCurrentRenderer()->getCurrentViewport();

	float viewW = currentViewport.width / scaleFactor; // oriol added scale (retina)
	float viewH = currentViewport.height / scaleFactor;

	ofMatrix4x4 ortho;

	ortho = ofMatrix4x4::newOrthoMatrix(0, viewW, 0, viewH, nearDist, farDist);

	ofGetCurrentRenderer()->matrixMode(OF_MATRIX_PROJECTION);
	ofGetCurrentRenderer()->loadMatrix(ortho); // make ortho our new projection matrix.

	ofGetCurrentRenderer()->matrixMode(OF_MATRIX_MODELVIEW);
	ofGetCurrentRenderer()->loadIdentityMatrix();

}