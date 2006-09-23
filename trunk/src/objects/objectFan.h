#ifndef FANOBJECT_H
#define FANOBJECT_H

#include "stdafx.h"
#include "object.h"

class ObjectFactory;
class PhysSimulation;

class FanObject : public Object {
	protected:
		float current_fan_speed;
	
	public:
		bool Init(PhysSimulation *p);
		
		void Update();
		void Collide(Object* obj);

		FanObject();
		~FanObject();
			
		friend class ObjectFactory;
};

#endif // FanObject_H   
