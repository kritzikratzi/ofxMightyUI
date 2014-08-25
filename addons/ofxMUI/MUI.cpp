/*
 *  MUI.cpp
 *  iPhoneEmptyExample
 *
 *  Created by hansi on 28.01.11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "MUI.h"
#if TARGET_OS_IPHONE
	#include "ofAppiOSWindow.h"
#endif

void mui_init(){
	#if TARGET_OS_IPHONE
	ofAppiOSWindow * w = ofAppiOSWindow::getInstance();
	if( w->isRetinaEnabled() ) mui::Helpers::enableRetinaHack();
	#endif
	//TODO: allow retina in osx too! 
}