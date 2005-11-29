#include "forceInput.h"

#define MAGNITUDE 10.0f

Vector2D ForceInput::GetForce(Object* obj) {
				
	if (!GetGameState()) {
			fprintf(stderr, "Gamestate is NULL??\n");
			abort();
	}

	// return a force based on 2 inputs.
	if (game_state->GetKey(GAMEKEY_LEFT)) 
		return Vector2D(-MAGNITUDE * TIMESTEP, 0.0f);

	else if (game_state->GetKey(GAMEKEY_RIGHT))
		return Vector2D( MAGNITUDE * TIMESTEP, 0.0f);

	else
		return Vector2D( 0.0f, 0.0f);	

}

ForceInput::ForceInput() {
	type = FORCE_INPUT;
}

ForceInput::~ForceInput() {}
