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
class PhysSimulation;

#include "globals.h"
#include "timer.h"
//! Represents the current state of the game.

//! This is the 'main object' for the game - the centrally
//! coordinated object which initializes, runs, and destroys
//! the entire game.  Anything of importance starts in this
//! class somewhere.
class GameState {
	protected:

		//! Random seed for the game
		int random_seed;
					
		//! Options parsed from the commandline
		GameOptions *options;
		
		//! The game's on-screen window
		Window *window;								
				
		//! Active input module
		BaseInput *input;

		//! The physics simulation
		PhysSimulation *simulation;

		//! Collection of all drawable objects
		//vector<Object*> objects;

		//! Collection of forces
		// vector<Force*> forces;
	
		//! Set this to TRUE to exit the game 
		bool exit_game;
			
		//! The state of the game engine
		enum state {
			STATE_NORMAL,
			STATE_PAUSED
		};
					
		int InitSystem();
		
		int InitTimers();
		int InitInput();
		// int InitSimulation();
		
		// void DestroyObjects();
		
		void Shutdown();

		void MainLoop();

		void Update();
		void Draw();
				
	public:
		int RunGame(GameOptions *_options);

 		void SetRandomSeed(int);
		int GetRandomSeed();

		BITMAP* GetDrawingSurface();
		bool GetKey(uint which_key);
		
		uint Width();
		uint Height();


		GameState();
		~GameState();
};

#endif
