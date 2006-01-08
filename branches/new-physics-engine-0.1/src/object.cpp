#include "object.h"
#include "gameState.h"
#include "window.h"
#include "force.h"
#include "globals.h"
#include "gameBase.h"
#include "animation.h"
#include "animations.h"
#include "physSimulation.h"
#include "xmlParser.h"

int Object::GetWidth() { return currentAnimation->GetWidth(); }
int Object::GetHeight() {	return currentAnimation->GetHeight(); }

//! Cache some commonly used stuff
void Object::SetupCachedVariables() {
	simulation = GetGameState()->GetPhysSimulation();
	level_width  = simulation->GetWidth();
	level_height = simulation->GetHeight();
}

bool Object::BaseInit() {
	SetupCachedVariables();
	ClearProperties(properties);
	return true;
}

void Object::Draw() {
	assert(simulation != NULL);
	DrawAtOffset(0,0);
}

//! This function does the real dirty work of drawing.
// 
//! Ultimately we need the actual, on-screen coordinates of where
//! to draw the sprite.  To get to those from the object's "world" coordinates
//! as computed by the physics engine, we need to take into account the 
//! position of the camera, and we need to flip the Y axis.  These
//! things are handled by the simulation->TransformXXX() methods.
void Object::DrawAtOffset(int offset_x, int offset_y, Sprite* sprite_to_draw) {	
	int x = (int)pos.GetX() + offset_x;
	int y = (int)pos.GetY() + offset_y;

	if (!sprite_to_draw)
		sprite_to_draw = currentSprite;

	// take into account the camera now.
	if (!properties.is_overlay)
		simulation->TransformWorldToView(x, y);
	
	// compute absolute x,y coordinates on the screen
	simulation->TransformViewToScreen(x, y);

	// draw for real
	if (sprite_to_draw)
		GetGameState()->GetWindow()->
		DrawSprite(sprite_to_draw, x, y, flip_x, flip_y);
}

void Object::ApplyForce(Force* force) {
	// ignore certain types of forces
	if (	(!properties.feels_gravity && force->GetType() == FORCE_GRAVITY) ||
				(!properties.feels_user_input && force->GetType() == FORCE_INPUT1) ||
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
				
	int i, max = animations.size();
	for (i = 0; i < max; i++) {
		animations[i]->Shutdown();
		delete animations[i];
		animations[i] = NULL;
	}
	animations.clear();
	
	currentAnimation = NULL;
	currentSprite = NULL;
}

Object::Object() {
	currentSprite = NULL;
	currentAnimation = NULL;
	flip_x = false; 
	mass = 1.0f;
	simulation = NULL;
}

// A static helper function to load animations
bool Object::LoadAnimations(XMLNode &xDef, AnimationMapping *animation_lookup) {
	int i, iterator, max;

	Animation* anim = NULL;
	CString anim_name;
	XMLNode xAnim, xAnims;
	
	xAnims = xDef.getChildNode("animations");
	max = xAnims.nChildNode("animation");
	
	animations.resize(max);

	for (i=iterator=0; i<max; i++) {
		xAnim = xAnims.getChildNode("animation", &iterator);
		anim_name = xAnim.getAttribute("name");
		
		anim = Animation::New(GetGameState(), xAnim);

		if (!anim) {
			return false;
		} else {
			// if we have animation names (e.g. "walking") then use them to figure
			// out which index we store this animation at
			// if not, just put it in the next available index
			int index;
			
			if (animation_lookup)
				index = (*animation_lookup)[anim_name];
			else 
				index = i;
			
			animations[index] = anim;
		}
	}

	// set the default animation XXX error check? how?
	CString default_name;
	int default_index; 

	if (!animation_lookup) {
		currentAnimation = animations[0];
	} else { 
		default_name = xAnims.getAttribute("default");
		default_index = (*animation_lookup)[default_name];
		currentAnimation = animations[default_index];
	}

	// set the current sprite to the first frame of the animation
	currentSprite = currentAnimation->GetCurrentSprite();

	return true;
}

Object::~Object() {}
