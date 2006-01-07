#include "objectRadiusBlock.h"

#include "globals.h"
#include "animation.h"
#include "gameState.h"
#include "physSimulation.h"

void RadiusBlockObject::Update() {
	if (currentAnimation) {
		currentAnimation->Update();
		currentSprite = currentAnimation->GetCurrentSprite();
	}
	theta += RADIUS_BLOCK_SPEED;
	pos.SetX( int(sinf(theta) * radius) ); 
	pos.SetY( int(cosf(theta) * radius) );
}

bool RadiusBlockObject::Init(GameState *_game_state) {
	SetGameState(_game_state);
	return BaseInit();
}

RadiusBlockObject::RadiusBlockObject() : theta(0.0f) {}
RadiusBlockObject::~RadiusBlockObject() {}

// Factory method, creates new RadiusBlockObjects
// XXX animation stuff needs to be abstracted
Object* RadiusBlockObject::New(GameState* gameState, XMLNode &xDef) {
	ObjectProperties props;
	RadiusBlockObject* obj = new RadiusBlockObject();

	if (!obj || !obj->Init(gameState) )
		return NULL;

	props.feels_user_input = 0;
	props.feels_gravity = 0;
	props.feels_friction = 0;
	props.is_overlay = 0;
	obj->SetProperties(props);

	obj->SetXY( Rand(0, obj->simulation->GetWidth()  ), 
							Rand(0, obj->simulation->GetHeight() ) );
	obj->SetTheta(Rand(0,360));
	obj->SetRadius(Rand(20,300));

	obj->animations.resize(1);
	obj->animations[0] = new Animation();
	obj->currentAnimation = obj->animations[0];
	obj->currentAnimation->Init(gameState);

	int duration = 6;
	obj->currentAnimation->PushImage("data/fly1.bmp", duration);
	obj->currentAnimation->PushImage("data/fly2.bmp", duration);

	return obj;
}
