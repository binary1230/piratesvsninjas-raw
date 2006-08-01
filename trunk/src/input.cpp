#include "input.h"

// - - - - - - - - - - - - - - - - - - 
// Default (hardcoded) key mappings
// - - - - - - - - - - - - - - - - - - 

// Player 1 default game keys (total count must = PLAYER_KEYS_COUNT)
#define DEFAULT_PLAYERKEY_P1_JUMP		KEY_C
#define DEFAULT_PLAYERKEY_P1_LEFT		KEY_LEFT
#define DEFAULT_PLAYERKEY_P1_RIGHT 	KEY_RIGHT
#define DEFAULT_PLAYERKEY_P1_UP 		KEY_UP
#define DEFAULT_PLAYERKEY_P1_DOWN		KEY_DOWN

// Player 2 default game keys
#define DEFAULT_PLAYERKEY_P2_JUMP		KEY_E
#define DEFAULT_PLAYERKEY_P2_LEFT		KEY_A
#define DEFAULT_PLAYERKEY_P2_RIGHT 	KEY_D
#define DEFAULT_PLAYERKEY_P2_UP 		KEY_W
#define DEFAULT_PLAYERKEY_P2_DOWN		KEY_S

// Other keys
#define DEFAULT_GAMEKEY_EXIT				KEY_ESC
#define DEFAULT_GAMEKEY_START				KEY_ENTER
#define DEFAULT_GAMEKEY_DEBUGPAUSE	KEY_F1
#define DEFAULT_GAMEKEY_DEBUGSTEP		KEY_F2
#define DEFAULT_GAMEKEY_SCREENSHOT	KEY_F5

void BaseInput::ClearKeys() {
	ClearKeys(game_key);
}

void BaseInput::ClearKeys(vector<int> &key_buffer) {
	uint i;
	for (i = 0; i < key_buffer.size(); i++) {
		key_buffer[i] = 0;
	}
}

// Turn a playerkey + controller combo into the correct GAMEKEY
// e.g. turn (GAMEKEY_JUMP, controller 1) into KEY #13
// REMEMBER: Controllers start at ONE not ZERO.
int BaseInput::ResolveControllerKey(uint gameKey, uint controller_number) {
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
bool BaseInput::Key(uint gameKey, uint controller_number) {
	int i = ResolveControllerKey(gameKey, controller_number);
	return game_key[i];
}

//! Returns true if a key was first released, then pressed.
//! Can be used to make sure that a player is pressing and releasing
//! a key, instead of just holding it down.  Make sure to call HandleKeyOnce()
//! to update this appropriately.
bool BaseInput::CheckKeyOnce(uint gameKey, uint controller_number) {
	int i = ResolveControllerKey(gameKey, controller_number);
	if (released_key[i] && game_key[i])
		return true;
	else 
		return false;
}

void BaseInput::HandleKeyOnce(uint gameKey, uint controller_number) {
	int i = ResolveControllerKey(gameKey, controller_number);
	released_key[i] = false;
}

bool BaseInput::KeyOnce(uint gameKey, uint controller_number) {
	if (CheckKeyOnce(gameKey, controller_number)) {
		HandleKeyOnce(gameKey, controller_number);
		return true;
	} else {
		return false;
	}
}

void BaseInput::UpdateKeyReleases() {
	for (int i = 0; i < GAMEKEY_COUNT; i++) {
		if (!game_key[i]) 
			released_key[i] = true;
	}
}

bool BaseInput::BaseInit() {
	gamekey_to_realkey.resize(GAMEKEY_COUNT);
	game_key.resize(GAMEKEY_COUNT);
	released_key.resize(GAMEKEY_COUNT);
	
	for (int i = 0; i < GAMEKEY_COUNT; i++) {
		released_key[i] = true;
	}

	return true;
}

void BaseInput::LoadDefaultKeyMappings() {
	fprintf(stderr, " BaseInput: Using default key mappings!\n");
	
	int player1_offset = PLAYERKEY_COUNT * 0;
	int player2_offset = PLAYERKEY_COUNT * 1;

	gamekey_to_realkey[PLAYERKEY_JUMP+player1_offset] =DEFAULT_PLAYERKEY_P1_JUMP;
	gamekey_to_realkey[PLAYERKEY_LEFT+player1_offset] =DEFAULT_PLAYERKEY_P1_LEFT;
	gamekey_to_realkey[PLAYERKEY_RIGHT+player1_offset]=DEFAULT_PLAYERKEY_P1_RIGHT;
	gamekey_to_realkey[PLAYERKEY_UP+player1_offset] 	=DEFAULT_PLAYERKEY_P1_UP;
	gamekey_to_realkey[PLAYERKEY_DOWN+player1_offset] =DEFAULT_PLAYERKEY_P1_DOWN;

	gamekey_to_realkey[PLAYERKEY_JUMP+player2_offset] =DEFAULT_PLAYERKEY_P2_JUMP;
	gamekey_to_realkey[PLAYERKEY_LEFT+player2_offset] =DEFAULT_PLAYERKEY_P2_LEFT;
	gamekey_to_realkey[PLAYERKEY_RIGHT+player2_offset]=DEFAULT_PLAYERKEY_P2_RIGHT;
	gamekey_to_realkey[PLAYERKEY_UP+player2_offset] 	=DEFAULT_PLAYERKEY_P2_UP;
	gamekey_to_realkey[PLAYERKEY_DOWN+player2_offset] =DEFAULT_PLAYERKEY_P2_DOWN;

	gamekey_to_realkey[GAMEKEY_EXIT] = DEFAULT_GAMEKEY_EXIT;
	gamekey_to_realkey[GAMEKEY_START] = DEFAULT_GAMEKEY_START;

	gamekey_to_realkey[GAMEKEY_DEBUGPAUSE] = DEFAULT_GAMEKEY_DEBUGPAUSE;
	gamekey_to_realkey[GAMEKEY_DEBUGSTEP] = DEFAULT_GAMEKEY_DEBUGSTEP;

	gamekey_to_realkey[GAMEKEY_SCREENSHOT] = DEFAULT_GAMEKEY_SCREENSHOT;
	
	ClearKeys();
}

bool BaseInput::LoadKeyMappings(char* filename) {
	fprintf(stderr, "BaseInput: Key map loading not supported yet!\n");
	return false;
}

BaseInput::BaseInput() {}
BaseInput::~BaseInput() {}
