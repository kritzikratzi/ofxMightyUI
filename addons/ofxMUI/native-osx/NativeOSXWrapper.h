//
//  NativeOSXWrapper.h
//  OSX
//
//  Created by Hansi on 25.08.14.
//
//

#import <Cocoa/Cocoa.h>

@interface NativeOSXWrapper : NSViewController{
	@public IBOutlet NSTextField * textField;
}
-(IBAction) editingEnded;

@end
