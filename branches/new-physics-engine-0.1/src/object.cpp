#include "object.h"

// Putting this here is a nasty hack to avoid weird dependency loops
#include "gameState.h"

int Object::GetX() {
	return (int)pos.GetX();
}

int Object::GetY() {
	return (int)pos.GetY();
}

void Object::SetX(int _x) {
	pos.SetX((float)_x);
}

void Object::SetY(int _y) {
	pos.SetY((float)_y);
}

void Object::SetXY(int _x, int _y) {
	pos.SetX((float)_x);
	pos.SetY((float)_y);
}

int Object::GetWidth() {
	return bitmap->w;
}

int Object::GetHeight() {
	return bitmap->h;
}

void Object::Draw() {
	DrawAtOffset(0,0);
}

void Object::DrawAtOffset(int _x, int _y) {	
	draw_sprite(game_state->GetDrawingSurface(), bitmap, GetX() + _x, GetY() + _y);
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

Object::Object() : 
bitmap(NULL), bitmap_is_deleteable(false)  {}
Object::~Object() {}
