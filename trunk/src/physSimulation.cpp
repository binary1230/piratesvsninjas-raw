#include "physSimulation.h"

#include "resourceLoader.h"
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
#include "window.h"
#include "gameOptions.h"

#include <stdio.h>
#include <map>
#include <vector>
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
void PhysSimulation::TransformViewToScreen(	int &x, int &y ) {
	y = game_state->ScreenHeight() - y;
}

//! Transforms an object's coordinates from its world coordinates
//! Into "view" coordinates (e.g. x < screen.width, y < screen.height)
void PhysSimulation::TransformWorldToView(int &x, int &y) {
	x = int((x - GetCameraX() ) * camera_scroll_speed);
	y = int((y - GetCameraY() ) * camera_scroll_speed);
	// y = y - GetCameraY();
}

//! Camera paramater: Absolute amount of pixels that 
//! the camera_follow object will be bounded at
#define CAM_ABS_THRESHOLD 40

//! Weighted average nums for the 'floaty' camera
//! Increase CAM_WEIGHT_CAM to make the camera 'snap' quicker
#define CAM_WEIGHT_POS 1.0f // why bother to mess with this one
#define CAM_WEIGHT_CAM 5.0f	// change this one

//! Function which moves the camera according to a weight, shown above
//! Uses a weighted average of the object coordinates and the new camera coords
#define CAM_MOVE_TO_CENTER(cam, o, o_size, s_size) 									\
	int( float( 																											\
			(((o + o_size / 2.0f) - (s_size / 2.0f)) * CAM_WEIGHT_POS) + 	\
			((cam) * CAM_WEIGHT_CAM) 																			\
		) / (CAM_WEIGHT_CAM + CAM_WEIGHT_POS) )

void PhysSimulation::ComputeNewCamera() {
	
	assert(camera_follow);
				
	int ox = camera_follow->GetX();
	int ow = camera_follow->GetWidth();
	int sw = GetGameState()->ScreenWidth();
	
	int oy = camera_follow->GetY();
	int oh = camera_follow->GetHeight();
	int sh = GetGameState()->ScreenHeight();
	
	camera_x = CAM_MOVE_TO_CENTER(camera_x, ox, ow, sw);
	camera_y = CAM_MOVE_TO_CENTER(camera_y, oy, oh, sh);
	
	if (ox - camera_x < CAM_ABS_THRESHOLD)
		camera_x = ox - CAM_ABS_THRESHOLD;
	else if ( (camera_x + sw) - (ox + ow) < CAM_ABS_THRESHOLD )
		camera_x = ox + ow + CAM_ABS_THRESHOLD - sw;
								
	if (oy - camera_y < CAM_ABS_THRESHOLD)
		camera_y = oy - CAM_ABS_THRESHOLD;
	else if ( (camera_y + sh) - (oy + oh) < CAM_ABS_THRESHOLD )
		camera_y  = oy + oh + CAM_ABS_THRESHOLD - sh;
	
	if (camera_x < 0) camera_x = 0;
	if (camera_x > width - sw) camera_x = width - sw;
	if (camera_y < 0) camera_y = 0;
	if (camera_y > height - sh) camera_x = height - sh;
}

void PhysSimulation::Shutdown() {
	ObjectListIter iter;
	int max, i;

	// delete all the objects
	for (iter = objects.begin(); iter != objects.end(); iter++) {
		(*iter)->Shutdown();
		delete (*iter);
		(*iter) = NULL;
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

#define CLEAR_SCREEN_STRING "\033[H\033[J\r\n"

//! Reset all objects for the next frame
void PhysSimulation::ResetForNextFrame() {
	ObjectListIter iter;

	int debug = GetGameState()->GetGameOptions()->GetDebugMessageLevel();

	if (debug)
		fprintf(stderr, CLEAR_SCREEN_STRING);

	for (iter = objects.begin(); iter != objects.end(); iter++) {
		(*iter)->ResetForNextFrame();
	}
}

//! Solve for next frame
void PhysSimulation::Solve() {
	ObjectListIter iter;
	int j, max;
	
	max = forces.size();

	// apply each force to the object
	for (iter = objects.begin(); iter != objects.end(); iter++) {
		for (j = 0; j < max; j++) {
			(*iter)->ApplyForce(forces[j]);
		}
	}
}

void PhysSimulation::GetCollideableObjects(vector<Object*> &objs) {
	ObjectListIter iter;
	objs.clear();

	// optimization: only allow collisions on certain layers?

	for (iter = objects.begin(); iter != objects.end(); iter++) {
		if ((*iter)->GetProperties().is_solid)
			objs.push_back(*iter);
	}
}


// TODO: probably a BIG source of CPU here.
// probably need to optimize, but PROFILE to find out.
void PhysSimulation::CheckForCollisions() {

	vector<Object*> objs;
	Object* player;

	GetCollideableObjects(objs);
	
	int i, j, max = objs.size();
	
	for (i = 0; i < max; i++) {
		if (objs[i]->GetProperties().is_player) {
			
			player = objs[i];

			for (j = 0; j < max; j++) {
				if (objs[j] != player && 
						objs[j]->GetProperties().is_solid &&
						objs[j]->IsColliding(player)) {

					objs[j]->Collide(player);
					player->Collide(objs[j]);
				}
			}
		}
	}	
}

//! Move all objects to newly computed positions.
//! NO COLLISION DETECTION HAPPENS YET
void PhysSimulation::MoveObjectsToNewPositions() {			
	ObjectListIter iter;
	for (iter = objects.begin(); iter != objects.end(); iter++) {
		(*iter)->MoveToNewPosition();
	}
}

//! Update all objects
void PhysSimulation::UpdateObjects() {
	ObjectListIter iter;
	Object* obj;
	for (iter = objects.begin(); iter != objects.end(); iter++) {
		obj = (*iter);
		if (obj) {
			if (!obj->IsDead()) {
				obj->Update();
			} else {
				obj->Shutdown();
				delete obj;
				(*iter) = obj = NULL;
			}
		}
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
	ResetForNextFrame();					// oldpos = current_pos
	Solve();											// Applies forces
	MoveObjectsToNewPositions();	// newpos
	CheckForCollisions();					// newpos = oldpos
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

	camera_x = camera_follow->GetX();
	camera_y = camera_follow->GetY();
	
	return 0;	
}

// Loads the header info from the Mode XML file
int PhysSimulation::LoadHeaderFromXML(XMLNode &xMode) {
	XMLNode xInfo = xMode.getChildNode("info");

	fprintf(stderr, " Loading Level: '%s'\n", xInfo.getChildNode("description").getText() );

	XMLNode xProps = xMode.getChildNode("properties");
	XMLNode xColor;
	int clear_color;

	// get width/height/camera xy
	width 			=	xProps.getChildNode("width").getInt();
	height 			= xProps.getChildNode("height").getInt();
	camera_x = xProps.getChildNode("camera_x").getInt();
	camera_y 	= xProps.getChildNode("camera_y").getInt();

	if (xProps.nChildNode("bgcolor") == 1) {
		xColor = xProps.getChildNode("bgcolor");
		clear_color = makecol(xColor.getChildNode("r").getInt(),
													xColor.getChildNode("g").getInt(),
													xColor.getChildNode("b").getInt() );
	} else {
		clear_color = 0;
	}

	GetGameState()->GetWindow()->SetClearColor(clear_color);

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
		file = GetGameState()->GetResourceLoader()->GetPathOf(file);
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

	// 1) How much do we scroll this layer by?
	float scroll_speed;
  sscanf(xLayer.getAttribute("scroll_speed"), "%f", &scroll_speed);
	layer->SetScrollSpeed(scroll_speed);
	
	// 2) NEW: special case.  Because I, Dom, am LAZY as HELL, I have
	// added a <REPEAT> tag which allows us to create, say, 50
	// objects while only having to declare just one (combine this
	// with random positions, and you have an interesting formula for
	// random level generation!)
	int times_to_repeat, j;
	XMLNode xRepeater;
	max = xLayer.nChildNode("repeat");

	for (i=iterator=0; i < max; i++) {

		// XXX should make this an ATTRIBUTE not the TEXT of the <repeat> tag
		xRepeater = xLayer.getChildNode("repeat", &iterator);
		times_to_repeat = xRepeater.getInt();
		
		// Repeat the creation of this object the specified # of times.
		// same exact object code as above...
		for (j=0; j < times_to_repeat; j++) {

			xObject = xRepeater.getChildNode("object");
			objDefName = xObject.getAttribute("objectDef");

			// create the object from the objectDefinition
			if (LoadObjectFromXML(objectDefs[objDefName], xObject, layer) == -1) {
				fprintf(stderr, "ERROR: Unable To Load  '%s'\n", objDefName.c_str());
				return -1;
			}
		}	
	}

	// 3) for every <object> we find, load it
	max = xLayer.nChildNode("object");

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

	int x,y;
	Object* obj  = objectFactory->CreateObject(xObjectDef, xObject);

	if (!obj) {
		return -1;
	} else {

		// if we have a <cameraFollow>, then we follow this object
		if (xObject.nChildNode("cameraFollow") == 1) {
			if (!camera_follow) {
				camera_follow = obj;
			} else {
				fprintf(stderr, "ERROR: multiple camera targets in map\n");
				return -1;
			}
		}

		// SPECIAL debug flag.  IF it is set, the object MAY print debug message
		if (xObject.nChildNode("debug") == 1) {
			fprintf(stderr, "-------------- Enabling debug mode. ----------\n");
			obj->SetDebugFlag(true);
		}
		
		if (xObject.nChildNode("position") == 1) {

			XMLNode xPos = xObject.getChildNode("position");
			CString type = xPos.getAttribute("type");
			if (type == CString("fixed")) {
				x = xPos.getChildNode("x").getInt();
				y = xPos.getChildNode("y").getInt();
				
			} else if (type == CString("random")) {
				int xmin = xPos.getChildNode("xmin").getInt();
				int ymin = xPos.getChildNode("ymin").getInt();
				int xmax = xPos.getChildNode("xmax").getInt();
				int ymax = xPos.getChildNode("ymax").getInt();
				x = Rand(xmin, xmax);
				y = Rand(ymin, ymax);

			} else {
				fprintf(stderr, "Unknown object position type: %s\n", type.c_str());
				return -1;
			}
				
			// if <alignBottom> is present, we align this sprite with ITs 
			// bottom coordinates. (e.g. saying 0 puts the player on the floor)
			if (xPos.nChildNode("alignBottom")>0) {
				y += obj->GetHeight();
			}
			
			// if <alignRight> is present, we take the X coordinate from the
			// right side instead of the left.
			if (xPos.nChildNode("alignRight")>0) {
				x += obj->GetWidth();
			}

			// if <alignScreenRight> is present, we align this sprite
			// to the SCREEN's right (useful only for overlays)
			if (xPos.nChildNode("alignScreenRight")>0) {
				x = GetGameState()->GetWindow()->Width() - obj->GetWidth() - x;
			}

			// if <alignScreenBottom> is present, we align this sprite
			// to the SCREEN's bottom (useful only for overlays)
			if (xPos.nChildNode("alignScreenBottom")>0) {
				y = GetGameState()->GetWindow()->Height() - obj->GetHeight() - y;
			}

			// One last position calculation:
			// We need to undo the offset of the background here
			// So users don't have to compensate in their data files
			if (layer->GetScrollSpeed() > 0.01f) {
				x = int( float(x) / layer->GetScrollSpeed() );
				y = int( float(y) / layer->GetScrollSpeed() );
			}

			// flipping
			if (xPos.nChildNode("flipx")>0) {
				obj->SetFlipX(true);
			}
			if (xPos.nChildNode("flipy")>0) {
				obj->SetFlipX(false);
			}
		
			obj->SetXY(x,y);

			// check for velocity - <velx> and <vely>
			if (xPos.nChildNode("velx")>0) {
				obj->SetVelX(xPos.getChildNode("velx").getFloat());
			}
			if (xPos.nChildNode("vely")>0) {
				obj->SetVelY(xPos.getChildNode("vely").getFloat());
			}
		}
			
		if (xObject.nChildNode("inputController") == 1) {
			int controller_num = xObject.getChildNode("inputController").getInt();
			obj->SetControllerNum(controller_num);
		}
		
		// Everything loaded OK, now we add it to the simulation
		objects.push_front(obj);
		layer->AddObject(obj);
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
