#ifndef COLLECTABLE_OBJECT_H
#define COLLECTABLE_OBJECT_H

#include "stdafx.h"

#include "object.h"

class GameState;
class ObjectFactory;

//! A "simple" Object (e.g. scenery) - No collisions
class CollectableObject : public Object {
	protected:
				
	public:
		bool Init();
		void Shutdown();
		
		void Update();

		CollectableObject();
		~CollectableObject();

		void OnCollide(Object* obj, const b2ContactPoint* pkContactPoint);
	
		friend class ObjectFactory;
};

#endif // CollectableObject_H   
