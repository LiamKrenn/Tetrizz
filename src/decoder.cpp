#include "../include/decoder.hpp"

#define code_newline 'f'
#define code_repetition 'e'

std::string decoder::Decoder::read_file_hex(const std::string& filename) {
  std::ifstream file(filename, std::ios::binary);
  std::ostringstream hexStream;
  hexStream << std::hex << std::setfill('0');
  char ch;
  while (file.get(ch)) {
    hexStream << std::setw(2)
              << static_cast<int>(static_cast<unsigned char>(ch));
  }
  return hexStream.str();
}

std::vector<std::uint8_t> decoder::Decoder::splitLevelColor(
    const std::string& str, char delimiter) {
  std::vector<std::uint8_t> tokens;
  std::string token;
  std::stringstream tokenStream(str);
  while (std::getline(tokenStream, token, delimiter)) {
    tokens.push_back(std::stoi(token));
  }
  return tokens;
}

std::vector<std::string> decoder::Decoder::split(const std::string& str,
                                                 char delimiter) {
  std::vector<std::string> tokens;
  std::string token;
  std::stringstream tokenStream(str);
  while (std::getline(tokenStream, token, delimiter)) {
    tokens.push_back(token);
  }
  return tokens;
}

std::vector<std::vector<char>> decoder::Decoder::decode(
    const std::string& filename) {
  std::string file_data = read_file_hex(filename);
  std::vector<std::string> output = split(file_data, code_newline);
  size_t width = std::stoi(output[0]);
  size_t heigth = std::stoi(output[1]);
  output.erase(output.begin(), output.begin() + 2);
  if (output.back() == "") {
    output.pop_back();
  }

  std::vector<std::vector<char>> coutput(heigth, std::vector<char>(width));
  coutput.reserve(heigth);

  for (const std::string& line : output) {
    std::vector<char> newline(width, ' ');
    int j = 0;
    for (size_t i = 0; i < line.length(); ++i) {
      char c = line[i];
      if (c == code_repetition) {
        std::fill(newline.begin() + j,
                  newline.begin() + j + std::stoi(line.substr(i + 1, 2)),
                  line[i - 1]);
        j += std::stoi(line.substr(i + 1, 2));
        i += 2;
      } else {
        newline[j] = c;
        j++;
      }
    }
    coutput.erase(coutput.begin());
    coutput.emplace_back(newline);
  }

  return coutput;
}

std::vector<std::vector<uint8_t>> decoder::Decoder::decodeLevelColors(
    const std::string& filename) {
  std::vector<std::vector<uint8_t>> colors;
  std::ifstream file(filename);
  std::string str;
  while (std::getline(file, str)) {
    colors.push_back(splitLevelColor(str, ';'));
  }
  return colors;
}

decoder::Decoder::Decoder() {
  background = decode("assets/background.bin");
  blocks = decode("assets/blocks.bin");
  background_flashing = decode("assets/flashing.bin");
  block1 = decode("assets/block1.bin");
  block2 = decode("assets/block2.bin");
  block3 = decode("assets/block3.bin");
  block_types[1] = block1;
  block_types[2] = block2;
  block_types[3] = block3;
  spritesheet = decode("assets/spritesheet.bin");
  level_colors = decodeLevelColors("assets/level_colors.txt");
}
