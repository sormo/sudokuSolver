#include "AlgorithmX.h"
#include <iostream>

void printSudoku(const uint32_t sudoku[9][9])
{
    for (uint32_t r = 0; r < 9; r++)
    {
        for (uint32_t c = 0; c < 9; c++)
        {
            std::cout << sudoku[r][c] << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

int main()
{
    const uint32_t sudoku[9][9] =
    {
        {0,0,4,0,5,0,0,0,0},
        {9,0,0,7,3,4,6,0,0},
        {0,0,3,0,2,1,0,4,9},
        {0,3,5,0,9,0,4,8,0},
        {0,9,0,0,0,0,0,3,0},
        {0,7,6,0,1,0,9,2,0},
        {3,1,0,9,7,0,2,0,0},
        {0,0,9,1,8,2,0,0,3},
        {0,0,0,0,6,0,1,0,0}
    };

    uint32_t solution[9][9];
    if (AlgorithmX::solveSudoku(sudoku, solution))
    {
        printSudoku(sudoku);
        printSudoku(solution);
    }
}
