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
#include <typeindex>

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
	};
	
	
	
	
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
		
		Row<SliderWithLabel,float> * addSliderInt( string label, int min, int max, int value );
		Row<SliderWithLabel,float> * addSlider( string label, float min, float max, float value, int decimalDigits = 2 );
		Row<ToggleButton,bool> * addToggle( string label, bool selected );
		Row<TextArea,string> * addText( string label, string text );
		
		void setLabelColumnWidth( float width ); 
		
		bool getBool( string rowId );
		void setBool( string rowId, bool value );
		int getInt( string rowId );
		void setInt( string rowId, int value );
		float getFloat( string rowId );
		void setFloat( string rowId, float value );
		string getString( string rowId );
		void setString( string rowId, string value );
		
		void layout();
		
		
		template<typename MuiType, typename DataType>
		void registerGetter( function<DataType(MuiType*)> func ){
			getters.insert(pair<type_index,data::Attribute>(type_index(typeid(Row<MuiType,DataType>*)), func));
		};
		
		template<typename MuiType, typename DataType>
		void registerSetter( function<void(MuiType*, DataType & type)> func ){
			setters.insert(pair<type_index,data::Attribute>(type_index(typeid(Row<MuiType,DataType>*)), func));
		};
		
		template<typename T>
		T getValue( string rowId ){
			map<string,mui::data::Attribute>::iterator it = rows.find(rowId);
			if( it != rows.end() ){
				mui::data::Attribute &attr = it->second;
				Row<Container,T> * row = attr.value_unsafe<Row<Container,T>*>();
				
				auto getter = getters.find(type_index(attr.type()));
				if( getter != getters.end() ){
					auto & func = getter->second.value_unsafe<function<T(Container*)>>();
					return func(row->control);
				}
				else{
					ofLogError()
					<< "ofxMightyUI in getValue<T>('" << rowId << "'): no getter for row '" << rowId << "'. You can register a custom one with " << endl
					<< "parameterPanel.registerGetter<MuiClass,DataType>([](MuiClass * obj){ return obj->myVal; }); ?" << endl;
				}
			}
			else{
				ofLogError() << "ofxMightyUI in getValue<T>('" << rowId << "'): No row with this name was found. " << endl;
			}
			
			return T();
		}
		
		template<typename T>
		void setValue( string rowId, const T &value ){
			map<string,mui::data::Attribute>::iterator it = rows.find(rowId);
			if( it != rows.end() ){
				mui::data::Attribute &attr = it->second;
				Row<Container,T> * row = attr.value_unsafe<Row<Container,T>*>();
				
				auto setter = setters.find(type_index(attr.type()));
				if( setter != setters.end() ){
					auto & func = setter->second.value_unsafe<function<void(Container*,const T&)>>();
					func(row->control, value);
					return;
				}
				else{
					ofLogError()
					<< "ofxMightyUI in setValue<T>('" << rowId << "'): no getter for row '" << rowId << "'. You can register a custom one with " << endl
					<< "parameterPanel.registerGetter<TargetClass,float>([](TargetClass * obj){ return obj->myVal; }); ?" << endl;
				}
			}
			else{
				ofLogError() << "ofxMightyUI in setValue<T>('" << rowId << "'): No row with this name was found. " << endl;
			}
			
			return;
		}
		
	protected:
		Label * titleLabel;
		map<string,data::Attribute> rows;
		map<type_index,data::Attribute> getters;
		map<type_index,data::Attribute> setters;
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
			MuiType * control;
			bool customLabel;
			
			Row( ParameterPanel * panel, string title, Label * existingLabel, MuiType * control, DataType & data ) : Container( 0,0, 100, 20), control(control), param(ofParameter<DataType>(title,data)), panel(panel){
				if( existingLabel == NULL ){
					titleLabel = new Label(title,0,0,70,20);
					add(titleLabel);
					customLabel = false;
				}
				else{
					titleLabel = existingLabel;
					customLabel = true;
				}
				add(control);
			}
			
			void layout(){
				if( customLabel ){
					control->x = 0;
					control->width = width;
				}
				else{
					titleLabel->width = panel->labelColumnWidth;
					control->x = titleLabel->width + 2;
					control->width = width - titleLabel->width - 2;
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
				control->requestFocus(args);
			}
		};
		
		struct Section : public Container{
			Label * titleLabel;
			string sectionId;
			
			Section( string sectionId, string title ) : Container( 0, 0, 100, 100 ), sectionId(sectionId){
				titleLabel = new Label(title,0,0,60,20);
				add(titleLabel);
			}
			vector<mui::data::Attribute> rows;
			
			void layout(){
				float yy;
				if( titleLabel->text == "" ){
					yy = 0;
				}
				else{
					titleLabel->width = width;
					yy = titleLabel->height;
				}
				for( data::Attribute & row : rows ){
					Container * c = row.value_unsafe<Container*>();
					c->y = yy;
					c->x = 5;
					c->width = width-10;
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