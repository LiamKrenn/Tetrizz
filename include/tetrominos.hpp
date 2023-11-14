#ifndef TETRIS_TETROMINOS
#define TETRIS_TETROMINOS

#include <inttypes.h>

#include <vector>

namespace tetromino {
class BaseTetromino {
 protected:
  int8_t rotation;
  std::vector<std::vector<std::vector<int8_t>>> shapes;

 public:
  int16_t next_offset_x;
  int16_t next_offset_y;
  BaseTetromino();
  virtual ~BaseTetromino();
  void rotate(bool clockwise);
  std::vector<std::vector<int8_t>>& currentShape(bool next = false);
};

class TTetromino : public BaseTetromino {
 public:
  TTetromino();
};

class JTetromino : public BaseTetromino {
 public:
  JTetromino();
};

class ZTetromino : public BaseTetromino {
 public:
  ZTetromino();
};

class OTetromino : public BaseTetromino {
 public:
  OTetromino();
};

class STetromino : public BaseTetromino {
 public:
  STetromino();
};

class LTetromino : public BaseTetromino {
 public:
  LTetromino();
};

class ITetromino : public BaseTetromino {
 public:
  ITetromino();
};

}  // namespace tetromino
#endif
