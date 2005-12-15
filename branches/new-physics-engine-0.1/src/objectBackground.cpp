#include "objectBackground.h"
#include "globals.h"
#include "gameState.h"

void BackgroundObject::Update() {
	scroll_offset -= speed;
}

void BackgroundObject::Draw() {
	DrawAtOffset( int(scroll_offset) % GetWidth(), 0 );
	DrawAtOffset( int(scroll_offset) % GetWidth() + GetWidth(), 0 );
}

bool BackgroundObject::Init(GameState *_game_state) {
		SetGameState(_game_state);
		return true;
}

BackgroundObject::BackgroundObject(float _speed) {	
	speed = _speed;
	scroll_offset = 0.0f;
}

BackgroundObject::~BackgroundObject() {}

Object* BackgroundObject::New(GameState* gameState) {
	ObjectProperties props;
	BackgroundObject* obj = new BackgroundObject(1.0f);
	
	if (!obj || !obj->Init(gameState) )
		return NULL;

	props.feels_user_input = 0;
	props.feels_gravity = 0;
	props.feels_friction = 0;
	obj->SetProperties(props);

	obj->SetXY(0, obj->game_state->Height());

	obj->currentAnimation = NULL;
	
	return obj;
}


