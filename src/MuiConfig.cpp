//
//  MuiConfig.cpp
//  Oscilloscope
//
//  Created by Hansi on 10.08.15.
//
//

#include "MuiConfig.h"


std::string mui::MuiConfig::font = "mui/fonts/Lato-Regular.ttf";
bool mui::MuiConfig::debugDraw = false;
int mui::MuiConfig::scrollPaneBleed = 30;
int mui::MuiConfig::scrollToBaseDuration = 600;
int mui::MuiConfig::scrollVelocityDecrease = 300;
int mui::MuiConfig::fontAtlasSize = 2048; 
int mui::MuiConfig::fontSize = 12;
bool mui::MuiConfig::detectRetina = true;
bool mui::MuiConfig::useRetinaAssets = true;
float mui::MuiConfig::scaleFactor = 1;
bool mui::MuiConfig::enableDebuggingShortcuts = true;
Poco::Path mui::MuiConfig::dataPath = Poco::Path();
ofLogLevel mui::MuiConfig::logLevel = OF_LOG_ERROR;
function<ofFileDialogResult(string,string)> mui::MuiConfig::systemSaveDialog = [](string file, string msg){
	return ofSystemSaveDialog(file,msg);
};
function<ofFileDialogResult(string)> mui::MuiConfig::systemLoadDialog = [](string msg){
	return ofSystemLoadDialog(msg);
};

