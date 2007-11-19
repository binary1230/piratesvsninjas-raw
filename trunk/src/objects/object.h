#ifndef OBJECT_H
#define OBJECT_H

#include "stdafx.h"
#include "globals.h"

#include "rect.h"
#include "vector2D.h"
#include "animations.h"

class Object;
class Force;
class Animation;
class Sprite;
class ObjectLayer;
class ObjectFactory;

struct CollisionDirection {
	unsigned up : 1;
	unsigned down : 1;
	unsigned left : 1;
	unsigned right : 1;
};

//! Various properties of an Object
struct ObjectProperties {
	
	// NOTE: If you add anything here, update ClearProperties()
	bool feels_gravity;	
	bool feels_user_input;
	bool feels_friction;

	//! If solid, another solid object cannot move through it
	bool is_solid;

	//! true if this object is an overlay
	//! e.g. not IN the world, but on top it,
	//! like our status bar or health or something.
	//! Overlays ignore camera information
	bool is_overlay;

	//! Object "types" (really should use RTTI or something.)
	bool is_player;
	bool is_spring;
	bool is_collectable;
	bool is_fan;
	bool is_door;
	bool is_ring;
	bool is_ball;
};

//! Clears property masks
inline void ClearProperties(struct ObjectProperties& p) {
	p.feels_gravity = 0;
	p.feels_user_input = 0;
	p.feels_friction = 0;
	p.is_overlay = 0;
	p.is_solid = 0;
	p.is_player = 0;
	p.is_spring = 0;
	p.is_collectable = 0;
	p.is_fan = 0;
	p.is_door = 0;
	p.is_ring = 0;
	p.is_ball = 0;
}

// Used for find()
bool ObjectIsDead(Object* obj);

//! A drawable entity in the world

//! Objects have physical properties associated with them, but do
//! not always have to take part in the world
class Object {
	protected:

		//! A pointer to the layer this object is on
		ObjectLayer* layer;

		//! Which controller (e.g. which joystick) use, if we are getting
		//! input for this object
		int controller_num;
		
		//! Bounding box for this object
		_Rect bbox;

		//! Projection rectange (for collisions)
		_Rect projRect;

		//! CACHED level width and height
		int level_width, level_height;
		
		//! Current position
		Vector2D pos;

		//! Old position (last frame)
		Vector2D old_pos;

		//! Current velocity 
		Vector2D vel;

		//! Current force
		Vector2D accel;
		
		//! The directions of current collisions (up,down,right,left)
		CollisionDirection d;
		
		//! Object properties
		struct ObjectProperties properties;
		
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

		bool BaseInit();	
		void BaseShutdown();

		//! True if this object is no longer in play and needs to be cleaned up
		bool is_dead;

		//! If true, this object can print debug info out if it wants to.
		bool debug_flag;

		//! How long, in frames, do we display this object.
		//! This value is decremented each frame.  When it reaches
		//! 0, the object is deleted.
		//! When set to -1, this value is ignored
		int display_time;

		// XML Props, rarely ever used.
		CString* objectDefName;

		// -- UNUSUED BELOW --

		//! Current fade-out time
		int fade_out_time_total;
		int fade_out_time_remaining;

		//! True if the object is fading in or out
		bool is_fading;

		//! Alpha (transparency) of this object (0=inviz, 255=opaque)
		int alpha;

		// -- END UNUSED --

		//! Do common object updates
		void BaseUpdate();

		//! Update display times
		void UpdateDisplayTime();

		//! (optional) Do a simple update of the animations
		//! Most objects will call this
		void UpdateSimpleAnimations();

		//! Update the fading stuff
		void UpdateFade();

		//! Width and Height of the object
		// (we may need to rethink where these come from)
		int width, height;

		//! Rotational parameters
		float rotate_angle, rotate_velocity;
		bool use_rotation;

		//! Whether to draw the bounding box or not
		bool draw_bounding_box;

		//! If this object should report collisions or not
		bool m_bCanCollide;
		
		// Protected constructor, this means we can't directly
		// instantiate Object's, we need to use a friend or derived class.
		Object();
	
	public:
		int tmp_debug_flag;

		// Whether to draw ALL the different rectangles or not (DEBUG)
		static bool debug_draw_bounding_boxes;
		
		// DEBUG ONLY: A unqiue ID that is incremented every time an object
		// is created.  The amount of created objects should match the amount
		// of free'd objects.
		static unsigned long debug_object_id;
		unsigned long unique_id;

		virtual bool Init() = 0;
		virtual void Shutdown() = 0;
		
		virtual void Update() = 0;

		void DrawBoundingRect(bool state) {draw_bounding_box = true;}
		
		//! Fade this object out over a given time (in frames)
		void FadeOut(int time);

		//! Move this object to its new position (done before 
		//! collision detection)
		void MoveToNewPosition();

		virtual void Draw();

		void Transform(	int &x, int &y, 
										const int &offset_x = 0, const int &offset_y = 0);

		void TransformRect(_Rect &r);

		inline void SetDisplayTime(int time) {
			display_time = time;
		}
		inline int GetDisplayTime() {
			return display_time;
		}
	
		//! Draw this object at its coordinates plus specified offset
		//! Optionally, you can pass in a specific sprite to draw, otherwise
		//! It'll just use the current sprite (most cases)
		void DrawAtOffset(int x, int y, Sprite* = NULL);	
		
		//! Functions to get/set position
		inline int GetX() const				{ return (int)pos.x; }
		inline int GetY() const				{ return (int)pos.y; }
		inline Vector2D GetXY() const { return pos; }; 

		inline void SetX(const int _x) 		{ pos.x = _x; }
		inline void SetY(const int _y) 		{ pos.y = _y; }
		inline void SetXY(const int _x, const int _y) {
				pos.x = _x;
				pos.y = _y;
		}
		inline void SetXY(const Vector2D &_pos) {
			pos = _pos;
		}

		inline int GetAlpha() { return alpha; };
		inline void SetAlpha(const int a) { alpha = a; };

		inline void SetFlipX(const bool val) { flip_x = val; };
		inline void SetFlipY(const bool val) { flip_y = val; };

		//! Functions to get/set velocity
		inline float GetVelX() 					{ return vel.x; }
		inline float GetVelY() 					{ return vel.y; }
		inline void SetVelX(const float _vx) 		{ vel.x = _vx; }
		inline void SetVelY(const float _vy) 		{ vel.y = _vy; }
		inline void SetVelXY(const float _vx, const float _vy) {
				vel.x = _vx;
				vel.y = _vy;
		}
		inline void SetVelXY(const Vector2D &_vel) {
			vel = _vel;
		}
		inline void SetVelRotate(const float vel) {
			rotate_velocity = vel;
		}

		inline void SetUseRotation(const bool state) {
			use_rotation = state;
		}

		//! Get width/height of this object
		inline int GetWidth() const {return width;};
		inline int GetHeight() const {return height;};
	
		//! Physics: reset this object's physics stuff for next frame
		void ResetForNextFrame();

		//! Apply a force to this object
		void ApplyForce(Force* f);
		
		inline float GetMass() const		{ return mass; }
		inline void SetMass(float m) 		{ mass = m; }
		
		struct ObjectProperties GetProperties() const { return properties; };
		inline void SetProperties(struct ObjectProperties p) { properties = p;}

		//! Setup some commonly used variables
		void SetupCachedVariables();

		//! Set which controller we monitor
		void SetControllerNum(uint _c) {controller_num = _c;};
		
		//! Return which controller we monitor
		uint GetControllerNum() const {return controller_num;};
		
		void SetDebugFlag(bool d) {debug_flag = d;};
		bool GetDebugFlag() const {return debug_flag;};
		
		//! Handle collisions with another object
		virtual void Collide(Object* obj);

		bool IsColliding(Object *obj) const;

		inline bool IsDead() const {return is_dead;};
		inline void SetIsDead(bool bVal) {is_dead = bVal;}

		//! Returns a vector used for collision detection
		//! This vector will be have a position that is guaranteed
		//! to make the passed object NOT collide with THIS object.
		//! (based on velocity)
		CollisionDirection GetBound(Object* obj, Vector2D &v);

		void UpdateProjectionRectFromVelocity();
		void UpdateProjectionRectFromCollisions(Vector2D &newPos);

		const _Rect& GetProjectionRect() const {return projRect;}

		//! Plays a sound, or does nothing if that sound is not loaded
		// void PlaySound(CString name);

		ObjectLayer* const GetLayer() const {return layer;};
		void SetLayer(ObjectLayer* const l) {layer = l;};
		
		//! Returns true if this type of object is able to collide with another
		inline bool CanCollide() const {
			return m_bCanCollide;
		}

		void SetObjectDefName(const char*);
		
		virtual ~Object();

		friend class ObjectFactory;
		friend class MapSaver;
};

#endif // OBJECT_H
