/*
 *  ofxMightyUI.h
 *
 *  by hansi, 2011-2018. https://github.com/kritzikratzi/ofxMightyUI
 *  MIT License
 */

#pragma once


#include "MuiCore.h"

#include "ScrollPane.h"
#include "Label.h"
#include "Root.h"
#include "Slider.h"
#include "Button.h"
#include "ToggleButton.h"
#include "BarButton.h"
#include "EmptyContainer.h"
#include "InternalWindow.h"
#include "SliderWithLabel.h"
#include "SegmentedSelect.h"
#include "Image.h"

void mui_init();

ofVec2f muiGetMousePos();
int muiGetMouseX();
int muiGetMouseY();
float muiGetWidth();
float muiGetHeight();
float muiGetDefaultDisplayScaling();

string muiToDataPath(string path, bool abs = true );
