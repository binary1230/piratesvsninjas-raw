#include "objectBackground.h"
#include "globals.h"
#include "gameState.h"
#include "animation.h"
#include "physSimulation.h"

void BackgroundObject::Update() {
	// scroll_offset -= speed;
	pos.SetX(0);

	if (currentAnimation) {
		currentAnimation->Update();
		currentSprite = currentAnimation->GetCurrentSprite();
	}
}

// XXX NEEDS TO USE THE NEW DRAWING CODE.
// THIS IS BROKEN. 
/*void BackgroundObject::Draw() {
	// We want to wrap the background around the level.  Compute the offset
	int camera_left = simulation->GetCameraLeft();
	int camera_top  = simulation->GetCameraTop();
	int offset_x = (int(scroll_offset) - camera_left) % GetWidth();
	int offset_y = -camera_top % GetHeight();
	
	// Draw it twice, repeating
	//DrawAtOffset( offset_x , offset_y );
	//DrawAtOffset( offset_x + GetWidth(), offset_y );
}*/

bool BackgroundObject::Init(GameState *_game_state) {
		SetGameState(_game_state);
		SetupCachedVariables();
		return true;
}

BackgroundObject::BackgroundObject(float _speed) {	
	speed = _speed;
	scroll_offset = 0.0f;
}

BackgroundObject::~BackgroundObject() {}

Object* BackgroundObject::New(GameState* gameState, XMLNode &xDef) {
	ObjectProperties props;
	BackgroundObject* obj = new BackgroundObject(1.0f);
	
	if (!obj || !obj->Init(gameState) )
		return NULL;

	props.feels_user_input = 0;
	props.feels_gravity = 0;
	props.feels_friction = 0;
	props.is_overlay = 1;
	props.tile_x = 1;
	props.tile_y = 1;
	obj->SetProperties(props);

	obj->SetXY(0, obj->game_state->ScreenHeight());
	
	obj->currentAnimation = NULL;
	obj->animations.resize(1);
	obj->animations[0] = new Animation();
	obj->currentAnimation = obj->animations[0];
	obj->currentAnimation->Init(gameState);

	int duration = 60;
	obj->currentAnimation->PushImage("data/back.bmp", duration);
	
	obj->currentSprite = obj->currentAnimation->GetCurrentSprite();
	
	return obj;
}
