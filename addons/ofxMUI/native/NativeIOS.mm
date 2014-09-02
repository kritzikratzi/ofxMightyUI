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
#include <CoreText/CoreText.h>


NativeIOSWrapper * mui_native_ios;

mui::TextField * mui::NativeIOS::textField = NULL; 

void mui::NativeIOS::init(){
	cout << "doing init..." << endl; 
	mui_native_ios = [[NativeIOSWrapper alloc] initWithNibName:@"NativeIOSWrapper" bundle:nil];
	UIView * view = ofxiOSGetGLParentView();
	[view addSubview:mui_native_ios.view];
}

void mui::NativeIOS::update(){
	if( textField != NULL ){
		ofPoint position = textField->getGlobalPosition(); 
		// landscape mode 
//		mui_native_ios->textField.frame = CGRectMake( position.x, position.y, textField->width, textField->height );
//		mui_native_ios->textField.frame = CGRectMake( position.x, position.y, textField->width, textField->height );
		mui_native_ios->textField.frame = CGRectMake( 0, 0, textField->width, textField->height );
//		mui_native_ios.view.frame = CGRectMake( 0, 0, ofGetWidth(), ofGetHeight() );
	}
}



void mui::NativeIOS::showTextField( TextField * tf ){
	hide();
	
	tf->visible = false; 
	
	textField = tf;
	ofPoint position = tf->getGlobalPosition(); 

	// load native font ...
	;
	UIFont *font = NULL;
	if( font == NULL ){
		
		NSString* fpath = ofxiOSStringToNSString( ofToDataPath( "mui/fonts/Minecraftia.ttf" ) );
		NSLog( @"font path = %@", fpath ); 
		CGDataProviderRef fontDataProvider = CGDataProviderCreateWithFilename([fpath UTF8String]);
		CGFontRef customFont = CGFontCreateWithDataProvider(fontDataProvider);
		CGDataProviderRelease(fontDataProvider);
//		NSString *fontName = (NSString *)CGFontCopyFullName(customFont);
//		NSString * fontName = (__bridge NSString *)CGFontCopyFullName(customFont);
		CFErrorRef error = NULL;
		CTFontManagerRegisterGraphicsFont(customFont, &error);
		if (error != NULL)
		{
			CFStringRef errorDesc = CFErrorCopyDescription(error);
			NSLog(@"Font not loaded: %@", errorDesc);
			CFRelease(errorDesc);
		}
		
		CGFontRelease(customFont);
		font = [UIFont fontWithName:@"Minecraftia" size:12];
	}
	
	mui_native_ios->textField.font = [font fontWithSize:1.0*tf->fontSize];
//	mui_native_ios->textField.font = [UIFont systemFontOfSize:1.4*tf->fontSize];
	mui_native_ios->textField.frame = CGRectMake( position.x, position.y, tf->width, tf->height );
//	mui_native_ios->textField.frame = CGRectMake( 0, 0, tf->width, tf->height );
	
	// use this for landscape right mode: 
	mui_native_ios.view.layer.anchorPoint = CGPointMake( 0, 0 ); 
	mui_native_ios.view.center = CGPointMake( 0, 0 );
	mui_native_ios.view.frame = CGRectMake( tf->x, tf->y, ofGetWidth(), ofGetHeight() );
	CGAffineTransform rotationTransform = CGAffineTransformIdentity;
	rotationTransform = CGAffineTransformRotate(rotationTransform,-DEG_TO_RAD*90);

//	mui_native_ios.view.transform = rotationTransform;
	
	// and this for portrait
//	mui_native_ios.view.frame = CGRectMake( position.x, position.y, tf->width, tf->height );
	
	mui_native_ios.view.hidden = false;

	[mui_native_ios->textField setTextColor: [UIColor colorWithRed:tf->fg.r/255.0 green:tf->fg.g/255.0 blue:tf->fg.b/255.0 alpha:tf->fg.a/255.0 ]];
	[mui_native_ios->textField setBackgroundColor:[UIColor colorWithRed:0 green:1 blue:0 alpha:1]];
	[mui_native_ios->textField setText:[NSString stringWithCString: tf->text.c_str() encoding:NSUTF8StringEncoding]];
	[mui_native_ios->textField becomeFirstResponder];
	//[mui_native_ios->textField selectAll:mui_native_ios->textField];
	[mui_native_ios->textField setClearButtonMode:UITextFieldViewModeWhileEditing];
}

void mui::NativeIOS::adjustRootPosition( int keyboardHeight ){
	if( textField != NULL ){
		ofPoint position = textField->getGlobalPosition(); 
		if( position.y + textField->height > ofGetHeight() - keyboardHeight ){
			Root::INSTANCE->prepareAnimation( 200 );
			Root::INSTANCE->animate( Root::INSTANCE->y, Root::INSTANCE->y - position.y - textField->height + ofGetHeight() - keyboardHeight );
			Root::INSTANCE->commitAnimation(); 
		}
	}
}


void mui::NativeIOS::hide(){
	if( textField != NULL ){
		textField->setText( string( [mui_native_ios->textField.text UTF8String] ) ); 
		textField->visible = true; 
//		textField->text = string( [mui_native_ios->textField.text UTF8String] );
//		textField->commit();
		
		[mui_native_ios->textField resignFirstResponder];
		mui_native_ios.view.hidden = true;
		
		Root::INSTANCE->prepareAnimation( 300 ); 
		Root::INSTANCE->animate( Root::INSTANCE->y, 0 );
		Root::INSTANCE->commitAnimation();
	}
}