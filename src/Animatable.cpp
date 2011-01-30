/*
 *  Animatable.cpp
 *  Cocoa Test
 *
 *  Created by Oriol Ferrer Mesià on 30/10/09.
 *  Copyright 2009 uri.cat. All rights reserved.
 *
 */

#include "Animatable.h"


void Animatable::setup(){

	initMathTables();	//just in case, wont do much harm if they are already inited
								//as they wont reinit twice if they were already inited

	transitionSpeed_ = 1.0f / DEFAULT_ANIMATION_DURATION;
	percentDone_ = 0.0f;
	animating_ = false;
	paused_ = false;
	playcount_ = 0;
	repeat_ = PLAY_ONCE;
	curveStyle_ = EASE_IN_EASE_OUT;
}


float Animatable::calcCurveAt( float percent ){

	float r = percent;
	
	switch ( curveStyle_ ) {

		case EASE_IN_EASE_OUT:
			r = 0.5f - 0.5f * fastCosRad( M_PI * percent ); break;
		
		case EASE_IN:
			r = 1.0f + fastSinRad( M_PI_2 * percent - M_PI_2); break;

		case EASE_OUT:	
			r = fastSinRad( M_PI_2 * percent ); break;

		case LINEAR:
			break;
	}
	
	return r;
}


void Animatable::update(float dt){

	if (animating_ == true && paused_ == false){
		
		percentDone_ += transitionSpeed_ * dt;
		
		if (percentDone_ >= 1.0f){

			animating_ = false;
			percentDone_ = 1.0f;
						
			switch (repeat_) {

				case PLAY_ONCE:	
					break;	//nothing to do;

				case LOOP_BACK_AND_FORTH_SWAP_CURVE:
					if ( curveStyle_ == EASE_IN)
						curveStyle_ = EASE_OUT;
					else if ( curveStyle_ == EASE_OUT)
							curveStyle_ = EASE_IN;					
					
				case LOOP_BACK_AND_FORTH:
					swapOriginDestination();	//this will tell our subclass to swap values so we can loop back
					
				case LOOP:	
					animating_ = true;
					percentDone_ = 0.0f;							
					break;

				
				case LOOP_BACK_AND_FORTH_ONCE_SWAP_CURVE:
					if ( curveStyle_ == EASE_IN)
						curveStyle_ = EASE_OUT;
					else if ( curveStyle_ == EASE_OUT)
							curveStyle_ = EASE_IN;					

				case LOOP_BACK_AND_FORTH_ONCE:
					if (playcount_ >= 1){	//time to stop
						//we are done
					}else{	
						animating_ = true;
						percentDone_ = 0.0f;
						playcount_++;
						swapOriginDestination();	//this will tell our subclass to swap values so we can loop back
					}
					break;
			}
		}
	}
}


void Animatable::startAnimation(){
	percentDone_ = 0.0f;
	animating_ = true;
	playcount_ = 0;
	paused_ = false;
}


void Animatable::reset(){
	percentDone_ = 0.0f;
	animating_ = false;
	playcount_ = 0;
	paused_ = false;
}


void Animatable::setDuration( float seconds ){
	transitionSpeed_ = 1.0f / seconds;
}


void Animatable::setRepeatType( animRepeat repeat ){
	repeat_ = repeat;
}


void Animatable::setCurve( animCurve curveStyle){
	curveStyle_ = curveStyle;
}


float Animatable::getPercentDone(){ 
	return percentDone_; 
}		


bool Animatable::isAnimating(){ 
	return animating_;
}


bool Animatable::hasFinishedAnimating(){ 
	return !animating_;
}


void Animatable::pause(){
	paused_ = true;
}


void Animatable::resume(){
	paused_ = false;
}
