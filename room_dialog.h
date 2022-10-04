#ifndef ROOM_DIALOG_H
#define ROOM_DIALOG_H

#include <QDialog>

namespace Ui {
class room_dialog;
}

class room_dialog : public QDialog
{
    Q_OBJECT

public:
    explicit room_dialog(QWidget *parent = nullptr);
    ~room_dialog();

private:
    Ui::room_dialog *ui;
};

#endif // ROOM_DIALOG_H
