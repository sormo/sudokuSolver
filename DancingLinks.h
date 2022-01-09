#pragma once
#include <optional>
#include <vector>
#include <memory>
#include "Common.h"
#include <iostream>

namespace DancingLinks
{
    namespace Detail
    {
        struct ColumnObject;

        struct DataObject
        {
            DataObject* left = nullptr;
            DataObject* right = nullptr;
            DataObject* up = nullptr;
            DataObject* down = nullptr;
            ColumnObject* header = nullptr;
            size_t rowIndex = -1;
            size_t colIndex = -1; // TODO test only
        };

        struct ColumnObject : public DataObject
        {
            uint32_t countOnes = 0;
        };

        template<class T>
        T* create(std::vector<std::unique_ptr<DataObject>>& data)
        {
            T* result = new T;
            data.emplace_back(result);
            return result;
        }

        template<size_t ROWS, size_t COLS>
        ColumnObject* createObjects(const bool matrix[ROWS][COLS], std::vector<std::unique_ptr<DataObject>>& data)
        {
            ColumnObject* root = create<ColumnObject>(data);

            std::vector<ColumnObject*> columnHeaders(COLS);
            for (size_t i = 0; i < COLS; i++)
                columnHeaders[i] = create<ColumnObject>(data);

            std::vector<std::vector<DataObject*>> rowObjects(ROWS, std::vector<DataObject*>());
            std::vector<std::vector<DataObject*>> colObjects(COLS, std::vector<DataObject*>());
            for (size_t r = 0; r < ROWS; r++)
            {
                for (size_t c = 0; c < COLS; c++)
                {
                    if (!matrix[r][c])
                        continue;
                    auto obj = create<DataObject>(data);
                    rowObjects[r].push_back(obj);
                    colObjects[c].push_back(obj);
                }
            }

            root->left = columnHeaders[COLS - 1];
            root->right = columnHeaders[0];

            for (size_t i = 0; i < COLS; i++)
            {
                columnHeaders[i]->left = i == 0 ? root : columnHeaders[i - 1];
                columnHeaders[i]->right = i == COLS - 1 ? root : columnHeaders[i + 1];
                if (!colObjects[i].empty())
                {
                    columnHeaders[i]->down = colObjects[i][0];
                    columnHeaders[i]->up = colObjects[i].back();
                }
                columnHeaders[i]->header = columnHeaders[i];
                columnHeaders[i]->countOnes = (uint32_t)colObjects[i].size();
            }

            for (size_t r = 0; r < ROWS; r++)
            {
                for (size_t i = 0; i < rowObjects[r].size(); i++)
                {
                    rowObjects[r][i]->left = i == 0 ? rowObjects[r].back() : rowObjects[r][i - 1];
                    rowObjects[r][i]->right = i == rowObjects[r].size() - 1 ? rowObjects[r][0] : rowObjects[r][i + 1];
                    rowObjects[r][i]->rowIndex = r;
                }
            }

            for (size_t c = 0; c < COLS; c++)
            {
                for (size_t i = 0; i < colObjects[c].size(); i++)
                {
                    colObjects[c][i]->up = i == 0 ? columnHeaders[c] : colObjects[c][i - 1];
                    colObjects[c][i]->down = i == colObjects[c].size() - 1 ? columnHeaders[c] : colObjects[c][i + 1];
                    colObjects[c][i]->header = columnHeaders[c];
                    colObjects[c][i]->colIndex = c;
                }
            }

            return root;
        }

        static void cover(ColumnObject* node)
        {
            node->left->right = node->right;
            node->right->left = node->left;
            for (DataObject* nodeCol = node->down; nodeCol != node; nodeCol = nodeCol->down)
            {
                // remove all rows of this column, these rows can't be used in solution
                for (DataObject* nodeRow = nodeCol->right; nodeRow != nodeCol; nodeRow = nodeRow->right)
                {
                    nodeRow->down->up = nodeRow->up;
                    nodeRow->up->down = nodeRow->down;
                    nodeRow->header->countOnes--;
                }
            }
        }

        static void uncover(ColumnObject* node)
        {
            for (DataObject* nodeCol = node->up; nodeCol != node; nodeCol = nodeCol->up)
            {
                // this row was removed from up/down of other columns, revert this
                for (DataObject* nodeRow = nodeCol->left; nodeRow != nodeCol; nodeRow = nodeRow->left)
                {
                    nodeRow->header->countOnes++;
                    nodeRow->down->up = nodeRow;
                    nodeRow->up->down = nodeRow;
                }
            }
            node->left->right = node;
            node->right->left = node;
        }

        static void search(ColumnObject* root, std::vector<size_t>& solution, std::vector<std::vector<size_t>>& solutions)
        {
            if (root->right == root)
            {
                solutions.push_back(solution);
                return;
            }

            // choose column with smallest number of ones
            ColumnObject* column = (ColumnObject*)root->right;
            for (ColumnObject* c = (ColumnObject*)column->right; c != root; c = (ColumnObject*)c->right)
            {
                if (c->countOnes < column->countOnes)
                    column = c;
            }

            cover(column);

            for (DataObject* node = column->down; node != column; node = node->down)
            {
                // pick row to solution and cover all columns of this row (they are satisfied in solution by this row)
                solution.push_back(node->rowIndex);
                for (DataObject* n = node->right; n != node; n = n->right)
                    cover(n->header);

                search(root, solution, solutions);

                solution.pop_back();
                for (DataObject* n = node->left; n != node; n = n->left)
                    uncover(n->header);
            }

            uncover(column);
        }
    }

    template <size_t ROWS, size_t COLS>
    std::optional<std::vector<std::vector<size_t>>> solve(const bool matrix[ROWS][COLS])
    {
        std::vector<std::unique_ptr<Detail::DataObject>> data;
        auto root = Detail::createObjects<ROWS, COLS>(matrix, data);

        std::vector<size_t> solution;
        std::vector<std::vector<size_t>> solutions;
        Detail::search(root, solution, solutions);

        return solutions;
    }

    bool solveSudoku(const uint32_t sudoku[9][9], uint32_t solvedSudoku[9][9]);
}
