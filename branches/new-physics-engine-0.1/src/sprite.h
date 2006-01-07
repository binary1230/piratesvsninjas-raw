#ifndef SPRITE_H
#define SPRITE_H

#include <allegro.h>

class Sprite {
	public:
		BITMAP* bmp;	
	
		int x_offset, y_offset;
		bool flip_x, flip_y;

		bool bitmap_is_deleteable;
};

#endif // SPRITE_H
