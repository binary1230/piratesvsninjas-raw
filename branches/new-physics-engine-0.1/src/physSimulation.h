#ifndef PHYS_SYSTEM_H
#define PHYS_SYSTEM_H

#include <vector>

using namespace std;

class PhysSimulation;

#include "globals.h"
#include "gameBase.h"
#include "objectFactory.h"
#include "object.h"
#include "forceFactory.h"
#include "force.h"
#include "forceInput.h"
#include "forceGravity.h"

//! Represents a physical simulation
class PhysSimulation : GameBase {
		protected:		
			//! Collection of all drawable objects
			ObjectList objects;

			//! Collection of forces
			ForceList forces;
		
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