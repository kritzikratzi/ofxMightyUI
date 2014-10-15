/*
 *  MUI.cpp
 *  iPhoneEmptyExample
 *
 *  Created by hansi on 28.01.11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "MUI.h"
#include <Poco/Util/Application.h>

#if TARGET_OS_IPHONE
	#include "ofAppiOSWindow.h"
	#include <CoreFoundation/CoreFoundation.h>
#endif

void mui_init(){
	#if TARGET_OS_IPHONE
	ofAppiOSWindow * w = ofAppiOSWindow::getInstance();
	if( w->isRetinaEnabled() ) mui::Helpers::enableRetinaHack();
	#endif
	//TODO: allow retina in osx too!
	
	Poco::Path appPath;
	#if TARGET_OS_IPHONE
		// http://www.cocoabuilder.com/archive/cocoa/193451-finding-out-executable-location-from-c-program.html
		CFBundleRef bundle = CFBundleGetMainBundle();
		CFURLRef    url  = CFBundleCopyExecutableURL(bundle); // CFBundleCopyResourcesDirectoryURL(bundle);
		CFURLRef absolute = CFURLCopyAbsoluteURL(url);
		CFStringRef path  = CFURLCopyFileSystemPath(url, kCFURLPOSIXPathStyle);
		CFIndex    maxLength = CFStringGetMaximumSizeOfFileSystemRepresentation(path);
		char        *result = (char*)malloc(maxLength);
		
		if(result) {
			if(!CFStringGetFileSystemRepresentation(path,result, maxLength)) {
				free(result);
				result = NULL;
			}
		}
		
		CFRelease(path);
		CFRelease(url);
		CFRelease(absolute);
		appPath = Poco::Path(result);
		appPath = appPath.parent();
	#elif TARGET_OS_MAC
		// http://www.cocoabuilder.com/archive/cocoa/193451-finding-out-executable-location-from-c-program.html
		CFBundleRef bundle = CFBundleGetMainBundle();
		CFURLRef    url  = CFBundleCopyExecutableURL(bundle); // CFBundleCopyResourcesDirectoryURL(bundle);
		CFURLRef absolute = CFURLCopyAbsoluteURL(url);
		CFStringRef path  = CFURLCopyFileSystemPath(url, kCFURLPOSIXPathStyle);
		CFIndex    maxLength = CFStringGetMaximumSizeOfFileSystemRepresentation(path);
		char        *result = (char*)malloc(maxLength);
		
		if(result) {
			if(!CFStringGetFileSystemRepresentation(path,result, maxLength)) {
				free(result);
				result = NULL;
			}
		}
		
		CFRelease(path);
		CFRelease(url);
		CFRelease(absolute);
		appPath = Poco::Path(result);
		appPath = appPath.parent().parent().pushDirectory("Resources");
	#else
		appPath = Poco::Path(ofToDataPath("", true));
	#endif
	
	mui::Helpers::dataPath = appPath.absolute();
}


string mui::MuiConfig::font = "mui/fonts/Minecraftia.ttf";
bool mui::MuiConfig::debugDraw = false;
int mui::MuiConfig::scrollPaneBleed = 30;
int mui::MuiConfig::scrollToBaseDuration = 600;
int mui::MuiConfig::scrollVelocityDecrease = 300;
int mui::MuiConfig::fontSize = 12;
