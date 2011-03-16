//
//  IOS.m
//  iPhoneEmptyExample
//
//  Created by hansi on 25.02.11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "MUI.h"
#include "ofxiPhone.h"
#include "ofxiPhoneExtras.h"
#include "NativeIOSWrapper.h"


NativeIOSWrapper * mui_native_ios;

mui::TextField * mui::NativeIOS::textField = NULL; 

void mui::NativeIOS::init(){
	cout << "doing init..." << endl; 
	mui_native_ios = [[NativeIOSWrapper alloc] initWithNibName:@"NativeIOSWrapper" bundle:nil];
	[ofxiPhoneGetUIWindow() addSubview:mui_native_ios.view];
}


void mui::NativeIOS::showTextField( TextField * tf ){
	hide(); 
	
	tf->visible = false; 
	
	textField = tf;
	ofPoint position = tf->getGlobalPosition(); 

	//mui_native_ios->textField.frame = CGRectMake( position.x, position.y, tf->width, tf->height );
	mui_native_ios->textField.frame = CGRectMake( 0, 0, tf->width, tf->height );
	mui_native_ios.view.frame = CGRectMake( position.x, position.y, tf->width, tf->height );
	mui_native_ios.view.hidden = false;

	[mui_native_ios->textField setText:[NSString stringWithCString: tf->text.c_str()]];
	[mui_native_ios->textField becomeFirstResponder];
}


void mui::NativeIOS::hide(){
	if( textField != NULL ){
		textField->visible = true; 
		textField->text = string( [mui_native_ios->textField.text UTF8String] );
		textField->commit(); 
		
		[mui_native_ios->textField resignFirstResponder]; 
		mui_native_ios.view.hidden = true;
	}
}
