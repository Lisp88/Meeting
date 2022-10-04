#include "main_windows.h"
#include "kernel.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Kernel kernel;
    return a.exec();
}
