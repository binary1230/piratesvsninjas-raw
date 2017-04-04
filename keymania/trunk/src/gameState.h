#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "stdafx.h"

class GameState;
class GameOptions;
class BaseInput;
class AssetManager;
class GameSound;

#include "globals.h"
#include "timer.h"
#include "singleton.h"

//! Represents the current state of the game.

//! The MAIN class in the game - the centrally
//! coordinated object which initializes, runs, and destroys
//! the entire game.  Anything of importance starts in this
//! class somewhere.
class GameState {
	DECLARE_SINGLETON_CLASS(GameState)

	protected:	
		//! Holds options which were parsed from the command line
		GameOptions *options;
	
		//! Initialize all game related stuff
		int InitSystem();								
		
		//! Init main game (keymania)
		bool InitMainGame();

		//! Init basic allegro stuff
		int InitAllegro();

		//! Init game timing
		int InitTimers();
		
		//! Init input subsystems
		int InitInput();								

		//! Init sound system
		int InitSound();

		//! Init the game network
		int InitNetwork();
		
		//! Shutdown the game
		void Shutdown();

		//! Main loop to call once everything is initialized
		void MainLoop();

		//! Update the state of the game
		void Update();

		//! Draw the current state of the game
		void Draw();

		//! Output the total running time
		void OutputTotalRunningTime();
		
		//! Set to TRUE to immediately shutdown the game 
		bool exit_game;	
	
		//! Flag to enable DEBUG pause toggling
		bool debug_pause_toggle;

		//! If a game has the same random seed, it will
		//! return the EXACT same sequence of random numbers.
		//! (useful for recording demos)
		int random_seed;
	
		//! Set to true if we are playing back a demo
		bool is_playing_back_demo;
					
	public:
		//! Main function - run the game
		int RunGame(GameOptions *_options);

		//! Set the random seed value
 		void SetRandomSeed(int);
		
		//! Get the random seed value
		int GetRandomSeed() const;

		//! Returns true if we are playing back a demo
		bool IsPlayingBackDemo() {return is_playing_back_demo;};

		//! Unconditionally exit game
		void SignalGameExit();
		
		//! Gets the game options
		inline GameOptions* GetGameOptions() {return options;};

		~GameState();
};

#define GAMESTATE GameState::GetInstance()

#endif
