#include "PuzzleLoader.h"

#include <fstream>

std::vector<SudokuGame::Board>
PuzzleLoader::loadFromFile(const std::string& filePath)
{
    std::ifstream file(filePath);

    if (!file.is_open())
        return {};

    std::vector<SudokuGame::Board> puzzles;

    while (true)
    {
        SudokuGame::Board board{};

        for (int row = 0;
            row < SudokuGame::BoardSize;
            row++)
        {
            for (int column = 0;
                column < SudokuGame::BoardSize;
                column++)
            {
                int value = 0;

                if (!(file >> value))
                {
                    bool finishedCleanly =
                        file.eof() &&
                        row == 0 &&
                        column == 0;

                    if (finishedCleanly)
                        return puzzles;

                    return {};
                }

                if (value < 0 || value > 9)
                    return {};

                board[row][column] = value;
            }
        }

        puzzles.push_back(board);
    }
}