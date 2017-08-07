/*
 *  Layout.h
 *
 *  Created by hansi on 26.03.11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */


#pragma once

#include "MuiCore.h"
#include <functional>

namespace mui{
	class Layout{
		public: 
		Layout(){}; 
		virtual ~Layout(){};
		virtual void layout( Container * container ) = 0;
		
		Container * container; 
	};
	
	
	
	class RowLayout : public Layout{
	public: 
		RowLayout( int spacing = 3 ) : Layout(), spacing(spacing){}; 
		void layout( Container * container ); 
		
		int spacing; 
	};
	
	class LambdaLayout : public Layout{
	public:
		std::function<void(mui::Container*)> layoutFunc;
		LambdaLayout(std::function<void(mui::Container*)> layoutFunc);
		void layout( Container * container ); 
	};
}