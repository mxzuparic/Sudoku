#include <QApplication>
#include <QWidget>

int main(int argc, char* argv[])
{
    QApplication application(argc, argv);

    QWidget window;
    window.setWindowTitle("Sudoku");
    window.resize(500, 600);
    window.show();

    return application.exec();
}