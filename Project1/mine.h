#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
using namespace std;

const int easy = 8;
const int medium = 6;
const int hard = 5;
const int superUltraHard = 4;
const int choose = -5;
const int win = -4;
const int lose = -3;
const int clicked = -2;
const int flag = -1;
const int mine = 0;

void setMines(vector< vector<int> >& matrix, int status);
int countMines(const vector< vector<int> >& matrix, int r, int c);
int countMinesInMatrix(const vector< vector<int> >& matrix);
