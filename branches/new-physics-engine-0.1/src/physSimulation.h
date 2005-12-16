#ifndef PHYS_SYSTEM_H
#define PHYS_SYSTEM_H

#include <vector>
using namespace std;

class PhysSimulation;
class GameState;
class Force;
class Object;
class ObjectFactory;
class ForceFactory;

#include "gameBase.h"

//! Represents a physical simulation
class PhysSimulation : GameBase {
		protected:		
			//! Collection of all drawable objects
			vector<Object*> objects;

			//! Collection of forces
			vector<Force*> forces;
		
			//! Creates new objects
			ObjectFactory *objectFactory;

			//! Creates new forces
			ForceFactory *forceFactory;
		
			//! Loads a simulation (eventually from a file)
			int Load();

			void ResetForNextFrame();
			void Solve();
			void UpdateObjects();
		public:

			int Init(GameState* gs);
			void Shutdown();

			void Draw();
			void Update();

			PhysSimulation();
			~PhysSimulation();
};

#endif
