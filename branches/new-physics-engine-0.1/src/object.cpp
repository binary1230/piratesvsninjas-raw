#include "object.h"
#include "gameState.h"
#include "force.h"
#include "globals.h"
#include "gameBase.h"
#include "animation.h"
#include "animations.h"
#include "physSimulation.h"
#include "xmlParser.h"

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
	assert(simulation != NULL);
	DrawAtOffset(-simulation->GetCameraLeft(), -simulation->GetCameraTop());
}

void Object::DrawAtOffset(int x, int y) {	

	// have to take into account Y coords are flipped on the screen
	int screen_y = game_state->ScreenHeight() - (int)pos.GetY() + y;
	int screen_x = (int)pos.GetX() + x;
	
	if (currentAnimation)
		currentAnimation->DrawAt(screen_x, screen_y, flip_x);
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

// A static helper function to load animations
bool Object::LoadAnimations(XMLNode &xDef) {
	int i, iterator, max;
	AnimationMapping animation_lookup = GetPlayerAnimationMappings();
	Animation* anim = NULL;
	CString anim_name;
	XMLNode xAnim, xAnims;
	
	animations.resize(PLAYER_MAX_ANIMATIONS);

	xAnims = xDef.getChildNode("animations");
	max = xAnims.nChildNode("animation");

	for (i=iterator=0; i<max; i++) {
		xAnim = xAnims.getChildNode("animation", &iterator);
		anim_name = xAnim.getAttribute("name");
		
		anim = Animation::New(GetGameState(), xAnim);

		if (!anim) {
			return false;
		} else {
			animations[animation_lookup[anim_name]] = anim;
			
			//fprintf(stderr, "--- animation '%s' is index #%i\n", anim_name.c_str(), 
			//								animation_lookup[anim_name] );
		}
	}

	// set the default animation XXX error check? how?
	CString default_name;
	int default_index; 

	default_name = xAnims.getAttribute("default");
	default_index = animation_lookup[default_name];
	currentAnimation = animations[default_index];

	//fprintf(stderr, "--- default_str = %s\n", xAnims.getAttribute("default"));
	// fprintf(stderr, "--- def = %i, current = %x\n", default_index, currentAnimation);
	// fprintf(stderr, " --- size = %i\n", animations.size());
	return true;
}

Object::~Object() {}
