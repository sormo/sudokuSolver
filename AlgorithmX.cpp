#include "AlgorithmX.h"
#include "SudokuCover.h"
#include <set>

namespace AlgorithmX
{
    size_t getIndex(uint32_t row, uint32_t col, uint32_t val)
    {
        return row * 9 * 9 + col * 9 + val;
    }

    std::tuple<uint32_t, uint32_t, uint32_t> getCell(size_t index)
    {
        uint32_t row = index / 81;
        uint32_t col = (index % 81) / 9;
        uint32_t val = (index % 81) % 9;

        return { row, col, val };
    }

    void applyExistingSudoku(const uint32_t sudoku[9][9], std::set<size_t>& removedRows, std::set<size_t>& removedCols, std::vector<size_t>& solution)
    {
        for (uint32_t r = 0; r < 9; r++)
        {
            for (uint32_t c = 0; c < 9; c++)
            {
                if (sudoku[r][c] == 0)
                    continue;

                // this row is part of solution, find first column that can be satisfied by this row
                size_t row = getIndex(r, c, sudoku[r][c] - 1);

                for (size_t col = 0; col < COL_COUNT; col++)
                {
                    if (!MatrixCover[row][col] || removedCols.count(col))
                        continue;
                    detail::applyRow<ROW_COUNT, COL_COUNT>(MatrixCover, row, col, removedRows, removedCols);
                    break;
                }

                solution.push_back(row);
            }
        }
    }

    void copySolutionToSudoku(const std::vector<size_t>& solution, uint32_t sudoku[9][9])
    {
        for (auto index : solution)
        {
            auto [r, c, v] = getCell(index);
            sudoku[r][c] = v + 1;
        }
    }

    bool solveSudoku(const uint32_t sudoku[9][9], uint32_t solvedSudoku[9][9])
    {
        std::set<size_t> removedRows, removedCols;
        std::vector<size_t> solution;

        applyExistingSudoku(sudoku, removedRows, removedCols, solution);

        if (auto result = detail::algorithmX<ROW_COUNT, COL_COUNT>(MatrixCover, removedRows, removedCols, solution))
        {
            copySolutionToSudoku(*result, solvedSudoku);

            return true;
        }

        return false;
    }
}
