// Object
// Base class for all objects displayable on the screen
// 
// TODO Replace BITMAP* sprite with animation class
#ifndef OBJECT_H
#define OBJECT_H

class Object;
class Force;

#include <allegro.h>
#include <stdio.h>
#include <vector>
using namespace std;

typedef vector<Object*> ObjectList;

#include "globals.h"
#include "vector2D.h"
#include "gameBase.h"
#include "objectFactory.h"
// #include "force.h"

//! A bit mask of various properties of an object
struct ObjectProperties {
	unsigned feels_gravity : 1;	
	unsigned feels_user_input : 1;
	unsigned feels_friction : 1;
};

//! An in-game object, can be affected by physical forces
class Object : public GameBase {
				
	protected:
		//! Current position
		Vector2D pos;

		//! Current velocity 
		Vector2D vel;

		//! Current force
		Vector2D accel;
		
		//! Object properties
		struct ObjectProperties properties;
		
		//! Bitmap to draw when this object is drawn
		BITMAP* bitmap;
		bool bitmap_is_deleteable;

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
		
		void SetBitmap(BITMAP* _bitmap);
		
		void SetBitmapIsDeleteable(bool _is_deletable);
		bool GetBitmapIsDeleteable();
		
		inline int GetX() 					{ return (int)pos.GetX(); }
		inline int GetY() 					{ return (int)pos.GetY(); }
		inline void SetX(int _x) 		{ pos.SetX((float)_x); }
		inline void SetY(int _y) 		{ pos.SetY((float)_y);	}
		
		inline void SetXY(int _x, int _y) {
				pos.SetX((float)_x);	
				pos.SetY((float)_y);
		}
		
		inline int GetWidth() { return bitmap->w; }
		inline int GetHeight() {	return bitmap->h; }
	
		void ResetForNextFrame();
		void ApplyForce(Force* f);
		
		inline float GetMass() 					{ return mass; }
		inline void SetMass(float m) 		{ mass = m; }
		
		inline struct ObjectProperties GetProperties() { return properties; }
		inline void SetProperties(struct ObjectProperties p) { properties = p;}
		
		Object();
		virtual ~Object();

		// for the class factory, allow it to access our private members
		friend Object* ObjectFactory::CreateObject(uint id);
};

#endif // OBJECT_H
