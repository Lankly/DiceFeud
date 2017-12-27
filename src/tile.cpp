/************
 * INCLUDES *
 ************/

#include <algorithm>
#include "tile.h"


/*******************************
 * STATIC PROPERTY DEFINITIONS *
 *******************************/

const size_t Tile::MAX_DICE_PER_TILE = 8;


/*******************
 * IMPLEMENTATIONS *
 *******************/

void Tile::setNumDice(const size_t newNum)
{
  num_dice_ = std::min(MAX_DICE_PER_TILE, newNum);
}

