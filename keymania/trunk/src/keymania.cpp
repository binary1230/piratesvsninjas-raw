#include "keymania.h"
#include "sprite.h"
#include "assetManager.h"
#include "gameSound.h"
#include "window.h"
#include "gameState.h"
#include "input.h"

DECLARE_SINGLETON(Keymania)

#define NUM_NOTES		4
#define NOTE_OFFSET 10
#define NOTE_SPEED 	10

bool Keymania::Init() {
	sBack = ASSETMANAGER->LoadSprite("back.png");
	sClef = ASSETMANAGER->LoadSprite("clef.png");
	sNote = ASSETMANAGER->LoadSprite("note.png", true);
	sStaff = ASSETMANAGER->LoadSprite("staff.png");

	// setup notes.
	notes.resize(NUM_NOTES);

	for (uint i = 0; i < notes.size(); i++) {
		notes[i].staff_pos = Rand(0, 8);
		notes[i].xpos = i * 200;
	}

	currentText = "";
	lastTextTime = 9999999;

	SOUND->LoadSound("ring.wav", "ring");

	if (sBack && sClef && sNote && sStaff)
		return true;
	else
		return false;
}

void Keymania::Shutdown() {
}

void Keymania::Draw() {
	WINDOW->DrawSprite(sBack, 0, 0);
	WINDOW->DrawSprite(sClef, 0, 150);
	
	// Draw the notes
	for (uint i = 0; i < notes.size(); i++) {
		uint alpha = 255;

		if (notes[i].xpos < 155)
			alpha = notes[i].xpos + 100;

		WINDOW->DrawSprite(	sNote, 
												notes[i].xpos, 
												100 + notes[i].staff_pos * NOTE_OFFSET,
												false, false, alpha);
	}

	// Draw the rating
	if (lastTextTime < 40)
		WINDOW->DrawText(110, 300 + lastTextTime, currentText, textColor);
}

#define CURSOR_AT 103
#define NOTE_CENTER_X_OFFSET 20

#define SCORE_THRESHOLD_GOOD 60
#define SCORE_THRESHOLD_GREAT 40
#define SCORE_THRESHOLD_PERFECT 23

#define BOO_TEXT_COUNT 8
char* boo_texts[] = {
	"No.",
	"Holy crap no.",
	"Terrible.",
	"Awful!",
	"You suck",
	"What was that?",
	"Booooo",
	"My EARS!!"
};

void Keymania::Update() {
	if (INPUT->Key(GAMEKEY_EXIT)) {
		GAMESTATE->SignalGameExit();
	}

	++lastTextTime;

	int closestNote = -1;
	int closestNoteX = 999999;

	for (uint i = 0; i < notes.size(); i++) {
		notes[i].xpos -= NOTE_SPEED;

		// figure out the closest note to the cursor
		if (notes[i].xpos < closestNoteX && notes[i].xpos > CURSOR_AT - 30) {
			closestNoteX = notes[i].xpos;
			closestNote = i;
		}
	
		// hack - reset the notes
		if (notes[i].xpos < -sNote->width) {
			notes[i].xpos = 700;
		}
	}
	
	int diff = notes[closestNote].xpos - CURSOR_AT + NOTE_CENTER_X_OFFSET;
	uint absdiff = abs(diff);
	
	if (diff <= 0)
		SOUND->PlaySound("ring");

	if (INPUT->KeyOnce(PLAYERKEY_JUMP, 1)) {
		fprintf(stderr, "diff=%i xpos=%i\n", diff, notes[closestNote].xpos);

		if (absdiff <= SCORE_THRESHOLD_PERFECT) {
			currentText = "PERFECT!";
			textColor = makecol(255,240,128);
		} else if (absdiff <= SCORE_THRESHOLD_GREAT) {
			currentText = "Great!";
			textColor = makecol(0,256,0);
		} else if (absdiff <= SCORE_THRESHOLD_GOOD) {
			currentText = "Good.";
			textColor = makecol(0,174,240);
		} else {
			currentText = boo_texts[Rand(0, BOO_TEXT_COUNT - 1)];
			textColor = makecol(155,30,30);
		}
		lastTextTime = 0;
	}
}

Keymania::Keymania() {
}

Keymania::~Keymania() {

}
