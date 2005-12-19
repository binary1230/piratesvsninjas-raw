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
#include "gameMode.h"

//! Represents a physical simulation (the main game levels)
class PhysSimulation : public GameMode {
		protected:		
			//! Collection of all drawable objects
			vector<Object*> objects;

			//! Collection of forces
			vector<Force*> forces;
		
			//! Creates new objects
			ObjectFactory *objectFactory;

			//! Creates new forces
			ForceFactory *forceFactory;

			//! Width and height of the entire level
			//! (usually much bigger than screen width/height)
			int width, height;

			//! Top Left corner of camera
			int camera_left, camera_top;

			//! Which object the camera should follow
			Object* camera_follow;
		
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

			int GetWidth() {return width;};
			int GetHeight() {return height;};

			int GetCameraLeft() {return camera_left;};
			int GetCameraTop() {return camera_top;};
			void ComputeNewCamera();

			PhysSimulation();
			~PhysSimulation();
};

#endif
