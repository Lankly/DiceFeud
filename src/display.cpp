#include <ncurses.h>
#include <stdexcept>
#include "display.h"
#include "color.h"
#include "tile.h"

/******************************
 * HELPER FUNCTION PROTOTYPES *
 ******************************/

/**
 * Wraps the init_color ncurses macro. Exists just to cast Color::<COLOR> to a
 * short int.
 *
 * @param {Color} c The color to set the rgb values for.
 * @param {short int} r The red value.
 * @param {short int} g The green value.
 * @param {short int} b The blue value.
 */
inline void Init_color(Color c, short int r, short int g, short int b);


/**
 * Wraps the init_pair ncurses macro. Exists just to cast Color::<COLOR> and
 * ColorPair::<PAIR> to short ints.
 *
 * @param {ColorPair} c The color to set the rgb values for.
 * @param {short int} f The foreground color.
 * @param {Color} b The background Color.
 */
inline void Init_pair(ColorPair cp, short int f, Color b);


/*******************
 * IMPLEMENTATIONS *
 *******************/

Display::Display(size_t width, size_t height)
  : game_width(width), game_height(height)
{
  // Get terminal dimensions
  WIN = initscr();
  getmaxyx(stdscr, known_terminal_height, known_terminal_width);

  // Terminal too small
  if (known_terminal_width < MINIMUM_WIDTH
      || known_terminal_height < MINIMUM_HEIGHT) {
    std::runtime_error("Terminal size cannot be less than board size.");
  }

  if (width > known_terminal_width || height > known_terminal_height) {
    std::runtime_error("Board cannot be larger than terminal.");
  }

  // Terminal does not support color
  if (!has_colors()) {
    std::runtime_error("Terminal does not support color.");
  }

  // Clear screen
  refresh();

  start_color();        /* Turns on color mode */
  cbreak();             /* Disable line buffering */
  curs_set(0);          /* Make cursor invisible */
  noecho();             /* getch() will not print characters */
  keypad(stdscr, true); /* getch will get tokens correctly */
  timeout(500);         /* getch timeout of half a second */


  /*** Initialize NCurses color information ***/

  Init_color(Color::BLUE, 400, 700, 1000);
  Init_color(Color::CYAN, 400, 1000, 1000);
  Init_color(Color::GREEN, 400, 1000, 400);
  Init_color(Color::ORANGE, 1000, 700, 400);
  Init_color(Color::PINK, 1000, 400, 1000);
  Init_color(Color::PURPLE, 400, 400, 1000);
  Init_color(Color::RED, 1000, 400, 400);

  Init_pair(ColorPair::WHITE_BLUE,   COLOR_WHITE, Color::BLUE);
  Init_pair(ColorPair::WHITE_CYAN,   COLOR_WHITE, Color::CYAN);
  Init_pair(ColorPair::WHITE_GRAY,   COLOR_WHITE, Color::GRAY);
  Init_pair(ColorPair::WHITE_GREEN,  COLOR_WHITE, Color::GREEN);
  Init_pair(ColorPair::WHITE_ORANGE, COLOR_WHITE, Color::ORANGE);
  Init_pair(ColorPair::WHITE_PINK,   COLOR_WHITE, Color::PINK);
  Init_pair(ColorPair::WHITE_PURPLE, COLOR_WHITE, Color::PURPLE);
  Init_pair(ColorPair::WHITE_RED,    COLOR_WHITE, Color::RED);
  init_pair(static_cast<short int> (ColorPair::WHITE_BLACK)
    , COLOR_WHITE, COLOR_BLACK);
}


Display::~Display()
{
  endwin();
}


int Display::blinkUntilKeypress(std::vector<Tile::coord_t> coordinates) const
{
  bool blink_on = true;
  int ch = ERR;

  // User can't select anything, just hit enter for them
  if (coordinates.size() < 1) { return '\n'; }

  // Find any space in the given coordinates (all should be exactly the same)
  size_t x, y;
  decodeCoordinate(coordinates[0], x, y);

  // Mark what it originally looked like before we started blinking
  int orig_ch = mvinch(y, x);
  int blink_ch =
    COLOR_PAIR(static_cast<int>(ColorPair::WHITE_BLACK))
    | (static_cast<char>(orig_ch));

  // This loop will happen every half second
  do
  {
    drawValue(coordinates, blink_on ? blink_ch : orig_ch);

    // flip state
    blink_on = !blink_on;
  } while ((ch = getch()) == ERR);

  // Make sure we restore what it originally looked like
  drawValue(coordinates, orig_ch);

  return ch;
}


void Display::drawValue(
  std::vector<Tile::coord_t>& coordinates
  , int character)
  const
{
  // We only need to clear the screen if it was resized.
  size_t max_x, max_y;
  getmaxyx(stdscr, max_y, max_x);
  if (max_x != known_terminal_width || max_y != known_terminal_height) {
    clear();
    known_terminal_width = max_x;
    known_terminal_height = max_y;
  }

  // Print character to screen at coordinates
  for (size_t coord : coordinates)
  {
    size_t x, y;
    decodeCoordinate(coord, x, y);

    mvaddch(y, x, character);
  }
}


int Display::getDisplayableCharacter(Color c, char d)
{
  ColorPair cp = ColorHelpers::getCPWithColoredBackground(c);

  auto to_return = static_cast<int> (d);

  return d | COLOR_PAIR(cp);
}


void Display::decodeCoordinate(size_t coord, size_t& x, size_t& y) const
{
  x = coord % game_width;
  y = coord / game_width;

  // Now center them
  size_t center_x = known_terminal_width / 2;
  size_t center_y = known_terminal_height / 2;

  x += center_x - (game_width / 2);
  y += center_y - (game_height / 2);
}


/***********
 * HELPERS *
 ***********/

inline void Init_color(Color c, short int r, short int g, short int b)
{
  init_color(static_cast<short int>(c), r, g, b);
}


inline void Init_pair(ColorPair cp, short int f, Color b)
{
  init_pair(static_cast<short int>(cp), f, static_cast<short int>(b));
}

