// this file is a TEST ONLY

#ifndef RADIUSBLOCKOBJECT_H
#define RADIUSBLOCKOBJECT_H

#define RADIUS_BLOCK_SPEED 0.01f

#include <math.h>
#include <stdio.h>

class RadiusBlockObject;
class GameState;

#include "object.h"

class RadiusBlockObject : public Object {
	protected:
		float theta;
		int radius;
				
	public:
		bool Init(GameState* _game_state);

		void Update();
		void Draw();

		inline void SetTheta(float angle) { theta = angle; }
		inline float GetTheta() { return theta; }
		inline void SetRadius(int r) { radius = r; }
		inline float GetRadius() { return radius; }

		RadiusBlockObject();
		~RadiusBlockObject();
			
		//! Factory method to create a new radius block object
		static Object* New(GameState*);
};

#endif // RadiusBlockObject_H   
