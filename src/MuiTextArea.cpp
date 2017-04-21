/*
 *  TextArea.cpp
 *
 *  Text TextArea
 *  - Supports vertical and horizontal alignments
 *  - Call commit() after changing text, fontSize, fontName or any of the other variables (except bounds!).
 *  - fg variable affects text color (no commit needed)
 *  - Doesn't support multiline
 */

#include "MuiTextArea.h"
#include "Root.h"

// how nasty is this??
mui::TextAreaInternal * internal(mui::TextArea * tf){
	return (mui::TextAreaInternal*)tf;
}

#define STB_TEXTEDIT_STRING mui::TextArea
#define STB_TEXTEDIT_CHARTYPE uint32_t

#define STB_TEXTEDIT_POSITIONTYPE int
//TODO: utf8 support
#define STB_TEXTEDIT_STRINGLEN(obj) ((int)internal(obj)->unicode.size())
#define STB_TEXTEDIT_CHARARR_LEN(arr,len) (len)
#define STB_TEXTEDIT_LAYOUTROW(r,obj,n) mui::TextAreaInternal::layout_func(r,obj,n)
#define STB_TEXTEDIT_GETWIDTH(obj,n,i) mui::TextAreaInternal::layout_width(obj,n,i)
#define STB_TEXTEDIT_KEYTOTEXT(key)    (((key) & KEYDOWN_BIT) ? 0 : (key))
// this only returns the first byte of a multi byte sequence :(
#define STB_TEXTEDIT_GETCHAR(tc,i)     (internal(tc)->unicode[i])
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


string mui::TextArea::unicode_to_utf8( uint32_t codepoint ){
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
			(char)((codepoint & 0x3f)          | 0x80),
			(char)((codepoint >> 18)           | 0xf0)
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

inline size_t mui::TextArea::utf8_to_unicode(const string & text, vector<uint32_t> & unicode, vector<size_t> & utf8_positions){
	size_t i = 0;
	size_t pos = 0;
	unicode.clear();
	utf8_positions.clear();
	
	for( auto c : ofUTF8Iterator(text) ){
		unicode.push_back(c);
		utf8_positions.push_back(pos);
		pos += octect_size(c);
		++i;
	}
	
	return i;
}

inline vector<uint32_t> mui::TextArea::utf8_to_unicode(const string & text){
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
		size_t lineLen = data->unicode_line_length[i];
		
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

// pos is the position in unicode chars
int mui::TextArea::delete_chars_impl(mui::TextArea *data, int pos, int num)
{
	if(num==0) return 0;
	size_t from = data->idx_utf8(pos);
	size_t to = data->idx_utf8(pos+num-1) + utf8_expected_len(data->text[pos+num-1]);
	
	if(from==to) return 0;
	
	data->text.erase(data->text.begin()+from, data->text.begin()+to);
	data->commit();
	// do an immediate commit() here?
	return 1;
}

// pos is the position in unicode chars
// newtext is utf32 encoded!
int mui::TextArea::insert_chars_impl(mui::TextArea *data, int pos, const STB_TEXTEDIT_CHARTYPE *newtext, int num){
	size_t idx = data->utf8_positions[pos];
	idx = min(data->text.size(), idx);
	
	
	stringstream str;
	for( int i = 0; i < num; i++){
		str << unicode_to_utf8(newtext[i]);
	}
	
	data->text.insert(idx, str.str());
	data->commit();
	return 1; // always succeeds
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

mui::TextArea::TextArea( std::string text_, float x_, float y_, float width_, float height_ ) :
Container( x_, y_, width_, height_ ),
text( text_), fontSize(-1), horizontalAlign(Left), verticalAlign(Middle),fontName(""),lastInteraction(0),selectAllOnFocus(false){
	state = new EditorState();
	focusTransferable = false;
	stb_textedit_initialize_state(state,0);
	if( fontSize < 0 ) fontSize = mui::MuiConfig::fontSize;
	commit();
};

mui::TextArea::~TextArea(){
	delete state;
}

//--------------------------------------------------------------
void mui::TextArea::update(){
	fontStyle.fontSize = fontSize;
	fontStyle.color = fg;
	fontStyle.fontID = fontName;
}


//--------------------------------------------------------------
void mui::TextArea::draw(){
	ofRectangle size = Helpers::alignBox( this, boundingBox.width, boundingBox.height, horizontalAlign, verticalAlign );
	
	if( hasKeyboardFocus() && state->select_start != state->select_end ){
		int left = state_select_min();
		int right = state_select_max();
		
		ofSetColor(fg*0.5 + bg*0.5);
		
		EditorCursor from = getEditorCursorForIndex(left);
		EditorCursor to = getEditorCursorForIndex(right);
		
		ofRectangle rect = from.rect;
		float yy = rect.y;
		
		bool reset = false;
		if( from.lineIt != to.lineIt ){
			// selec to end of line
			rect.width = (*from.lineIt).lineW - rect.x;
			ofDrawRectangle(rect);
			from.lineIt ++;
			reset = true;
			
			yy += (*from.lineIt).lineH;
		}
		
		while( from.lineIt != to.lineIt ){
			StyledLine &line = *from.lineIt;
			float x = size.x-boundingBox.x + line.elements.front().x;
			float y = yy;
			ofDrawRectangle(x, y, line.lineW, line.lineH );
			yy += line.lineH;
			++from.lineIt;
			reset = true;
		}
		
		if( from.lineIt != lines.end() ){
			StyledLine &line = *from.lineIt;
			float x = reset?(size.x-boundingBox.x + line.elements.front().x):from.rect.x;
			float y = reset?yy:from.rect.y;
			
			ofDrawRectangle(x, y, to.rect.x + to.rect.width - x, line.lineH );
			++from.lineIt;
		}
	}
	
	//	mui::Helpers::getFontStash().drawColumn(text, fontStyle, size.x-boundingBox.x, size.y-boundingBox.y, width);
	mui::Helpers::getFontStash().drawLines(lines, size.x-boundingBox.x, size.y-boundingBox.y, MuiConfig::debugDraw);
	ofSetColor( 255 );
	if( hasKeyboardFocus()){
		ofNoFill();
		ofDrawRectangle(0,0,width,height);
		ofFill();
		// getting the time is slow, but it can only happen
		// for a single textfield here because of the focus (so we're fine)
		uint64_t time = ofGetElapsedTimeMillis();
		if( ((time-lastInteraction)%1000) < 500 ){
			ofRectangle bounds = getEditorCursorForIndex(state->cursor).rect;
			ofDrawRectangle(bounds.x+bounds.width, bounds.y, 2, bounds.height);
		}
	}
}

const string mui::TextArea::getText(){
	return text;
}

void mui::TextArea::setText( string text ){
	this->text = text;
	text = text;
	utf8_to_unicode(text, unicode, utf8_positions);
	commit();
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
	commit();
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
	boundingBox = Helpers::getFontStash().getTextBounds(text, fontStyle, 0, 0);
	
	fontStyle.fontSize = fontSize;
	fontStyle.color = fg;
	fontStyle.fontID = fontName;
	text = text;
	
	vector<StyledText> blocks{ {text,fontStyle} };
	lines = Helpers::getFontStash().layoutLines(blocks, width);
	strlenWithLineStarts = 0;
	unicode_line_length.clear();
	unicode.clear();
	utf8_positions.clear();
	
	unicode.reserve(text.size()*1.5);
	utf8_positions.reserve(text.size()*1.5);
	
	size_t pos = 0;
	for(uint32_t ch : ofUTF8Iterator(text)){
		unicode.push_back(ch);
		utf8_positions.push_back(pos);
		pos += utf8_expected_len(ch);
	}
	
	
	bool first = false;
	for( int i = 0; i < lines.size(); i++){
		StyledLine & line = lines[i];
		unicode_line_length.push_back(0);
		for( auto el : line.elements ){
			unicode_line_length[i] += utf8_strlen(el.content.styledText.text);
		}
		strlenWithLineStarts += unicode_line_length[i];
		if( first ) first = false;
		else strlenWithLineStarts ++;
	}
	
	// Orient y on a simple uppercase character
	// Otherwise things go up and down unexpectedly
	ofRectangle baselineSize = Helpers::getFontStash().getTextBounds("M", fontStyle, 0, 0);
	boundingBox.height = baselineSize.height;
	boundingBox.y = baselineSize.y;
}

void mui::TextArea::touchDown(ofTouchEventArgs &touch){
	lastInteraction = ofGetElapsedTimeMillis();
	if( selectAllOnFocus && !hasKeyboardFocus()){
		stb_textedit_key(this, state, STB_TEXTEDIT_K_TEXTEND);
		stb_textedit_key(this, state, STB_TEXTEDIT_K_TEXTSTART | STB_TEXTEDIT_K_SHIFT);
	}
	else{
		stb_textedit_click(this, state, touch.x, touch.y);
	}
	requestKeyboardFocus();
}

void mui::TextArea::touchMoved(ofTouchEventArgs &touch){
	stb_textedit_drag(this, state, touch.x, touch.y);
}

bool mui::TextArea::keyPressed( ofKeyEventArgs &key ){
	lastInteraction = ofGetElapsedTimeMillis();
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
	cout << key.key << endl;
	if (ofGetKeyPressed(OF_KEY_CONTROL) && key.key >= 1 && key.key <= 26) {
		key.key = key.codepoint = 'a' + (key.key - 1);
	}
	cout << key.key << endl;
#endif
	
	switch(key.key){
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
		case OF_KEY_ESC:
			// do nothing!
			break;
		default:
			//ok, what about other shortcuts? ...
			if(MUI_ROOT->getKeyPressed(MUI_KEY_ACTION) && key.keycode==GLFW_KEY_A ){
				state->select_start = 0;
				state->select_end = strlenWithLineStarts;
				state->cursor = state->select_end;
			}
			else if(MUI_ROOT->getKeyPressed(MUI_KEY_ACTION) && key.codepoint == 'z'){
				if (ofGetKeyPressed(OF_KEY_SHIFT)) stb_text_redo(this, state);
				else stb_text_undo(this, state);
			}
			else if(MUI_ROOT->getKeyPressed(MUI_KEY_ACTION) && key.codepoint == 'Z'){
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
				vector<uint32_t> text_unicode = utf8_to_unicode(text);
				stb_textedit_paste(this, state, &text_unicode[0], text_unicode.size());
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
					}
				} else {
					stb_textedit_delete_selection(this,state); // implicity clamps
					if (STB_TEXTEDIT_INSERTCHARS(this, state->cursor, &codept, 1)) {
						stb_text_makeundo_insert(state, state->cursor, 1);
						++state->cursor;
						state->has_preferred_x = 0;
					}
				}
			}
	}
	ofNotifyEvent(onChange, text, this);
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
		int len = unicode_line_length[lineIdx];
		if( pos + len <= cursorPos ){
			pos += len;
			yy += line.lineH;
		}
		else{
			for( vector<LineElement>::iterator elementIt = line.elements.begin(); elementIt != line.elements.end(); ++elementIt ){
				LineElement &el = *elementIt;
				len = utf8_strlen(el.content.styledText.text);
				if( pos + len <= cursorPos ){
					pos += len;
				}
				else{
					float x;
					switch(el.content.type){
						case SEPARATOR_INVISIBLE:
						case SEPARATOR:
							x = el.area.x + el.area.width;
							
						case WORD:
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



#pragma mark Editor Data

size_t mui::TextArea::idx_utf8(size_t unicode_idx){
	return utf8_positions[unicode_idx];
}

string mui::TextArea::substr_utf8( size_t unicode_index, size_t len){
	if(len==0) return "";
	size_t from = idx_utf8(unicode_index);
	size_t to = idx_utf8(unicode_index+len-1) + utf8_expected_len(text[unicode_index+len-1]);
	
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
