# Sudoku

## How to Run

To build and run the application, you need Visual Studio 2022 with C++ tools, CMake and Qt 6 for MSVC 2022 64-bit.

Open the repository folder in Visual Studio and set CMAKE_PREFIX_PATH to your Qt installation folder. For example: C:/Qt/6.11.2/msvc2022_64.

Select the **x64 Debug** configuration, choose **Build All** and run Sudoku.exe.

## Running the tests

After building, open **Test** > **Test Explorer** in Visual Studio and select **Run All Tests**.

## Controls

Choose a difficulty from the drop-down menu and select **New Game** to start a new puzzle.

Click a cell and use the number keys from 1 to 9 to enter a value. Use Backspace, Delete or 0 to clear it. Starting numbers cannot be changed, and conflicting values are marked in red.

Selecting a cell highlights its row, column, 3 × 3 block and matching numbers. Numbers in the bottom row can also be clicked to highlight them on the board. A number turns green when all of its values are correctly placed.

**Hint** fills the selected empty cell or chooses one randomly if no empty cell is selected. After using a hint, it becomes available again after 10 seconds.

**Save Game** saves the current puzzle and elapsed time. **Load Game** restores the saved game.
