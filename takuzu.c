#include <stdio.h>
#include <math.h>
#include "takuzu.h"


/**
 * @brief Recursively solves a Takuzu puzzle.
 * 
 * For each empty cell in the grid, try 0 (a cell is 0 by default so we only
 * update actions). If the new grid is valid, solve it. If not, we fill in a 1.
 * If that also fails, the puzzle has no solution. If no empty cells remain,
 * the puzzle is solved and printed.
 * 
 * @param puzzle The Takuzu puzzle to be solved.
 * 
 * @return true if a solution is found, false otherwise.
 */
bool solve(Puzzle puzzle)
{   
    for (int i = 0; i < puzzle.size*puzzle.size; i++)
    {
        if (!(puzzle.actions & 1ULL << i)) { continue; }

        puzzle.actions ^= 1ULL << i; 
        if (isValid(&puzzle) && solve(puzzle)) { return true; }

        puzzle.grid |= 1ULL << i;
        if (isValid(&puzzle) && solve(puzzle)){ return true; }

        return false;
    }
    printPuzzle(&puzzle);
    return true;
}

/**
 * @brief Checks if the puzzle is valid or not.
 *
 * The puzzle is valid if it meets or can meet the following requirements:
 * - All rows and columns are balanced, i.e. contain as many 1's as 0's.
 * - No row or column has three adjacent cells with the same value.
 * - All rows and columns are unique. 
 *
 * The first two requirements are explained in their corresponding fuctions.
 * The uniqueness constraint is enforced by comparing each row and column
 * with all previously seen rows and columns. This is only done for rows
 * and columns that contain no empty cells.
 *
 * @param puzzle The puzzle to be checked for validity.
 *
 * @return true if the puzzle is valid, false otherwise.
 */
bool isValid(const Puzzle* puzzle)
{
    unsigned char rows[puzzle->size];
    unsigned char cols[puzzle->size];
    int rows_index = 0;
    int cols_index = 0; 

    for (int i = 0; i < puzzle->size; i++)
    {   
        Puzzle row = getRow(puzzle, i);
        Puzzle col = getCol(puzzle, i);
        
        if (!isBalanced(&row) || hasTriplets(&row)) { return false; }
        if (!isBalanced(&col) || hasTriplets(&col)) { return false; }

        if (!row.actions)
        {
            for (int j = 0; j < rows_index; j++)
            {
                if (row.grid == rows[j]) { return false; }
            }
            rows[rows_index++] = row.grid;
        }

        if (!col.actions)
        {
            for (int j = 0; j < cols_index; j++)
            {
                if (col.grid == cols[j]) { return false; }
            }
            cols[cols_index++] = col.grid;
        }
    }
    return true;
}

/**
 * @brief Extracts the row at the specified index from the puzzle.
 *
 * The desired row is obtained by right shifting the relevant bits into the 
 * N least significant bits where N is size. The rest of the bits are discarded
 * with a bitmask.
 *
 * @param puzzle The puzzle from which to extract the row.
 * @param index The index of the row to retrieve (0 is bottom, N-1 is top).
 *
 * @return A Puzzle struct containing the extracted row, actions and size.
 */
Puzzle getRow(const Puzzle* puzzle, int index)
{
    return (Puzzle) {
        .grid = (puzzle->grid >> index * puzzle->size) & (1ULL << puzzle->size) - 1,
        .actions = (puzzle->actions >> index * puzzle->size) & (1ULL << puzzle->size) - 1,
        .size = puzzle->size
    };
}

/**
 * @brief Extracts the column at the specified index from the puzzle.
 *
 * The column is extracted for both the puzzle's grid and its actions.
 * The grid is first aligned with the least significant bit by adding index.
 * Then, we iterate through the column by right shifting an additional i*size.
 * The current value of col is left shifted by 1 to make room for the next bit.
 * Lastly, bitwise OR that next bit with the least significant bit of the grid.
 *
 * @param puzzle The puzzle from which to extract the column.
 * @param index The index of the column to retrieve (0 is right, N-1 is left).
 * 
 * @return A Puzzle struct containing the extracted column, actions and size.
*/ 
Puzzle getCol(const Puzzle* puzzle, int index)
{
    Puzzle col = { .grid = 0, .actions = 0, .size = puzzle->size };
    
    for (int i = 0; i < puzzle->size; i++)
    {
        col.grid = col.grid << 1 | puzzle->grid >> (index + i * puzzle->size) & 1ULL;
        col.actions = col.actions << 1 | puzzle->actions >> (index + i * puzzle->size) & 1ULL;
    }
    return col;
}

/**
 * @brief Checks if the row or column is or can become balanced.
 *
 * For each non-empty cell, increase the count depending on the cell's value.
 *
 * @param rowOrCol The row or column to be checked for balance.
 *
 * @return true if there are no more than size/2 1's or 0's , false otherwise.
 */
bool isBalanced(const Puzzle* rowOrCol)
{
    int count_0 = 0;
    int count_1 = 0;
    for (int i = 0; i < rowOrCol->size; i++)
    {
        if (!(rowOrCol->actions & 1ULL << i))
        {
            rowOrCol->grid & 1ULL << i ? count_1++ : count_0++;
        }
    }
    return (count_0 <= rowOrCol->size/2) && (count_1 <= rowOrCol->size/2);
}

/**
 * @brief Checks for three adjacent bits with the same value in a row or column.
 *
 * The bitmask 7U (or 00000111) extracts the three least significant bits.
 * Check if all three cells are non-empty using 'actions', if they are,
 * add one to the row or column because 111 + 1 = 000 and 000 + 1 = 001
 * so the result is <= 1 if the three least significant bits are equal.
 *
 * @param rowOrCol The row or column to be checked for triplets.
 *
 * @return true if triplets are found, false otherwise.
 */
bool hasTriplets(const Puzzle* rowOrCol)
{
    for (int i = 0; i < rowOrCol->size-2; i++)
    {
        if (!(rowOrCol->actions & 7ULL << i))
        {
            if ((rowOrCol->grid + 1 & 7ULL << i) <= 1) { return true; }
        }
    }
    return false;
}

/**
 * @brief Prints out a nicely formatted version of the puzzle's grid.
 * 
 * Starts a new line every N cells where N is puzzle->size (but skip the first).
 * If the cell is empty, prints a space, else prints the cell's value.
 * Prints a separator if it is not the last item in the row.
 * 
 * @param puzzle The puzzle to be printed.
 */
void printPuzzle(const Puzzle* puzzle)
{
    for (int i = 0; i < puzzle->size*puzzle->size; i++)
    {   
        if (i % puzzle->size == 0 && i)
        {
            printf("\n");
            for (int j = 0; j < puzzle->size-1; j++) { printf("---+"); }
            printf("---\n");
        }

        printf(puzzle->actions >> i & 1ULL ? "   " : " %d ", puzzle->grid >> i & 1ULL);

        if (i % puzzle->size != puzzle->size-1) { printf("|"); }
    }
    printf("\n");
}

/**
 * @brief Validates the string representation of a Takuzu puzzle.
 * 
 * A valid Takuzu puzzle consists of only '0's and '1's and has a length of 16,
 * 36, or 64. Anything less than 16 renders the puzzle trivial, anything longer
 * than 64 and the puzzle no longer fits in an unsigned long long integer.
 * 
 * @param puzzleString The string representation of the puzzle to be validated.
 * 
 * @return true if the puzzle string is valid, false otherwise.
 */
bool validatePuzzleString(const char* puzzleString)
{   
    int length = 0;

    while (*puzzleString)
    {
        if (*puzzleString != '1' && *puzzleString != '0' && *puzzleString != ' ')
        {
            printf("Error: Invalid character found: %c.\n", *puzzleString);
            printf("A valid Takuzu puzzle consists of only '0's and '1's.\n");
            return false;
        }
        puzzleString++;
        length++;
    }

    if (length != 16 && length != 36 && length != 64)
    {
        printf("Error: Invalid puzzle length: %d.\n", length);
        printf("Puzzle length must be 16, 36 or 64.\n");
        return false;
    }

    return true;
}

/**
 * @brief Parses the string representation of a Takuzu puzzle into a struct.
 * 
 * The grid is a binary representation of the puzzle using an unsigned long long
 * integer in which each bit represents a cell in the grid (unsigned long long 
 * is 64 bits so the max grid size is 8x8). Cells that are empty or are filled
 * in with a 0 cannot be distinguished from each other, so 'actions' keeps track
 * of which bits are empty. Initially, all cells are empty so all bits in 'grid'
 * are 0 and all bits in 'actions' are 1. Size represents the size of the grid.
 * 
 * @param puzzleString The string representation of the puzzle to be parsed.
 * 
 * @return A Puzzle struct representing the parsed puzzle.
 */
Puzzle getPuzzle(const char* puzzleString)
{
    Puzzle puzzle = { .grid = 0, .actions = -1, .size = 0 };

    unsigned length = 0;
    while (*puzzleString)
    {   
        if (*puzzleString != ' ') { puzzle.actions ^= 1ULL << length; }
        if (*puzzleString == '1') { puzzle.grid |= 1ULL << length; }

        length++;
        puzzleString++;
    }

    puzzle.size = sqrt(length);
    return puzzle;
}
