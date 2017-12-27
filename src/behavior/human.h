#ifndef HUMAN_H
#define HUMAN_H

#include "../player.h"
#include "../display.h"

using std::vector;

class Human : public Player
{

  public:

    /****************
     * CONSTRUCTORS *
     ****************/

    Human(Color c)
      : Player(c)
    { }


    /***********
     * METHODS *
     ***********/

    virtual bool takeTurn(std::mt19937& rng, Display& d, Board& b) override;

};

#endif

