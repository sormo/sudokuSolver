#include "AlgorithmX.h"
#include "SudokuCover.h"
#include "Common.h"

namespace AlgorithmX
{
    void applyExistingSudoku(const uint32_t sudoku[9][9], std::set<size_t>& removedRows, std::set<size_t>& removedCols, std::vector<size_t>& solution)
    {
        for (uint32_t r = 0; r < 9; r++)
        {
            for (uint32_t c = 0; c < 9; c++)
            {
                if (sudoku[r][c] == 0)
                    continue;

                // this row is part of solution, find first column that can be satisfied by this row
                size_t row = getRowInSudokuCover(r, c, sudoku[r][c] - 1);

                for (size_t col = 0; col < COL_COUNT; col++)
                {
                    if (!SudokuCover[row][col] || removedCols.count(col))
                        continue;
                    Detail::applyRow<ROW_COUNT, COL_COUNT>(SudokuCover, row, removedRows, removedCols);
                    break;
                }

                solution.push_back(row);
            }
        }
    }

    bool solveSudoku(const uint32_t sudoku[9][9], uint32_t solvedSudoku[9][9])
    {
        std::set<size_t> removedRows, removedCols;
        std::vector<size_t> solution;

        applyExistingSudoku(sudoku, removedRows, removedCols, solution);

        if (auto result = Detail::algorithmX<ROW_COUNT, COL_COUNT>(SudokuCover, removedRows, removedCols, solution))
        {
            copySolutionToSudoku(*result, solvedSudoku);

            return true;
        }

        return false;
    }
}
