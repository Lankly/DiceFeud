#ifndef AI_MEDIUM_H
#define AI_MEDIUM_H

#include "../player.h"
#include "../display.h"

class AIMedium : public Player
{

  public:

    /****************
     * CONSTRUCTORS *
     ****************/

    AIMedium(Color c) : Player(c) { }


    /***********
     * METHODS *
     ***********/

    virtual bool takeTurn(std::mt19937& rng, Display& d, Board& b) override;

};

#endif

