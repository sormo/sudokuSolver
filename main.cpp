#include "AlgorithmX.h"
#include "DancingLinks.h"
#include <chrono>
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

    printSudoku(sudoku);

    uint32_t solution[9][9] = {};

    auto start = std::chrono::high_resolution_clock::now();
    if (AlgorithmX::solveSudoku(sudoku, solution))
    {
        auto duration = std::chrono::high_resolution_clock::now() - start;
        std::cout << "Duration " << std::chrono::duration_cast<std::chrono::milliseconds>(duration) << "\n";

        printSudoku(solution);
    }

    start = std::chrono::high_resolution_clock::now();
    if (DancingLinks::solveSudoku(sudoku, solution))
    {
        auto duration = std::chrono::high_resolution_clock::now() - start;
        std::cout << "Duration " << std::chrono::duration_cast<std::chrono::milliseconds>(duration) << "\n";

        printSudoku(solution);
    }
}
