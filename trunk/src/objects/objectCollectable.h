#ifndef COLLECTABLE_OBJECT_H
#define COLLECTABLE_OBJECT_H

#include "stdafx.h"

#include "object.h"

class GameState;
class ObjectFactory;
class PhysSimulation;

//! A "simple" Object (e.g. scenery) - No collisions
class CollectableObject : public Object {
	protected:
				
	public:
		bool Init(GameState* _game_state, PhysSimulation *p);
		
		void Update();

		CollectableObject();
		~CollectableObject();

		void Collide(Object* obj);
	
		friend class ObjectFactory;
};

#endif // CollectableObject_H   
