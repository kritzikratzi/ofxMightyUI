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
#include "ScrollPane.h"


namespace mui{
	class TextAreaView;

	class TextAreaInternal {
	public:
		vector<uint32_t> utf32; // utf-32 text
		static void layout_func(void *row, TextArea *data, int start_i);
		static float layout_width(mui::TextArea * data, int n, int i);
		static int delete_chars(mui::TextArea *data, int pos, int num);
		static int insert_chars(mui::TextArea *data, int pos, const uint32_t *newtext, int num);
		static int move_to_word_previous(mui::TextArea *str, int c);
		static int move_to_word_next(mui::TextArea *str, int c);
	};



	class TextArea : public mui::ScrollPane, private TextAreaInternal {
	public: 
		TextArea(std::string text = "", float x = 0, float y = 0, float width = 200, float height = 20); 
		~TextArea(); 

		// alignments
		HorizontalAlign horizontalAlign;
		VerticalAlign verticalAlign;

		// text settings
		int fontSize;
		string fontName; // subject to change!
		bool selectAllOnFocus;
		mui::EventHandler<string> onChange{ this };
		mui::EventHandlerConsumable<uint32_t> onCharacterAdded{ this };
		mui::EventHandler<string> onCursorMoved{ this };
		mui::EventHandler<ofTouchEventArgs> onContextClick{ this };

		float minHeight = 20;
		float autoChangeHeight = false;
		bool softWrap = true;
		bool drawActiveBorder = true;
		bool multiLine = true; 

		// the currently computed text boundaries
		// the height and y offset are currently always the size of a capital 'M'
		ofRectangle boundingBox;

		// internally redirect those, so that it goes straight into the real textarea and not the scroller
		bool hasKeyboardFocus() override;
		bool requestKeyboardFocus() override;
		
		// used internally
		void update() override;
		void drawBackground() override;
		void layout() override;

		bool keyPressed(ofKeyEventArgs &key) override;
		bool keyReleased(ofKeyEventArgs &key) override;

		// change the size to accomodate all contents
		virtual void sizeToFit(float padX = 0, float padY = 0);
		// change the width to accomodate all contents
		virtual void sizeToFitWidth(float padX = 0);
		// changes the height of the TextArea to accomodate all contents
		virtual void sizeToFitHeight(float padY = 0);

		// changes text + commit
		const string getText();
		void setText(string text);
		void setTextAndNotify(string text);
		string getSelectedText();

		void insertTextAtCursor(string text, bool select = false);

		int getCursorLine();
		int getCursorColumn();
		ofVec2f getCursorPosition();

		void selectAll();
		void selectNothing();

		/// \brief Sets selection range. Counting is in utf32 (ie. the "Ä" in "ÜÄ" would be at index 1. remember you can't directly substring with this result!)
		void setSelectedRange(size_t start, size_t end);
		void setSelectedRangeUtf8(size_t start, size_t end); 
		/// \brief Returns the selection range. Counting is in utf32 (ie. the "Ä" in "ÜÄ" would be at index 1. remember you can't directly substring with this result!)
		pair<size_t, size_t> getSelectedRange();
		pair<size_t, size_t> getSelectedRangeUtf8();

		// call this after you made changes to any variables (changing x/y is okay without doing a commit() ).
		// doing text-maths too insane to do on every frame!
		virtual void commit(bool relayoutView=true) override;

		virtual ofRectangle box(float tblr = 0) { return box(tblr, tblr, tblr, tblr); }
		virtual ofRectangle box(float tb, float lr) { return box(tb, lr, tb, lr); }
		virtual ofRectangle box(float t, float lr, float b) { return box(t, lr, b, lr); }
		virtual ofRectangle box(float t, float r, float b, float l);

		struct EditorCursor {
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


		// use this to modify the text being set/inserted/typed
		function <string(string)> onInsert;


		TextAreaInternal * internalData() { return dynamic_cast<mui::TextAreaInternal*>(this); }
	private:
		size_t idx_utf8(size_t utf32_index);
		string substr_utf8(size_t utf32_index, size_t len);
		vector<size_t> utf8_positions; // mapping from utf-32 pos to utf-8 pos
		string text; // utf-8 text
		ofxFontStashStyle fontStyle;

		static string utf32_to_utf8(uint32_t codepoint);
		static int utf32_chlen(uint32_t codepoint); 
		static size_t utf8_strlen(const string & line);
		static size_t utf8_to_utf32(const string & text, vector<uint32_t> & utf32, vector<size_t> & utf8_positions);
		static vector<uint32_t> utf8_to_utf32(const string & text);
		static size_t count_chars(const StyledLine & line);
		static void layout_func_impl(void *row, TextArea *data, int start_i);
		static float layout_width_impl(mui::TextArea * data, int n, int i);
		static int delete_chars_impl(mui::TextArea *data, int pos, int num);
		static int insert_chars_impl(mui::TextArea *data, int pos, const uint32_t *newtext, int num);
		static int is_word_boundary(mui::TextArea *str, int idx);
		static int move_to_word_previous_impl(mui::TextArea *str, int c);
		static int move_to_word_next_impl(mui::TextArea *str, int c);
		static int utf8_expected_len(char first);
		static int octect_size(uint32_t codepoint);
	private:

		vector<StyledLine> lines;
		vector<size_t> lineNumberSourceToDisplay; // map source lines to the first display line. start at 0
		vector<size_t> lineNumberDisplayToSource; // map display lines to source lines. start at 0. 
		vector<int> utf32_line_length; // runlength of each line

		size_t strlenWithLineStarts;


		class EditorState;
		EditorState *state;
		TextAreaView *editor_view;
		uint64_t lastInteraction;
		EditorCursor getEditorCursorForIndex(int pos);

		int state_select_min();
		int state_select_max();
		int state_select_len();

		friend class TextAreaView;
		friend class TextAreaInternal;
	};


	class TextAreaView : public mui::Container{
	public: 
		TextAreaView( mui::TextArea * textArea, float x_ = 0, float y_ = 0, float width_ = 200, float height_ = 20 );
		~TextAreaView();
		
		virtual void draw() override;
		
		void touchDown( ofTouchEventArgs &touch ) override;
		void touchUp(ofTouchEventArgs &touch) override;
		void touchUpOutside(ofTouchEventArgs &touch) override;
		void touchMoved(ofTouchEventArgs &touch) override;
		void touchMovedOutside(ofTouchEventArgs &touch) override;
		bool mouseScroll(ofMouseEventArgs &args) override;
		
		friend class TextAreaInternal; 
	private: 
		mui::TextArea * t; 
	};

};

#endif
