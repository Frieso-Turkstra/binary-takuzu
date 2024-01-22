#include <stdio.h>
#include <time.h>
#include "takuzu.h"


int main(void)
{
    /* An unsigned long long is used to represented a grid, in which each cell
    is represented by a bit (unsigned long long is 64 bits so the max grid size
    is 8x8). Cells that are empty or filled in with 0 cannot be distinguished,
    so 'actions' keeps track of which bits are empty. Initially, all cells are
    empty so all bits in 'state' are 0 and all bits in 'actions' are 1. */
    unsigned long long grid = 0;
    unsigned long long actions = -1;

    load(&grid, &actions);
    printGrid(grid, actions);
    solve(grid, actions);

    return 0;
}
