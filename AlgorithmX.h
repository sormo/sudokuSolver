#pragma once
#include <optional>
#include <vector>
#include <set>

namespace AlgorithmX
{
    template<size_t ROWS, size_t COLS>
    std::optional<std::vector<std::size_t>> solve(const bool matrix[ROWS][COLS])
    {
        std::set<size_t> removedRows, removedCols;
        std::vector<size_t> solution;

        return algorithmX(matrix, removedRows, removedCols, solution);
    }

    bool solveSudoku(const uint32_t sudoku[9][9], uint32_t solvedSudoku[9][9]);

    namespace detail
    {
        template<size_t ROWS, size_t COLS>
        void applyRow(const bool matrix[ROWS][COLS], size_t row, size_t col, std::set<size_t>& removedRows, std::set<size_t>& removedCols)
        {
            for (size_t remCol = 0; remCol < COLS; remCol++)
            {
                if (removedCols.count(remCol))
                    continue;

                // check whether this column has set row which was picked for solution
                if (!matrix[row][remCol])
                    continue;

                for (size_t remRow = 0; remRow < ROWS; remRow++)
                {
                    if (removedRows.count(remRow))
                        continue;

                    if (!matrix[remRow][remCol])
                        continue;

                    removedRows.insert(remRow);
                }

                removedCols.insert(remCol);
            }
        }

        template<size_t ROWS, size_t COLS>
        std::optional<std::vector<size_t>> algorithmX(const bool matrix[ROWS][COLS], std::set<size_t> removedRows, std::set<size_t> removedCols, std::vector<size_t>& solution)
        {
            if (removedCols.size() == COLS)
                return solution;

            // pick some unsatisfied column
            for (size_t col = 0; col < COLS; col++)
            {
                if (removedCols.count(col))
                    continue;

                // pick row that will satisfy column col
                for (size_t row = 0; row < ROWS; row++)
                {
                    if (removedRows.count(row))
                        continue;

                    if (matrix[row][col])
                    {
                        solution.push_back(row);
                        std::set<size_t> newRemovedRows(removedRows);
                        std::set<size_t> newRemovedCols(removedCols);

                        // we have picked row for solution
                        // now :
                        // all columns which has set this row will be satisifed
                        // all rows which are set in satisfied columns, can't be used
                        applyRow<ROWS, COLS>(matrix, row, col, newRemovedRows, newRemovedCols);

                        if (auto result = algorithmX<ROWS, COLS>(matrix, newRemovedRows, newRemovedCols, solution))
                            return result;

                        solution.pop_back();
                    }
                }

                // if no row can satisfies this column, fail
                return {};
            }

            return {};
        }
    }
}
