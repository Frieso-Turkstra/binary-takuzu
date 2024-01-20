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
    for (grid >>= index; grid; grid >>= N)
    {
        col = col << 1 | grid & 1ULL;
    }
    return col;
}


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


/// @brief Checks for three adjacent bits with the same value in a row or column.
///
/// The bitmask 7U (or 00000111) extracts the three least significant bits.
/// Add one to the row or column so that extracting the least three significant
/// bits yields a 0 or a 1 if all bits are equal and >1 if they are different.
/// 111 + 1 = 000; 000 + 1 = 001 so result is <= 1 if all bits are equal.
/// Lastly, right shift the row or column by 1 to check the next three bits.
///
/// @param rowOrCol The row or column to be checked for triplets.
///
/// @return True if triplets are found, false otherwise.
bool hasTriplets(unsigned char rowOrCol)
{
    for (int i = 0; i < N-2; i++)
    {
        if (((rowOrCol + 1) & 7U) <= 1){ return true; }
        rowOrCol >>= 1;
    }
    return false;
}


/// @brief 
/// @param grid 
/// @return 
bool isSolved(unsigned long long grid)
{   
    // Check if all rows and cols are unique, balanced and have no triplets.
    unsigned long long rows[N];
    unsigned long long cols[N];
    int index = 0;
    	
    for (int i = 0; i < N; i++)
    {   
        // get row and col
        unsigned char row = getRow(grid, i);
        unsigned char col = getCol(grid, i);

        // check if row and col are balanced and have no triplets
        if (!isBalanced(row) || hasTriplets(row) ||
            !isBalanced(col) || hasTriplets(col)) { return false; }

        // check if row and col are unique, if so, store them
        for (int j = 0; j < index; j++)
        {
            if (row == rows[j] || col == cols[j]) { return false; }
        }
        rows[index] = row;
        cols[index] = col;
        index++;
    }
    return true;
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





void prettyPrint(unsigned long long state, unsigned long long actions)
{
    // Loop over all cells
    for (unsigned i = 0; i < N*N; i++)
    {   
        // Start a new line every N cells (but skip the first)
        if (i % N == 0 && i)
        {
            printf("\n---+");
            for (int j = 1; j < N-1; j++) { printf("---+"); }
            printf("---\n");
        }

        // If the cell is empty, print a space, else, print the cell's value
        printf(bit_check(actions, i) ? "   " : " %d ", bit_check(state, i));

        // Print a separator if it is not the last item in the row
        if (i % N != N-1) { printf("|"); }
    }
    printf("\n");
}

bool bit_check(unsigned long long number, unsigned n)
{
    // Returns the nth bit in number
    return (number >> n) & 1U;
}

void bitPrint(unsigned long long number, int n)
{
    // Prints the first n bits of 'number'
    // assert n is not bigger than 64
    for (int i = 0; i < n; i++)
    {
        printf("%d", number & 1);
        number >>= 1;
    }
    printf("\n");
}



bool isValidRowOrCol(unsigned long long rowOrCol, unsigned long long actions)
{
    if (!actions) // no actions available, all cells are filled in
    {
        // check if rowOrCol is balanced and has no triplets
        return (isBalanced(rowOrCol) && !hasTriplets(rowOrCol));
    }
    // there are still some cells that are empty
    // check for uniqueness is not needed for rows/cols with empty cells
    // check for balance can only fail if there are more than N/2 0's or 1's
    // check for triples can only be done if all three cells are filled

    // balance check
    unsigned int count_1 = 0;
    unsigned int count_0 = 0;
    for (int i = 0; i < N; i++)
    {
        if ((actions >> i) & 1){ continue; } // skip empty cells

        if ((rowOrCol >> i) & 1){ count_1++; }
        else { count_0++; }
    }
    if (count_0 > N/2 || count_1 > N/2) { printf("not balanced\n"); return false; }


    // check for triplets (only check if all corresponding actions are 0)
    unsigned long long adj_mask = 7; // equivalent to (1 << 3) - 1
    for (int i = 0; i < N-2; i++)
    {
        // if some actions are possible (cells are empty), continue
        if (((actions >> i) & adj_mask)) { continue; }
        // 111 + 1 = 000; 000 + 1 = 001 so result is <= 1 if all bits are equal
        if ((((rowOrCol >> i) + 1) & adj_mask) <= 1){ printf("has triplets\n"); return false; }
    }
    return true;
}

bool isValid(unsigned long long state, unsigned long long actions)
{
    // different from isSolved which only returns true for solved boards
    // isValid can also return true for partial solutions as long as the 
    // constraints can still be met.

    unsigned long long rows[N];
    unsigned long long cols[N];
    int rows_index = 0;
    int cols_index = 0;

    for (int i = 0; i < N; i++)
    {   
        unsigned char row = getRow(state, i);
        unsigned char actions_row = getRow(actions, i);
        if (!isValidRowOrCol(row, actions_row))
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

        unsigned char col = getCol(state, i);
        unsigned char actions_col = getCol(actions, i);
        if (!isValidRowOrCol(col, actions_col))
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

unsigned long long solve(unsigned long long state, unsigned long long actions)
{
    if (isSolved(state))
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
