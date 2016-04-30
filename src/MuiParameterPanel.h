//
//  MuiParameterPanel.h
//  MuiEditorTest
//
//  Created by Hansi on 30.04.16.
//
//

#ifndef __MuiEditorTest__MuiParameterPanel__
#define __MuiEditorTest__MuiParameterPanel__

#include "MuiCore.h"
#include "Label.h"
#include "SliderWithLabel.h"
#include "ToggleButton.h"
#include "MuiTextArea.h"

namespace mui{
	
	namespace data{
		// http://stackoverflow.com/a/25109643
		class Attribute {
		private:
			struct Head {
				virtual ~Head() {}
				virtual void *copy() = 0;
				const type_info& type;
				Head(const type_info& type): type(type) {}
				void *data() { return this + 1; }
			};
			template <class T> struct THead: public Head {
				THead(): Head(typeid(T)) {}
				virtual ~THead() override { ((T*)data())->~T(); }
				virtual void *copy() override {
					return new(new(malloc(sizeof(Head) + sizeof(T)))
							   THead() + 1) T(*(const T*)data()); }
			};
			void *data;
			Head *head() const { return (Head*)data - 1; }
			void *copy() const { return data ? head()->copy() : nullptr; }
		public:
			Attribute(): data(nullptr) {}
			Attribute(const Attribute& src): data(src.copy()) {}
			Attribute(Attribute&& src): data(src.data) { src.data = nullptr; }
			template <class T> Attribute(const T& src): data(
															 new(new(malloc(sizeof(Head) + sizeof(T))) THead<T>() + 1) T(src)) {}
			~Attribute() {
				if(!data) return;
				Head* head = this->head();
				head->~Head(); free(head); }
			bool empty() const {
				return data == nullptr; }
			const type_info& type() const {
				assert(data);
				return ((Head*)data - 1)->type; }
			template <class T>
			T& value() {
				if (!data || type() != typeid(T))
					throw bad_cast();
				return *(T*)data; }
			template <class T>
			const T& value() const {
				if (!data || type() != typeid(T))
					throw bad_cast();
				return *(T*)data; }
			template <class T>
			T& value_unsafe() {
				if (!data)
					throw bad_cast();
				return *(T*)data; }
			template <class T>
			const T& value_unsafe() const {
				if (!data)
					throw bad_cast();
				return *(T*)data; }
			template <class T>
			void setValue(const T& it) {
				if(!data)
					data = new(new(malloc(sizeof(Head) + sizeof(T)))
							   THead<T>() + 1) T(it);
				else {
					if (type() != typeid(T)) throw bad_cast();
					*(T*)data = it; }}
		};
	}
	
	class ParameterPanel : public Container{
	public:
		class Section;
		template<typename MuiType, typename DataType>
		class Row;
		
		ParameterPanel(string title = "");
		
		string getTitle();
		void setTitle( string text );
		
		string getSectionTitle( string sectionId );
		void setSectionTitle( string sectionId, string titel );

		Section * addSection( string sectionId );
		Section * addSection( string sectionId, string label );
		Section * getSection( string sectionId );
		Section * getCurrentSection();
		Section * getOrCreateSection( string sectionId, string label );
		
		Row<SliderWithLabel,float> * addSlider( string label, float min, float max, float value );
		Row<ToggleButton,bool> * addToggle( string label, bool selected );
		Row<TextArea,string> * addText( string label, string text );
		
		
		float getFloat( string component );
		void setFloat( string rowId, float value );
		
		void layout();
		
	private:
		Label * titleLabel;
		map<string,data::Attribute> rows;
		vector<Section*> sections;
		Section * currentSection;
		float labelColumnWidth;
		ofColor labelFg; 
		
	public:
		
		template<typename MuiType, typename DataType>
		struct Row : public Container{
			ParameterPanel * panel;
			Label * titleLabel;
			ofParameter<DataType> param;
			MuiType * content;
			bool customLabel;
			
			Row( ParameterPanel * panel, string title, Label * existingLabel, MuiType * content, DataType & data ) : Container( 0,0, 100, 30), content(content), param(ofParameter<DataType>(title,data)), panel(panel){
				if( existingLabel == NULL ){
					titleLabel = new Label(title,0,0,100,30);
					add(titleLabel);
					customLabel = false;
				}
				else{
					titleLabel = existingLabel;
					customLabel = true;
				}
				add(content);
			}
			
			void layout(){
				if( customLabel ){
					content->x = 0;
					content->width = width;
				}
				else{
					titleLabel->width = panel->labelColumnWidth;
					content->x = titleLabel->width + 2;
					content->width = width - titleLabel->width - 2;
				}
			}
			
			string getTitle(){
				return titleLabel->text;
			}
			
			void setTitle( string title ){
				titleLabel->text = title;
				titleLabel->commit();
			}
			
			void touchDown( ofTouchEventArgs &args ){
				content->requestFocus(args);
			}
		};
		
		struct Section : public Container{
			Label * titleLabel;
			string sectionId;
			
			Section( string sectionId, string title ) : Container( 0, 0, 100, 100 ), sectionId(sectionId){
				titleLabel = new Label(title,0,0,100,30);
				add(titleLabel);
			}
			vector<mui::data::Attribute> rows;
			
			void layout(){
				titleLabel->width = width;
				float yy = titleLabel->height;
				for( data::Attribute & row : rows ){
					Container * c = row.value_unsafe<Container*>();
					c->y = yy; 
					c->width = width;
					yy += c->height;
					c->handleLayout(); 
				}
				
				this->height = yy;
			}
			
			template<typename MuiType,typename DataType>
			void addRow( Row<MuiType,DataType> * row ){
				rows.push_back(row);
				add(row);
			}
			
			string getTitle(){
				return titleLabel->text;
			}
			
			void setTitle( string title ){
				titleLabel->text = title;
				titleLabel->commit();
			}
		};
	};
};

#endif /* defined(__MuiEditorTest__MuiParameterPanel__) */
