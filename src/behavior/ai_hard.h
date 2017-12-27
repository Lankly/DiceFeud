#ifndef AI_HARD_H
#define AI_HARD_H

#include "../player.h"
#include "../display.h"

class AIHard : public Player
{

  public:

    /****************
     * CONSTRUCTORS *
     ****************/

    AIHard(Color c) : Player(c) { }


    /***********
     * METHODS *
     ***********/

    virtual bool takeTurn(std::mt19937& rng, Display& d, Board& b) override;

};

#endif

