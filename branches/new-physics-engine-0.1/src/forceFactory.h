#ifndef FORCE_FACTORY_H
#define FORCE_FACTORY_H

#include <allegro.h>
#include <stdio.h>

class Force;
class GameState;

#include "gameBase.h"
#include "force.h"

// THIS CLASS IS BAD.
// It will be rewritten, for now it JUST WORKS.
class ForceFactory : public GameBase {
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
