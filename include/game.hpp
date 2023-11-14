#ifndef TETRIS_GAME
#define TETRIS_GAME

#include <signal.h>
#include <stdio_ext.h>
#include <termios.h>

#include <algorithm>
#include <array>
#include <chrono>
#include <condition_variable>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <memory>
#include <numeric>
#include <random>
#include <set>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#include "decoder.hpp"
#include "input.hpp"
#include "printer.hpp"
#include "tetrominos.hpp"

using namespace decoder;
using namespace printer;
using namespace tetromino;
using namespace input;

namespace tetris {
class Game {
 private:
  Decoder dec = Decoder();
  Input input = Input();
  std::shared_ptr<bool> exit_program = input.getExitPointer();
  Printer printer =
      Printer(exit_program, dec.spritesheet, dec.block_types, dec.level_colors);

  uint8_t down_press_score;
  uint64_t score;
  uint64_t current_level;
  uint64_t line_count;

  std::shared_ptr<BaseTetromino> nextTetromino;
  std::shared_ptr<BaseTetromino> currentTetromino;
  std::pair<int8_t, int8_t> tetrominoCoords;

  std::vector<std::vector<int16_t>> block_field;
  std::vector<uint16_t> statistics;

  uint64_t frameCount;
  uint16_t framesPerDrop;

  void clearLines(int rows, int start_index);
  void addScore(uint64_t lines_cleard);
  void lockTetromino();
  std::shared_ptr<BaseTetromino> randomPiece();
  void setLevel(uint64_t level);
  void moveLeft();
  void moveRight();
  void moveDown();
  void rotateCW();
  void rotateCCW();
  std::vector<std::vector<int16_t>> combineTetrominoField();

 public:
  Game();
  void start(int start_level = 0);
};
}  // namespace tetris

#endif
