#pragma once
#include <vector>

// convert row, column and value to row index in SudokuCover
size_t getRowInSudokuCover(uint32_t row, uint32_t col, uint32_t val);
// convert row index of SudokuCover to row, column and value
std::tuple<uint32_t, uint32_t, uint32_t> getCellInSudokuCover(size_t rowIndex);
// copy solution (row indices of SudokuCover) to sudoku
void copySolutionToSudoku(const std::vector<size_t>& solution, uint32_t sudoku[9][9]);
