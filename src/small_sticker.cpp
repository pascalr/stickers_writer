#include <SDL.h>
#include <stdio.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>

#include "small_hrcode_constants.h"

int mmToPx(double mm, int dpi=HRCODE_DPI) {
  return round(mm*dpi/25.4);
}

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
TTF_Font* gFont = NULL;

// Source:
// https://stackoverflow.com/questions/20233469/how-do-i-take-and-save-a-bmp-screenshot-in-sdl-2
// https://stackoverflow.com/questions/22315980/sdl2-c-taking-a-screenshot
bool saveScreenshotBMP(std::string filepath) {

  int w; int h;
  SDL_GetWindowSize(gWindow, &w, &h);

  SDL_Surface *sshot = SDL_CreateRGBSurface(0, w, h, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
  SDL_RenderReadPixels(gRenderer, NULL, SDL_PIXELFORMAT_ARGB8888, sshot->pixels, sshot->pitch);
  SDL_SaveBMP(sshot, filepath.c_str());
  SDL_FreeSurface(sshot);
  return true;
}

// Source:
// https://stackoverflow.com/questions/38334081/howto-draw-circles-arcs-and-vector-graphics-in-sdl
// https://stackoverflow.com/questions/41524497/c-sdl2-rendering-a-circle/41570334
void drawCircle(SDL_Renderer * renderer, int32_t cx, int32_t cy, int32_t radius)
{
  const int32_t diameter = (radius * 2);

  int32_t x = (radius - 1);
  int32_t y = 0;
  int32_t tx = 1;
  int32_t ty = 1;
  int32_t error = (tx - diameter);

  while (x >= y) {

    SDL_RenderDrawLine(renderer, cx - x, cy - y, cx + x, cy - y);
    SDL_RenderDrawLine(renderer, cx - y, cy + x, cx + y, cy + x);
    SDL_RenderDrawLine(renderer, cx - y, cy - x, cx + y, cy - x);
    SDL_RenderDrawLine(renderer, cx - x, cy + y, cx + x, cy + y);

    if (error <= 0) {
      ++y;
      error += ty;
      ty += 2;
    }

    if (error > 0) {
      --x;
      tx += 2;
      error += (tx - diameter);
    }
  }
}

class Sticker {
public:

  Sticker(int id) : id(id) {}
  ~Sticker() {
    //free();
  }

  void free() {
    if (textSurface != NULL) {SDL_FreeSurface( textSurface );}
    if (texture != NULL) {SDL_DestroyTexture( texture );}
    textSurface = NULL;
    texture = NULL;
  }

  void draw(int centerX, int centerY) {
  
    // Paint a white background
    //screenSurface = SDL_GetWindowSurface( gWindow );
  	//SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0xFF, 0xFF, 0xFF ) );
  
    // Add black text  
    SDL_Color textColor = { 0, 0, 0 };
  
    std::stringstream textValue; textValue << std::setfill('0') << std::setw(2) << id;
    textSurface = TTF_RenderText_Solid( gFont, textValue.str().c_str(), textColor );
    if( textSurface == NULL ) {
      printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() ); return;
    }
  
    //Create texture from surface pixels
    texture = SDL_CreateTextureFromSurface( gRenderer, textSurface );
  
    if( texture == NULL ) {
      printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() ); return;
    }
  
    //Get image dimensions
    int width = textSurface->w;
    int height = textSurface->h;
  			
    // Draw contour  
  	SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
    drawCircle(gRenderer, centerX, centerY, mmToPx(HRCODE_OUTER_DIA/2.0));
  	SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
    drawCircle(gRenderer, centerX, centerY, mmToPx(HRCODE_INNER_DIA/2.0));
  
    // Draw markers
  	SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
    drawCircle(gRenderer, centerX+mmToPx(HRCODE_MARKERS_INTERSPACE/2.0), centerY-mmToPx(HRCODE_MARKERS_DIST_Y_FROM_MIDDLE), mmToPx(HRCODE_MARKER_DIA/2.0));
    drawCircle(gRenderer, centerX-mmToPx(HRCODE_MARKERS_INTERSPACE/2.0), centerY-mmToPx(HRCODE_MARKERS_DIST_Y_FROM_MIDDLE), mmToPx(HRCODE_MARKER_DIA/2.0));
  	
    //Render text
    int x = centerX - (width / 2);
    int y = centerY - (HRCODE_FONT_SIZE/16);
    //int y = centerY - (height / 2) + (HRCODE_FONT_SIZE/16);
    //int y = centerY-mmToPx(HRCODE_MARKERS_DIST_Y_FROM_MIDDLE) - (height / 2) + (HRCODE_FONT_SIZE/16);
  	SDL_Rect renderQuad = { x, y, width, height };
  	SDL_RenderCopy( gRenderer, texture, NULL, &renderQuad );

    //int textY = y + height + mmToPx(HRCODE_TEXT_LINE_OFFSET);
    //// Draw lines
    ////SDL_RenderDrawLine(gRenderer, centerX-mmToPx(8), textY, centerX+mmToPx(8), textY);
    //SDL_RenderDrawLine(gRenderer, centerX-mmToPx(HRCODE_LINE_1_WIDTH/2.0), textY+mmToPx(HRCODE_LINE_INTERSPACE*1), centerX+mmToPx(HRCODE_LINE_1_WIDTH/2.0), textY+mmToPx(HRCODE_LINE_INTERSPACE*1));
    //SDL_RenderDrawLine(gRenderer, centerX-mmToPx(HRCODE_LINE_2_WIDTH/2.0), textY+mmToPx(HRCODE_LINE_INTERSPACE*2), centerX+mmToPx(HRCODE_LINE_2_WIDTH/2.0), textY+mmToPx(HRCODE_LINE_INTERSPACE*2));
    //SDL_RenderDrawLine(gRenderer, centerX-mmToPx(HRCODE_LINE_3_WIDTH/2.0), textY+mmToPx(HRCODE_LINE_INTERSPACE*3), centerX+mmToPx(HRCODE_LINE_3_WIDTH/2.0), textY+mmToPx(HRCODE_LINE_INTERSPACE*3));
    
    //free();
  }
   
  int id; 
  SDL_Texture* texture = NULL;
  SDL_Surface* textSurface = NULL;
};

bool init(int screenWidth, int screenHeight) {

	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() ); return false;
	}
	//Set texture filtering to linear
	//if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
	//{
	//	printf( "Warning: Linear texture filtering not enabled!" );
	//}

	gWindow = SDL_CreateWindow( "Stickers", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_SHOWN );
	if( gWindow == NULL )	{
		printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() ); return false;
	}

	//Create vsynced renderer for window
	gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
	if( gRenderer == NULL )	{
		printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() ); return false;
	}

	//Initialize renderer color
	SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

	//Initialize PNG loading
	//int imgFlags = IMG_INIT_PNG;
	//if( !( IMG_Init( imgFlags ) & imgFlags ) )
	//{
	//	printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() ); return false;
	//}

	 //Initialize SDL_ttf
	if( TTF_Init() == -1 ) {
		printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() ); return false;
	}

  gFont = TTF_OpenFont( HRCODE_FONT_NAME, HRCODE_FONT_SIZE );
  if ( gFont == NULL ) {
    printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() ); return false;
  }

  return true;
}

int main( int argc, const char* argv[] )
{

  // These variables should be given to the program as parameters.
  int nb_cols = 1; // HRCODE_NB_COLS
  int nb_rows = 1; // HRCODE_NB_ROWS
  int delay = 30;
  int id = 1;

  std::cout << "Usage: stickers_writer id delay" << std::endl;

  if ( argc > 1 ) { // First argument is id
    id = atoi( argv[1] );
  }
  if ( argc > 2 ) { // Second argument is delay
    delay = atoi( argv[2] );
  }

  int windowWidth = mmToPx(HRCODE_STICKER_WIDTH * nb_cols);
  int windowHeight = mmToPx(HRCODE_STICKER_WIDTH * nb_rows);

  std::vector<Sticker> stickers;

  if (init(windowWidth, windowHeight)) {
  
    //Clear screen
	  SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
	  SDL_RenderClear( gRenderer );

    for (int i = 0; i < nb_cols; i++) {
      for (int j = 0; j < nb_rows; j++) {
        int centerX = mmToPx(i * HRCODE_STICKER_WIDTH + (HRCODE_STICKER_WIDTH / 2.0));
        int centerY = mmToPx(j * HRCODE_STICKER_WIDTH + (HRCODE_STICKER_WIDTH / 2.0));
        Sticker sticker(id);
        sticker.draw(centerX, centerY);
        stickers.push_back(sticker);
        id++;
      }
    }
  	
    //Update screen
  	SDL_RenderPresent( gRenderer );

    SDL_Delay(delay);
	
    //SDL_Surface* screenSurface = SDL_GetWindowSurface( gWindow );
    //SDL_SaveBMP(screenSurface, OUT_FILE_NAME);
    saveScreenshotBMP(HRCODE_OUT_FILE_NAME);

    for (Sticker& sticker : stickers) {
      sticker.free(); // Not sure this is necesary because already done in destructor. I thought maybe it must be done before SDL_Quit. Anyway it does not hurt.
    }
  }
  	
  TTF_CloseFont( gFont );
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	SDL_Quit();

	return 0;
}
