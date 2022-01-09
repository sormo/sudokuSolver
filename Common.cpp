#include "Common.h"

size_t getRowInSudokuCover(uint32_t row, uint32_t col, uint32_t val)
{
    return row * 9 * 9 + col * 9 + val;
}

std::tuple<uint32_t, uint32_t, uint32_t> getCellInSudokuCover(size_t index)
{
    uint32_t row = index / 81;
    uint32_t col = (index % 81) / 9;
    uint32_t val = (index % 81) % 9;

    return { row, col, val };
}

void copySolutionToSudoku(const std::vector<size_t>& solution, uint32_t sudoku[9][9])
{
    for (auto index : solution)
    {
        auto [r, c, v] = getCellInSudokuCover(index);
        sudoku[r][c] = v + 1;
    }
}
