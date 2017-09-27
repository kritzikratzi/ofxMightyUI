//
//  L.h
//
//  Created by Hansi on 26.07.15.
//
//
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
		L( mui::Container * target ) : targets({target}){
		}
		
		L( std::initializer_list<mui::Container*> targets ) : targets(targets){
		}
		
		template<typename T>
		L( vector<T*> targets ) : targets(targets.begin(),targets.end()){
		}
		
		L & with( function<void(mui::Container*)> func ){
			return *this;
		}
		
		L & x( const float x ){
			for(auto target : targets ){
				target->x = x;
			}
			return *this;
		}
		
		L & y( float y ){
			for(auto target : targets ){
				target->y = y;
			}
			return *this;
		}
		
		L & pos( float x, float y ){
			for(auto target : targets ){
				target->x = x;
				target->y = y;
			}
			return *this;
		}
		
		L & posTL( float x, float y ){
			for(auto target : targets ){
				target->x = x;
				target->y = y;
			}
			return *this;
		}
		
		L & posTR( float x, float y ){
			for(auto target : targets ){
				if( target->parent == NULL ) continue;
				target->x = target->parent->width - x - target->width;
				target->y = y;
			}
			return *this;
		}
		
		L & posBL( float x, float y ){
			for(auto target : targets ){
				if( target->parent == NULL ) continue;
				target->x = x;
				target->y = target->parent->height - y - target->height;
			}
			return *this;
		}
		
		L & posBR( float x, float y ){
			for(auto target : targets ){
				if( target->parent == NULL ) continue;
				target->x = target->parent->width - x - target->width;
				target->y = target->parent->height - y - target->height;
			}
			return *this;
		}
		
		L & rightOf( mui::Container * dest, float space = 0 ){
			for(auto target : targets ){
				target->x = dest->x + dest->width + space;
				target->y = dest->y;
			}
			return *this;
		}
		
		L & leftOf( mui::Container * dest, float space = 0 ){
			for(auto target : targets ){
				target->x = dest->x - target->width - space;
				target->y = dest->y;
			}
			return *this;
		}
		
		L & alignLeftEdgeTo( mui::Container * dest, float space = 0, mui::HorizontalAlign align = mui::Left ){
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
		
		L & alignHorizontalCenterTo( mui::Container * dest, float space = 0, mui::HorizontalAlign align = mui::Center ){
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
		
		L & alignRightEdgeTo( mui::Container * dest, float space = 0, mui::HorizontalAlign align = mui::Right ){
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
		
		L & maxWidth( float width ){
			for(auto target : targets ){
				target->width = min(width, target->width);
			}
			return *this;
		}
		
		L & width( float width ){
			for(auto target : targets ){
				target->width = width;
			}
			return *this;
		}
		
		L & height( float height ){
			for(auto target : targets ){
				target->height = height;
			}
			return *this;
		}
		
		L & size( float width, float height ){
			for(auto target : targets ){
				target->width = width;
				target->height = height;
			}
			return *this;
		}
		
		L & stretchToRightEdgeOfParent( float space = 0 ){
			for(auto target : targets ){
				if( target->parent == NULL ) return *this;
				target->width = target->parent->width - space - target->x;
			}
			return *this;
		}
		
		L & stretchToBottomEdgeOfParent( float space = 0 ){
			for(auto target : targets ){
				if( target->parent == NULL ) return *this;
				target->height = target->parent->height - space - target->y;
			}
			return *this;
		}
		
		L & widthTo( mui::Container * dest, float space = 0 ){
			for(auto target : targets ){
				target->width = dest->x - target->x - space;
			}
			return *this;
		}
		
		L & heightTo( mui::Container * dest, float space = 0 ){
			for(auto target : targets ){
				target->height = dest->y - target->y - space;
			}
			return *this;
		}
		
		L & below( mui::Container * dest, float space = 0 ){
			for(auto target : targets ){
				target->x = dest->x;
				target->y = dest->y + dest->height + space;
			}
			return *this;
		}
		
		L & above( mui::Container * dest, float space = 0 ){
			for(auto target : targets ){
				target->x = dest->x;
				target->y = dest->y - target->height - space;
			}
			return *this;
		}
		
		L & moveBy( float x, float y ){
			for(auto target : targets ){
				target->x += x;
				target->y += y;
			}
			return *this;
		}
		
		L & spreadEvenlyHorizontally( float x, float width, float padding = 0 ){
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
		
		L & spreadEvenlyHorizontally( float x, float width, mui::HorizontalAlign align ){
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
		
		L & spreadEvenlyVertically( float y, float height, float padding = 0 ){
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
		
		L & spreadEvenlyVertically( float y, float height, mui::VerticalAlign align ){
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
		
		L & colums( ofVec2f p0, float spacing = 1 ){
			size_t N = targets.size();
			float x = p0.x;
			for(int i = 0; i < N; i++ ){
				if(targets[i]->visible){
					targets[i]->x = x;
					targets[i]->y = p0.y;
					x += targets[i]->width + spacing;
				}
			}
			return *this;
		}
		
		L & columsFromRight( ofVec2f p0, float spacing = 1 ){
			size_t N = targets.size();
			float x = p0.x;
			for(int i = N-1; i >= 0; i-- ){
				if(targets[i]->visible){
					targets[i]->x = x - targets[i]->width;
					targets[i]->y = p0.y;
					x -= targets[i]->width + spacing;
				}
			}
			return *this;
		}
		
		L & rows( ofVec2f p0, float spacing = 1 ){
			size_t N = targets.size();
			float y = p0.y;
			for(int i = 0; i < N; i++ ){
				if(targets[i]->visible){
					targets[i]->x = p0.x;
					targets[i]->y = y;
					y += targets[i]->height + spacing;
				}
			}
			return *this;
		}
		
		L & bounds(float x, float y, float w, float h){
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
		
		L filter(function<bool(mui::Container*)> filterFunc){
			std::vector<mui::Container*> newTargets;
			for(auto & target : targets){
				if(target->visible){
					newTargets.push_back(target);
				}
			}
			return L(newTargets);
		}
		
		ofRectangle boundingBox(){
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
