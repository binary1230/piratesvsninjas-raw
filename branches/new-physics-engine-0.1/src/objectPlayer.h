// this file is a TEST ONLY

#ifndef PLAYEROBJECT_H
#define PLAYEROBJECT_H

#include <allegro.h>
#include <math.h>
#include <stdio.h>
#include <vector>

using namespace std;

class ObjectPlayer;

#include "globals.h"
#include "object.h"
#include "input.h"
#include "inputLiveHandler.h"
#include "force.h"
#include "forceGravity.h"
#include "forceInput.h"
#include "vector2D.h"

// directions

class PlayerObject : public Object {
	protected:
		BaseInput* input;
				
	public:
		void Update();

		bool Init(GameState* _game_state);
		
		PlayerObject();
		~PlayerObject();
};

#endif // PLAYER_OBJECT_H
