# Tetris

## Project Description

This is a simple implementation of the classic game Tetris written in C using the SDL (Simple DirectMedia Layer) library for graphics and SDL_ttf for text rendering. The game follows the standard rules of Tetris where players manipulate falling tetrominoes, moving them sideways and rotating them, with the goal of creating horizontal lines without gaps. When a line is completed, it disappears, and any blocks above it fall down to fill the space.

### Features

- Graphical display using SDL for rendering.
- Integration with SDL_ttf for text rendering.
- Basic controls: Arrow keys to move tetrominoes horizontally and rotate them, with the ability to speed up the falling speed.
- Score tracking: Players earn points for each line cleared. The score increases as the game progresses.
- Game over condition: The game ends when the stack of tetrominoes reaches the top of the playfield.

### Rules

[Source - Tetris Wiki](https://tetris.fandom.com/wiki/Tetris_Guideline)

- Playfield:

  - 10x20 grid

- Tetromino pieces and colors:
  - Cyan I
  - Yellow O
  - Purple T
  - Green S
  - Red Z
  - Blue J
  - Orange L

![Tetris Pieces](./images/tetris_pieces.png)

- Tetromino Start Locations:

  - I and O spawn in middle columns
  - Rest spawn in left-middle columns
  - Spawn horizontally with J, L, T spawning flat-side first

[Source - tetris.com](https://tetris.com/play-tetris)

- Keyboard mappings:

  - SPACE: hard drop
  - Left Arrow / Right Arrow: move left / right
  - Down Arrow: soft drop
  - Up Arrow: rotate right
  - ESC: exit program

- Scoring: ![Tetris Scoring](./images/tetris_controls_and_scoring.png)

## Dependencies

- SDL (Simple DirectMedia Layer): [SDL Website](https://www.libsdl.org/)
- SDL_ttf: [SDL_ttf Website](https://www.libsdl.org/projects/SDL_ttf/)

  On ubuntu the following commands work:

  `sudo apt-get install libsdl2-dev`

  `sudo apt-get install libsdl2-ttf-dev`

## Compiling and Running

1. Install SDL and SDL_ttf libraries on your system.
2. Clone the repository to your local machine.
3. Compile the source code using a C compiler with SDL and SDL_ttf flags. We used cmake for compilation.
4. Run the executable from the build folder.

   `./src/main`

## How to Play

- Use the arrow keys to move and rotate the falling tetrominoes.
- Try to complete horizontal lines to clear them and earn points.
- Be careful not to let the stack of tetrominoes reach the top of the playfield, or the game will end.

## Testing Approach

Due to the nature of functions requiring user input, traditional unit testing was replaced with extensive user testing methods:

Manual Testing: Team members and external testers extensively played the game, focusing on gameplay mechanics, user interface, and overall experience.

Playtesting Sessions: Scheduled sessions with specific tasks provided valuable feedback on gameplay, controls, and difficulty levels.
