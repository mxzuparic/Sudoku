find_package(Qt6 REQUIRED COMPONENTS Widgets Test)

qt_standard_project_setup()

add_library(SudokuLogic
    "SudokuGame.cpp"
    "SudokuGame.h"
    "PuzzleLoader.cpp"
    "PuzzleLoader.h"
)

target_include_directories(SudokuLogic PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

qt_add_executable(Sudoku
    "Sudoku.cpp"
    "MainWindow.cpp"
    "MainWindow.h"
)

target_link_libraries(Sudoku PRIVATE
    Qt6::Widgets
    SudokuLogic
)

set_target_properties(Sudoku PROPERTIES
    WIN32_EXECUTABLE ON
)

add_custom_command(TARGET Sudoku POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/data"
        "$<TARGET_FILE_DIR:Sudoku>/data"
)

add_executable(SudokuTests
    "Tests/SudokuGameTests.cpp"
)

target_link_libraries(SudokuTests PRIVATE
    Qt6::Test
    SudokuLogic
)

target_compile_definitions(SudokuTests PRIVATE
    TEST_DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data"
)

add_test(
    NAME SudokuGameTests
    COMMAND SudokuTests
)