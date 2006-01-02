#include "objectController.h"

void ControllerObject::Update() {
				
}

void ControllerObject::Draw() {
	
}

bool ControllerObject::Init(GameState* _game_state) {
	SetGameState(_game_state);
	SetupCachedVariables();
	return true;
}

void ControllerObject::Shutdown() {

}

Object* ControllerObject::New(GameState* _game_state, XMLNode &xDef) {
	Object* obj = new ControllerObject();
	return obj;
}

ControllerObject::ControllerObject() {
	buttons.clear();
	controller = NULL;
}

ControllerObject::~ControllerObject() {}
