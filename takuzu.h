#ifndef BINAIRO_H
#define BINAIRO_H

#define bool int
#define true 1
#define false 0

#define N 6

unsigned char getRow(unsigned long long grid, int index);
unsigned char getCol(unsigned long long grid, int index);
bool isBalanced(unsigned char rowOrCol);
bool hasTriplets(unsigned char rowOrCol);
bool isSolved(unsigned long long grid);

unsigned long long str2ull(const char* bits);
void prettyPrint(unsigned long long grid, unsigned long long actions);
bool isValidRowOrCol(unsigned long long rowOrCol, unsigned long long actions);
bool isValid(unsigned long long grid, unsigned long long actions);
bool bit_check(unsigned long long number, unsigned n);
void bitPrint(unsigned long long number, int n);
void load(unsigned long long* grid, unsigned long long* actions);
unsigned long long solve(unsigned long long grid, unsigned long long actions);

#endif
