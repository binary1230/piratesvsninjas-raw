#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <allegro.h>
#include <stdio.h>
#include <vector>

using namespace std;

class GameState;
class GameOptions;
class BaseInput;
class Window;
class GameMode;
class ResourceLoader;
class PhysSimulation;

#include "globals.h"
#include "timer.h"
#include "StdString.h"
#include "xmlParser.h"

//! Represents the current state of the game.

//! The MAIN class in the game - the centrally
//! coordinated object which initializes, runs, and destroys
//! the entire game.  Anything of importance starts in this
//! class somewhere.
class GameState {
	protected:	
		//! Holds options which were parsed from the command line
		GameOptions *options;
	
		//! Holds current game XML file
		XMLNode xGame;
		
		//! Loads things and knows about path handling
		ResourceLoader* resourceLoader;
		
		//! The game's on-screen window
		Window *window;								
				
		//! Active input module
		BaseInput *input;

		//! All open game modes (e.g. menu, simulation, etc)
		vector<GameMode*> modes;
		int currentModeIndex;		//! The index of the current mode.

		//! Points to one of the game modes in 'modes' array
		GameMode *currentMode;

		//! Points to the active physics simulation (if there is one)
		PhysSimulation* physSimulation;
		
		int InitSystem();								//! Initialize all game related stuff
		
		int InitTimers();								//! Init game timing
		int InitInput();								//! Init input subsystems

		//! Load master game config from an XML file, return an XMLNode which is
		//! the first game Mode's config we should now load.
		XMLNode LoadXMLConfig(CString xml_filename);
		
		//! Load a game mode from an XML config file
		int LoadGameMode(XMLNode);

		//! Load the default configuration (if no config file can be found)
		int LoadDefaultConfig();				
		
		//! Shutdown the game
		void Shutdown();

		//! Main loop to call once everything is initialized
		void MainLoop();

		//! Update the state of the game
		void Update();

		//! Draw the current state of the game
		void Draw();
		
		bool exit_game;	//! Set to TRUE to immediately shutdown the game 

		//! If a game has the same random seed, it will
		//! return the EXACT same sequence of random numbers.
		//! (useful for recording demos)
		int random_seed;
	
		//! Set to true to signal the end of the current mode
		bool end_current_mode;
		
		//! Exits the current mode and deletes it, free its memory
		//! Exits the game if it is the last mode left.
		void EndCurrentMode();

		//! Switches to the next mode if it exists. If not, does nothing.
		//! Returns false on error
		bool SwitchToNextMode();

		//! Switches the current mode to its parent if it exists. If it does
		//! not exist, exit the game.
		void SwitchToParentMode();
		
			
	public:
		//! Main function - run the game
		int RunGame(GameOptions *_options);

		//! Set the random seed value
 		void SetRandomSeed(int);

		//! Get the random seed value
		int GetRandomSeed() const;

		//! Return a surface which we can draw on
		BITMAP* GetDrawingSurface();

		//! Returns true if the key is being pressed, false otherwise
		bool GetKey(uint which_key) const;
	
		//! Return screen width
		uint ScreenWidth()  const;
		uint ScreenHeight() const;

		//! Get our resource loader.
		inline ResourceLoader* GetResourceLoader() const {return resourceLoader;};

		//! Unconditionally exit game
		void SignalExit() { exit_game = true; };
		
		//! The current mode calls this to signal it wants to end
		void SignalEndCurrentMode();

		//! Returns the currently active physics simulation (if there is one)
		PhysSimulation* GetPhysSimulation();

		GameState();
		~GameState();
};

#endif
