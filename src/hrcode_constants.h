#ifndef _HRCODE_CONSTANTS_H
#define _HRCODE_CONSTANTS_H

// Variables that you can change
const int NB_ROWS = 2;
const int NB_COLS = 2;

const char* OUT_FILE_NAME = "out.bmp";

const int DPI = 300; // Dots per inch (pixels per inch)

//const char* FONT_NAME = "fonts/monospace/MonospaceBold.ttf";
//const char* FONT_NAME = "fonts/monospace/Monospace.ttf";
//const char* FONT_NAME = "fonts/cmu/cmunorm.ttf";
//const char* FONT_NAME = "fonts/Courier Prime/Courier Prime Bold.ttf";
//const char* FONT_NAME = "fonts/Courier Prime/Courier Prime.ttf";
//const char* FONT_NAME = "fonts/Courier Prime Sans/Courier Prime Sans Bold.ttf";
//const char* FONT_NAME = "fonts/Courier Prime Sans/Courier Prime Sans.ttf";
//const char* FONT_NAME = "fonts/droid-sans-mono/DroidSansMono.ttf";
const char* FONT_NAME = "fonts/LiberationMono-Regular.ttf";
//const char* FONT_NAME = "fonts/LiberationMono-Bold.ttf";
const int FONT_SIZE = 64;
//const int TEXT_Y_OFFSET = 4;

const double SPACE_BETWEEN_STICKERS = 4; // mm
const double OUTER_DIA = 36; // mm
const double INNER_DIA = 32; // mm
const double STICKER_WIDTH = OUTER_DIA + SPACE_BETWEEN_STICKERS; // mm

const double MARKER_DIA = 4.4; // mm
const double MARKERS_INTERSPACE = 16.5; // mm
const double MARKERS_DIST_FROM_MIDDLE = 9.3; // mm

const double CHAR_HEIGHT = 4; // mm

const double LINE_1_WIDTH = 25; // mm
const double LINE_2_WIDTH = 25; // mm
const double LINE_3_WIDTH = 15; // mm

const double TEXT_LINE_OFFSET = -1.5; // mm
const double LINE_INTERSPACE = 6.8; // mm
//const double LINE_1_HEIGHT = 1; // mm
//const double LINE_2_HEIGHT = -6; // mm
//const double LINE_3_HEIGHT = -13; // mm

#endif
