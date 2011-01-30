/*
 *  Root.h
 *  iPhoneEmptyExample
 *
 *  Created by hansi on 28.01.11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef MUI_ROOT
#define MUI_ROOT


#include "Container.h"

namespace mui{

	class Root : public Container {
	public: 
		Root() : Container( 0, 0, ofGetWidth(), ofGetHeight() ){};
		virtual void handleDraw(); 
/*		virtual bool handleTouchDown( ofTouchEventArgs &touch );
		virtual bool handleTouchMoved( ofTouchEventArgs &touch );
		virtual bool handleTouchUp( ofTouchEventArgs &touch );
		virtual bool handleTouchDoubleTap( ofTouchEventArgs &touch );*/
		
	};
}


#endif
