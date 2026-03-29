#include <stdio.h>

#define CSIZE 3		// Z+ && x < 4, 1-3
#define MSIZE 9	// C*C, 9 combinations

//Variables and Sets
typedef struct 
{
	int x;
	int y;
} Pos;

typedef struct  
{
	/* Applicable Sets */
	
	int C[CSIZE];
	//n has no upper bound, represented as a condition (>= 0)
	Pos M[MSIZE];
	
	/* System Variables */
	
	// V = {true, flase}, represented with 1 or 0
	int good;
	int go;
	int start;
	int over;
	int found;
	
	// N
	int val;
	
	// subsets of M
	Pos R[MSIZE];
	Pos B[MSIZE];
	Pos S[MSIZE];
	Pos T[MSIZE];
	Pos F[MSIZE];
	
	int rSize;
	int bSize;
	int sSize;
	int tSize;
	int fSize;

} GameState;

//Set utilities


// =========================================
// FUNCTIONS USED EXCLUSIVELY IN FUNCTIONS.C
// =========================================

//Checks if two pairs are equal
int
equalPos (Pos a, Pos b)
{
	int isEqual;
	
	if(a.x == b.x && a.y == b.y)
		isEqual = 1;
	else
		isEqual = 0;
		
	return isEqual;
}

//checks if set contains int
int
containsSet (Pos set[], Pos val, int size) // 1 means pair1 contains pair2, 0 otherwise
{
	int i, 
		contains = 0;
	
	for (i = 0; i < size; i++)
	{
		if (equalPos(set[i], val))
			contains = 1;
	} 
	
	return contains;
}

void
add (Pos set[], Pos val, int *size)
{
	int setContains = containsSet(set, val, *size);
	
	if(setContains == 0 && *size < MSIZE) // errors if the pair is a member of the set
	{
		(*size)++;
		set[*size - 1] = val;
	}
	else
	{
		printf("Error: Set already contains pair\n");
	}
}


void
removeSet (Pos set[], Pos val, int *size)
{
	int i, 
		posI = -1;
	
	for (i = 0; i < *size; i++)
    {
        if (equalPos(set[i], val) == 1 && posI == -1)
        {
            posI = i;  // store first occurrence only
        }
    }

    // Remove if found
    if (posI != -1)
    {
        for (i = posI; i < *size - 1; i++)
        {
            set[i] = set[i + 1];
        }

        (*size)--;
    }
    else
    {
        printf("Error: Set does not contain pair\n");
    }
}
// =================================
// FUNCTIONS TO BE USED IN LOGIC.C
// =================================

// wrapper function for containsSet
int
contains (GameState* gs, char set, Pos p)
{
    if (set == 'R')
        return containsSet(gs->R, p, gs->rSize);
    else if (set == 'B')
        return containsSet(gs->B, p, gs->bSize);

    return 0;
}

// wrapper function for add
void
addToSet (GameState* gs, char set, Pos p)
{
    if (set == 'R')
        add(gs->R, p, &gs->rSize);
    else if (set == 'B')
        add(gs->B, p, &gs->bSize);
}

void
removeFromSet (GameState* gs, char set, Pos p)
{
    if (set == 'R')
        removeSet(gs->R, p, &gs->rSize);
    else if (set == 'B')
        removeSet(gs->B, p, &gs->bSize);
}

// switches player
void
switchPlayer (GameState* gs)
{
    gs->go = !gs->go;
}

// returns the current player
char 
currentPlayer (GameState* gs)
{
    if (gs->go)
        return 'R';
    else
        return 'B';
}

// Check if position is valid position inside game grid
int isValidPosition (GameState* gs, Pos p)
{
    return (p.x >= 0 && p.x < 3 && p.y >= 0 && p.y < 3);
}

// Update based on changes in variable information
void
updateFacts (GameState * game)
{
	int i;
	
	// F = M - (R U B)
	game->fSize = 0; // Reset fSize
	
	for(i = 0; i < MSIZE; i++)
	{
		//Does not add to F if value is found in R or B
		if (containsSet(game->R, game->M[i], game->rSize) == 0 && 
			containsSet(game->B, game->M[i], game->bSize) == 0)
		{
			game->F[game->fSize] = game->M[i];
			game->fSize++;
		}
	}
	
	/* over <-> (|F| = 3 V val >= 20 V ~start ^ 
				(|R| > 0 ^ |B| = 0 V |R| = 0 ^ |B| > 0)) */
	if (game->fSize == 3 || game->val >= 20 || (!game->start &&
	   ((game->rSize > 0 && game->bSize == 0) || (game->rSize == 0 && game->bSize > 0)))
	   || game->fSize == 0) //not in original expression but required for the game logic to run properly
	{
		game->over = 1;
	}
}

// ===========================
// INITIALIZATION OF VARIABLES
// ===========================
void
initGameState (GameState *game)
{
	int i, 
		j, 
		k = 0;

	// Initialize C = {1, 2, 3}
	for(i = 0; i < CSIZE; i++)
		game->C[i] = i + 1;


	/* Initialize M = C * C
	
	M[1] = (1, 1)
	M[2] = (1, 2)
	M[3] = (1, 3)
	M[4] = (2, 1)
	...
	M[9] = (3, 3)
	
	*/
	for(i = 0; i < CSIZE; i++)
	{
		for(j = 0; j < CSIZE; j++)
		{
			game->M[k].x = i;
			game->M[k].y = j;
			k++;
		}
	}

	// Initialize true / false
	game->good = 0;
	game->go = 1;
	game->start = 1;
	game->over = 0;
	game->found = 0;
	
	//Initialize val N
	game->val = 0;
	
	//initialize array sizes
	game->rSize = 0;
	game->bSize = 0;
	game->sSize = 0;
	game->tSize = 0;
	game->fSize = 0;
}

// =================
// DISPLAY FUNCTIONS
// =================
void 
printBoard (GameState* gs)
{
    char board[3][3];
    int i, 
		j;

    for(i = 0; i < 3; i++)
        for(j = 0; j < 3; j++)
            board[i][j] = '.';

    for(i = 0; i < gs->rSize; i++)
        board[gs->R[i].x][gs->R[i].y] = 'R';

    for(i = 0; i < gs->bSize; i++)
        board[gs->B[i].x][gs->B[i].y] = 'B';

    for(i = 0; i < 3; i++)
    {
        for(j = 0; j < 3; j++)
            printf("%c ", board[i][j]);
        printf("\n");
    }
}

void
printSets (GameState* gs)
{
    int i;

    printf("R: ");
    for(i = 0; i < gs->rSize; i++)
        printf("(%d,%d) ", gs->R[i].x + 1, gs->R[i].y + 1);

    printf("\nB: ");
    for(i = 0; i < gs->bSize; i++)
        printf("(%d,%d) ", gs->B[i].x + 1, gs->B[i].y + 1);

    printf("\n");
}
