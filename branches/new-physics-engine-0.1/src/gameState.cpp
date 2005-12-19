#include "gameState.h"

#include "gameOptions.h"
#include "input.h"
#include "inputLiveHandler.h"
#include "inputRecord.h"
#include "inputPlayback.h"
#include "window.h"
#include "physSimulation.h"
#include "gameMode.h"
#include "resourceLoader.h"

// XXX todo:  mode switching code is VERY hackish and bad.
//            need to fix that.

PhysSimulation* GameState::GetPhysSimulation() {
	return physSimulation;					
}

int GameState::LoadConfig(char* xml_filename) {
	return 0;
}

void GameState::SetRandomSeed(int val) { 
	random_seed = val; 
	srand(val); 
};

int GameState::GetRandomSeed() const { 
	return random_seed; 
};

bool GameState::GetKey(uint which_key) const	{ 
	return input->Key(which_key); 
};

BITMAP* GameState::GetDrawingSurface() { 
	return window->GetDrawingSurface(); 
};

uint GameState::ScreenWidth() const {
	return window->Width();
}

uint GameState::ScreenHeight() const {
	return window->Height();
}

void GameState::SignalEndCurrentMode() {
		end_current_mode = true;
}

//! Initialize game systems - main function

//! This is the first init function, it needs to initialize
//! Allegro, the window, the input subsystem, and the default game mode
//! BE CAREFUL, things need to be done IN ORDER here.
int GameState::InitSystem() {
				
		exit_game = false;
		end_current_mode = false;

		allegro_init();			// must be called FIRST
		InitTimers();				// must be called SECOND

		SetRandomSeed(42);
		
		resourceLoader = new ResourceLoader();
		if (!resourceLoader || resourceLoader->Init(this) < 0) {
			fprintf(stderr, "ERROR: InitSystem: failed to create resourceLoader!\n");
			return -1;
		}

		resourceLoader->AppendToSearchPath("../");

		if (LoadConfig("data/default.xml") == -1) {
			fprintf(stderr, "ERROR: InitSystem: failed to load configuration!\n");
			return -1;
		}
		
		window = new Window();
		if ( !window ||	window->Init(this, SCREEN_SIZE_X, SCREEN_SIZE_Y, 
										options->IsFullscreen(), options->GraphicsMode()) < 0 ) {
			fprintf(stderr, "ERROR: InitSystem: failed to init window!\n");
			return -1;
		}

		if (InitInput() == -1) {
			fprintf(stderr, "ERROR: InitSystem: failed to init input subsystem!\n");
			return -1;
		}

		// Initialize the default "game mode" (e.g. menu, simulation, etc)
		modes.resize(1);
		currentMode = modes[0] = physSimulation = new PhysSimulation();
		currentModeIndex = 0;
		if ( !modes[0] || modes[0]->Init(this) < 0) {
			fprintf(stderr, "ERROR: InitSystem: failed to init simulation!\n");
			return -1;
		}
					
		return 0;
}

//! Init input subsystems
// a little hackish... just a bit.
int GameState::InitInput() {
				
	// init the right kind of class based on
	// whether or not we are recording/playing back a demo
	if ( options->RecordDemo() )
		input = new InputRecord();	
	else if ( options->PlaybackDemo() )
		input = new InputPlayback();
	else
		input = new InputLive();
	
	if ( !input || !input->Init(this, options->GetDemoFilename()) < 0 ) {
		return -1;
	}

	return 0;
}

//! Init game timers
//! This MUST be called BEFORE any other allegro initializations.
int GameState::InitTimers() {
	install_timer();
	LOCK_VARIABLE(outstanding_updates);
	LOCK_FUNCTION((void*)Timer);
	return install_int_ex(Timer, BPS_TO_TIMER(FPS));
}

//! The 'main' function for the game

//! It takes a pointer to the game options (fullscreen/etc).
//! It initializes everything, and returns 0 if successful
//! or 1 on error.
int GameState::RunGame(GameOptions* _options) {
		
		options = _options;
		
		if (InitSystem() == -1) {
			fprintf(stderr, "ERROR: Failed to init game!\n");
			return -1;	
		} else {
	
			// XXX SHOULD NOT TEST option->is_xxx should TEST input->is_xxx()
			if (options->RecordDemo())
				input->BeginRecording();
			else if (options->PlaybackDemo())
				input->BeginPlayback();
			
			outstanding_updates = 0;	// reset our timer to 0.
			MainLoop();

			// XXX SHOULD NOT TEST option->is_xxx should TEST input->is_xxx()
			if (options->RecordDemo())
				input->EndRecording();
			else if (options->PlaybackDemo())
				input->EndPlayback();
		}
	
		Shutdown();

		return 0;
}

//! The Main Loop

//! The most important function.  It will make sure that the game 
//! is updating at the correct speed, and it will Draw everything
//! at the correct speed.
void GameState::MainLoop() {
				
	while (!exit_game) {

		// outstanding_updates is incremented once every 1/60th of a sec.
		// We may need to update more than once on slower computers
		// before we can draw, in order to keep the game the same speed
		// no matter the speed of the computer
		while (outstanding_updates > 0) {
			Update();	// mode signals handled here
			outstanding_updates--;
		}
		if (!exit_game) Draw();

		// wait for 1/60th sec to elapse (if we're on a fast computer)
		while (outstanding_updates <= 0);
  }
}

//! Update all game status
void GameState::Update() {
	input->Update();
	currentMode->Update();

	// see if we were signalled
	if (end_current_mode) {
		EndCurrentMode();
		if (exit_game)
			return;
	}
}

//! Draw the current mode
void GameState::Draw() {
	window->Clear();
	currentMode->Draw();
	window->Flip();
}

//! Shutdown the game

//! Clean up everything we allocated
void GameState::Shutdown() {

	int i, max = modes.size();
	for (i = 0; i < max; i++) {
		if (modes[i]) {
			modes[i]->Shutdown();
			delete modes[i];
			modes[i] = NULL;
		}
	}

	currentMode = NULL;
				
	if (input) {
		input->Shutdown();
		delete input;
		input = NULL;
	}
		
	// window destruction code must be LAST
	if (window) {
		window->Shutdown();
		delete window;
		window = NULL;
	}
		
	allegro_exit();
}

//! Exits the current mode and deletes it, free its memory
//! Exits the game if it is the last mode left.

// XXX stop whining and use iterators.  this is messier than it needs to be
// XXX probably need a modeList class to handle this messiness.
// XXX should we remove currentMode->next too?  that memory will be freed
//     on shutdown() though...
void GameState::EndCurrentMode() {
	if (currentMode) {
										
		// get the prev mode (will be NULL if this is the last mode in the game)
		GameMode* parent = currentMode->GetParentMode();
					
		// kill this mode
		currentMode->Shutdown();
		currentMode = parent;
					
		// delete + remove it from the modes list
		delete modes[currentModeIndex];
		modes.erase(	modes.begin()+currentModeIndex,
									modes.begin()+currentModeIndex + 1);
	}

	// if there is no more current mode, exit the game
	if (!currentMode) {
		SignalExit();
	}

	end_current_mode = false;
}

//! Switches to the next mode if it exists. If not, does nothing.
//! Returns false if there is no next mode (usually an error)
bool GameState::SwitchToNextMode() {
	if (currentMode) {
		GameMode* next = currentMode->GetNextMode();
		if (next)
			currentMode = next;
		else 
			return false;
	}
	return true;
}

//! Switches the current mode to its parent if it exists. If it does
//! not exist, exit the game.
void GameState::SwitchToParentMode() {
	if (currentMode) {
		GameMode* parent = currentMode->GetParentMode();
		if (parent)
			currentMode = parent;
		else 
			SignalExit();
	}
}

GameState::GameState() {
	window = NULL; 
	input = NULL;  
	currentMode = NULL; 
	physSimulation = NULL;
}
GameState::~GameState() {}
