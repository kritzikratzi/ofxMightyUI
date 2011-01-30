/*
 *  ButtonPopup.mm
 *  iPhoneEmptyExample
 *
 *  Created by hansi on 16.12.10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 * OLD! IGNORE! 
 */

#include "ButtonPopup.h"


ButtonPopup::ButtonPopup(){
}


void ButtonPopup::draw(){
	
	int width = 150; 
	int height = 30; 
	
	ofTranslate( 50.5, 50.5, 0 ); 
	
	ofFill(); 
	ofSetColor( 0, 0, 0, 0 ); 
	mui::Helpers::roundedRect( 0, 0, width, height, 3 ); 
	ofBeginShape(); 
	ofVertex( width/2+2, height ); 
	ofVertex( width/2, height+3 ); 
	ofVertex( width/2-2, height );
	ofEndShape( true );
	
	ofNoFill(); 
	ofSetColor( 255, 255, 255 ); 
	mui::Helpers::roundedRect( 0, 0, width, height, 3 ); 
	ofBeginShape(); 
	ofVertex( width/2+2, height ); 
	ofVertex( width/2, height+3 ); 
	ofVertex( width/2-2, height ); 
	ofEndShape( false );
	ofSetColor( 0, 0, 0 ); 
	ofLine( width/2-1, height, width/2+1, height ); 
}