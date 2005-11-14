#include "objectRadiusBlock.h"

void RadiusBlockObject::Update() {
	theta += RADIUS_BLOCK_SPEED;
}

bool RadiusBlockObject::Init(GameState *_game_state) {
		SetGameState(_game_state);
		return true;
}

void RadiusBlockObject::Draw() {
	DrawAtOffset(	int(sinf(theta) * radius) , 
								int(cosf(theta) * radius) );
}

RadiusBlockObject::RadiusBlockObject() : theta(0.0f) {}
RadiusBlockObject::~RadiusBlockObject() {}
