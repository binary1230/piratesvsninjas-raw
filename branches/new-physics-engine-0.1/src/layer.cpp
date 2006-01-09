#include "layer.h"
#include "gameState.h"
#include "xmlParser.h"
#include "StdString.h"
#include "object.h"

ObjectLayer::ObjectLayer() {
	scroll_speed = 1;
	visible = true;	
}

ObjectLayer::~ObjectLayer() {}

void ObjectLayer::PushObject(Object* obj) {
	objects.push_back(obj);
}

bool ObjectLayer::Init(GameState* gs) {
	scroll_speed = 1;
	visible = true;
	SetGameState(gs);
	return true;
}

void ObjectLayer::Shutdown() {
	SetGameState(NULL);	
}
