#include <iostream>
#include <conio.h>  // for _kbhit() and _getch()
#include <windows.h> // for Sleep()

using namespace std;

// Define the screen size
const int width = 20;
const int height = 20;

// Declare variables
int x, y, fruitX, fruitY, score;
int tailX[100], tailY[100];
int nTail;
bool gameOver;
enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
eDirection dir;

// Function to initialize the game
void Setup() {
    gameOver = false;
    dir = STOP;  // Initially, the snake doesn't move
    x = width / 2;  // Initial x-coordinate of the snake's head
    y = height / 2; // Initial y-coordinate of the snake's head
    fruitX = rand() % width;
    fruitY = rand() % height;
    score = 0;
    nTail = 2;  // Set initial length of the snake to 3

    // Initialize the snake's tail positions (it starts with length 3)
    for (int i = 0; i < nTail; i++) {
        tailX[i] = x - i - 1;  // Tail positions are one unit to the left of the head initially
        tailY[i] = y;
    }
}

// Function to draw the screen
void Draw() {
    system("cls"); // clear the console
    for (int i = 0; i < width + 2; i++) cout << "#"; // Top border
    cout << endl;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (j == 0) cout << "#"; // Left border
            if (i == y && j == x) cout << "O"; // Snake headE
            else if (i == fruitY && j == fruitX) cout << "F"; // Fruit
            else {
                bool printTail = false;
                for (int k = 0; k < nTail; k++) {
                    if (tailX[k] == j && tailY[k] == i) {
                        cout << "o"; // Tail
                        printTail = true;
                        break;
                    }
                }
                if (!printTail) cout << " "; // Empty space
            }
            if (j == width - 1) cout << "#"; // Right border
        }
        cout << endl;
    }

    for (int i = 0; i < width + 2; i++) cout << "#"; // Bottom border
    cout << endl;
    cout << "Score: " << score << endl; // Display score
}

// Function to take user input for movement
void Input() {
    if (_kbhit()) {
        switch (_getch()) {
            case 'a': if (dir != RIGHT) dir = LEFT; break;
            case 'd': if (dir != LEFT) dir = RIGHT; break;
            case 'w': if (dir != DOWN) dir = UP; break;
            case 's': if (dir != UP) dir = DOWN; break;
            case 'x': gameOver = true; break; // Press 'x' to quit the game
            case 'r': gameOver = false; Setup(); break; // Press 'r' to restart the game
        }
    }
}

// Function to update the game state
void Logic() {
    if (dir == STOP) return;  // If the direction is STOP, the snake won't move

    int prevX = tailX[0], prevY = tailY[0];
    int prev2X, prev2Y;
    tailX[0] = x;
    tailY[0] = y;

    for (int i = 1; i < nTail; i++) {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    switch (dir) {
        case LEFT: x--; break;
        case RIGHT: x++; break;
        case UP: y--; break;
        case DOWN: y++; break;
        default: break;
    }

    // Check for collision with the wall
    if (x >= width || x < 0 || y >= height || y < 0) gameOver = true;

    // Check for collision with itself
    for (int i = 0; i < nTail; i++) {
        if (tailX[i] == x && tailY[i] == y) gameOver = true;
    }

    // Check if the snake eats the fruit
    if (x == fruitX && y == fruitY) {
        score += 10;
        fruitX = rand() % width;
        fruitY = rand() % height;
        nTail++;  // Increase the length of the snake after eating fruit
    }
}

// Function to prompt for restart after game over
void RestartPrompt() {
    char restartChoice;
    cout << "Game Over! Press 'r' to restart or 'x' to exit: " << endl;
    cout << "FINAL SCORE: " << score << endl;
    cin >> restartChoice;

    if (restartChoice == 'r') {
        Setup(); // Re-initialize the game state
    } else if (restartChoice == 'x') {
        gameOver = true; // Exit the game
    }
}

int main() {
    // Display welcome message once
    cout << "Welcome! to this amazing game where snake grows every time it's moved." << endl;
    cout << "Instructions of this game are: " << endl;
    cout << "PRESS 'w' = Move UP | PRESS 's' = Move DOWN | PRESS 'd' = Move Right | PRESS 'a' = Move Left" << endl;
    cout << "PRESS 'x' to quit this game and PRESS 'r' to restart this game" << endl; 
    cout << "If the snake touches the wall, the game ends and if the snake's mouth touches its tail, the game ends." << endl;

    // Wait for the user to press any key before starting the game
    cout << "Press any key to start the game..." << endl;
    _getch(); // Wait for the user to press a key

    // Main game loop
    while (true) {
        Setup();

        // Loop until game over
        while (!gameOver) {
            Draw();
            Input();
            Logic();
            Sleep(100); // Delay to make the game playable at a normal speed
        }

        // Ask for restart when the game ends
        RestartPrompt();

        // Exit if the user presses 'x'
        if (gameOver) {
            break;
        }
    }

    return 0;
}