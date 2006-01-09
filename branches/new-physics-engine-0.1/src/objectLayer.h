#ifndef OBJECTLAYER_H
#define OBJECTLAYER_H

#include <allegro.h>
#include <vector>
using namespace std;

#include "gameBase.h"
// #include "xmlParser.h"

class Object;

//! Basically a container class representing "layers" 
//! (e.g. background, foreground, etc).  A layer holds
//! the objects on it.

class ObjectLayer : public GameBase {
	protected:
		vector<Object*> objects;
		bool visible;
		float scroll_speed;
	
	public:
		bool Init(GameState*);
		void Shutdown();

		//! Get/set layer scroll speed
		void SetScrollSpeed(float _scroll_speed) { scroll_speed = _scroll_speed ;};
		float GetScrollSpeed() { return scroll_speed; };

		//! Get/Set layer visibility
		bool IsVisible() { return visible; };
		void SetVisible(bool _visible) { visible = _visible; };

		//! Return a pointer to a vector of objects in this layer
		//vector<Object*> * GetObjects() {return &objects; };
		//void SetObjects(const vector<Object*> &objs) {objects = objs; };
		void Draw();
		
		//! Push an object onto this layer
		void PushObject(Object*);
		
		ObjectLayer();
		virtual ~ObjectLayer();
};

#endif // OBJECTLAYER_H
