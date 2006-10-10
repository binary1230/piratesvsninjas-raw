#include "objectCollectable.h"

#include "globals.h"
#include "animation.h"
#include "gameState.h"
#include "gameSound.h"

void CollectableObject::Update() {
	BaseUpdate();
	UpdateSimpleAnimations();
}

bool CollectableObject::Init(PhysSimulation *p) {
	simulation = p;
	return BaseInit();
}

CollectableObject::CollectableObject() {}
CollectableObject::~CollectableObject() {}

void CollectableObject::Collide(Object* obj) {
	if (obj->GetProperties().is_player) {
		SOUND->PlaySound("ring");
		is_dead = true;
	}
}
