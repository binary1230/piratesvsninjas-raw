#ifndef BackgroundObject_H
#define BackgroundObject_H

#include <math.h>
#include <stdio.h>

class BackgroundObject;

#include "globals.h"
#include "object.h"

class BackgroundObject : public Object {
	protected:
		float scroll_offset;
		float speed;
				
	public:
		bool Init(GameState* _game_state);

		void Update();
		void Draw();

		BackgroundObject(float _speed = 0.2);
		~BackgroundObject();
};

#endif // BackgroundObject_H   
