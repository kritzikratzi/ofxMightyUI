//
//  ofxiOSEAGLView+retinaPatch.h
//  emptyExample
//
//  Created by Oriol Ferrer Mesià on 21/01/13.
//
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "EAGLView.h"
//#import "ofxiOSEAGLView.h"


@interface ofxiOSEAGLView (retinaPatch)

- (void) updateDimensions ;
- (void) drawView ;

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event;
- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event;
- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event;
- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event;


@end
