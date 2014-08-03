ofxMightyUI
===========


Features
--------

this is very much a work in progress. the following sorta works: 

- windows (draggable, with titles and optional left/right toolbar buttons)
- labels (with all sorts of alignments)
- sliders (multitouch enabled, optionally with value labels)
- buttons
- textfields (operating system textfield is overlayed during editing so it allows copy&paste)
- scrollpanes
- segmented buttons (the equivalent of radio buttons in a way)
it's all butt ugly though. also eventually slow! 
- retina support 


Setup
-----
The included project is broken at the moment (sorry). 
To get up and running grab the empty example from OF080. I suggest you add mui as a submodule: 

	git submodule add git@github.com:kritzikratzi/ofxMightyUI.git
	
In XCode:

- Add the folder ofxMightyUI/addons to your XCode project
- Go to Project>Targets>emptyExample>Build Phases>Link Binary with Libraries. Click the '+' icon and add the CoreText framework. 


Usage
-------
There's an example in the `src/` folder. 


Licensing 
---------

- CppTweener: MIT License, http://code.google.com/p/tweener/
- image assets in data/mui: cc-sa-by, http://www.teehanlax.com/blog/2010/08/12/iphone-4-gui-psd-retina-display/
- lato-regular type: sil open font license, http://www.fontsquirrel.com/fonts/lato
- ofxFBO: not mentioned, need to check! 
 
the rest is released under the wtfpl: 
http://sam.zoy.org/wtfpl/


in the long run this is supposed to be a very very small 
ui library that runs cross-platform (mac os+ios at least)
with multitouch support. 

the library aims to only provide a minimal set of elements 
needed for basic interactive artworks:
sliders, segmented controls, buttons, labels, 
toolbars/navigation bars, internal windows, textfields

the library is  (and most likely won't be suitable)
as neither a proper gui replacement or as a scenegraph. 
there's already lots of other people doing that.  
