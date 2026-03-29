// main.c
#include <stdio.h>
#include <stdbool.h>
#include "gamestate.h"

int main(void) {
    GameState gs;
    initGameState(&gs);

    printf("=== Logical Tic-Tac-Toe (Set Theory Edition) ===\n");
    printf("Red (R) goes first.\n");
    printf("Enter moves as: row column  (example: 2 2 for center)\n");
    printf("Board is 1-indexed (rows/columns 1 to 3)\n\n");

    while (true) {
        // Display current state
        printBoard(&gs);
        printf("\nCurrent sets:\n");
        printSets(&gs);

        // Check if game has ended
        const char* result = gameOver(&gs);
        if (result[0] != '\0') {
            printf("\n========================================\n");
            printf("=== GAME OVER: %s ===\n", result);
            printf("========================================\n");
            break;
        }

        // Prompt current player
        char player = currentPlayer(&gs);
        printf("\n%s's turn → Enter row column: ", (player == 'R' ? "Red" : "Blue"));

        int row, col;
        if (scanf("%d %d", &row, &col) != 2) {
            printf("Invalid input! Please enter two numbers (row column).\n");
            while (getchar() != '\n');   // clear input buffer
            continue;
        }

        // Convert 1-based user input to 0-based Pos
        Pos p = {row - 1, col - 1};

        if (nextPlayerMove(&gs, p)) {
            printf("→ Move accepted at (%d, %d)\n", row, col);
        } else {
            printf("→ Illegal move! Position out of bounds or invalid.\n");
        }
    }

    // Final board display
    printf("\nFinal board:\n");
    printBoard(&gs);
    printf("\nThanks for playing!\n");

    return 0;
}
