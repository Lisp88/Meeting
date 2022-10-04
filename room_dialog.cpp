#include "room_dialog.h"
#include "ui_room_dialog.h"

room_dialog::room_dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::room_dialog)
{
    ui->setupUi(this);
}

room_dialog::~room_dialog()
{
    delete ui;
}
