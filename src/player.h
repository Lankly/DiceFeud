#ifndef PLAYER_H
#define PLAYER_H

/************************
 * FORWARD DECLARATIONS *
 ************************/

class Board;


/************
 * INCLUDES *
 ************/

#include <random>
#include "color.h"
#include "display.h"


/**********
 * PLAYER *
 **********/

class Player
{

  public:

    /****************
     * CONSTRUCTORS *
     ****************/

    Player(Color c) : color_(c) { }


    /***************
     * DESTRUCTORS *
     ***************/

    virtual ~Player() = default;


    /***********
     * METHODS *
     ***********/

    /*** GETTERS ***/

    /**
     * Returns the color that this player has been assigned.
     */
    Color getColor() const { return color_; };


    /*** UTILITY ***/

    /**
     * Lets the player take their turn. If the player did not move because they
     * have lost the game, false is returned.
     *
     * @param {Board&} b The current state of the game's board.
     * @returns {bool} False if the player has lost the game.
     */
    virtual bool takeTurn(std::mt19937& rng, Display& d, Board& b) = 0;


  protected:

    Color color_;

};

#endif

