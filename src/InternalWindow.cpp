/*
 *  InternalWindow.cpp
 *
 *  Created by hansi on 29.01.11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 */

#include "InternalWindow.h"
#include "Label.h"
#include "BarButton.h"

#include <algorithm>

mui::InternalWindow::InternalWindow( std::string title, float x, float y, float width, float height, float radius ) :
Container( x, y, width, height ), radius(radius), leftBarButton(nullptr), rightBarButton(nullptr), view(nullptr), padding(10) {
	label = new Label( title, 0, 0, width, 44 ); 
	label->horizontalAlign = Center; 
	label->verticalAlign = Middle; 
	label->fg.r = label->fg.g = label->fg.b = 255; 
	label->fontSize = 16; 
	label->commit(); 
	
	name = string("internalwindow-") + title; 
	
	add( label ); 
	
	view = new Container( 0, 44, width, height - 44 ); 
	add( view ); 
	view->name = "view-" + title; 
}

mui::InternalWindow::~InternalWindow(){
	delete label;
	delete view;
}

//--------------------------------------------------------------
void mui::InternalWindow::update(){
	label->width = width;
	view->x = padding; 
	view->y = 44 + padding; 
	view->width = width - 2*padding; ;
	view->height = height - 44 - 2*padding;
	
	if( leftBarButton != NULL ){
		leftBarButton->width = leftBarButton->label->boundingBox.width + 24;
		leftBarButton->height = 44;
		leftBarButton->x = leftBarButton->y = 0;
	}
	
	if( rightBarButton != NULL ){
		rightBarButton->width = rightBarButton->label->boundingBox.width + 24;
		rightBarButton->height = 44;
		rightBarButton->x = width - rightBarButton->width;
		rightBarButton->y = 0;
	}
}


//--------------------------------------------------------------
void mui::InternalWindow::draw(){	
}


//--------------------------------------------------------------
void mui::InternalWindow::drawBackground(){
	ofSetColor( bg );
	ofFill(); 
	Helpers::roundedRect( 0, 0, width, height, radius );
	ofNoFill(); 
	//Helpers::roundedRect( 1, 22, width-2, height-22, radius ); 
	
	ofSetColor( 255 );
	Helpers::beginImages();
	Helpers::drawImage( "titlebar_left", 0, 0, 5, 44 );
	Helpers::drawImage( "titlebar_center", 5, 0, width - 9, 44 );
	Helpers::drawImage( "titlebar_right", width-5, 0, 5, 44 );
	Helpers::endImages();
	ofFill();
}


//--------------------------------------------------------------
void mui::InternalWindow::setLeftBarButton( BarButton * leftBarButton ){
	if( this->leftBarButton != NULL ){
		//children.remove( leftBarButton );
		vector<Container*>::iterator it = find( children.begin(), children.end(), this->leftBarButton ); 
		if( it != children.end() ) children.erase( it ); 
	}
	
	add( leftBarButton ); 
	this->leftBarButton = leftBarButton; 
}


//--------------------------------------------------------------
void mui::InternalWindow::setRightBarButton( BarButton * rightBarButton ){
	if( this->rightBarButton != NULL ){
		vector<Container*>::iterator it = find( children.begin(), children.end(), this->rightBarButton ); 
		if( it != children.end() ) children.erase( it ); 
	}
	
	add( rightBarButton ); 
	this->rightBarButton = rightBarButton; 
}


//--------------------------------------------------------------
void mui::InternalWindow::touchDown( ofTouchEventArgs &touch ){
	dragX = touch.x; 
	dragY = touch.y; 
}


//--------------------------------------------------------------
void mui::InternalWindow::touchMoved( ofTouchEventArgs &touch ){
	float deltaX = touch.x - dragX;
	float deltaY = touch.y - dragY;
	x += deltaX; 
	y += deltaY; 
}


//--------------------------------------------------------------
void mui::InternalWindow::touchMovedOutside( ofTouchEventArgs &touch ){
	touchMoved( touch );
}


//--------------------------------------------------------------
void mui::InternalWindow::touchUp( ofTouchEventArgs &touch ){
}


//--------------------------------------------------------------
void mui::InternalWindow::touchDoubleTap( ofTouchEventArgs &touch ){
}