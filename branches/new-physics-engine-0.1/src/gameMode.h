#ifndef GAMEMODE_H
#define GAMEMODE_H

class GameState;

#include "gameBase.h"
#include "xmlParser.h"

//! GameMode class
//! Base class for all "modes" of the game
//! For example, a level is a "mode", the menu screen is a "mode"
//! and the bonus stage is a "mode"

//! This class doesn't do anything except act as an interface to enforce
//! the fact that all modes need to be able to Update() and Draw(), among
//! other things.

class GameMode : public GameBase {
		protected:
			GameMode* next;		// mode to go to on gameState->NextMode()
			GameMode* parent; // mode to go to on gameState->ExitMode()

		public:
			virtual int Init(GameState*, XMLNode) = 0;
			virtual void Shutdown() = 0;

			virtual void Draw() = 0;
			virtual void Update() = 0;

			inline void SetNextMode(GameMode* n) {next = n;};
			inline GameMode* GetNextMode() {return next;};
			
			inline void SetParentMode(GameMode* p) {parent = p;};
			inline GameMode* GetParentMode() {return parent;};

			GameMode();
			virtual ~GameMode();
};

#endif // GAMEMODE_H
