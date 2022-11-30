#ifndef MAIN_DIALOG_H
#define MAIN_DIALOG_H

#include <QDialog>
#include <QCloseEvent>
#include <QMessageBox>
#include <QDebug>
#include "customwidget.h"//自定义可移动的窗体
namespace Ui {
class main_dialog;
}

class main_dialog : public CustomMoveDialog
{
    Q_OBJECT

public:
    explicit main_dialog(QWidget *parent = nullptr);
    ~main_dialog();

    void closeEvent(QCloseEvent* event);

    void setinfo(int icon, QString name);
    //按键失效
    void push_button_enable(bool flag);

signals:
    void signal_close();
    //加入会议
    void signal_join_meeting();
    //创建会议
    void signal_create_meeting();
    //设置信息
    void signal_set_info();

private slots:
    //void on_pushButton_2_clicked();
    //------------------------------ui自动生成槽函数--------------------
    void on_pb_min_window_clicked();

    void on_pb_close_window_clicked();

    void on_pb_join_meeting_clicked();

    void on_pb_create_meeting_clicked();

    //--------------------自定义槽函数

    void on_pb_icon_clicked();

private:
    Ui::main_dialog *ui;

    int m_iconid;
    QString m_name;
};

#endif // MAIN_DIALOG_H
