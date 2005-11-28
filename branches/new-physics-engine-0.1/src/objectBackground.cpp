#include "objectBackground.h"

void BackgroundObject::Update() {
	scroll_offset -= speed;
}

void BackgroundObject::Draw() {
	DrawAtOffset( int(scroll_offset) % bitmap->w, 0 );
	DrawAtOffset( int(scroll_offset) % bitmap->w + bitmap->w, 0 );
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
