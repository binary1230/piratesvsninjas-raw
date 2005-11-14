#ifndef FORCE_FACTORY_H
#define FORCE_FACTORY_H

#include <allegro.h>
#include <stdio.h>

class ForceFactory;

#include "globals.h"
#include "gameBase.h"

#include "force.h"
#include "forceInput.h"
#include "forceGravity.h"

#include "platform.h"

// THIS CLASS IS BAD.
// It will be rewritten, for now it JUST WORKS.
class ForceFactory : GameBase {
	protected:

	public:
		int Init(GameState* _game_state);
		void Shutdown();

		Force* CreateForce(ForceType type);
		void DeleteForce(Force*);
		
		ForceFactory();
		~ForceFactory();
};

#endif // FORCE_FACTORY_H
