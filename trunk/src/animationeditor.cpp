#include "stdafx.h"
#include "animationeditor.h"
#include "assetManager.h"
#include "gameState.h"
#include "input.h"
#include "window.h"

int AnimationEditorMode::Init( XMLNode xMode )
{
	m_pkCursorSprite = ASSETMANAGER->LoadSprite("cursor.png", true);
	assert(m_pkCursorSprite && "Unable to load cursor");

	return true;
}

void AnimationEditorMode::Shutdown()
{

}

void AnimationEditorMode::Draw()
{
	WINDOW->DrawText(10, 10, m_sFlashText);
	WINDOW->DrawSprite(m_pkCursorSprite, INPUT->MouseX(), INPUT->MouseY());
}

void AnimationEditorMode::Update()
{
	INPUT->Update();

	char s[][256] = {
		{ "hey" }, 
		{ "hi" }, 
		{ NULL }
	};

	if (INPUT->KeyOnce(GAMEKEY_EXIT))
		GAMESTATE->SignalEndCurrentMode();
}

AnimationEditorMode::AnimationEditorMode() :
m_pkCursorSprite(NULL), 
m_sFlashText("Sprite/Animation Editor")
{

}

AnimationEditorMode::~AnimationEditorMode()
{

}

void AnimationEditorMode::SetFlashText(char * format, ... )
{
	va_list args;
	const int bufsize = 4000;
	static char buffer[bufsize];

	va_start( args, format );
	vsnprintf( buffer, bufsize - 1, format, args );

	m_sFlashText = buffer;
}