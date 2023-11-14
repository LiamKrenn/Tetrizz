#include "../include/printer.hpp"

std::string numberToString(int64_t number, size_t width) {
  std::string str_number = std::to_string(number);
  if (str_number.size() <= width) {
    return std::string(width - std::to_string(number).size(), '0') +
           std::to_string(number);
  } else {
    return std::to_string(number);
  }
}

std::vector<std::vector<char>> getSprite(
    std::vector<std::vector<char>>& spritesheet, int16_t startRow,
    int16_t startCol) {
  std::vector<std::vector<char>> sprite(sprite_size,
                                        std::vector<char>(sprite_size));

  for (size_t i = 0; i < sprite_size; ++i) {
    std::copy(spritesheet[startRow + i].begin() + startCol,
              spritesheet[startRow + i].begin() + startCol + sprite_size,
              sprite[i].begin());
  }

  return sprite;
}

std::vector<std::vector<char>> placeMatrixInMatrix(
    const std::vector<std::vector<char>>& matrix1,
    std::vector<std::vector<char>>& matrix2, int16_t offset_x,
    int16_t offset_y) {
  for (size_t i = 0; i < matrix1.size(); ++i) {
    for (size_t j = 0; j < matrix1[i].size(); ++j) {
      if (matrix1[i][j] != 'B' && matrix1[i][j] != ' ') {
        matrix2[i + offset_y][j + offset_x] = std::move(matrix1[i][j]);
      }
    }
  }
  return matrix2;
}

bool printer::Printer::checkNewFrameDifference(
    const std::vector<std::vector<char>>& new_frame) {
  for (size_t i = 0; i < current_screen.size(); ++i) {
    for (size_t j = 0; j < current_screen[i].size(); ++j) {
      if (new_frame[i][j] != code_transparent &&
          new_frame[i][j] != current_screen[i][j]) {
        return true;
      }
    }
  }
  return false;
}

printer::Printer::Printer(
    std::shared_ptr<bool> exitp, std::vector<std::vector<char>> spritesheet,
    std::map<int16_t, std::vector<std::vector<char>>> block_t,
    std::vector<std::vector<uint8_t>> lvl_colors) {
  exit_program = exitp;
  block_types = block_t;
  level_colors = lvl_colors;

  for (size_t i = 0; i < spritesheet_width * spritesheet_height; ++i) {
    sprites[spritesheetchars[i]] =
        getSprite(spritesheet, i / spritesheet_width * sprite_size,
                  i % spritesheet_width * sprite_size);
  }

  for (size_t i = 0; i < block_field_height; ++i) {
    blocks.emplace_back(block_field_width, 0);
  }

  colors_256 = {{'0', color_black},
                {'1', color_white},
                {'2', color_bg_cyan},
                {'3', color_bg_gray},
                {'4', color_red},
                {'5', -1},
                {'6', -1},
                {'7', -1},
                {'8', -1},
                {'9', level_colors[current_level][3]},
                {'a', text_fg_color},
                {'b', text_bg_color},
                {'c', level_colors[current_level][1]},
                {'d', level_colors[current_level][2]}};

  for (size_t i = 0; i < screen_height; ++i) {
    current_screen.emplace_back(screen_width, code_transparent);
    new_screen.emplace_back(screen_width, code_transparent);
  }
}

void printer::Printer::addToNewFrame(std::vector<std::vector<char>>& pixels,
                                     int16_t offset_x, int16_t offset_y) {
  // place pixels new frame with offset
  new_screen = placeMatrixInMatrix(pixels, new_screen, offset_x, offset_y);
}

void printer::Printer::print(bool game) {
  // Terminal size
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

  if (game) {
    // add blocks to frame
    addBlocks();

    addString(numberToString(line_count, line_number_width),
              line_count_offset_x, line_count_offset_y);
    addString(numberToString(top_score, score_number_width), score_offset_x,
              top_score_offset_y);
    addString(numberToString(current_score, score_number_width), score_offset_x,
              current_score_offset_y);
    addString(numberToString(current_level, level_number_width),
              current_level_offset_x, current_level_offset_y);
    addNext();
  }

  // return if no change
  if (!checkNewFrameDifference(new_screen)) return;

  // generate unique pixel pairs, that change from old to new frame
  std::set<std::pair<char, char>> pairs;
  for (size_t i = 0; i < new_screen.size(); i += 2) {
    for (size_t j = 0; j < new_screen[i].size(); ++j) {
      if (new_screen[i][j] != current_screen[i][j] ||
          new_screen[i + 1][j] != current_screen[i + 1][j]) {
        pairs.emplace(new_screen[i][j], new_screen[i + 1][j]);
      }
    }
  }

  // print characters for each pair
  std::ostringstream out_stream;
  std::for_each(
      std::execution::par_unseq, pairs.begin(), pairs.end(),
      [w, this, &out_stream](std::pair<char, char> pair) {
        // no change for this pair
        if (pair.first == '\000')
          pair.first =
              code_transparent;  // invalid character (core issue unknown
        if (pair.second == '\000')
          pair.second =
              code_transparent;  //               due to time limitations)

        if (pair.first == code_transparent && pair.second == code_transparent)
          return;

        // print ansi color code once at the beginning
        std::string color_code =
            "\033[38;5;" + std::to_string(colors_256[pair.first]);
        if (pair.first != pair.second) {
          color_code += ";48;5;" + std::to_string(colors_256[pair.second]);
        }
        color_code += 'm';
        std::string character = (pair.first == pair.second) ? "█" : "▀";
        out_stream << "\x1b[1;1f" << color_code;

        // print the character on every pixel where the pair color is
        for (size_t i = 0; i < new_screen.size(); i += 2) {
          std::pair<char, char> prev_pair = {'z', 'z'};
          if (i / 2 + 1 >= w.ws_row) continue;
          ;

          for (size_t j = 0; j < new_screen[i].size(); ++j) {
            if (current_screen[i][j] == new_screen[i][j] &&
                current_screen[i + 1][j] == new_screen[i + 1][j]) {
              prev_pair = {'z', 'z'};
              continue;
            }

            // if pair equals current pixel
            if (new_screen[i][j] == pair.first &&
                new_screen[i + 1][j] == pair.second) {
              // only print if terminal is wide enough
              if (w.ws_col - size_t(1) > j) {
                // if previous pair is not same color, jump cursor to current
                // loop position
                if (pair != prev_pair) {
                  out_stream << "\x1b[" << (i / 2) + 1 << ";" << j + 1 << "f";
                  prev_pair = std::move(pair);
                }
                // else print without jumping (saving characters)
                out_stream << character;

                // current_screen[i / 2][j] = pair.first;
                // current_screen[i / 2 + 1][j] = pair.second;
              }
            } else {
              // reset prev_pair
              prev_pair = {'z', 'z'};
            }
          }
        }
      });
  // current_screen = new_screen;
  std::swap(current_screen, new_screen);
  new_screen = current_screen;

  // reset new_screen
  // for (std::vector<char>& row : new_screen) {
  //  std::fill(row.begin(), row.end(), code_transparent);
  //}
  // undo all cursor colors and flush
  std::cout << std::move(out_stream.str()) << "\n\x1b[0m" << std::flush;
  // new frame empty
}

void printer::Printer::setBlock(int16_t x, int16_t y, int16_t type) {
  blocks[y + block_field_hidden_rows][x] = type;
}

void printer::Printer::setField(std::vector<std::vector<int16_t>> field) {
  if (field.size() != block_field_height ||
      field[0].size() != block_field_width)
    return;
  blocks = field;
}

void printer::Printer::addString(std::string string, int16_t offset_x,
                                 int16_t offset_y) {
  for (size_t i = 0; i < string.length(); ++i) {
    addChar(string[i], offset_x + (i * sprite_size), offset_y);
  }
}

void printer::Printer::addChar(char character, int16_t offset_x,
                               int16_t offset_y) {
  addToNewFrame(sprites[character], offset_x, offset_y);
}

void printer::Printer::addBlocks() {
  // generates pixel matrix from block matrix that only stores wheter a block is
  // there and what type of block

  // matrix that stores the pixels (2 rows smaller than blocks matrix)
  // matrix is set to all black initially
  std::vector<std::vector<char>> blocks_matrix(
      (block_field_height - block_field_hidden_rows) * sprite_size,
      std::vector<char>(block_field_width * sprite_size, code_black));
  // each row in blocks matrix (skip first 2 rows)
  for (size_t i = block_field_hidden_rows; i < block_field_height; ++i) {
    // each column in blocks matrix
    for (size_t j = 0; j < block_field_width; ++j) {
      // if there is no block continue
      if (blocks[i][j] == 0) continue;
      // else set each pixel in the blocks_matrix matrix according to the
      // block_type template
      for (size_t k = 0; k < sprite_size; ++k) {
        for (size_t l = 0; l < sprite_size; ++l) {
          blocks_matrix[(i - block_field_hidden_rows) * sprite_size + k]
                       [j * sprite_size + l] = block_types[blocks[i][j]][k][l];
        }
      }
    }
  }
  addToNewFrame(blocks_matrix, block_offset_x, block_offset_y);
}

void printer::Printer::addNext() {
  // same principal as in addBlocks() above, just adding the tetromino in the
  // next slot
  if (this->nextTetromino == nullptr) return;
  auto nextTetromino = this->nextTetromino->currentShape(true);
  std::vector<std::vector<char>> blocks_matrix(
      5 * sprite_size, std::vector<char>(5 * sprite_size, code_transparent));
  for (size_t i = 0; i < 5; ++i) {
    for (size_t j = 0; j < 5; ++j) {
      if (nextTetromino[i][j] == 0) continue;
      for (size_t k = 0; k < sprite_size; ++k) {
        for (size_t l = 0; l < sprite_size; ++l) {
          blocks_matrix[i * sprite_size + k][j * sprite_size + l] =
              block_types[nextTetromino[i][j]][k][l];
        }
      }
    }
  }
  std::vector<std::vector<char>> black_background(
      16, std::vector<char>(32, code_black));

  addToNewFrame(black_background, 192, 112);
  if (!hide_next_bool) {
    addToNewFrame(blocks_matrix, this->nextTetromino->next_offset_x,
                  this->nextTetromino->next_offset_y);
  }
}

void printer::Printer::setScore(int64_t points) { current_score = points; }
void printer::Printer::setTopScore(int64_t points) { top_score = points; }
void printer::Printer::setLineCount(int16_t lines) { line_count = lines; }
void printer::Printer::setLevel(int16_t level) {
  int16_t modlvl = level % 256;
  if (current_level == modlvl) return;
  current_level = level;
  colors_256['c'] = level_colors[modlvl][1];
  colors_256['d'] = level_colors[modlvl][2];
  colors_256['9'] = level_colors[modlvl][3];
  for (size_t i = 0; i < current_screen.size(); ++i) {
    for (size_t j = 0; j < current_screen[i].size(); ++j) {
      if (current_screen[i][j] == 'c' || current_screen[i][j] == 'd' ||
          current_screen[i][j] == '9') {
        current_screen[i][j] = code_transparent;
      }
    }
  }
}
void printer::Printer::setNextTetromino(
    std::shared_ptr<BaseTetromino> tetrominoShape) {
  nextTetromino = std::move(tetrominoShape);
}

void printer::Printer::initBackground(
    std::vector<std::vector<char>> background) {
  if (background.size() != screen_height ||
      background[0].size() != screen_width)
    return;
  new_screen = background;
  print(false);
}

void printer::Printer::setStatistics(std::vector<uint16_t> stats) {
  for (size_t i = 0; i < stats.size(); i++) {
    std::string c = numberToString(stats[i], 3);
    addString(c, 48, 88 + (16 * i));
  }
}

void printer::Printer::hide_next() { hide_next_bool = !hide_next_bool; }
