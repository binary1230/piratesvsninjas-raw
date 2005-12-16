#include "objectFactory.h"

#include "globals.h"
#include "platform.h"
#include "resourceLoader.h"
#include "object.h"
#include "objectIDs.h"
#include "objectPlayer.h"
#include "objectRadiusBlock.h"
#include "objectBackground.h"
#include "animation.h"
#include "objectIDs.h"

/* XXX TODO:
 * - NASTY!! SO BADLY WRITTEN, CLEAN IT UP
 * - width/height autocalc
 * - read in all hardcoded stuff from DATA files.
 * - break this up into smaller functions
 * - For now it 'just works'
 *
 * This is the ugliest code.. EVAR, it ALL needs to be redone.
 * Soon this will all be replaced with a nice XML parser factory thing.
 */
Object* ObjectFactory::CreateObject(uint id) {

	Object* obj = NULL;
	
	switch(id) {
					
		case OBJECT_ID_BACKGROUND:
			obj = BackgroundObject::New(GetGameState());
			break;
			
		case OBJECT_ID_PLAYER:
			obj = PlayerObject::New(GetGameState());
			break;
			
		case OBJECT_ID_RADIUS_BLOCK:
			obj = RadiusBlockObject::New(GetGameState());
			break;
			
		default:
			fprintf(stderr, "ObjectFactory: Unknown Object ID passed: %i\n", id);
	}

	return obj;
}

int ObjectFactory::Init(GameState* _game_state) {
	SetGameState(_game_state);
	resourceLoader = new ResourceLoader();
	if (!resourceLoader || resourceLoader->Init(GetGameState()) == -1)
		return -1;

	// XXX '..' doesn't work yet, resourceLoader's fault.
	resourceLoader->AppendToSearchPath("../");

	return 0;
}

void ObjectFactory::Shutdown() {
	if (resourceLoader) {
		resourceLoader->Shutdown();
		free(resourceLoader);
		resourceLoader = NULL;
	}
}

ObjectFactory::ObjectFactory() {}
ObjectFactory::~ObjectFactory() {} 
