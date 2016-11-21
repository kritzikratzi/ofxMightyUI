/*
 *  TextArea.h
 *  part of ofxMightyUI 
 *
 *  In development, do not use! 
 *
 *  Created by hansi some point in 2016
 */

#ifndef MUI_TextArea
#define MUI_TextArea

#include "MuiCore.h"

namespace mui{
	class TextArea : public Container{
	public: 
		TextArea( std::string text_ = "TextArea", float x_ = 0, float y_ = 0, float width_ = 200, float height_ = 20 );
		~TextArea(); 
		// alignments
		HorizontalAlign horizontalAlign; 
		VerticalAlign verticalAlign;
		
		// text settings
		std::string text;
		int fontSize;
		string fontName; // subject to change!
		bool selectAllOnFocus;
		ofEvent<string> onChange; 
		
		// the currently computed text boundaries
		// the height and y offset are currently always the size of a capital 'M'
		ofRectangle boundingBox;
		
		// used internally
		virtual void update();
		virtual void draw();
		virtual void drawBackground();
		virtual void layout();
		
		virtual void touchDown( ofTouchEventArgs &touch );
		virtual void touchMoved( ofTouchEventArgs &touch );
		
		virtual bool keyPressed( ofKeyEventArgs &key );
		virtual bool keyReleased( ofKeyEventArgs &key );
		
		// change the size to accomodate all contents
		virtual void sizeToFit( float padX = 0, float padY = 0 );
		// change the width to accomodate all contents
		virtual void sizeToFitWidth( float padX = 0 );
		// changes the height of the TextArea to accomodate all contents
		virtual void sizeToFitHeight( float padY = 0 );
		
		// changes text + commit
		void setText( string text );
		void setTextAndNotify( string text ); 
		
		// call this after you made changes to any variables (changing x/y is okay without doing a commit() ).
		// doing text-maths too insane to do on every frame!
		virtual void commit();
		
		virtual ofRectangle box( float tblr = 0 ){ return box( tblr, tblr, tblr, tblr ); }
		virtual ofRectangle box( float tb, float lr ){ return box( tb, lr, tb, lr ); }
		virtual ofRectangle box( float t, float lr, float b ){ return box( t, lr, b, lr ); }
		virtual ofRectangle box( float t, float r, float b, float l );

		struct EditorCursor{
			vector<StyledLine>::iterator lineIt;
			vector<LineElement>::iterator elementIt;
			ofRectangle rect;
			
			bool operator==(const EditorCursor& other) {
				return lineIt == other.lineIt && elementIt == other.elementIt;
			}
			bool operator!=(const EditorCursor& other) {
				return !(*this == other);
			}
		};
		
		struct EditorData{
			string text;
			ofxFontStashStyle fontStyle;
			vector<StyledLine> lines;
			int strlenWithLineStarts;
			bool changed;
			TextArea * textarea;
			
			EditorData(TextArea * textarea) : changed(false),textarea(textarea){}
		};
		
	private:
		
		
		class EditorState;
		EditorData data;
		EditorState *state;
		uint64_t lastInteraction;
		EditorCursor getEditorCursorForIndex( int pos );
		
		int state_select_min();
		int state_select_max();
		int state_select_len();
	};
};

#endif