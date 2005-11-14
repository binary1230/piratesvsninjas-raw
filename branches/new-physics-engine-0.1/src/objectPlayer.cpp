#include "objectPlayer.h"

void PlayerObject::Update() {

	/*int w = game_state->Width(), h = game_state->Height();
	BITMAP* dst_bitmap = game_state->GetDrawingSurface();*/
	
	/*
	x = vectors[_dX].CalcNextStep();
	y = -vectors[_dY].CalcNextStep();

	if (y > h - bitmap->h) {
			y = h - bitmap->h;
			vectors[_dY].position = -y;
	}
		
	vectors[_dX].v_decay = 1.00f;	// no decay in the air

	// if we're OK to jump now.. do it.
	if (y == h - bitmap->h) {
		vectors[_dX].v_decay = 0.99f;	// decay on the ground
		if (game_state->GetKey(GAMEKEY_JUMP)) {
			vectors[_dY].velocity = 10; // Rand(2,12);
	  }
	}
	
	if (x < 0) {
			vectors[_dX].position = x = 0;
			vectors[_dX].velocity = -vectors[_dX].velocity;
	}
	
	if (x > w - bitmap->w) {
			vectors[_dX].position = x = w - bitmap->w;
			vectors[_dX].velocity = -vectors[_dX].velocity;
	}*/
}

void PlayerObject::Draw() {
	
	bool flip = false;
	
	/*if (vectors[_dX].acceleration == 0) {
		if (vectors[_dX].velocity > 0) {
			flip = true;
		}
	} else if (vectors[_dX].acceleration > 0) {
		flip = true;
	} */

	if (flip)
		draw_sprite_h_flip(game_state->GetDrawingSurface(), bitmap, GetX(), GetY());
	else 
		draw_sprite(game_state->GetDrawingSurface(), bitmap, GetX(), GetY());
}

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
