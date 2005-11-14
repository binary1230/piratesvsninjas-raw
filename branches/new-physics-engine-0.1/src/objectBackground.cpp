#include "objectBackground.h"

void BackgroundObject::Update() {
}

bool BackgroundObject::Init(GameState *_game_state) {
		SetGameState(_game_state);
		return true;
}

BackgroundObject::BackgroundObject() {}
BackgroundObject::~BackgroundObject() {}
