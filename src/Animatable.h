/*
 *  Animatable.h
 *  Cocoa Test
 *
 *  Created by Oriol Ferrer Mesià on 30/10/09.
 *  Copyright 2009 uri.cat. All rights reserved.
 *
 */

#ifndef _ANIMATABLE_INC
#define _ANIMATABLE_INC

#include <stdio.h>
#include <stdlib.h>
#include "MathTables.h"

#define DEFAULT_ANIMATION_DURATION	1.0f

class Animatable{
	
	public:

		enum animRepeat{
			PLAY_ONCE = 0,
			LOOP, 
			LOOP_BACK_AND_FORTH, 
			LOOP_BACK_AND_FORTH_SWAP_CURVE,	//only for EASE_IN && EASE_OUT, will swap them when looping back
			LOOP_BACK_AND_FORTH_ONCE,
			LOOP_BACK_AND_FORTH_ONCE_SWAP_CURVE //only for EASE_IN && EASE_OUT, will swap them when looping back
		};
	
		enum animCurve{
			EASE_IN_EASE_OUT = 0,
			EASE_IN,
			EASE_OUT,
			LINEAR
		};


		void setup();
		void update(float dt);

		void pause();					//really needed?
		void resume();					//

		void setCurve( animCurve curveStyle_ );
		void setRepeatType( animRepeat repeat );
		void setDuration( float seconds );

		float getPercentDone();			///get how much of the animation has been "walked"
		bool isAnimating();				///is the animation still going on?
		bool hasFinishedAnimating();	///has the animation finished?


		virtual ~Animatable(void) {}
		Animatable() {}


	protected:

		bool		animating_;
		bool		paused_;
	
		int 		playcount_;
	
		float		transitionSpeed_;	///this is 1/N (N == # of updates) it will take for the transition to end
		float		percentDone_;		/// [0..1]
	
		animRepeat 	repeat_;
		animCurve	curveStyle_;
	
		float calcCurveAt( float percent );

		void startAnimation();			///Used by subclasses to indicate we are starting an anim
		void reset();					///Used by subclasses to indicate a reset of an animation
	
	public:
	
		virtual void swapOriginDestination() = 0;
	
};

#endif