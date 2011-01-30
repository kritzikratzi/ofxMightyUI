
/**
*  AnimatableFloat.h
*  
*
*  Created by Oriol Ferrer Mesia on 24/09/08.
*
*

  ###### WHAT IS IT ######

			   val
				|
				|				   ****
				|			    ***
				|			  **
				|		 	 *
				|		    *
				|		   *
				|   	 ** 
				|	  ***
				| **** 
				+------------------------------> time

	This is meant to be used as a float replacement. It allows you to make a float evolve over time
	from its original value to a newly imposed value. But instead of using a linear curve over time, it
	will use a nice slow-start slow-end curve that's more suitable for certain animations/movements.


  ###### HOW TO USE ######

	//Learn by example: 
	//lets say we want an alpha value to grow from 0 to 1 nicely
	//we would do:

		:::::::::::::::::::::::::::::::::::::::::::::::::

		AnimatableFloat myAlpha;
		  
		void init(){
			
			myAlpha.reset( 0 );			//set the starting value
			myAlpha.setDuration( 1 );	//set the animation duration (in seconds)
			myAlpha.animateTo( 1.0 );	//trigger the animation, giving it a destiation value (1.0)
		}

		void update(double dt){
			
			myAlpha.update( dt );			//update the AnimatableFloat
		}

		void draw(){
			
			glColor4f(1 .0f, 1.0f, 1.0f, myAlpha.val() );	//retrieve the current value for myAlpha by calling "val()"
			drawMyColoredStuff();
		}

*/

#ifndef AnimatableFloat_h__
#define AnimatableFloat_h__

#include "Animatable.h"
#include "MathTables.h"

class AnimatableFloat : public Animatable{
	    	
	public:

		AnimatableFloat();
		~AnimatableFloat(){};

		void update( float dt );
	
		void animateTo( float newVal );
		void animateToIfFinished( float newVal );	///starts new animation to newVal only if there isnt an anim going on
		void animateFromTo( float originalValue, float destinationValue );

		float val();								///gives you the current value.
		void reset(float newVal);					///sets an original value, and stops animation
		void reset();								///goes back to the original value

		// ## MUST IMPLEMENT ##
		void swapOriginDestination() ;				//must be implemented, superclass will call when loop_back_and_forth
													//must swap values [ originalVal_  <->  target_val ]

	
		///trying to make everyone life's easier with operators, I want an AnimatableFloat to behave 
		///like a float here, but doesn't quite work all the time
		///so you'd better use the .val() method to get the current value

		operator float(){ return val(); }			///return current value when asking for float
		operator double(){ return val(); }			///return current value when asking for double
	
	private:

		float		originalVal_;
		float		targetVal_;

};

#endif // AnimatableFloat_h__