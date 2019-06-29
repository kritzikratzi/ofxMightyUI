/*
 *  TextArea.cpp
 *
 *  Text TextArea
 *  - Supports vertical and horizontal alignments
 *  - Call commit() after changing text, fontSize, fontName or any of the other variables (except bounds!).
 *  - fg variable affects text color (no commit needed)
 *  - Only support multiline
 */

#include "MuiTextArea.h"
#include "Root.h"
#include <regex>

// how nasty is this??
mui::TextAreaInternal * internal(mui::TextArea * tf){
	return tf->internalData();
}

#define STB_TEXTEDIT_STRING mui::TextArea
#define STB_TEXTEDIT_CHARTYPE uint32_t

#define STB_TEXTEDIT_POSITIONTYPE int
//TODO: utf8 support
#define STB_TEXTEDIT_STRINGLEN(obj) ((int)internal(obj)->utf32.size())
#define STB_TEXTEDIT_CHARARR_LEN(arr,len) (len)
#define STB_TEXTEDIT_LAYOUTROW(r,obj,n) mui::TextAreaInternal::layout_func(r,obj,n)
#define STB_TEXTEDIT_GETWIDTH(obj,n,i) mui::TextAreaInternal::layout_width(obj,n,i)
#define STB_TEXTEDIT_KEYTOTEXT(key)    (((key) & KEYDOWN_BIT) ? 0 : (key))
// this only returns the first byte of a multi byte sequence :(
#define STB_TEXTEDIT_GETCHAR(tc,i)     (internal(tc)->utf32[i])
#define STB_TEXTEDIT_NEWLINE           '\n'
#define STB_TEXTEDIT_IS_SPACE(ch)      isspace(ch)
#define STB_TEXTEDIT_DELETECHARS       mui::TextAreaInternal::delete_chars
#define STB_TEXTEDIT_INSERTCHARS       mui::TextAreaInternal::insert_chars

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
#define STB_TEXTEDIT_MOVEWORDLEFT mui::TextAreaInternal::move_to_word_previous
#define STB_TEXTEDIT_MOVEWORDRIGHT mui::TextAreaInternal::move_to_word_next

#include "../libs/stb_textedit/include/stb_textedit/stb_textedit.h"

#pragma mark UTF8-Helpers

inline int mui::TextArea::octect_size( uint32_t codepoint ){
	if (codepoint < 0x80) // two octets
		return 1;
	else if (codepoint < 0x800) // two octets
		return 2;
	else if (codepoint < 0x10000) // three octets
		return 3;
	else // four octets
		return 4;
}

int mui::TextArea::utf8_expected_len( char first ){
	switch((unsigned char)first){
		case 0xc0: return 2;
		case 0xe0: return 3;
		case 0xf0: return 4;
		default: return 1;
	}
}


string mui::TextArea::utf32_to_utf8( uint32_t codepoint ){
	if (codepoint < 0x80)                        // one octet
		return string({(char)codepoint});
	else if (codepoint < 0x800) {                // two octets
		return string({
			(char)((codepoint >> 6)            | 0xc0),
			(char)((codepoint & 0x3f)          | 0x80)
		});
	}
	else if (codepoint < 0x10000) {              // three octets
		return string({
			(char)((codepoint >> 12)           | 0xe0),
			(char)(((codepoint >> 6) & 0x3f)   | 0x80),
			(char)((codepoint & 0x3f)          | 0x80)
		});
	}
	else {
		return string({
			(char)((codepoint >> 18)           | 0xf0),
			(char)(((codepoint >> 12) & 0x3f)  | 0x80),
			(char)(((codepoint >> 6) & 0x3f)   | 0x80),
			(char)((codepoint & 0x3f)          | 0x80)
		});
	}
}

inline size_t mui::TextArea::utf8_strlen(const string & line ){
	size_t i = 0;
	for( auto c : ofUTF8Iterator(line) ){
		++i;
	}
	return i;
}

inline size_t mui::TextArea::utf8_to_utf32(const string & text, vector<uint32_t> & utf32, vector<size_t> & utf8_positions){
	size_t i = 0;
	size_t pos = 0;
	utf32.clear();
	utf8_positions.clear();
	
	for( auto c : ofUTF8Iterator(text) ){
		utf32.push_back(c);
		utf8_positions.push_back(pos);
		pos += octect_size(c);
		++i;
	}
	
	return i;
}

inline vector<uint32_t> mui::TextArea::utf8_to_utf32(const string & text){
	vector<uint32_t> res;
	for( auto c : ofUTF8Iterator(text) ){
		res.push_back(c);
	}
	return res;
}

// define the functions we need
void mui::TextArea::layout_func_impl(void *row_ptr, mui::TextArea *data, int start_i)
{
	StbTexteditRow * row = (StbTexteditRow*)row_ptr;
	const ofRectangle &boundingBox = data->boundingBox;
	ofRectangle size = mui::Helpers::alignBox( data, boundingBox.width, boundingBox.height, data->horizontalAlign, data->verticalAlign );
	//	mui::Helpers::getFontStash().getTextBounds(data->text, data->fontStyle, boundingBox.x, boundingBox.y);
	
	// figure out where we currently are
	int pos = 0;
	float y = 0;
	for( int i = 0; i < data->lines.size(); i++){
		const StyledLine & line = data->lines[i];
		int lineLen = (int)data->utf32_line_length[i];
		
		if( pos >= start_i ){
			assert(line.elements.size()>0);
			row->num_chars = lineLen;
			row->x0 = size.x + line.elements.front().x;
			row->x1 = row->x0 + line.lineW;
			row->baseline_y_delta = 0; //???
			row->ymin = size.y + y;
			row->ymax = size.y + y + line.lineH;
			return;
		}
		
		pos += lineLen;
		y += line.lineH;
	}
	
	row->num_chars = 0;
	row->x0 = size.x;
	row->x1 = size.x+size.width;
	row->baseline_y_delta = 0;
	row->ymin = y;
	row->ymax = y + 10;
}

// n: line starts at index n
// i: is the char offset within the line
// return value: is advance to the next letter (taking kerning and maybe other funny things into account)
//               i think it is: equivalent to: xpos(n+i+1)-xpos(n+i)
float mui::TextArea::layout_width_impl(mui::TextArea * data, int n, int i ){
//	if(i==0) return data->find_xpos_right(n);
//	else return data->find_xpos_right(n+i)-data->find_xpos_right(n+i-1);
	if(i==0) return data->getEditorCursorForIndex(n+1).rect.x;
	else return data->getEditorCursorForIndex(n+i+1).rect.x-data->getEditorCursorForIndex(n+i).rect.x;
}

// pos is the position in utf32 chars
int mui::TextArea::delete_chars_impl(mui::TextArea *data, int pos, int num)
{
	if(num==0) return 0;
	size_t from = data->idx_utf8(pos);
	size_t to = data->idx_utf8(pos+num);
	
	if(from==to) return 0;
	
	data->text.erase(data->text.begin()+from, data->text.begin()+to);
	data->commit();
	// do an immediate commit() here?
	return 1;
}

// pos is the position in unicode chars
// newtext is utf32 encoded!
int mui::TextArea::insert_chars_impl(mui::TextArea *data, int pos, const STB_TEXTEDIT_CHARTYPE *newtext, int num){
	size_t idx;
	if(pos==0) idx = 0;
	else if(pos<data->utf32.size()) idx = data->utf8_positions[pos-1]+octect_size(data->utf32[pos-1]);
	else idx = data->text.length();
	idx = min(data->text.size(), idx);
	
	
	stringstream str;
	int len = 0;
	for( int i = 0; i < num; i++){
		uint32_t ch = newtext[i];
		if(data->onCharacterAdded.notifyRequireAll(ch)){
			str << utf32_to_utf8(ch);
			len ++;
		}
	}

	string text = str.str(); 
	if (data->onInsert) text = data->onInsert(text);
	
	data->text.insert(idx, text);
	data->commit();
	return len; // always succeeds
}


int mui::TextArea::is_word_boundary( STB_TEXTEDIT_STRING *str, int idx )
{
	return idx > 0 ? (STB_TEXTEDIT_IS_SPACE( STB_TEXTEDIT_GETCHAR(str,idx-1) ) && !STB_TEXTEDIT_IS_SPACE( STB_TEXTEDIT_GETCHAR(str, idx) ) ) : 1;
}

static bool is_cool_coding_char(STB_TEXTEDIT_CHARTYPE c){
	const static char * chars = "[]|{}().!=+-_,;:^&|#";
	for(int i = strlen(chars)-1;i>=0; i--){
		if((uint32_t)chars[i] == c) return true;
	}
	return false;
}

int mui::TextArea::move_to_word_previous_impl( STB_TEXTEDIT_STRING *str, int c )
{
	--c; // always move at least one character
	while( c >= 0 && !is_word_boundary( str, c ) && !is_cool_coding_char(STB_TEXTEDIT_GETCHAR(str,c)))
		--c;
	
	if( c < 0 )
		c = 0;
	
	return c;
}


int mui::TextArea::move_to_word_next_impl( STB_TEXTEDIT_STRING *str, int c )
{
	const int len = STB_TEXTEDIT_STRINGLEN(str);
	++c; // always move at least one character
	while( c < len && !is_word_boundary( str, c ) && !is_cool_coding_char(STB_TEXTEDIT_GETCHAR(str,c)))
		++c;
	
	if( c > len )
		c = len;
	
	return c;
}



#define STB_TEXTEDIT_IMPLEMENTATION
#include "../libs/stb_textedit/include/stb_textedit/stb_textedit.h"
#undef STB_TEXTEDIT_IMPLEMENTATION

// this exists only because it's not possible to forward declare unnamed structs
class mui::TextArea::EditorState : public STB_TexteditState{
};

mui::TextArea::TextArea( std::string text_, float x_, float y_, float width_, float height_ ) : mui::ScrollPane( x_, y_, width_, height_ ),
text( text_), fontSize(-1), horizontalAlign(Left), verticalAlign(Middle),fontName(""),lastInteraction(0),selectAllOnFocus(false){
	editor_view = new mui::TextAreaView(this, 0, 0, width, height);
	editor_view->cursor = mui::Cursor::IBeam;
	view->add(editor_view);

	state = new EditorState();
	focusTransferable = false;
	stb_textedit_initialize_state(state,0);
	if( fontSize < 0 ) fontSize = mui::MuiConfig::fontSize;
	commit();
	state->cursor = strlenWithLineStarts;

};

mui::TextArea::~TextArea(){
	editor_view->remove(); 
	delete editor_view; 
	delete state;
}

// internally redirect those, so that it goes straight into the real textarea and not the scroller
bool mui::TextArea::hasFocus(){return editor_view->hasFocus(); }
bool mui::TextArea::hasFocus( ofTouchEventArgs &touch ){ return editor_view->hasFocus(touch); };
bool mui::TextArea::requestFocus( ofTouchEventArgs &args ){ return editor_view->requestFocus(args); };

// internally redirect those, so that it goes straight into the real textarea and not the scroller
bool mui::TextArea::hasKeyboardFocus(){ return editor_view->hasKeyboardFocus(); };
bool mui::TextArea::requestKeyboardFocus(){ return editor_view->requestKeyboardFocus(); };

//--------------------------------------------------------------
void mui::TextArea::update(){
	mui::ScrollPane::update(); 
	fontStyle.fontSize = fontSize;
	fontStyle.color = fg;
	fontStyle.fontID = fontName;
}


const string mui::TextArea::getText(){
	return text;
}

void mui::TextArea::setText( string text ){
	state->select_start = 0;
	state->select_end = strlenWithLineStarts;
	state->cursor = state->select_end;
	vector<uint32_t> text_utf32 = utf8_to_utf32(text);
	stb_textedit_paste(this, state, &text_utf32[0], (int)text_utf32.size());
}

void mui::TextArea::setTextAndNotify( string text ){
	setText(text);
	ofNotifyEvent(onChange, text, this);
}

//--------------------------------------------------------------
void mui::TextArea::drawBackground(){
	Container::drawBackground();
}

//--------------------------------------------------------------
void mui::TextArea::layout(){
	mui::ScrollPane::layout();
}

void mui::TextArea::sizeToFit( float padX, float padY ){
	commit(); // update bounding box
	width = boundingBox.width + padX;
	height = boundingBox.height + padY;
	layout(); // tell ourselves about the size change
}

void mui::TextArea::sizeToFitWidth( float padX ){
	commit();
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
	mui::Helpers::loadFont(fontName);
	
	if (!multiLine) canScrollX = canScrollY = false;
	fontStyle.fontSize = fontSize;
	fontStyle.color = fg;
	fontStyle.fontID = fontName;

	boundingBox = Helpers::getFontStash().getTextBounds(text, fontStyle, 0, 0);
	
	vector<StyledText> blocks{ {text,fontStyle} };
	lines = Helpers::getFontStash().layoutLines(blocks, softWrap?editor_view->width:9999999);
	lineNumberSourceToDisplay.clear();
	lineNumberDisplayToSource.clear();
	
	int lineNumSrc = 0;
	int lineNumDisp = 0;
	int lastLineStart = 0;
	for( auto & line : lines ){
		auto & last = line.elements.back();
		lineNumberDisplayToSource.push_back(lineNumSrc);
		if(last.content.styledText.text == "\n"){
			lineNumberSourceToDisplay.push_back(lastLineStart);
			lineNumSrc ++;
			lastLineStart = lineNumDisp+1;
		}
		lineNumDisp ++;
	}
	lineNumberSourceToDisplay.push_back(lastLineStart);
	
	if(lines.size() == 0){
		lines = Helpers::getFontStash().layoutLines({{" ",fontStyle}}, 10);
	}
	strlenWithLineStarts = 0;
	utf32_line_length.clear();
	utf32.clear();
	utf8_positions.clear();
	
	utf32.reserve(text.size()*1.5);
	utf8_positions.reserve(text.size()*1.5);
	
	size_t pos = 0;
	for(uint32_t ch : ofUTF8Iterator(text)){
		utf32.push_back(ch);
		utf8_positions.push_back(pos);
		pos += octect_size(ch);
	}
	
	
	bool first = false;
	for( int i = 0; i < lines.size(); i++){
		StyledLine & line = lines[i];
		utf32_line_length.push_back(0);
		for( auto el : line.elements ){
			utf32_line_length[i] += (int)utf8_strlen(el.content.styledText.text);
		}
		strlenWithLineStarts += utf32_line_length[i];
		if( first ) first = false;
		else strlenWithLineStarts ++;
	}
	
	// Orient y on a simple uppercase character
	// Otherwise things go up and down unexpectedly
	ofRectangle baselineSize = Helpers::getFontStash().getTextBounds("M", fontStyle, 0, 0);
	boundingBox.height = baselineSize.height;
	boundingBox.y = baselineSize.y;
	
	if(true/*autoChangeHeight*/){
		float h = 0;
		for( int i = (int)lines.size()-1;i>=0;i--){
			StyledLine & line = lines[i];
			for(int j = (int)line.elements.size()-1; j>=0; j--){
				h = MAX(h,line.elements[j].baseLineY);
			}
		}
		
		// add one more line! to be sure we have enough space for the decenders
		if (multiLine) h += baselineSize.height / 3;

		h = max(minHeight, h - baselineSize.y);

		editor_view->width = viewportWidth;

		if (autoChangeHeight) {
			editor_view->height = h;
			height = h;
		}
		else {
			h = max(viewportHeight, h); 
			if (h != editor_view->height) {
				editor_view->height = h;
				MUI_ROOT->needsLayout = true;
			}
		}
	}

	mui::ScrollPane::commit(); 
}

bool mui::TextArea::keyPressed( ofKeyEventArgs &key ){
	lastInteraction = ofGetElapsedTimeMillis();
	short redoPt = state->undostate.redo_point;
	short undoPt = state->undostate.undo_point;
	short undoWhere = state->undostate.undo_rec[MIN(98,undoPt)].where;
	bool certainlyChanged = false;
	
	int keyMask =
	(ofGetKeyPressed(OF_KEY_SHIFT)?STB_TEXTEDIT_K_SHIFT:0) |
#if defined(TARGET_OSX)
	(ofGetKeyPressed(OF_KEY_ALT)?STB_TEXTEDIT_K_CONTROL:0)
#else
	(ofGetKeyPressed(OF_KEY_CONTROL)?STB_TEXTEDIT_K_CONTROL:0)
#endif
	;
	
#ifdef _WIN32
	// windows is funny with ctrl shortcuts
	if (ofGetKeyPressed(OF_KEY_CONTROL) && key.key >= 1 && key.key <= 26) {
		key.key = key.codepoint = 'a' + (key.key - 1);
	}
#endif
	
	switch(key.key){
		case OF_KEY_HOME:
			stb_textedit_key(this, state, STB_TEXTEDIT_K_LINESTART|keyMask);
			break;
		case OF_KEY_END:
			stb_textedit_key(this, state, STB_TEXTEDIT_K_LINEEND|keyMask);
			break;
		case OF_KEY_UP:
			// on osx cmd+up goes to the start
			if(ofGetKeyPressed(OF_KEY_COMMAND)) stb_textedit_key(this, state, STB_TEXTEDIT_K_TEXTSTART);
			else stb_textedit_key(this, state, STB_TEXTEDIT_K_UP|keyMask);
			break;
		case OF_KEY_DOWN:
			// on osx cmd+down goes to the end
			if(ofGetKeyPressed(OF_KEY_COMMAND)) stb_textedit_key(this, state, STB_TEXTEDIT_K_TEXTEND);
			else stb_textedit_key(this, state, STB_TEXTEDIT_K_DOWN|keyMask);
			break;
		case OF_KEY_LEFT:
			if(MUI_ROOT->getKeyPressed(OF_KEY_SUPER)) stb_textedit_key(this, state, STB_TEXTEDIT_K_LINESTART|keyMask );
			else stb_textedit_key(this, state, STB_TEXTEDIT_K_LEFT|keyMask);
			break;
		case OF_KEY_RIGHT:
			if(MUI_ROOT->getKeyPressed(OF_KEY_SUPER)) stb_textedit_key(this, state, STB_TEXTEDIT_K_LINEEND|keyMask );
			else stb_textedit_key(this, state, STB_TEXTEDIT_K_RIGHT|keyMask);
			break;
		case OF_KEY_BACKSPACE:
			stb_textedit_key(this, state, STB_TEXTEDIT_K_BACKSPACE|keyMask);
			break;
		case OF_KEY_DEL:
			stb_textedit_key(this, state, STB_TEXTEDIT_K_DELETE|keyMask);
			break;
		case OF_KEY_RETURN:
			stb_textedit_key(this, state, STB_TEXTEDIT_NEWLINE|keyMask);
			break;
		case OF_KEY_PAGE_UP:
			break;
		case OF_KEY_PAGE_DOWN:
			break;
		case OF_KEY_ESC:
			// do nothing!
			break;
		default:
			//ok, what about other shortcuts? ...
			if(MUI_ROOT->getKeyPressed(MUI_KEY_ACTION) && key.keycode==GLFW_KEY_A ){
				state->select_start = 0;
				state->select_end = (int)strlenWithLineStarts;
				state->cursor = state->select_end;
			}
			else if(MUI_ROOT->getKeyPressed(MUI_KEY_ACTION) && key.codepoint == 'z'){
				if (ofGetKeyPressed(OF_KEY_SHIFT)) stb_text_redo(this, state);
				else stb_text_undo(this, state);
			}
			else if(MUI_ROOT->getKeyPressed(MUI_KEY_ACTION) && (key.codepoint == 'Z' || key.codepoint == 'y')){
				stb_text_redo(this, state);
			}
			else if(MUI_ROOT->getKeyPressed(MUI_KEY_ACTION) && key.codepoint == 'x'){
				ofGetWindowPtr()->setClipboardString(getSelectedText());
				stb_textedit_key(this, state, STB_TEXTEDIT_K_DELETE );
			}
			else if(MUI_ROOT->getKeyPressed(MUI_KEY_ACTION) && key.codepoint == 'c'){
				ofGetWindowPtr()->setClipboardString(getSelectedText());
			}
			else if(MUI_ROOT->getKeyPressed(MUI_KEY_ACTION) && key.codepoint == 'v'){
				string text = ofGetWindowPtr()->getClipboardString();
				vector<uint32_t> text_utf32 = utf8_to_utf32(text);
				stb_textedit_paste(this, state, &text_utf32[0], (int)text_utf32.size());
			}
			else if (key.key == OF_KEY_TAB && state->select_start != state->select_end) {
				// go back to prev line break
				if (state->select_start > state->select_end) swap(state->select_start, state->select_end);
				while (state->select_start > 0 && STB_TEXTEDIT_GETCHAR(this, state->select_start -1) != '\n'){
					state->select_start--;
				}

				string text = getSelectedText();
				if (ofGetKeyPressed(OF_KEY_SHIFT)) {
					if (text[0] == '\t') text = text.substr(1);
					std::regex e("\n\t");
					text = regex_replace(text, e, "\n");
				}
				else {
					std::regex e("\n");
					text = "\t" + regex_replace(text, e, "\n\t");
				}

				
				insertTextAtCursor(text,true);
			}
			else if(MUI_ROOT->getKeyPressed(MUI_KEY_ACTION)){
				// a shortcut of sorts, but not for us. 
				return false;
			}
			else{
				uint32_t codept = key.codepoint;
				
				if (!utf8::internal::is_code_point_valid(codept)){
					// what is it? don't know! ignore it!
					return true;
				}
				else if( codept == 0x09 ){
					// control character, but ok
				}
				else if( codept < 0x1F || codept == 0x7F || (codept >= 0x0080 && codept <= 0x009F) ){
					// control character
					return true;
				}
				
				// the next 20 or so lines are basically a verbatim copy from
				// stb_textedit_key() with a tiny change:
				// a char doesn't have a fixed size
				//				EditorData * str = this;
				
				// can't add newline in single-line mode
				if (codept == '\n' && state->single_line)
					break;
				
				if (state->insert_mode && !STB_TEXT_HAS_SELECTION(state) && state->cursor < STB_TEXTEDIT_STRINGLEN(this)) {
					stb_text_makeundo_replace(this, state, state->cursor, 1, 1);
					STB_TEXTEDIT_DELETECHARS(this, state->cursor, 1);
					if (STB_TEXTEDIT_INSERTCHARS(this, state->cursor, &codept, 1)) {
						++state->cursor;
						state->has_preferred_x = 0;
						certainlyChanged = true;
					}
				} else {
					stb_textedit_delete_selection(this,state); // implicity clamps
					if (STB_TEXTEDIT_INSERTCHARS(this, state->cursor, &codept, 1)) {
						stb_text_makeundo_insert(state, state->cursor, 1);
						++state->cursor;
						state->has_preferred_x = 0;
						certainlyChanged = true;
					}
				}
			}
	}
	
	if(certainlyChanged || undoPt != state->undostate.undo_point || redoPt != state->undostate.redo_point || undoWhere != state->undostate.undo_rec[MIN(98,state->undostate.undo_point)].where ){
		ofNotifyEvent(onChange, text, this);
	}
	
	// check if we need to scroll
	{
		EditorCursor cursor = getEditorCursorForIndex(state->cursor);
		scrollIntoView(cursor.rect);
	}
	
	return true;
}

bool mui::TextArea::keyReleased( ofKeyEventArgs &key ){
	return true;
}


mui::TextArea::EditorCursor mui::TextArea::getEditorCursorForIndex( int cursorPos ){
	EditorCursor result;
	ofRectangle size = mui::Helpers::alignBox( this, boundingBox.width, boundingBox.height, horizontalAlign, verticalAlign );
	
	//TODO: this should be cached
	int pos = 0;
	float yy = 0;
	ofRectangle bounds;
	for( int lineIdx = 0; lineIdx < lines.size(); lineIdx++ ){
		StyledLine &line = lines[lineIdx];
		int len = utf32_line_length[lineIdx];
		if( pos + len <= cursorPos ){
			pos += len;
			yy += line.lineH;
		}
		else{
			for( vector<LineElement>::iterator elementIt = line.elements.begin(); elementIt != line.elements.end(); ++elementIt ){
				LineElement &el = *elementIt;
				len = (int)utf8_strlen(el.content.styledText.text);
				if( pos + len <= cursorPos ){
					pos += len;
				}
				else{
					float x;
					switch(el.content.type){
						case SEPARATOR_INVISIBLE:
						case SEPARATOR:
							x = el.area.x + el.area.width;
							
						case WORD_BLOCK:
							// it's here, and we know the offset inside the word already.
							// now let's find it within the string.
							size_t len = cursorPos-pos;
							ofRectangle size = mui::Helpers::getFontStash().getTextBounds(substr_utf8(pos, len), el.content.styledText.style, 0, 0);
							// this could be wrong. hard to tell for my purpose. (layout_width)
							x = el.x + size.x + size.width;
					}
					
					string sub = substr_utf8(pos,cursorPos-pos);
					
					bounds.height = line.lineH;
					bounds.x = size.x - boundingBox.x + x + bounds.width;
					bounds.y = size.y - boundingBox.y + yy - line.lineH*0.9;
					bounds.width = 0;
					bounds.height = line.lineH;
					
					result.rect = bounds;
					result.lineIt = lines.begin()+lineIdx;
					result.elementIt = elementIt;
					return result;
				}
			}
			break;
		}
	}
	
	if( bounds == ofRectangle() ){
		// still nothing? must be at last position!
		if( lines.size() > 0 ){
			float lineH = lines.back().lineH;
			bounds = lines.back().elements.back().area;
			bounds.x = size.x - boundingBox.x + bounds.x;
			bounds.y = size.y - boundingBox.y + yy - 2*lineH;
			bounds.height = lineH;
			
			result.lineIt = lines.end()-1;
			// there should be always at least one element on each line,
			// even if it's just a SEPARATOR_INVISIBLE
			result.elementIt = lines.back().elements.end()-1;
			result.rect = bounds;
		}
		else{
			result.lineIt = lines.end();
		}
	}
	
	// this is SLOW!
	// maybe at least remember it?
	return result;
}


int mui::TextArea::state_select_min(){
	return min(state->select_start,state->select_end);
}

int mui::TextArea::state_select_max(){
	return max(state->select_start,state->select_end);
}

int mui::TextArea::state_select_len(){
	int res = state->select_start - state->select_end;
	return res<0?-res:res;
}

string mui::TextArea::getSelectedText(){
	return substr_utf8(state_select_min(),state_select_len());
}

void mui::TextArea::insertTextAtCursor(string text, bool select){
	size_t sel_start = state->select_start;
	vector<uint32_t> text_utf32 = utf8_to_utf32(text);
	stb_textedit_paste(this, state, &text_utf32[0], (int)text_utf32.size());
	commit();
	if (select) {
		setSelectedRange(sel_start, sel_start + text_utf32.size());
	}
}


int mui::TextArea::getCursorLine(){
	int cursor = state->cursor;
	
	for(int i = 0; i < lines.size(); i++){
		if(utf32_line_length[i]<cursor){
			cursor -= utf32_line_length[i];
		}
		else{
			return lineNumberDisplayToSource[i];
		}
	}
	
	return max(0,(int)lineNumberDisplayToSource.back());
}

int mui::TextArea::getCursorColumn(){
	int cursor = state->cursor;
	
	for(int i = 0; i < lines.size(); i++){
		if(utf32_line_length[i]<cursor){
			cursor -= utf32_line_length[i];
		}
		else{
			return cursor;
		}
	}
	
	return lines.size()==0?0:max(0,(int)utf32_line_length[0]-1);
}

ofVec2f mui::TextArea::getCursorPosition(){
	EditorCursor c = getEditorCursorForIndex(state->cursor);
	return {c.rect.x, (*c.elementIt).baseLineY};
}

void mui::TextArea::selectAll(){
	stb_textedit_key(this, state, STB_TEXTEDIT_K_TEXTSTART);
	stb_textedit_key(this, state, STB_TEXTEDIT_K_TEXTEND | STB_TEXTEDIT_K_SHIFT);
}

void mui::TextArea::selectNothing(){
	state->select_start = state->select_end;
}

void mui::TextArea::setSelectedRange(size_t start, size_t end){
	if(utf32.size()==0) return;
	
	start = CLAMP(start,0,utf32.size()-1);
	end = CLAMP(end,0,utf32.size()-1);
	
	state->select_start = min(start,end);
	state->select_end = max(start,end);
}

pair<size_t,size_t> mui::TextArea::getSelectedRange(){
	return make_pair(state->select_start,state->select_end);
}




#pragma mark Editor Data

size_t mui::TextArea::idx_utf8(size_t utf32_idx){
	int sz = utf8_positions.size();
	return sz==0?0:(utf32_idx<sz?
					utf8_positions[utf32_idx]:
					(utf8_positions[sz-1]+octect_size(utf32[sz-1]))
					);
	
}

string mui::TextArea::substr_utf8( size_t utf32_index, size_t len){
	size_t N = utf32.size(); 
	if(len==0 || N == 0) return "";
	size_t from = idx_utf8(utf32_index);
	size_t to = idx_utf8(CLAMP(utf32_index+len-1,0,N-1)) + octect_size(utf32[CLAMP(utf32_index+len-1,0,N-1)]);
	
	return text.substr(from,to-from);
}


void mui::TextAreaInternal::layout_func(void *row, TextArea *data, int start_i){
	return mui::TextArea::layout_func_impl(row,data,start_i);
}

float mui::TextAreaInternal::layout_width(mui::TextArea * data, int n, int i ){
	return mui::TextArea::layout_width_impl(data, n, i );
}

int mui::TextAreaInternal::delete_chars(mui::TextArea *data, int pos, int num){
	return mui::TextArea::delete_chars_impl(data, pos, num);
}

int mui::TextAreaInternal::insert_chars(mui::TextArea *data, int pos, const uint32_t *newtext, int num){
	return mui::TextArea::insert_chars_impl(data, pos, newtext, num);
}

int mui::TextAreaInternal::move_to_word_previous( mui::TextArea *str, int c ){
	return mui::TextArea::move_to_word_previous_impl( str, c );
}

int mui::TextAreaInternal::move_to_word_next( mui::TextArea *str, int c ){
	return mui::TextArea::move_to_word_next_impl( str, c );
}





//--------------------------------------------------------------
mui::TextAreaView::TextAreaView(mui::TextArea * textArea, float x_, float y_, float width_, float height_) :t(textArea), mui::Container(x_,y_,width_,height_){
	focusTransferable = false;
}

//--------------------------------------------------------------
mui::TextAreaView::~TextAreaView() {
}

//--------------------------------------------------------------
void mui::TextAreaView::draw() {
	ofRectangle size = Helpers::alignBox(this, t->boundingBox.width, t->boundingBox.height, t->horizontalAlign, t->verticalAlign);

	if (hasKeyboardFocus() && t->state->select_start != t->state->select_end) {
		int left = t->state_select_min();
		int right = t->state_select_max();

		ofSetColor(fg*0.5 + bg*0.5);

		mui::TextArea::EditorCursor from = t->getEditorCursorForIndex(left);
		mui::TextArea::EditorCursor to = t->getEditorCursorForIndex(right);

		ofRectangle rect = from.rect;
		float yy = rect.y;

		bool reset = false;
		if (from.lineIt != to.lineIt) {
			// selec to end of line
			rect.width = (*from.lineIt).lineW - rect.x;
			ofDrawRectangle(rect);
			from.lineIt++;
			reset = true;

			yy += (*from.lineIt).lineH;
		}

		while (from.lineIt != to.lineIt) {
			StyledLine &line = *from.lineIt;
			float x = size.x - t->boundingBox.x + line.elements.front().x;
			float y = yy;
			ofDrawRectangle(x, y, line.lineW, line.lineH);
			yy += line.lineH;
			++from.lineIt;
			reset = true;
		}

		if (from.lineIt != t->lines.end()) {
			StyledLine &line = *from.lineIt;
			float x = reset ? (size.x - t->boundingBox.x + line.elements.front().x) : from.rect.x;
			float y = reset ? yy : from.rect.y;

			ofDrawRectangle(x, y, to.rect.x + to.rect.width - x, line.lineH);
			++from.lineIt;
		}
	}

	//	mui::Helpers::getFontStash().drawColumn(text, fontStyle, size.x-boundingBox.x, size.y-boundingBox.y, width);
	mui::Helpers::getFontStash().drawLines(t->lines, size.x - t->boundingBox.x, size.y - t->boundingBox.y, MuiConfig::debugDraw);
	if (hasKeyboardFocus()) {
		if (t->drawActiveBorder) {
			ofSetColor(200); 
			ofNoFill();
			ofDrawRectangle(0.5, 0.5, width - 1, height - 1);
			ofFill();
		}
		// getting the time is slow, but it can only happen
		// for a single textfield here because of the focus (so we're fine)
		uint64_t time = ofGetElapsedTimeMillis();
		if (((time - t->lastInteraction) % 1000) < 500) {
			ofSetColor(255);
			ofRectangle bounds = t->getEditorCursorForIndex(t->state->cursor).rect;
			ofDrawRectangle(bounds.x + bounds.width, bounds.y + 2, 2, bounds.height - 2);
		}
	}

	ofSetColor(255);
}


void mui::TextAreaView::touchDown(ofTouchEventArgs &touch) {
	t->lastInteraction = ofGetElapsedTimeMillis();

	if (t->selectAllOnFocus && !hasKeyboardFocus()) {
		t->selectAll();
	}
	else {
		stb_textedit_click(t, t->state, touch.x, touch.y);
	}
	requestKeyboardFocus();
}

void mui::TextAreaView::touchUp(ofTouchEventArgs &touch) {
}

void mui::TextAreaView::touchUpOutside(ofTouchEventArgs &touch) {
}

void mui::TextAreaView::touchMoved(ofTouchEventArgs &touch) {
	stb_textedit_drag(t, t->state, touch.x, touch.y);
}

void mui::TextAreaView::touchMovedOutside(ofTouchEventArgs &touch) {
	stb_textedit_drag(t, t->state, touch.x, touch.y);
}

void mui::TextAreaView::mouseScroll(ofMouseEventArgs &args) {
	t->mouseScroll(args);
}
