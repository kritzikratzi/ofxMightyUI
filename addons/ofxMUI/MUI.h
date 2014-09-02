/*
 *  MUI.h
 *  iPhoneEmptyExample
 *
 *  Created by hansi on 28.01.11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

// retina mode on or off? 
// this will be dynamic one day, 
// but you can safely use the define for querying
#define MUI_RETINA false

// default font
#define MUI_FONT "mui/fonts/Minecraftia.ttf"

// enable/disable debug drawing. this will put rectangles 
// around each component
#define MUI_DEBUG_DRAW false

// already deprecated, don't use
#define MUI_SCROLLPANE_BLEED 30

// time it takes for a scrollpane to scroll back to it's initial position, time in ms
// should be 6*325, feels way too slow
#define MUI_SCROLL_TO_BASE_DURATION 600

// rate at which the scroll velocity decreases, not sure about the unit, pixels/s?
// guessing around ... 
#define MUI_SCROLL_VELOCITY_DECREASE 300 

// which font rendering thingie to use?
// works with ofTrueTypeFont and ofxTrueTypeFontFS
// enable the include in case of the FS font...
#define MUI_FONT_TYPE ofxTrueTypeFontFS
#include "../ofxFontStash/ofxTrueTypeFontFS.h"

#define MUI_FONT_SIZE 12

#ifndef MUI_CRAZY_IMPORTANT
#define MUI_CRAZY_IMPORTANT

namespace mui{
	enum HorizontalAlign{
		Left, Right, Center
	};
	enum VerticalAlign{
		Top, Bottom, Middle /* i wish i could support baseline, but it's not in ofTrueTypeFont yet! */
	};
	enum Direction{
		Horizontal = 1, Vertical = 2, Both = 3
	}; 
}

#endif

#include <vector>
#include "ofMain.h"
#include "ofxFBOTexture.h"
#include "CppTweener.h"
#include <Poco/BasicEvent.h>
#include <Poco/Delegate.h>
#include <math.h>

// wtf windows...  
// https://code.google.com/p/py-lepton/source/browse/trunk/lepton/renderer.h?r=222
#ifndef fminf
	#define fminf(x, y) (((x) < (y)) ? (x) : (y))
#endif
#ifndef fmaxf
	#define fmaxf(x, y) (((x) < (y)) ? (y) : (x))
#endif
#ifndef roundf
#define roundf(x) (x >= 0.0f ? floorf(x + 0.5f) : ceilf(x - 0.5f))
#endif

#if TARGET_OS_IPHONE
	#include "ofxiPhone.h"
	#include "NativeIOS.h"

	#ifndef OF_MAX_TOUCHES
		#warning OF_MAX_TOUCHES not defined, assuming 20.
		#define OF_MAX_TOUCHES 20
	#endif
//#elif TARGET_OS_MAC
//	#include "NativeOSX.h"
#elif TARGET_OS_MAC
	#warning don't use textfields, lol
#elif TARGET_OS_WIN32
	#pragma message ("don't use textfields!") 
#elif MUI_BE_INSANE
#else
	#error No native textfield implementation for this platform. You can define MUI_BE_INSANE to skip over this error if you know you're not using textfields.
#endif


#ifndef OF_MAX_TOUCHES 
#pragma message(" OF_MAX_TOUCHES not defined, assuming 20.")
#define OF_MAX_TOUCHES 20
#endif

#include "Helpers.h"
#include "Container.h"
#include "Layout.h"
#include "ScrollPane.h"
#include "Label.h"
#include "Root.h"
#include "Slider.h"
#include "Button.h"
#include "ToggleButton.h"
#include "BarButton.h"
#include "EmptyContainer.h"
#include "InternalWindow.h"
#include "SliderWithLabel.h"
#include "TextField.h"
#include "SegmentedSelect.h"

void mui_init(); 