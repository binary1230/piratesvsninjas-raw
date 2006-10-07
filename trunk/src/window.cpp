#include "window.h"

#include "gameState.h"
#include "globals.h"
#include "sprite.h"
#include "gameOptions.h"

// #define DEFAULT_COLOR_DEPTH 16
#define DEFAULT_COLOR_DEPTH desktop_color_depth()

// leave this undefined usually.
// #define ALTERNATE_GFX_MODE GFX_XDGA2

DECLARE_SINGLETON(Window)

int screen_size_x = DEFAULT_SCREEN_SIZE_X;
int screen_size_y = DEFAULT_SCREEN_SIZE_Y;

void Window::Screenshot(char* filename) {
	CString file;
	static int screenshot_num = 0;

	if (filename) {
		file = filename;
	} else {
		// Make one up.
		file.Format("ninjas-screenshot%i.png", screenshot_num++);
	}

	fprintf(stderr, " -- saving screenshot '%s'\n", file.c_str());

	save_bitmap(file.c_str(), GetDrawingSurface(), NULL);
}

void Window::BlitBitmap(  BITMAP* bmp, int source_x, int source_y, 
									int dest_x, int dest_y, int width, int height) {

	// if its position is offscreen, don't draw it.
	if (dest_x + bmp->w < 0 || dest_x >= (int)width ||
			dest_y + bmp->h < 0 || dest_y >= (int)height )
			return;

	blit(bmp, drawing_surface, source_x, source_y, dest_x, dest_y, width, height);
}


// public function
void Window::DrawBitmap(	BITMAP* bmp, int x, int y, 
													bool flip_x, bool flip_y, 
													bool use_alpha, int alpha) {

	// if its position is offscreen, don't draw it.
	if (x + bmp->w < 0 || x >= (int)width || 
			y + bmp->h < 0 || y >= (int)height ) 
			return;

	// Draw the bitmap
	DrawBitmapAt(bmp, x, y, flip_x, flip_y, use_alpha, alpha);
}

void Window::DrawRect(_Rect &r, int col) {
	rect(	drawing_surface, 
				(int)r.getx1(), 
				(int)r.gety1(), 
				(int)r.getx2(), 
				(int)r.gety2(), 
				col);
}

void Window::DrawRect(int x1, int y1, int x2, int y2, int col) {
	rect(drawing_surface, x1, y1, x2, y2, col);
}

void Window::DrawFillRect(int x1, int y1, int x2, int y2, int col) {
	rectfill(drawing_surface, x1, y1, x2, y2, col);
}

// HACK HACK HACK - get this from somewhere!!
#define FONT_HEIGHT 10

static void StringSplit(CString str, CString delim, vector<CString> &results) {
	uint cutAt;
	results.clear();
	while( (cutAt = str.find_first_of(delim)) != str.npos ) {
		if(cutAt > 0) {
			results.push_back(str.substr(0,cutAt));
		}

		str = str.substr(cutAt+1);
	}
	
	if(str.length() > 0)	{
		results.push_back(str);
	}
}

void Window::DrawText(int x, int y, CString text) {

	vector<CString> lines;
	StringSplit(text, "|", lines);
	int i, max = lines.size();

	int _x = x;
	int _y = y;

	for (i = 0; i < max; i++) {
		textout_ex(	drawing_surface, font, lines[i].c_str(), 
								_x, _y, makecol(255,255,255), -1);
		_y += FONT_HEIGHT;
	}
}

// private internal method only
void Window::DrawBitmapAt(	BITMAP* bmp, int x, int y, 
														bool flip_x, bool flip_y, 
														bool use_alpha, int alpha) {
	
	// Note: transparency support is still weird.

	if (!use_alpha) {
		if (!flip_x) 
			draw_sprite(drawing_surface, bmp, x, y);
		else
			draw_sprite_h_flip(drawing_surface, bmp, x, y);
	} else {
		// XXX DOES NOT FLIP ALPHA-BLENDED SPRITES RIGHT NOW
		// set_trans_blender(256,256,256,alpha);
		set_alpha_blender();
		//set_trans_blender(0,256,256,256);
		// blit(bmp, drawing_surface, 0, 0, x, y, bmp->w, bmp->h);
		draw_trans_sprite(drawing_surface, bmp, x, y);
		set_trans_blender(256,256,256,256);
	}
}

// public function
// NOTE: two things can tell the sprite to flip itself, either
// calling this function with flip_x/y true, or having the sprite
// set its flip_x/y flag to true.  We may have to flip it twice (e.g.
// NOT flip it at all) 
//
// Holy sweetness. Remember that '^' is XOR, and XOR rocks.
void Window::DrawSprite(	Sprite* sprite, int x, int y, 
													bool flip_x, bool flip_y, int alpha) {
	DrawBitmap( sprite->bmp, 
							x + sprite->x_offset, y + sprite->y_offset, 
							sprite->flip_x ^ flip_x, sprite->flip_y ^ flip_y, 
							sprite->use_alpha, alpha);
}

int Window::Init( uint _width, uint _height, 
									bool _fullscreen, int _mode) {
	
	int depth = DEFAULT_COLOR_DEPTH;
	int gfx_mode;
	int vheight;
	
	width = _width;
	height = _height;
	mode = _mode;
	clear_color = 0;

	set_color_depth(depth);

	// Special case: We won't be drawing _anything_
	if (!GAMESTATE->GetGameOptions()->DrawGraphics() ) {
		fprintf(stderr, "Window: DISABLING ALL GRAPHICS\n");
		set_gfx_mode(GFX_TEXT, 320, 240, 0, 0);
		mode = MODE_NOBUFFERING;
		initialized = true;
		return 0;
	}
	
	if (_fullscreen)
			gfx_mode = GFX_AUTODETECT_FULLSCREEN;
	else
			gfx_mode = GFX_AUTODETECT_WINDOWED;

#	ifdef ALTERNATE_GFX_MODE
	gfx_mode = ALTERNATE_GFX_MODE;
# endif // ALTERNATE_GFX_MODE

	if (mode == MODE_PAGEFLIPPING || mode == MODE_TRIPLEBUFFERING)
		vheight = height * 2;
	else 
		vheight = 0;

	if (get_color_depth() != depth)
		fprintf(stderr, "window: Warning: Asked for %i-bit color mode, got %i-bit instead.\n", depth, get_color_depth());
									
	if (set_gfx_mode(gfx_mode, width, height, 0, vheight) != 0) {
		fprintf(stderr, 
						"window: Can't set graphics mode! (%i, %i, fullscreen = %i) \n"
						"Try setting a different graphics mode or try non-fullscreen\n"
						"Allegro error says: '%s'\n",
						width, height, _fullscreen, allegro_error);
		return -1;
	}
	
	set_window_title(VERSION_STRING);
	
	if (mode == MODE_DOUBLEBUFFERING) {
		clear_to_color(screen, clear_color);
		
		// initialize back buffering
		backbuf = create_bitmap(width, height);
		if (!backbuf) {
			fprintf(stderr, "window: can't create back buffer!\n");
			return -2;
		}
		
		clear_to_color(backbuf, clear_color);
		drawing_surface = backbuf;
		
	} else if (mode == MODE_PAGEFLIPPING) {
	
		// set up page flipping
		page[0] = create_video_bitmap(width, height);
		page[1] = create_video_bitmap(width, height);

		if ((!page[0]) || (!page[1])) {
			fprintf(stderr, "window: can't setup page flipping!\n");
			return -2;
		}

		active_page = 0;
		drawing_surface = page[active_page];
		
	} else if (mode == MODE_NOBUFFERING) {

		clear_to_color(screen, clear_color);
		drawing_surface = screen;
		
	} else {
		
		fprintf(stderr, "window: specified mode not supported!\n");
		return -2;
		
	}

	initialized = true;

	return 0;
}

void Window::Clear() {
	switch (mode) {
		case MODE_PAGEFLIPPING:
			clear_to_color(page[active_page], clear_color);
			break;
		case MODE_DOUBLEBUFFERING:
			clear_to_color(backbuf, clear_color);
			break;
		case MODE_NOBUFFERING:
			clear_to_color(screen, clear_color);
			break;
		default:
			fprintf(stderr, "ERROR: Unkown buffering mode %u\n", mode);
	}
}

void Window::BeginDrawing() {
	acquire_screen();
}

void Window::EndDrawing() {
	release_screen();
}

// draws the backbuffer to the screen and erases the backbuffer
void Window::Flip() {
	if (mode == MODE_PAGEFLIPPING) {
		show_video_bitmap(page[active_page]);
	
		if (active_page == 1)
			active_page = 0;
		else 
			active_page = 1;
	
		drawing_surface = page[active_page];
	} else if (mode == MODE_DOUBLEBUFFERING) {
		vsync();
		blit(backbuf, screen, 0, 0, 0, 0, width, height);
	} else if (mode == MODE_NOBUFFERING) {
		// do nothing	
	}
}

void Window::Shutdown() {
	if (!initialized)
			return;

	if (mode == MODE_PAGEFLIPPING) {
		destroy_bitmap(page[0]);
		destroy_bitmap(page[1]);
	} else if (mode == MODE_DOUBLEBUFFERING) {
		destroy_bitmap(backbuf);
	}
	
	drawing_surface = NULL;
	initialized = false;
}

Window::Window() : initialized(false), drawing_surface(NULL) {
	page[0] = NULL;
	page[1] = NULL;
}
Window::~Window() {}
