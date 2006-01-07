#ifndef ANIMATION_H
#define ANIMATION_H

#include <allegro.h>
#include <stdio.h>
#include <vector>
#include <map>
using namespace std;

class Sprite;

#include "gameBase.h"
#include "xmlParser.h"
#include "StdString.h"

// Maps an string to an integer ID (e.g. "bad_guy_1" to BAD_GUY_1)
// Used to keep track of stuff while parsing XML
typedef map<const CString, uint> AnimationMapping;

//! An animation frame.  Each animation is an array of these.
struct AnimFrame {
	Sprite* sprite;				//! Sprite data
	
	int duration;					//! Number of frames to show before advancing to next
	AnimFrame* nextFrame;	//! Pointer to next frame, or NULL if end of animation
};

//! The animation class
//! Holds sprites and displays them in a preset order and timing
//! 
//! This class holds a specific series of images that an object can use.
//! Typically, objects have a few different 'animations', for example,
//! a player object might have 2 animations - a walking and jumping animation
//! EACH of these would be seperate instances of the animation class.
class Animation : public GameBase {
	protected:
		vector<struct AnimFrame*> frames;	//! Collection of frames in this animation
		AnimFrame* currentFrame;	//! Points to the current frame we are drawing

		int elapsed_time;					//! The elapsed amount of game frames that 
															//! our current animation frame
															//! has been up 
	
		bool freeze_animation;		//! True if we do not advance the animation

		int speed_multiplier;			//! Factor to multiply the current animation 
															//! speed by. (e.g 2 = 2x as slow)
		
	public:
		void DrawAt(int _x, int _y, bool flip_x=0, bool flip_y=0);
		void Update();

		//! The speed multiplier can slow down the animation speed.
		//! Higher values result in a slower animation
		void SetSpeedMultiplier(int m) {speed_multiplier = m;};

		void ResetAnimation();		//! Set the animation back to the first frame

		bool Init(GameState*);		//! Initialize the animation
		void Shutdown();					//! Cleanup this animation

		//! Used in constructing a new animation, pushes this image onto it.
		bool PushImage(const char* filename, const int duration);

		int GetWidth();
		int GetHeight();
		
		inline Sprite* GetCurrentSprite() {return currentFrame->sprite;}

		Animation();
		~Animation();

		//! Static factory method
		static Animation* New(GameState* gameState, XMLNode &xAnim);
};

#endif // ANIMATION_H
