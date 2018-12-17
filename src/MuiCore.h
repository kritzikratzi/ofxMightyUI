//
//  MuiCore.h
//
//  Created by Hansi on 10.08.15.
//
//

#ifndef __Oscilloscope__MuiCore__
#define __Oscilloscope__MuiCore__


#include <vector>
#include <cmath>

#include "ofMain.h"
#include "CppTweener.h"

#ifndef OF_MAX_TOUCHES
#define OF_MAX_TOUCHES 20
#endif

namespace mui{
	enum HorizontalAlign{
		Left, Right, Center
	};
	enum VerticalAlign{
		Top, Bottom, Middle /* i wish i could support baseline, but it's not in ofTrueTypeFont yet! */
	};
	enum Direction{
		Horizontal = 1, Vertical = 2, Both = 3
	};

	class Inset {
	public: 
		float left{ 0 }, right{ 0 }, top{ 0 }, bottom{ 0 };
		Inset(float v = 0) : top(v), right(v), bottom(v), left(v){};
		Inset(float tb, float lr) : top(tb), right(lr), bottom(tb), left(lr) {};
		Inset(float top, float right, float bottom, float left) : top(top), right(right), bottom(bottom), left(left) {}; 
	};
	

	struct Cursor {
		enum CursorType {
			Default,
			Hand,
			IBeam,
			Crosshair,
			ResizeH,
			ResizeV,
			Custom
		};

		Cursor(CursorType type = Default, short customId = 0) : type(type),customId(customId){}
		Cursor& operator= (CursorType t) { type = t; if (type != Custom) customId = 0; return *this; }
		bool operator==(const Cursor & other) const { return type == other.type && (type != Custom || customId == other.customId); }
		bool operator!=(const Cursor & other) const { return type != other.type || (type == Custom && customId != other.customId); }

		CursorType type{ Default };
		short customId{ 0 };
	};
	
	// forward declare all classes not added directly
	class BarButton;
	class Button;
	class Container; 
	class Image;
	class InternalWindow;
	class Label;
	class Layout;
	class Root;
	class ScrollPane;
	
	template<typename T>
	class SegmentedSelect;
	
	class Slider;
	class SliderWithLabel;
	class TextureAtlas;
	class ToggleButton;

	class FilePicker;
	class TextArea; 
	
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
			Attribute& operator= (const Attribute & src){ data = src.copy(); return *this; }
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
}

// now include bare minimum
#include "MuiConfig.h"
#include "MuiEvents.h"
#include "Helpers.h"
#include "Layout.h"
#include "Container.h"



// wtf windows...
// https://code.google.com/p/py-lepton/source/browse/trunk/lepton/renderer.h?r=222
// um... looks like this can be cleaned up by paying more attention to float/double
#ifndef fminf
#define fminf(x, y) (((x) < (y)) ? (x) : (y))
#endif
#ifndef fmaxf
#define fmaxf(x, y) (((x) < (y)) ? (y) : (x))
#endif
#ifndef roundf
#define roundf(x) (x >= 0.0f ? floorf(x + 0.5f) : ceilf(x - 0.5f))
#endif



#endif /* defined(__Oscilloscope__MuiCore__) */
