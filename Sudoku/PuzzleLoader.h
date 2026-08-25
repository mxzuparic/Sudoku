#pragma once

#include "SudokuGame.h"

#include <string>
#include <vector>

class PuzzleLoader
{
public:
    static std::vector<SudokuGame::Board> loadFromFile(
        const std::string& filePath);
};
