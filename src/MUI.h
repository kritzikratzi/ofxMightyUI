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


#include "Helpers.h"
#include "Container.h"
#include "ScrollPane.h"
#include "Label.h"
#include "Root.h"
#include "Slider.h"
#include "InternalWindow.h"
#include "Button.h"
