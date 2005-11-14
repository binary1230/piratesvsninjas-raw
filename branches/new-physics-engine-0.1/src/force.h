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

enum ForceType {
	FORCE_GRAVITY,
	FORCE_INPUT,
	FORCE_SIMPLE_CIRCLE,
};

class Force : public GameBase {
	protected:
		ForceType type;
				
	public:
		void Init(GameState* _game_state);
		virtual Vector2D GetAcceleration() = 0;
		
		void Shutdown();

		inline ForceType GetType() {return type;};
		// inline void SetType(ForceType t) {type = t;};

		Force();
		virtual ~Force();
};


#endif // FORCE_H
