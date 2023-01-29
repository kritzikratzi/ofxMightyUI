/*
 *  SegmentedSelect.h
 *  ofxMightyUI
 *
 *  Created by hansi on 29.01.11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 *
 */

#ifndef MUI_SEGMENTED_SELECTED
#define MUI_SEGMENTED_SELECTED

#include "MuiCore.h"
#include "Button.h"
#include "Label.h"

namespace mui{
	template<typename T>
	class SegmentedSelect;
	
	template<typename T>
	class SegmentedButton : public Button{
	public: 
		SegmentedButton( std::string title_, T value, float x_ = 0, float y_ = 0, float width_ = 200, float height_ = 30 ) : Button( title_, x_, y_, width_, height_ ), selected(false), value(value) {
			opaque = true;
		}
		
		bool selected;
		bool isLeftmost;
		bool isRightmost;
		SegmentedSelect<T> * segmentedSelect; // reference back to parent
		T value;

		
		
		virtual void drawBackground(){
			segmentedSelect->onDrawButtonBackground(this);
		}

	};
	
	template<typename T>
	class SegmentedSelect : public Container{
	public: 
		SegmentedSelect( float x_ = 0, float y_ = 0, float width_ = 200, float height_ = 30 ): Container( x_, y_, width_, height_ ), selected(NULL){
			
			// default implementation for onDrawButtonBackground():
			onDrawButtonBackground = [&](mui::SegmentedButton<T>* button){
				if (button->ignoreEvents) {
					auto & c = button->segmentedSelect->buttonBgDefault;
					ofSetColor(c.r/2, c.g/2, c.b/2, c.a/2);
					ofDrawRectangle(0, 0, button->width, button->height);
				}
				else if( button->selected || button->pressed ){
					ofSetColor(button->segmentedSelect->buttonBgSelected);
					ofDrawRectangle(0, 0, button->width, button->height);
				}
				else{
					ofSetColor(button->segmentedSelect->buttonBgDefault);
					ofDrawRectangle(0, 0, button->width, button->height);
				}
				
				if( !button->isRightmost && button->segmentedSelect->buttonSeparatorColor.a > 0){
					ofSetColor(button->segmentedSelect->buttonSeparatorColor);
					ofDrawRectangle(button->width-1, 0, 1, button->height );
				}
				ofSetColor(255); // restore to default!
			};
		}
		
		virtual ~SegmentedSelect(){
			for(auto c : children){
				auto s = dynamic_cast<SegmentedButton<T>*>(c);
				if(s) delete s;
			}
		}
		
		virtual SegmentedButton<T> * addSegment( string text, T value ){
			SegmentedButton<T> * button = new SegmentedButton<T>( text, value );
			button->width = button->label->boundingBox.width + 10;
			button->height = height;
			ofAddListener(button->onPress, this, &SegmentedSelect::onButtonPress );
			
			add( button );
			commit();
			return button;
		}
		
		virtual void commit(){
			vector<Container*>::iterator it = children.begin();
			int x = 0;
			bool first = true;
			SegmentedButton<T> * last = NULL;
			
			float eqWidth = width/max((int)children.size(),1);
			while( it != children.end() ){
				SegmentedButton<T> * button = (SegmentedButton<T>*) *it;
				if (!button->visible) { ++it; continue; }
				button->selected = button == selected;
				button->segmentedSelect = this;
				button->x = x;
				button->label->fontSize = buttonFontSize;
				if(buttonFontName != "") button->label->fontName = buttonFontName;
				button->label->fg = button->selected ? buttonFgSelected : buttonFgDefault;
				button->label->commit();

				if(equallySizedButtons){
					button->width = eqWidth;
				}
				else{
					button->width = ceilf(button->label->boundingBox.width + buttonPadding);
				}
				x += button->width;
				button->height = height;
				button->isLeftmost = first;
				button->isRightmost = false;
				
				first = false;
				last = button;
				
				++it;
			}
			
			if( last != NULL ){
				last->isRightmost = true; 
			}
		}
		
		virtual void layout(){
			commit();
		}
		
		// iterator?
		virtual size_t getNumSegments(){
			return children.size();
		};
		virtual SegmentedButton<T> * getSegment( int num ){
			mui::SegmentedButton<T> * button = (mui::SegmentedButton<T>*)children[num];
			return button;
		}
		
		virtual const T getSelectedValueOr( const T defaultValue ){
			return selected == NULL || selected->ignoreEvents? defaultValue : selected->value;
		}
		
		virtual const int getSelectedIndex(){
			int i = 0;
			for( mui::Container * c : children ){
				mui::SegmentedButton<T> * button = (mui::SegmentedButton<T>*)c;
				if( button != nullptr && button == selected && !button->ignoreEvents){
					return i;
				}
				i++;
			}
			return -1; 
		}
		
		virtual SegmentedButton<T> * setSelectedIndex( const int num ){
			setSelected(getSegment(num)->value);
			return selected;
		}
		
		virtual SegmentedButton<T> * setSelectedIndexAndNotify( const int num ){
			setSelectedAndNotify(getSegment(num)->value);
			return selected;
		}
		
		virtual SegmentedButton<T> * setSelected( const T value ){
			for( mui::Container * c : children ){
				mui::SegmentedButton<T> * button = (mui::SegmentedButton<T>*)c;
				if( button != nullptr && button->value == value ){
					selected = button;
					commit();
					return button;
				}
			}
			return selected;
		}
		virtual SegmentedButton<T> * setSelectedAndNotify( const T value ){
			setSelected(value);
			if( selected != NULL ) selected->clickAndNotify();
			return selected;
		}

		void sizeToFitWidth(){
			commit();
			if(children.size() == 0 ){
				width = 0;
			}
			else{
				width = children.back()->x + children.back()->width;
			}
		}
		
	private:
		virtual void onButtonPress( const void* sender, ofTouchEventArgs &args ){
			SegmentedButton<T> * button = (SegmentedButton<T>*) sender;
			selected = button;
			ofNotifyEvent(onChange, selected, this);
			ofNotifyEvent(onChangeValue, selected->value, this);
			commit();
		}
	
	public:
		ofEvent<SegmentedButton<T>*> onChange;
		ofEvent<T> onChangeValue;
		SegmentedButton<T> * selected;
		
		bool equallySizedButtons{false};
		
		// button drawing is handled here (mostly)
		function<void(SegmentedButton<T>*)> onDrawButtonBackground; // see constructor for default implementation
		ofColor buttonBgDefault{ 100 };
		ofColor buttonBgSelected{ 150 };
		ofColor buttonFgDefault{ 255 };
		ofColor buttonFgSelected{ 255 };
		ofColor buttonSeparatorColor{255};
		int buttonFontSize{mui::MuiConfig::fontSize}; // needs a commit() after change!
		string buttonFontName{""}; // needs a commit() after change!
		float buttonPadding{10}; // total (l+r) padding
	};
};

#endif