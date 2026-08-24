find_package(Qt6 REQUIRED COMPONENTS Widgets Test)

qt_standard_project_setup()

add_library(SudokuLogic
    "SudokuGame.cpp"
    "SudokuGame.h"
)

target_include_directories(SudokuLogic PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

qt_add_executable(Sudoku
    "Sudoku.cpp"
)

target_link_libraries(Sudoku PRIVATE
    Qt6::Widgets
    SudokuLogic
)

set_target_properties(Sudoku PROPERTIES
    WIN32_EXECUTABLE ON
)

add_executable(SudokuTests
    "Tests/SudokuGameTests.cpp"
)

target_link_libraries(SudokuTests PRIVATE
    Qt6::Test
    SudokuLogic
)

add_test(
    NAME SudokuGameTests
    COMMAND SudokuTests
)