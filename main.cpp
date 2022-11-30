#include "kernel.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Kernel::get_instance();
    //Kernel kernel;
    return a.exec();
}
