#include "SudokuGame.h"

bool SudokuGame::loadPuzzle(const Board& puzzle)
{
    if (!isBoardValid(puzzle))
        return false;

    Board solvedBoard = puzzle;

    if (!solveBoard(solvedBoard))
        return false;

    initialBoard = puzzle;
    currentBoard = puzzle;
    solutionBoard = solvedBoard;
    puzzleLoaded = true;

    return true;
}

int SudokuGame::valueAt(int row, int column) const
{
    if (!isInsideBoard(row, column))
        return 0;

    return currentBoard[row][column];
}

int SudokuGame::solutionValueAt(
    int row,
    int column) const
{
    if (!puzzleLoaded || !isInsideBoard(row, column))
        return 0;

    return solutionBoard[row][column];
}

bool SudokuGame::isFixed(int row, int column) const
{
    if (!puzzleLoaded || !isInsideBoard(row, column))
        return false;

    return initialBoard[row][column] != 0;
}

bool SudokuGame::isValidMove(
    int row,
    int column,
    int value) const
{
    if (!puzzleLoaded)
        return false;

    return isValidPlacement(
        currentBoard,
        row,
        column,
        value);
}

bool SudokuGame::hasConflict(
    int row,
    int column) const
{
    if (!puzzleLoaded ||
        !isInsideBoard(row, column))
    {
        return false;
    }

    int value = currentBoard[row][column];

    if (value == 0)
        return false;

    return !isValidPlacement(
        currentBoard,
        row,
        column,
        value);
}

bool SudokuGame::isSolved() const
{
    return puzzleLoaded &&
        currentBoard == solutionBoard;
}

bool SudokuGame::isNumberComplete(int value) const
{
    if (!puzzleLoaded ||
        value < 1 ||
        value > 9)
    {
        return false;
    }

    for (int row = 0; row < BoardSize; ++row)
    {
        for (int column = 0;
            column < BoardSize;
            ++column)
        {
            bool currentlyContainsValue =
                valueAt(row, column) == value;

            bool shouldContainValue =
                solutionValueAt(row, column) == value;

            if (currentlyContainsValue !=
                shouldContainValue)
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
    if (!puzzleLoaded)
        return false;

    if (!isInsideBoard(row, column))
        return false;

    if (isFixed(row, column))
        return false;

    if (value < 1 || value > 9)
        return false;

    currentBoard[row][column] = value;
    return true;
}

bool SudokuGame::clearValue(int row, int column)
{
    if (!puzzleLoaded)
        return false;

    if (!isInsideBoard(row, column))
        return false;

    if (isFixed(row, column))
        return false;

    currentBoard[row][column] = 0;
    return true;
}

void SudokuGame::reset()
{
    if (puzzleLoaded)
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

bool SudokuGame::isBoardValid(const Board& board) const
{
    for (int row = 0; row < BoardSize; row++)
    {
        for (int column = 0;
            column < BoardSize;
            column++)
        {
            int value = board[row][column];

            if (value < 0 || value > 9)
                return false;

            if (value != 0 &&
                !isValidPlacement(
                    board,
                    row,
                    column,
                    value))
            {
                return false;
            }
        }
    }

    return true;
}

bool SudokuGame::isValidPlacement(
    const Board& board,
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
            board[row][i] == value)
        {
            return false;
        }

        if (i != row &&
            board[i][column] == value)
        {
            return false;
        }
    }

    int startRow = (row / 3) * 3;
    int startColumn = (column / 3) * 3;

    for (int r = startRow; r < startRow + 3; r++)
    {
        for (int c = startColumn;
            c < startColumn + 3;
            c++)
        {
            if ((r != row || c != column) &&
                board[r][c] == value)
            {
                return false;
            }
        }
    }

    return true;
}

bool SudokuGame::solveBoard(Board& board) const
{
    for (int row = 0; row < BoardSize; row++)
    {
        for (int column = 0;
            column < BoardSize;
            column++)
        {
            if (board[row][column] != 0)
                continue;

            for (int value = 1;
                value <= BoardSize;
                value++)
            {
                if (!isValidPlacement(
                    board,
                    row,
                    column,
                    value))
                {
                    continue;
                }

                board[row][column] = value;

                if (solveBoard(board))
                    return true;

                board[row][column] = 0;
            }

            return false;
        }
    }

    return true;
}