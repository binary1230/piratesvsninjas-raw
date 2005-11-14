#include "objectPlayer.h"

#define JUMP_VELOCITY 50.0f

void PlayerObject::Update() {
	int w = game_state->Width();
	int h = game_state->Height();

	// Compute the new position
	pos = Solve();

	// See if we're out of bounds
	if (pos.GetX() < 0) {
		vel.SetX(-vel.GetX());
		pos.SetX(0);
	} else if (pos.GetX() > w - bitmap->w) {
		vel.SetX(-vel.GetX());
		pos.SetX(w - bitmap->w);
	}

	// See if we hit the floor
	if (pos.GetY() < bitmap->h) {
			pos.SetY(bitmap->h);
	}

	// If we're on the floor.. 
	if (pos.GetY() == bitmap->h) {
	
		// Then we can jump.
		if (game_state->GetKey(GAMEKEY_JUMP)) {
			vel.SetY(JUMP_VELOCITY / mass);
	  }	
	}

	// figure out whether to flip the sprite or not
	flip_x = false;

	if (force.GetX() == 0.0f) {
		if (vel.GetX() > 0.0f) {
			flip_x = true;
		}
	} else if (force.GetX() > 0.0f) {
		flip_x = true;
	} 
}

// void PlayerObject::Draw() {
	
	// adjust Y for screen coordinates
	//int x = int(pos.GetX());
	//int y = int(game_state->Height() - pos.GetY());

//}

bool PlayerObject::Init(GameState* _game_state) {
	/*Force* force;*/

	SetGameState(_game_state);
	ResetForNextFrame();
	/*assert(GetGameState() != NULL);

	vectors[_dX].Clear();
	vectors[_dY].Clear();

	vectors[_dX].v_decay = 0.99f;

	force = new ForceInput();
	force->Init(GetGameState());
	vectors[_dX].Attach(force);

	force = new ForceGravity();
	force->Init(GetGameState());
	vectors[_dY].Attach(force);

	return true;
	*/
	return true;
}

PlayerObject::PlayerObject() {
}

PlayerObject::~PlayerObject() {}
