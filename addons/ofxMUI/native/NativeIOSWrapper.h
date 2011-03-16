//
//  IOSWrapper.h
//  iPhoneEmptyExample
//
//  Created by hansi on 25.02.11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

@interface NativeIOSWrapper : UIViewController {
	@public IBOutlet UITextField * textField; 
}

-(IBAction) editingEnded; 
@end

