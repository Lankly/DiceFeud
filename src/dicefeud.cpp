#include <algorithm>
#include <array>
#include <deque>
#include <memory>
#include <random>
#include <stdexcept>
#include <vector>
#include "dicefeud.h"
#include "color.h"
#include "display.h"
#include "behavior/human.h"
#include "behavior/ai_easy.h"
#include "behavior/ai_medium.h"
#include "behavior/ai_hard.h"

/*******************
 * IMPLEMENTATIONS *
 *******************/

DiceFeud::DiceFeud(std::mt19937& rng, Display& d, size_t numPlayers)
  : d_(d)
  , board_(rng, d, Display::MINIMUM_WIDTH, Display::MINIMUM_HEIGHT - 1)
{
  if (numPlayers < 2) {
    std::invalid_argument("There must be at least 2 players.");
  }
  if (numPlayers > 8) {
    std::invalid_argument("There cannot be more than 8 players.");
  }

  // We want to have a random order to our turns, and deque doesn't let us
  // shuffle, so put everything into a vector for now
  std::vector<std::unique_ptr<Player>> players_arr;


  // One player is always a PURPLE Human
  players_arr.emplace_back(new Human(Color::PURPLE));


  // We want a different ordering of colors for every new game
  std::vector<Color> colors = {
    Color::BLUE, Color::CYAN, Color::GRAY, Color::GREEN
      , Color::ORANGE, Color::PINK, Color::PURPLE, Color::RED
  };
  std::shuffle(std::begin(colors), std::end(colors), rng);


  // To make it more and more unlikely that we will select the same
  // difficulty of AI for each of the newly-generated players_arr, we will keep
  // an array of weights that we will decrement whenever one of them is chosen.
  std::array<size_t, 3> difficulty_weights = {
    numPlayers, numPlayers, numPlayers
  };


  // Generate the AI Players
  for (int i = 0; i < numPlayers; ++i)
  {
    // We have to reconstruct the weights every time.
    std::discrete_distribution<size_t> dist(
        std::begin(difficulty_weights)
        , std::end(difficulty_weights));

    // Make our selection of difficulty for this AI.
    size_t difficulty = dist(rng);

    switch (difficulty)
    {
      case 0:
        {
          players_arr.emplace_back(new AIEasy(colors[i]));
          break;
        }

      case 1:
        {
          players_arr.emplace_back(new AIMedium(colors[i]));
          break;
        }

      default:
        {
          players_arr.emplace_back(new AIHard(colors[i]));
        }
    }

    // Make it more unlikely for this to be picked again
    --(difficulty_weights[difficulty]);
  }

  // Random order of turns
  std::shuffle(std::begin(players_arr), std::end(players_arr), rng);

  // Push into our deque
  for (std::unique_ptr<Player>& p : players_arr)
  {
    players_.push_back(std::move(p));
  }

  // Now assign each player to random tiles on the board.
  std::list<Tile>::const_iterator end = board_.getTilesEnd();
  for(std::list<Tile>::const_iterator cur_tile = board_.getTiles()
    ; cur_tile != end
    ; ++cur_tile)
  {
    // Pull player off front of deque
    std::unique_ptr<Player> cur_player (std::move(players_.front()));
    players_.pop_front();

    // Set this tile's color to this player's color
    board_.setTileColor((*cur_tile).getId(), (*cur_player).getColor());

    // Put player back in deque
    players_.push_back(std::move(cur_player));
  }
}


bool DiceFeud::play(std::mt19937& rng)
{
  board_.draw();

  while (players_.size() > 1)
  {
    std::unique_ptr<Player> cur = std::move(players_.front());
    players_.pop_front();

    // Let the player take their turn
    bool defeated = cur->takeTurn(rng, d_, board_) == false;

    if (!defeated) {
      // Move this to the back of the queue
      players_.push_back(std::move(cur));
    }
  }

  return gameOver();
}


bool DiceFeud::gameOver()
{
  if (players_.size() == 1) { throw std::logic_error("No players in game!"); }

  std::unique_ptr<Player> winner = std::move(players_.front());
  players_.pop_front();

  // Is the Human Player the winner?
  Human* was_human = dynamic_cast<Human*>(winner.get());

  if (was_human) {

  }
  else {

  }

  return true;
}

