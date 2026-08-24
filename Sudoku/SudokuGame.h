#pragma once

#include <array>

class SudokuGame
{
public:
    static constexpr int BoardSize = 9;

    using Board =
        std::array<std::array<int, BoardSize>, BoardSize>;

    void loadPuzzle(const Board& puzzle);

    int valueAt(int row, int column) const;
    bool isFixed(int row, int column) const;
    bool isValidMove(int row, int column, int value) const;

    bool setValue(int row, int column, int value);
    bool clearValue(int row, int column);

    void reset();

private:
    Board initialBoard{};
    Board currentBoard{};
    Board solutionBoard{};

    bool isInsideBoard(int row, int column) const;
};