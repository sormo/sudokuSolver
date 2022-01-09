#include "DancingLinks.h"
#include "SudokuCover.h"
#include "Common.h"

namespace DancingLinks
{
    void applyExistingSudoku(Detail::ColumnObject* root, const uint32_t sudoku[9][9], std::vector<size_t>& solution)
    {
        for (uint32_t i = 0; i < 9; i++)
        {
            for (uint32_t j = 0; j < 9; j++)
            {
                if (sudoku[i][j] == 0)
                    continue;

                auto rowIndex = getRowInSudokuCover(i, j, sudoku[i][j] - 1);

                bool satisfied = false;
                for (auto column = root->right; column != root; column = column->right)
                {
                    for (auto row = column->down; row != column; row = row->down)
                    {
                        if (row->rowIndex != rowIndex)
                            continue;

                        // we have found some row for this sudoku cell
                        // cover all columns which are set in this row (rows which are set in the columns are not usable)

                        solution.push_back(row->rowIndex);

                        Detail::cover(row->header);
                        for (auto node = row->right; node != row; node = node->right)
                            Detail::cover(node->header);

                        satisfied = true;
                        break;
                    }
                    if (satisfied)
                        break;
                }
            }
        }
    }

    bool solveSudoku(const uint32_t sudoku[9][9], uint32_t solvedSudoku[9][9])
    {
        std::vector<std::unique_ptr<Detail::DataObject>> data;
        auto root = Detail::createObjects<ROW_COUNT, COL_COUNT>(SudokuCover, data);

        std::vector<size_t> solution;
        applyExistingSudoku(root, sudoku, solution);

        std::vector<std::vector<size_t>> solutions;
        Detail::search(root, solution, solutions);

        if (solutions.size() != 1)
            return false;

        copySolutionToSudoku(solutions[0], solvedSudoku);

        return true;
    }
}
