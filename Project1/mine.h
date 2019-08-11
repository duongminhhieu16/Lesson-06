#pragma once
#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
using namespace std;

const int easy = 7;
const int medium = 6;
const int hard = 5;
const int superUltraHard = 4;
const int credits = -8;
const int help = -7;
const int choose = -6;
const int nothing = -5;
const int win = -4;
const int lose = -3;
const int clicked = -2;
const int flag = -1;
const int mine = 0;
const int playAgain = 10;

void setMines(vector< vector<int> >& matrix, int status);
int countMines(const vector< vector<int> >& matrix, int r, int c);
int countMinesInMatrix(const vector< vector<int> >& matrix);
