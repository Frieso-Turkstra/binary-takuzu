#include <stdlib.h>
#include <stdio.h>
#include "takuzu.h"


int main(int argc, char** argv)
{
    if (argc != 2)
    {
        printf("Error: Invalid number of arguments.\n");
        printf("Usage: %s [puzzleString]\n", argv[0]);
        printf("Example: %s '0  1      000  0'\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (!isValidPuzzleString(argv[1]))
    {
        return EXIT_FAILURE;
    }

    Puzzle puzzle = getPuzzle(argv[1]);
    
    if (!isValid(&puzzle))
    {
        printf("Error: Invalid puzzle provided.\n");
        return EXIT_FAILURE;
    }

    if (solve(puzzle))
    {
        printf("Solved!\n");
    }
    else
    {
        printf("No solution found...\n");
    }

    return EXIT_SUCCESS;
}
