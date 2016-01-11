//
//  MuiConfig.h
//  Oscilloscope
//
//  Created by Hansi on 10.08.15.
//
//

#ifndef __Oscilloscope__MuiConfig__
#define __Oscilloscope__MuiConfig__

#include <Poco/Path.h>
#include "ofMain.h"


// default font
#define MUI_FONT MuiConfig::font

// already deprecated, don't use
#define MUI_SCROLLPANE_BLEED mui::MuiConfig::scrollPaneBleed

// time it takes for a scrollpane to scroll back to it's initial position, time in ms
// should be 6*325, feels way too slow
#define MUI_SCROLL_TO_BASE_DURATION mui::MuiConfig::scrollToBaseDuration

// rate at which the scroll velocity decreases, not sure about the unit, pixels/s?
// guessing around ...
#define MUI_SCROLL_VELOCITY_DECREASE mui::MuiConfig::scrollVelocityDecrease

#define MUI_DEBUG_DRAW mui::MuiConfig::debugDraw

#define MUI_FONT_TYPE_OPENFRAMEWORKS 0
#define MUI_FONT_TYPE_FONTSTASH 1

// which font rendering thingie to use?
// works with ofTrueTypeFont and ofxTrueTypeFontFS
// it's nasty, but just add the correct font type to your IDEs global
// preprocessor flags!
#ifndef MUI_FONT_TYPE_ACTIVE
#define MUI_FONT_TYPE_ACTIVE MUI_FONT_TYPE_OPENFRAMEWORKS
#endif

#if MUI_FONT_TYPE_ACTIVE == MUI_FONT_TYPE_OPENFRAMEWORKS
#define MUI_FONT_TYPE ofTrueTypeFont
#else
#define MUI_FONT_TYPE ofxTrueTypeFontFS
#include "_fontstash/ofxTrueTypeFontFS.h"
#endif

#define MUI_FONT_SIZE 12


namespace mui{
	class MuiConfig{
	public:
		// default font
		static std::string font;
		
		// enable/disable debug drawing. this will put rectangles
		// around each component
		static bool debugDraw;
		
		// already deprecated, don't use
		static int scrollPaneBleed;
		
		// time it takes for a scrollpane to scroll back to it's initial position, time in ms
		// should be 6*325, feels way too slow
		static int scrollToBaseDuration;
		
		// rate at which the scroll velocity decreases, not sure about the unit, pixels/s?
		// guessing around ...
		static int scrollVelocityDecrease;
		
		// standard font size
		static int fontSize;
		
		// automaticaly detect retina on device like the ipad?
		// this must be set before calling mui_init()
		static bool detectRetina;
		static bool useRetinaAssets;
		
		// pixels per pixel.
		static int scaleFactor;
		
		// path to the data folder (containing the mui folder)
		static Poco::Path dataPath;
		
		static ofLogLevel logLevel;
	};
}

#endif /* defined(__Oscilloscope__MuiConfig__) */
