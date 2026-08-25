# Sudoku

## How to Run

To build and run the application, you need Visual Studio 2022 with C++ tools, CMake and Qt 6 for MSVC 2022 64-bit.

Open the repository folder in Visual Studio and set CMAKE_PREFIX_PATH to your Qt installation folder. For example: C:/Qt/6.11.2/msvc2022_64.

Select the **x64 Debug** configuration, choose **Build All** and run Sudoku.exe.

## Controls

Click an empty cell and use the number keys from 1 to 9 to enter a value.

Use Backspace, Delete or 0 to clear the selected cell. Starting numbers cannot be changed. If a number conflicts with the same row, column or 3 × 3 block, it will be marked in red.