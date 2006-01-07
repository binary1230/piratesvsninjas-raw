#ifndef OBJ_CONTROLLER_H
#define OBJ_CONTROLLER_H

#include <allegro.h>
#include <vector>
using namespace std;

class GameState;
class Sprite;

#include "object.h"
#include "xmlParser.h"

struct Button {
	bool active;
	Sprite* sprite;
};

class ControllerObject : public Object {
	protected:
		Sprite* controller_sprite;
		vector<struct Button> buttons;
		
	public:
		bool Init(GameState*);
		void Shutdown();

		void Update();
		void Draw();

		ControllerObject();
		~ControllerObject();

		static Object* New(GameState*, XMLNode &xDef);
};

#endif // OBJ_CONTROLLER_H
