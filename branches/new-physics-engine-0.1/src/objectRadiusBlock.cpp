#include "objectRadiusBlock.h"

#include "globals.h"
#include "animation.h"

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
		
Object* RadiusBlockObject::New(GameState* gameState) {
	ObjectProperties props;
	RadiusBlockObject* obj = new RadiusBlockObject();
	
	if (!obj || !obj->Init(gameState))
		return NULL;

	props.feels_user_input = 0;
	props.feels_gravity = 0;
	props.feels_friction = 0;
	obj->SetProperties(props);

	obj->SetXY(Rand(0, SCREEN_SIZE_X), Rand(0, SCREEN_SIZE_Y));
	obj->SetTheta(Rand(0,360));
	obj->SetRadius(Rand(20,300));

	obj->currentAnimation = NULL;
	
	return obj;
}
