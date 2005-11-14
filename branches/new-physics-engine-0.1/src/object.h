// Object
// Base class for all objects displayable on the screen
// 
// TODO Replace BITMAP* sprite with animation class
#ifndef OBJECT_H
#define OBJECT_H

class Object;

#include <allegro.h>
#include <stdio.h>
#include <vector>
using namespace std;

typedef vector<Object*> ObjectList;

#include "globals.h"
#include "gameBase.h"
#include "vector2D.h"
#include "force.h"

//! A bit mask of various properties of an object
struct ObjectProperties {
	unsigned ignores_gravity : 1;	
	unsigned ignores_user_input : 1;
};

//! An in-game object, can be affected by physical forces
class Object : public GameBase {
				
	protected:
		//! Current position
		Vector2D pos;

		//! Current velocity (not always used)
		Vector2D vel;

		//! Current force (not always used)
		Vector2D force;
		
		//! Object properties
		struct ObjectProperties properties;
		
		//! Bitmap to draw when this object is drawn
		BITMAP* bitmap;
		bool bitmap_is_deleteable;
		
	public:
		virtual bool Init(GameState* _game_state) = 0;
		virtual void Shutdown();
		
		virtual void Update() = 0;
		virtual void Draw() = 0;
		
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
		
		inline struct ObjectProperties GetProperties() {return properties;};
		inline void SetProperties(struct ObjectProperties p) {properties = p;};
		
		Object();
		virtual ~Object();
};

#endif // OBJECT_H
