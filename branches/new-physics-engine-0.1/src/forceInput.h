/* Forces: Gravity
 * all time increments are 1/60th of a second */

#ifndef FORCEINPUT_H
#define FORCEINPUT_H

#include <allegro.h>

class ForceInput;
class Object;

#include "force.h"
#include "globals.h"

class ForceInput : public Force {
	public:
		Vector2D GetForce(Object*);
		uint controller_num;

		//! Set which controller we monitor
		void SetControllerNum(uint _c) {controller_num = _c;};
		
		ForceInput();
		virtual ~ForceInput();
};

#endif // FORCEINPUT_H
