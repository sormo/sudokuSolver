NUMBER_OF_ROWS = 9 * 9 * 9
NUMBER_OF_COLS = 9 * 9 * 4

def getIndex(row, col, val):
    return row * 9 * 9 + col * 9 + val

def generateSudokuCoverMatrix():
    result = [[ 0 for _ in range(NUMBER_OF_COLS) ] for _ in range(NUMBER_OF_ROWS)]

    # there are four constraint sets
    constraintIndex = 0

    # single value in cell
    for r in range(9):
        for c in range(9):
            for v in range(9):
                result[getIndex(r,c,v)][constraintIndex] = 1
            constraintIndex += 1

    # single value in row
    for r in range(9):
        for v in range(9):
            for c in range(9):
                result[getIndex(r,c,v)][constraintIndex] = 1
            constraintIndex += 1

    # single value in column
    for c in range(9):
        for v in range(9):
            for r in range(9):
                result[getIndex(r,c,v)][constraintIndex] = 1
            constraintIndex += 1

    # single value in box
    for r in range(0, 9, 3):
        for c in range(0, 9, 3):
            for v in range(9):
                for dr in range(3):
                    for dc in range(3):
                        result[getIndex(r + dr,c + dc, v)][constraintIndex] = 1
                constraintIndex += 1

    return result

def printMatrixToFile(file, matrix):
    print('constexpr size_t COL_COUNT = 9 * 9 * 4;', file=file)
    print('constexpr size_t ROW_COUNT = 9 * 9 * 9;', file=file)
    print('constexpr bool SudokuCover[ROW_COUNT][COL_COUNT] =', file=file)
    print('{', file=file)
    for r in matrix:
        print('    { ' + ','.join([str(x) for x in r]) + ' },', file=file)
    print('};', file=file)

matrix = generateSudokuCoverMatrix()

with open('SudokuCover.h', 'w') as file:
    printMatrixToFile(file, matrix)
