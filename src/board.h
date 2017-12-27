#ifndef BOARD_H
#define BOARD_H

#include <list>
#include "color.h"
#include "display.h"
#include "player.h"
#include "tile.h"

class Board
{

  public:

    /****************
     * CONSTRUCTORS *
     ****************/

    Board(
      std::mt19937& rng
      , Display& d
      , const size_t width
      , const size_t height);


    /*********
     * TYPES *
     *********/

    using tile_iterator = std::list<Tile>::const_iterator;


    /***********
     * METHODS *
     ***********/

    /*** GETTERS ***/

    /**
     * Returns the beginning of the tiles this board owns.
     *
     * @returns {std::list<Tile>::iterator} An iterator to the beginning of the
     * tiles list.
     */
    tile_iterator getTiles() const { return std::begin(tiles_); }

    /**
     * Returns the end of the tiles this board owns.
     *
     * @returns {std::list<Tile>::iterator} An iterator to the end of the tiles
     * list.
     */
    tile_iterator getTilesEnd() const { return std::end(tiles_); }

    /**
     * Returns all the tiles that share a particular color.
     *
     * @param {Color} c The color to search for.
     * @returns {std::vector<std::list<Tile>::iterator>} A vector of iterators
     * to access tile objects.
     */
    std::list<tile_iterator> getTilesByColor(Color c) const;

    /**
     * Returns all the tiles that share a border with the provided tile.
     *
     * @param {Tile} t The tile to search for tiles that border it.
     * @returns {std::vector<std::list<Tile>::iterator>} A vector of iterators
     * to access tile objects.
     */
    std::list<tile_iterator> getAdjacentTiles(const Tile& t) const;


    /*** SETTERS ***/

    /**
     * Sets the tile with the given id to the provided color.
     *
     * @param {size_t} tile_id The id of the tile to set the color of.
     * @param {Color} c The color to set the tile to.
     */
    void setTileColor(size_t tile_id, Color c);


    /*** UTILITY ***/

    /**
     * Prints the current state of the board to the screen.
     */
    void draw() const;

    /**
     * Handles a fight between two tiles.
     *
     * @param {std::mt19937&} rng Used for randomness.
     * @param {size_t} attacker_id The attacking tile's id.
     * @param {size_t} defender_id The defending tile's id. (Wins ties)
     */
    void fight(std::mt19937& rng, size_t attacker_id, size_t defender_id);

    /**
     * Removes tiles that have a particular Color.
     *
     * @param {Color} c The color to filter on.
     * @param {std::vector<tile_iterator>&} tiles The tiles to filter.
     * @returns {std::vector<tile_iterator>} The filtered tiles.
     */
    static std::list<tile_iterator> filterColoredTiles(
      Color c
      , std::list<tile_iterator> tiles);

    /**
     * Removes tiles that don't have any adjacent enemy tiles.
     *
     * @param {std::vector<tile_iterator>&} tiles The tiles to filter.
     * @returns {std::vector<tile_iterator>} The filtered tiles.
     */
    std::list<tile_iterator> filterForFrontlineTiles(
      std::list<tile_iterator> tiles);


  private:

    /***********
     * METHODS *
     ***********/

    /**
     * Checks if two Tiles are adjacent on this board.
     *
     * @param {Tile} t1 The first tile.
     * @param {Tile} t2 The second tile.
     * @returns {bool} True if they are adjacent.
     */
    bool areAdjacent(const Tile& t1, const Tile& t2) const;

    /**
     * Marks two tiles as adjacent in the Board's adjacency matrix.
     *
     * @param {Tile} t1 The first tile.
     * @param {Tile} t2 The second tile.
     */
    void markAdjacent(const Tile& t1, const Tile& t2);

    /**
     * Finds a tile with the given id.
     *
     * @param {size_t} id The id.
     * @returns {Tile&} A reference to the tile.
     */
    Tile& getTileById(size_t id);


    /**************
     * PROPERTIES *
     **************/

    Display& d_;
    size_t width_, height_;
    std::list<Tile> tiles_;
    std::vector<bool> adjacency_;

};

#endif

