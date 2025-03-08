# TetrisMZ

TetrisMZ is a simple implementation of the classic Tetris game, built in C using SDL2. This project is designed to be cross-platform and provides basic functionality like scoring, level progression, and a menu system.

## Features
- Classic Tetris gameplay, but only with 8 blocks on the bottom.
- Menu with options:
    - New Game
    - View Highest Scores
    - Controls
    - Quit
- Save and display top 10 highest scores.
- Smooth graphics rendering using SDL2.
- Portable build system using **Makefile**

## Requirements
### Dependencies
Ensure the following libraries are installed on your system:
- SDL2
- SDL2_ttf
- SDL2_image

### Installation Guide
**Linux (Debian/Ubuntu):**
```bash
sudo apt install libsdl2-dev libsdl2-ttf-dev libsdl2-image-dev
```
**Linux (Fedora):**
```bash
sudo dnf install SDL2-devel SDL2_ttf-devel SDL2_image-devel
```
**macOS (homebrew has to be installed):**
```bash
brew install sdl2 sdl2_ttf sdl2_image
```
**Windows:**
Download and install precompiled SDL2 libraries from [libsdl.org](https://www.libsdl.org/)

## Build and Run
### Building the Game
To build the game, simply run the command below in your terminal of choice:
```bash
make
```
If your SDL2 installation is in a custom location, you can specify the paths as below:
```bash
make SDL2_CFLAGS="-Icustom/include" SDL2_LIBS="-Lcustom/lib -lSDL2 -lSDL2_ttf -lSDL2_image"
```
### Running the Game
After building, run the game and enjoy:
```bash
./tetris
```

## Controls
- **A**: Move tetromino left.
- **D**: Move tetromino right.
- **S**: Speed up the fall of the tetromino.
- **R**: Rotate the tetromino.

## High Scores
- The top 10 high scores are saved automatically.
- You can view them from the main menu, by pressing Highest scores button.

## Development
### Folder Structure
```
tetrisMZ/
├── src/          # Source files
├── obj/          # Compiled object files (generated)
├── resources/    # Font and Tetris logo image
├── README.md     # Project documentation
├── tetris     # The executable to run the game (generated)
└── Makefile      # Build configuration
```

### Key files
- **src/game.c**: Core game loop and state management.
- **src/render.c**: Rendering functions.
- **src/logic.c**: Game logic and collision detection.
- **src/highscores.c**: High score management
- **src/handlers.c**: Input handling.

## Cleaning and Rebuilding
- Clean build artifacts:
```bash
make clean
```
- Clean and rebuild:
```bash
make rebuild
```