#include <stdio.h>  // for printf()
#include <limits.h> // for ULLONG_MAX
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
unsigned char getRow(unsigned long long grid, int index)
{
    return (grid >> index * N) & (1 << N) - 1; 
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
unsigned char getCol(unsigned long long grid, int index)
{
    unsigned char col = 0;
    grid >>= index;
    for (int i = 0; i < N; i++)
    {
        col = col << 1 | grid & 1ULL;
        grid >>= N;
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
bool isBalanced(unsigned char rowOrCol, unsigned char actions)
{
    int count_0 = 0;
    int count_1 = 0;
    for (int i = 0; i < N; i++)
    {
        if (!(actions & 1U)) { rowOrCol & 1U ? count_1++ : count_0++; }

        rowOrCol >>= 1;
        actions >>= 1;
    }
    return (count_0 <= N/2) && (count_1 <= N/2);
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
bool hasTriplets(unsigned char rowOrCol, unsigned char actions)
{
    for (int i = 0; i < N-2; i++)
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


/// @brief Checks if the grid is valid or not.
///
/// The grid is valid if it meets or _can_ meet the following requirements:
/// - All rows and columns are balanced, i.e. contain as many 1's as 0's.
/// - No row or column has three adjacent cells with the same value.
/// - All rows and columns are unique. 
///
/// The first two requirements are explained in their corresponding fuctions.
/// The uniqueness constraint is enforced by comparing each row and column
/// with all previously seen rows and columns. This is only done for rows
/// and columns that contain no empty cells.
///
/// @param grid The grid to be checked for validity.
/// @param actions The actions available (empty cells) in the grid.
///
/// @return True if the grid valid, false otherwise.
bool isValid(unsigned long long grid, unsigned long long actions)
{
    unsigned char rows[N];
    unsigned char cols[N];
    int rows_index = 0;
    int cols_index = 0; 

    for (int i = 0; i < N; i++)
    {   
        unsigned char row = getRow(grid, i);
        unsigned char col = getCol(grid, i);
        unsigned char row_actions = getRow(actions, i);
        unsigned char col_actions = getCol(actions, i);
        
        if (!isBalanced(row, row_actions) || hasTriplets(row, row_actions) ||
            !isBalanced(col, col_actions) || hasTriplets(col, col_actions))
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
void print(unsigned long long grid, unsigned long long actions)
{
    for (unsigned i = 0; i < N*N; i++)
    {   
        // Start a new line every N cells (but skip the first)
        if (i % N == 0 && i)
        {
            printf("\n");
            for (int j = 0; j < N-1; j++) { printf("---+"); }
            printf("---\n");
        }

        // If the cell is empty, print a space, else, print the cell's value
        printf(actions & 1U ? "   " : " %d ", grid & 1U);

        // Print a separator if it is not the last item in the row
        if (i % N != N-1) { printf("|"); }

        grid >>= 1;
        actions >>= 1;
    }
    printf("\n");
}


unsigned long long solve(unsigned long long state, unsigned long long actions)
{
    if (!actions && isValid(state, actions))
    {
        printf("Solved!\n");
        return state;
    }

    // for each possible action
    for (int i = 0; i < N*N; i++)
    {
        if ((actions >> i) & 1ULL)
        {   
            // try to make it a one and see if it is valid
            // make sure to make a copy and not change the original state
            unsigned long long state_copy = state;
            unsigned long long actions_copy = actions;
            state_copy |= (1ULL << i);
            actions_copy ^= (1ULL << i);
            
            // if it is valid, call the solve function on the new board
            if (isValid(state_copy, actions_copy))
            {
                solve(state_copy, actions_copy);
            }
            // if it is not solved or we exit from the solve function call above
            // make the move impossible so it stays a zero
            // continue to next cell
            actions ^= (1ULL << i);
        }
    }
}


/*---TEMP/UTILS FUNCTIONS-----------------------------------------------------*/

void load(unsigned long long* state, unsigned long long* actions)
{
    /*
    This is a temporary function and is going to be replaced by either a full
    binary puzzle generator or a simple binary string to integer function. 
    */
    
    enum cell { EMPTY, WHITE, BLACK };
    enum cell puzzle[N*N] = {
        EMPTY, EMPTY, EMPTY, EMPTY, WHITE, EMPTY,
        BLACK, EMPTY, EMPTY, BLACK, EMPTY, EMPTY,
        BLACK, EMPTY, BLACK, BLACK, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        WHITE, WHITE, EMPTY, EMPTY, WHITE, EMPTY,
    };

    // update state/actions
    for (int i = 0; i < N*N; i++)
    {
        // skip empty cells
        if (puzzle[i] == EMPTY) { continue; }
        
        // cell is already occupied so remove it from actions
        // white cells are set to 1 (black cells are already 0)
        *actions ^= 1ULL << i; 
        if (puzzle[i] == WHITE) { *state |= 1ULL << i; }
    }
}

void bitPrint(unsigned long long number, int n)
{
    // Prints the first n bits of 'number'
    for (int i = 0; i < n; i++)
    {
        printf("%d", number & 1);
        number >>= 1;
    }
    printf("\n");
}

unsigned long long str2ull(const char* bits)
{
    unsigned long long num = 0;
    int bitCount = 0;

    while (*bits)
    {
        // Invalid character found
        if (*bits != '0' && *bits != '1') { return ULLONG_MAX; }

        // Set bit equal to value in *bits and left shift
        num = num << 1 | (*bits++ & 1);

        // Check if the number of bits exceeds 64
        if (bitCount++ > 64) { return ULLONG_MAX; }
    }
    return num;
}

/*
bool isValid_backup(unsigned long long grid, unsigned long long actions)
{
    unsigned char rows[N];
    unsigned char cols[N];
    int rows_index = 0;
    int cols_index = 0; 

    for (int i = 0; i < N; i++)
    {   
        unsigned char row = getRow(grid, i);
        unsigned char row_actions = getRow(actions, i);
        unsigned char col = getCol(grid, i);
        unsigned char col_actions = getCol(actions, i);

        if (!isValidRowOrCol(row, row_actions))
        {
            return false;
        }
        // row is balanced and has no triplets, now also
        // check if row is unique, if so, store it
        for (int j = 0; j < rows_index; j++)
        {
            if (row == rows[j]) { printf("not unique\n"); return false; }
        }
        rows[rows_index++] = row;

        if (!isValidRowOrCol(col, col_actions))
        {
            return false;
        }
        // col is balanced and has no triplets, now also
        // check if row is unique, if so, store it
        for (int j = 0; j < cols_index; j++)
        {
            if (col == cols[j]) { printf("not unique\n"); return false; }
        }
        cols[cols_index++] = col;
    }
    return true;
}
*/

/*
This function only works for fully filled in rows and columns

/// @brief Checks if the number of 1's and 0's in a row or column are balanced.
///
/// Iterate over the row or column by right shifting once each step.
/// For each step, add the least significant bit's value to 'count'.
/// Only set bits can increase 'count' so 'count' represents the number of 1's. 
/// If N < 8, ensure that all bits beyond N are set to zero.
///
/// @param rowOrCol The row or column to be checked for balance.
///
/// @return True if the number of set bits makes up half of the size of 
///         the row or column (size is N), false otherwise.
bool isBalanced(unsigned char rowOrCol)
{
    int count;
    for (count = 0; rowOrCol; rowOrCol >>= 1)
    {
        count += rowOrCol & 1U;
    }
    return count == N / 2;
}
*/

/*
This function only works for fully filled in rows and columns
bool hasTriplets(unsigned char rowOrCol)
{
    for (int i = 0; i < N-2; i++)
    {
        if (((rowOrCol + 1) & 7U) <= 1){ return true; }
        rowOrCol >>= 1;
    }
    return false;
}
*/

