#include "MainWindow.h"

#include "PuzzleLoader.h"

#include <QCoreApplication>
#include <QGridLayout>
#include <QKeyEvent>
#include <QPushButton>
#include <QWidget>

#include <string>
#include <vector>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    setWindowTitle("Sudoku");

    loadInitialPuzzle();
    createBoard();
    refreshBoard();
}

void MainWindow::loadInitialPuzzle()
{
    std::string filePath =
        (QCoreApplication::applicationDirPath() +
            "/data/easy.txt").toStdString();

    std::vector<SudokuGame::Board> puzzles =
        PuzzleLoader::loadFromFile(filePath);

    if (!puzzles.empty())
        game.loadPuzzle(puzzles.front());
}

void MainWindow::createBoard()
{
    QWidget* centralWidget = new QWidget(this);
    QGridLayout* layout = new QGridLayout(centralWidget);

    layout->setSpacing(0);
    layout->setContentsMargins(20, 20, 20, 20);

    for (int row = 0;
        row < SudokuGame::BoardSize;
        row++)
    {
        for (int column = 0;
            column < SudokuGame::BoardSize;
            column++)
        {
            QPushButton* button =
                new QPushButton(centralWidget);

            button->setFixedSize(55, 55);
            button->setFocusPolicy(Qt::NoFocus);

            connect(
                button,
                &QPushButton::clicked,
                this,
                [this, row, column]()
                {
                    selectCell(row, column);
                });

            layout->addWidget(button, row, column);
            cellButtons[row][column] = button;
        }
    }

    setCentralWidget(centralWidget);
    adjustSize();
}

void MainWindow::refreshBoard()
{
    for (int row = 0; row < SudokuGame::BoardSize; ++row)
    {
        for (int column = 0; column < SudokuGame::BoardSize; ++column)
        {
            QPushButton* button = cellButtons[row][column];

            int value = game.valueAt(row, column);
            button->setText(value == 0 ? "" : QString::number(value));

            bool fixedCell = game.isFixed(row, column);
            bool conflicting =
                !fixedCell && game.hasConflict(row, column);

            QString style =
                "font-size: 26px;"
                "font-weight: 600;"
                "background-color: white;";

            if (conflicting)
            {
                style +=
                    "color: #d32f2f;"
                    "background-color: #ffe2e2;";
            }
            else if (fixedCell)
            {
                style += "color: #1f2937;";
            }
            else
            {
                style += "color: #2563eb;";
            }

            if (row == selectedRow &&
                column == selectedColumn &&
                !conflicting)
            {
                style += "background-color: #eaf3ff;";
            }

            int leftBorder = column == 0 ? 3 : 0;
            int topBorder = row == 0 ? 3 : 0;
            int rightBorder = column % 3 == 2 ? 3 : 1;
            int bottomBorder = row % 3 == 2 ? 3 : 1;

            style += QString(
                "border-left: %1px solid #334155;"
                "border-top: %2px solid #334155;"
                "border-right: %3px solid #334155;"
                "border-bottom: %4px solid #334155;"
            )
                .arg(leftBorder)
                .arg(topBorder)
                .arg(rightBorder)
                .arg(bottomBorder);

            button->setStyleSheet(style);
        }
    }
}

void MainWindow::selectCell(int row, int column)
{
    selectedRow = row;
    selectedColumn = column;

    refreshBoard();
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
    bool hasSelection =
        selectedRow >= 0 &&
        selectedColumn >= 0;

    if (!hasSelection)
    {
        QMainWindow::keyPressEvent(event);
        return;
    }

    int key = event->key();

    if (key >= Qt::Key_1 && key <= Qt::Key_9)
    {
        int value = key - Qt::Key_0;

        game.setValue(
            selectedRow,
            selectedColumn,
            value);

        refreshBoard();
        return;
    }

    if (key == Qt::Key_Backspace ||
        key == Qt::Key_Delete ||
        key == Qt::Key_0)
    {
        game.clearValue(
            selectedRow,
            selectedColumn);

        refreshBoard();
        return;
    }

    QMainWindow::keyPressEvent(event);
}