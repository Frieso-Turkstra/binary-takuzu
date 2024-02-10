#ifndef TAKUZU_H
#define TAKUZU_H

typedef enum { false, true } bool;
typedef struct
{
    unsigned long long grid;
    unsigned long long actions;
    unsigned size;
} Puzzle;

bool isValidPuzzleString(const char* puzzleString);
Puzzle getPuzzle(const char* puzzleString);
bool solve(Puzzle puzzle);
bool isValid(const Puzzle* puzzle);
unsigned char getRow(unsigned long long grid, int index, unsigned size);
unsigned char getCol(unsigned long long grid, int index, unsigned size);
bool isBalanced(unsigned char rowOrCol, unsigned char actions, unsigned size);
bool hasTriplets(unsigned char rowOrCol, unsigned char actions, unsigned size);
void printPuzzle(const Puzzle* puzzle);

#endif
