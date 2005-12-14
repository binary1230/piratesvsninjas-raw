#include "object.h"

// Putting this here is a nasty hack to avoid weird dependency loops
#include "gameState.h"
#include "force.h"

void Object::Draw() {
	DrawAtOffset(0,0);
}

void Object::DrawAtOffset(int _x, int _y) {	
	int x = int(pos.GetX()) + _x;
	int y = game_state->Height() - int(pos.GetY()) + _y;
	
	if (flip_x) 
		draw_sprite(game_state->GetDrawingSurface(), bitmap, x, y);
	else
		draw_sprite_h_flip(game_state->GetDrawingSurface(), bitmap, x, y);
}

void Object::Update() {
	// nothing to update for regular objects
}

void Object::SetBitmap(BITMAP* _bitmap) {
	bitmap = _bitmap;
}
				
void Object::SetBitmapIsDeleteable(bool _bitmap_is_deleteable) {
	bitmap_is_deleteable = _bitmap_is_deleteable;
}

bool Object::GetBitmapIsDeleteable() {
	return bitmap_is_deleteable;
}

void Object::Shutdown() {
	if (bitmap_is_deleteable) 
		destroy_bitmap(bitmap);

	bitmap = NULL;
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
	Vector2D newpos = pos;

	vel += accel;
	newpos += vel;

	return newpos;
}

Object::Object() : bitmap(NULL), bitmap_is_deleteable(false), 
									 flip_x(false), mass(1.0f)  {}
Object::~Object() {}
