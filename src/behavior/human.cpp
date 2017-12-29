#include <stdexcept>
#include <list>
#include "human.h"
#include "../board.h"
#include "../display.h"

/******************************
 * HELPER FUNCTION PROTOTYPES *
 ******************************/

/**
 * Lets the user make a selection of the provided options.
 *
 * @param {Display&} A display object, used to communicate with the player.
 * @param {std::vector<Board::tile_iterator>} options The available tiles to
 * select from.
 * @returns {Board::tile_iterator} The selected tile.
 */
Board::tile_iterator make_selection(
  Display& d
  , std::list<Board::tile_iterator>& options);


/*******************
 * IMPLEMENTATIONS *
 *******************/

bool Human::takeTurn(std::mt19937&rng, Display& d, Board& b)
{
  // Refresh board
  b.draw();

  // Get possible attacking tiles
  std::list<Board::tile_iterator> my_tiles =
    b.filterForFrontlineTiles(b.getTilesByColor(getColor()));

  // Player has lost the game.
  if (my_tiles.size() == 0) {
    return false;
  }

  // Select attacking tile
  d.printMessage("Select your tile.");
  Board::tile_iterator cur_selection = make_selection(d, my_tiles);

  // Get possible defending tiles
  d.printMessage("Select enemy tile.");
  std::list<Board::tile_iterator> enemy_tiles =
    Board::filterColoredTiles(
      (*cur_selection).getColor()
      , b.getAdjacentTiles(*cur_selection));

  // Select defending tile
  Board::tile_iterator enemy_selection = make_selection(d, enemy_tiles);

  d.clearMessageBar();

  // Fight
  b.fight(rng, (*cur_selection).getId(), (*enemy_selection).getId());

  return true;
}


/***********************************
 * HELPER FUNCTION IMPLEMENTATIONS *
 ***********************************/

Board::tile_iterator make_selection(
  Display& d
  , std::list<Board::tile_iterator>& options)
{
  if (options.size() < 1) {
    throw std::invalid_argument("Cannot select from empty list.");
  }

  int input = 0;
  std::list<Board::tile_iterator>::iterator beginning = std::begin(options);
  std::list<Board::tile_iterator>::iterator last = std::prev(std::end(options));
  std::list<Board::tile_iterator>::iterator cur_selection = beginning;

  // Until enter
  while (input != static_cast<int> ('\n'))
  {
    input = d.blinkUntilKeypress((**cur_selection).getCoordinates());

    // Process non-enter input
    switch (input)
    {
      // Previous
      case Display::UP:
      case Display::LEFT:
        if (cur_selection == beginning) {
          //Wrap
          cur_selection = last;
        }
        else {
          --cur_selection;
        }
        break;

      // Next
      case Display::DOWN:
      case Display::RIGHT:
        if (cur_selection == last) {
          // Wrap
          cur_selection = beginning;
        }
        else {
          ++cur_selection;
        }
        break;
    }
  }

  return *cur_selection;
}

