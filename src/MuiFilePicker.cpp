//
//  MuiFilePicker.cpp
//  Oscilloscope
//
//  Created by Hansi on 30.05.18.
//
//

#include "ofxMightyUI.h"
#include "MuiFilePicker.h"
#include "MuiL.h"

mui::FilePicker::FilePicker(mui::FilePicker::Operation op) : operation(op), mui::Container(0,0,100,30){
	opaque = true;
	
	label = new Label( "", 0, 0, width, height );
	label->verticalAlign = Middle;
	label->fg.r = label->fg.g = label->fg.b = 255;
	label->fontSize = mui::MuiConfig::fontSize;
	bg = ofColor( 128, 50 );
	add( label );
	
	
	clearButton = new mui::Button("x", 0, 0, 20, 20);
	add(clearButton);
	
	resetFormatter();
	setFile("");
}

mui::FilePicker::~FilePicker(){
	delete label;
	delete clearButton;
}


void mui::FilePicker::setOperation(mui::FilePicker::Operation operation_){
	operation = operation_;
	needsLayout = true; 
}

mui::FilePicker::Operation mui::FilePicker::getOperation(){
	return operation;
}


void mui::FilePicker::touchDown(ofTouchEventArgs & args){
	pressed = true;
};

void mui::FilePicker::touchUp(ofTouchEventArgs & args){
	wantsPick = true;
	pressed = false;
};

void mui::FilePicker::touchUpOutside(ofTouchEventArgs & args){
	pressed = false;
};

void mui::FilePicker::drawBackground(){
	if(wantsPick){
		wantsPick = false;
		ofFileDialogResult res;
		if(operation == Operation::LOAD){
			res = ofSystemLoadDialog("Pick any audio file");
		}
		else{
			auto file = selectedPath==""?defaultSaveName:ofFile(selectedPath,ofFile::Reference).getFileName();
			res = ofSystemSaveDialog(
									 file,
									 "Please pick destination file"
									 );
		}
		if(res.bSuccess){
			setFileAndNotify(res.getPath());
		}
	}
	if( pressed ){
		ofSetColor( bg.r/2, bg.g/2, bg.b/2, bg.a );
	}
	else{
		ofSetColor( bg.r, bg.g, bg.b, bg.a );
	}
	ofDrawRectangle( 0, 0, width, height );
	ofSetColor(255);
}

void mui::FilePicker::layout(){
	if(clearButton->visible){
		mui::L(clearButton).posTR(2,2);
		mui::L(label).widthTo(clearButton,2);
	}
	else{
		mui::L(label).posTL(0,0).stretchToRightEdgeOfParent().stretchToBottomEdgeOfParent();
	}
}


bool mui::FilePicker::fileDragged(ofDragInfo & evt){
	if(evt.files.size()>0){
		setFileAndNotify(evt.files.back());
		return true;
	}
	
	
	return false;
};

const std::filesystem::path & mui::FilePicker::getFile() const{
	return selectedPath;
}

void mui::FilePicker::setFile(const std::filesystem::path & path){
	selectedPath = path;
	label->setText(formatter(selectedPath));
	clearButton->visible = selectedPath != "";
	needsLayout = true;
}

void mui::FilePicker::setFileAndNotify(const std::filesystem::path & file){
	setFile(file);
	onFileChanged.notify(selectedPath);
}

void mui::FilePicker::resetFormatter(){
	setFormatter( [](const std::filesystem::path & file){
		return ofFile(file,ofFile::Reference).getAbsolutePath();
	} );
}

void mui::FilePicker::setFormatter(const function<std::string(const std::filesystem::path & file)> & formatter_){
	formatter = formatter_;
	setFile(selectedPath);
}


function<std::string(const std::filesystem::path & file)> mui::FilePicker::getFormatter() const{
	return formatter; 
}

void mui::FilePicker::setDefaultSaveName(string name){
	defaultSaveName = name;
}

string mui::FilePicker::getDefaultSaveName(){
	return defaultSaveName;
}
