#include "objectDoor.h"
#include "globals.h"
#include "animation.h"
#include "gameState.h"
#include "allegro_wrapper.h"

// activate the door's action
// someday this will be a 'switch (action) {case warp: ... }' etc
void DoorObject::Activate() {

	// XXX perhaps fade, or SOMETHING here instead of just exiting.
	
	GetGameState()->SignalEndCurrentMode();
}

void DoorObject::Update() {
	if (currentAnimation) {
		currentAnimation->Update();
		currentSprite = currentAnimation->GetCurrentSprite();
	}
}

bool DoorObject::Init(GameState *_game_state, PhysSimulation *p) {
	SetGameState(_game_state);
	simulation = p;
	return BaseInit();
}

DoorObject::DoorObject() {}
DoorObject::~DoorObject() {}
