#include <cmath>
#include <float.h>
#include <random>
#include <stdexcept>
#include <string>
#include "board.h"
#include "display.h"
#include "tile.h"

/******************************
 * HELPER FUNCTION PROTOTYPES *
 ******************************/

/**
 * Gets the distance between two points in a matrix represented by an array.
 *
 * @param {size_t} p1 The first point.
 * @param {size_t} p2 The second point.
 * @param {size_t} width The width of the matrix.
 * @returns {float} The distance between the two points.
 */
double get_dist(Tile::coord_t p1, Tile::coord_t p2, size_t width);

/**
 * Gets an index into the adjacency matrix using two coordinates.
 *
 * @param {size_t} c1 The first coordinate (smaller).
 * @param {size_t} c2 The second coordinate (larger).
 * @returns {size_t} The index into the matrix.
 */
inline size_t get_adjacency_index(Tile::coord_t c1, Tile::coord_t c2);


/*******************
 * IMPLEMENTATIONS *
 *******************/

Board::Board(
  std::mt19937& rng
  , Display& d
  , const size_t width
  , const size_t height)
  : d_(d)
  , width_(width)
  , height_(height)
{
  if (width == 0 || height == 0) {
    throw std::invalid_argument("Board cannot have no size.");
  }

  if (width < Display::MINIMUM_WIDTH || height < Display::MINIMUM_HEIGHT) {
    throw std::invalid_argument("Board dimensions cannot be below minimums.");
  }

  size_t num_spaces = width * height;
  size_t num_generated = 0;
  size_t min_size_per_tile = num_spaces / 30;
  size_t max_size_per_tile = num_spaces / 25;
  size_t max_attempts = 100;

  // We will use this temporary object to keep track of which tiles are where
  // It will then be used to create our adjacency matrix at the end.
  std::vector<Board::tile_iterator> occupied (num_spaces, std::end(tiles_));

  // We will use this so that we don't accidentally pick a starting point for
  // a tile that is in-use by another tile
  std::vector<size_t> space_weights (num_spaces, 1);

  // Generate each tile (A tile consists of multiple spaces)
  for (size_t percent_generated = 0
      ; percent_generated < 80
      ; percent_generated = 100 * num_generated / num_spaces)
  {
    Tile cur_tile;

    // Pick a random starting space
    std::discrete_distribution<Tile::coord_t> starting_weights (
      std::begin(space_weights)
      , std::end(space_weights));
    Tile::coord_t starting_coord = starting_weights(rng);
    Tile::coord_t coord = starting_coord;

    // Figure out which nearby tiles are open and add then to another vector of
    // weights for picking where to go next
    std::vector<double> next_space_weights (num_spaces, 0);
    size_t cur_tile_size = 1;

    // Let tile know it owns this space
    cur_tile.addCoordindate(coord);

    // Generate tile based on starting point
    bool success = false;
    for(;;)
    {
      // Find an adjacent space
      // Left available?
      size_t new_coord = coord - 1;
      if ((coord % width != 0) && (occupied[new_coord] == std::end(tiles_))) {
        next_space_weights[new_coord] = DBL_MAX;
      }
      // Right available?
      new_coord = coord + 1;
      if ((coord % width != (width - 1))
          && (occupied[new_coord] == std::end(tiles_))) {
        next_space_weights[new_coord] = DBL_MAX;
      }
      // Up?
      new_coord = coord - width;
      if (((coord / width) != 0)
          && (occupied[new_coord] == std::end(tiles_))) {
        next_space_weights[new_coord] = DBL_MAX;
      }
      // Down?
      new_coord = coord + width;
      if (((coord / width) != (height - 1))
          && (occupied[new_coord] == std::end(tiles_))) {
        next_space_weights[new_coord] = DBL_MAX;
      }

      // Calculate the weights for these new points, check for any valid weights
      bool no_valid_weights = true;
      for (double& space : next_space_weights)
      {
        if (space == DBL_MAX) {
          space = (width * height / 2) - get_dist(starting_coord, space, width);
        }
        if (space != 0) { no_valid_weights = false; }
      }

      // Can't find next space
      if (no_valid_weights) {
        size_t cur_tile_size = cur_tile.getCoordinates().size();

        success = (cur_tile_size >= min_size_per_tile)
            && (cur_tile_size <= max_size_per_tile);

        break;
      }

      // Find the next space
      std::discrete_distribution<Tile::coord_t> next_space (
        std::begin(next_space_weights)
        , std::end(next_space_weights));
      coord = next_space(rng);
      next_space_weights[coord] = 0;

      // If we ran out of attempts, stop and do not add this tile
      if (occupied[coord] != std::end(tiles_)) {
        // Unmark all the spaces this tile occupied
        for(Tile::coord_t coord : cur_tile.getCoordinates())
        {
          occupied[coord] = std::end(tiles_);
        }
        break;
      }

      cur_tile.addCoordindate(coord);

      // Stop adding spaces if too big
      if (++cur_tile_size >= min_size_per_tile) {
        success = true;
        break;
      }
    }

    // Tile generation was successful
    if (success) {
      num_generated += cur_tile.getCoordinates().size();
      cur_tile.setId(tiles_.size());
      tiles_.push_front(std::move(cur_tile));

      const std::vector<Tile::coord_t>& cur_coordinates =
        tiles_.front().getCoordinates();

      // Mark spaces as occupied by this tile
      for (Tile::coord_t cur_coord : cur_coordinates)
      {
        occupied[cur_coord] = std::begin(tiles_);
        space_weights[cur_coord] = 0;
      }
    }
  }

  // Randomly give each tile a number of dice.
  // Starting chances should favor middle numbers
  std::vector<size_t> num_dice_weights (Tile::MAX_DICE_PER_TILE);
  size_t last_value = 0;
  for (size_t index = 0; index < Tile::MAX_DICE_PER_TILE; ++index)
  {
    if (index <= (Tile::MAX_DICE_PER_TILE / 2)) {
      num_dice_weights[index] = ++last_value;
    }
    else {
      num_dice_weights[index] = --last_value;
    }
  }
  std::discrete_distribution<size_t> num_dice_distribution (
    std::begin(num_dice_weights)
    , std::end(num_dice_weights));
  for (Tile& tile : tiles_)
  {
    tile.setNumDice(num_dice_distribution(rng) + 1);
  }

  // Generate adjacenty matrix
  adjacency_.resize(tiles_.size() * (tiles_.size() - 1) / 2);
  Board::tile_iterator empty_space = std::end(tiles_);
  for (size_t i = 0; i < num_spaces; ++i)
  {
    Board::tile_iterator cur_space = occupied[i];
    if (cur_space == std::end(tiles_)) { continue; }

    // Since we start in the upper left-hand corner, we only have to look down
    // and to the right to see unchecked spaces
    // Right
    if (i % width != (width - 1)) {
      Board::tile_iterator right_space = occupied[i + 1];
      if (right_space != empty_space && right_space != cur_space) {
        markAdjacent(*cur_space, *right_space);
      }
    }
    // Down
    if (i / width != (height - 1)) {
      Board::tile_iterator down_space = occupied[i + width];
      if (down_space != empty_space && down_space != cur_space) {
        markAdjacent(*cur_space, *down_space);
      }
    }
  }
}


std::list<Board::tile_iterator> Board::getTilesByColor(Color c) const
{
  std::list<tile_iterator> to_return;

  for (tile_iterator cur = std::begin(tiles_)
      ; cur != std::end(tiles_)
      ; ++cur)
  {
    if ((*cur).getColor() == c) {
      to_return.push_back(cur);
    }
  }

  return to_return;
}


std::list<Board::tile_iterator> Board::getAdjacentTiles(const Tile& t) const
{
  std::list<tile_iterator> to_return;

  for (tile_iterator cur = std::begin(tiles_)
      ; cur != std::end(tiles_)
      ; ++cur)
  {
    if (areAdjacent(t, *cur)) {
      to_return.push_back(cur);
    }
  }

  return to_return;
}


void Board::setTileColor(size_t tile_id, Color c)
{
  getTileById(tile_id).setColor(c);
}


void Board::draw() const
{
  // Draw each tile one by one
  for (const Tile& t : tiles_)
  {
    std::vector<size_t> coordinates = t.getCoordinates();
    Color cur_color = t.getColor();
    // This is okay because we will never have double-digits numbers
    char dice_num = std::to_string(t.getNumDice()).front();
    auto character = Display::getDisplayableCharacter(cur_color, dice_num);

    d_.drawValue(coordinates, character);
  }
}


void Board::fight(std::mt19937& rng, size_t attacker_id, size_t defender_id)
{
  Tile& attacker = getTileById(attacker_id);
  Tile& defender = getTileById(defender_id);

  size_t attacker_dice = attacker.getNumDice();
  size_t defender_dice = defender.getNumDice();
  size_t attacker_total = 0, defender_total = 0;

  for (size_t i = 0; i < attacker_dice; ++i) {
    attacker_total += (rng() % 6) + 1;
  }
  for (size_t i = 0; i < defender_dice; ++i) {
    defender_total += (rng() % 6) + 1;
  }

  // Attacker won
  if (attacker_total > defender_total) {
    defender.setNumDice(attacker_dice - 1);
    defender.setColor(attacker.getColor());
  }

  // In all cases, attacker's tile gets reduced to 1.
  attacker.setNumDice(1);
}


bool Board::areAdjacent(const Tile& t1, const Tile& t2) const
{
  // Since, to determine if a tile is adjacent, we only need to check one,
  // we will only mark the one with the lower id.

  Tile::coord_t smaller = std::min(t1.getId(), t2.getId());
  Tile::coord_t larger = std::max(t1.getId(), t2.getId());

  Tile::coord_t index = get_adjacency_index(smaller, larger);

  return adjacency_[index];
}


void Board::markAdjacent(const Tile& t1, const Tile& t2)
{
  // Since, to determine if a tile is adjacent, we only need to check one,
  // we will only mark the one with the lower id.

  Tile::coord_t smaller = std::min(t1.getId(), t2.getId());
  Tile::coord_t larger = std::max(t1.getId(), t2.getId());

  Tile::coord_t index = get_adjacency_index(smaller, larger);

  adjacency_[index] = true;
}


Tile& Board::getTileById(size_t id)
{
  for (Tile& t : tiles_)
  {
    if (t.getId() == id) {
      return t;
    }
  }

  throw std::out_of_range("No tile with the given id exists.");
}


std::list<Board::tile_iterator> Board::filterColoredTiles(
  Color c,
  std::list<Board::tile_iterator> tiles)
{
  // This will be used to keep track of tiles we're going to remove from tiles.
  std::list<std::list<Board::tile_iterator>::iterator> to_erase;

  for (std::list<Board::tile_iterator>::iterator cur = std::begin(tiles)
    ; cur != std::end(tiles)
    ; ++cur)
  {
    if ((**cur).getColor() == c) {
      to_erase.push_back(cur);
    }
  }

  // Delete the tiles of the given color from the passed-in tiles.
  for(std::list<Board::tile_iterator>::iterator cur : to_erase)
  {
    tiles.erase(cur);
  }

  return tiles;
}


std::list<Board::tile_iterator> Board::filterForFrontlineTiles(
  std::list<Board::tile_iterator> tiles)
{
  // This will be used to keep track of tiles we're going to remove from tiles.
  std::list<std::list<Board::tile_iterator>::iterator> to_erase;

  for (std::list<Board::tile_iterator>::iterator cur = std::begin(tiles)
    ; cur != std::end(tiles)
    ; ++cur)
  {
    std::list<Board::tile_iterator> adjacent = getAdjacentTiles(**cur);

    bool is_frontline = false;
    for (Board::tile_iterator tile : adjacent)
    {
      if ((*tile).getColor() != (**cur).getColor()) {
        is_frontline = true;
        break;
      }
    }

    if (!is_frontline) {
      to_erase.push_back(cur);
    }
  }

  // Delete the non-frontlines from the passed-in tiles.
  for(std::list<Board::tile_iterator>::iterator cur : to_erase)
  {
    tiles.erase(cur);
  }

  return tiles;
}


/***********************************
 * HELPER FUNCTION IMPLEMENTATIONS *
 ***********************************/

double get_dist(Tile::coord_t p1, Tile::coord_t p2, size_t width)
{
  double p1_x = p1 % width
    , p1_y    = p1 / width
    , p2_x    = p2 % width
    , p2_y    = p2 / width;

  return sqrt(std::pow(p1_x - p2_x, 2) + (p1_y - p2_y));
}


inline size_t get_adjacency_index(Tile::coord_t c1, Tile::coord_t c2)
{
  return (c2 * (c2 - 1) / 2) + c1;
}

