#pragma once

#include "SudokuGame.h"

#include <QMainWindow>

#include <array>
#include <string>

class QLabel;
class QKeyEvent;
class QPushButton;
class QTimer;

class MainWindow : public QMainWindow
{
public:
    explicit MainWindow(QWidget* parent = nullptr);

protected:
    void keyPressEvent(QKeyEvent* event) override;

private:
    SudokuGame game;

    std::array<
        std::array<QPushButton*, SudokuGame::BoardSize>,
        SudokuGame::BoardSize> cellButtons{};

    std::array<
        QLabel*,
        SudokuGame::BoardSize> numberStatusLabels{};

    int selectedRow = -1;
    int selectedColumn = -1;
    int currentPuzzleIndex = -1;
    std::string currentDifficulty = "easy";
    QTimer* gameTimer = nullptr;
    QLabel* timerLabel = nullptr;

    int elapsedSeconds = 0;

    void loadInitialPuzzle();
    void createBoard();
    void refreshBoard();
    void selectCell(int row, int column);
    void startNewGame();
    void refreshNumberStatus();
    void resetTimer();
    void updateTimerDisplay();
    void giveHint();
};