//
//  NativeOSXWrapper.m
//  OSX
//
//  Created by Hansi on 25.08.14.
//
//

#import "NativeOSXWrapper.h"
#include "NativeOSX.h"

@interface NativeOSXWrapper ()

@end

@implementation NativeOSXWrapper

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Initialization code here.
    }
    return self;
}

-(void)viewDidLoad {
	self.view.hidden = YES;
	[textField setBordered:NO];
	// TODO: get this in there 
//	self.view.frame = NSMakeRect( 0, 0, ofGetWidth(), ofGetHeight() );
}

-(IBAction) editingEnded {
	mui::NativeOSX::hide();
}

//TODO:
/*- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
	// onclick, or something
}*/


@end
