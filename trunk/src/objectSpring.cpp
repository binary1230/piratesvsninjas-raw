#include "objectSpring.h"

#include "globals.h"
#include "animation.h"
#include "gameState.h"

void SpringObject::Update() {
	UpdateFade();
	if (currentAnimation) {
		currentAnimation->Update();
		currentSprite = currentAnimation->GetCurrentSprite();
	}
}

bool SpringObject::Init(GameState *_game_state) {
	SetGameState(_game_state);
	return BaseInit();
}

SpringObject::SpringObject() {}
SpringObject::~SpringObject() {}

void SpringObject::Collide(Object* obj) {
	if (obj->GetProperties().is_player) {
		currentAnimation->Unfreeze();
		PlaySound("spring");
	}
}


