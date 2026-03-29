// logic.c
#include <stdio.h>
#include <stdbool.h>
#include "gamestate.h"

// ======================
// Function prototypes
// ======================

void removePos(GameState* gs, Pos p);
void replacePos(GameState* gs, Pos p);
void expand(GameState* gs, Pos p);
void update(GameState* gs, Pos p);

bool nextPlayerMove(GameState* gs, Pos p);
const char* gameOver(const GameState* gs);

// ======================
// Behavioral functions (using Partner A's utilities)
// ======================

void removePos(GameState* gs, Pos p) {
    // Safely remove from both players' sets if present
    if (contains(gs, 'R', p)) {
        removeFromSet(gs, 'R', p);
    }
    if (contains(gs, 'B', p)) {
        removeFromSet(gs, 'B', p);
    }
}

void replacePos(GameState* gs, Pos p) {
    removePos(gs, p);                          // clear the position first
    addToSet(gs, currentPlayer(gs), p);        // claim for current player
    updateFacts(gs);                           // Partner A recomputes F, over, etc.
}

void expand(GameState* gs, Pos p) {
    char player = currentPlayer(gs);
    
    // Core position
    if (isValidPosition(gs, p) && 
        !contains(gs, 'R', p) && !contains(gs, 'B', p)) {
        addToSet(gs, player, p);
    }
    
    // Orthogonal neighbors: up, down, left, right
    Pos neighbors[4] = {
        {p.x - 1, p.y},   // up
        {p.x + 1, p.y},   // down
        {p.x,     p.y - 1}, // left
        {p.x,     p.y + 1}  // right
    };
    
    for (int i = 0; i < 4; i++) {
        Pos n = neighbors[i];
        if (isValidPosition(gs, n) &&
            !contains(gs, 'R', n) && !contains(gs, 'B', n)) {  // only empty cells
            addToSet(gs, player, n);
        }
    }
    
    updateFacts(gs);   // Partner A updates derived facts
}

void update(GameState* gs, Pos p) {
    // Main logical operation — choose the rule you want to use
    // Currently set to replacement (common in set-logic games)
    // Change to expand(gs, p); when you want growth-style play
    replacePos(gs, p);
    // expand(gs, p);   // ← uncomment this line to switch to expansion mode
}

// ======================
// Turn-taking
// ======================

bool nextPlayerMove(GameState* gs, Pos p) {
    if (!isValidPosition(gs, p)) {
        return false;
    }

    //Can add extra rules here

    update(gs, p);        // Apply the chosen logical rule
    switchPlayer(gs);     // Alternate Red <-> Blue

    return true;
}

// ======================
// Game termination (finalized)
// ======================

const char* gameOver(const GameState* gs) {
    // Partner A is responsible for maintaining an "over" flag and winner
    // in updateFacts(). We just read the result here.

    // Placeholder until Partner A finishes updateFacts():
    // return "";   // game continues

    // Once Partner A implements it, you can do something like:
    // if (gs->over) {
    //     if (gs->winner == 'R') return "R wins";
    //     if (gs->winner == 'B') return "B wins";
    //     return "Draw";
    // }
    return "";   // ← change this when Partner A provides the real check
}
