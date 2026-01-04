# =========================================================
# MinesweeperC
# =========================================================

A graphical Minesweeper game written in C++ using SFML, demonstrating advanced C++ programming, 2D game logic, and interactive GUI design.

# ---------------------------------------------------------
# Features
# ---------------------------------------------------------

- Classic Minesweeper gameplay with graphical interface
- Random mine placement via the smiley face button
- Manual and custom board support:
    - Configure board size and number of mines via `config.cfg` in `SFML_Demo\boards`
    - Load predefined boards using `.brd` files in the `boards` folder
    - In-game test/debug boards can be loaded via buttons for rapid testing
- Interactive gameplay:
    - Left-click tiles to reveal safe spaces or number of adjacent mines
    - Right-click to place or toggle flags on suspected mines
    - Automatic recursive revealing of empty areas (flood-fill)
    - Game state visual feedback (smiley face updates to win/lose)
- Dynamic mine counter:
    - Three-digit counter displays remaining mines (mines minus flags) in real time
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

- Modify `config.cfg` to change board dimensions and number of mines
- Create custom board layouts by adding `.brd` files to the `boards` folder
- The game automatically loads mines and numbers based on your board file

# ---------------------------------------------------------
# Gameplay
# ---------------------------------------------------------

- Left-click tiles to reveal safe areas
- Right-click tiles to place or remove flags
- Smiley face resets the board and reflects current game state (happy, win, lose)
- Revealed tiles show the number of adjacent mines
- Flood-fill algorithm automatically clears empty areas

# ---------------------------------------------------------
# Concepts Demonstrated
# ---------------------------------------------------------

- Advanced C++ programming (2D arrays, vectors, modular design)
- Object-oriented principles in a game environment
- File input/output for configuration and custom board loading
- Event-driven programming with SFML
- Real-time rendering and interactive GUI design
- Recursive algorithms (flood-fill) for game logic
- Dynamic UI elements, including counters and state-dependent visuals
