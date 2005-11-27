#include "objectPlayer.h"

#define JUMP_VELOCITY 13.0f

void PlayerObject::Update() {

	int w = game_state->Width();

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
			vel.SetY(JUMP_VELOCITY);
	  }	
	}

	// figure out whether to flip the sprite or not
	flip_x = false;

	if (accel.GetX() == 0.0f) {
		if (vel.GetX() > 0.0f) {
			flip_x = true;
		}
	} else if (accel.GetX() > 0.0f) {
		flip_x = true;
	} 
}

bool PlayerObject::Init(GameState* _game_state) {
	SetGameState(_game_state);
	return true;
}

PlayerObject::PlayerObject() {
}

PlayerObject::~PlayerObject() {}
