#ifndef MAP_EDITOR_H
#define MAP_EDITOR_H

#include "stdafx.h"
#include "physSimulation.h"

class Sprite;

class MapEditor : public GameWorld {
	public:
		virtual int Init(XMLNode);
		virtual void Shutdown();

		virtual void Draw();
		virtual void Update();

		virtual void LoadMusic(const char* filename);
		virtual int LoadObjectDefsFromXML(XMLNode&);

		void ComputeNewScrolling();
		
		MapEditor();
		virtual ~MapEditor();
	protected:
		Sprite* cursor_sprite;

		XMLNode xObjDefs;
};

#endif // MAP_EDITOR_H
