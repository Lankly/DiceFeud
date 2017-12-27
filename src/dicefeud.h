#ifndef DICEFEUD_H
#define DICEFEUD_H

#include <deque>
#include <memory>
#include <random>
#include "board.h"
#include "display.h"

class DiceFeud
{

  public:

    /****************
     * CONSTRUCTORS *
     ****************/

    DiceFeud(std::mt19937& rng, Display& d, size_t numPlayers);


    /***********
     * METHODS *
     ***********/

    /**
     * Runs this game. If the player wishes to continue at the end of the game,
     * true will be returned.
     *
     * @param {std::mt19937&} rng Provides randomness.
     * @returns {bool} True if the player wishes to continue playing.
     */
    bool play(std::mt19937& rng);


  private:

    /**************
     * PROPERTIES *
     **************/

    Board board_;
    Display& d_;
    std::deque<std::unique_ptr<Player>> players_;


    /***********
     * METHODS *
     ***********/

    /**
     * Shows the Game Over screen, indicating whether the user won or lost, and
     * prompts the user whether they would like to start a new game or not.
     *
     * @returns {bool} True if the player wishes to continue.
     */
    bool gameOver();

};

#endif

