/*
 *  MUI.h
 *  iPhoneEmptyExample
 *
 *  Created by hansi on 28.01.11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#define MUI_RETINA false
#define MUI_FONT "mui/fonts/Lato-Regular.ttf"
#define MUI_DEBUG_DRAW false
#define MUI_SCROLLPANE_BLEED 30



#ifndef MUI_CRAZY_IMPORTANT
#define MUI_CRAZY_IMPORTANT

namespace mui{
	enum HorizontalAlign{
		Left, Right, Center
	};
	enum VerticalAlign{
		Top, Bottom, Middle /* i wish i could support baseline, but it's not in ofTrueTypeFont yet! */
	};
}

#endif

#include <vector>
#include "ofMain.h"
#include "ofxiPhone.h"
#include "ofxFBOTexture.h"

#ifdef TARGET_OS_IPHONE
#include "NativeIOS.h"
#elif MUI_BE_INSANE
#else
#error No native textfield implementation for this platform. You can define MUI_BE_INSANE to skip over this error if you know you're not using textfields. 
#endif

#include "Helpers.h"
#include "Container.h"
#include "ScrollPane.h"
#include "Label.h"
#include "Root.h"
#include "Slider.h"
#include "Button.h"
#include "BarButton.h"
#include "EmptyContainer.h"
#include "InternalWindow.h"
#include "SliderWithLabel.h"
#include "TextField.h"
#include "SegmentedSelect.h"