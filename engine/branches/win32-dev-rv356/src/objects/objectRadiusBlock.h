// this file is a TEST ONLY

#ifndef RADIUSBLOCKOBJECT_H
#define RADIUSBLOCKOBJECT_H

#define RADIUS_BLOCK_SPEED 0.01f

#include <math.h>
#include <stdio.h>

class RadiusBlockObject;
class GameState;
class ObjectFactory;
class PhysSimulation;

#include "object.h"
#include "xmlParser.h"

//! An object that moves around in a circle

//! \deprecated This thing was originally just a test Object
class RadiusBlockObject : public Object {
	protected:
		float theta;
		int radius;

	public:
		bool Init(GameState* _game_state, PhysSimulation *p);

		void Update();

		inline void SetTheta(float angle) { theta = angle; }
		inline float GetTheta() { return theta; }
		inline void SetRadius(int r) { radius = r; }
		inline float GetRadius() { return radius; }

		RadiusBlockObject();
		~RadiusBlockObject();
			
		//! Factory method to create a new radius block object
		friend class ObjectFactory;
};

#endif // RadiusBlockObject_H   
