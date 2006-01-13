#ifndef STILLOBJECT_H
#define STILLOBJECT_H

#include <stdio.h>

class GameState;

#include "object.h"
#include "xmlParser.h"

//! A "static" object, like scenery: no collisions
class StaticObject : public Object {
	protected:
				
	public:
		bool Init(GameState* _game_state);
		
		void Update();

		StaticObject();
		~StaticObject();
			
		static Object* New(GameState* gameState, XMLNode &xDef, XMLNode &xObj);
};

#endif // StaticObject_H   
