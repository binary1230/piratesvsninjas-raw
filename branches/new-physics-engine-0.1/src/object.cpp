#include "object.h"
#include "gameState.h"
#include "force.h"
#include "globals.h"
#include "gameBase.h"
#include "animation.h"
#include "physSimulation.h"

int Object::GetWidth() { return currentAnimation->Width(); }
int Object::GetHeight() {	return currentAnimation->Height(); }

//! Cache some commonly used stuff
void Object::SetupCachedVariables() {
	simulation = GetGameState()->GetPhysSimulation();
	level_width  = simulation->GetWidth();
	level_height = simulation->GetHeight();
}

void Object::Draw() {
	// takes into account the camera here
	DrawAtOffset(-simulation->GetCameraLeft(), -simulation->GetCameraTop());
}

void Object::DrawAtOffset(int x, int y) {	

	int real_y = game_state->ScreenHeight() - (int)pos.GetY() + y;
	
	if (currentAnimation)
		currentAnimation->DrawAt(
										(int)pos.GetX() + x, 
										real_y, 
										flip_x);
}

void Object::ApplyForce(Force* force) {
	// ignore certain types of forces
	if (	(!properties.feels_gravity && force->GetType() == FORCE_GRAVITY) ||
				(!properties.feels_user_input && force->GetType() == FORCE_INPUT) ||
				(!properties.feels_friction && force->GetType() == FORCE_FRICTION) )
		return;
	else
		accel += force->GetForce(this) / mass;
}

void Object::ResetForNextFrame() {
	accel.Clear();
}

//! Solve for new position based on velocity
Vector2D Object::Solve() {
	// Vector2D newpos = pos;

	vel += accel;
	pos += vel;

	return pos;
}

void Object::Shutdown() {
	if (animations.size() > 0) {
		int i, max = animations.size();
		for (i = 0; i < max; i++) {
			animations[i]->Shutdown();
			delete animations[i];
			animations[i] = NULL;
		}
		animations.clear();
	}
	currentAnimation = NULL;
}

Object::Object() {
	currentAnimation = NULL;
	flip_x = false; 
	mass = 1.0f;
	simulation = NULL;
}

Object::~Object() {}
