#include "stdafx.h"
#include "vector2D.h"

// calculate the sum of the forces
// acting on this vector at 1/60th 
// of a second later.  we store things
// internally as float, but return ints
/*int vec::CalcNextStep() {

	int i, max = forces.size();
	
	// 1) calc sum of forces to get new acceleration
	acceleration = 0;
	for (i = 0; i < max; i++)
		acceleration += forces[i]->GetAcceleration();		

	// 2) add acceleration to velocity to get new velocity
	velocity = (velocity + acceleration) * v_decay;
	if (v_decay != 1.0f && velocity < 0.25 && velocity > -0.25)
			velocity = 0;
	
	// 3) add velocity to position to get new position
	position += velocity;
	
	// 4) return the new position
	return GetPosition();
}*/


