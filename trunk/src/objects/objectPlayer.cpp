#include "stdafx.h"
#include "objectPlayer.h"

#include "gameState.h"
#include "globals.h"
#include "input.h"
#include "gameSound.h"
#include "force.h"
#include "forceGravity.h"
#include "forceInput.h"
#include "vector2D.h"
#include "animation.h"
#include "animations.h"
#include "gameWorld.h"
#include "objectDoor.h"
#include "objectFactory.h"
#include "objectSpring.h"
#include "gameSound.h"
#include "effectsManager.h"
#include "globalDefines.h"

#define DEFAULT_JUMP_VELOCITY 8.0f
#define DEFAULT_DRAG 0.95f
#define DEFAULT_MIN_VELOCITY 0.3f

// The name of the object to spawn for "skidding"
// like the white puffs when you skid
#define SKID_OBJECT_TYPE "skid"

void PlayerObject::Shutdown() {
	BaseShutdown();
}

void PlayerObject::ScreenBoundsConstraint() {
	if (WORLD->PlayerAllowedOffscreen())
		return;

	if (pos.x < 0) {
		// vel.SetX(-vel.x); // flip velocity
		vel.x = 0; 							// stop
		pos.x = 0;
	} else if (pos.x > (WORLD->GetWidth() - width) ) {
		// vel.SetX(-vel.x);	// flip velocity
		vel.x = 0;							// stop
		pos.x = WORLD->GetWidth() - width;
	}
}

void PlayerObject::UpdateSpriteFlip() {
	if (accel.x == 0.0f) {
		if (vel.x > 0.0f)
			flip_x = false;
		else if (vel.x < 0.0f)
			flip_x = true;
	} else if (accel.x > 0.0f) {
		flip_x = false;
	} else {
		flip_x = true;
	}
}

void PlayerObject::UpdateRunningAnimationSpeed() {
	// alter the speed of the animation based on the velocity
	// TRACE("vel=%f\n", fabs(vel.x));
	if (fabs(vel.x) < 3.0f)
		currentAnimation->SetSpeedMultiplier(10);// slow
	else if (fabs(vel.x) < 7.0f)
		currentAnimation->SetSpeedMultiplier(6);// med
	else if (fabs(vel.x) < 13.0f)
		currentAnimation->SetSpeedMultiplier(2);// slight fast
	else 
		currentAnimation->SetSpeedMultiplier(1);// max
}

void PlayerObject::DoWalkThroughDoor() {
	state = WALKING_THRU_DOOR;

	// XXX don't have an animation for this yet...
	currentAnimation = animations[PLAYER_WALKING];

	if (door_in_front_of_us)
		door_in_front_of_us->Activate();

	door_in_front_of_us = NULL;
}

// Things common to STANDING, WALKING, and RUNNING
void PlayerObject::DoCommonGroundStuff() {
	if (!d.down) {
		DoFalling();
		return;
	}	

	if (door_in_front_of_us && INPUT->KeyOnce(PLAYERKEY_UP, controller_num)) {
		DoWalkThroughDoor();
		return;
	}

	if (INPUT->KeyOnce(PLAYERKEY_JUMP, controller_num)) {
		vel.y = jump_velocity;
		SOUND->PlaySound("jump");
		DoJumping();
		return;
  }	
	
	// Do ghetto friction
	vel *= drag;
}

void PlayerObject::DoStanding() {	
	state = STANDING;

	DoCommonGroundStuff();

	currentAnimation = animations[PLAYER_STANDING];

	if (fabs(accel.x) > 0.0f || fabs(vel.x) > 0.0f ) {
		DoWalking();
		return;
	}
}

void PlayerObject::DoWalking() {
	state = WALKING;
	
	DoCommonGroundStuff();
	
	currentAnimation = animations[PLAYER_WALKING];

	// if we go too slow, then stop us and make us STANDING
	if (accel.x == 0.0f && fabs(vel.x) < min_velocity) {
		vel.x = 0;
		DoStanding();
	}

	UpdateSkidding();

	UpdateRunningAnimationSpeed();
}

void PlayerObject::UpdateSkidding() {
	if (next_skid_time > 0)
		next_skid_time--;

	// If acceleration and velocity are in the opposite directions,
	// then we are skidding and trying to turn around
	if (	on_skateboard || 
			(accel.x > 0.0f && vel.x < 0.0f) ||
			(accel.x < 0.0f && vel.x > 0.0f) ) {

		if (next_skid_time == 0) {
			next_skid_time = 0;

			// Create a "skid" object (little white whisp at player's feet)
			Object* objSkid = EFFECTS->TriggerObject(this, "skid");
			
			if (objSkid) {
				float skid_vel_x = 6.0f;

				if (vel.x < 0.0f)
					skid_vel_x *= -1.0f;

				objSkid->SetDisplayTime(Rand(1,10));
				objSkid->SetVelXY(skid_vel_x, 0.0f);
				objSkid->FadeOut(Rand(4,10));
			}
		}
	}
}

// no distinction from walking yet.
void PlayerObject::DoRunning() {
	state = RUNNING;
}

void PlayerObject::DoJumping() {
	state = JUMPING;

	currentAnimation = animations[PLAYER_JUMPING];

	// really shouldn't have a downward 
	// collision on an upward jump
	if (d.down) {
		DoStanding();
		return;
	}

	if (vel.y < 0) {
		DoFalling();
		return;
	}
}

void PlayerObject::DoFalling() {
	state = FALLING;

	// XXX: should be PLAYER_FALLING when we have one.
	currentAnimation = animations[PLAYER_JUMPING];

	if (vel.y < -30.0f)
		vel.y = -30.0f;

	if (d.down) {
		DoStanding();
	}
}

void PlayerObject::DoWhistling() {
	state = WHISTLING;
}
void PlayerObject::DoLookingUp() {
	state = LOOKINGUP;
}
void PlayerObject::DoCrouchingDown() {
	state = CROUCHINGDOWN;
}

// Do things common to most every state
void PlayerObject::DoCommonStuff() {
	
	// let's do something interesting and random here, why not?
	// if they press a key here, then drop a bouncy ball.

	if (INPUT->KeyOnce(PLAYERKEY_ACTION1, controller_num) && 
			state != WALKING_THRU_DOOR) {
		
		Object* objBall = EFFECTS->TriggerEffect(this, "bomb");
			
		if (!objBall)
			return;

		int strength;
		if (!GLOBALS->Value("bomb_throw_strength", strength))
			return;

		float sign;
		if (flip_x)
			sign = -1;
		else
			sign = 1;


		if (GetInput(PLAYERKEY_UP, controller_num))
			objBall->SetVelXY(0.0f, vel.y + strength*1.7);

		else if (GetInput(PLAYERKEY_DOWN, controller_num))
			objBall->SetVelXY(0.0f, vel.y - strength);

		else
			objBall->SetVelXY(sign * strength + vel.x, vel.y + 6.0f);

		//objBall->SetVelXY(vel.x, 0.0f);
	}
}

void PlayerObject::Update() {
	BaseUpdate();
	
	assert(currentAnimation != NULL);
	currentAnimation->Update();

	ScreenBoundsConstraint();
	UpdateState();
	UpdateSpriteFlip();

	// set the current sprite to the current animation
	currentSprite = currentAnimation->GetCurrentSprite();
	
	// this will be set true on each collision with a door
	door_in_front_of_us = NULL;
}

void PlayerObject::Collide(Object* obj) {

	if (obj->GetProperties().is_door) {
		door_in_front_of_us = (DoorObject*)obj;
		return;
	}
		
	if (obj->GetProperties().is_fan || obj->GetProperties().is_ball)  
		return;

	if (obj->GetProperties().is_solid && !obj->GetProperties().is_player) {
		Vector2D newpos;
		d = GetBound(obj, newpos);
    
		pos = newpos;
		UpdateProjectionRectFromCollisions(newpos);

		if (d.left || d.right)
			vel.x = 0;

		if (d.down || d.up)
			vel.y = 0;
	}

	if (obj->GetProperties().is_spring) {
		SpringObject* sObj = (SpringObject*)obj;
		
		if (sObj->IsSpringActive())
			vel = sObj->GetSpringVector();
	}

	if (obj->GetProperties().is_ring) {
		++ring_count;
	}
}

bool PlayerObject::Init() {
	next_skid_time = 0;
	
	controller_num = 1;
	state = FALLING; 
	door_in_front_of_us = NULL;
	ring_count = 0;

	return BaseInit();
}

PlayerObject::PlayerObject() {
	jump_velocity = DEFAULT_JUMP_VELOCITY;
	min_velocity = DEFAULT_MIN_VELOCITY;
	mass = 1.0f;
	drag = DEFAULT_DRAG;
	state = FALLING;
	door_in_front_of_us = NULL;
	ring_count = 0;
	next_skid_time = 0;
}

PlayerObject::~PlayerObject() {}

bool PlayerObject::LoadPlayerProperties(XMLNode &xDef) {
	XMLNode xProps = xDef.getChildNode("properties");

	properties.is_player = 1;
	properties.is_solid = 1;
	on_skateboard = false;

	if (xProps.nChildNode("onSkateboard"))
		on_skateboard = true;

	return (xProps.getChildNode("jumpVelocity").getFloat(jump_velocity) &&
					xProps.getChildNode("minVelocity").getFloat(min_velocity) &&
					xProps.getChildNode("drag").getFloat(drag) );
}

void PlayerObject::UpdateState() {

	DoCommonStuff();

	switch (state) {
		case STANDING:
			DoStanding();
			break;
		case WALKING:
			DoWalking();
			break;
		case RUNNING:
			DoRunning();
			break;
		case JUMPING:
			DoJumping();
			break;
		case FALLING:
			DoFalling();
			break;
		case WHISTLING:
			DoWhistling();
			break;
		case LOOKINGUP:
			DoLookingUp();
			break;
		case CROUCHINGDOWN:
			DoCrouchingDown();
			break;
		case WALKING_THRU_DOOR:
			DoWalkThroughDoor();
			break;
		default:
			TRACE(" -- PLAYEROBJECT ERROR: Unkown state asked for!\n");
			assert(false);
			break;
	}
}
