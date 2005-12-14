#ifndef OBJECT_FACTORY_H
#define OBJECT_FACTORY_H

#include <allegro.h>
#include <stdio.h>

class ObjectFactory;
class Object;

#include "globals.h"
#include "gameBase.h"

#include "platform.h"
#include "resourceLoader.h"

#include "objectIDs.h"

// THIS CLASS IS BAD.
// It will be rewritten, for now it JUST WORKS.
class ObjectFactory : GameBase {
	protected:
		ResourceLoader* resourceLoader;

	public:
		int Init(GameState* _game_state);
		void Shutdown();

		Object* CreateObject(uint id);
		void DeleteObject(Object*);
		
		ObjectFactory();
		~ObjectFactory();
};

#endif // OBJECT_FACTORY_H
