#include "animation.h"
#include "gameState.h"

// XXX factory method memory leak
// XXX memory leak if Init fails, anim never gets freed
// EVERYTHING allocated like this (and objects) need to be fixed

Animation::Animation() {}
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

	if (!currentFrame) {
	  fprintf(stderr, "NO CURRENT FRAME!\n");
		return;
	}
				
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
bool Animation::PushImage(char* file, int duration) {
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

// XXX buggy-ass parsing code.  NEEEEED XML.
/*Animation* Animation::New(GameState* gameState, char* file, int id) {
	
	Animation* anim = new Animation();
	int lineno = -1;
	char buf[1024];
	bool not_eof;
	
	if (!anim) goto end;
	if (!anim->Init(gameState)) goto error_init;
	
	FILE* f = fopen(file, "r");
	if (!f) goto error_file;

	// read each from the file until we get to the line we want or EOF.
	while (fgets(buf, 1024, f) && lineno != id) {
		lineno++;
	}

	if (lineno != id) goto not_found;

	
						
	// sucess
	close(f);
	return anim;

	not_found:
		fprintf(stderr, "requested animation ID %i not found!\n", id);

	// failures
	error_file:
		close(f);
	
	error_init:
		free anim;
	
	end:
		return NULL;
}*/
