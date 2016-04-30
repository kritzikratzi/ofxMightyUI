//
//  MuiParameterPanel.c
//  MuiEditorTest
//
//  Created by Hansi on 30.04.16.
//
//

#include "MuiParameterPanel.h"

using namespace mui;

mui::ParameterPanel::ParameterPanel( string title ) : mui::Container(0,0,100,100), labelColumnWidth(100), labelFg(255){
	titleLabel = new mui::Label(title);
	add(titleLabel);
}


void mui::ParameterPanel::layout(){
	float yy = titleLabel->height;
	for( Section* section : sections ){
		section->y = yy;
		section->width = width;
		section->handleLayout();
		
		yy += section->height;
	}
	
	height = yy;
}

string mui::ParameterPanel::getTitle(){
	return titleLabel->text;
}

void mui::ParameterPanel::setTitle( string text ){
	titleLabel->text = text;
	titleLabel->commit();
}

string mui::ParameterPanel::getSectionTitle( string sectionId ){
	Section * section = getSection(sectionId);
	if( section != NULL ){
		return section->getTitle();
	}
	else{
		return "";
	}
}

mui::ParameterPanel::Section * mui::ParameterPanel::getSection(string sectionId){
	for( Section * section : sections ){
		if( section->sectionId == sectionId ){
			return section;
		}
	}
	
	return NULL;
}

void mui::ParameterPanel::setSectionTitle( string sectionId, string title ){
	Section * section = getSection(sectionId);
	if( section != NULL ){
		section->setTitle( title );
	}
}


mui::ParameterPanel::Section * mui::ParameterPanel::getCurrentSection(){
	if( currentSection == NULL ){
		return getOrCreateSection("", "");
	}
	else{
		return currentSection; 
	}
}

mui::ParameterPanel::Section * mui::ParameterPanel::addSection( string sectionId ){
	return addSection( sectionId, sectionId );
}

mui::ParameterPanel::Section * mui::ParameterPanel::addSection( string sectionId, string title ){
	Section * section = new Section( sectionId, title );
	sections.push_back(section);
	add(section);
	currentSection = section; 
	
	return section;
}

mui::ParameterPanel::Section * mui::ParameterPanel::getOrCreateSection( string sectionId, string title ){
	for( Section * section : sections ){
		if( section->sectionId == sectionId ){
			return section;
		}
	}
	
	return addSection(sectionId, title);
}

float mui::ParameterPanel::getFloat( string rowId ){
	map<string,mui::data::Attribute>::iterator it = rows.find(rowId);
	if( it != rows.end() ){
		mui::data::Attribute &attr = it->second;
		if(attr.type() == typeid(Row<SliderWithLabel,float>*)){
			auto row = attr.value<Row<SliderWithLabel,float>*>();
			return row->content->slider->value;
		}
	}
	
	return 0;
}

void mui::ParameterPanel::setFloat( string rowId, float value ){
	map<string,mui::data::Attribute>::iterator it = rows.find(rowId);
	if( it != rows.end() ){
		mui::data::Attribute &attr = it->second;
		cout << "attr = " << attr.type().name() << endl;
	}
}


mui::ParameterPanel::Row<mui::SliderWithLabel,float> * mui::ParameterPanel::addSlider( string title, float min, float max, float value ){
	SliderWithLabel * slider = new SliderWithLabel(0,0,100,30,  min,max,value, 2);
	slider->label->fg = labelFg;
	auto row = new ParameterPanel::Row<SliderWithLabel,float>(this, title,NULL,slider,slider->slider->value);
	
	getCurrentSection()->addRow(row);
	rows.insert(pair<string,data::Attribute>(title,row));
	return row;
}

mui::ParameterPanel::Row<mui::ToggleButton,bool> * mui::ParameterPanel::addToggle( string title, bool selected ){
	ToggleButton * button = new ToggleButton(title);
	button->selected = selected;
	button->label->horizontalAlign = Left;
	button->fg = labelFg;
	button->bg = ofColor(0,0);
	button->selectedBg = ofColor(0,0);
	button->selectedFg = labelFg;
	button->checkbox = true;
	button->checkboxAlign = Right;
	
	auto row = new ParameterPanel::Row<ToggleButton,bool>(this, title,button->label,button,button->selected);
	
	getCurrentSection()->addRow(row);
	rows.insert(pair<string,data::Attribute>(title,row));
	return row;
}

mui::ParameterPanel::Row<mui::TextArea,string> * mui::ParameterPanel::addText( string title, string text ){
	TextArea * textArea = new TextArea(text);
	textArea->fg = ofColor(255);
	textArea->selectAllOnFocus = true;
	textArea->commit(); 
	
	auto row = new ParameterPanel::Row<TextArea,string>(this, title,NULL,textArea,textArea->text);
	
	getCurrentSection()->addRow(row);
	rows.insert(pair<string,data::Attribute>(title,row));
	return row;
}