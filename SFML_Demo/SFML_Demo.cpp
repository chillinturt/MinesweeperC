#include <SFML/Graphics.hpp>
#include <iostream>
#include "Random.h"
#include <unordered_map>
#include "TextureManager.h"
#include <fstream>
#include <vector>

using namespace std;
using namespace sf;

int width, height, numMines, gameState;
string path = "boards/";
vector<vector<Sprite>> boardHidden; // 2D vector to store the sprites
vector<vector<Sprite>> boardRevealed;
vector<vector<Sprite>> boardMines;  // 2D vector to store mines
vector<vector<Sprite>> boardNumbers;  // 2D vector to store number sprites
vector<vector<bool>> visible; // 2D vector to track sprite visibility
vector<vector<bool>> minesVisible; // 2D vector to track mine visibility
vector<vector<bool>> numbersVisible; // 2D vector to track number sprite visibility
vector<vector<bool>> flagged; // 2D vector to track flagged tiles

// Declare sprites for the digits in the counter
Sprite digitSprites[3];

void initializeDigitSprites() {
    // Set the position of the digit sprites on the screen
    for (int i = 0; i < 3; ++i) {
        digitSprites[i].setTexture(TextureManager::GetTexture("digits"));
        digitSprites[i].setPosition(32.0f * i, static_cast<float>(height * 32 + 16)); // Adjust position as needed
    }
}

void updateDigitSprites(int number) {
    // Handle negative numbers (if needed)
    bool isNegative = number < 0;
    if (isNegative) {
        number = -number; // Make the number positive for calculation
    }

    int hundreds = (number / 100) % 10;
    int tens = (number / 10) % 10;
    int ones = number % 10;

    // Set texture rects for each digit
    if (isNegative) {
        digitSprites[0].setTextureRect(IntRect(10 * 21, 0, 21, 32)); // "-" sign
    }
    else {
        digitSprites[0].setTextureRect(IntRect(hundreds * 21, 0, 21, 32)); // Hundreds place
    }
    digitSprites[1].setTextureRect(IntRect(tens * 21, 0, 21, 32)); // Tens place
    digitSprites[2].setTextureRect(IntRect(ones * 21, 0, 21, 32)); // Ones place
}


void readConfig(const string& filename) {
    string fullPath = path + filename;
    ifstream inputFile(fullPath);
    if (inputFile.is_open()) {
        inputFile >> width >> height >> numMines;
        inputFile.close();
    }
    else {
        cerr << "Unable to open file" << endl;
        exit(1); // Exit the program with an error code
    }
}

void generateBoard(int w, int h) {
    boardHidden.resize(h, vector<Sprite>(w));
    boardRevealed.resize(h, vector<Sprite>(w));
    boardMines.resize(h, vector<Sprite>(w));
    boardNumbers.resize(h, vector<Sprite>(w));
    visible.resize(h, vector<bool>(w, true)); // Initialize all sprites as visible
    minesVisible.resize(h, vector<bool>(w, false)); // Initialize all mines as hidden
    numbersVisible.resize(h, vector<bool>(w, false)); // All numbers are initially hidden
    flagged.resize(h, vector<bool>(w, false)); // Initialize all tiles as unflagged

    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; ++x) {
            Sprite tile(TextureManager::GetTexture("tile_revealed"));
            tile.setPosition(static_cast<float>(x * 32), static_cast<float>(y * 32));
            boardRevealed[y][x] = tile;
        }
    }

    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; ++x) {
            Sprite tileHidden(TextureManager::GetTexture("tile_hidden"));
            tileHidden.setPosition(static_cast<float>(x * 32), static_cast<float>(y * 32));
            boardHidden[y][x] = tileHidden;

            Sprite mine(TextureManager::GetTexture("mine"));
            mine.setPosition(static_cast<float>(x * 32), static_cast<float>(y * 32));
            boardMines[y][x] = mine;
        }
    }

    // Initialize number sprites
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; ++x) {
            Sprite number;
            number.setPosition(static_cast<float>(x * 32), static_cast<float>(y * 32));
            boardNumbers[y][x] = number;
        }
    }
}

bool isMouseOverSprite(const Sprite& sprite, const Vector2i& mousePosition) {
    FloatRect bounds = sprite.getGlobalBounds();
    return bounds.contains(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y));
}

void floodFill(int x, int y) {
    // Check bounds
    if (x < 0 || x >= width || y < 0 || y >= height) return;
    // Check if the tile is already revealed or flagged
    if (!visible[y][x] || flagged[y][x]) return;

    // Mark the tile as revealed
    visible[y][x] = false;

    // Calculate the number of adjacent mines
    int dx[] = { -1, -1, -1, 0, 0, 1, 1, 1 };
    int dy[] = { -1, 0, 1, -1, 1, -1, 0, 1 };
    int mineCount = 0;
    bool adjacentFlag = false;

    for (int i = 0; i < 8; ++i) {
        int nx = x + dx[i];
        int ny = y + dy[i];
        if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
            if (minesVisible[ny][nx]) {
                mineCount++;
            }
            if (flagged[ny][nx]) {
                adjacentFlag = true;
            }
        }
    }

    // Stop the flood fill if there's a flagged tile adjacent to the current tile
    if (adjacentFlag) {
        return;
    }

    if (mineCount == 0) {
        // Set number texture to be blank (or a visible default texture)
        boardNumbers[y][x].setTexture(Texture());
        numbersVisible[y][x] = false;

        // Continue flooding the surrounding tiles
        for (int i = 0; i < 8; ++i) {
            int nx = x + dx[i];
            int ny = y + dy[i];
            floodFill(nx, ny);
        }
    }
    else {
        // Set the number texture based on the count of adjacent mines
        string textureName = "number_" + to_string(mineCount);
        boardNumbers[y][x].setTexture(TextureManager::GetTexture(textureName));
        numbersVisible[y][x] = true;

        cout << "Setting number texture for (" << x << ", " << y << ") to " << textureName << endl; // Debug line
    }
}

void revealMines() {
    // Reveal mines based on minesVisible vector
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (minesVisible[y][x]) {
                visible[y][x] = false; // Ensure mines are revealed
            }
        }
    }
    cout << "Mines revealed on the board." << endl;
}

bool checkWinCondition() {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            // If a tile is not a mine and is still hidden, the player hasn't won yet
            if (!minesVisible[y][x] && visible[y][x]) {
                return false;
            }
        }
    }
    return true; // All non-mine tiles are revealed
}

void handleMouseClick(const Vector2i& mousePosition) {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (isMouseOverSprite(boardHidden[y][x], mousePosition) && visible[y][x]) {
                if (gameState == 1 || gameState == 2) return;
                if (flagged[y][x]) {
                    cout << "Tile at (" << x << ", " << y << ") is flagged. Cannot reveal." << endl;
                    return; // Do nothing if the tile is flagged
                }

                if (minesVisible[y][x]) {
                    // Mine is clicked, set the sad face
                    gameState = 1;
                    revealMines();
                }
                else {
                    cout << "Clicked on tile at (" << x << ", " << y << ")" << endl;
                    floodFill(x, y);

                    // Check for win condition
                    if (checkWinCondition()) {
                        gameState = 2; // Set the game state to win

                        // Flag remaining hidden tiles that are not mines
                        for (int yy = 0; yy < height; ++yy) {
                            for (int xx = 0; xx < width; ++xx) {
                                if (visible[yy][xx] && minesVisible[yy][xx]) {
                                    flagged[yy][xx] = true; // Flag the tile
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}


void handleRightClick(const Vector2i& mousePosition) {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (isMouseOverSprite(boardHidden[y][x], mousePosition) && visible[y][x]) {
                flagged[y][x] = !flagged[y][x]; // Toggle the flag state
                cout << "Flag toggled at (" << x << ", " << y << ")" << endl;
            }
        }
    }
}

void resetBoard() {
    // Reset all tiles to visible and hide all numbers
    gameState = 0;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            visible[y][x] = true;
            minesVisible[y][x] = false; // Hide mines on reset
            numbersVisible[y][x] = false; // Hide numbers on reset
            boardNumbers[y][x].setTexture(Texture()); // Reset number textures to blank
            flagged[y][x] = false;
        }
    }
    generateBoard(width, height);
    cout << "Board reset: all tiles are now visible, mines hidden, and numbers cleared." << endl;
}


void calculateNumbers() {
    int dx[] = { -1, -1, -1, 0, 0, 1, 1, 1 };
    int dy[] = { -1, 0, 1, -1, 1, -1, 0, 1 };

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (minesVisible[y][x]) continue;

            int mineCount = 0;
            for (int i = 0; i < 8; ++i) {
                int nx = x + dx[i];
                int ny = y + dy[i];

                if (nx >= 0 && nx < width && ny >= 0 && ny < height && minesVisible[ny][nx]) {
                    mineCount++;
                }
            }

            if (mineCount > 0) {
                string textureName = "number_" + to_string(mineCount);
                boardNumbers[y][x].setTexture(TextureManager::GetTexture(textureName));
                numbersVisible[y][x] = true;
            }
            else {
                boardNumbers[y][x].setTexture(Texture());
                numbersVisible[y][x] = false;
            }
        }
    }
}

void placeRandomMines() {
    resetBoard();  // Start by resetting the board
    int minesPlaced = 0;

    while (minesPlaced < numMines) {
        int x = Random::Int(0, width - 1);
        int y = Random::Int(0, height - 1);

        // Check if a mine is already placed at this position
        if (!minesVisible[y][x]) {
            minesVisible[y][x] = true;
            minesPlaced++;
        }
    }

    calculateNumbers();  // After placing the mines, calculate the numbers
}

void loadMinesFromFile(const string& filename) {
    resetBoard();
    string fullPath = path + filename;
    ifstream inputFile(fullPath);
    if (inputFile.is_open()) {
        string line;
        int y = 0;
        while (getline(inputFile, line) && y < height) {
            for (int x = 0; x < width; ++x) {
                if (line[x] == '1') {
                    minesVisible[y][x] = true; // Mark this tile as containing a mine
                }
            }
            ++y;
        }
        inputFile.close();
    }
    else {
        cerr << "Unable to open mine file" << endl;
        exit(1); // Exit the program with an error code
    }
    calculateNumbers();
}

int countVisibleMines() {
    int visibleMinesCount = 0;

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (minesVisible[y][x]) {
                visibleMinesCount++;
            }
        }
    }
    return visibleMinesCount;
}

int countFlags() {
    int flagCount = 0;

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (flagged[y][x]) {
                flagCount++;
            }
        }
    }

    return flagCount;
}

int main()
{
    cout << "Program started" << endl;

    readConfig("config.cfg");

    cout << "Width: " << width << ", Height: " << height << ", Mines: " << numMines << endl;
    generateBoard(width, height);

    sf::RenderWindow window(sf::VideoMode(width * 32, (height * 32) + 100), "SFML works!");

    initializeDigitSprites();

    // Button sprites
    sf::Sprite currentFace(TextureManager::GetTexture("face_happy"));
    currentFace.setPosition(368, 512);
    currentFace.setScale(1, 1);

    sf::Sprite debug(TextureManager::GetTexture("debug"));
    debug.setPosition(544, 512);
    debug.setScale(1, 1);

    sf::Sprite test1(TextureManager::GetTexture("test_1"));
    test1.setPosition(608, 512);
    test1.setScale(1, 1);

    sf::Sprite test2(TextureManager::GetTexture("test_2"));
    test2.setPosition(672, 512);
    test2.setScale(1, 1);

    sf::Sprite test3(TextureManager::GetTexture("test_3"));
    test3.setPosition(736, 512);
    test3.setScale(1, 1);

    // numbers
    sf::Sprite number1(TextureManager::GetTexture("number_1"));
    sf::Sprite number2(TextureManager::GetTexture("number_2"));
    sf::Sprite number3(TextureManager::GetTexture("number_3"));
    sf::Sprite number4(TextureManager::GetTexture("number_4"));
    sf::Sprite number5(TextureManager::GetTexture("number_5"));
    sf::Sprite number6(TextureManager::GetTexture("number_6"));
    sf::Sprite number7(TextureManager::GetTexture("number_7"));
    sf::Sprite number8(TextureManager::GetTexture("number_8"));

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    Vector2i mousePosition = sf::Mouse::getPosition(window);

                    if (isMouseOverSprite(currentFace, mousePosition)) {
                        placeRandomMines();
                    }
                    else if (isMouseOverSprite(test1, mousePosition)) {
                        loadMinesFromFile("testboard1.brd");
                    }
                    else if (isMouseOverSprite(test2, mousePosition)) {
                        loadMinesFromFile("testboard2.brd");
                    }
                    else if (isMouseOverSprite(test3, mousePosition)) {
                        loadMinesFromFile("testboard3.brd");
                    }
                    else if (isMouseOverSprite(debug, mousePosition)) {
                        revealMines();
                    }
                    else {
                        handleMouseClick(mousePosition);
                    }
                }
            }
            if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
                Vector2i mousePosition = sf::Mouse::getPosition(window);
                handleRightClick(mousePosition);
            }

        }
        updateDigitSprites(countVisibleMines() - countFlags());

        // Update currentFace based on gameState
        if (gameState == 0) {
            currentFace.setTexture(TextureManager::GetTexture("face_happy"));
        }
        else if (gameState == 1) {
            currentFace.setTexture(TextureManager::GetTexture("face_lose"));
        }
        else if (gameState == 2) {
            currentFace.setTexture(TextureManager::GetTexture("face_win"));
        }

        window.clear();
        window.draw(currentFace);
        window.draw(debug);
        window.draw(test1);
        window.draw(test2);
        window.draw(test3);

        // Draw revealed tiles
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                if (!visible[y][x]) {
                    window.draw(boardRevealed[y][x]);
                }
            }
        }

        // Draw mines
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                if (minesVisible[y][x] && !visible[y][x]) {
                    window.draw(boardMines[y][x]);
                }
            }
        }

        // Draw numbers
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                if (numbersVisible[y][x]) {
                    window.draw(boardNumbers[y][x]);
                }
            }
        }

        // Draw hidden tiles last
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                if (visible[y][x]) {
                    window.draw(boardHidden[y][x]);
                }
            }
        }

        // Draw flags
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                if (flagged[y][x] && visible[y][x]) {
                    Sprite flag(TextureManager::GetTexture("flag"));
                    flag.setPosition(static_cast<float>(x * 32), static_cast<float>(y * 32));
                    window.draw(flag);
                }
            }
        }

        for (int i = 0; i < 3; ++i) {
            window.draw(digitSprites[i]);
        }


        window.display();
    }

    cout << "Program ended" << endl;

    // Clear out any textures before the program ends
    TextureManager::Clear();

    return 0;
}

