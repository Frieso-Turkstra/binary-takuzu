#ifndef TAKUZU_H
#define TAKUZU_H

#define bool int
#define true 1
#define false 0

#define N 8

unsigned char getRow(unsigned long long grid, int index);
unsigned char getCol(unsigned long long grid, int index);
bool isBalanced(unsigned char rowOrCol, unsigned char actions);
bool hasTriplets(unsigned char rowOrCol, unsigned char actions);
bool isValid(unsigned long long grid, unsigned long long actions);
void printGrid(unsigned long long grid, unsigned long long actions);

unsigned long long str2ull(const char* bits);
void bitPrint(unsigned long long number, int n);
void load(unsigned long long* grid, unsigned long long* actions);
unsigned long long solve(unsigned long long grid, unsigned long long actions);

#endif
