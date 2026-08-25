#include <QtTest>

#include "SudokuGame.h"

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

QTEST_APPLESS_MAIN(SudokuGameTests)

#include "SudokuGameTests.moc"