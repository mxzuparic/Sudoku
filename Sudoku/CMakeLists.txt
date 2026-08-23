find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_standard_project_setup()

qt_add_executable(Sudoku
    "Sudoku.cpp"
    "Sudoku.h"
)

target_link_libraries(Sudoku PRIVATE Qt6::Widgets)

set_target_properties(Sudoku PROPERTIES
    WIN32_EXECUTABLE ON
)