#include <stdio.h>
#include <stdbool.h>
// #include "gamestate.h"   

// Assume these are declared in gamestate.h
// typedef struct { int x, y; } Pos;
// typedef struct GameState GameState;

void printBoard(const GameState* gs);           // partner's job 
const char* playerName(char player);            

int main(void) {
    GameState gs;
    initGameState(&gs);           // partner's init function

    bool running = true;

    while (running) {
        printBoard(&gs);
        printf("\nSets:\n");
        printSets(&gs);           // partner's debug print

        const char* status = gameOver(&gs);
        if (status[0] != '\0') {
            printf("\nGAME OVER: %s\n", status);
            running = false;
            continue;
        }

        printf("\n%s to move (enter row col): ", playerName(gs.currentPlayer));

        int row, col;
        if (scanf("%d %d", &row, &col) != 2) {
            printf("Invalid input.\n");
            while (getchar() != '\n'); // clear input buffer
            continue;
        }

        // Normalize to 0-based if you prefer
        Pos p = {row - 1, col - 1};   

        if (!isValidPosition(&gs, p)) {
            printf("Position out of bounds.\n");
            continue;
        }

        if (nextPlayerMove(&gs, p)) {
            // move was accepted — board changed
            printf("Move accepted.\n");
        } else {
            printf("Illegal move.\n");
        }
    }

    printf("\nThanks for playing.\n");
    return 0;
}

