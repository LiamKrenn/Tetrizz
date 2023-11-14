#include "../include/game.hpp"

using namespace decoder;
using namespace printer;
using namespace tetromino;
using namespace input;
using namespace tetris;

int last_rand = -1;

std::map<uint8_t, uint64_t> baseScorePerLine = {
    {1, 40},
    {2, 100},
    {3, 300},
    {4, 1200},
};

uint16_t getFPD(uint64_t level) {
  if (level >= 0 && level <= 9) {
    switch (level) {
      case 0:
        return 48;
        break;
      case 1:
        return 43;
        break;
      case 2:
        return 38;
        break;
      case 3:
        return 33;
        break;
      case 4:
        return 28;
        break;
      case 5:
        return 23;
        break;
      case 6:
        return 18;
        break;
      case 7:
        return 13;
        break;
      case 8:
        return 8;
        break;
      case 9:
        return 6;
        break;
      default:
        break;
    }
  } else if (level >= 10 && level <= 12) {
    return 5;
  } else if (level >= 13 && level <= 15) {
    return 4;
  } else if (level >= 16 && level <= 18) {
    return 3;
  } else if (level >= 19 && level <= 28) {
    return 2;
  }
  return 1;
}

bool validPosition(std::pair<int8_t, int8_t> coords,
                   const std::vector<std::vector<int8_t>>& shape,
                   const std::vector<std::vector<int16_t>>& field) {
  for (int16_t i = -2; i <= 2; i++) {
    for (int16_t j = -2; j <= 2; j++) {
      if (shape[i + 2][j + 2] == 0) continue;
      if ((0 > coords.second + i || coords.second + i >= block_field_height ||
           0 > coords.first + j || coords.first + j >= block_field_width) ||
          field[coords.second + i][coords.first + j] != 0) {
        return false;
      }
    }
  }
  return true;
}

Game::Game() {
  tetrominoCoords = {5, block_field_hidden_rows};
  for (size_t i = 0; i < block_field_height; i++) {
    block_field.emplace_back(std::vector<int16_t>(block_field_width, 0));
  }

  for (size_t i = 0; i < 7; i++) {
    statistics.push_back(0);
  }

  score = 0;
  line_count = 0;
  current_level = 7;
  down_press_score = 0;
  printer.initBackground(dec.background);
}

void Game::start(int start_level) {
  currentTetromino = randomPiece();
  nextTetromino = randomPiece();
  combineTetrominoField();
  current_level = start_level;
  setLevel(current_level);
  printer.setScore(score);
  printer.setTopScore(69420);
  printer.setLineCount(line_count);
  printer.setNextTetromino(nextTetromino);
  std::thread inputThread(std::ref(input));
  while (!*exit_program) {
    auto start = high_resolution_clock::now();

    bool pressedDown = false;
    for (int key : input.getInput()) {
      if (key == DOWN) {
        pressedDown = true;
      } else if (key == LEFT) {
        moveLeft();
      } else if (key == RIGHT) {
        moveRight();
      } else if (key == A) {
        rotateCW();
      } else if (key == B) {
        rotateCCW();
      } else if (key == SELECT) {
        printer.hide_next();
      }
    }

    // step
    if (pressedDown || (frameCount != 0 && frameCount % framesPerDrop == 0)) {
      moveDown();
      if (pressedDown) {
        down_press_score++;
        frameCount = 0;
      }
    };

    printer.setField(combineTetrominoField());
    printer.print();
    frameCount++;

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    std::this_thread::sleep_for(microseconds(16666) - duration);
  }

  input.restoreTerminal();
}

void Game::setLevel(uint64_t level) {
  current_level = level;
  framesPerDrop = getFPD(current_level);
  printer.setLevel(current_level);
}

void Game::addScore(uint64_t lines_cleard) {
  score += baseScorePerLine[lines_cleard] * (current_level + 1);
  printer.setScore(score);
}

void Game::clearLines(int rows, int start_index) {
  for (int i = start_index; i < start_index + rows; i++) {
    block_field.erase(block_field.begin() + i);
    block_field.insert(block_field.begin(),
                       std::vector<int16_t>(block_field_width, 0));
  }
  if ((line_count + rows) / 10 > line_count / 10 &&
      line_count + rows > current_level * 10) {
    setLevel(current_level + 1);
  }
  line_count += rows;
  printer.setLineCount(line_count);
}

void Game::lockTetromino() {
  if (tetrominoCoords.first == 5 &&
      tetrominoCoords.second == block_field_hidden_rows) {
    *exit_program = true;
    return;
  }

  score = score + down_press_score;
  printer.setScore(score);
  down_press_score = 0;

  block_field = combineTetrominoField();
  std::vector<std::pair<int8_t, int8_t>> line_clears;

  int previous = 0;
  for (size_t i = 0; i < block_field.size(); i++) {
    bool fullRow = true;

    for (size_t j = 0; j < block_field[i].size(); j++) {
      if (block_field[i][j] == 0) {
        fullRow = false;
        break;
      }
    }

    if (fullRow) {
      previous++;

    } else if (previous != 0) {
      clearLines(previous, i - previous);
      addScore(previous);
      previous = 0;
    }
  }

  if (previous != 0) {
    clearLines(previous, block_field.size() - previous);
    addScore(previous);
  }

  currentTetromino.swap(nextTetromino);
  nextTetromino = randomPiece();
  printer.setNextTetromino(nextTetromino);
  tetrominoCoords = {5, block_field_hidden_rows};
}

void Game::moveLeft() {
  tetrominoCoords.first--;
  if (!validPosition(tetrominoCoords, currentTetromino->currentShape(),
                     block_field)) {
    tetrominoCoords.first++;
  }
}

void Game::moveRight() {
  tetrominoCoords.first++;
  if (!validPosition(tetrominoCoords, currentTetromino->currentShape(),
                     block_field)) {
    tetrominoCoords.first--;
  };
}

void Game::moveDown() {
  tetrominoCoords.second++;
  if (!validPosition(tetrominoCoords, currentTetromino->currentShape(),
                     block_field)) {
    tetrominoCoords.second--;
    lockTetromino();
  };
}

void Game::rotateCW() {
  currentTetromino->rotate(true);
  if (!validPosition(tetrominoCoords, currentTetromino->currentShape(),
                     block_field)) {
    currentTetromino->rotate(false);
  }
}

void Game::rotateCCW() {
  currentTetromino->rotate(false);
  if (!validPosition(tetrominoCoords, currentTetromino->currentShape(),
                     block_field)) {
    currentTetromino->rotate(true);
  }
}

std::vector<std::vector<int16_t>> Game::combineTetrominoField() {
  auto shape = currentTetromino->currentShape();
  std::vector<std::vector<int16_t>> new_block_field(
      block_field_height, std::vector<int16_t>(block_field_width, 0));
  new_block_field = block_field;

  for (int16_t i = -2; i <= 2; i++) {
    for (int16_t j = -2; j <= 2; j++) {
      if (shape[i + 2][j + 2] == 0) continue;
      new_block_field[tetrominoCoords.second + i][tetrominoCoords.first + j] =
          shape[i + 2][j + 2];
    }
  }
  return new_block_field;
}

std::shared_ptr<BaseTetromino> Game::randomPiece() {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> distrib(0, 6);

  if (last_rand != -1) {
    statistics[last_rand]++;
  }

  int random_number = distrib(gen);
  if (random_number == last_rand) {
    random_number = distrib(gen);
  }
  last_rand = random_number;
  printer.setStatistics(statistics);

  switch (random_number) {
    case 0:
      return std::make_shared<BaseTetromino>(TTetromino());
      break;
    case 1:
      return std::make_shared<BaseTetromino>(JTetromino());
      break;
    case 2:
      return std::make_shared<BaseTetromino>(ZTetromino());
      break;
    case 3:
      return std::make_shared<BaseTetromino>(OTetromino());
      break;
    case 4:
      return std::make_shared<BaseTetromino>(STetromino());
      break;
    case 5:
      return std::make_shared<BaseTetromino>(LTetromino());
      break;
    case 6:
      return std::make_shared<BaseTetromino>(ITetromino());
      break;
  }
}
