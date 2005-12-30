#include "animation.h"
#include "gameState.h"

#include <map>
using namespace std;

// XXX factory method memory leak
// XXX memory leak if Init fails, anim never gets freed
// EVERYTHING allocated like this (and objects) need to be fixed

Animation::Animation() {
	currentFrame = NULL;
}
Animation::~Animation() {}

//! Initialize the animation
bool Animation::Init(GameState* _gameState) {
	SetGameState(_gameState);
	frames.clear();
	currentFrame = NULL;
	elapsed_time = 0;
	freeze_animation = false;
	speed_multiplier = 1;
	return true;
}

// Draw current frame at specified position, flipping if requested
void Animation::DrawAt(int _x, int _y, bool flip_x) {
	//if (!GetGameState())
	// 	fprintf(stderr, "NO GAME STATE!\n");

	assert(currentFrame != NULL);
	assert(currentFrame->bmp != NULL);
				
	int x = _x + currentFrame->x_offset;
	int y = _y + currentFrame->y_offset;
	
	if (flip_x) 
		draw_sprite(game_state->GetDrawingSurface(), currentFrame->bmp, x, y);
	else
		draw_sprite_h_flip(game_state->GetDrawingSurface(), currentFrame->bmp, x, y);
}

//! Update the animation, advancing to the next frame
//! if enough time has passed
void Animation::Update() {

	// don't need to do anything if we have less than 2 frames or are frozen
	if (frames.size() < 2 || freeze_animation)
		return;

	elapsed_time++;

	// if it's time to advance to the next frame..
	if (elapsed_time >= currentFrame->duration * speed_multiplier) {
		elapsed_time = 0;
		
		// NOTE: if no currentFrame->nextFrame, NEVER advance until reset
		if (currentFrame->nextFrame)
			currentFrame = currentFrame->nextFrame;
	}
}

//! Reset this animation back to the first frame
void Animation::ResetAnimation() {
	freeze_animation = false;
	if (frames.size())
		currentFrame = frames[0];
	else
		currentFrame = NULL;
}

//! Free memory associated with this animation
void Animation::Shutdown() {
	int i, max = frames.size();
	for (i = 0; i < max; i++) 
		if (frames[i]->bitmap_is_deleteable)
			destroy_bitmap(frames[i]->bmp);
}

//! Kind of wanky, add another frame to this animation
//! Use for temporary loading routines, need to rethink this one.
//! Returns false on error
bool Animation::PushImage(const char* file, const int duration) {
	AnimFrame *f = new AnimFrame();

	f->bmp = load_bitmap(file, NULL);
	if (!f->bmp) {
		fprintf(stderr, "Can't load file: '%s' - not adding to animation\n", file);
		return false;
	}
	f->bitmap_is_deleteable = true;
	f->duration = duration;
	f->x_offset = 0;
	f->y_offset = 0;
	f->nextFrame = NULL;
	frames.push_back(f);

	f->nextFrame = frames[0];
	if (frames.size() > 1) {
		int last = frames.size() - 1;
		frames[last - 1]->nextFrame = f;
	} else {
		currentFrame = frames[0];
	}

	return true;
}

//! Static helper method to create new animations from XML
//! XXX Does not currently support frames out of order and
//! other wackiness.  soon enough, my young apprentice - soon enough.
Animation* Animation::New(GameState* gameState, XMLNode &xAnim) {
	Animation* anim = new Animation();
	int duration;
	CString filename;
	
	if (!anim || !anim->Init(gameState) )
		return NULL;

	XMLNode xFrames, xImg;
	int i, iterator, max;
	
	xFrames = xAnim.getChildNode("frames");
	max = xFrames.nChildNode("img");
		
	// fprintf(stderr, "--- max=%i\n", max);

	// loop through each <img> tag
	for (i=iterator=0; i<max; i++) {
					
		xImg = xFrames.getChildNode("img", &iterator);
		
		sscanf(xImg.getAttribute("duration"), "%i", &duration);
		filename = xImg.getAttribute("name");
		
		// fprintf(stderr, "--- f=%s, d=%i\n", filename.c_str(), duration);

		// Create the frame
		if (!anim->PushImage(filename, duration)) {
			anim->Shutdown();
			free(anim);
			return NULL;
		}
	}
	
	return anim;
}
