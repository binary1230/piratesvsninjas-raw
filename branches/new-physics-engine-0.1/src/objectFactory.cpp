#include "objectFactory.h"

#include "globals.h"
#include "object.h"
#include "objectIDs.h"
#include "objectPlayer.h"
#include "objectRadiusBlock.h"
#include "objectBackground.h"

/* XXX TODO:
 * - NASTY!! SO BADLY WRITTEN, CLEAN IT UP
 * - width/height autocalc
 * - read in all hardcoded stuff from DATA files.
 * - break this up into smaller functions
 * - For now it 'just works'
 *
 * This is the ugliest code.. EVAR, it ALL needs to be redone.
 * Soon this will all be replaced with a nice XML parser factory thing.
 *
 * UPDATE: some of the nastiness is now gone, replaced with XML
 * goodness.
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
	return 0;
}

void ObjectFactory::Shutdown() {
	SetGameState(NULL);
}

ObjectFactory::ObjectFactory() {}
ObjectFactory::~ObjectFactory() {} 
