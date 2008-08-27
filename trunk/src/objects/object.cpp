#include "stdafx.h"
#include "object.h"

#include "gameState.h"
#include "window.h"
#include "globals.h"
#include "animation.h"
#include "animations.h"
#include "gameWorld.h"
#include "gameOptions.h"
#include "gameSound.h"
#include "objectLayer.h"
#include "sprite.h"
#include "physics.h"

bool Object::debug_draw_bounding_boxes = 0;

void Object::SetObjectDefName(const char* _name) {
	SAFE_DELETE(objectDefName);
	objectDefName = new CString(_name);
}

// Used as criteria for STL find()
bool ObjectIsDead(Object* obj) {
	assert(obj != NULL);
	return obj->IsDead();
}

// Objects can call this if they use
// simple animations.
void Object::UpdateSimpleAnimations() {
	if (!currentAnimation)
		return;

	currentAnimation->Update();
	currentSprite = currentAnimation->GetCurrentSprite();
}

// Base update stuff used by all objects
void Object::BaseUpdate() {
	UpdateDisplayTime();
	UpdateFade();

	if (properties.uses_new_physics && m_pkPhysicsBody)
	{
		m_pkPhysicsBody->ApplyImpulse(b2Vec2(m_fImpulseToApplyX,m_fImpulseToApplyY), m_pkPhysicsBody->GetWorldCenter());
		m_fImpulseToApplyX = 0.0f;
		m_fImpulseToApplyY = 0.0f;

		pos.x = METERS_TO_PIXELS(m_pkPhysicsBody->GetPosition().x) - width / 2;
		pos.y = METERS_TO_PIXELS(m_pkPhysicsBody->GetPosition().y) - height / 2;
		use_rotation = true;
		rotate_angle = RAD_TO_DEG(-m_pkPhysicsBody->GetAngle());
	}

	if (use_rotation)
	{
		if (!properties.uses_new_physics)
			rotate_angle += rotate_velocity;
		// else
			//rotate_angle
	}
}

// Decrement the display time, when it reaches 0, we 
// destroy this object. If it's -1, then it lives forever
void Object::UpdateDisplayTime() {
	if (display_time == -1)
		return;

	if (display_time > 0)
		display_time--;

	if (display_time == 0)
		is_dead = true;
}

//! Cache some commonly used stuff
// ('cause the profiler says so!)
void Object::SetupCachedVariables() {
	assert(WORLD != NULL);
	level_width  = WORLD->GetWidth();
	level_height = WORLD->GetHeight();
	
	if (animations.size() > 0 && animations[0]) {
		width = animations[0]->GetWidth();
		height = animations[0]->GetHeight();
	} else {
		width = 0;
		height = 0;
	}

	m_bCanCollide |=properties.is_physical ||
					properties.is_player || 
					properties.is_spring ||
					properties.is_collectable || 
					properties.is_fan ||
					properties.is_door ||
					properties.is_ring ||
					properties.is_ball;
}

void Object::InitPhysics()
{
	if (!PHYSICS)
	{
		TRACE("ERROR: Physics subsystem not yet initialized! Can't init object physics!");
		assert(PHYSICS);
		return;
	}

	if (!properties.is_physical)
		return;

	if (properties.uses_new_physics)
	{
		if (properties.is_static)
			m_pkPhysicsBody = PHYSICS->CreateStaticPhysicsBox(pos.x, pos.y, width, height, properties.ignores_collisions);
		else
			m_pkPhysicsBody = PHYSICS->CreateDynamicPhysicsBox(pos.x, pos.y, width, height, properties.ignores_physics_rotation);

		m_pkPhysicsBody->SetUserData(this);
	}
}

void Object::UpdateFade() {
	if (!is_fading)
		return;

	if (!fade_out_time_remaining) {
		is_fading = false;
	} else {
		--fade_out_time_remaining;
	}
	
	alpha = uint(	( (float)fade_out_time_remaining / (float)fade_out_time_total)
								* 255.0f);
}

void Object::FadeOut(int time) {
	fade_out_time_total = fade_out_time_remaining = time;
	alpha = 255;
	is_fading = true;
}

bool Object::BaseInit() {
	m_bDrawBoundingBox = false;
	tmp_debug_flag = 0;
	ClearProperties(properties);
	is_dead = false;
	fade_out_time_total = fade_out_time_remaining = 0;
	is_fading = false;
	alpha = 255;
	display_time = -1;
	width = height = 0;
	controller_num = 0;
	level_width = 0;
	level_height = 0;
	rotate_angle = rotate_velocity = 0.0f;
	use_rotation = false;
	m_fImpulseToApplyX = 0.0f;
	m_fImpulseToApplyY = 0.0f;
	return true;
}

void Object::Draw() {
	assert(WORLD != NULL);

	if (tmp_debug_flag)
		TRACE("DEBUG FLAG!!\n");

	DrawAtOffset(0,0);
}

//! Ultimately we need the actual, on-screen coordinates of where
//! to draw the sprite.  To get to those from the object's "world" coordinates
//! as computed by the physics engine, we need to take into account the 
//! position of the camera, and we need to flip the Y axis.  These
//! things are handled by the WORLD->TransformXXX() methods.
//
//! This function populates x,y (reference params) with the 
//! correctly transformed coordinates.
void Object::Transform(int &x, int &y, const int &offset_x, const int &offset_y) {
	x = (int)pos.x + offset_x;
	y = (int)pos.y + offset_y;

	// take into account the camera now.
	if (!properties.is_overlay)
		WORLD->TransformWorldToView(x, y);
	
	// compute absolute x,y coordinates on the screen
	y = y + GetHeight();
	WORLD->TransformViewToScreen(x, y);
}

// Same as Transform(), just for rectangles only.
void Object::TransformRect(_Rect &r) {

	int x1, x2, y1, y2, w, h;
	
	// r.Fix();
	
	x1 = (int)r.getx1();	
	y1 = (int)r.gety1();	
	x2 = (int)r.getx2();
	y2 = (int)r.gety2();
	w = x2 - x1;
	h = y2 - y1;

	// take into account the camera now.
	if (!properties.is_overlay) {
		WORLD->TransformWorldToView(x1, y1);
		WORLD->TransformWorldToView(x2, y2);
	}
	
	// compute absolute x,y coordinates on the screen
	WORLD->TransformViewToScreen(x1, y1);
	WORLD->TransformViewToScreen(x2, y2);

	r.set(x1,y1,x2,y2);
	/*r.setx1(x1); 	r.sety1(y1);
	r.setx2(x2); 	r.sety2(y2);*/
	
	// r.setx2(x1 + w); 	r.sety2(y1 + h);
}

//! This function does the real dirty work of drawing.
void Object::DrawAtOffset(int offset_x, int offset_y, Sprite* sprite_to_draw) 
{	
	int x, y;
	Transform(x, y, offset_x, offset_y);
				
	// draw for real
	if (!sprite_to_draw)
		sprite_to_draw = currentSprite;

	if (sprite_to_draw)
		WINDOW->DrawSprite(sprite_to_draw, x, y, flip_x, flip_y, use_rotation, rotate_angle, alpha);

	// bounding box stuff below.

	/*if (m_bDrawBoundingBox) 
	{
		_Rect bbox_t;

		// get current bounding box
		bbox_t.set(	pos.x, pos.y, pos.x + width, pos.y + height );

		// draw current bounding rectangle, pink
		TransformRect(bbox_t);
		WINDOW->DrawRect(bbox_t, makecol(255,0,255));
	}*/
}

void Object::ResetForNextFrame() 
{
	m_kCurrentCollision.up = m_kCurrentCollision.down = m_kCurrentCollision.left = m_kCurrentCollision.right = 0;

	if (m_pkPhysicsBody)
	{
		const b2Vec2& kPos = m_pkPhysicsBody->GetPosition();
		pos.x = kPos.x;
		pos.y = kPos.y;
	}
}

void Object::BaseShutdown() {
	m_pkLayer->RemoveObject(this);

	int i, max = animations.size();
	for (i = 0; i < max; i++) {
		if (animations[i])
			animations[i]->Shutdown();
		SAFE_DELETE(animations[i]);
	}
	animations.clear();
	
	currentAnimation = NULL;
	currentSprite = NULL;
	m_pkLayer = NULL;
	is_dead = true;
	display_time = -1;

	if (PHYSICS && m_pkPhysicsBody)
		PHYSICS->RemoveFromWorld(m_pkPhysicsBody);

	SAFE_DELETE(objectDefName);
}

unsigned long Object::debug_object_id = 0;

Object::Object() {
	objectDefName = NULL;
	unique_id = Object::debug_object_id++;
	m_pkLayer = NULL;
	currentSprite = NULL;
	currentAnimation = NULL;
	flip_x = false; 
	flip_y = false; 
	is_dead = true;
	debug_flag = false;
	pos.x = pos.y = 0.0f;
	display_time = -1;
	rotate_angle = rotate_velocity = 0.0f;
	use_rotation = false;
	m_bDrawBoundingBox = false;
	m_bCanCollide = false;
	m_pkPhysicsBody = NULL;
}

void Object::OnCollide( Object* obj, const b2ContactPoint* pkContactPoint )
{
	// default is no action, this is overidden in higher classes
}

Object::~Object() 
{
	SAFE_DELETE(objectDefName);
}

void Object::PlayAnimation( uint uiIndex )
{
	// assert(uiIndex >=0 && uiIndex < animations.size() && "Animation index out of range.");

	if (uiIndex >=0 && uiIndex < animations.size())
		currentAnimation = animations[uiIndex];
	else
		TRACE("ERROR: Animation #%i not found! Can't switch to it.\n", uiIndex);
}

void Object::SetImpulse( float x, float y )
{
	if (!properties.uses_new_physics)
		return;

	m_fImpulseToApplyX = x;
	m_fImpulseToApplyY = y;
}