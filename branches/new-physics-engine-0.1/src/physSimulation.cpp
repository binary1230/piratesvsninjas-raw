#include "physSimulation.h"

#include "xmlParser.h"
#include "globals.h"
#include "objectFactory.h"
#include "object.h"
#include "objectIDs.h"
#include "forceFactory.h"
#include "force.h"
#include "forceInput.h"
#include "forceGravity.h"
#include "input.h"
#include "gameState.h"

int PhysSimulation::Init(GameState* gs) {
	SetGameState(gs);

	objectFactory = new ObjectFactory();
	if ( !objectFactory || objectFactory->Init(GetGameState()) < 0 ) {
		fprintf(stderr, "ERROR: InitSystem: failed to init objectFactory!\n");
		return -1;
	}

	forceFactory = new ForceFactory();
	if ( !forceFactory || forceFactory->Init(GetGameState()) < 0 ) {
		fprintf(stderr, "ERROR: InitSystem: failed to init forceFactory!\n");
		return -1;
	}

	objects.clear();
	forces.clear();
	
	return Load();
}

// camera threshold in pixels
#define CAM_THRESHOLD 40

void PhysSimulation::ComputeNewCamera() {
	int ox = camera_follow->GetX();
	int ow = camera_follow->GetWidth();
	int sw = GetGameState()->ScreenWidth();
	/*int oy = camera_follow->GetY();
	int oh = camera_follow->GetHeight();
	int sh = GetGameState()->ScreenHeight();*/

	if (ox - camera_left < CAM_THRESHOLD)
		camera_left = ox - CAM_THRESHOLD;
	else if ( (camera_left + sw) - (ox + ow) < CAM_THRESHOLD )
		camera_left = ox + ow + CAM_THRESHOLD - sw;
								
	// XXX needs testing, should work OK.
	/*if (oy - camera_top < CAM_THRESHOLD)
		camera_top = oy - CAM_THRESHOLD;
	else if ( (camera_top + sh) - (ox + ow) < CAM_THRESHOLD )
		camera_top = oy + oh + CAM_THRESHOLD - sh;*/

	// XXX better done as min/max
	if (camera_left < 0) camera_left = 0;
	if (camera_left > width) camera_left = width;
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

	// delete the force factory
	if (forceFactory) {
		forceFactory->Shutdown();
		delete forceFactory;
		forceFactory = NULL;
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
	int i, j, max_o, max_f;
	
	max_o = objects.size();
	max_f = forces.size();

	// apply each force to the object
	for (i = 0; i < max_o; i++) {
		for (j = 0; j < max_f; j++) {
			objects[i]->ApplyForce(forces[j]);
		}
	}
}

//! Update all objects
void PhysSimulation::UpdateObjects() {			
	int i, max = objects.size();
	for (i = 0; i < max; i++) {
		objects[i]->Update();
	}
}

//! Master update for the Physics simulation
void PhysSimulation::Update() {
	
	// If they pressed the 'exit' key (typically ESCAPE)
	// Then end the physics simulation
	if (GetGameState()->GetKey(GAMEKEY_EXIT)) {
		GetGameState()->SignalEndCurrentMode();
		return;
	}

	// Do the physics simulation
	ResetForNextFrame();
	Solve();
	UpdateObjects();
		
	// Calc where to put the camera now
	ComputeNewCamera();
}

//! Eventually, load the initial state from a map file
//! For now, we just create some random objects + forces for a demo
int PhysSimulation::Load() {
				
	Object* new_obj;
	int i, max = 30;

	// long level, same height as screen though
	width=320 * 8;
	height=240;
	camera_left = 0; camera_top = 0;

	// -- Create some random objects --
	objects.clear();
	
	// Create the background
	new_obj = objectFactory->CreateObject(OBJECT_ID_BACKGROUND);
	if (!new_obj)
		return -1;
	
	objects.push_back(new_obj);

	// Create a bunch of "radius blocks" (they move in a circle)
	for (i = 0; i < max; i++) {
		new_obj = objectFactory->CreateObject(OBJECT_ID_RADIUS_BLOCK);
		if (!new_obj)
			return -1;

		objects.push_back(new_obj);
	}
	
	// Create the main player
	new_obj = objectFactory->CreateObject(OBJECT_ID_PLAYER);
	if (!new_obj)
		return -1;

	objects.push_back(new_obj);
	
	// Follow the player object
	camera_follow = new_obj;

	// -- Create some forces --
	
	Force* new_force = NULL;
	forces.clear();

	if ( (new_force = forceFactory->CreateForce(FORCE_GRAVITY)) )
		forces.push_back(new_force);
	else
		return -1;
	
	if ( (new_force = forceFactory->CreateForce(FORCE_INPUT)) )
		forces.push_back(new_force);
	else
		return -1;

	return 0;	
}

PhysSimulation::PhysSimulation() : objects(0), forces(0) {}
PhysSimulation::~PhysSimulation() {}
