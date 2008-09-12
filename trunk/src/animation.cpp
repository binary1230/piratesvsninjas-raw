#include "stdafx.h"
#include "animation.h"
#include "gameState.h"
#include "sprite.h"
#include "window.h"
#include "assetManager.h"
#include "gameSound.h"
#include "effectsManager.h"
#include "object.h"

AnimFrame::AnimFrame() { Clear(); }
AnimFrame::~AnimFrame() { Clear(); }

void AnimFrame::Clear() 
{
	frame_type = ANIMFRAME_INVALID;
	sprite = NULL;
	duration = -1;
	freeze_at_end = false;
	nextFrame = NULL;
}

// ------------------------------------------- //

bool Animation::Init() 
{
	frames.clear();
	currentFrame = NULL;
	elapsed_time = 0;
	freeze_animation = false;
	speed_multiplier = 1;
	attachedObject = NULL;
	return true;
}

// Draw current frame at specified position, flipping if requested
void Animation::DrawAt(int x, int y, bool flip_x, bool flip_y) 
{
	assert(currentFrame != NULL);
	WINDOW->DrawSprite(currentFrame->sprite, x, y);
}

//! Update the animation, advancing to the next frame
//! if enough time has passed
void Animation::Update() 
{
	bool need_to_switch_frames = false;

	// don't need to do anything if we have less than 2 frames or are frozen
	if (frames.size() < 2 || freeze_animation)
		return;

	++elapsed_time;

	if (elapsed_time >= currentFrame->duration * speed_multiplier)
		need_to_switch_frames = true;
					
	if (need_to_switch_frames) {
		elapsed_time = 0;
		SwitchToNextFrame();
		
		if (currentFrame->freeze_at_end) {
			// set the new elapsed time to be the length of this frame
			// so that when we unfreeze the animation it goes right to
			// the next frame
			elapsed_time = currentFrame->duration * speed_multiplier;
			freeze_animation = true;
		}
	}
}

// Switch this animation to its next frame
// (maybe this is too complicated.)
void Animation::SwitchToNextFrame() 
{
	// special cases
	// if the frame is an EFFECT frame or a SOUND frame, then
	// trigger that EFFECT or SOUND, and then immediately advance
	// to the next frame in the animation

	CString soundName, effectName;
	Object* obj;
	AnimFrame* oldFrame = currentFrame;

	// NOTE: if no currentFrame->nextFrame, NEVER advance it until reset
	if (currentFrame->nextFrame)
		currentFrame = currentFrame->nextFrame;

	switch (oldFrame->frame_type) {
		case ANIMFRAME_SPRITE:
			if (currentFrame->frame_type != ANIMFRAME_SPRITE)
				SwitchToNextFrame();
			break;

		case ANIMFRAME_SOUND:
			soundName = oldFrame->extraData;

			if (soundName.size() == 0)
				TRACE(	"ERROR: No sound name specified "
													"in animation sound frame\n");

			SOUND->PlaySound(soundName);
			SwitchToNextFrame();
			break;

		case ANIMFRAME_EFFECT:
			effectName = oldFrame->extraData;

			if (effectName.size() == 0)
				TRACE(	"ERROR: No sound name specified "
													"in animation sound frame\n");

			obj = EFFECTS->TriggerEffect( attachedObject, effectName );

			SwitchToNextFrame();
			break;

		case ANIMFRAME_JUMP:

			// do NOTHING!
			SwitchToNextFrame();
			break;

		case ANIMFRAME_DESTROY:

			if (attachedObject)
				attachedObject->SetIsDead(true);

			break;

		// not nice for now.
		default: case ANIMFRAME_INVALID:
			assert(0 && "ERROR: Got an invalid frame type!");
			break;
	}

	// the next frame MUST BE a sprite frame.
	assert(currentFrame->frame_type == ANIMFRAME_SPRITE);

	if (currentFrame == frames[0] && attachedObject)
		attachedObject->OnAnimationLooped();
}

//! Reset this animation back to the first frame
void Animation::ResetAnimation() 
{
	freeze_animation = false;
	if (frames.size())
		currentFrame = frames[0];
	else
		currentFrame = NULL;
}

//! Free memory associated with this animation
void Animation::Shutdown() 
{
	int i, max = frames.size();

	// DO NOT FREE SPRITES

	for (i = 0; i < max; i++) {
		if (frames[i]) {
			delete frames[i];
			frames[i] = NULL;
		}
	}
}

//! Add a sprite frame to this animation
bool Animation::CreateSpriteFrame(	const char* _file, 
									const int duration, 
									bool freeze_at_end, 
									bool use_alpha) 
{				
	AnimFrame *f = new AnimFrame();
	assert(f != NULL);
	assert(_file != NULL);

	f->sprite = ASSETMANAGER->LoadSprite(_file, use_alpha);

	if (!f->sprite) {
		TRACE("ERROR: Can't load image file: '%s'\n", _file);
		return false;
	}

	f->frame_type = ANIMFRAME_SPRITE;
	f->sprite->use_alpha = use_alpha;
	f->duration = duration;
	f->freeze_at_end = freeze_at_end;

	return PushFrame(f);
}

bool Animation::CreateEffectFrame(	const CString &effectData, 
									const bool freeze_at_end ) 
{	

	AnimFrame *f = new AnimFrame();
	assert(f != NULL);
	assert(effectData.size() > 0);

	f->frame_type = ANIMFRAME_EFFECT;
	f->duration = 0;
	f->extraData = effectData;

	return PushFrame(f);
}

bool Animation::CreateSoundFrame(	const CString &soundData, 
									const bool freeze_at_end	) 
{	
	AnimFrame *f = new AnimFrame();
	assert(f != NULL);
	assert(soundData.size() > 0);

	f->frame_type = ANIMFRAME_SOUND;
	f->duration = 0;
	f->extraData = soundData;

	return PushFrame(f);
}

bool Animation::CreateDestroyFrame()
{
	AnimFrame* f = new AnimFrame();
	assert(f != NULL);

	f->frame_type = ANIMFRAME_DESTROY;
	f->duration = 0;
	f->extraData = "";

	return PushFrame(f);
}

//! Add a JUMP frame to this animation
//! Think of this like a goto statement -> the next frame will loop back on a previous frame
//! iFrameToJumpTo is an INDEX starting with ZERO.  The XML is a frame NUMBER that starts at ONE
bool Animation::CreateJumpFrame( int iFrameToJumpTo ) 
{				
	AnimFrame *f = new AnimFrame();
	assert(f != NULL);

	f->sprite = NULL;
	f->frame_type = ANIMFRAME_JUMP;
	f->duration = 0;

	if (!PushFrame(f))
		return false;

	// we are now the last frame of the sequence
	int me = frames.size() - 1;
	
	if (iFrameToJumpTo < 0 || iFrameToJumpTo >= (int)frames.size())
	{
		TRACE("ERROR: A jump frame wants to jump to jump to an illegal frame number: %i", iFrameToJumpTo);
		return false;
	}
	else if (iFrameToJumpTo == me || frames[iFrameToJumpTo] == f)
	{
		TRACE("ERROR: A jump frame tried to jump back to itself -> infinite loop");
		return false;
	}
	else if (frames[iFrameToJumpTo]->frame_type == ANIMFRAME_JUMP)
	{
		TRACE("ERROR: A jump frame wants to jump to ANOTHER jump frame (disallowed for now), frame number: %i", iFrameToJumpTo);
		return false;
	}

	// now that we're all set, let's switch the pointers up
	f->nextFrame = frames[iFrameToJumpTo];
	return true;
}

bool Animation::PushFrame(AnimFrame* f) 
{
	frames.push_back(f);

	f->nextFrame = frames[0];

	// frames->nextFrame links to the frame to play after this one.
	// ensure that the last frame always links to the first one
	if (frames.size() <= 1) {
		currentFrame = frames[0];
 	} else {
		int lastFrame = frames.size() - 1;
		frames[lastFrame - 1]->nextFrame = f;
	}

	return true;
}

//! Static helper method to create new animations from XML
//!
//! Does not currently support frames out of order and
//! other wackiness.  Soon enough, my young apprentice.
//!
Animation* Animation::Load(XMLNode &xAnim, Object* attachedObject) 
{
	int duration;
	CString sprite_filename, frame_type, extraData;
	int freeze_at_end;
	XMLNode xFrames, xFrame;
	int i, iterator, numFrames, numSpriteFrames = 0;
	bool use_alpha = false;			// whether we use the sprite's alpha channel
	int first_sprite_frame = -1; // which frame is the first sprite frame
	
	Animation* anim = new Animation();
	
	if (!anim || !anim->Init() )
		return NULL;

	anim->attachedObject = attachedObject;
	
	xFrames = xAnim.getChildNode("frames");
	numFrames = xFrames.nChildNode("frame");

	assert(numFrames != 0 && "ERROR: No <frame> tags found!\n");

	// loop through each <frame> tag
	for (i=iterator=0; i<numFrames; i++) {

		xFrame = xFrames.getChildNode("frame", &iterator);
			
		if (CString("true") == CString(xFrame.getAttribute("alpha_enabled"))) {
			use_alpha = true;
		}
	
		const char* freeze = xFrame.getAttribute("pause");
		if (!freeze)
			freeze_at_end = 0;
		else
			sscanf(freeze, "%i", &freeze_at_end);

		// if this frame has a "type" attribute, use it, if not assume it's a
		// of type "sprite" (as opossed to "sound" and "effect" types)
		frame_type = xFrame.getAttribute("type");
		
		if (frame_type.size() == 0)
			frame_type = "sprite";

		// figure out what type of frame this is and do The Right Thing
		if (frame_type == "sprite") 
		{
			// normally what happens - you get an image filename
			sprite_filename = xFrame.getAttribute("name");

			if (!xFrame.getAttributeInt("duration", duration)) {
				anim->Shutdown();
				SAFE_DELETE(anim);
				return NULL;
			}
	
			if (!anim->CreateSpriteFrame(	sprite_filename, 
											duration, 
											freeze_at_end != 0, 
											use_alpha)) {
				anim->Shutdown();
				SAFE_DELETE(anim);
				return NULL;
			}

			if (first_sprite_frame == -1)
				first_sprite_frame = i;

			++numSpriteFrames;

		} 
		else if (frame_type == "effect")
		{
			// effect frames don't display anything but instead trigger effects 
			// such as smoke/dust/etc
			extraData = xFrame.getAttribute("data");
			assert(extraData.size() != 0);

			if (!anim->CreateEffectFrame(extraData, freeze_at_end != 0)) {
				anim->Shutdown();
				SAFE_DELETE(anim);
				return NULL;
			}
		} 
		else if (frame_type == "sound") 
		{
			// sound frames don't display anything but instead trigger sounds 
			extraData = xFrame.getAttribute("data");
			assert(extraData.size() != 0);

			if (!anim->CreateSoundFrame(extraData, freeze_at_end != 0)) {
				anim->Shutdown();
				SAFE_DELETE(anim);
				return NULL;
			}
		}
		else if (frame_type == "destroy") 
		{
			if (!anim->CreateDestroyFrame()) {
				anim->Shutdown();
				SAFE_DELETE(anim);
				return NULL;
			}
		} 
		else if (frame_type == "jumpToFrame")
		{
			bool bFailed = false;
			int iFrameIndexToJumpTo;

			bFailed = !xFrame.getAttributeInt("num", iFrameIndexToJumpTo);
			iFrameIndexToJumpTo--; // subtract one since this is an INDEX not a FRAME NUMBER
			bFailed = bFailed || !anim->CreateJumpFrame(iFrameIndexToJumpTo);

			if (bFailed)
			{
				anim->Shutdown();
				SAFE_DELETE(anim);
				return NULL;
			}
		}
		else 
		{

			TRACE("ERROR: Invalid frame type specified: '%s'\n", 
							frame_type.c_str());
			anim->Shutdown();
			SAFE_DELETE(anim);
			return NULL;
		}
	}

	// hack.
	assert(numSpriteFrames >= 1 && 	"ERROR: Animations must have at least 1\n"
									"       sprite frame or else it will hang\n"
									"       when played back!" );

	// TODO: Currently this just gets the width and height from the
	// first frame.  However, we may want to get it from any
	// frame, not just the first one.  In the future, we should make
	// that an option in XML
	assert(first_sprite_frame != -1);

	anim->width = anim->frames[first_sprite_frame]->sprite->width;
	anim->height = anim->frames[first_sprite_frame]->sprite->height;

	if (anim->currentFrame->freeze_at_end)	
		anim->freeze_animation = true;
	
	return anim;
}

Animation::Animation()
{
	currentFrame = NULL;
}

Animation::~Animation() {}
