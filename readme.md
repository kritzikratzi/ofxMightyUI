ofxMightyUI
===========

Documentation is not exactly up to date. 


## About

A handy UI addon for [openFrameworks](http://www.openframeworks.cc) Version 0.10. 


This is supposed to be a small but flexible UI library that runs cross-platform
with multitouch support. 

The library aims to only provide a minimal set of elements needed for basic interactive artworks:
sliders, segmented controls, buttons, labels, toolbars/navigation bars, internal windows, textfields

ofxMightyUI is a personal project and developed for my specific needs. 

## Screenshots


<img alt="OsciStudio Screenshot" src="docs/screenshot-oscistudio.png" width="100%">
<img alt="Oscilloscope Screenshot" src="docs/screenshot-oscilloscope.png" width="100%">




## Features


This is very much a work in progress. The following works/worked/should work: 

- Internal Windows (draggable, with titles and optional left/right toolbar buttons)
- Labels (with all sorts of alignments)
- Sliders (multitouch enabled, optionally with value labels)
- Buttons
- Textfields (based on stb_textedit)
- Scrollpanes
- Segmented buttons (the equivalent of radio buttons in a way)
- Retina support 


## Setup


Download the current master branch for ofxMightyUI and ofxFontStash, either as zip or clone from git with

	cd OF/addons
	git clone https://github.com/kritzikratzi/ofxMightyUI.git
	git clone https://github.com/kritzikratzi/ofxFontStash2.git


### All platforms

Edit `addons.make` and add this line: 

	ofxMightyUI
	ofxFontStash2


Then regenerate your project. If you get missing fonts, then double check that the folder addons/ofxMighty/bin/data gets copied to your bin folder. 


## Usage

There's a longer example in the `src/` folder. 



**ofApp.h**

	#include "MUI.h"
	
	class testApp{
		// Root element to which you can add your UI elements
		mui::Root *root;
		
		mui::Button * button;
		mui::SliderWithLabel * slider; 
		
		// callback used for the button onPress
		void onButtonPress( const void* sender, ofTouchEventArgs &args ); 
	}
	
**ofApp.cpp**

	void testApp::setup(){	
		...
		
		// create root element
		// this attaches to OF and handles draw/update/mouse/key/... by itself. 
		root = new mui::Root();
		
		// create button and register add listener
		button = new mui::Button( "Button", 20, 200, 70, 30 ); 
		ofAddListener( button->onPress, this, &testApp::onButtonPress );
		root->add( button ); 
	}
	
	//--------------------------------------------------------------
	void testApp::onButtonPress( const void* sender, ofTouchEventArgs &args ){
		cout << "the button was pressed!" << endl; 
	}

## Components


|Component|Description|
|---------|---|
|Container|Basis for everything. Has (optional) opaque background and takes care of handling touch and key events|
|Root|Takes care of embeding ofxMightyUI into an openFrameworks base class|
|Label|A text label with vertical and horizontal alignment and text color|
|Button|A clickable button with an onPress eventlistener. Contains a label|
|ToggleButton|Same as button, but has two states (selected=true/false)|
|InternalWindow||
|ScrollPane|Takes care of clipping and efficiently display lots of items. Supports scroll wheel and touch scroll, optional paging and autoscroll (for logs). |
|SegmentedSelected|A set of buttons to choose between various options (e.g. color=red/green/blue). Uses templates to attach data to the options|
|Slider|A simple horizontal slider|
|SliderWithLabel|A slider, with a label that displays the current value.|
|TextField|Deprecated! Native textfield overlay. Will be gone soon.|
|TextArea|In progress, unusable. TextField replacement|



## Layouting 

Especially more complex layouts can be tricky to get right. ofxMightyUI uses a very straight forward approach without springs, layout managers or bindings. 

Here is a simple example. We have a form with two labels+sliders (hue and size). 

	#include "MuiL.h" // L for layout
	
	class MyForm : public mui::Container{
		mui::Label * sizeLabel; 
		mui::Slider * sizeSlider; 
		
		mui::Label * hueLabel; 
		mui::Slider * sizeSlider; 
		
		MyForm(){
			sizeLabel = mui::Label("Size:", 0,0,100,30); 
			add(sizeLabel); 
			
			sizeSlider = new mui::Slider(0,0,100,30, 100,200,150 ); // a slider 100x30 large, values 100.200, default 150
			add(sizeSlider); 
			
			hueLabel = new mui::Label("Hue:", 0,0, 100,30); 
			add(hueLabel); 
			
			hueSlider = new mui::Slider(0,0,100,30, 100,200,150 ); // a slider 100x30 large, values 100.200, default 150
			add(hueSlider); 
		}
		
		~MyForm(){
			delete sizeLabel; 
			delete sizeSlider; 
			delete hueLabel; 
			delete hueSlider; 
		}
		
		// call on the first frame, and whenever the size of the window changes
		void layout(){
			// move the size label 10 px from top left
			mui::L(sizeLabel).posTL(10,10); 
			
			// place the slider next to the label (5px space), and use all available width (minus ten pixels)
			mui::L(sizeSlider).rightOf(sizeLabel, 5); 
			
			// place the hue label below the size label, leaving a 5 px space. 
			mui::L(hueLabel).below(sizeLabel,5); 
			
			// and the hue slider next to the hue label. 
			mui::L(hueSlider).rightOf(hueLabel, 5); 
			
			// in this case, we want our own size 
			ofRectangle size = getChildBounds(); 
			width = size.width; 
			height = size.height; 
		}
	};



## Events/Listeners

All events (update,draw,mouse,touch,keyboard,layouting) can be intercepted with lambda functions. 

See `Container.h` for a full list. 

	// create a button with a border 
	mui::Button * button = new mui::Button(); 
	button->onDraw.add([button](){
		// red when over, otherwise gray
		if(isMouseOver()) ofSetColor(255,0,0); 
		else ofSetColor(150); 
		
		ofDrawRectangle(0,0,button->width,button->height); 
		ofSetColor(255); // always reset to white after drawing!
	});
	


## Licensing 

Third party software/assets used: 

- ofxFontStash2 / fontstash (MIT license?)
- [CppTweener](http://code.google.com/p/tweener/): MIT License
- [iPhone4 GUI for Photoshop](http://www.teehanlax.com/blog/2010/08/12/iphone-4-gui-psd-retina-display/) cc-sa-by
- [Lato Regular font](http://www.fontsquirrel.com/fonts/lato): Sil open font license
 
The project itself is released under the [MIT](https://opensource.org/licenses/MIT) License. 

