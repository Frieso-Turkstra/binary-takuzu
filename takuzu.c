#include <stdio.h>  // for printf()
#include <math.h>
#include "takuzu.h"


/// @brief Extracts the row at the specified index from 'grid'.
///
/// The desired row is obtained by right shifting the relevant bits into the 
/// N least significant bits. The rest of the bits are discarded with a bitmask.
/// Since N <= 8, converting unsigned long long to unsigned char is safe.
///
/// @param grid The unsigned long long integer representing the grid.
/// @param index The index of the row to retrieve (0 is bottom, N-1 is top row).
///
/// @return The extracted row as an unsigned char. 
unsigned char getRow(unsigned long long grid, int index, unsigned size)
{
    return (grid >> index * size) & (1 << size) - 1; 
}

/// @brief Extracts the column at the specified index from 'grid'.
///
/// The desired column is obtained by first aligning the relevant column
/// with the least significant bit, 'grid >>= index'. Then, we can iterate
/// through each row in the column by right shifting the grid N positions.
/// The current value of col is left shifted by 1 to make room for the next bit.
/// Lastly, bitwise OR that next bit with the least significant bit of 'grid'.
/// Since N <= 8, converting unsigned long long to unsigned char is safe.
///
/// @param grid The unsigned long long integer representing the grid.
/// @param index The index of the column to retrieve (0 is right, N-1 is left column).
///
/// @return The extracted column as an unsigned char. 
unsigned char getCol(unsigned long long grid, int index, unsigned size)
{
    unsigned char col = 0;
    grid >>= index;
    for (int i = 0; i < size; i++)
    {
        col = col << 1 | grid & 1ULL;
        grid >>= size;
    }
    return col;
}

/// @brief Checks if the row or column is or can become balanced.
///
/// Iterate over the row or column and actions by right shifting once each step.
/// For each step, check if the cell is non-empty using 'actions'.
/// If the cell is non-empty, increase the count depending on the cell's value.
///
/// @param rowOrCol The row or column to be checked for balance.
/// @param actions The actions available (empty cells) in the row or column.
///
/// @return True if there are no more than N/2 1's or 0's, false otherwise.
bool isBalanced(unsigned char rowOrCol, unsigned char actions, unsigned size)
{
    int count_0 = 0;
    int count_1 = 0;
    for (int i = 0; i < size; i++)
    {
        if (!(actions & 1U)) { rowOrCol & 1U ? count_1++ : count_0++; }

        rowOrCol >>= 1;
        actions >>= 1;
    }
    return (count_0 <= size/2) && (count_1 <= size/2);
}

/// @brief Checks for three adjacent bits with the same value in a row or column.
///
/// The bitmask 7U (or 00000111) extracts the three least significant bits.
/// Check if all three cells are non-empty using 'actions', if they are,
/// add one to the row or column because 111 + 1 = 000 and 000 + 1 = 001
/// so the result is <= 1 if the three least significant bits are equal.
///
/// @param rowOrCol The row or column to be checked for triplets.
/// @param actions The actions available (empty cells) in the row or column.
///
/// @return True if triplets are found, false otherwise.
bool hasTriplets(unsigned char rowOrCol, unsigned char actions, unsigned size)
{
    for (int i = 0; i < size-2; i++)
    {
        if (!(actions & 7U))
        {
            if ((rowOrCol + 1 & 7U) <= 1) { return true; }
        }
        rowOrCol >>= 1;
        actions >>= 1;
    }
    return false;
}

/// @brief Checks if the puzzle is valid or not.
///
/// The puzzle is valid if it meets or can meet the following requirements:
/// - All rows and columns are balanced, i.e. contain as many 1's as 0's.
/// - No row or column has three adjacent cells with the same value.
/// - All rows and columns are unique. 
///
/// The first two requirements are explained in their corresponding fuctions.
/// The uniqueness constraint is enforced by comparing each row and column
/// with all previously seen rows and columns. This is only done for rows
/// and columns that contain no empty cells.
///
/// @param puzzle The puzzle to be checked for validity.
///
/// @return True if the puzzle valid, false otherwise.
bool isValid(const Puzzle* puzzle)
{
    unsigned char rows[puzzle->size];
    unsigned char cols[puzzle->size];
    int rows_index = 0;
    int cols_index = 0; 

    for (int i = 0; i < puzzle->size; i++)
    {   
        unsigned char row = getRow(puzzle->grid, i, puzzle->size);
        unsigned char col = getCol(puzzle->grid, i, puzzle->size);
        unsigned char row_actions = getRow(puzzle->actions, i, puzzle->size);
        unsigned char col_actions = getCol(puzzle->actions, i, puzzle->size);
        
        if (!isBalanced(row, row_actions, puzzle->size) || hasTriplets(row, row_actions, puzzle->size) ||
            !isBalanced(col, col_actions, puzzle->size) || hasTriplets(col, col_actions, puzzle->size))
        {
            return false;
        }

        if (!row_actions)
        {
            for (int j = 0; j < rows_index; j++)
            {
                if (row == rows[j]) { return false; }
            }
            rows[rows_index++] = row;
        }

        if (!col_actions)
        {
            for (int j = 0; j < cols_index; j++)
            {
                if (col == cols[j]) { return false; }
            }
            cols[cols_index++] = col;
        }
    }
    return true;
}

/// @brief Prints out a nicely formatted version of the grid.
///
/// @param grid The grid to be printed.
/// @param actions The actions available (empty cells) in the grid.
void printPuzzle(const Puzzle* puzzle)
{
    unsigned N = puzzle->size;
    for (int i = 0; i < N*N; i++)
    {   
        // Start a new line every N cells (but skip the first)
        if (i % N == 0 && i)
        {
            printf("\n");
            for (int j = 0; j < N-1; j++) { printf("---+"); }
            printf("---\n");
        }

        // If the cell is empty, print a space, else, print the cell's value
        printf(puzzle->actions >> i & 1ULL ? "   " : " %d ", puzzle->grid >> i & 1ULL);

        // Print a separator if it is not the last item in the row
        if (i % N != N-1) { printf("|"); }
    }
    printf("\n");
}


bool solve(Puzzle puzzle)
{   
    for (int i = 0; i < puzzle.size*puzzle.size; i++)
    {
        if (!(puzzle.actions & 1ULL << i)) { continue; }

        // try 0
        puzzle.actions ^= 1ULL << i; 
        if (isValid(&puzzle) && solve(puzzle)) { return true; }

        // try 1
        puzzle.grid |= 1ULL << i;
        if (isValid(&puzzle) && solve(puzzle)){ return true; }

        // no solution
        return false;
    }
    // If no empty cells remain, the puzzle is solved
    //printf("Solved!\n");
    //printPuzzle(&puzzle);
    return true;
}

/**
 * @brief Validates the string representation of a Takuzu puzzle.
 * 
 * A valid Takuzu puzzle consists of only '0's and '1's and has a length of 16,
 * 36, or 64. Anything less than 16 renders the puzzle trivial, anything longer
 * than 64 and the puzzle no longer fits in an unsigned long long integer.
 * 
 * @param puzzleString The string representation of the Takuzu puzzle to be validated.
 * 
 * @return true if the puzzle string is valid, false otherwise.
 */
bool isValidPuzzleString(const char* puzzleString)
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
 * @brief Parses the string representation of a Takuzu puzzle into a struct representation.
 * 
 * The grid is a binary representation of the puzzle using an unsigned long long
 * integer in which each bit represents a cell in the grid (unsigned long long 
 * is 64 bits so the max grid size is 8x8). Cells that are empty or are filled
 * in with a 0 cannot be distinguished from each other, so 'actions' keeps track
 * of which bits are empty. Initially, all cells are empty so all bits in 'grid'
 * are 0 and all bits in 'actions' are 1. Size represents the size of the grid.
 * 
 * @param puzzleString The string representation of the Takuzu puzzle to be parsed.
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
