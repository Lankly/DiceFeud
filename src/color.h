#ifndef COLOR_H
#define COLOR_H

enum class Color : short int
{
  BLUE = 7 // Start here to not interfere with ncurses built-in colors
  , CYAN
  , GRAY
  , GREEN
  , ORANGE
  , PINK
  , PURPLE
  , RED
};

// This should contain the same colors in the same order as Color.
enum class ColorPair
{
  WHITE_BLUE = static_cast<int> (Color::BLUE)
  , WHITE_CYAN
  , WHITE_GRAY
  , WHITE_GREEN
  , WHITE_ORANGE
  , WHITE_PINK
  , WHITE_PURPLE
  , WHITE_RED
  , WHITE_BLACK
};

class ColorHelpers
{

  public:

    static ColorPair getCPWithColoredBackground (Color c)
    {
      return static_cast<ColorPair> (c);
    }

};

#endif

