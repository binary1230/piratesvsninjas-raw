#ifndef OBJ_CONTROLLER_H
#define OBJ_CONTROLLER_H

#include <allegro.h>
#include <vector>
using namespace std;

class ControllerObject;
class GameState;

#include "object.h"
#include "xmlParser.h"

struct Button {
	bool active;
	int x_offset, y_offset;
	BITMAP* bmp;
};

class ControllerObject : public Object {
	protected:
		BITMAP* controller;
		vector<Button> buttons;
		
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
