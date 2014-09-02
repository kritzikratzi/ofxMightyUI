//
//  OSX.h
//  iPhoneEmptyExample
//
//  Created by hansi on 25.02.11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#ifndef MUI_NATIVE_OSX
#define MUI_NATIVE_OSX

namespace mui{
	class TextField; 
	
	class NativeOSX{
	public: 
		static void init(); // don't call this, mui::Root does! 
		
		static void showTextField( TextField * tf );
		static void hide();
		static void update(); 
		
		static void adjustRootPosition( int keyboardHeight );  // don't call this either, mui::NativeOSXWrapper does!
		
	private:
		static TextField * textField;

	};
	
	
}

#endif