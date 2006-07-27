#ifndef CREDITS_H
#define CREDITS_H

#include <allegro.h>
#include <stdio.h>

class GameState;

#include "gameBase.h"
#include "gameMode.h"
#include "StdString.h"
#include "xmlParser.h"

//! Represents a physical simulation (the main game levels)
class CreditsMode : public GameMode {
		protected:		
			BITMAP* credits_bmp;
			int scroll_speed;
			int scroll_offset;

		public:
			int Init(GameState*, XMLNode);
			void Shutdown();

			void Draw();
			void Update();

			CreditsMode();
			~CreditsMode();
};

#endif
