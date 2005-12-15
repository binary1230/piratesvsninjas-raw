#include "object.h"
#include "gameState.h"
#include "force.h"
#include "globals.h"
#include "gameBase.h"
#include "animation.h"

int Object::GetWidth() { return currentAnimation->Width(); }
int Object::GetHeight() {	return currentAnimation->Height(); }

void Object::Draw() {
	DrawAtOffset(0,0);
}

void Object::DrawAtOffset(int x, int y) {
	if (currentAnimation)
		currentAnimation->DrawAt(
										(int)pos.GetX() + x, 
										game_state->Height() - (int)pos.GetY() + y, 
										flip_x);
}

/*void Object::SetBitmap(BITMAP* _bitmap) {
	bitmap = _bitmap;
}
				
void Object::SetBitmapIsDeleteable(bool _bitmap_is_deleteable) {
	bitmap_is_deleteable = _bitmap_is_deleteable;
}

bool Object::GetBitmapIsDeleteable() {
	return bitmap_is_deleteable;
}*/

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
	Vector2D newpos = pos;

	vel += accel;
	newpos += vel;

	return newpos;
}

void Object::Shutdown() {
}

Object::Object() {
	currentAnimation = NULL;
	flip_x = false; 
	mass = 1.0f;
}

Object::~Object() {}
