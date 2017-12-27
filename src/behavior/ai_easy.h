#ifndef AI_EASY_H
#define AI_EASY_H

#include "../player.h"
#include "../display.h"

class AIEasy : public Player
{

  public:

    /****************
     * CONSTRUCTORS *
     ****************/

    AIEasy(Color c) : Player(c) { }


    /***********
     * METHODS *
     ***********/

    virtual bool takeTurn(std::mt19937& rng, Display& d, Board& b) override;

};

#endif

