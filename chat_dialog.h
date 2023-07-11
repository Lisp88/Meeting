#ifndef CHAT_DIALOG_H
#define CHAT_DIALOG_H

#include <QDialog>
#include "customwidget.h"
#include "chat_info_widget.h"
#include "QDebug"
namespace Ui {
class chat_dialog;
}

class chat_dialog : public CustomMoveDialog
{
    Q_OBJECT

public:
    explicit chat_dialog(QWidget *parent = nullptr);
    ~chat_dialog();

    void add_info(int icon_id, QString name, QString text);

    void close_chat();
signals:
    void signal_send_info(QString text);
private slots:
    void on_pb_send_clicked();

    void on_pb_close_chat_clicked();
private:
    Ui::chat_dialog *ui;
};

#endif // CHAT_DIALOG_H
