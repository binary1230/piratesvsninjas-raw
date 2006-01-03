#ifndef ANIMATION_H
#define ANIMATION_H

#include <allegro.h>
#include <stdio.h>
#include <vector>
#include <map>
using namespace std;

#include "gameBase.h"
#include "xmlParser.h"
#include "StdString.h"

// Maps an string to an integer ID (e.g. "bad_guy_1" to BAD_GUY_1)
// Used to keep track of stuff while parsing XML
typedef map<const CString, uint> AnimationMapping;

//! An animation frame.  Each animation is an array of these.
struct AnimFrame {
	BITMAP* bmp;								//! Bitmap data for this frame
	bool bitmap_is_deleteable;	//! If the bitmap data is deleteable
	
	int duration;					//! Number of frames to show before advancing to next
	AnimFrame* nextFrame;	//! Pointer to next frame, or NULL if end of animation

	int x_offset;					//! Offset to draw this frame in the X direction
	int y_offset;					//! Offset to draw this frame in the Y direction
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
		void DrawAt(int _x, int _y, bool flip_x = false);	
		void Update();

		//! The speed multiplier can slow down the animation speed.
		//! Higher values result in a slower animation
		void SetSpeedMultiplier(int m) {speed_multiplier = m;};

		void ResetAnimation();		//! Set the animation back to the first frame

		bool Init(GameState*);		//! Initialize the animation
		void Shutdown();					//! Cleanup this animation

		//! Used in constructing a new animation, pushes this image onto it.
		bool PushImage(const char* filename, const int duration);

		int Width() {return frames[0]->bmp->w;};
		int Height() {return frames[0]->bmp->h;};
		
		Animation();
		~Animation();

		//! Static factory method
		static Animation* New(GameState* gameState, XMLNode &xAnim);
};

#endif // ANIMATION_H
