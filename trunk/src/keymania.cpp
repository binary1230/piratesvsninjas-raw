#include "keymania.h"
#include "sprite.h"
#include "assetManager.h"
#include "gameSound.h"
#include "window.h"
#include "gameState.h"
#include "input.h"

DECLARE_SINGLETON(Keymania)

bool Keymania::Init() {
	test = ASSETMANAGER->LoadSprite("test.png");

	if (test)
		return true;
	else
		return false;
}

void Keymania::Shutdown() {
	test = NULL;	
}

void Keymania::Draw() {
	WINDOW->DrawSprite(test, 0, 0);
}

void Keymania::Update() {
	if (INPUT->Key(GAMEKEY_EXIT)) {
		GAMESTATE->SignalGameExit();
	}
}

Keymania::Keymania() {
	test = NULL;
}

Keymania::~Keymania() {

}
