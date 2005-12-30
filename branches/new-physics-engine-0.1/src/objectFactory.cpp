#include "objectFactory.h"

#include "globals.h"
#include "object.h"
#include "objectIDs.h"
#include "objectPlayer.h"
#include "objectRadiusBlock.h"
#include "objectBackground.h"
#include "StdString.h"

#include <map>

using namespace std;

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
 * goodness. (some of it anyway.)
 */

// Creates an object from an XML definition
// in: xObjectDef - XML representation of an object's definition
// in: xObject - XML representation of additional object paramaters
// returns: newly create Object*, or NULL if it failed
Object* ObjectFactory::CreateObject(XMLNode &xObjectDef, XMLNode &xObject) {

	Object* obj = NULL;

	// this shouldn't go here.
	// maps strings of object types to numeric ID's.
	map<const CString, uint> types;
	types["RadiusBlock"] 	= OBJECT_ID_RADIUS_BLOCK;
	types["Background"] 		= OBJECT_ID_BACKGROUND;
	types["Player"] 				= OBJECT_ID_PLAYER;
	
	CString objType = xObjectDef.getAttribute("type");
	
	uint id = types[objType];

	switch(id) {
					
		case OBJECT_ID_BACKGROUND:
			obj = BackgroundObject::New(GetGameState(), xObjectDef);
			break;
			
		case OBJECT_ID_PLAYER:
			obj = PlayerObject::New(GetGameState(), xObjectDef);
			break;
			
		case OBJECT_ID_RADIUS_BLOCK:
			obj = RadiusBlockObject::New(GetGameState(), xObjectDef);
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
