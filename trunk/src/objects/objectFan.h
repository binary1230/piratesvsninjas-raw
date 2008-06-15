#ifndef FANOBJECT_H
#define FANOBJECT_H

#include "stdafx.h"
#include "object.h"

class ObjectFactory;

class FanObject : public Object {
	protected:
		float current_fan_speed;
	
	public:
		bool Init();
		void Shutdown();
		
		void Update();
		void OnCollide(Object* obj, const b2ContactPoint* pkContactPoint);

		FanObject();
		~FanObject();
			
		friend class ObjectFactory;
};

#endif // FanObject_H   
