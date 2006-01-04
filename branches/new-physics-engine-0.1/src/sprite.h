#ifndef SPRITE_H
#define SPRITE_H

#include <allegro.h>

typedef struct {
	BITMAP* bmp;	
	
	int x_offset, y_offset;
	bool flip_x, flip_y;
} Sprite;

#endif // SPRITE_H
