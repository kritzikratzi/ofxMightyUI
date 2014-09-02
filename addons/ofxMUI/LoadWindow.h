/*
 *  SaveWindow.h
 *  iPhoneEmptyExample
 *
 *  Created by hansi on 28.02.11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef TESTAPP_UICONTROLLER
#define TESTAPP_UICONTROLLER


class LoadWindow : public mui::InternalWindow{
public:
	LoadWindow() : mui::InternalWindow( "Load", 0, 0, 320, 400 ){ init(); }; 
	
	void init();
	void show();
	virtual void drawBackground(); 
	
	
	mui::InternalWindow * loadWindow;
	mui::ScrollPane * loadItemsScroller;
	
	mui::Label * yourFilesLabel; 
	mui::Label * templatesLabel; 
	
	vector<mui::Button*> yourFiles; 
	vector<mui::Button*> templateFiles; 
};


#endif