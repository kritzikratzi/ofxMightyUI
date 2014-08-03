//
//  IOSWrapper.mm
//  iPhoneEmptyExample
//
//  Created by hansi on 25.02.11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "MUI.h"
#import "NativeIOSWrapper.h"
#include "NativeIOS.h"
#include "ofxIPhoneExtras.h"


@implementation NativeIOSWrapper
-(void)viewDidLoad {
	self.view.hidden = YES;
	textField.borderStyle = UITextBorderStyleNone; 
	self.view.frame = CGRectMake( 0, 0, ofGetWidth(), ofGetHeight() );
	
	
	// thanks to http://iphonedevelopertips.com/user-interface/adjust-textfield-hidden-by-keyboard.html
	// for explaining how to figure out where the keyboard is
	[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector (keyboardDidShow:) name: UIKeyboardDidShowNotification object:nil];
	[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector (keyboardDidHide:) name: UIKeyboardDidHideNotification object:nil];	
}

-(IBAction) editingEnded {
	mui::NativeIOS::hide(); 
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
    UITouch *touch = [touches anyObject];
    if( touch.view == self.view ){
        mui::NativeIOS::hide();
    }
    else{
    }
}


// and again: 
// thanks to http://iphonedevelopertips.com/user-interface/adjust-textfield-hidden-by-keyboard.html
-(void) keyboardDidShow: (NSNotification *)notif {
	// Get the size of the keyboard.
	NSDictionary* info = [notif userInfo];
	NSValue* aValue = [info objectForKey:UIKeyboardBoundsUserInfoKey];
	CGSize keyboardSize = [aValue CGRectValue].size;
	
	mui::NativeIOS::adjustRootPosition( keyboardSize.height ); 
}

-(void) keyboardDidHide: (NSNotification *)notif {
	mui::NativeIOS::hide(); 
}


@end
