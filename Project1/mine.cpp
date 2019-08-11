#pragma once
#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
using namespace std;

void setMines(vector< vector<int> >& matrix, int status)
{
    int r = matrix.size();
    if(r <= 0) return;
    int c = matrix[0].size();
    for(int i = 0; i < r; i++)
    {
        for(int j = 0; j < c; j++)
        {
            matrix[i][j] = rand() % status;
        }
    }
}

int countMines(const vector< vector<int> >& matrix, int r, int c)
{
    int mines = 0;
    int cnt = 0;
    if(matrix[r][c] == mines) return 10;//mine's place in the picture
    if(r == 0)
    {
        if(c == 0)
        {
            if(matrix[r][c+1] == mines) cnt++;
            if(matrix[r+1][c+1] == mines) cnt++;
            if(matrix[r+1][c] == mines) cnt++;
        }
        else if(c + 1 == matrix[0].size())
        {
            if(matrix[r][c-1] == mines) cnt++;
            if(matrix[r+1][c-1] == mines) cnt++;
            if(matrix[r+1][c] == mines) cnt++;
        }
        else
        {
            if(matrix[r][c+1] == mines) cnt++;
            if(matrix[r+1][c+1] == mines) cnt++;
            if(matrix[r+1][c] == mines) cnt++;
            if(matrix[r][c-1] == mines) cnt++;
            if(matrix[r+1][c-1] == mines) cnt++;
        }
    }
    else if(r + 1 == matrix.size())
    {
         if(c == 0)
        {

            if(matrix[r][c+1] == mines) cnt++;
            if(matrix[r-1][c+1] == mines) cnt++;
            if(matrix[r-1][c] == mines) cnt++;
        }
        else if(c == matrix[0].size()-1)
        {
            if(matrix[r][c-1] == mines) cnt++;
            if(matrix[r-1][c-1] == mines) cnt++;
            if(matrix[r-1][c] == mines) cnt++;
        }
        else
        {
            if(matrix[r][c+1] == mines) cnt++;
            if(matrix[r-1][c+1] == mines) cnt++;
            if(matrix[r-1][c] == mines) cnt++;
            if(matrix[r][c-1] == mines) cnt++;
            if(matrix[r-1][c-1] == mines) cnt++;
        }
    }
    else
    {
        if(c == 0)
        {
            if(matrix[r][c+1] == mines) cnt++;
            if(matrix[r-1][c+1] == mines) cnt++;
            if(matrix[r-1][c] == mines) cnt++;
            if(matrix[r+1][c] == mines) cnt++;
            if(matrix[r+1][c+1] == mines) cnt++;
        }
        else if(c + 1 == matrix[0].size())
        {
            if(matrix[r][c-1] == mines) cnt++;
            if(matrix[r-1][c-1] == mines) cnt++;
            if(matrix[r-1][c] == mines) cnt++;
            if(matrix[r+1][c-1] == mines) cnt++;
            if(matrix[r+1][c] == mines) cnt++;
        }
        else
        {
            if(matrix[r][c+1] == mines) cnt++;
            if(matrix[r][c-1] == mines) cnt++;
            if(matrix[r+1][c+1] == mines) cnt++;
            if(matrix[r+1][c-1] == mines) cnt++;
            if(matrix[r+1][c] == mines) cnt++;
            if(matrix[r-1][c+1] == mines) cnt++;
            if(matrix[r-1][c-1] == mines) cnt++;
            if(matrix[r-1][c] == mines) cnt++;
        }
    }
    return cnt;
}

int countMinesInMatrix(const vector< vector<int> >& matrix)
{
    int cnt = 0;
    int r = matrix.size();
    if(r <= 0) return 0;
    int c = matrix[0].size();
    for(int i = 0; i < r; i++)
    {
        for(int j = 0; j < c; j++)
        {
            if(matrix[i][j] == 0)
            {
                cnt++;
            }
        }
    }
    return cnt;
}
