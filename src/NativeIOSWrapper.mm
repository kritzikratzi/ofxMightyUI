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
}

-(IBAction) editingEnded {
	mui::NativeIOS::hide(); 
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
    // assign a UITouch object to the current touch
    //UITouch *touch = [[event allTouches] anyObject];
	//cout << "AHA!" << endl; 
	mui::NativeIOS::hide(); 
}


@end
