/*
 *  SaveWindow.cpp
 *  iPhoneEmptyExample
 *
 *  Created by hansi on 28.02.11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "MUI.h"
#include "LoadWindow.h"



void LoadWindow::init(){
	loadItemsScroller = new mui::ScrollPane( 5, 0, 130, view->height ); 
	view->add( loadItemsScroller );
	
	
	float y = 10; 
	
	// Add the your files label and buttons ... 
	yourFilesLabel = new mui::Label( "Your Files", 0, y, 130, 25 ); 
	yourFilesLabel->fontSize = 10; 
	yourFilesLabel->fg = mui::Helpers::grey( 0 ); 
	yourFilesLabel->commit(); 
	loadItemsScroller->view->add( yourFilesLabel );
	y += 25; 
	
	for( int i = 0; i < 10; i++ ){
		mui::Button * b = new mui::Button( "  some file (" + ofToString( i+1, 0 ) + ")", 0, y, 130, 25 ); 
		b->label->horizontalAlign = mui::Left; 
		b->label->fontSize = 10; 
		b->label->fg = mui::Helpers::grey( 75 ); 
		b->label->commit(); 
		b->bg = mui::Helpers::grey( 255, 0 ); 
		loadItemsScroller->view->add( b ); 
		y += 25; 
	}
	
	y += 15; 
	
	// Add the template files label and buttons ... 
	templatesLabel = new mui::Label( "Templates", 0, y, 130, 25 ); 
	templatesLabel->fontSize = 10; 
	templatesLabel->fg = mui::Helpers::grey( 0 ); 
	templatesLabel->commit(); 
	loadItemsScroller->view->add( templatesLabel ); 
	y += 25; 
	
	for( int i = 0; i < 10; i++ ){
		mui::Button * b = new mui::Button( "  template (" + ofToString( i+1, 0 ) + ")", 0, y, 130, 25 ); 
		b->label->horizontalAlign = mui::Left; 
		b->label->fontSize = 10; 
		b->label->fg = mui::Helpers::grey( 75 ); 
		b->label->commit(); 
		b->bg = mui::Helpers::grey( 255, 0 ); 
		loadItemsScroller->view->add( b ); 
		y += 25; 
	}
	
	loadItemsScroller->commit();
}



void LoadWindow::show(){
}

void LoadWindow::drawBackground(){
	InternalWindow::drawBackground(); 
	
	ofSetColor( 0xCCCCCC ); 
	ofLine( loadItemsScroller->width + 5, view->y, loadItemsScroller->width + 5, view->y + view->height ); 
}
