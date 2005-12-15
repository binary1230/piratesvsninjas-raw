/* Forces: Gravity
 * all time increments are 1/60th of a second */

#ifndef FORCEINPUT_H
#define FORCEINPUT_H

#include <allegro.h>

class ForceInput;
class Object;

#include "force.h"

class ForceInput : public Force {
	public:
		Vector2D GetForce(Object*);

		ForceInput();
		virtual ~ForceInput();
};

#endif // FORCEINPUT_H
