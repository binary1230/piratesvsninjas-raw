#include "objectRadiusBlock.h"

#include "globals.h"
#include "animation.h"
#include "gameState.h"
#include "physSimulation.h"

void RadiusBlockObject::Update() {
	BaseUpdate();
	UpdateSimpleAnimations();

	theta += RADIUS_BLOCK_SPEED;
	pos.SetX( int(sinf(theta) * radius) ); 
	pos.SetY( int(cosf(theta) * radius) );
}

bool RadiusBlockObject::Init(GameState *_game_state, PhysSimulation *p) {
	SetGameState(_game_state);
	simulation = p;
	return BaseInit();
}

RadiusBlockObject::RadiusBlockObject() : theta(0.0f) {}
RadiusBlockObject::~RadiusBlockObject() {}
