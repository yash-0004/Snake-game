#include <iostream>
#include <conio.h>  // for _kbhit() and _getch()
#include <windows.h> // for Sleep()

using namespace std;

// Define the screen size
const int width = 20;
const int height = 20;

class SnakeGame {
private:
    int x, y, fruitX, fruitY, score;
    int tailX[100], tailY[100];
    int nTail;
    bool gameOver;

    enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
    eDirection dir;

public:
    // Constructor to initialize the game
    SnakeGame() {
        Setup();
    }

    // Function to initialize the game
    void Setup() {
        gameOver = false;
        dir = STOP;  // Initially, the snake doesn't move
        x = width / 2;
        y = height / 2;
        fruitX = rand() % width;
        fruitY = rand() % height;
        score = 0;
        nTail = 2;  // Initial length of the snake

        // Initialize tail positions
        for (int i = 0; i < nTail; i++) {
            tailX[i] = x - i - 1;
            tailY[i] = y;
        }
    }

    // Function to draw the screen
    void Draw() {
        system("cls"); // Clear the console
        for (int i = 0; i < width + 2; i++) cout << "#"; // Top border
        cout << endl;

        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                if (j == 0) cout << "#"; // Left border
                if (i == y && j == x) cout << "O"; // Snake head
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
                case 'x': gameOver = true; break; // Quit game
                case 'r': Setup(); break; // Restart game
            }
        }
    }

    // Function to update the game state
    void Logic() {
        if (dir == STOP) return;  // If direction is STOP, don't move

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

        // Check for collision with walls
        if (x >= width || x < 0 || y >= height || y < 0) gameOver = true;

        // Check for collision with itself
        for (int i = 0; i < nTail; i++) {
            if (tailX[i] == x && tailY[i] == y) gameOver = true;
        }

        // Check if snake eats the fruit
        if (x == fruitX && y == fruitY) {
            score += 10;
            fruitX = rand() % width;
            fruitY = rand() % height;
            nTail++;  // Increase the snake length
        }
    }

    // Function to check if the game is over
    bool isGameOver() {
        return gameOver;
    }

    // Function to restart or quit after game over
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
};

int main() {
    // Display welcome message once
    cout << "Welcome! to this amazing Snake Game." << endl;
    cout << "Instructions: " << endl;
    cout << "PRESS 'w' = Move UP | PRESS 's' = Move DOWN | PRESS 'd' = Move RIGHT | PRESS 'a' = Move LEFT" << endl;
    cout << "PRESS 'x' to quit the game and PRESS 'r' to restart" << endl;
    cout << "If the snake touches the wall or its own tail, the game ends." << endl;

    // Wait for the user to start
    cout << "Press any key to start the game..." << endl;
    _getch(); // Wait for user input

    // Create a game object
    SnakeGame game;

    // Main game loop
    while (true) {
        while (!game.isGameOver()) {
            game.Draw();
            game.Input();
            game.Logic();
            Sleep(100); // Game speed
        }

        // Ask for restart when game ends
        game.RestartPrompt();

        // Exit if the game is over
        if (game.isGameOver()) {
            break;
        }
    }

    return 0;
}