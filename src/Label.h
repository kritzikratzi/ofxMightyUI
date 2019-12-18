/*
 *  Label.h
 *
 *  Created by hansi on 29.01.11.
 *
 *  A single line text label with alignments.
 *  - Supports vertical and horizontal alignments
 *  - Call commit() after changing text, fontSize, fontName, ellipsis mode or any of the other variables (except bounds!).
 *  - fg variable affects text color (no commit needed)
 *  - Doesn't support multiline
 */

#ifndef MUI_LABEL
#define MUI_LABEL

#include "MuiCore.h"

namespace mui{
	class Label : public Container{
	public: 
		Label( std::string text_ = "Label", float x_ = 0, float y_ = 0, float width_ = 200, float height_ = 20 );
		// alignments
		HorizontalAlign horizontalAlign; 
		VerticalAlign verticalAlign;
		
		// text settings
		std::string text;
		int fontSize;
		string fontName; // subject to change!
		bool ellipsisMode;
		bool verticalAlignUseBaseline; // defaults to true. disable to v-align icons. 
		Inset inset; 

		// the currently computed text boundaries
		// the height and y offset are currently always the size of a capital 'M'
		ofRectangle boundingBox;
		
		// used internally
		virtual void update();
		virtual void draw();
		virtual void drawBackground();
		virtual void layout();
		
		// change the size to accomodate all contents
		virtual void sizeToFit( float padX = 0, float padY = 0 );
		// change the width to accomodate all contents
		virtual void sizeToFitWidth( float padX = 0 );
		// changes the height of the label to accomodate all contents
		virtual void sizeToFitHeight( float padY = 0 );
		
		
		/// \brief Sets the label's text and recalculates the bounding box
		virtual void setText(const string & text);
		
		/// \brief returns the label's text
		virtual const string & getText();
		
		// call this after you made changes to any variables (changing x/y is okay without doing a commit() ).
		// doing text-maths too insane to do on every frame!
		virtual void commit();
		
		virtual ofRectangle box( float tblr = 0 ){ return box( tblr, tblr, tblr, tblr ); }
		virtual ofRectangle box( float tb, float lr ){ return box( tb, lr, tb, lr ); }
		virtual ofRectangle box( float t, float lr, float b ){ return box( t, lr, b, lr ); }
		virtual ofRectangle box( float t, float r, float b, float l );

	private: 
        std::string displayText;
		//ofVbo displayTextVbo;
		//int displayTextNumTris; 
		fs2::Style fontStyle;
	};
};

#endif
