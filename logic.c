// logic.c
#include <stdio.h>
#include <stdbool.h>
#include "functions.c"

void removePos(GameState* gs, Pos p) {
    // TODO: Partner will implement set removal
    // For now: empty stub
}

void replacePos(GameState* gs, Pos p) {
    removePos(gs, p);           // clear from anyone
    // TODO: Partner adds p to current player's set
}

void expand(GameState* gs, Pos p) {
    // TODO: Partner adds p to current player's set (no removal)
}

void update(GameState* gs, Pos p) {
    replacePos(gs, p);          // Default: use replacement style (most common)
    // You can change this to expand() or custom logic later
}

// ======================
// Turn-taking
// ======================

bool nextPlayerMove(GameState* gs, Pos p) {
    if (!isValidPosition(gs, p)) {
        return false;
    }

    update(gs, p);              // Main logical move
    switchPlayer(gs);           // Alternate R <-> B

    return true;
}

// ======================
// Game end check
// ======================

const char* gameOver(const GameState* gs) {
    // TODO: Partner implements real win condition using set operations
    // Placeholder for now:
    return "";   // game continues until partner adds win logic
}
