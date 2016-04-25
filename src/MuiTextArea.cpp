/*
 *  TextArea.cpp
 *
 *  Text TextArea
 *  - Supports vertical and horizontal alignments
 *  - Call commit() after changing text, fontSize, fontName, ellipsis mode or any of the other variables (except bounds!).
 *  - fg variable affects text color (no commit needed)
 *  - Doesn't support multiline
 */

#include "MuiTextArea.h"

#define STB_TEXTEDIT_STRING mui::TextArea::EditorData
#define STB_TEXTEDIT_CHARTYPE char
#include "../libs/stb_textedit/include/stb_textedit/stb_textedit.h"



// a lot of the stb implementation comes from the example:
// https://github.com/nothings/stb/blob/master/tests/textedit_sample.c

// define the functions we need
void layout_func(StbTexteditRow *row, mui::TextArea::EditorData *data, int start_i)
{
	const ofRectangle &boundingBox = data->textarea->boundingBox;
	ofRectangle size = mui::Helpers::alignBox( data->textarea, boundingBox.width, boundingBox.height, data->textarea->horizontalAlign, data->textarea->verticalAlign );
	mui::Helpers::getFontStash().getTextBounds(data->text, data->fontStyle, boundingBox.x, boundingBox.y);

	int remaining_chars = data->text.size() - start_i;
	row->num_chars = remaining_chars > 20 ? 20 : remaining_chars; // should do real word wrap here
	row->x0 = size.x;
	row->x1 = size.x+size.width; // need to account for actual size of characters
	row->baseline_y_delta = size.height;
	row->ymin = size.y;
	row->ymax =  size.y+size.height;
}

float layout_width(mui::TextArea::EditorData * data, int n, int i ){
	ofRectangle size = mui::Helpers::getFontStash().getTextBounds(data->text.substr(n+i,1), data->fontStyle, 0, 0);
	return size.width;
}

int delete_chars(mui::TextArea::EditorData *str, int pos, int num)
{
	str->text.erase(str->text.begin()+pos, str->text.begin()+pos+num);
	str->changed = true;
	return 1; // always succeeds
}

int insert_chars(mui::TextArea::EditorData *str, int pos, const STB_TEXTEDIT_CHARTYPE *newtext, int num)
{
	str->text.insert(pos, newtext,num);
	str->changed = true;
	return 1; // always succeeds
}

#define STB_TEXTEDIT_POSITIONTYPE int
//TODO: utf8 support
#define STB_TEXTEDIT_STRINGLEN(obj) (obj->text.size())
#define STB_TEXTEDIT_LAYOUTROW(r,obj,n) layout_func(r,obj,n)
#define STB_TEXTEDIT_GETWIDTH(obj,n,i) layout_width(obj,n,i)
#define STB_TEXTEDIT_KEYTOTEXT(key)    (((key) & KEYDOWN_BIT) ? 0 : (key))
#define STB_TEXTEDIT_GETCHAR(tc,i)     ((tc)->text[i])
#define STB_TEXTEDIT_NEWLINE           '\n'
#define STB_TEXTEDIT_IS_SPACE(ch)      isspace(ch)
#define STB_TEXTEDIT_DELETECHARS       delete_chars
#define STB_TEXTEDIT_INSERTCHARS       insert_chars

#define KEYDOWN_BIT 0x8000

#define STB_TEXTEDIT_K_SHIFT           0x4000
#define STB_TEXTEDIT_K_CONTROL         0x2000
#define STB_TEXTEDIT_K_LEFT            (KEYDOWN_BIT | 1) // actually use VK_LEFT, SDLK_LEFT, etc
#define STB_TEXTEDIT_K_RIGHT           (KEYDOWN_BIT | 2) // VK_RIGHT
#define STB_TEXTEDIT_K_UP              (KEYDOWN_BIT | 3) // VK_UP
#define STB_TEXTEDIT_K_DOWN            (KEYDOWN_BIT | 4) // VK_DOWN
#define STB_TEXTEDIT_K_LINESTART       (KEYDOWN_BIT | 5) // VK_HOME
#define STB_TEXTEDIT_K_LINEEND         (KEYDOWN_BIT | 6) // VK_END
#define STB_TEXTEDIT_K_TEXTSTART       (STB_TEXTEDIT_K_LINESTART | STB_TEXTEDIT_K_CONTROL)
#define STB_TEXTEDIT_K_TEXTEND         (STB_TEXTEDIT_K_LINEEND   | STB_TEXTEDIT_K_CONTROL)
#define STB_TEXTEDIT_K_DELETE          (KEYDOWN_BIT | 7) // VK_DELETE
#define STB_TEXTEDIT_K_BACKSPACE       (KEYDOWN_BIT | 8) // VK_BACKSPACE
#define STB_TEXTEDIT_K_UNDO            (KEYDOWN_BIT | STB_TEXTEDIT_K_CONTROL | 'z')
#define STB_TEXTEDIT_K_REDO            (KEYDOWN_BIT | STB_TEXTEDIT_K_CONTROL | 'y')
#define STB_TEXTEDIT_K_INSERT          (KEYDOWN_BIT | 9) // VK_INSERT
#define STB_TEXTEDIT_K_WORDLEFT        (STB_TEXTEDIT_K_LEFT  | STB_TEXTEDIT_K_CONTROL)
#define STB_TEXTEDIT_K_WORDRIGHT       (STB_TEXTEDIT_K_RIGHT | STB_TEXTEDIT_K_CONTROL)
#define STB_TEXTEDIT_K_PGUP            (KEYDOWN_BIT | 10) // VK_PGUP -- not implemented
#define STB_TEXTEDIT_K_PGDOWN          (KEYDOWN_BIT | 11) // VK_PGDOWN -- not implemented

#define STB_TEXTEDIT_IMPLEMENTATION
#include "../libs/stb_textedit/include/stb_textedit/stb_textedit.h"
#undef STB_TEXTEDIT_IMPLEMENTATION

// this exists only because it's not possible to forward declare unnamed structs
class mui::TextArea::EditorState : public STB_TexteditState{
};

mui::TextArea::TextArea( std::string text_, float x_, float y_, float width_, float height_ ) :
	Container( x_, y_, width_, height_ ),
	ellipsisMode(true), text( text_), fontSize(-1), horizontalAlign(Left), verticalAlign(Middle),fontName(""),data(this){
		state = new EditorState();
		stb_textedit_initialize_state(state,0);
		if( fontSize < 0 ) fontSize = mui::MuiConfig::fontSize;
		commit();
};

mui::TextArea::~TextArea(){
	delete state; 
}

//--------------------------------------------------------------
void mui::TextArea::update(){
	data.fontStyle.fontSize = fontSize;
	data.fontStyle.color = fg;
	data.fontStyle.fontID = fontName;
	
	if( data.changed ){
		data.changed = false;
		text = data.text; 
		commit();
	}
}


//--------------------------------------------------------------
void mui::TextArea::draw(){
	ofRectangle size = Helpers::alignBox( this, boundingBox.width, boundingBox.height, horizontalAlign, verticalAlign );
	mui::Helpers::getFontStash().drawColumn(data.text, data.fontStyle, size.x-boundingBox.x, size.y-boundingBox.y, width);
	ofSetColor( 255 );
	if( hasKeyboardFocus() ){
		ofNoFill();
		ofDrawRectangle(0,0,width,height);
		ofFill();
		
		int cursorPos = state->cursor;
		// this is SLOW!
		// maybe at least remember it?
		ofRectangle bounds = mui::Helpers::getFontStash().getTextBounds(data.text.substr(0,cursorPos), data.fontStyle, size.x-boundingBox.x, size.y-boundingBox.y);
		ofSetColor(255,150);
		ofDrawRectangle(bounds.x+bounds.width, bounds.y+bounds.height-10, 2, 10);
	}
}


//--------------------------------------------------------------
void mui::TextArea::drawBackground(){
    Container::drawBackground(); 
}

//--------------------------------------------------------------
void mui::TextArea::layout(){
	// we might have to recalculate the text if things change ...
	if( ellipsisMode ){
		commit();
	}
}

void mui::TextArea::sizeToFit( float padX, float padY ){
	commit(); // update bounding box
	ellipsisMode = false;
	width = boundingBox.width + padX;
	height = boundingBox.height + padY;
	layout(); // tell ourselves about the size change
}

void mui::TextArea::sizeToFitWidth( float padX ){
	commit();
	ellipsisMode = false;
	width = boundingBox.width + padX;
	layout();
}

void mui::TextArea::sizeToFitHeight( float padY ){
	commit();
	height = boundingBox.height + padY;
	layout();
}

//--------------------------------------------------------------
//deprecated
ofRectangle mui::TextArea::box( float t, float r, float b, float l ){
	ofRectangle size = Helpers::alignBox( this, boundingBox.width, boundingBox.height, horizontalAlign, verticalAlign );
	return ofRectangle( size.x - boundingBox.x - l, size.y - boundingBox.y + t, boundingBox.width + l + r, boundingBox.height + t + b );
}

//--------------------------------------------------------------
void mui::TextArea::commit(){
	update();
	// magic trick #2
	// MUI_FONT_TYPE * font = Helpers::getFont( Helpers::retinaMode?(fontSize*2):fontSize );
	// magic trick #2.2: fuck retina, we compute the bounding box at normal size!
	mui::Helpers::loadFont(fontName);
	boundingBox = Helpers::getFontStash().getTextBounds(text, data.fontStyle, 0, 0);
	// NASTY HACK#158
	boundingBox.x = 0;

    if( ellipsisMode ){
        if( boundingBox.width > width && text.length() > 3 ){
			// don't mess with the original boundingBox object. it's publicly readable!
			ofRectangle box = boundingBox;
            int len = text.length() - 3;
            while( box.width > width && len >= 0 ){
                data.text = text.substr(0, len ) + "...";
				box = Helpers::getFontStash().getTextBounds(data.text, data.fontStyle, 0, 0 );
                len --;
            }
        }
        else{
            data.text = text;
        }
    }
    else{
        data.text = text;
    }
	
	// Orient y on a simple uppercase character
	// Otherwise things go up and down unexpectedly
	ofRectangle baselineSize = Helpers::getFontStash().getTextBounds("M", data.fontStyle, 0, 0);
	boundingBox.height = baselineSize.height;
	boundingBox.y = baselineSize.y;
}

void mui::TextArea::touchDown(ofTouchEventArgs &touch){
	stb_textedit_click(&data, state, touch.x, touch.y);
}

void mui::TextArea::keyPressed( ofKeyEventArgs &key ){
	// this is some extra work,
	// but it gives us a lot of control
	// (the alternative would be to use OF_* constants in the defines directly)
//	int shiftFlag = MUI_ROOT->getKeyPressed(OF_KEY_SHIFT);
	cout << "key=" << key.key << endl;
	cout << "keycode=" << key.keycode << endl;
	cout << "codepoint=" << key.codepoint << endl;
	cout << "scancode=" << key.scancode << endl;

	switch(key.key){
		case OF_KEY_LEFT:
			stb_textedit_key(&data, state, STB_TEXTEDIT_K_LEFT );
			break;
		case OF_KEY_RIGHT:
			stb_textedit_key(&data, state, STB_TEXTEDIT_K_RIGHT );
			break;
		case OF_KEY_BACKSPACE:
			stb_textedit_key(&data, state, STB_TEXTEDIT_K_BACKSPACE );
			break;
		case OF_KEY_DEL:
			stb_textedit_key(&data, state, STB_TEXTEDIT_K_DELETE );
			break;
		default:
			//todo: copied this from built-in utf8::append
			if (!utf8::internal::is_code_point_valid(key.codepoint)){
				cerr << "invalid code point: " << key.codepoint << endl;
				return;
			}
			
			if (key.codepoint < 0x80)                        // one octet
				stb_textedit_key(&data, state, key.codepoint );
			else if (key.codepoint < 0x800) {                // two octets
				stb_textedit_key(&data, state, ((key.codepoint >> 6)            | 0xc0) );
				stb_textedit_key(&data, state, ((key.codepoint & 0x3f)          | 0x80) );
			}
			else if (key.codepoint < 0x10000) {              // three octets
				stb_textedit_key(&data, state, ((key.codepoint >> 12)           | 0xe0) );
				stb_textedit_key(&data, state, (((key.codepoint >> 6) & 0x3f)   | 0x80) );
				stb_textedit_key(&data, state, ((key.codepoint & 0x3f)          | 0x80) );
			}
			else {                                // four octets
				stb_textedit_key(&data, state, ((key.codepoint >> 18)           | 0xf0) );
				stb_textedit_key(&data, state, (((key.codepoint >> 12) & 0x3f)  | 0x80) );
				stb_textedit_key(&data, state, (((key.codepoint >> 6) & 0x3f)   | 0x80) );
				stb_textedit_key(&data, state, ((key.codepoint & 0x3f)          | 0x80) );
				stb_textedit_key(&data, state, ((key.codepoint >> 18)           | 0xf0) );
			}
	}
}

void mui::TextArea::keyReleased( ofKeyEventArgs &key ){
	
}
