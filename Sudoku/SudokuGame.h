#pragma once

#include <array>

class SudokuGame
{
public:
    static constexpr int BoardSize = 9;

    using Board =
        std::array<std::array<int, BoardSize>, BoardSize>;

    bool loadPuzzle(const Board& puzzle);
    bool applyHint(int row, int column);

    int valueAt(int row, int column) const;
    int solutionValueAt(int row, int column) const;

    bool isFixed(int row, int column) const;
    bool isValidMove(int row, int column, int value) const;
    bool hasConflict(int row, int column) const;
    bool isSolved() const;
    bool isNumberComplete(int value) const;

    bool setValue(int row, int column, int value);
    bool clearValue(int row, int column);

    Board initialPuzzle() const;
    Board currentState() const;
    bool restoreState(const Board& state);

    void reset();

private:
    Board initialBoard{};
    Board currentBoard{};
    Board solutionBoard{};

    bool puzzleLoaded = false;

    bool isInsideBoard(int row, int column) const;
    bool isBoardValid(const Board& board) const;

    bool isValidPlacement(
        const Board& board,
        int row,
        int column,
        int value) const;

    bool solveBoard(Board& board) const;
};