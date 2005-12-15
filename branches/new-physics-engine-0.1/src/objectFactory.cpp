#include "objectFactory.h"

#include "platform.h"
#include "resourceLoader.h"
#include "object.h"
#include "objectIDs.h"
#include "objectPlayer.h"
#include "objectRadiusBlock.h"
#include "objectBackground.h"
#include "animation.h"

/* XXX TODO:
 * - NASTY!! SO BADLY WRITTEN, CLEAN IT UP
 * - width/height autocalc
 * - animation adding.
 * - read in all hardcoded stuff from DATA files.
 * - break this up into smaller functions
 * - For now it 'just works'
 *
 * This is the ugliest code.. EVAR, it ALL needs to be redone.
 * Soon this will all be replaced with a nice XML parser factory thing.
 */

/*
Object* ObjectFactory::CreateBackgroundObject() {
	ObjectProperties props;
	BackgroundObject* obj = new BackGroundObject(1.0f);
	
	if (!obj)
		return NULL;

	props.feels_user_input = 0;
	props.feels_gravity = 0;
	props.feels_friction = 0;
	obj->SetProperties(props);

	obj->SetXY(0, game_state->Height());

	obj->currentAnimation = NULL;
	
	return obj;
}

Object* ObjectFactory::CreateRadiusBlockObject() {
	ObjectProperties props;
	RadiusBlockObject* obj = new RadiusBlockObject();
	
	if (!obj)
		return NULL;

	props.feels_user_input = 0;
	props.feels_gravity = 0;
	props.feels_friction = 0;
	obj->SetProperties(props);

	obj->SetXY(Rand(0, SCREEN_SIZE_X), Rand(0, SCREEN_SIZE_Y));
	obj->SetTheta(Rand(0,360));
	obj->SetRadius(Rand(20,300));

	obj->currentAnimation = NULL;
	
	return obj;
}*/

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
	
	/*switch (id) {
		case OBJECT_ID_BACKGROUND:
			
			bg = new BackgroundObject(1.0f);
			if ( bg && bg->Init(GetGameState()) ) {
							
				PALETTE pal;
				BITMAP* bmp = resourceLoader->OpenBitmap("data/back.tga", &pal);
				
				if (bmp) {

					bg->SetXY(0, game_state->Height());
					bg->SetProperties(props);
					
				} else {
					fprintf(stderr, "ObjectFactory: Failed to load Player's bitmap - %s.\n", get_correct_path("data/back.tga"));
					if (bg) {
							free(bg);
							bg = NULL;
					}
				}
			} else {
				fprintf(stderr, "ObjectFactory: Failed to create Player object.\n");
				if (bg) {
						free(bg);
						bg = NULL;
				}	
			}

			new_obj = bg;
			break;

					
		case OBJECT_ID_RADIUS_BLOCK:
			
			rblock = new RadiusBlockObject();
			if ( rblock && rblock->Init(GetGameState()) ) {
							
				PALETTE pal;
				BITMAP* bmp = load_bitmap(get_correct_path("data/miroku.tga"), pal);
				
				if (bmp) {

					set_palette(pal);

					rblock->SetXY(Rand(0, SCREEN_SIZE_X), Rand(0, SCREEN_SIZE_Y));
					rblock->SetTheta(Rand(0,360));
					rblock->SetRadius(Rand(20,300));
					rblock->SetProperties(props);
					
				} else {
					fprintf(stderr, "ObjectFactory: Failed to load Player's bitmap - %s.\n", get_correct_path("data/back.tga"));
					if (rblock) {
							free(rblock);
							rblock = NULL;
					}
				}
			} else {
				fprintf(stderr, "ObjectFactory: Failed to create Player object.\n");
				if (rblock) {
						free(rblock);
						rblock = NULL;
				}	
			}

			new_obj = rblock;
			break;
			
		case OBJECT_ID_PLAYER:
			
			player = new PlayerObject();

			if ( player && player->Init(GetGameState()) ) {
							
				PALETTE pal;
				BITMAP* bmp = load_bitmap(get_correct_path("data/wait1.bmp"), pal);
				
				if (bmp) {

					set_palette(pal);

					player->SetX(20);
					player->SetY(80);
					player->SetMass(1.0f);

					props.feels_user_input = 1;
					props.feels_gravity = 1;
					props.feels_friction = 0;
					player->SetProperties(props);
					
				} else {
					fprintf(stderr, "ObjectFactory: Failed to create Sonic sprite's bitmap.\n");
					if (player) {
							free(player);
							player = NULL;
					}	
				}
		
			} else {
				fprintf(stderr, "ObjectFactory: Failed to create Sonic sprite.\n");
				if (player) {
						free(player);
						player = NULL;
				}
			}

			new_obj = player;
			break;

		default:
			fprintf(stderr, "ObjectFactory: Unknown ID passed: %i\n", id);
			new_obj = NULL;
	}
	
	return new_obj;
	*/
}

int ObjectFactory::Init(GameState* _game_state) {
	SetGameState(_game_state);
	resourceLoader = new ResourceLoader();
	if (!resourceLoader || resourceLoader->Init(GetGameState()) == -1)
		return -1;

	// XXX doesn't work yet, resourceLoader's fault.
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
