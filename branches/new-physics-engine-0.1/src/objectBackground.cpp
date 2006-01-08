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

// We want to wrap the background around the level.  Compute the offset
void BackgroundObject::Draw() {
	int camera_left = simulation->GetCameraLeft();
	int camera_top  = simulation->GetCameraTop();
	int offset_x = (int(scroll_offset) - camera_left) % GetWidth();
	int offset_y = -camera_top % GetHeight();

	simulation->TransformViewToScreen(offset_x, offset_y);
	
	// Draw it twice, repeating
	DrawAtOffset( offset_x , offset_y );
	DrawAtOffset( offset_x + GetWidth(), offset_y );
}

bool BackgroundObject::Init(GameState *_game_state) {
		SetGameState(_game_state);
		return BaseInit();
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

	props.is_overlay = 1;
	obj->SetProperties(props);

	// obj->SetXY(0, obj->game_state->ScreenHeight());
	
	if (!obj->LoadAnimations(xDef) )
		return NULL;
	
	return obj;
}
