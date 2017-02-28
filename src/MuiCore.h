//
//  MuiCore.h
//
//  Created by Hansi on 10.08.15.
//
//

#ifndef __Oscilloscope__MuiCore__
#define __Oscilloscope__MuiCore__


#include <vector>
#include <cmath>

#include "ofMain.h"
#include "CppTweener.h"

#ifndef OF_MAX_TOUCHES
#define OF_MAX_TOUCHES 20
#endif

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

	class Inset {
	public: 
		float left{ 0 }, right{ 0 }, top{ 0 }, bottom{ 0 };
		Inset(float v = 0) : top(v), right(v), bottom(v), left(v){};
		Inset(float tb, float lr) : top(tb), right(lr), bottom(tb), left(lr) {};
		Inset(float top, float right, float bottom, float left) : top(top), right(right), bottom(bottom), left(left) {}; 
	};
	
	// forward declare all classes not added directly
	class BarButton;
	class Button;
	class Container; 
	class Image;
	class InternalWindow;
	class Label;
	class Layout;
	class Root;
	class ScrollPane;
	
	template<typename T>
	class SegmentedSelect;
	
	class Slider;
	class SliderWithLabel;
	class TextField;
	class TextureAtlas;
	class ToggleButton;
}

// now include bare minimum
#include "MuiConfig.h"
#include "MuiEvents.h"
#include "Helpers.h"
#include "Layout.h"
#include "Container.h"



// wtf windows...
// https://code.google.com/p/py-lepton/source/browse/trunk/lepton/renderer.h?r=222
// um... looks like this can be cleaned up by paying more attention to float/double
#ifndef fminf
#define fminf(x, y) (((x) < (y)) ? (x) : (y))
#endif
#ifndef fmaxf
#define fmaxf(x, y) (((x) < (y)) ? (y) : (x))
#endif
#ifndef roundf
#define roundf(x) (x >= 0.0f ? floorf(x + 0.5f) : ceilf(x - 0.5f))
#endif



#endif /* defined(__Oscilloscope__MuiCore__) */
