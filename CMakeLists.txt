cmake_minimum_required (VERSION 3.1)
project (wordplay)

find_package(Curses)

include_directories(
  src
  src/behavior
  ${CURSES_INCLUDE_DIRS})

add_executable(dicefeud
  src/main.cpp
  src/board.cpp
  src/dicefeud.cpp
  src/display.cpp
  src/tile.cpp
  src/behavior/ai_easy.cpp
  src/behavior/ai_hard.cpp
  src/behavior/ai_medium.cpp
  src/behavior/human.cpp)
set_property(TARGET dicefeud PROPERTY CXX_STANDARD 11)
set_property(TARGET dicefeud PROPERTY CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)
target_link_libraries(dicefeud ${CURSES_LIBRARIES})
