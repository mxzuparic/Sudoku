#include "SudokuGame.h"

void SudokuGame::loadPuzzle(const Board& puzzle)
{
    initialBoard = puzzle;
    currentBoard = puzzle;
    solutionBoard = {};
}

int SudokuGame::valueAt(int row, int column) const
{
    if (!isInsideBoard(row, column))
        return 0;

    return currentBoard[row][column];
}

bool SudokuGame::isFixed(int row, int column) const
{
    if (!isInsideBoard(row, column))
        return false;

    return initialBoard[row][column] != 0;
}

bool SudokuGame::isValidMove(
    int row,
    int column,
    int value) const
{
    if (!isInsideBoard(row, column))
        return false;

    if (value < 1 || value > 9)
        return false;

    for (int i = 0; i < BoardSize; i++)
    {
        if (i != column &&
            currentBoard[row][i] == value)
        {
            return false;
        }

        if (i != row &&
            currentBoard[i][column] == value)
        {
            return false;
        }
    }

    int startRow = (row / 3) * 3;
    int startColumn = (column / 3) * 3;

    for (int r = startRow; r < startRow + 3; r++)
    {
        for (int c = startColumn; c < startColumn + 3; c++)
        {
            if ((r != row || c != column) &&
                currentBoard[r][c] == value)
            {
                return false;
            }
        }
    }

    return true;
}

bool SudokuGame::setValue(
    int row,
    int column,
    int value)
{
    if (!isInsideBoard(row, column))
        return false;

    if (isFixed(row, column))
        return false;

    if (!isValidMove(row, column, value))
        return false;

    currentBoard[row][column] = value;
    return true;
}

bool SudokuGame::clearValue(int row, int column)
{
    if (!isInsideBoard(row, column))
        return false;

    if (isFixed(row, column))
        return false;

    currentBoard[row][column] = 0;
    return true;
}

void SudokuGame::reset()
{
    currentBoard = initialBoard;
}

bool SudokuGame::isInsideBoard(
    int row,
    int column) const
{
    return row >= 0 &&
        row < BoardSize &&
        column >= 0 &&
        column < BoardSize;
}