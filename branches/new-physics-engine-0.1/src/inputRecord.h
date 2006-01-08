/* Live Input 
 *
 * Repsonsible for handling 'live' input (keyboard, mouse, etc)
 */

#ifndef InputRecordHANDLER_H
#define InputRecordHANDLER_H

#include <allegro.h>
#include <stdio.h>
#include <vector>
using namespace std;

class InputRecord;

#include "input.h"

//! Live input 

//! Reads input devices in realtime (used for normal game play)
class InputRecord : public BaseInput {
	
	protected:
		//! The current demo file being written
		FILE* demofile;
		
		//! The last state of the keys
		vector<int> old_key;
		
		//! The frame count we are on [updated by Update()]
		unsigned long frame_counter;

		bool InitRecorder(char* filename);
	
	public:
		int Init(GameState* _game_state, char* _demo_file = NULL);
		void Shutdown();

		void Update();
		
		void BeginRecording();
		void EndRecording();

		void BeginPlayback()	{NotSupported();}
		void EndPlayback()	{NotSupported();}

		InputRecord();
		~InputRecord();
};

#endif // InputRecordHANDLER_H
