/*
 *  Layout.cpp
 *  iPhoneEmptyExample
 *
 *  Created by hansi on 26.03.11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "Layout.h"



void mui::RowLayout::layout( Container * container ){
	vector<Container*> * children = &container->children; 
	vector<Container*>::iterator it = children->begin(); 
	int y = 0; 
	
	while( it != children->end() ){
		cout << " Y = " << y << endl; 
		(*it)->x = 0;
		(*it)->y = y;
		y += (*it)->height + spacing;
		
		++it; 
	}
}