/* Forces: Gravity
 * all time increments are 1/60th of a second */

#ifndef FORCEGRAV_H
#define FORCEGRAV_H

class ForceGravity;
class Force;
class Object;

#include "globals.h"
#include "force.h"

class ForceGravity : public Force {
	protected:
		float gravitational_constant;
		
	public:
		Vector2D GetForce(Object*);

		ForceGravity(float _magnitude = -9.81f);
		virtual ~ForceGravity();
};

#endif // FORCEGRAB_H
