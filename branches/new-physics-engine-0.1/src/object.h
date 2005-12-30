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

#include <allegro.h>
#include <stdio.h>
#include <vector>
using namespace std;

typedef vector<Object*> ObjectList;

#include "gameBase.h"
#include "vector2D.h"
#include "xmlParser.h"

//! A bit mask of various properties of an object
struct ObjectProperties {
	unsigned feels_gravity : 1;	
	unsigned feels_user_input : 1;
	unsigned feels_friction : 1;
};

//! An in-game object, can be affected by physical forces
class Object : public GameBase {
	protected:

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

		//! Whether to flip the sprite when drawing
		bool flip_x;
		bool flip_y;

		//! Object's Physical Mass
		float mass;

		//! Solve for the new position of this object
		Vector2D Solve();
		
	public:
		virtual bool Init(GameState* _game_state) = 0;
		virtual void Shutdown();
		
		virtual void Update() = 0;
		virtual void Draw();
		
		void DrawAtOffset(int x, int y);	// Draw this object at its coordinates
																			// plus this offset.
		
		// void SetBitmap(BITMAP* _bitmap);
		
		// void SetBitmapIsDeleteable(bool _is_deletable);
		// bool GetBitmapIsDeleteable();
		
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
		bool LoadAnimations(XMLNode &xDef);
		
		Object();
		virtual ~Object();
};

#endif // OBJECT_H
