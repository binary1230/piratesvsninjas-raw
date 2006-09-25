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
#include "physSimulation.h"
#include "objectDoor.h"
#include "objectFactory.h"

#define DEFAULT_JUMP_VELOCITY 8.0f
#define DEFAULT_DRAG 0.95f
#define DEFAULT_MIN_VELOCITY 0.3f

// The name of the object to spawn for "skidding"
// like the white puffs when you skid
#define SKID_OBJECT_TYPE "skid"

void PlayerObject::ScreenBoundsConstraint() {
	if (pos.GetX() < 0) {
		// vel.SetX(-vel.GetX()); // flip velocity
		vel.SetX(0); 							// stop
		pos.SetX(0);
	} else if (pos.GetX() > (simulation->GetWidth() - GetWidth()) ) {
		// vel.SetX(-vel.GetX());	// flip velocity
		vel.SetX(0);							// stop
		pos.SetX(simulation->GetWidth() - GetWidth());
	}
}

void PlayerObject::UpdateSpriteFlip() {
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
}

void PlayerObject::UpdateRunningAnimationSpeed() {
	// alter the speed of the animation based on the velocity
	// fprintf(stderr, "vel=%f\n", fabs(vel.GetX()));
	if (fabs(vel.GetX()) < 3.0f)
		currentAnimation->SetSpeedMultiplier(10);// slow
	else if (fabs(vel.GetX()) < 7.0f)
		currentAnimation->SetSpeedMultiplier(6);// med
	else if (fabs(vel.GetX()) < 13.0f)
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
		vel.SetY(jump_velocity);
		PlaySound("jump");
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

	if (fabs(accel.GetX()) > 0.0f || fabs(vel.GetX()) > 0.0f ) {
		DoWalking();
		return;
	}
}

void PlayerObject::DoWalking() {
	state = WALKING;
	
	if (next_skid_time > 0)
		next_skid_time--;

	DoCommonGroundStuff();
	
	currentAnimation = animations[PLAYER_WALKING];

	// if we go too slow, then stop us and make us STANDING
	if (accel.GetX() == 0.0f && fabs(vel.GetX()) < min_velocity) {
		vel.SetX(0);
		DoStanding();
	}

	// If acceleration and velocity are in the opposite directions,
	// then we are skidding and trying to turn around
	if (	(accel.GetX() > 0.0f && vel.GetX() < 0.0f) ||
				(accel.GetX() < 0.0f && vel.GetX() > 0.0f) ) {

		if (next_skid_time == 0) {
			next_skid_time = 1;
			Object* objSkid = OBJECT_FACTORY->CreateObject(SKID_OBJECT_TYPE);
			
			float skid_vel_x = 8.0f;

			if (vel.GetX() < 0.0f)
				skid_vel_x *= -1.0f;

			objSkid->SetDisplayTime(2);
			objSkid->SetXY(pos);
			objSkid->SetVelXY(skid_vel_x, 0.0f);
			
			if (objSkid)
				simulation->AddObject(objSkid, layer);
		}
	}
		
	UpdateRunningAnimationSpeed();
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

	if (vel.GetY() < 0) {
		DoFalling();
		return;
	}
}

void PlayerObject::DoFalling() {
	state = FALLING;

	currentAnimation = animations[PLAYER_JUMPING];

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
		
	if (obj->GetProperties().is_fan) 
		return;

  if (obj->GetProperties().is_solid) {
    
		Vector2D newpos;
    d = GetBound(obj, newpos);
    
		pos = newpos;
		UpdateProjectionRectFromCollisions(newpos);

    if (d.left || d.right)
      vel.SetX(0);

    if (d.down || d.up)
      vel.SetY(0);
  }

	if (obj->GetProperties().is_spring) {
		vel.SetY(obj->GetProperties().spring_strength);
	}

	if (obj->GetProperties().is_ring) {
		ring_count++;
	}
}

bool PlayerObject::Init(PhysSimulation *p) {
	next_skid_time = 0;
	simulation = p;
	
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

	return (xProps.getChildNode("jumpVelocity").getFloat(jump_velocity) &&
					xProps.getChildNode("minVelocity").getFloat(min_velocity) &&
					xProps.getChildNode("drag").getFloat(drag) );
}

void PlayerObject::UpdateState() {
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
			fprintf(stderr, " -- PLAYEROBJECT ERROR: Unkown state asked for!\n");
			assert(false);
			break;
	}
}
