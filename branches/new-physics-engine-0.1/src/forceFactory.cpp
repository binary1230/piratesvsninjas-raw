#include "forceFactory.h"
#include "force.h"
#include "forceInput.h"
#include "forceGravity.h"

// XXX NASTY!! SO BADLY WRITTEN, CLEAN IT UP
Force* ForceFactory::CreateForce(ForceType type) {
	
	Force* force = NULL;
	ForceInput* inputForce;
	ForceGravity* gravityForce;
	
	switch (type) {
		case FORCE_INPUT:
	
			inputForce = new ForceInput();
			if ( inputForce && inputForce->Init(GetGameState()) ) {

				// Initialize it here
				// inputForce->SetSomething(...)
			
			} else {
				fprintf(stderr, "ForceFactory: Failed to create new Input Force\n");
				if (inputForce) {
						free(inputForce);
						inputForce = NULL;
				}	
			}

			force = inputForce;
			break;

		case FORCE_GRAVITY:
			gravityForce = new ForceGravity();
			if ( gravityForce && gravityForce->Init(GetGameState()) ) {

				// Initialize it here
				// gravityForce->SetSomething(...)
			
			} else {
				fprintf(stderr, "ForceFactory: Failed to create new Input Force\n");
				if (gravityForce) {
						free(gravityForce);
						gravityForce = NULL;
				}	
			}

			force = gravityForce;
			break;
		default:
			fprintf(stderr, "ForceFactory: Unknown ID passed: %i\n", type);
			force = NULL;
	}
	
	return force;
}

/*void ForceFactory::DeleteForce(Force* obj) {
	obj->Delete();
	delete obj;
}*/

int ForceFactory::Init(GameState* _game_state) {
	SetGameState(_game_state);
	return 0;
}

void ForceFactory::Shutdown() {
}

ForceFactory::ForceFactory() {}
ForceFactory::~ForceFactory() {} 
