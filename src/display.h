#ifndef DISPLAY_H
#define DISPLAY_H

#include <ncurses.h>
#include <string>
#include <vector>
#include "color.h"
#include "tile.h"

class Display
{

  public:

    /******************************
     * CONSTRUCTORS & DESTRUCTORS *
     ******************************/

    Display(size_t width, size_t height);

    ~Display();


    /***********
     * METHODS *
     ***********/

    /**
     * Blinks the given coordinates until the user presses a key and returns it.
     * It is assumed that every coordinate will have the same character
     * on-screen.
     *
     * @param {std::vector<size_t>} coordinates The coordinates to blink.
     * @returns {int} The key pressed by the user.
     */
    int blinkUntilKeypress(std::vector<Tile::coord_t> coordinates) const;

    /**
     * Removes any message currently in the message bar.
     */
    void clearMessageBar();

    /**
     * Turns an index into an array into an index into a matrix, where the width
     * of that matrix is the known game_width. Additionally, centers it on the
     * screen according to the known game dimensions.
     *
     * @param {size_t} coord The coordinate to decode.
     * @param {size_t&} x Where the result's x-value is stored.
     * @param {size_t&} y Where the result's y-value is stored.
     */
    void decodeCoordinate(size_t coord, size_t& x, size_t& y) const;

    /**
     * Draws the same character to the screen at every coordinate given.
     *
     * @param {std::vector<size_t>} coordinates The coordinates to draw the
     * character to.
     * @param {int} character An ncurses-printable character.
     */
    void drawValue(
      std::vector<Tile::coord_t>& coordinates
      , int character)
      const;

    /**
     * Returns a character that ncurses can use.
     *
     * @param {Color} c The color that this character should be.
     * @param {char} d The character to display.
     * @returns {int} An ncurses-printable character.
     */
    static int getDisplayableCharacter(Color c, char d);

    /**
     * Prints a message below the game board.
     *
     * @param {std::string|const char *} msg The message to print.
     */
    void printMessage(std::string msg);
    void printMessage(const char* msg);


    /**************
     * PROPERTIES *
     **************/

    static const int UP = KEY_UP;
    static const int DOWN = KEY_DOWN;
    static const int LEFT = KEY_LEFT;
    static const int RIGHT = KEY_RIGHT;
    static const size_t MINIMUM_WIDTH = 80;
    static const size_t MINIMUM_HEIGHT = 24;


  private:

    /**************
     * PROPERTIES *
     **************/

    /*** CLASS PROPERTIES ***/

    mutable size_t known_terminal_width = 0
    , known_terminal_height = 0;


    /*** INSTANCE PROPERTIES ***/

    size_t game_width, game_height;
    WINDOW* WIN;

};

#endif

