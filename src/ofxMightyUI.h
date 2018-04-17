/*
 *  ofxMightyUI.h
 *
 *  by hansi, 2011-2016. https://github.com/kritzikratzi/ofxMightyUI
 *  MIT License
 */

#pragma once



#if TARGET_OS_IPHONE
#include "ofxiPhone.h"
#include "NativeIOS.h"
//#elif TARGET_OS_MAC
//	#include "NativeOSX.h"
#elif TARGET_OS_MAC
//	#warning don't use textfields, lol
#elif TARGET_OS_WIN32
//	#pragma message ("don't use textfields!")
#elif defined(__ANDROID_API__)
//	#warning No ofxMui textfield implementation for android yet!
#elif MUI_BE_INSANE
#else
//	#warning No native textfield implementation for this platform. You can define MUI_BE_INSANE to skip over this warning if you know you're not using textfields.
#endif

#include "MuiCore.h"

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
#include "Image.h"

void mui_init();

ofVec2f muiGetMousePos();
int muiGetMouseX();
int muiGetMouseY();
float muiGetWidth();
float muiGetHeight();

string muiToDataPath(string path, bool abs = true );
