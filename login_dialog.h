#ifndef LOGIN_DIALOG_H
#define LOGIN_DIALOG_H

#include <QDialog>
#include <QCloseEvent>
namespace Ui {
class login_dialog;
}

class login_dialog : public QDialog
{
    Q_OBJECT

signals:
    void signal_close();
    void signal_register(QString name, QString tel, QString password);//接收页面注册响应
    void signal_login(QString name, QString password);//接收页面登录响应
//    void test();
public:
    explicit login_dialog(QWidget *parent = nullptr);
    ~login_dialog();

    void closeEvent(QCloseEvent* event);

private slots:
    void on_pb_register_clicked();

    void on_pb_register_exit_clicked();

    void on_pb_login_clicked();

    void on_pb_login_eixt_clicked();

private:
    Ui::login_dialog *ui;
};

#endif // LOGIN_DIALOG_H
