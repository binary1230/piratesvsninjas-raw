#include "physSimulation.h"

int PhysSimulation::Init(GameState* gs) {
	SetGameState(gs);

	objectFactory = new ObjectFactory();
	if ( !objectFactory || objectFactory->Init(GetGameState()) < 0 ) {
		fprintf(stderr, "ERROR: InitSystem: failed to init objectFactory!\n");
		return -1;
	}
	
	return Load();
}

void PhysSimulation::Shutdown() {
	int i, max;

	// delete all the objects
	max = objects.size();
	for (i = 0; i < max; i++) {
		objects[i]->Shutdown();
		delete objects[i];
		objects[i] = NULL;
	}
	objects.clear();

	// delete all the forces
	max = forces.size();
	for (i = 0; i < max; i++) {
		forces[i]->Shutdown();
		delete forces[i];
		forces[i] = NULL;
	}
	forces.clear();
			
	// delete the object factory
	if (objectFactory) {
		objectFactory->Shutdown();
		delete objectFactory;
		objectFactory = NULL;
	}
				
	game_state = NULL;
}

//! Draw all objects in this physics simulation
void PhysSimulation::Draw() {
	int i, max = objects.size();

	for (i = 0; i < max; i++) {
		objects[i]->Draw();
	}
}

//! Reset all objects for the next frame
void PhysSimulation::ResetForNextFrame() {
	int i, max = objects.size();

	for (i = 0; i < max; i++) {
		objects[i]->ResetForNextFrame();
	}
}

//! Solve for next frame
void PhysSimulation::Solve() {
	int i, max_o, max_f;
	
	max_o = objects.size();
	max_f = forces.size();

	for (i = 0; i < max_o; i++) {
		for (j = 0; j < max_f; j++) {
			objects[i]->ApplyForce(forces[j]);
		}
	}
}

//! Solve for next frame
void PhysSimulation::UpdateObjects() {
	int i, max = objects.size();

	for (i = 0; i < max; i++) {
		objects[i]->Update();
	}
}

//! Master update for the Physics simulation
void PhysSimulation::Update() {
	ResetForNextFrame();
	Solve();
	UpdateObjects();
}

//! Eventually, load the initial state from a map file
//! For now, we just create some random objects for a demo
int PhysSimulation::Load() {
				
	Object* new_obj;
	
	int i, max = 5;
	objects.resize(0);

	new_obj = objectFactory->CreateObject(OBJECT_ID_BACKGROUND);
	if (!new_obj)
		return -1;
	
	objects.push_back(new_obj);
	
	for (i = 0; i < max; i++) {
		new_obj = objectFactory->CreateObject(OBJECT_ID_RADIUS_BLOCK);
		if (!new_obj)
			return -1;

		objects.push_back(new_obj);
	}
	
	new_obj = objectFactory->CreateObject(OBJECT_ID_MOUSE_BLOCK);
	if (!new_obj)
		return -1;

	objects.push_back(new_obj);
	return 0;	
}

PhysSimulation::PhysSimulation() : objects(0), forces(0) {}
PhysSimulation::~PhysSimulation() {}
