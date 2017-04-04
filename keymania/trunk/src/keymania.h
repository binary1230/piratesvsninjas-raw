#ifndef KEYMANIA_H
#define KEYMANIA_H

#include "stdafx.h"
#include "singleton.h"

class Sprite;

struct Note {
	uint staff_pos;	// 0 thru 7
	int xpos;
};

class Keymania {
	
	DECLARE_SINGLETON_CLASS(Keymania)

	protected:
		Sprite *sBack, *sClef, *sNote, *sStaff;
		CString currentText;
		uint lastTextTime;
		int textColor;
		int note_x;

		vector<Note> notes;

	public:
		bool Init();
		void Shutdown();

		void Draw();
		void Update();

		~Keymania();
};

#define KEYMANIA Keymania::GetInstance()

#endif // KEYMANIA_H
