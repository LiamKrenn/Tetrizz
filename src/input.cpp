#include "../include/input.hpp"

std::shared_ptr<bool> exit_program = std::make_shared<bool>(false);

void exitHandler(int) { *exit_program = true; }

input::Input::Input() {
  tcgetattr(STDIN_FILENO, &old_tio);  // save old terminal settings
  new_tio = old_tio;
  new_tio.c_lflag &=
      (~ICANON &
       ~ECHO);  // disable canonical mode (buffered i/o) and local echo
  tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);  // enable new settings

  struct sigaction act;
  act.sa_handler = exitHandler;
  sigaction(SIGINT, &act, NULL);
}

void input::Input::operator()() {
  while (!*exit_program) {
    char key = getchar();
    int translated;
    if (key == -1 || key == 195 || key == -61) {  // invalid, äöü...
      __fpurge(stdin);
      continue;
    } else if (key == 27) {   // Escape Codes (Arrows, del, pgup, pgdown, ...)
      if (getchar() == 91) {  // Arrow
        switch (getchar()) {  // get which arrow
          case UP:
            translated = UP;
            break;
          case DOWN:
            translated = DOWN;
            break;
          case RIGHT:
            translated = RIGHT;
            break;
          case LEFT:
            translated = LEFT;
            break;
          default:
            continue;
            break;
        }
      }
    } else {  // any other key
      translated = std::tolower(key);
    }
    __fpurge(stdin);
    std::lock_guard<std::mutex> lock(mtx);
    q.push(translated);
  }
  tcsetattr(STDIN_FILENO, TCSANOW, &old_tio);  // reset to old terminal settings
}

std::vector<int> input::Input::getInput() {
  std::lock_guard<std::mutex> lock(mtx);
  std::vector<int> inputs;
  while (!q.empty()) {
    inputs.push_back(q.front());
    q.pop();
  }
  return inputs;
}

std::shared_ptr<bool> input::Input::getExitPointer() { return exit_program; }

void input::Input::restoreTerminal() {
  tcsetattr(STDIN_FILENO, TCSANOW, &old_tio);
}
