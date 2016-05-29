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
	titleLabel->fontSize *= 1.2;
	add(titleLabel);
	currentSection = NULL; 
//	const pair<type_info,data::Attribute> d(typeid(Row<SliderWithLabel,float>), data::Attribute(3.0));
	
	registerGetter<SliderWithLabel,float>([](SliderWithLabel * obj){
		return obj->slider->value; }
	);
	registerGetter<SliderWithLabel,int>([](SliderWithLabel * obj){
		return (int)obj->slider->value; }
	);
	registerGetter<TextArea,string>([](TextArea * obj){ return obj->text; } );
	registerGetter<ToggleButton,bool>([](ToggleButton * obj){ return obj->selected; } );
	registerGetter<ToggleButton,int>([](ToggleButton * obj){ return obj->selected?1:0; } );
	
	registerSetter<SliderWithLabel,float>([](SliderWithLabel * obj,const float & val){ obj->slider->value = val; } );
	registerSetter<SliderWithLabel,int>([](SliderWithLabel * obj,const int & val){ obj->slider->value = val; } );
	registerSetter<TextArea,string>([](TextArea * obj,const string & val){ obj->text = val; } );
	registerSetter<ToggleButton,bool>([](ToggleButton * obj,const float & val){ obj->selected = val; } );
	registerSetter<ToggleButton,int>([](ToggleButton * obj,const int & val){ obj->selected = val==0; } );

	bg = ofColor(50);
	opaque = true;
}

void mui::ParameterPanel::setLabelColumnWidth(float width){
	labelColumnWidth = width;
	handleLayout();
}

void mui::ParameterPanel::layout(){
	float yy = titleLabel->height;
	for( Section* section : sections ){
		if(section->visible){
			section->y = yy;
			section->width = width;
			section->handleLayout();
			
			yy += section->height;
		}
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

bool mui::ParameterPanel::getBool( string rowId ){
	return getValue<bool>(rowId);
}

void mui::ParameterPanel::setBool( string rowId, bool value ){
	return setValue<bool>( rowId, value );
}

int mui::ParameterPanel::getInt( string rowId ){
	return getValue<int>(rowId);
}

void mui::ParameterPanel::setInt( string rowId, int value ){
	return setValue<int>( rowId, value );
}

float mui::ParameterPanel::getFloat( string rowId ){
	return getValue<float>(rowId);
}

void mui::ParameterPanel::setFloat( string rowId, float value ){
	return setValue<float>( rowId, value );
}

string mui::ParameterPanel::getString( string rowId ){
	return getValue<string>(rowId);
}

void mui::ParameterPanel::setString( string rowId, string value ){
	return setValue<string>( rowId, value );
}


mui::ParameterPanel::Row<mui::SliderWithLabel,float> * mui::ParameterPanel::addSliderInt( string title, int min, int max, int value ){
	SliderWithLabel * slider = new SliderWithLabel(0,0,100,20,  min,max,value, 0);
	slider->label->fg = labelFg;
	auto row = new ParameterPanel::Row<SliderWithLabel,float>(this, title,NULL,slider,slider->slider->value);
	
	getCurrentSection()->addRow(row);
	rows.insert(pair<string,data::Attribute>(title,row));
	return row;
}


mui::ParameterPanel::Row<mui::SliderWithLabel,float> * mui::ParameterPanel::addSlider( string title, float min, float max, float value, int decimalDigits ){
	SliderWithLabel * slider = new SliderWithLabel(0,0,100,20,  min,max,value, decimalDigits);
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

mui::ParameterPanel::Row<mui::Container,bool> * mui::ParameterPanel::addContainer( string title, mui::Container * container ){
	auto row = new ParameterPanel::Row<Container,bool>(this, title,NULL,container,container->visible);
	
	getCurrentSection()->addRow(row);
	rows.insert(pair<string,data::Attribute>(title,row));
	return row;
}