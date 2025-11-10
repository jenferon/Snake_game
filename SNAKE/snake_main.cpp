#include <iostream>
#include <ncurses.h>

using namespace std;

// define variables for the game
// height and width of the boundary
const int width = 60;
const int height = 30;

// Snake head coordinates of snake (x-axis, y-axis)
int x, y;
// Food coordinates
int fruitX, fruitY;
// variable to store the score of he player
int playerScore;
// Array to store the coordinates of snake tail (x-axis,
// y-axis)
int snakeTailX[100], snakeTailY[100];
// variable to store the length of the sanke's tail
int snakeTailLen;
// for storing snake's moving snakesDirection
enum snakesDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
// snakesDirection variable
snakesDirection sDir;
// boolean variable for checking game is over or not
bool isGameOver;

void GameInit()
{
    isGameOver = false;
    sDir = STOP;
    x = width / 2;
    y = height / 2;
    fruitX = rand() % width;
    fruitY = rand() % height;
    playerScore = 0;
}

// Function for creating the game board & rendering
void GameRender(string playerName)
{
    clear(); // Clear the console

    // Creating top walls with '-'
    for (int i = 0; i < width + 2; i++)
        mvaddch(0, i, '-');

    // Draw game area
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (j == 0)
                mvaddch(i + 1, j, '|');

            if (i == y && j == x)
                mvaddch(i + 1, j + 1, 'O'); // head
            else if (i == fruitY && j == fruitX)
                mvaddch(i + 1, j + 1, '#'); // fruit
            else {
                bool printTail = false;
                for (int k = 0; k < snakeTailLen; k++) {
                    if (snakeTailX[k] == j && snakeTailY[k] == i) {
                        mvaddch(i + 1, j + 1, 'o');
                        printTail = true;
                    }
                }
                if (!printTail)
                    mvaddch(i + 1, j + 1, ' ');
            }

            if (j == width - 1)
                mvaddch(i + 1, j + 2, '|');
        }
    }

    // Bottom wall
    for (int i = 0; i < width + 2; i++)
        mvaddch(height + 1, i, '-');

    // Display score
    mvprintw(height + 3, 0, "Score: %d", playerScore);

    refresh(); // update the screen
}

// Function for updating the game state
void UpdateGame()
{
    int prevX = snakeTailX[0];
    int prevY = snakeTailY[0];
    int prev2X, prev2Y;
    snakeTailX[0] = x;
    snakeTailY[0] = y;

    for (int i = 1; i < snakeTailLen; i++) {
        prev2X = snakeTailX[i];
        prev2Y = snakeTailY[i];
        snakeTailX[i] = prevX;
        snakeTailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    switch (sDir) {
        case LEFT:
            x--;
            break;
        case RIGHT:
            x++;
            break;
        case UP:
            y--;
            break;
        case DOWN:
            y++;
            break;
        default:
            break;
    }

    // Checks for snake's collision with the wall (|)
    if (x >= width || x < 0 || y >= height || y < 0)
        isGameOver = true;

    // Checks for collision with the tail (o)
    for (int i = 0; i < snakeTailLen; i++) {
        if (snakeTailX[i] == x && snakeTailY[i] == y)
            isGameOver = true;
    }

    // Checks for snake's collision with the food (#)
    if (x == fruitX && y == fruitY) {
        playerScore += 10;
        fruitX = rand() % width;
        fruitY = rand() % height;
        snakeTailLen++;
    }
}

// Function to handle user UserInput
void UserInput()
{
    

    int ch = getch();
    switch (ch) {
        case 'a':
        case 'A':
        case KEY_LEFT:
            sDir = LEFT;
            break;
        case 'd':
        case 'D':
        case KEY_RIGHT:
            sDir = RIGHT;
            break;
        case 'w':
        case 'W':
        case KEY_UP:
            sDir = UP;
            break;
        case 's':
        case 'S':
        case KEY_DOWN:
            sDir = DOWN;
            break;
        case 'x':
            isGameOver = true;
            break;
    }
        refresh();

}

int main(){
  string playerName;
    cout << "enter your name: ";
    cin >> playerName;

    GameInit();
    // Checks if a key is pressed or not
    initscr();            // Start ncurses mode
    cbreak();             // Disable line buffering
    noecho();             // Don't echo pressed keys
    keypad(stdscr, TRUE); // Enable arrow keys
    nodelay(stdscr, TRUE); // Non-blocking input
    while (!isGameOver) {
        GameRender(playerName);
        UserInput();
        UpdateGame();
        napms(150); // sleep for 100ms
    }
    endwin();
    return 0;
}