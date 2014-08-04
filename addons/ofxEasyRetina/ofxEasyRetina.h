
#pragma once

#include "ofMain.h"
#include "ofxiPhone.h"
#include "ofxiPhoneExtras.h"

class ofxEasyRetina{

	public:

		ofxEasyRetina();
		void setDeviceScaleFactor(float factor);
		void setNearestMagnification(); //to get pixel art looks when nor using retina
		void setLinearMagnification();	//to get blurry looks when nor using retina
		float getScaleFactor();

		void setupScreenPerspective(float width = 0, float height = 0, float fov = 60, float nearDist = 0, float farDist = 0);
		void setupScreenOrtho(float nearDist = -1, float farDist = 1);

	private:

		void loadMatrix (const ofMatrix4x4 & m);
		void loadMatrix (const float * m);

		float scaleFactor ;
};
