#ifndef TAKUZU_H
#define TAKUZU_H

#define bool int
#define true 1
#define false 0

#define N 6

unsigned char getRow(unsigned long long grid, int index);
unsigned char getCol(unsigned long long grid, int index);
bool isBalanced(unsigned char rowOrCol, unsigned char actions);
bool hasTriplets(unsigned char rowOrCol, unsigned char actions);
bool isValid(unsigned long long grid, unsigned long long actions);
void printGrid(unsigned long long grid, unsigned long long actions);
bool solve(unsigned long long grid, unsigned long long actions);
bool load(const char* puzzle, unsigned long long* grid, unsigned long long* actions);

#endif
