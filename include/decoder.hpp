#ifndef TETRIS_DECODER
#define TETRIS_DECODER

#include <array>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

namespace decoder {
class Decoder {
 private:
  std::string read_file_hex(const std::string& filename);
  std::vector<std::string> split(const std::string& str, char delimiter);
  std::vector<std::uint8_t> splitLevelColor(const std::string& str,
                                            char delimiter);
  std::vector<std::vector<char>> decode(const std::string& filename);
  std::vector<std::vector<uint8_t>> decodeLevelColors(
      const std::string& filename);

 public:
  std::vector<std::vector<char>> background;
  std::vector<std::vector<char>> blocks;
  std::vector<std::vector<char>> background_flashing;
  std::vector<std::vector<char>> block1;
  std::vector<std::vector<char>> block2;
  std::vector<std::vector<char>> block3;
  std::map<int16_t, std::vector<std::vector<char>>> block_types;
  std::vector<std::vector<char>> spritesheet;
  std::vector<std::vector<std::uint8_t>> level_colors;
  Decoder();
};
}  // namespace decoder

#endif