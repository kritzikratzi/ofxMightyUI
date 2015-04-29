ofxMightyUI
===========

INSTRUCTIONS ARE OUT OF DATE. 
---
LOTS OF REFACTORING GOING ON. 
THIS IS UNUSABLE AT THE MOMENT. 
PLEASE BE PATIENT. 


TODO: 

- ☑︎ switch from poco events to ofEvent (same behind the scenes, but nicer to use)
- ☑︎ moving files around to follow ofx addon standard. already sortof works with project generator. hurray! 
- ☐ hm... seems the project generator doesn't set up copying of data files? 
- ☐ test android (+retina)
- ☐ test ios (+retina)
- ☐ test windows (is there retina?)
- ☐ finish textfield overlays
- ☐ stupid rotation issues
- ☐ see if i can depend on the "proper" ofxFontStash
- ☐ cleanup include structure. how does it even compile? 
- ☑︎ make a basic example
- ☐ make a more complex example
- ☐ share code with the midi mapping? --> could be optional addon?

TODO OSX: 

- what is needed for the user to enable retina? [should be supported]
- to copy resources: ```cp -R ../../../addons/ofxMightyUI/bin/data/* "$TARGET_BUILD_DIR/$PRODUCT_NAME.app/Contents/Resources/"``` can this be done with projectgen? 


---


A handy UI addon for [openFrameworks](http://www.openframeworks.cc). 

Features
--------

This is very much a work in progress. The following works: 

- Windows (draggable, with titles and optional left/right toolbar buttons)
- Labels (with all sorts of alignments)
- Sliders (multitouch enabled, optionally with value labels)
- Buttons
- Textfields (operating system textfield is overlayed during editing so it allows copy&paste)
- Scrollpanes
- Segmented buttons (the equivalent of radio buttons in a way)
It's all a bit ugly still. Also eventually slow! 
- Retina support 


Setup
-----
The included project is broken at the moment (sorry). 
To get up and running grab the empty example from OF080. I suggest you add ofxMightyUI as a submodule: 

	git submodule add git@github.com:kritzikratzi/ofxMightyUI.git

To update to a newer of ofxMightyUI run

	git submodule foreach git pull

**iOS**

- Add the folder ofxMightyUI/addons to your XCode project
- Go to Project>Targets>emptyExample>Build Phases>Link Binary with Libraries. Click the '+' icon and add the CoreText framework. 
- Go to Project>Targets>emptyExample>Build Phases>Run Script and add <br>
  	```cp -rf ofxMightyUI/bin/data/ "$TARGET_BUILD_DIR/$PRODUCT_NAME.app"```

**MacOS X**

- Add the folder ofxMightyUI/addons to your XCode project, don't add ofxEasyRetina. 
- Go to Project>Targets>emptyExample>Build Phases>Run Script and add <br>
  	```cp -rf ofxMightyUI/bin/data/ "$TARGET_BUILD_DIR/$PRODUCT_NAME.app/Contents/Resources"```
  	
**Windows**

- Create a groups (filters) for ofxMightyUI and subgroups for ofxMUI and ofxFontStash
- Add the .cpp and .h from those folders to their respective groups
- Go to Project>Properties, choose "All configurations" from the dropdown on top
- Go to C/C++>General and edit additional include directories. Add the following<br>
		```ofxMightyUI\addons\ofxMUI```<br>
		```ofxMightyUI\addons\ofxFontStash```

- Go to Build Events>Post-Build Events and edit command line. <br>
  		```xcopy /e /i /y "$(ProjectDir)..\..\..\export\vs\*.dll" "$(ProjectDir)bin"```<br>
  		```xcopy /e /i /y "$(ProjectDir)ofxMightyUI\bin\data\mui" "$(ProjectDir)bin\data\mui"```

**Android**

- Refresh your project view (press F5), the ofxMightyUI subproject should show up
- Right click>Resource Configuration>Exclude From Build>Release and Debug for these folders: 
	- ofxMightyUI/addons/ofxFBO
	- ofxMightyUI/addons/ofxEasyRetina
	- ofxMightyUI/addons/ofxMUI/native
	- ofxMightyUI/addons/ofxMUI/native-osx
	- ofxMightyUI/src
	
Also find the excludes section in config.make and add those same folders: 

	PROJECT_EXCLUSIONS = $(PROJECT_ROOT)/ofxMightyUI/addons/ofxFBO
	PROJECT_EXCLUSIONS += $(PROJECT_ROOT)/ofxMightyUI/addons/ofxEasyRetina
	PROJECT_EXCLUSIONS += $(PROJECT_ROOT)/ofxMightyUI/addons/ofxMUI/native
	PROJECT_EXCLUSIONS += $(PROJECT_ROOT)/ofxMightyUI/addons/ofxMUI/native-osx
	PROJECT_EXCLUSIONS += $(PROJECT_ROOT)/ofxMightyUI/src

Usage
-------
There's a longer example in the `src/` folder. 

**testApp.h**

	#include "MUI.h"
	
	class testApp{
		// Root element to which you can add your UI elements
		mui::Root *root;
		
		mui::Button * button;
		mui::SliderWithLabel * slider; 
		
		// callback used for the button onPress
		void onButtonPress( const void* sender, ofTouchEventArgs &args ); 
	}
	
**testApp.cpp**

	void testApp::setup(){	
		...
		
		// create root element
		root = new mui::Root();
		
		// create button and register add listener
		button = new mui::Button( "Button", 20, 200, 70, 30 ); 
		ofAddListener( button->onPress += this, &testApp::onButtonPress );
		root->add( button ); 
	}
	
	//--------------------------------------------------------------
	void testApp::onButtonPress( const void* sender, ofTouchEventArgs &args ){
		cout << "the button was pressed!" << endl; 
	}

Licensing 
---------

Third party software/assets used: 

- ofxFontStash / fontstash (MIT license?)
- ofxEasyRetina (would be cool if this stayed external)
- [CppTweener](http://code.google.com/p/tweener/): MIT License
- [iPhone4 GUI for Photoshop](http://www.teehanlax.com/blog/2010/08/12/iphone-4-gui-psd-retina-display/) cc-sa-by
- [Lato Regular font](http://www.fontsquirrel.com/fonts/lato): Sil open font license
 
The project itself is released under [WTFPL](http://sam.zoy.org/wtfpl/). 

About
-----

In the long run this is supposed to be a small but flexible 
UI library that runs cross-platform
with multitouch support. 

The library aims to only provide a minimal set of elements 
needed for basic interactive artworks:
sliders, segmented controls, buttons, labels, 
toolbars/navigation bars, internal windows, textfields

ofxMightyUI is (and most likely won't be suitable)
as neither a proper GUI replacement or as a scenegraph. 
turns out: it is a neat little scenegraph. so maybe yes. 

