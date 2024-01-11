//
//  L.h
//
//  Created by Hansi on 26.07.15.
// V1.18 (2022/09/16)
//      * added method to center align
//      * added method to get the first parent of all targets
// V1.17 (2019/11/25)
//      * added methods for vertical alignment
//      * added method to align in parent
//      * added empty constructor
//      * methods are now const-correct
// V1.16 (2019/7/9)
//      * guess a lot went undocumented? or unmerged?
//      * added stretchToLeftEdgeOfParent and stretchToTopEdgeOfParent
// V1.15 (2017/11/8)
//      * fixed typo, renamed colums* to columns*
// V1.14
//      * added columns() to complete rows()
// V1.13
//      * fixed boundingBox() calculation
// V1.12
//      * added a filter(func) function
// V1.11
//      * added spreadEvenlyVertically to complement spreadEvenlyHorizontally
// V1.10
//      * added a rows() method to lay out a bunch of things below one another
// V1.9
//      * multiple targets are now possible
//      * added alignHorizontalCenterTo to complete the alignToLeft/Right edge methods
//      * added spreadEvenlyHorizontally to lay out a grid of elements
//      * added boundingBox() to compute the bounding box of selected elements
// V1.8
//      * added above()
// V1.7
//      * added size()
// v1.6
//      * added stretchToBottomEdgeOfParent()
// v1.5
//      * added height(...)
// v1.4
//      * posTR,posBR,posBL take element size into account
//      * fixed widthTo(...)
// v1.3 added
//      * width: set a fixed width
// v1.2 added
//      * moveBy: moves by x/y pixels
//      * stretchToRightEdgeOfParent: stretches to right edge (optional margin)
//      * remove stretchToRightEdgeOf(): was too easy to use wrongly
// v1.1 added
//      * posTL (default), posBL, posBR, posTR which positions component in a corner of it's parent
//      * heightTo, widthTo
// v1.0 initial version
#ifndef mui_L_h
#define mui_L_h

#include "ofxMightyUI.h"

namespace mui{
	
	class L{
	public:
		
		L(){
		}
		
		L( mui::Container * target ) : targets({target}){
		}
		
		L( std::initializer_list<mui::Container*> targets ) : targets(targets){
		}
		
		template<typename T>
		L( vector<T*> targets ) : targets(targets.begin(),targets.end()){
		}
		
		const L & with( std::function<void(mui::Container*)> func ) const{
			for(mui::Container * c : targets) func(c);
			return *this;
		}
		
		const L & x( const float x ) const{
			for(auto target : targets ){
				target->x = x;
			}
			return *this;
		}
		
		const L & y( float y ) const{
			for(auto target : targets ){
				target->y = y;
			}
			return *this;
		}
		
		const L & pos( float x, float y ) const{
			for(auto target : targets ){
				target->x = x;
				target->y = y;
			}
			return *this;
		}
		
		const L & posTL( float x, float y ) const{
			for(auto target : targets ){
				target->x = x;
				target->y = y;
			}
			return *this;
		}
		
		const L & posTR( float x, float y ) const{
			for(auto target : targets ){
				if( target->parent == NULL ) continue;
				target->x = target->parent->width - x - target->width;
				target->y = y;
			}
			return *this;
		}
		
		const L & posBL( float x, float y ) const{
			for(auto target : targets ){
				if( target->parent == NULL ) continue;
				target->x = x;
				target->y = target->parent->height - y - target->height;
			}
			return *this;
		}
		
		const L & posBR( float x, float y ) const{
			for(auto target : targets ){
				if( target->parent == NULL ) continue;
				target->x = target->parent->width - x - target->width;
				target->y = target->parent->height - y - target->height;
			}
			return *this;
		}
		
		const L & rightOf( mui::Container * dest, float space = 0 ) const{
			for(auto target : targets ){
				target->x = dest->x + dest->width + space;
				target->y = dest->y;
			}
			return *this;
		}
		
		const L & leftOf( mui::Container * dest, float space = 0 ) const{
			for(auto target : targets ){
				target->x = dest->x - target->width - space;
				target->y = dest->y;
			}
			return *this;
		}
		
		const L & alignLeftEdgeTo( mui::Container * dest, float space = 0, mui::HorizontalAlign align = mui::Left ) const{
			for(auto target : targets ){
				if( align == mui::Left ){
					target->x = dest->x + space;
				}
				else if( align == mui::Right ){
					target->x = dest->x + dest->width - space;
				}
				else if( align == mui::Center ){
					target->x = dest->x + dest->width/2 + space;
				}
			}
			return *this;
		}
		
		const L & alignHorizontalCenterTo( mui::Container * dest, float space = 0, mui::HorizontalAlign align = mui::Center ) const{
			for(auto target : targets ){
				if( align == mui::Left ){
					target->x = dest->x + space - target->width/2;
				}
				else if( align == mui::Right ){
					target->x = dest->x + dest->width - target->width/2 - space;
				}
				else if( align == mui::Center ){
					target->x = dest->x + dest->width/2 - target->width/2 + space;
				}
			}
			return *this;
		}
		
		const L & alignRightEdgeTo( mui::Container * dest, float space = 0, mui::HorizontalAlign align = mui::Right ) const{
			for(auto target : targets ){
				if( align == mui::Left ){
					target->x = dest->x - target->width + space;
				}
				else if( align == mui::Right ){
					target->x = dest->x + dest->width - target->width - space;
				}
				else if( align == mui::Center ){
					target->x = dest->x + dest->width/2 - target->width + space;
				}
			}
			return *this;
		}
		
		const L & alignTopEdgeTo( mui::Container * dest, float space = 0, mui::VerticalAlign align = mui::Top ) const{
			for(auto target : targets ){
				if( align == mui::Top ){
					target->y = dest->y + space;
				}
				else if( align == mui::Bottom ){
					target->y = dest->y + dest->height - space;
				}
				else if( align == mui::Middle ){
					target->y = dest->y + dest->height/2 + space;
				}
			}
			return *this;
		}
		
		const L & alignVerticalMiddleTo( mui::Container * dest, float space = 0, mui::VerticalAlign align = mui::Middle ) const{
			for(auto target : targets ){
				if( align == mui::Top ){
					target->y = dest->y + space - target->height/2;
				}
				else if( align == mui::Bottom ){
					target->y = dest->y + dest->height - target->height/2 - space;
				}
				else if( align == mui::Middle ){
					target->y = dest->y + dest->height/2 - target->height/2 + space;
				}
			}
			return *this;
		}
		
		const L & alignBottomEdgeTo( mui::Container * dest, float space = 0, mui::VerticalAlign align = mui::Bottom ) const{
			for(auto target : targets ){
				if( align == mui::Top ){
					target->y = dest->y - target->height + space;
				}
				else if( align == mui::Bottom ){
					target->y = dest->y + dest->height - target->height - space;
				}
				else if( align == mui::Middle ){
					target->y = dest->y + dest->height/2 - target->height + space;
				}
			}
			return *this;
		}
		
		const L & alignToParent( mui::HorizontalAlign halign = mui::Center, mui::VerticalAlign valign = mui::Middle, mui::Inset inset = mui::Inset(0,0,0,0)) const{
			for(auto target : targets){
				if( halign == mui::Left ) target->x = inset.left;
				else if(halign == mui::Center) target->x = (target->parent?target->parent->width/2:0) - target->width/2;
				else if(halign == mui::Right) target->x = (target->parent?target->parent->width:0) - target->width - inset.right;
				
				if( valign == mui::Top) target->y = inset.top;
				else if(valign == mui::Middle) target->y = (target->parent?target->parent->height/2:0) - target->height/2;
				else if(valign == mui::Bottom) target->y = (target->parent?target->parent->height:0) - target->height - inset.bottom;
			}
			
			return *this; 
		}
		
		const L & center(float space = 0) const{
			float w = 0;
			mui::Container * parent = firstParent();
			
			for(auto target : targets){
				w += target->width;
			}
			
			w += space*(targets.size()-1);
			
			float x0 = parent->width/2 - w/2;
			for(auto target : targets){
				target->x = x0;
				x0 += target->width + space;
			}
			
			return *this;
		}

		const L & maxWidth( float width ) const{
			for(auto target : targets ){
				target->width = std::min(width, target->width);
			}
			return *this;
		}
		
		const L & width( float width ) const{
			for(auto target : targets ){
				target->width = width;
			}
			return *this;
		}
		
		const L & height( float height ) const{
			for(auto target : targets ){
				target->height = height;
			}
			return *this;
		}
		
		const L & size( float width, float height ) const{
			for(auto target : targets ){
				target->width = width;
				target->height = height;
			}
			return *this;
		}
		
		const L & stretchToTopEdgeOfParent(float space = 0) const{
			for (auto target : targets) {
				target->height += target->y - space;
				target->y = space;
			}
			return *this;
		}

		const L & stretchToLeftEdgeOfParent(float space = 0) const{
			for (auto target : targets) {
				target->width += target->x - space;
				target->x = space; 
			}
			return *this;
		}

		const L & stretchToRightEdgeOfParent( float space = 0 ) const{
			for(auto target : targets ){
				if( target->parent == NULL ) continue;
				target->width = target->parent->width - space - target->x;
			}
			return *this;
		}
		
		const L & stretchToBottomEdgeOfParent( float space = 0 ) const{
			for(auto target : targets ){
				if( target->parent == NULL ) continue;
				target->height = target->parent->height - space - target->y;
			}
			return *this;
		}
		
		const L & widthTo( mui::Container * dest, float space = 0 ) const{
			for(auto target : targets ){
				target->width = dest->x - target->x - space;
			}
			return *this;
		}
		
		const L & heightTo( mui::Container * dest, float space = 0 ) const{
			for(auto target : targets ){
				target->height = dest->y - target->y - space;
			}
			return *this;
		}
		
		const L & below( mui::Container * dest, float space = 0 ) const{
			for(auto target : targets ){
				target->x = dest->x;
				target->y = dest->y + dest->height + space;
			}
			return *this;
		}
		
		const L & above( mui::Container * dest, float space = 0 ) const{
			for(auto target : targets ){
				target->x = dest->x;
				target->y = dest->y - target->height - space;
			}
			return *this;
		}
		
		const L & moveBy( float x, float y ) const{
			for(auto target : targets ){
				target->x += x;
				target->y += y;
			}
			return *this;
		}
		
		const L & spreadEvenlyHorizontally( float x, float width, float padding = 0 ) const{
			size_t N = targets.size();
			if(N==1){
				targets.front()->x = x;
				targets.front()->width = width;
			}
			else if(N>1){
				float w = (width-(N-1)*padding)/N;
				for(int i = 0; i < N; i++ ){
					targets[i]->x = x+i*(w+padding);
					targets[i]->width = w;
				}
			}
			return *this;
		}
		
		const L & spreadEvenlyHorizontally( float x, float width, mui::HorizontalAlign align ) const{
			size_t N = targets.size();
			if( N > 0 ){
				float w = width/N;
				float move = align==mui::Left?0:(align==mui::Center?0.5:1.0);
				for(int i = 0; i < N; i++ ){
					targets[i]->x = x+i*w + (w-targets[i]->width)*move;
				}
			}
			return *this;
		}
		
		const L & spreadEvenlyVertically( float y, float height, float padding = 0 ) const{
			size_t N = targets.size();
			if(N==1){
				targets.front()->y = y;
				targets.front()->height = height;
			}
			else if(N>1){
				float h = (height-(N-1)*padding)/N;
				for(int i = 0; i < N; i++ ){
					targets[i]->y = y+i*(h+padding);
					targets[i]->height = h;
				}
			}
			return *this;
		}
		
		const L & spreadEvenlyVertically( float y, float height, mui::VerticalAlign align ) const{
			size_t N = targets.size();
			if( N > 0 ){
				float h = height/N;
				float move = align==mui::Top?0:(align==mui::Middle?0.5:1.0);
				for(int i = 0; i < N; i++ ){
					targets[i]->y = y+i*h + (h-targets[i]->height)*move;
				}
			}
			return *this;
		}
		
		const L & columns( ofVec2f p0, float spacing = 1 ) const{
			size_t N = targets.size();
			float x = p0.x;
			for(int i = 0; i < N; i++ ){
				targets[i]->x = x;
				targets[i]->y = p0.y;
				x += targets[i]->width + spacing;
			}
			return *this;
		}
		
		const L & columnsFromRight( ofVec2f p0, float spacing = 1 ) const{
			size_t N = targets.size();
			float x = p0.x;
			if (N > 0) {
				for (size_t i = N; i > 0; ) {
					auto target = targets[--i]; 
					target->x = x - target->width;
					target->y = p0.y;
					x -= target->width + spacing;
				}
			}
			return *this;
		}
		
		L operator +(const mui::L & other){
			vector<mui::Container*> t = targets;
			t.insert(t.end(), other.targets.begin(), other.targets.end());
			return L(t);
		}
		
		L &operator +=(const mui::L & other){
			targets.insert(targets.end(), other.targets.begin(), other.targets.end());
			return *this;
		}
		
		mui::Container * first() const{
			return targets.size()>0?targets[0] : nullptr;
		}
		
		mui::Container * last() const{
			return targets.size()>0?targets.back() : nullptr;
		}
		
		const L & rows( ofVec2f p0, float spacing = 1) const{
			size_t N = targets.size();
			float y = p0.y;
			for(int i = 0; i < N; i++ ){
				targets[i]->x = p0.x;
				targets[i]->y = y;
				y += targets[i]->height + spacing;
			}
			return *this;
		}
		
		const L & bounds(float x, float y, float w, float h) const{
			for(auto target : targets){
				target->x = x;
				target->y = y;
				target->width = w;
				target->height = h;
			}

			return *this; 
		}
		
		L filterVisible(){
			std::vector<mui::Container*> newTargets;
			for(auto & target : targets){
				if(target->visible){
					newTargets.push_back(target);
				}
			}
			return L(newTargets);
		}
		
		L filter(std::function<bool(mui::Container*)> filterFunc){
			std::vector<mui::Container*> newTargets;
			for(auto & target : targets){
				if(filterFunc(target)){
					newTargets.push_back(target);
				}
			}
			return L(newTargets);
		}
		
		mui::Container * firstParent() const{
			for(auto target : targets){
				if(target->parent) return target->parent;
			}
			return nullptr;
		}
		
		ofRectangle boundingBox() const{
			ofRectangle result;
			bool first = true;
			
			for( auto target : targets ){
				if(first) result = target->getBounds(), first = false;
				else result = result.getUnion(target->getBounds());
			}
			return result; 
		}
		
		std::vector<mui::Container*> targets;
	};
}

#endif
