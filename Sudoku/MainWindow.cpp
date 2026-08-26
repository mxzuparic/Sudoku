#include "MainWindow.h"

#include "PuzzleLoader.h"

#include <QCoreApplication>
#include <QGridLayout>
#include <QKeyEvent>
#include <QPushButton>
#include <QWidget>
#include <QMessageBox>
#include <QRandomGenerator>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QChar>
#include <QTimer>

#include <string>
#include <vector>
#include <utility>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    setWindowTitle("Sudoku");

    loadInitialPuzzle();
    createBoard();
    refreshBoard();

    gameTimer = new QTimer(this);

    connect(
        gameTimer,
        &QTimer::timeout,
        this,
        [this]()
        {
            ++elapsedSeconds;
            updateTimerDisplay();
        });

    resetTimer();
}

void MainWindow::loadInitialPuzzle()
{
    std::string filePath =
        QCoreApplication::applicationDirPath().toStdString()
        + "/data/"
        + currentDifficulty
        + ".txt";

    std::vector<SudokuGame::Board> puzzles =
        PuzzleLoader::loadFromFile(filePath);

    if (!puzzles.empty())
    {
        int newPuzzleIndex = 0;

        if (puzzles.size() > 1)
        {
            do
            {
                newPuzzleIndex =
                    QRandomGenerator::global()->bounded(
                        static_cast<int>(puzzles.size()));
            } while (newPuzzleIndex ==
                currentPuzzleIndex);
        }

        currentPuzzleIndex = newPuzzleIndex;

        game.loadPuzzle(
            puzzles[currentPuzzleIndex]);
    }
}

void MainWindow::startNewGame()
{
    loadInitialPuzzle();

    selectedRow = -1;
    selectedColumn = -1;

    refreshBoard();
    resetTimer();
}

void MainWindow::createBoard()
{
    QWidget* centralWidget = new QWidget(this);
    QVBoxLayout* mainLayout =
        new QVBoxLayout(centralWidget);

    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(12);

    QWidget* controlsWidget =
        new QWidget(centralWidget);

    QHBoxLayout* controlsLayout =
        new QHBoxLayout(controlsWidget);

    controlsLayout->setContentsMargins(0, 0, 0, 0);

    QLabel* difficultyLabel =
        new QLabel("Difficulty:", controlsWidget);

    QComboBox* difficultyComboBox =
        new QComboBox(controlsWidget);

    difficultyComboBox->addItems(
        { "Easy", "Medium", "Hard" });

    difficultyComboBox->setFixedWidth(110);
    difficultyComboBox->setFocusPolicy(Qt::NoFocus);

    timerLabel =
        new QLabel("00:00", controlsWidget);

    timerLabel->setAlignment(Qt::AlignCenter);
    timerLabel->setMinimumWidth(70);

    timerLabel->setStyleSheet(
        "font-size: 18px;"
        "font-weight: 600;"
        "color: #334155;");

    QPushButton* hintButton =
        new QPushButton("Hint", controlsWidget);

    hintButton->setFixedWidth(70);
    hintButton->setFocusPolicy(Qt::NoFocus);

    connect(
        hintButton,
        &QPushButton::clicked,
        this,
        &MainWindow::giveHint);

    QPushButton* newGameButton =
        new QPushButton("New Game", controlsWidget);

    newGameButton->setFixedWidth(120);
    newGameButton->setFocusPolicy(Qt::NoFocus);

    connect(
        newGameButton,
        &QPushButton::clicked,
        this,
        &MainWindow::startNewGame);

    connect(
        difficultyComboBox,
        &QComboBox::currentTextChanged,
        this,
        [this](const QString& difficulty)
        {
            currentDifficulty =
                difficulty.toLower().toStdString();

            currentPuzzleIndex = -1;

            startNewGame();
        });

    controlsLayout->addWidget(difficultyLabel);
    controlsLayout->addWidget(difficultyComboBox);
    controlsLayout->addStretch();
    controlsLayout->addWidget(timerLabel);
    controlsLayout->addSpacing(8);
    controlsLayout->addWidget(hintButton);
    controlsLayout->addSpacing(8);
    controlsLayout->addWidget(newGameButton);

    mainLayout->addWidget(controlsWidget);

    QWidget* boardWidget =
        new QWidget(centralWidget);

    QGridLayout* boardLayout =
        new QGridLayout(boardWidget);

    boardLayout->setSpacing(0);
    boardLayout->setContentsMargins(0, 0, 0, 0);

    for (int row = 0;
        row < SudokuGame::BoardSize;
        ++row)
    {
        for (int column = 0;
            column < SudokuGame::BoardSize;
            ++column)
        {
            QPushButton* button =
                new QPushButton(boardWidget);

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

            boardLayout->addWidget(
                button,
                row,
                column);

            cellButtons[row][column] = button;
        }
    }

    mainLayout->addWidget(boardWidget);

    QWidget* numberStatusWidget =
        new QWidget(centralWidget);

    QHBoxLayout* numberStatusLayout =
        new QHBoxLayout(numberStatusWidget);

    numberStatusLayout->setContentsMargins(0, 0, 0, 0);
    numberStatusLayout->setSpacing(6);

    numberStatusLayout->addStretch();

    for (int value = 1;
        value <= SudokuGame::BoardSize;
        ++value)
    {
        QLabel* label =
            new QLabel(
                QString::number(value),
                numberStatusWidget);

        label->setAlignment(Qt::AlignCenter);
        label->setFixedSize(45, 38);

        numberStatusLayout->addWidget(label);
        numberStatusLabels[value - 1] = label;
    }

    numberStatusLayout->addStretch();

    mainLayout->addWidget(numberStatusWidget);

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
    refreshNumberStatus();
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
        bool wasSolved = game.isSolved();

        bool valueEntered = game.setValue(
            selectedRow,
            selectedColumn,
            value);

        refreshBoard();

        if (valueEntered &&
            !wasSolved &&
            game.isSolved())
        {
            gameTimer->stop();

            QMessageBox::information(
                this,
                "Sudoku",
                "Puzzle solved!");
        }

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

void MainWindow::refreshNumberStatus()
{
    for (int value = 1;
        value <= SudokuGame::BoardSize;
        ++value)
    {
        QLabel* label =
            numberStatusLabels[value - 1];

        QString style =
            "font-size: 20px;"
            "font-weight: 600;"
            "border-radius: 6px;";

        if (game.isNumberComplete(value))
        {
            style +=
                "color: #15803d;"
                "background-color: #dcfce7;"
                "border: 1px solid #86efac;";
        }
        else
        {
            style +=
                "color: #475569;"
                "background-color: #f8fafc;"
                "border: 1px solid #cbd5e1;";
        }

        label->setStyleSheet(style);
    }
}

void MainWindow::resetTimer()
{
    elapsedSeconds = 0;
    updateTimerDisplay();
    gameTimer->start(1000);
}

void MainWindow::updateTimerDisplay()
{
    int minutes = elapsedSeconds / 60;
    int seconds = elapsedSeconds % 60;

    timerLabel->setText(
        QString("%1:%2")
        .arg(minutes, 2, 10, QChar('0'))
        .arg(seconds, 2, 10, QChar('0')));
}

void MainWindow::giveHint()
{
    int hintRow = -1;
    int hintColumn = -1;

    bool selectedCellNeedsHint =
        selectedRow >= 0 &&
        selectedColumn >= 0 &&
        !game.isFixed(
            selectedRow,
            selectedColumn) &&
        game.valueAt(
            selectedRow,
            selectedColumn) !=
        game.solutionValueAt(
            selectedRow,
            selectedColumn);

    if (selectedCellNeedsHint)
    {
        hintRow = selectedRow;
        hintColumn = selectedColumn;
    }
    else
    {
        std::vector<std::pair<int, int>> candidates;

        for (int row = 0;
            row < SudokuGame::BoardSize;
            ++row)
        {
            for (int column = 0;
                column < SudokuGame::BoardSize;
                ++column)
            {
                if (!game.isFixed(row, column) &&
                    game.valueAt(row, column) !=
                    game.solutionValueAt(row, column))
                {
                    candidates.emplace_back(
                        row,
                        column);
                }
            }
        }

        if (!candidates.empty())
        {
            int randomIndex =
                QRandomGenerator::global()->bounded(
                    static_cast<int>(
                        candidates.size()));

            hintRow =
                candidates[randomIndex].first;

            hintColumn =
                candidates[randomIndex].second;
        }
    }

    if (hintRow == -1)
        return;

    if (!game.applyHint(hintRow, hintColumn))
        return;

    selectedRow = hintRow;
    selectedColumn = hintColumn;

    refreshBoard();

    if (game.isSolved())
    {
        gameTimer->stop();

        QMessageBox::information(
            this,
            "Sudoku",
            "Puzzle solved!");
    }
}