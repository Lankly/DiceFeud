#include <iostream>
#include <random>
#include "dicefeud.h"

int main(int argc, char** argv)
{
  std::random_device randomDevice;
  std::mt19937 rng(randomDevice());

  const size_t NUM_PLAYERS = 8;
  Display d (80, 24);

  try
  {
    bool done = false;

    while (!done)
    {
      DiceFeud game(rng, d, NUM_PLAYERS);

      done = game.play(rng) == false;
    }
  }
  catch (const std::exception& ex)
  {
    std::cout << "Error in program: " << ex.what() << std::endl;
  }
}

