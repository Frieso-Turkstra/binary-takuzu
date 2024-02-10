#ifndef TAKUZU_H
#define TAKUZU_H

typedef enum { false, true } bool;
typedef struct
{
    unsigned long long grid;
    unsigned long long actions;
    unsigned size;
} Puzzle;

bool solve(Puzzle puzzle);
bool isValid(const Puzzle* puzzle);
Puzzle getRow(const Puzzle* puzzle, int index);
Puzzle getCol(const Puzzle* puzzle, int index);
bool isBalanced(const Puzzle* rowOrCol);
bool hasTriplets(const Puzzle* rowOrCol);
void printPuzzle(const Puzzle* puzzle);
bool isValidPuzzleString(const char* puzzleString);
Puzzle getPuzzle(const char* puzzleString);

#endif
