#include "main_windows.h"
#include "ui_main_windows.h"

main_windows::main_windows(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::main_windows)
{
    ui->setupUi(this);
}

main_windows::~main_windows()
{
    delete ui;
}

