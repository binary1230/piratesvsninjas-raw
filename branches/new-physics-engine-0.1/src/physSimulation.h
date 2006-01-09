#ifndef PHYS_SYSTEM_H
#define PHYS_SYSTEM_H

#include <vector>
#include <map>
using namespace std;

class PhysSimulation;
class GameState;
class Force;
class Object;
class ObjectFactory;
class ForceFactory;
class ObjectLayer;

#include "gameBase.h"
#include "gameMode.h"
#include "StdString.h"
#include "xmlParser.h"

// Maps an object definition name to an XMLNode 
// (e.g. maps "bad_guy_1" to its corresponding XML data)
// only used for parsing XML
typedef map<const CString, XMLNode> ObjectDefMapping;

//! Represents a physical simulation (the main game levels)
class PhysSimulation : public GameMode {
		protected:		
			//! ALL objects in the scene
			vector<Object*> objects;

			//! Layers, which hold pointers to objects.
			vector<ObjectLayer*> layers;

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

			//! Physics functions
			void ResetForNextFrame();
			void Solve();
			void UpdateObjects();
		
			//! Sets up simulation from an XML file
			//XXX should be moved into a friend factory class
			int Load(XMLNode&);
			int LoadHeaderFromXML(XMLNode&);
			int LoadObjectsFromXML(XMLNode&);
			int LoadForcesFromXML(XMLNode&);
			int LoadObjectDefsFromXML(XMLNode&, ObjectDefMapping&);
			int LoadLayerFromXML(XMLNode &, ObjectLayer*, ObjectDefMapping&);

		public:
			int Init(GameState*, XMLNode);
			void Shutdown();

			void Draw();
			void Update();

			int GetWidth() {return width;};
			int GetHeight() {return height;};

			void ComputeNewCamera();

			int GetCameraLeft() {return camera_left;};
			int GetCameraTop() {return camera_top;};
			
			void TransformWorldToView(int &x, int &y);
			void TransformViewToScreen(int &x, int &y);

			PhysSimulation();
			~PhysSimulation();
};

#endif
