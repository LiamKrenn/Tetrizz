# Tetrizz - Tetris for console

This is a clone of the famous NES-Tetris, that runs entirely in the command line in full resolution.

To achieve this, I've written a whole "graphics engine", that only updates the necessary pixels. This was necessary to achieve the constant 60 FPS, like the original NES console does.

<!-- vscode-markdown-toc -->

- [Features](#features)
- [Controls](#controls)
- [Roadmap](#roadmap)
- [Requirements](#requirements)
- [Issues](#issues)

<!-- vscode-markdown-toc-config
	numbering=false
	autoSave=false
	/vscode-markdown-toc-config -->
<!-- /vscode-markdown-toc -->

## <a name='Features'></a>Features

- Original scoring system
- Controls and speeds (drop speed, etc.) like the original
- Tetromino randomizer like the original
- Same bugs as original (intentionally)

## <a name='Controls'></a>Controls

- A: <kbd>x</kbd>
- B: <kbd>y</kbd>
- start: <kbd>⏎ Enter</kbd>
- select: <kbd>⇥ Tab</kbd>
- D-Pad: <kbd>↓</kbd> <kbd>↑</kbd> <kbd>←</kbd> <kbd>→</kbd>

## <a name='Roadmap'></a>Roadmap

- Finishing all animations
- Menu screen and end screen
- B-Mode
- (maybe capability for windows, although there are performance issues on the experimental builds for windows)

## <a name='Requirements'></a>Requirements

- Linux
- xterm 256 color support in your terminal of choice
  - It should be fast enough for any terminal, but if you experience performance problems, I recommend something like [alacritty](https://github.com/alacritty/alacritty) or [kitty](https://github.com/kovidgoyal/kitty)
- 256 characters in width and 112 characters in height
  - Zooming out in terminals is done with <kbd>Control</kbd> + <kbd>-</kbd> or <kbd>Control</kbd> + <kbd>⇧ Shift</kbd> + <kbd>-</kbd> most of the time

## <a name='Issues'></a>Issues

If you experience any problems, feel free to contact me!
