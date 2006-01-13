#include "physSimulation.h"

#include "xmlParser.h"
#include "globals.h"
#include "objectFactory.h"
#include "object.h"
#include "objectIDs.h"
#include "forceFactory.h"
#include "force.h"
#include "forceInput.h"
#include "forceGravity.h"
#include "input.h"
#include "gameState.h"
#include "objectLayer.h"

#include <map>
using namespace std;

int PhysSimulation::Init(GameState* gs, XMLNode xMode) {
	SetGameState(gs);

	objectFactory = new ObjectFactory();
	if ( !objectFactory || objectFactory->Init(GetGameState()) < 0 ) {
		fprintf(stderr, "ERROR: InitSystem: failed to init objectFactory!\n");
		return -1;
	}

	forceFactory = new ForceFactory();
	if ( !forceFactory || forceFactory->Init(GetGameState()) < 0 ) {
		fprintf(stderr, "ERROR: InitSystem: failed to init forceFactory!\n");
		return -1;
	}

	objects.clear();
	forces.clear();
	
	return Load(xMode);
}

//! Transforms view coordinates into absolute screen coordinates
//! e.g. flip the Y axis mostly.
void PhysSimulation::TransformViewToScreen(int &x, int &y) {
	y = game_state->ScreenHeight() - y;
}

//! Transforms an object's coordinates from its world coordinates
//! Into "view" coordinates (e.g. x < screen.width, y < screen.height)
void PhysSimulation::TransformWorldToView(int &x, int &y) {
	x = int((x - GetCameraLeft() ) * camera_scroll_speed);
	y = y - GetCameraTop();
}

// camera threshold in pixels
#define CAM_THRESHOLD 40

void PhysSimulation::ComputeNewCamera() {
	
	assert(camera_follow);
				
	int ox = camera_follow->GetX();
	int ow = camera_follow->GetWidth();
	int sw = GetGameState()->ScreenWidth();
	/*int oy = camera_follow->GetY();
	int oh = camera_follow->GetHeight();
	int sh = GetGameState()->ScreenHeight();*/

	if (ox - camera_left < CAM_THRESHOLD)
		camera_left = ox - CAM_THRESHOLD;
	else if ( (camera_left + sw) - (ox + ow) < CAM_THRESHOLD )
		camera_left = ox + ow + CAM_THRESHOLD - sw;
								
	// XXX needs testing, should work OK.
	/*if (oy - camera_top < CAM_THRESHOLD)
		camera_top = oy - CAM_THRESHOLD;
	else if ( (camera_top + sh) - (ox + ow) < CAM_THRESHOLD )
		camera_top = oy + oh + CAM_THRESHOLD - sh;*/

	// XXX better done as min/max
	if (camera_left < 0) camera_left = 0;
	if (camera_left > width) camera_left = width;
}

void PhysSimulation::Shutdown() {
	int i, max;

	// delete all the objects
	max = objects.size();
	for (i = 0; i < max; i++) {
		objects[i]->Shutdown();
		delete objects[i];
		objects[i] = NULL;
	}
	objects.clear();

	// delete all the forces
	max = forces.size();
	for (i = 0; i < max; i++) {
		forces[i]->Shutdown();
		delete forces[i];
		forces[i] = NULL;
	}
	forces.clear();
			
	// delete the object factory
	if (objectFactory) {
		objectFactory->Shutdown();
		delete objectFactory;
		objectFactory = NULL;
	}

	// delete the force factory
	if (forceFactory) {
		forceFactory->Shutdown();
		delete forceFactory;
		forceFactory = NULL;
	}

	game_state = NULL;
}

//! Draw all objects in this physics simulation
void PhysSimulation::Draw() {
	int i, max = layers.size();

	for (i = 0; i < max; i++) {
		layers[i]->Draw();
	}
}

//! Reset all objects for the next frame
void PhysSimulation::ResetForNextFrame() {
	int i, max = objects.size();

	for (i = 0; i < max; i++) {
		objects[i]->ResetForNextFrame();
	}
}

//! Solve for next frame
void PhysSimulation::Solve() {
	int i, j, max_o, max_f;
	
	max_o = objects.size();
	max_f = forces.size();

	// apply each force to the object
	for (i = 0; i < max_o; i++) {
		for (j = 0; j < max_f; j++) {
			objects[i]->ApplyForce(forces[j]);
		}
	}
}

//! Update all objects
void PhysSimulation::UpdateObjects() {			
	int i, max = objects.size();
	for (i = 0; i < max; i++) {
		objects[i]->Update();
	}
}

//! Master update for the Physics simulation
void PhysSimulation::Update() {
	
	// If they pressed the 'exit' key (typically ESCAPE)
	// Then end the physics simulation
	if (GetGameState()->GetKey(GAMEKEY_EXIT)) {
		GetGameState()->SignalEndCurrentMode();
		return;
	}

	// Do the physics simulation
	ResetForNextFrame();
	Solve();
	UpdateObjects();
		
	// Calc where to put the camera now
	ComputeNewCamera();
}

//! MASTER LOAD FUNCTION:
//! Load the simulation from data in an XML file
int PhysSimulation::Load(XMLNode &xMode) {			

	objects.clear();
	forces.clear();
	if (LoadHeaderFromXML(xMode) == -1 ||
			LoadObjectsFromXML(xMode) == -1 ||
			LoadForcesFromXML(xMode) == -1 )
		return -1;
	
	return 0;	
}

// loads misc junk from the XML file
int PhysSimulation::LoadHeaderFromXML(XMLNode &xMode) {
	XMLNode xInfo = xMode.getChildNode("info");

	fprintf(stderr, " Loading Level: '%s'\n", xInfo.getChildNode("description").getText() );

	XMLNode xProps = xMode.getChildNode("properties");

	// get width/height/camera xy
	width 			=	xProps.getChildNode("width").getInt();
	height 			= xProps.getChildNode("height").getInt();
	camera_left = xProps.getChildNode("camera_left").getInt();
	camera_top 	= xProps.getChildNode("camera_top").getInt();

	return 0;
}

//! Helper function
//! Loads Object Definitions from XML, puts them in an ObjectMapping
int PhysSimulation::LoadObjectDefsFromXML(XMLNode &xObjs, 
																					ObjectDefMapping &objectDefs) {
	// Object definitions can take 2 forms in the XML file
	// 1) an <objectDef> tag
	// 2) an <include_xml_file> tag which we then open and get an <objectDef>
	
	int i, max, iterator;
	XMLNode xObjectDef;
	CString objName, file;
	
	// 1) handle <objectDef> tags
	max = xObjs.nChildNode("objectDef");
	iterator = 0;
	for (i = iterator = 0; i < max; i++) {
		xObjectDef = xObjs.getChildNode("objectDef", &iterator);
		objName = xObjectDef.getAttribute("name");
		objectDefs[objName] = xObjectDef;
	}

	// 2) handle <include_xml_file> tags
	max = xObjs.nChildNode("include_xml_file");
	
	for (i = iterator = 0; i < max; i++) {
					
		// get the filename
		file = xObjs.getChildNode("include_xml_file", &iterator).getText();
		
		// open that file, get the objectDef
		xObjectDef = XMLNode::openFileHelper(file.c_str(), "objectDef");

		// save it
		objName = xObjectDef.getAttribute("name");
		objectDefs[objName] = xObjectDef;
	}

	return 1;
}

/* example of how structure of our XML looks:
 * 
 * <mode>
 *
 * 	<objectDefinitions> .. .. .. </objectDefinitions>
 * 
 *	<map>
 * 		<layer>
 * 			<object type="player"> .. .. .. </object>
 * 			<object type="enemy2"> .. .. .. </object>
 * 		</layer>
 * 		<layer> ... </layer>
 * 	</map>
 * 	
 * </mode>
 */
	
//! Master XML parsing routine for the physics simulation
//! Calls other helpers to deal with different parts of the XML.
int PhysSimulation::LoadObjectsFromXML(XMLNode &xMode) {	
  int i, max, iterator = 0;  
	XMLNode xMap, xObjs, xLayer;	
	ObjectDefMapping objectDefs; 

	camera_follow = NULL;

	// 1) load all "object definitions" (e.g. [bad guy 1])
	xObjs = xMode.getChildNode("objectDefinitions");
	LoadObjectDefsFromXML(xObjs, objectDefs);

	// 2) load all the <object>s found in each <layer> in <map>
	xMap = xMode.getChildNode("map");
	max = xMap.nChildNode("layer");

	// Parse each layer
	iterator = 0;
  for (i=0; i < max; i++) {
		xLayer = xMap.getChildNode("layer", &iterator);
		
		ObjectLayer* layer = new ObjectLayer();
		layer->Init(GetGameState());
		layers.push_back(layer);
		
		if (LoadLayerFromXML(xLayer, layer, objectDefs) == -1) {
			return -1;
		}
	}

	// Finished loading objects, do a few sanity checks
	if (!camera_follow) {
		fprintf(stderr, "ERROR: No <cameraFollow> found, cannot proceed.\n");
		return -1;
	}

	return 0;
}

//! Parse XML info from a <layer> block
int PhysSimulation::LoadLayerFromXML(
								XMLNode &xLayer, 
								ObjectLayer* layer, 
								ObjectDefMapping &objectDefs) {

	int i, iterator, max;
	XMLNode xObject;
	CString objDefName;

	// How much do we scroll this layer by?
	float scroll_speed;
  sscanf(xLayer.getAttribute("scroll_speed"), "%f", &scroll_speed);
	layer->SetScrollSpeed(scroll_speed);
	
	max = xLayer.nChildNode("object");
	
	// Foreach <object> we find, load it
  for (i=iterator=0; i < max; i++) {

		xObject = xLayer.getChildNode("object", &iterator);
		objDefName = xObject.getAttribute("objectDef");

		// create the object from the objectDefinition
		if (LoadObjectFromXML(objectDefs[objDefName], xObject, layer) == -1) {
			fprintf(stderr, "ERROR: Unable To Load  '%s'\n", objDefName.c_str());
			return -1;
		}
	}

	return 0;
}

// Do the REAL work of loading an object from XML
int PhysSimulation::LoadObjectFromXML(
								XMLNode &xObjectDef,
								XMLNode &xObject,
								ObjectLayer* layer) {

	Object* obj  = objectFactory->CreateObject(xObjectDef, xObject);

	if (!obj) {
		return -1;
	} else {

		if (xObject.nChildNode("cameraFollow") == 1) {
			if (!camera_follow) {
				camera_follow = obj;
			} else {
				fprintf(stderr, "ERROR: multiple camera targets in map\n");
				return -1;
			}
		}

		if (xObject.nChildNode("position") == 1) {
			XMLNode xPos = xObject.getChildNode("position");
			CString type = xPos.getAttribute("type");
			if (type == CString("fixed")) {
				int x = xPos.getChildNode("x").getInt();
				int y = xPos.getChildNode("y").getInt();

				// if <alignBottom> is present, we do that.
				if (xPos.nChildNode("alignBottom")>0) {
					y += obj->GetHeight();
				}
					
				obj->SetXY(x,y);
			} else if (type == CString("random")) {
				int xmin = xPos.getChildNode("xmin").getInt();
				int ymin = xPos.getChildNode("ymin").getInt();
				int xmax = xPos.getChildNode("xmax").getInt();
				int ymax = xPos.getChildNode("ymax").getInt();
				obj->SetXY(Rand(xmin, xmax), Rand(ymin, ymax));
			} else {
				fprintf(stderr, "Unknown object position type: %s\n", type.c_str());
				return -1;
			}
		}
			
		if (xObject.nChildNode("inputController") == 1) {
			int controller_num = xObject.getChildNode("inputController").getInt();
			obj->SetControllerNum(controller_num);
		}
		
		// Everything loaded OK, now we add it to the simulation
		objects.push_back(obj);
		layer->PushObject(obj);
	}

	return 0;
}


// loads the forces from the XML file
int PhysSimulation::LoadForcesFromXML(XMLNode &xMode) {
	
	// XXX need to actually load from XML instead of hardcoding...
	Force* new_force = NULL;

	if ( (new_force = forceFactory->CreateForce(FORCE_GRAVITY)) )
		forces.push_back(new_force);
	else
		return -1;
	
	if ( (new_force = forceFactory->CreateForce(FORCE_INPUT1)) )
		forces.push_back(new_force);
	else
		return -1;

	if ( (new_force = forceFactory->CreateForce(FORCE_INPUT2)) )
		forces.push_back(new_force);
	else
		return -1;
	
	return 0;
}

PhysSimulation::PhysSimulation() : objects(0), forces(0) {
	camera_scroll_speed = 1.0f;
}
PhysSimulation::~PhysSimulation() {}
