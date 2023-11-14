#ifndef TETRIS_INPUT
#define TETRIS_INPUT

#include <signal.h>
#include <stdio.h>
#include <stdio_ext.h>
#include <termios.h>
#include <unistd.h>

#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

#define A 120
#define B 121
#define START 10
#define SELECT 9
#define UP 65
#define DOWN 66
#define RIGHT 67
#define LEFT 68

namespace input {
class Input {
 private:
  termios old_tio;
  termios new_tio;
  std::queue<int> q;
  std::mutex mtx;

 public:
  Input();
  void operator()();
  std::vector<int> getInput();
  std::shared_ptr<bool> getExitPointer();
  void restoreTerminal();
};
}  // namespace input

#endif
