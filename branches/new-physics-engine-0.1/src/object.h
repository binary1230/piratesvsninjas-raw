// Object
// Base class for all objects displayable on the screen
// 
// TODO: Replace BITMAP* sprite with animation class
// TODO: Free all animations that are loaded.
#ifndef OBJECT_H
#define OBJECT_H

class Object;
class Force;
class Animation;
class PhysSimulation;
class Sprite;

#include <allegro.h>
#include <stdio.h>
#include <vector>
using namespace std;

typedef vector<Object*> ObjectList;

#include "globals.h"
#include "gameBase.h"
#include "vector2D.h"
#include "xmlParser.h"
#include "animations.h"

//! A bit mask of various properties of an object
//! NOTE: If you add anything here, update ClearProperties()
struct ObjectProperties {
	unsigned feels_gravity : 1;	
	unsigned feels_user_input : 1;
	unsigned feels_friction : 1;
	
	unsigned is_overlay : 1;				// is this object just an overlay?
																	// e.g. not IN the world, but on top it,
																	// like our status bar or health or something.
};

inline void ClearProperties(struct ObjectProperties& p) {
	p.feels_gravity = 0;
	p.feels_user_input = 0;
	p.feels_friction = 0;
	p.is_overlay = 0;
}

//! Sets properties to sane values
void ClearProperties(struct ObjectProperties&);

//! An in-game object, can be affected by physical forces
class Object : public GameBase {
	protected:

		//! Which controller (e.g. which joystick) use, if we are getting
		//! input for this object
		int controller_num;
		
		//! Width/height of this object
		int width, height;

		//! CACHED level width and height
		int level_width, level_height;
		
		//! CACHED simulation this object is a part of
		PhysSimulation* simulation;
					
		//! Current position
		Vector2D pos;

		//! Current velocity 
		Vector2D vel;

		//! Current force
		Vector2D accel;
		
		//! Object properties
		struct ObjectProperties properties;
		
		//! XXX OLD Bitmap to draw when this object is drawn
		// BITMAP* bitmap;
		// bool bitmap_is_deleteable;

		//! Points to the current animation
		Animation* currentAnimation;

		//! Array of all loaded animations
		vector<Animation*> animations;

		//! Points to the sprite we should draw on next Draw()
		Sprite* currentSprite;

		//! Whether to flip the sprite when drawing
		bool flip_x;
		bool flip_y;

		//! Object's Physical Mass
		float mass;

		//! Solve for the new position of this object
		Vector2D Solve();

		//! Base class initialization
		bool BaseInit();	
		
	public:
		virtual bool Init(GameState* _game_state) = 0;
		virtual void Shutdown();
		
		virtual void Update() = 0;
		virtual void Draw();
		
		//! Draw this object at its coordinates plus specified offset
		//! Optionally, you can pass in a specific sprite to draw, otherwise
		//! It'll just use the current sprite (most cases)
		void DrawAtOffset(int x, int y, Sprite* = NULL);	
		
		inline int GetX() 					{ return (int)pos.GetX(); }
		inline int GetY() 					{ return (int)pos.GetY(); }
		inline void SetX(int _x) 		{ pos.SetX((float)_x); }
		inline void SetY(int _y) 		{ pos.SetY((float)_y);	}
		
		inline void SetXY(int _x, int _y) {
				pos.SetX((float)_x);	
				pos.SetY((float)_y);
		}
		
		//! Get width/height of this object
		int GetWidth(); 
		int GetHeight(); 
	
		//! Physics: reset this object's physics stuff for next frame
		void ResetForNextFrame();

		//! Apply a force to this object
		void ApplyForce(Force* f);
		
		inline float GetMass() 					{ return mass; }
		inline void SetMass(float m) 		{ mass = m; }
		
		inline struct ObjectProperties GetProperties() { return properties; }
		inline void SetProperties(struct ObjectProperties p) { properties = p;}

		//! Setup some commonly used variables
		void SetupCachedVariables();

		//! Load animations from an XML object definition 
		bool LoadAnimations(XMLNode &xDef, AnimationMapping* = NULL);
		
		//! Set which controller we monitor
		void SetControllerNum(uint _c) {controller_num = _c;};
		
		//! Return which controller we monitor
		uint GetControllerNum() {return controller_num;};
		
		Object();
		virtual ~Object();
};

#endif // OBJECT_H
