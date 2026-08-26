#include <string>
#include <vector>

#include <QtTest>

#include "SudokuGame.h"
#include "PuzzleLoader.h"

namespace
{
    SudokuGame::Board createPuzzle()
    {
        return { {
            {{5, 3, 0, 0, 7, 0, 0, 0, 0}},
            {{6, 0, 0, 1, 9, 5, 0, 0, 0}},
            {{0, 9, 8, 0, 0, 0, 0, 6, 0}},
            {{8, 0, 0, 0, 6, 0, 0, 0, 3}},
            {{4, 0, 0, 8, 0, 3, 0, 0, 1}},
            {{7, 0, 0, 0, 2, 0, 0, 0, 6}},
            {{0, 6, 0, 0, 0, 0, 2, 8, 0}},
            {{0, 0, 0, 4, 1, 9, 0, 0, 5}},
            {{0, 0, 0, 0, 8, 0, 0, 7, 9}}
        } };
    }
}

class SudokuGameTests : public QObject
{
    Q_OBJECT

private slots:
    void acceptsValidMove();
    void rejectsDuplicateInRow();
    void rejectsDuplicateInColumn();
    void rejectsDuplicateInBox();
    void rejectsChangingFixedCell();
    void solvesLoadedPuzzle();
    void rejectsInvalidPuzzle();
    void loadsPuzzleFromFile();
    void storesConflictingMove();
    void recognizesCompletedNumber();
    void loadsAllDifficultyFiles();
};

void SudokuGameTests::acceptsValidMove()
{
    SudokuGame game;
    game.loadPuzzle(createPuzzle());

    QVERIFY(game.setValue(0, 2, 4));
    QCOMPARE(game.valueAt(0, 2), 4);
}

void SudokuGameTests::rejectsDuplicateInRow()
{
    SudokuGame game;
    game.loadPuzzle(createPuzzle());

    QVERIFY(!game.isValidMove(0, 2, 5));
}

void SudokuGameTests::rejectsDuplicateInColumn()
{
    SudokuGame game;
    game.loadPuzzle(createPuzzle());

    QVERIFY(!game.isValidMove(0, 2, 8));
}

void SudokuGameTests::rejectsDuplicateInBox()
{
    SudokuGame game;
    game.loadPuzzle(createPuzzle());

    QVERIFY(!game.isValidMove(0, 2, 6));
}

void SudokuGameTests::rejectsChangingFixedCell()
{
    SudokuGame game;
    game.loadPuzzle(createPuzzle());

    QVERIFY(!game.setValue(0, 0, 4));
    QCOMPARE(game.valueAt(0, 0), 5);
}

void SudokuGameTests::solvesLoadedPuzzle()
{
    SudokuGame game;

    QVERIFY(game.loadPuzzle(createPuzzle()));

    QCOMPARE(game.solutionValueAt(0, 2), 4);
    QCOMPARE(game.solutionValueAt(8, 0), 3);
}

void SudokuGameTests::rejectsInvalidPuzzle()
{
    SudokuGame::Board puzzle = createPuzzle();

    puzzle[0][2] = 5;

    SudokuGame game;

    QVERIFY(!game.loadPuzzle(puzzle));
}

void SudokuGameTests::loadsPuzzleFromFile()
{
    std::string filePath =
        std::string(TEST_DATA_DIR) + "/easy.txt";

    std::vector<SudokuGame::Board> puzzles =
        PuzzleLoader::loadFromFile(filePath);

    QCOMPARE(static_cast<int>(puzzles.size()), 2);
    QVERIFY(!puzzles.empty());

    SudokuGame game;

    QVERIFY(game.loadPuzzle(puzzles.front()));

    QCOMPARE(game.valueAt(0, 0), 5);
    QCOMPARE(game.valueAt(0, 2), 0);
    QCOMPARE(game.solutionValueAt(0, 2), 4);
}

void SudokuGameTests::storesConflictingMove()
{
    SudokuGame game;
    game.loadPuzzle(createPuzzle());

    QVERIFY(game.setValue(0, 2, 5));

    QCOMPARE(game.valueAt(0, 2), 5);
    QVERIFY(game.hasConflict(0, 2));
}

void SudokuGameTests::recognizesCompletedNumber()
{
    SudokuGame game;
    game.loadPuzzle(createPuzzle());

    int targetValue = 0;

    for (int row = 0;
        row < SudokuGame::BoardSize &&
        targetValue == 0;
        ++row)
    {
        for (int column = 0;
            column < SudokuGame::BoardSize;
            ++column)
        {
            if (!game.isFixed(row, column))
            {
                targetValue =
                    game.solutionValueAt(
                        row,
                        column);

                break;
            }
        }
    }

    QVERIFY(targetValue >= 1);
    QVERIFY(!game.isNumberComplete(targetValue));

    for (int row = 0;
        row < SudokuGame::BoardSize;
        ++row)
    {
        for (int column = 0;
            column < SudokuGame::BoardSize;
            ++column)
        {
            if (!game.isFixed(row, column) &&
                game.solutionValueAt(
                    row,
                    column) == targetValue)
            {
                QVERIFY(game.setValue(
                    row,
                    column,
                    targetValue));
            }
        }
    }

    QVERIFY(game.isNumberComplete(targetValue));
}

void SudokuGameTests::loadsAllDifficultyFiles()
{
    const std::vector<std::string> fileNames =
    {
        "easy.txt",
        "medium.txt",
        "hard.txt"
    };

    for (const std::string& fileName : fileNames)
    {
        std::string filePath =
            std::string(TEST_DATA_DIR)
            + "/"
            + fileName;

        std::vector<SudokuGame::Board> puzzles =
            PuzzleLoader::loadFromFile(filePath);

        QCOMPARE(
            puzzles.size(),
            static_cast<std::size_t>(2));

        for (const SudokuGame::Board& puzzle : puzzles)
        {
            SudokuGame game;
            QVERIFY(game.loadPuzzle(puzzle));
        }
    }
}

QTEST_APPLESS_MAIN(SudokuGameTests)

#include "SudokuGameTests.moc"