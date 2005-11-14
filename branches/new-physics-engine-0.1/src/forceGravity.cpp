#include "forceGravity.h"

Vector2D ForceGravity::GetAcceleration(Object* obj) {
		// quick physics info
		//  acelleration = g = -9.81 m/sec^2 (gravity)
		
		//  t = 0.016666667 (1/60th of a second)
		//  at = -0.1635
		
		// Gravity has no X acceleration
		return Vector2D(0.0f, -0.1635f * obj->GetMass()); 
}

ForceGravity::ForceGravity() {
	type = FORCE_GRAVITY;
}
ForceGravity::~ForceGravity() {}
