#include "objectPlayer.h"

#include "gameState.h"
#include "globals.h"
#include "input.h"
#include "inputLiveHandler.h"
#include "force.h"
#include "forceGravity.h"
#include "forceInput.h"
#include "vector2D.h"
#include "animation.h"

#define JUMP_VELOCITY 7.0f
#define DRAG 0.95f

// our animations 
#define PLAYER_WALKING 0
#define PLAYER_STANDING 1
#define PLAYER_JUMPING 2
#define PLAYER_MAX_ANIMATIONS 3

void PlayerObject::Update() {

	currentAnimation->Update();
				
	int w = game_state->Width();

	// Compute the new position
	pos = Solve();

	// See if we're out of bounds
	if (pos.GetX() < 0) {
		vel.SetX(-vel.GetX());
		pos.SetX(0);
	} else if (pos.GetX() > w - GetWidth()) {
		vel.SetX(-vel.GetX());
		pos.SetX(w - GetWidth());
	}

	// See if we hit the floor
	if (pos.GetY() < GetHeight()) {
			pos.SetY(GetHeight());
	}

	// If we're on the floor.. 
	if (pos.GetY() == GetHeight()) {
					
		// Then we can jump.
		if (game_state->GetKey(GAMEKEY_JUMP)) {
			vel.SetY(JUMP_VELOCITY);
	  }	else {
			vel *= DRAG;	
			
			// If on floor, do we draw the standing sprite?
			if (/*fabs(accel.GetX()) > 0.0f &&*/ fabs(vel.GetX()) < 0.2f) {
				vel.SetX(0);
				currentAnimation = animations[PLAYER_STANDING];
			} else {
				currentAnimation = animations[PLAYER_WALKING];

				// alter the speed of the animation based on the velocity
				if (vel.GetX() < 2.0f)
					currentAnimation->SetSpeedMultiplier(2);
				else if (vel.GetX() < 0.5f)
					currentAnimation->SetSpeedMultiplier(3);
				else
					currentAnimation->SetSpeedMultiplier(1);
			}
		}
	} else {
		// We're not on the floor, we're falling or jumping
		currentAnimation = animations[PLAYER_JUMPING];
	}

	// figure out whether to flip the sprite or not
	if (accel.GetX() == 0.0f) {
		if (vel.GetX() > 0.0f)
			flip_x = true;
		else if (vel.GetX() < 0.0f)
			flip_x = false;
	} else if (accel.GetX() > 0.0f) {
		flip_x = true;
	} else {
		flip_x = false;
	}
}

bool PlayerObject::Init(GameState* _game_state) {
	SetGameState(_game_state);
	return true;
}

PlayerObject::PlayerObject() {}
PlayerObject::~PlayerObject() {}

// Factory method, creates new PlayerObjects
Object* PlayerObject::New(GameState* gameState) {
	ObjectProperties props;
	PlayerObject* obj = new PlayerObject();

	if (!obj || !obj->Init(gameState) )
		return NULL;

	props.feels_user_input = 1;
	props.feels_gravity = 1;
	props.feels_friction = 0;
	obj->SetProperties(props);

	obj->SetX(20);
	obj->SetY(80);
	obj->SetMass(1.0f);
	
	Animation* anim;
	obj->animations.resize(PLAYER_MAX_ANIMATIONS);

	int duration;
	
	anim = obj->animations[PLAYER_WALKING] = new Animation();
	anim->Init(gameState);
	duration = 4;
	anim->PushImage("data/run1.bmp", duration);
	anim->PushImage("data/run2.bmp", duration);
	anim->PushImage("data/run3.bmp", duration);
	anim->PushImage("data/run4.bmp", duration);
	anim->PushImage("data/run5.bmp", duration);
	anim->PushImage("data/run6.bmp", duration);
	anim->PushImage("data/run7.bmp", duration);
	anim->PushImage("data/run8.bmp", duration);
	
	anim = obj->animations[PLAYER_STANDING] = new Animation();
	anim->Init(gameState);
	duration = 2;
	anim->PushImage("data/wait1.bmp", duration);
	anim->PushImage("data/wait2.bmp", duration * 8);

	anim = obj->animations[PLAYER_JUMPING] = new Animation();
	anim->Init(gameState);
	duration = 3;
	anim->PushImage("data/spin1.bmp", duration);
	anim->PushImage("data/spin2.bmp", duration);
	anim->PushImage("data/spin3.bmp", duration);
	anim->PushImage("data/spin4.bmp", duration);

	obj->currentAnimation = obj->animations[PLAYER_STANDING];	
	
	return obj;
}
