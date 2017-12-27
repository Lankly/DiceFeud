#ifndef TILE_H
#define TILE_H

/************************
 * FORWARD DECLARATIONS *
 ************************/

class Tile;


/************
 * INCLUDES *
 ************/

#include <algorithm>
#include <vector>
#include "color.h"


/*********
 * CLASS *
 *********/

class Tile
{

  public:

    /*********
     * TYPES *
     *********/

    using coord_t = size_t;


    /***********
     * METHODS *
     ***********/

    /*** GETTERS ***/

    /**
     * Gets the color of this tile
     *
     * @returns {Color} The color of this tile.
     */
    Color getColor() const { return color_; }

    /**
     * Gets the id of this tile.
     *
     * @returns {size_t} The id of this tile.
     */
    size_t getId() const { return id_; }

    /**
     * Gets the coordinates that this tile occupies. Since a tile occupies many
     * spaces, it is necessary to store them like this.
     *
     * @returns {std::vector<size_t>} A vector of coordinates.
     */
    const std::vector<coord_t>& getCoordinates() const { return coordinates_; }

    /**
     * Gets the number of dice on this tile.
     *
     * @returns {const size_t} The number of dice on this tile.
     */
    size_t getNumDice() const { return  num_dice_; }


    /*** MUTATORS ***/

    // Color

    /**
     * Sets the color of this tile to the specified color.
     *
     * @param {Color} c The color to set this tile to.
     */
    void setColor(Color c) { color_ = c; }

    // Coordinates

    /**
     * Adds a coordinate to this tile.
     *
     * @param {size_t} coord The coordinate to add.
     */
    void addCoordindate(coord_t coord) { coordinates_.push_back(coord); }

    // Dice

    /**
     * Sets the number of dice on this tile. Will cap at the max.
     *
     * @param {size_t} newNum The new number of dice to set the tile to.
     */
    void setNumDice(const size_t newNum);

    /**
     * Increments the number of dice on the tile. Will do nothing if the tile is
     * already at the max.
     */
    void incNumDice() {
      num_dice_ = std::max(num_dice_ + 1, MAX_DICE_PER_TILE); }

    /**
     * Decrements the number of dice on the tile. Will do nothing if the tile is
     * already at 0.
     */
    void decNumDice() {
      num_dice_ = std::min(static_cast<int> (num_dice_) - 1, 1); }

    // ID

    void setId(size_t new_id) { id_ = new_id; }


    /**************
     * PROPERTIES *
     **************/

    /*** CONSTANTS ***/

    static const size_t MAX_DICE_PER_TILE;


  private:

    /**************
     * PROPERTIES *
     **************/

    /*** VARIABLES ***/

    Color color_ = Color::BLUE;
    size_t id_ = 0;
    size_t num_dice_ = 0;
    std::vector<coord_t> coordinates_;

};

#endif

