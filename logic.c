// logic.c
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "functions.c"

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
    if (!isValidPosition(gs, p))
        return false;
	
	if (contains(gs, 'R', p) || contains(gs, 'B', p))
    	return false;
    
    //Can add extra rules here

    update(gs, p);        // Apply the chosen logical rule
    switchPlayer(gs);     // Alternate Red <-> Blue

    return true;
}

// ======================
// Game termination (finalized)
// ======================
int
winLogic(const GameState* gs, char player)
{
    Pos* set;
    int size,
    	i,
    	win = 0;

    if (player == 'R') {
        set = (Pos*) gs->R;
        size = gs->rSize;
    } else {
        set = (Pos*) gs->B;
        size = gs->bSize;
    }

	//every possible winning combination
    Pos
	wins[8][3] = 
	{
		//x
        {{0,0},{0,1},{0,2}},
	    {{1,0},{1,1},{1,2}},
	    {{2,0},{2,1},{2,2}},
	
		//y
	    {{0,0},{1,0},{2,0}},
	    {{0,1},{1,1},{2,1}},
	    {{0,2},{1,2},{2,2}},
	
		//diagonals
	    {{0,0},{1,1},{2,2}},
	    {{0,2},{1,1},{2,0}}
    };

    for (i = 0; i < 8; i++) {
        if (containsSet(set, wins[i][0], size) &&
            containsSet(set, wins[i][1], size) &&
            containsSet(set, wins[i][2], size))
        {
            win = 1;
        }
    }

    return win;
}

const
char* gameOver(const GameState* gs)
{
    if (winLogic(gs, 'R'))
        return "Red wins!";

    if (winLogic(gs, 'B'))
        return "Blue wins!";

    if (gs->fSize == 0)
        return "It's a Draw!'";

    return "";
}
