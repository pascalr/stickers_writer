/*This source code copyrighted by Lazy Foo' Productions (2004-2020)
and may not be redistributed without written permission.*/

//Using SDL and standard IO
#include <SDL.h>
#include <stdio.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>

#include "hrcode_constants.h"

int mmToPx(double mm, int dpi=DPI) {
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
//    SDL_Surface* saveSurface = NULL;
//    SDL_Surface* infoSurface = NULL;
//    infoSurface = SDL_GetWindowSurface(SDLWindow);
//    if (infoSurface == NULL) {
//        std::cerr << "Failed to create info surface from window in saveScreenshotBMP(string), SDL_GetError() - " << SDL_GetError() << "\n";
//    } else {
//        unsigned char * pixels = new (std::nothrow) unsigned char[infoSurface->w * infoSurface->h * infoSurface->format->BytesPerPixel];
//        if (pixels == 0) {
//            std::cerr << "Unable to allocate memory for screenshot pixel data buffer!\n";
//            return false;
//        } else {
//            if (SDL_RenderReadPixels(SDLRenderer, &infoSurface->clip_rect, infoSurface->format->format, pixels, infoSurface->w * infoSurface->format->BytesPerPixel) != 0) {
//                std::cerr << "Failed to read pixel data from SDL_Renderer object. SDL_GetError() - " << SDL_GetError() << "\n";
//                delete[] pixels;
//                return false;
//            } else {
//                saveSurface = SDL_CreateRGBSurfaceFrom(pixels, infoSurface->w, infoSurface->h, infoSurface->format->BitsPerPixel, infoSurface->w * infoSurface->format->BytesPerPixel, infoSurface->format->Rmask, infoSurface->format->Gmask, infoSurface->format->Bmask, infoSurface->format->Amask);
//                if (saveSurface == NULL) {
//                    std::cerr << "Couldn't create SDL_Surface from renderer pixel data. SDL_GetError() - " << SDL_GetError() << "\n";
//                    delete[] pixels;
//                    return false;
//                }
//                SDL_SaveBMP(saveSurface, filepath.c_str());
//                SDL_FreeSurface(saveSurface);
//                saveSurface = NULL;
//            }
//            delete[] pixels;
//        }
//        SDL_FreeSurface(infoSurface);
//        infoSurface = NULL;
//    }
//    return true;
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
  
    std::stringstream textValue; textValue << std::setfill('0') << std::setw(3) << id;
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
    drawCircle(gRenderer, centerX, centerY, mmToPx(OUTER_DIA/2.0));
  	SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
    drawCircle(gRenderer, centerX, centerY, mmToPx(INNER_DIA/2.0));
  
    // Draw markers
  	SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
    drawCircle(gRenderer, centerX+mmToPx(MARKERS_INTERSPACE/2.0), centerY-mmToPx(MARKERS_DIST_Y_FROM_MIDDLE), mmToPx(MARKER_DIA/2.0));
    drawCircle(gRenderer, centerX-mmToPx(MARKERS_INTERSPACE/2.0), centerY-mmToPx(MARKERS_DIST_Y_FROM_MIDDLE), mmToPx(MARKER_DIA/2.0));
  	
    //Render text
    int x = centerX - (width / 2);
    int y = centerY-mmToPx(MARKERS_DIST_Y_FROM_MIDDLE) - (height / 2) + (FONT_SIZE/16);
  	SDL_Rect renderQuad = { x, y, width, height };
  	SDL_RenderCopy( gRenderer, texture, NULL, &renderQuad );

    int textY = y + height + mmToPx(TEXT_LINE_OFFSET);

    // Draw lines
    //SDL_RenderDrawLine(gRenderer, centerX-mmToPx(8), textY, centerX+mmToPx(8), textY);
    SDL_RenderDrawLine(gRenderer, centerX-mmToPx(LINE_1_WIDTH/2.0), textY+mmToPx(LINE_INTERSPACE*1), centerX+mmToPx(LINE_1_WIDTH/2.0), textY+mmToPx(LINE_INTERSPACE*1));
    SDL_RenderDrawLine(gRenderer, centerX-mmToPx(LINE_2_WIDTH/2.0), textY+mmToPx(LINE_INTERSPACE*2), centerX+mmToPx(LINE_2_WIDTH/2.0), textY+mmToPx(LINE_INTERSPACE*2));
    SDL_RenderDrawLine(gRenderer, centerX-mmToPx(LINE_3_WIDTH/2.0), textY+mmToPx(LINE_INTERSPACE*3), centerX+mmToPx(LINE_3_WIDTH/2.0), textY+mmToPx(LINE_INTERSPACE*3));
    
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

  gFont = TTF_OpenFont( FONT_NAME, FONT_SIZE );
  if ( gFont == NULL ) {
    printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() ); return false;
  }

  return true;
}

int main( int argc, char* args[] )
{
  int windowWidth = mmToPx(STICKER_WIDTH * NB_COLS);
  int windowHeight = mmToPx(STICKER_WIDTH * NB_ROWS);

  std::vector<Sticker> stickers;

  if (init(windowWidth, windowHeight)) {
  
    //Clear screen
	  SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
	  SDL_RenderClear( gRenderer );

    int id = 1;
    for (int i = 0; i < NB_COLS; i++) {
      for (int j = 0; j < NB_ROWS; j++) {
        int centerX = mmToPx(i * STICKER_WIDTH + (STICKER_WIDTH / 2.0));
        int centerY = mmToPx(j * STICKER_WIDTH + (STICKER_WIDTH / 2.0));
        Sticker sticker(id);
        sticker.draw(centerX, centerY);
        stickers.push_back(sticker);
        id++;
      }
    }
  	
    //Update screen
  	SDL_RenderPresent( gRenderer );

    SDL_Delay( 5000 );
	
    //SDL_Surface* screenSurface = SDL_GetWindowSurface( gWindow );
    //SDL_SaveBMP(screenSurface, OUT_FILE_NAME);
    saveScreenshotBMP(OUT_FILE_NAME);

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
