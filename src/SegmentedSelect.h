/*
 *  SegmentedSelect.h
 *  ofxMightyUI
 *
 *  Created by hansi on 29.01.11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef MUI_SEGMENTED_SELECTED
#define MUI_SEGMENTED_SELECTED

#include "MuiCore.h"
#include "Button.h"
#include "Label.h"

namespace mui{
	template<typename T>
	class SegmentedButton : public Button{
	public: 
		SegmentedButton( std::string title_, T value, float x_ = 0, float y_ = 0, float width_ = 200, float height_ = 30 ) : Button( title_, x_, y_, width_, height_ ), selected(false), value(value) {
			label->fontSize -= 2;
			label->commit();
		}
		
		bool selected;
		bool roundedLeft; 
		bool roundedRight;
		T value;
		
		
		virtual void drawBackground(){
			ofSetColor( 255, 255, 255 );
			Helpers::beginImages();
			if( selected || pressed ){
				if( roundedLeft ) Helpers::drawImage( "segment_left_active", 0, 0, 5, 29 );
				Helpers::drawImage( "segment_center_active", roundedLeft?5:0, 0, width-((roundedLeft?5:0)+(roundedRight?5:0)), 29 );
				if( roundedRight ) Helpers::drawImage( "segment_right_active", width-5, 0, 5, 29 );
			}
			else{
				if( roundedLeft ) Helpers::drawImage( "segment_left", 0, 0, 5, 29 );
				Helpers::drawImage( "segment_center", roundedLeft?5:0, 0, width-((roundedLeft?5:0)+(roundedRight?5:0)), 29 );
				if( roundedRight ) Helpers::drawImage( "segment_right", width-5, 0, 5, 29 );
			}
			
			if( !roundedRight ){
				Helpers::drawImage( "segment_separator", width-1, 0, 1, 29 );
			}
			Helpers::endImages();
		}
	};
	
	template<typename T>
	class SegmentedSelect : public Container{
	public: 
		SegmentedSelect( float x_ = 0, float y_ = 0, float width_ = 200, float height_ = 30 ): Container( x_, y_, width_, height_ ), selected(NULL){
		}
		
		SegmentedButton<T> * selected;
		
		virtual void addSegment( string text, T value ){
			SegmentedButton<T> * button = new SegmentedButton<T>( text, value );
			button->width = button->label->boundingBox.width + 10;
			ofAddListener(button->onPress, this, &SegmentedSelect::onButtonPress );
			
			add( button );
			commit();
		}
		
		virtual void commit(){
			vector<Container*>::iterator it = children.begin();
			int x = 0;
			bool first = true;
			SegmentedButton<T> * last = NULL;
			
			while( it != children.end() ){
				SegmentedButton<T> * button = (SegmentedButton<T>*) *it;
				button->x = x;
				x += button->width;
				button->selected = button == selected;
				button->roundedLeft = first;
				button->roundedRight = false;
				
				first = false;
				last = button;
				
				++it;
			}
			
			if( last != NULL ){
				last->roundedRight = true; 
			}
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
			return selected == NULL? defaultValue : selected->value;
		}
		
		virtual const int getSelectedIndex(){
			int i = 0;
			for( mui::Container * c : children ){
				mui::SegmentedButton<T> * button = (mui::SegmentedButton<T>*)c;
				if( button != nullptr && button == selected ){
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

		ofEvent<SegmentedButton<T>*> onChange;
		ofEvent<T> onChangeValue;
		
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
	};
};

#endif