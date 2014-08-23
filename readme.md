ofxMightyUI
===========

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
	
In XCode:

- Add the folder ofxMightyUI/addons to your XCode project
- Go to Project>Targets>emptyExample>Build Phases>Link Binary with Libraries. Click the '+' icon and add the CoreText framework. 
- Go to Project>Targets>emptyExample>Build Phases>Run Script and add <br>
  	```cp -rf ofxMightyUI/bin/data/ "$TARGET_BUILD_DIR/$PRODUCT_NAME.app"```

Usage
-------
There's an example in the `src/` folder. 


Licensing 
---------

Third party software/assets used: 

- [CppTweener](http://code.google.com/p/tweener/): MIT License
- [iPhone4 GUI for Photoshop](http://www.teehanlax.com/blog/2010/08/12/iphone-4-gui-psd-retina-display/) cc-sa-by
- [Lato Regular font](http://www.fontsquirrel.com/fonts/lato): Sil open font license
- [ofxFBO](https://code.google.com/p/kyle/source/browse/trunk/openframeworks/addons/ofxFbo/src/ofxFbo.h?r=83): not mentioned, need to check! 
 
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
there's already lots of other people doing that. I think. 
