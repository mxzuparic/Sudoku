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
    for (int row = 0;
        row < SudokuGame::BoardSize;
        row++)
    {
        for (int column = 0;
            column < SudokuGame::BoardSize;
            column++)
        {
            QPushButton* button =
                cellButtons[row][column];

            int value = game.valueAt(row, column);

            if (value == 0)
                button->setText("");
            else
                button->setText(QString::number(value));

            QString style =
                "font-size: 20px;"
                "border: 1px solid #777777;";

            if (game.isFixed(row, column))
            {
                style +=
                    "font-weight: bold;"
                    "background-color: #dddddd;"
                    "color: #222222;";
            }
            else
            {
                style +=
                    "background-color: white;"
                    "color: #2563eb;";
            }

            bool conflicting =
                !game.isFixed(row, column) &&
                game.hasConflict(row, column);

            if (conflicting)
            {
                style +=
                    "color: #d32f2f;"
                    "background-color: #ffe2e2;";
            }

            bool selected =
                row == selectedRow &&
                column == selectedColumn;

            if (selected)
            {
                style +=
                    "background-color: #cfe3ff;"
                    "border: 2px solid #2563eb;";
            }

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