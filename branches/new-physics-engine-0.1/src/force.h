/* A force vector
 * all time increments are 1/60th of a second
 */

#ifndef FORCE_H
#define FORCE_H

#include <vector>
using namespace std;

class Force;
class GameState;

typedef vector<Force*> ForceList;

#include "globals.h"
#include "gameBase.h"
#include "vector2D.h"
#include "object.h"

enum ForceType {
	FORCE_GRAVITY,
	FORCE_INPUT,
	FORCE_SIMPLE_CIRCLE,
	FORCE_FRICTION
};

class Force : public GameBase {
	protected:
		ForceType type;
				
	public:
		bool Init(GameState* _game_state);
		virtual Vector2D GetAcceleration(Object* obj) = 0;
		
		void Shutdown();

		inline ForceType GetType() {return type;};
		// inline void SetType(ForceType t) {type = t;};

		Force();
		virtual ~Force();
};


#endif // FORCE_H
