/**
*  AnimatableFloat.cpp
*  
* 
*  Created by Oriol Ferrer Mesia on 2008/10/09.
*
*/


#include "AnimatableFloat.h"



AnimatableFloat::AnimatableFloat(){

	Animatable::setup();
	
	originalVal_ = targetVal_= 0.0f;
}


void AnimatableFloat::update( float dt ){
	
	Animatable::update( dt );
	
}


void AnimatableFloat::animateTo( float newVal ){

	originalVal_ = val();
	targetVal_ = newVal;
	
	Animatable::startAnimation();
	
}


void AnimatableFloat::animateFromTo( float originalValue, float destinationValue ){

	Animatable::startAnimation();
	
	originalVal_ = originalValue;
	targetVal_ = destinationValue;
}


void AnimatableFloat::animateToIfFinished( float newVal ){
	if ( animating_ == false ){
		animateTo(newVal);
	}
}



float AnimatableFloat::val(){
	
	float mappedDistribution = calcCurveAt( percentDone_ );	///percentDone_ is [0..1] & tells me where we are between orig and target
	return originalVal_ + ( targetVal_ - originalVal_ ) * mappedDistribution ;
}


void AnimatableFloat::reset( float newVal ){

	Animatable::reset();
	originalVal_ = targetVal_ = newVal; 
}


void AnimatableFloat::reset(){

	Animatable::reset();
	targetVal_ = originalVal_;
}

void AnimatableFloat::swapOriginDestination(){
	float tempVal = originalVal_;
	originalVal_ = targetVal_;
	targetVal_ = tempVal;
}

