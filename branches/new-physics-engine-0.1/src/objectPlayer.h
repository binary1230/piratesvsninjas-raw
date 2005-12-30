// this file is a TEST ONLY

#ifndef PLAYEROBJECT_H
#define PLAYEROBJECT_H

#include <allegro.h>
#include <math.h>
#include <stdio.h>
#include <vector>

using namespace std;

class ObjectPlayer;
class Animation;
class GameState;
class BaseInput;
class Animation;

#include "object.h"
#include "xmlParser.h"

class PlayerObject : public Object {
	protected:
		BaseInput* input;
		vector<Animation*> animations;

		float jump_velocity;
		float min_velocity;
		float drag;
				
	public:
		void Update();

		bool Init(GameState* _game_state);
		
		PlayerObject();
		~PlayerObject();
		
		static Object* New(GameState* gameState, XMLNode &xDef);
};

#endif // PLAYER_OBJECT_H
