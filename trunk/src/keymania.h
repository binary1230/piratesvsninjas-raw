#ifndef KEYMANIA_H
#define KEYMANIA_H

#include "stdafx.h"
#include "singleton.h"

class Sprite;

class Keymania {
	
	DECLARE_SINGLETON_CLASS(Keymania)

	protected:
		Sprite* test;

	public:
		bool Init();
		void Shutdown();

		void Draw();
		void Update();

		~Keymania();
};

#define KEYMANIA Keymania::GetInstance()

#endif // KEYMANIA_H
