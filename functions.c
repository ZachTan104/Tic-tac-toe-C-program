#include <stdio.h>

#define CSIZE 3		// Z+ && x < 4, 1-3
#define MSIZE 9	// C*C, 9 combinations

//Varuables and Sets
struct 
Pair
{
	int val1;
	int val2;
};

struct 
GameState 
{
	/* Applicable Sets */
	
	int C[CSIZE];
	//n has no upper bound, represented as a condition (>= 0)
	struct Pair M[MSIZE];
	
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
	struct Pair R[MSIZE];
	struct Pair B[MSIZE];
	struct Pair S[MSIZE];
	struct Pair T[MSIZE];
	struct Pair F[MSIZE];
	
	int rSize;
	int bSize;
	int sSize;
	int tSize;
	int fSize;
};

//Set utilities

//Checks if two pairs are equal
int
equalPos (struct Pair pair1, struct Pair pair2)
{
	int isEqual;
	
	if(pair1.val1 == pair2.val1 && pair1.val2 == pair2.val2)
		isEqual = 1;
	else
		isEqual = 0;
		
	return isEqual;
}

//checks if set contains int
int
containsInt (int val, int set[], int size) //1 means pair1 contains pair2, 0 otherwise
{
	int i, 
		contains = 0;
	
	for (i = 0; i < size; i++)
	{
		if(set[i] == val)
			contains = 1;
	}
	
	return contains;
}


int
containsPair (struct Pair set[], struct Pair val, int size) // 1 means pair1 contains pair2, 0 otherwise
{
	int i, 
		contains = 0;
	
	for (i = 0; i < size; i++)
	{
		if (equalPos(set[i], val) == 1)
			contains = 1;
	} 
	
	return contains;
}

void
addPair (struct Pair set[], struct Pair val, int *size)
{
	int contains = containsPair(set, val, *size);
	
	if(contains == 0 && *size < MSIZE) // errors if the pair is a member of the set
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
removePair (struct Pair set[], struct Pair val, int *size)
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



// initialize variables
void
initGameState(struct GameState *game)
{
	int i, j, k = 0;

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
			game->M[k].val1 = game->C[i];
			game->M[k].val2 = game->C[j];
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

void
updateFacts(struct GameState * game)
{
	int i;

	// F = M - (R U B)
	
	game->fSize = 0; // Reset fSize
	
	for(i = 0; i < MSIZE; i++)
	{
		//Does not add to F if value is found in R or B
		if (containsPair(game->R, game->M[i], game->rSize) == 0 && 
			containsPair(game->B, game->M[i], game->bSize) == 0)
		{
			game->F[game->fSize] = game->M[i];
			game->fSize++;
		}
	}
	
	/* over <-> (|F| = 3 V val >= 20 V ~start ^ 
				(|R| > 0 ^ |B| = 0 V |R| = 0 ^ |B| > 0)) */
	if (game->fSize == 3 || game->val >= 20 || (!game->start &&
	   ((game->rSize > 0 && game->bSize == 0) || (game->rSize == 0 && game->bSize > 0))))
	{
		game->over = 1;
	}
}

void
printSet(struct Pair set[], int size)
{
    int i;

    printf("{ ");

    for (i = 0; i < size; i++)
    {
        printf("(%d,%d)", set[i].val1, set[i].val2);

        if (i < size - 1)
            printf(", ");
    }

    printf(" }\n");
}
