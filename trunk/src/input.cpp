#include "input.h"
#include "gameState.h"
#include "gameOptions.h"

// - - - - - - - - - - - - - - - - - - 
// Default (hardcoded) key mappings
// - - - - - - - - - - - - - - - - - - 

// Player 1 default game keys (total count must = PLAYER_KEYS_COUNT)
#define DEFAULT_PLAYERKEY_P1_JUMP			KEY_C
#define DEFAULT_PLAYERKEY_P1_LEFT			KEY_LEFT
#define DEFAULT_PLAYERKEY_P1_RIGHT 		KEY_RIGHT
#define DEFAULT_PLAYERKEY_P1_UP 			KEY_UP
#define DEFAULT_PLAYERKEY_P1_DOWN			KEY_DOWN
#define DEFAULT_PLAYERKEY_P1_ACTION1 	KEY_D

// Player 2 default game keys
#define DEFAULT_PLAYERKEY_P2_JUMP			KEY_E
#define DEFAULT_PLAYERKEY_P2_LEFT			KEY_DEL
#define DEFAULT_PLAYERKEY_P2_RIGHT 		KEY_PGDN
#define DEFAULT_PLAYERKEY_P2_UP 			KEY_HOME
#define DEFAULT_PLAYERKEY_P2_DOWN			KEY_END
#define DEFAULT_PLAYERKEY_P2_ACTION1 	KEY_3

// Other keys
#define DEFAULT_GAMEKEY_EXIT				KEY_ESC
#define DEFAULT_GAMEKEY_START				KEY_ENTER
#define DEFAULT_GAMEKEY_DEBUGPAUSE	KEY_F1
#define DEFAULT_GAMEKEY_DEBUGSTEP		KEY_F2
#define DEFAULT_GAMEKEY_SCREENSHOT	KEY_F5

DECLARE_SINGLETON(Input)

void Input::ClearKeys() {
	ClearKeys(game_key);
}

void Input::ClearKeys(vector<int> &key_buffer) {
	uint i;
	for (i = 0; i < key_buffer.size(); i++) {
		key_buffer[i] = 0;
	}
}

// Turn a playerkey + controller combo into the correct GAMEKEY
// e.g. turn (GAMEKEY_JUMP, controller 1) into KEY #13
// REMEMBER: Controllers start at ONE not ZERO.
int Input::ResolveControllerKey(uint gameKey, uint controller_number) {
	assert(controller_number >= 0 && controller_number < GAMEKEY_COUNT);

	if (!controller_number)
		return gameKey;
	else 
		return gameKey + ((controller_number - 1) * PLAYERKEY_COUNT);
}

// REMEMBER, controller numbers start at 1
// 
// This function computes an offset into game_key that represents
// the Nth player.
bool Input::Key(uint gameKey, uint controller_number) {
	int i = ResolveControllerKey(gameKey, controller_number);
	return game_key[i];
}

//! Returns true if a key was first released, then pressed.
//! Can be used to make sure that a player is pressing and releasing
//! a key, instead of just holding it down.  Make sure to call HandleKeyOnce()
//! to update this appropriately.
bool Input::CheckKeyOnce(uint gameKey, uint controller_number) {
	int i = ResolveControllerKey(gameKey, controller_number);
	if (released_key[i] && game_key[i])
		return true;
	else 
		return false;
}

void Input::HandleKeyOnce(uint gameKey, uint controller_number) {
	int i = ResolveControllerKey(gameKey, controller_number);
	released_key[i] = false;
}

bool Input::KeyOnce(uint gameKey, uint controller_number) {
	if (CheckKeyOnce(gameKey, controller_number)) {
		HandleKeyOnce(gameKey, controller_number);
		return true;
	} else {
		return false;
	}
}

void Input::UpdateKeyReleases() {
	for (int i = 0; i < GAMEKEY_COUNT; i++) {
		if (!game_key[i]) 
			released_key[i] = true;
	}
}

bool Input::InitPlayback(CString filename) {
	type = INPUT_PLAYBACK;

	const uint BUF_SIZE = 256; 
	char line2[256];
	bool error = false;
	
	next_frame_data.resize(GAMEKEY_COUNT);

	at_eof = false;
	uint seed;
				
	if (demofile) {
		fprintf(stderr, "InputPlayback: ERROR already reading demo file.\n");
		return false;
	} 

	demofile = fopen(filename.c_str(), "r");
	
	if (!demofile) {
			fprintf(stderr,	"InputPlayback: ERROR can't open demofile '%s'.\n",
							filename.c_str());
			return false;
	}
	
	fprintf(stderr, "InputRecord: Playing back demo from file '%s'.\n", 
							filename.c_str());

	// 1st line2: 'DEMO' header + version info
	// (todo.. we could check for engine version numbers in this line2)
	if ( error	|| !fgets(line2, BUF_SIZE, demofile) 
							|| strncmp(line2, "DEMO", 4) != 0 )
		error = true;

	// 2nd line2, random seed
	if ( error	|| !fgets(line2, BUF_SIZE, demofile)
							|| sscanf(line2, "%u\n", &seed) != 1) {
		error = true;
	} else {
		GAMESTATE->SetRandomSeed(seed);
		fprintf(stderr, "InputPlayback: Using random seed %u\n", seed);
	}

	if (error) {
		fclose(demofile);
		demofile = NULL;
		at_eof = true;
	}

	frame_counter = 0;
	
	ClearKeys();
	ClearKeys(next_frame_data);
	GetNextFrameData();

	return !error;
}

void Input::UseNextFrameData() {	
	uint i;
	for (i = 0; i < GAMEKEY_COUNT; i++) {
		game_key[i] = next_frame_data[i];
	}
}

//! Reads the next frame from the demo file.  Closes the
//! file if we are at the end of file.
void Input::GetNextFrameData() {
	const uint BUF_SIZE = 512;
	char buf[BUF_SIZE];
	char *s = buf;
	bool error = false;	

	uint next_frame;
	uint key, value;
	uint count = 0;

	if (!demofile || at_eof)
		return;
	
	if (!fgets(buf, BUF_SIZE, demofile)) {
		at_eof = true;
		return;
	}
	
	// the file format is like this:
	// "frame_number key1 value1 [key2] [value2]\n"
	
	// read the frame number
	count = sscanf(buf, "%u", &next_frame);
	if (count != 1 || next_frame <= frame_counter)
		error = true;
		
	// go to next space
	if (error || (s = strchr(buf, ' ')) == NULL)
		error = true;
	else
		next_frame_num = next_frame;
			
	// read each key,value pair,
	// overwrite existing values in next_data_
	count = 2;
	while (!error && count == 2 && s) {
						
		s++;	// skip over current space
		count = sscanf(s, "%u %u", &key, &value);
			
		if (count == 2) {
			next_frame_data[key] = value;

			// skip 2 spaces
			s = strchr(s, ' ');
			if (s) 
				s = strchr(s+1, ' ');	// will be NULL at end of the line
			else
				count = 1;						// fall through to error handler
		}
			
		if (count == 1) {
			// total badness.. we need to handle this better,
			// but just DIE right here.
			fprintf(stderr, "HUGE HUGE INTERNAL ERROR: Could only read one value\n"
											"from the demo file, it is probably corrupted.\n");
			error = true;
			exit(-1);
		}
			
	}	
}

bool Input::InitRecorder(CString filename) {
	type = INPUT_RECORDED;
	old_key.resize(GAMEKEY_COUNT);

	if (demofile) {
		fprintf(stderr, "InputRecord: ERROR already saving demo file.\n");
		return false;
	} 

	demofile = fopen(filename.c_str(), "w");
	
	if (!demofile) {
			fprintf(stderr,	"InputRecord: ERROR can't write to demofile '%s'.\n",
							filename.c_str());
			return false;
	}

	fprintf(stderr, "InputRecord: Recording demo to file '%s'.\n", 
							filename.c_str());

	// write 'DEMO' header + game version number and some extra info
	fprintf(demofile, "DEMO:ninja-engine saved demo file:%s:%s\n",
										VERSION_STRING, ALLEGRO_PLATFORM_STR);
	
	// write the current random seed
	fprintf(demofile, "%i\n", GAMESTATE->GetRandomSeed() );
	
	return true;
}

int Input::Init() {
	
	if (!CommonInit()) {
		return -1;
	}

	if (GAMESTATE->GetGameOptions()->RecordDemo()) {
		if (!InitRecorder(GAMESTATE->GetGameOptions()->GetDemoFilename()))
			return -1;
	} else if (GAMESTATE->GetGameOptions()->PlaybackDemo()) {
		if (!InitPlayback(GAMESTATE->GetGameOptions()->GetDemoFilename()))
			return -1;
	} else {
		if (!InitLive())
			return -1;
	}

	return 0;
}

bool Input::InitLive() {
	type = INPUT_LIVE;
	return true;
}


bool Input::CommonInit() {
	install_mouse();
	install_keyboard();

	gamekey_to_realkey.resize(GAMEKEY_COUNT);
	game_key.resize(GAMEKEY_COUNT);
	released_key.resize(GAMEKEY_COUNT);
	
	for (int i = 0; i < GAMEKEY_COUNT; i++) {
		released_key[i] = true;
	}

	demofile = NULL;
	
	LoadDefaultKeyMappings();
	return true;
}

void Input::LoadDefaultKeyMappings() {
	fprintf(stderr, " Input: Using default key mappings!\n");
	
	int player1_offset = PLAYERKEY_COUNT * 0;
	int player2_offset = PLAYERKEY_COUNT * 1;

	gamekey_to_realkey[PLAYERKEY_JUMP+player1_offset] =DEFAULT_PLAYERKEY_P1_JUMP;
	gamekey_to_realkey[PLAYERKEY_LEFT+player1_offset] =DEFAULT_PLAYERKEY_P1_LEFT;
	gamekey_to_realkey[PLAYERKEY_RIGHT+player1_offset]=DEFAULT_PLAYERKEY_P1_RIGHT;
	gamekey_to_realkey[PLAYERKEY_UP+player1_offset] 	=DEFAULT_PLAYERKEY_P1_UP;
	gamekey_to_realkey[PLAYERKEY_DOWN+player1_offset] =DEFAULT_PLAYERKEY_P1_DOWN;
	gamekey_to_realkey[PLAYERKEY_ACTION1+player1_offset] =DEFAULT_PLAYERKEY_P1_ACTION1;

	gamekey_to_realkey[PLAYERKEY_JUMP+player2_offset] =DEFAULT_PLAYERKEY_P2_JUMP;
	gamekey_to_realkey[PLAYERKEY_LEFT+player2_offset] =DEFAULT_PLAYERKEY_P2_LEFT;
	gamekey_to_realkey[PLAYERKEY_RIGHT+player2_offset]=DEFAULT_PLAYERKEY_P2_RIGHT;
	gamekey_to_realkey[PLAYERKEY_UP+player2_offset] 	=DEFAULT_PLAYERKEY_P2_UP;
	gamekey_to_realkey[PLAYERKEY_DOWN+player2_offset] =DEFAULT_PLAYERKEY_P2_DOWN;
	gamekey_to_realkey[PLAYERKEY_ACTION1+player2_offset] =DEFAULT_PLAYERKEY_P2_ACTION1;

	gamekey_to_realkey[GAMEKEY_EXIT] = DEFAULT_GAMEKEY_EXIT;
	gamekey_to_realkey[GAMEKEY_START] = DEFAULT_GAMEKEY_START;

	gamekey_to_realkey[GAMEKEY_DEBUGPAUSE] = DEFAULT_GAMEKEY_DEBUGPAUSE;
	gamekey_to_realkey[GAMEKEY_DEBUGSTEP] = DEFAULT_GAMEKEY_DEBUGSTEP;

	gamekey_to_realkey[GAMEKEY_SCREENSHOT] = DEFAULT_GAMEKEY_SCREENSHOT;
	
	ClearKeys();
}

bool Input::LoadKeyMappings(char* filename) {
	fprintf(stderr, "Input: Key map loading not supported yet!\n");
	return false;
}

void Input::Shutdown() {
	if (demofile) {
		fprintf(stderr, "WARN: closing demofile, but "
										"record/playback still in progress.");
		fclose(demofile);
	}
		
	demofile = NULL;

	remove_mouse();
	remove_keyboard();
}

void Input::Update() {
	switch (type) {
		case INPUT_RECORDED:
			UpdateRecord();
			break;
		case INPUT_PLAYBACK:
			UpdatePlayback();
			break;
		case INPUT_LIVE:
			UpdateLive();
			break;
	}
}

//! This is A little complicated..
//! First we save the old keys being pressed.
//! Then we get the new keys being pressed.
//! If anything is different, write it to the demo with the frame counter
void Input::UpdateRecord() {
	
	uint i;
	bool keys_changed = false;
	
	// TODO: check to make sure this doesn't get too big.
	frame_counter++;
	
	for (i = 0; i < GAMEKEY_COUNT; i++) {
		// Save the old keys
		old_key[i] = game_key[i];

		// Get new keys
		if ( key[gamekey_to_realkey[i]] )
			game_key[i] = 1;
		else 
			game_key[i] = 0;

		// Output any differences between the old keys and the new keys to a file
		if ( demofile && (old_key[i] != game_key[i]) ) {
				if (!keys_changed) {
						fprintf(demofile, "%lu", frame_counter);
						keys_changed = true;
				}

				// remember, we are writing out GAMEKEYs not REAL keys.
				// e.g. KEY_JUMP, not KEY_SPACEBAR
				fprintf(demofile, " %u %u", i, game_key[i]);
		}
	}
	
	if (keys_changed && demofile) {
			fprintf(demofile, "\n");
	}

	UpdateKeyReleases();
}

//! Update the state of the input
void Input::UpdatePlayback() {
	
	// TODO: check to make sure this doesn't get too big.
	frame_counter++;

	// Read keystrokes from Demo file if they exist
	if (frame_counter == next_frame_num && !at_eof) {
			UseNextFrameData();	
			GetNextFrameData();
	}	

	// SPECIAL EXCEPTION
	// everything comes back from the demo file,
	// but we still allow the user to press GAMEKEY_EXIT key LIVE
	// so they can exit the demo manually
	if (key[gamekey_to_realkey[GAMEKEY_EXIT]])
		game_key[GAMEKEY_EXIT] = 1;	
	
	UpdateKeyReleases();
}

//! Live Input update (freeze state of input)
//! freeze the current state of the input into gamekey[].
//! key[] is from allegro, it is the current state of what 
//! is currently being pressed
void Input::UpdateLive() {
	uint i;
	for (i = 0; i < GAMEKEY_COUNT; i++) {
		game_key[i] = key[gamekey_to_realkey[i]];
	}

	UpdateKeyReleases();
}

void Input::BeginRecording()	{				
	if (!demofile) {
		fprintf(stderr,	"InputRecord: ERROR InitRecorder() not called yet!\n");
		return;
	}
	
	ClearKeys();
	ClearKeys(old_key);

	frame_counter = 0;
}

void Input::EndRecording()	{
	fclose(demofile);
	demofile = NULL;
}

void Input::Begin() {
	if (type == INPUT_RECORDED)
		BeginRecording();
	else if (type == INPUT_PLAYBACK)
		BeginPlayback();
}

void Input::End() {
	if (type == INPUT_RECORDED)
		EndRecording();
	else if (type == INPUT_PLAYBACK)
		EndPlayback();
}

void Input::BeginPlayback()	{				
	ClearKeys();

	if (!demofile) {
		fprintf(stderr,	"InputPlayback: ERROR InitPlayback() not called yet!\n");
		return;
	}	

	frame_counter = 0;
}

void Input::EndPlayback()	{
	fclose(demofile);
	demofile = NULL;
}

Input::Input() : demofile(NULL) {}
Input::~Input() {}
