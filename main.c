#include <stdlib.h>
#include <stdio.h>
#include <time.h>
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

    struct timeval tv;
    gettimeofday(&tv,NULL);
    unsigned long start = 1000000 * tv.tv_sec + tv.tv_usec;
    bool solved = solve(puzzle);
    gettimeofday(&tv,NULL);
    unsigned long end = 1000000 * tv.tv_sec + tv.tv_usec;

    unsigned long duration = end-start;
    printf("seconds: %d milli: %d micro: %d", duration / 1000000, duration / 1000, duration % 1000);

    if (!solved)
    {
        printf("No solution found...\n");
    }
    return EXIT_SUCCESS;
}
