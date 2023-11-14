#include "../include/tetrominos.hpp"

#define base_offset_x 188
#define base_offset_y 96

tetromino::BaseTetromino::BaseTetromino() : rotation(0) {}

tetromino::BaseTetromino::~BaseTetromino() {}

void tetromino::BaseTetromino::rotate(bool clockwise) {
  if (clockwise) {
    rotation = (rotation + 1) % shapes.size();
  } else {
    rotation = (rotation - 1 + shapes.size()) % shapes.size();
  }
}

std::vector<std::vector<int8_t>>& tetromino::BaseTetromino::currentShape(
    bool next) {
  return shapes[next ? 0 : rotation];
}

tetromino::TTetromino::TTetromino() {
  shapes = {{{0, 0, 0, 0, 0},
             {0, 0, 0, 0, 0},
             {0, 1, 1, 1, 0},
             {0, 0, 1, 0, 0},
             {0, 0, 0, 0, 0}},

            {{0, 0, 0, 0, 0},
             {0, 0, 1, 0, 0},
             {0, 1, 1, 0, 0},
             {0, 0, 1, 0, 0},
             {0, 0, 0, 0, 0}},

            {{0, 0, 0, 0, 0},
             {0, 0, 1, 0, 0},
             {0, 1, 1, 1, 0},
             {0, 0, 0, 0, 0},
             {0, 0, 0, 0, 0}},

            {{0, 0, 0, 0, 0},
             {0, 0, 1, 0, 0},
             {0, 0, 1, 1, 0},
             {0, 0, 1, 0, 0},
             {0, 0, 0, 0, 0}}};
  next_offset_x = base_offset_x;
  next_offset_y = base_offset_y;
}

tetromino::JTetromino::JTetromino() {
  shapes = {{{0, 0, 0, 0, 0},
             {0, 0, 0, 0, 0},
             {0, 3, 3, 3, 0},
             {0, 0, 0, 3, 0},
             {0, 0, 0, 0, 0}},

            {{0, 0, 0, 0, 0},
             {0, 0, 3, 0, 0},
             {0, 0, 3, 0, 0},
             {0, 3, 3, 0, 0},
             {0, 0, 0, 0, 0}},

            {{0, 0, 0, 0, 0},
             {0, 3, 0, 0, 0},
             {0, 3, 3, 3, 0},
             {0, 0, 0, 0, 0},
             {0, 0, 0, 0, 0}},

            {{0, 0, 0, 0, 0},
             {0, 0, 3, 3, 0},
             {0, 0, 3, 0, 0},
             {0, 0, 3, 0, 0},
             {0, 0, 0, 0, 0}}};
  next_offset_x = base_offset_x;
  next_offset_y = base_offset_y;
}

tetromino::ZTetromino::ZTetromino() {
  shapes = {{{0, 0, 0, 0, 0},
             {0, 0, 0, 0, 0},
             {0, 2, 2, 0, 0},
             {0, 0, 2, 2, 0},
             {0, 0, 0, 0, 0}},

            {{0, 0, 0, 0, 0},
             {0, 0, 0, 2, 0},
             {0, 0, 2, 2, 0},
             {0, 0, 2, 0, 0},
             {0, 0, 0, 0, 0}}};
  next_offset_x = base_offset_x;
  next_offset_y = base_offset_y;
}

tetromino::OTetromino::OTetromino() {
  shapes = {{{0, 0, 0, 0, 0},
             {0, 0, 0, 0, 0},
             {0, 1, 1, 0, 0},
             {0, 1, 1, 0, 0},
             {0, 0, 0, 0, 0}}};
  next_offset_x = base_offset_x + 4;
  next_offset_y = base_offset_y;
}

tetromino::STetromino::STetromino() {
  shapes = {{{0, 0, 0, 0, 0},
             {0, 0, 0, 0, 0},
             {0, 0, 3, 3, 0},
             {0, 3, 3, 0, 0},
             {0, 0, 0, 0, 0}},

            {{0, 0, 0, 0, 0},
             {0, 0, 3, 0, 0},
             {0, 0, 3, 3, 0},
             {0, 0, 0, 3, 0},
             {0, 0, 0, 0, 0}}};
  next_offset_x = base_offset_x;
  next_offset_y = base_offset_y;
}

tetromino::LTetromino::LTetromino() {
  shapes = {{{0, 0, 0, 0, 0},
             {0, 0, 0, 0, 0},
             {0, 2, 2, 2, 0},
             {0, 2, 0, 0, 0},
             {0, 0, 0, 0, 0}},

            {{0, 0, 0, 0, 0},
             {0, 2, 2, 0, 0},
             {0, 0, 2, 0, 0},
             {0, 0, 2, 0, 0},
             {0, 0, 0, 0, 0}},

            {{0, 0, 0, 0, 0},
             {0, 0, 0, 2, 0},
             {0, 2, 2, 2, 0},
             {0, 0, 0, 0, 0},
             {0, 0, 0, 0, 0}},

            {{0, 0, 0, 0, 0},
             {0, 0, 2, 0, 0},
             {0, 0, 2, 0, 0},
             {0, 0, 2, 2, 0},
             {0, 0, 0, 0, 0}}};
  next_offset_x = base_offset_x;
  next_offset_y = base_offset_y;
}

tetromino::ITetromino::ITetromino() {
  shapes = {{{0, 0, 0, 0, 0},
             {0, 0, 0, 0, 0},
             {1, 1, 1, 1, 0},
             {0, 0, 0, 0, 0},
             {0, 0, 0, 0, 0}},

            {{0, 0, 1, 0, 0},
             {0, 0, 1, 0, 0},
             {0, 0, 1, 0, 0},
             {0, 0, 1, 0, 0},
             {0, 0, 0, 0, 0}}};
  next_offset_x = base_offset_x + 4;
  next_offset_y = base_offset_y + 4;
}
