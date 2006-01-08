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
			if (accel.GetX() == 0.0f && fabs(vel.GetX()) < min_velocity) {
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
			flip_x = false;
		else if (vel.GetX() < 0.0f)
			flip_x = true;
	} else if (accel.GetX() > 0.0f) {
		flip_x = false;
	} else {
		flip_x = true;
	}

	// set the current sprite to the current animation
	currentSprite = currentAnimation->GetCurrentSprite();
}

bool PlayerObject::Init(GameState* _game_state) {
	SetGameState(_game_state);
	return BaseInit();
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
//XXX memory leaks on failures here.
Object* PlayerObject::New(GameState* gameState, XMLNode &xDef) {
	
	// ObjectProperties props;
	PlayerObject* obj = new PlayerObject();

	// init the object
	if (!obj || !obj->Init(gameState) )
		return NULL;

	// load the animations
	AnimationMapping animation_map = GetPlayerAnimationMappings();
	if (!obj->LoadAnimations(xDef, &animation_map))
		return NULL;

	// get the object properties
	if (!obj->LoadProperties(xDef))
		return NULL;

	return obj;
}

bool PlayerObject::LoadProperties(XMLNode &xDef) {
	XMLNode xProps = xDef.getChildNode("properties");

	sscanf(xProps.getChildNode("jumpVelocity").getText(), 
									"%f", &jump_velocity);
	sscanf(xProps.getChildNode("minVelocity").getText(), 
									"%f", &min_velocity);
	sscanf(xProps.getChildNode("drag").getText(), 
									"%f", &drag);
	sscanf(xProps.getChildNode("mass").getText(), 
									"%f", &mass);	
	
	properties.feels_gravity = 		xProps.nChildNode("affectedByGravity"); 
	properties.feels_user_input =	xProps.nChildNode("affectedByInput1"); 
	properties.feels_friction =		xProps.nChildNode("affectedByFriction"); 

	return true;
}
