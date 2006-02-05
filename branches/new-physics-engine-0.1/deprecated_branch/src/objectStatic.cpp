#include "objectStatic.h"

#include "globals.h"
#include "animation.h"
#include "gameState.h"

void StaticObject::Update() {
	if (currentAnimation) {
		currentAnimation->Update();
		currentSprite = currentAnimation->GetCurrentSprite();
	}

	pos = Solve();
}

bool StaticObject::Init(GameState *_game_state) {
	SetGameState(_game_state);
	return BaseInit();
}

StaticObject::StaticObject() {}
StaticObject::~StaticObject() {}

Object* StaticObject::New(	GameState* gameState, 
																XMLNode &xDef, 
																XMLNode &xObj) {
				
	StaticObject* obj = new StaticObject();

	if (!obj || !obj->Init(gameState) )
		return NULL;

	if (!obj->LoadAnimations(xDef))
		return NULL;

	return obj;
}