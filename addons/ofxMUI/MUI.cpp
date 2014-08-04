/*
 *  MUI.cpp
 *  iPhoneEmptyExample
 *
 *  Created by hansi on 28.01.11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "MUI.h"
#include "ofAppiOSWindow.h"

void mui_init(){
	#ifdef TARGET_OS_IPHONE
	ofAppiOSWindow * w = ofAppiOSWindow::getInstance();
	if( w->isRetinaEnabled() ) mui::Helpers::enableRetinaHack();
	#endif
}