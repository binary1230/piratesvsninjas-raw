// represents a window
#ifndef WINDOW_H
#define WINDOW_H

#include "globals.h"
#include "rect.h"

class GameState;
class Sprite;

// Modes for creating the display
// Listed in order from fastest, but nastier, to slowest, but nicest

// NO buffering (fastest, looks bad) NOT RECOMMENDED
#define MODE_NOBUFFERING			0

// Double buffering (reduces tearing) recommended default
#define MODE_DOUBLEBUFFERING	1

// Page flipping, (really reduces tearing)
#define MODE_PAGEFLIPPING			2

// Tripe buffering, (slowest, looks nicest) 
// ONLY use this on really fast machines where you can still
// see flickering or tearing.
#define MODE_TRIPLEBUFFERING	3

extern int screen_size_x;
extern int screen_size_y;

enum FADE_STATE {
	FADING_IN,	// alpha decreasing
	FADING_OUT, // alpha increasing
	FADED_OUT,	// alpha at 255 and staying there
	FADED_NONE	// alpha at 0 and staying there
};


//! The onscreen window
class GameWindow {

	DECLARE_SINGLETON_CLASS(GameWindow)

	protected:
		bool initialized;
		uint width, height;
		FONT* main_font;
		
		uint fade_rate;
		int fade_alpha;
		FADE_STATE fading_state;

		//! Init openGL stuff
		bool InitGL();

		//! Draw the fade
		void DrawFade();

		//! Update the fade
		void UpdateFade();

	public:
		int Init(	uint _width, uint _height, bool _fullscreen = 0,
							int _mode = MODE_DOUBLEBUFFERING);

		void SetTitle( const char* szTitle );
		void Shutdown();

		void Clear();
		void Flip();

		inline uint Width() {return width;};
		inline uint Height() {return height;};

		void DrawSprite(	Sprite* sprite, int x, int y, 
											bool flip_x=0, bool flip_y=0, 
											bool use_rotation=false, float rotate_angle=0.0f,
											GLuint alpha=255, bool bDrawBoundingBoxOnly=false);
	
		//! Draw text at specified XY
		void DrawText(int x, int y, CString text);

		//! Draw a solid rectange
		//! use makecol(r,g,b) for color
		void DrawRect(	_Rect &r, int col, 
										bool filled=false, int alpha=255);

		void DrawRect(	int x1, int y1, int x2, int y2, 
										int color, bool filled=false, int alpha=255);


		//! Draw a quad (includes LOTS of other params)
		void DrawQuad(	int x1, int y1, int x2, int y2, 
										int col1, int col2, int col3, int col4,
										bool filled, int alpha );

		//! Draw quad with specified vertical gradient, 
		//! which is interpolated between the two points
		void DrawBackgroundGradient(	int bottom_col, int top_col, 
																	int bottom_y, int top_y, 
																	int level_height );

		void SetClearColor(uint r, uint g, uint b);

		void BeginDrawing();
		void EndDrawing();

		//! Take a screenshot, call directly after Draw()
		//! Leave the filename NULL to guess an 
		//! automatically incrementing filename
		void Screenshot(char* filename = NULL);

		//! Start a fade out to black
		void FadeOut(int rate = 1);

		//! Start a fade in from black
		void FadeIn(int rate = 1);

		//! Set the screen to be faded in (e.g. no fade)
		void SetFadedIn();

		//! Set the screen to be faded out (e.g. black)
		void SetFadedOut();

		//! Draw window-specific stuff per frame
		//! (e.g. fades)
		//! Call LAST after all other drawing for the frame
		void Draw();

		//! Update window-specific stuff per frae
		//! (e.g. fades)
		//! Call LAST after all other drawing for the frame
		void Update();

		virtual ~GameWindow();

		friend class MapSaver;
};

#define WINDOW GameWindow::GetInstance()

#endif // WINDOW_H
