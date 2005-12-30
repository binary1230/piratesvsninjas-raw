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
#include "animations.h"
#include "physSimulation.h"

#define DEFAULT_JUMP_VELOCITY 8.0f
#define DEFAULT_DRAG 0.95f
#define DEFAULT_MIN_VELOCITY 0.3f

#define FLOOR_HEIGHT 21

// XXX the physics is all hardcoded in here for now
// eventually, ALL collision stuff will be taken care
// of by the objects outside this one.
void PlayerObject::Update() {

	assert(currentAnimation != NULL);
	currentAnimation->Update();
				
	int w = simulation->GetWidth();
	int floor_height = FLOOR_HEIGHT + GetHeight();

	// Compute the new position
	pos = Solve();

	// See if we're out of bounds
	if (pos.GetX() < 0) {
		vel.SetX(-vel.GetX());
		pos.SetX(0);
	} else if (pos.GetX() > (w - GetWidth()) ) {
		vel.SetX(-vel.GetX());
		pos.SetX(w - GetWidth());
	}

	// See if we hit the floor
	if (pos.GetY() < floor_height) {
			pos.SetY(floor_height);
	}

	// If we're on the floor.. 
	if (pos.GetY() == floor_height) {
					
		// Then we can jump.
		if (game_state->GetKey(GAMEKEY_JUMP)) {
			vel.SetY(jump_velocity);
	  }	else {
			vel *= drag;	
			
			// If on floor, do we draw the standing sprite?
			if (/*fabs(accel.GetX()) > 0.0f &&*/ fabs(vel.GetX()) < min_velocity) {
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
	SetupCachedVariables();
	return true;
}

PlayerObject::PlayerObject() {
	jump_velocity = DEFAULT_JUMP_VELOCITY;
	min_velocity = DEFAULT_MIN_VELOCITY;
	mass = 1.0f;
	drag = DEFAULT_DRAG;
}
PlayerObject::~PlayerObject() {}

//! Factory method, creates new PlayerObjects from XML files
//! NOTE: this only takes an ObjectDefinition XML fragment,
//! it does not take the Object XML fragment.
Object* PlayerObject::New(GameState* gameState, XMLNode &xDef) {
	
	// ObjectProperties props;
	PlayerObject* obj = new PlayerObject();
	int i, max, iterator = 0;

	XMLNode xProps;

	// init the object
	if (!obj || !obj->Init(gameState) )
		return NULL;

	// load the animations
	obj->LoadAnimations(xDef);
	
		// END OF MOVED STUFF.
	
	//fprintf(stderr, "--- default_str = %s\n", xAnims.getAttribute("default"));
	//fprintf(stderr, "--- def = %i, current = %x\n", def, obj->currentAnimation);

	// get the object properties
	xProps = xDef.getChildNode("properties");

	sscanf(xProps.getChildNode("jumpVelocity").getText(), 
									"%f", &obj->jump_velocity);
	sscanf(xProps.getChildNode("minVelocity").getText(), 
									"%f", &obj->min_velocity);
	sscanf(xProps.getChildNode("drag").getText(), 
									"%f", &obj->drag);
	sscanf(xProps.getChildNode("mass").getText(), 
									"%f", &obj->mass);
	
	return obj;
}

// --------------------------------------------------------------
// XXX OLD METHOD, HARDCODED 
// --------------------------------------------------------------
/* Factory method, creates new PlayerObjects
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
	duration = 4;
	anim->PushImage("data/wait1.bmp", duration);
	anim->PushImage("data/wait2.bmp", duration * 6);

	anim = obj->animations[PLAYER_JUMPING] = new Animation();
	anim->Init(gameState);
	duration = 3;
	anim->PushImage("data/spin1.bmp", duration);
	anim->PushImage("data/spin2.bmp", duration);
	anim->PushImage("data/spin3.bmp", duration);
	anim->PushImage("data/spin4.bmp", duration);

	obj->currentAnimation = obj->animations[PLAYER_STANDING];	
	
	return obj;
}*/


