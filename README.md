# =========================================================
# MinesweeperC
# =========================================================

A command-line/graphical Minesweeper game written in C++ using SFML.

# ---------------------------------------------------------
# Features
# ---------------------------------------------------------

- Classic Minesweeper gameplay with graphical interface using SFML
- Random mine placement (press the smiley face to reset)
- Manual and custom board support:
    - Configure board size and number of mines via `config.cfg` in `SFML_Demo\boards`
    - Manual boards supported via `testboard` files in the `boards` folder
- Interactive gameplay:
    - Click tiles to reveal safe spaces or adjacent mine counts
    - Place flags on suspected mines
    - Automatic revealing of adjacent empty tiles
    - Game over when a mine is clicked
- Full visual feedback and expected Minesweeper mechanics

# ---------------------------------------------------------
# Getting Started
# ---------------------------------------------------------

Requirements:
- C++17 compatible compiler
- SFML library installed
- Optional: Visual Studio solution (`SFML_Demo.sln`) included for easy building

Building the game:
1. Open `SFML_Demo.sln` in Visual Studio (or configure in your preferred IDE)
2. Build the project
3. Run the executable

# ---------------------------------------------------------
# Customization
# ---------------------------------------------------------

- Edit `config.cfg` to change board dimensions and number of mines
- Create your own board layouts by adding `.brd` files in the `boards` folder
- Mines and numbers will be automatically loaded based on your board file

# ---------------------------------------------------------
# Gameplay
# ---------------------------------------------------------

- Left-click to reveal a tile
- Right-click to place a flag
- Use the smiley face button to reset the board
- The game shows the number of adjacent mines for each revealed tile
- Reveals empty areas automatically if no mines are adjacent

# ---------------------------------------------------------
# Concepts Demonstrated
# ---------------------------------------------------------

- C++ object-oriented programming
- 2D game logic and board management
- File input for custom board loading
- Graphics handling with SFML
- Event-driven input and rendering
- User interface design and interactivity
