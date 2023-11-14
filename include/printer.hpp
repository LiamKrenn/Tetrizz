#ifndef TETRIS_PRINTER
#define TETRIS_PRINTER

// IMPORTANT: DON'T MODIFY THE ORDER OF INPUTS!
// clang-format off
#include <inttypes.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <parallel/for_each.h>
#include <parallel/sort.h>
#include <parallel/algo.h>
#include <execution>
#include <sstream>
#include <thread>

#include <algorithm>
#include <array>
#include <chrono>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <sstream>
#include <string>
// clang-format on
#include "../include/tetrominos.hpp"
#define color_black 0
#define color_white 15
#define color_red 196
#define color_bg_gray 243
#define color_bg_cyan 123

#define code_black '0'
#define code_transparent ' '

#define screen_height 224
#define screen_width 256

#define spritesheet_width 26
#define spritesheet_height 3
#define sprite_size 8
#define spritesheetchars                                                   \
  "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789><_     " \
  ",/()\". -"

#define block_field_hidden_rows 2
#define block_field_height 22
#define block_field_width 10
#define block_offset_x 96
#define block_offset_y 40

#define line_number_width 3
#define line_count_offset_x 152
#define line_count_offset_y 16
#define score_number_width 6
#define score_offset_x 192
#define top_score_offset_y 32
#define current_score_offset_y 56
#define level_number_width 2
#define current_level_offset_x 208
#define current_level_offset_y 160

#define text_fg_color color_white
#define text_bg_color color_black
using namespace tetromino;
using namespace std::chrono;

#include "tetrominos.hpp"

namespace printer {
class Printer {
 private:
  std::shared_ptr<bool> exit_program;
  int16_t current_level;
  int16_t line_count;
  int64_t current_score;
  int64_t top_score;

  bool hide_next_bool = false;

  std::shared_ptr<tetromino::BaseTetromino> nextTetromino;

  std::vector<std::vector<uint8_t>> level_colors;
  std::vector<std::vector<int16_t>> blocks;
  std::map<char, int16_t> colors_256;

  std::map<int16_t, std::vector<std::vector<char>>> block_types;
  std::map<char, std::vector<std::vector<char>>> sprites;

  std::vector<std::vector<char>> new_screen;
  std::vector<std::vector<char>> current_screen;

  bool checkNewFrameDifference(const std::vector<std::vector<char>>& new_frame);

  void addToNewFrame(std::vector<std::vector<char>>& pixels, int16_t offset_x,
                     int16_t offset_y);
  void addString(std::string string, int16_t offset_x, int16_t offset_y);
  void addChar(char character, int16_t offset_x, int16_t offset_y);
  void addBlocks();
  void addNext();

 public:
  void hide_next();
  void initBackground(std::vector<std::vector<char>> background);
  void setScore(int64_t points);
  void setTopScore(int64_t points);
  void setLineCount(int16_t lines);
  void setLevel(int16_t level);
  void setBlock(int16_t x, int16_t y, int16_t type);
  void setField(std::vector<std::vector<int16_t>> field);
  void setNextTetromino(std::shared_ptr<tetromino::BaseTetromino> tetromino);
  void setStatistics(std::vector<uint16_t> stats);
  void print(bool game = true);

  Printer(std::shared_ptr<bool> exit_program,
          std::vector<std::vector<char>> spritesheet,
          std::map<int16_t, std::vector<std::vector<char>>> block_t,
          std::vector<std::vector<uint8_t>> level_colors);
};
}  // namespace printer
#endif
