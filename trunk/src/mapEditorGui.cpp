#include "stdafx.h"
#include "mapEditor.h"
#include "mapEditorGui.h"
#include "objectLayer.h"
#include "mapSaver.h"
#include "sprite.h"
#include "window.h"
#include "assetManager.h"
#include "input.h"
#include "gameState.h"
#include "window.h"
#include "object.h"
#include "objectFactory.h"
#include "luaManager.h"

void MapEditorGui::Shutdown()
{
	LUA->RunVoidFunctionNoArgs("mapeditorgui_shutdown", true);
	LUA->ReleaseCurrentLuaScript();
}

void MapEditorGui::Start()
{
	LUA->RunVoidFunctionNoArgs("mapeditorgui_run", true);
}

void MapEditorGui::Update()
{
	// No need to update LUA, it'll do it's own Update()
}

bool MapEditorGui::Init() 
{
	LUA->LoadLuaScript("src/lua/mapeditorGUI.lua");
	LUA->RunVoidFunctionNoArgs("mapeditorgui_init", true);

	return true;
}

MapEditorGui::MapEditorGui()  {}
MapEditorGui::~MapEditorGui() {}
