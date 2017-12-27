#include <list>
#include <random>
#include <vector>
#include "ai_easy.h"
#include "../board.h"
#include "../display.h"
#include "../tile.h"

bool AIEasy::takeTurn(std::mt19937& rng, Display& d, Board& b)
{
  std::list<Board::tile_iterator> my_tiles =
    b.filterForFrontlineTiles(b.getTilesByColor(getColor()));

  if (my_tiles.size() == 0) {
    return false;
  }

  // Select one at random
  std::vector<size_t> weights (my_tiles.size(), 1);
  std::discrete_distribution<size_t> distribution (
    std::begin(weights)
    , std::end(weights));
  Board::tile_iterator my_selection =
    *std::next(std::begin(my_tiles), distribution(rng));


  // Get possible defending tiles
  std::list<Board::tile_iterator> enemy_tiles =
    Board::filterColoredTiles(
      (*my_selection).getColor()
      , b.getAdjacentTiles(*my_selection));


  // Select enemy tile at random
  weights.resize(enemy_tiles.size());
  std::discrete_distribution<size_t> enemy_distribution (
    std::begin(weights)
    , std::end(weights));
  Board::tile_iterator enemy_selection =
    *std::next(std::begin(enemy_tiles), enemy_distribution(rng));

  // Fight
  b.fight(rng, (*my_selection).getId(), (*enemy_selection).getId());

  return true;
}

