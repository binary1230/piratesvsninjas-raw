#ifndef DOOROBJECT_H
#define DOOROBJECT_H

#include "stdafx.h"

#include "object.h"

class GameState;
class ObjectFactory;
class PhysSimulation;

class DoorObject : public Object {
	protected:
		// e.g. What action happens on Activate()
	
	public:
		bool Init(GameState* _game_state, PhysSimulation *p);
		
		void Update();

		// Activate this door's action.
		void Activate();

		DoorObject();
		~DoorObject();
			
		friend class ObjectFactory;
};

#endif // DoorObject_H   
