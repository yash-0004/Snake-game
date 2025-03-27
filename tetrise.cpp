#include <iostream>
#include <conio.h>
#include <vector>
#include <windows.h>
#include <ctime>
#define BLOCK 'O'  // Smaller block representation

using namespace std;

const int WIDTH = 10;
const int HEIGHT = 20;

char board[HEIGHT][WIDTH];
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
COORD cursorPosition = {0, 0};

struct Tetromino {
    vector<vector<int>> shape;
    int x, y;
};

vector<vector<int>> tetrominoes[7] = {
    {{0, 0, 0, 0},
     {1, 1, 1, 1},
     {0, 0, 0, 0}},

    {{0, 0, 0, 0},
     {0, 1, 1, 0},
     {0, 1, 1, 0}},

    {{0, 1, 0, 0},
     {0, 1, 0, 0},
     {1, 1, 1, 0}},

    {{0, 1, 1, 0},
     {1, 1, 0, 0},
     {0, 0, 0, 0}},

    {{1, 1, 0, 0},
     {0, 1, 1, 0},
     {0, 0, 0, 0}},

    {{1, 1, 1, 0},
     {0, 0, 1, 0},
     {0, 0, 1, 0}},

    {{1, 1, 1, 0},
     {1, 0, 0, 0},
     {1, 0, 0, 0}}
};

Tetromino currentPiece, nextPiece;
bool gameOver = false;
bool isPaused = false;
int score = 0, level = 1, linesCleared = 0;
int speed = 700;

void InitBoard() {
    for (int i = 0; i < HEIGHT; i++)
        for (int j = 0; j < WIDTH; j++)
            board[i][j] = ' ';
}

void DrawBoard() {
    cursorPosition = {0, 0};
    SetConsoleCursorPosition(hConsole, cursorPosition);
    char tempBoard[HEIGHT][WIDTH];
    memcpy(tempBoard, board, sizeof(board));
    
    for (int i = 0; i < currentPiece.shape.size(); i++) {
        for (int j = 0; j < currentPiece.shape[i].size(); j++) {
            if (currentPiece.shape[i][j]) {
                int y = currentPiece.y + i;
                int x = currentPiece.x + j;
                if (y >= 0 && y < HEIGHT && x >= 0 && x < WIDTH)
                    tempBoard[y][x] = (char)BLOCK;
            }
        }
    }
    
    for (int i = 0; i < HEIGHT; i++) {
        cout << "|";
        for (int j = 0; j < WIDTH; j++)
            cout << (tempBoard[i][j] == ' ' ? '.' : tempBoard[i][j]);
        cout << "|\n";
    }
    
    cout << "Score: " << score << "  Level: " << level << endl;
    if (isPaused) cout << "GAME PAUSED - Press 'P' to Resume" << endl;
}

// Check if piece can move or rotate
bool CanMove(int dx, int dy, const vector<vector<int>>& shape) {
    for (int i = 0; i < shape.size(); i++) {
        for (int j = 0; j < shape[i].size(); j++) {
            if (shape[i][j]) {
                int newX = currentPiece.x + j + dx;
                int newY = currentPiece.y + i + dy;
                if (newX < 0 || newX >= WIDTH || newY >= HEIGHT || (newY >= 0 && board[newY][newX] != ' '))
                    return false;
            }
        }
    }
    return true;
}

// Clears full lines and updates score/level
void ClearLines() {
    int lines = 0;
    for (int i = HEIGHT - 1; i >= 0; i--) {
        bool full = true;
        for (int j = 0; j < WIDTH; j++) {
            if (board[i][j] == ' ') {
                full = false;
                break;
            }
        }
        if (full) {
            lines++;
            // Shift down
            for (int k = i; k > 0; k--) {
                for (int j = 0; j < WIDTH; j++) {
                    board[k][j] = board[k - 1][j];
                }
            }
            // Clear top row
            for (int j = 0; j < WIDTH; j++) {
                board[0][j] = ' ';
            }
            i++; // Recheck this row after shifting
        }
    }
    if (lines > 0) {
        linesCleared += lines;
        score += lines * 100;
        if (linesCleared % 5 == 0) {
            level++;
            if (speed > 100) speed -= 50;  // Increase game speed
        }
    }
}

void PlacePiece() {
    for (int i = 0; i < currentPiece.shape.size(); i++) {
        for (int j = 0; j < currentPiece.shape[i].size(); j++) {
            if (currentPiece.shape[i][j])
                board[currentPiece.y + i][currentPiece.x + j] = (char)BLOCK;
        }
    }
    ClearLines();  // Check for completed lines
    currentPiece = nextPiece;
    nextPiece = {tetrominoes[rand() % 7], WIDTH / 2 - 2, 0};
    if (!CanMove(0, 0, currentPiece.shape))
        gameOver = true;
}

void MovePiece(int dx, int dy) {
    if (CanMove(dx, dy, currentPiece.shape)) {
        currentPiece.x += dx;
        currentPiece.y += dy;
    } else if (dy == 1) {
        PlacePiece();
    }
}

void RotatePiece() {
    vector<vector<int>> rotated;
    int rows = currentPiece.shape.size();
    int cols = currentPiece.shape[0].size();
    rotated.resize(cols, vector<int>(rows, 0));
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            rotated[j][rows - i - 1] = currentPiece.shape[i][j];
    if (CanMove(0, 0, rotated))
        currentPiece.shape = rotated;
}

void GameLoop() {
    while (!gameOver) {
        DrawBoard();
        if (_kbhit()) {
            char ch = _getch();
            if (ch == 27) break; // Escape to quit
            if (ch == 'p' || ch == 'P') {
                isPaused = !isPaused;
                if (isPaused) {
                    DrawBoard();
                }
                while (isPaused) {
                    if (_kbhit()) {
                        char pauseKey = _getch();
                        if (pauseKey == 'p' || pauseKey == 'P') {
                            isPaused = false;
                        }
                    }
                    Sleep(100);  // Reduce CPU usage while paused
                }
            }
            if (!isPaused && ch == -32) { // Arrow keys
                ch = _getch();
                if (ch == 75) MovePiece(-1, 0); // Left
                if (ch == 77) MovePiece(1, 0);  // Right
                if (ch == 80) MovePiece(0, 1);  // Down
                if (ch == 72) RotatePiece();    // Up
            } else if (!isPaused && ch == ' ') {
                while (CanMove(0, 1, currentPiece.shape))
                    MovePiece(0, 1);
                PlacePiece();
            }
        }
        if (!isPaused) {
            Sleep(speed);
            MovePiece(0, 1);
        }
    }
    DrawBoard();
    cout << "Game Over! Final Score: " << score << "\n";
}

int main() {
    cout << "CLICK EVERY KEY ONLY ONCE\n";
    srand(time(0));
    InitBoard();
    currentPiece = {tetrominoes[rand() % 7], WIDTH / 2 - 2, 0};
    nextPiece = {tetrominoes[rand() % 7], WIDTH / 2 - 2, 0};
    GameLoop();
    return 0;
}
