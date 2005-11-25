#include "objectFactory.h"

// XXX NASTY!! SO BADLY WRITTEN, CLEAN IT UP
// break this up into smaller functions.. COME ON MORON!!X0r.
// ALSO, tons of memory leaks.  We will break this thing down
//  For now it 'just works'
Object* ObjectFactory::CreateObject(uint id) {
	
	Object* new_obj = NULL;
	RadiusBlockObject *rblock;
	PlayerObject *player;
	BackgroundObject *bg;
	ObjectProperties props;

	props.feels_gravity = 0;
	props.feels_user_input = 0;
	
	switch (id) {
		case OBJECT_ID_BACKGROUND:
			
			bg = new BackgroundObject();
			if ( bg && bg->Init(GetGameState()) ) {
							
				PALETTE pal;
				BITMAP* bmp = load_bitmap(get_correct_path("data/back.tga"), pal);
				
				if (bmp) {

					set_palette(pal);

					bg->SetBitmapIsDeleteable(true);
					bg->SetBitmap(bmp);
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

					rblock->SetBitmapIsDeleteable(true);
					rblock->SetBitmap(bmp);
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
				BITMAP* bmp = load_bitmap(get_correct_path("data/kenshin.tga"), pal);
				
				if (bmp) {

					set_palette(pal);

					player->SetBitmapIsDeleteable(true);
					player->SetBitmap(bmp);
					player->SetX(20);
					player->SetY(80);
					player->SetMass(3.0f);

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
}

/*void ObjectFactory::DeleteObject(Object* obj) {
	obj->Delete();
	delete obj;
}*/

int ObjectFactory::Init(GameState* _game_state) {
	SetGameState(_game_state);
	return 0;
}

void ObjectFactory::Shutdown() {
}

ObjectFactory::ObjectFactory() {}
ObjectFactory::~ObjectFactory() {} 
