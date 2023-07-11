#include "login_dialog.h"
#include "ui_login_dialog.h"
#include "QMessageBox"
#include "regex"
login_dialog::login_dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::login_dialog)
{
    ui->setupUi(this);

    ui->tabWidget->setCurrentIndex(0);
}

login_dialog::~login_dialog()
{
    delete ui;
}

#include <QDebug>
void login_dialog::closeEvent(QCloseEvent* event)
{
    event->ignore();
    if(QMessageBox::question(this, "退出提示", "是否退出?") == QMessageBox::Yes)
    {
        qDebug()<<"client login close";
        Q_EMIT signal_close();
    }else{
        return;
    }
}

//-------------------------ui 登录 注册 槽函数 实现-------------------------
void login_dialog::on_pb_register_clicked()
{
    QString name = ui->le_register_user->text();
    QString tel = ui->le_register_tel->text();
    QString password = ui->le_register_password->text();
    //判断合法
    //用户名
    if(name.isEmpty()){
        QMessageBox::about(this, "提示", "用户名不允许为空！");
        return;
    }else if(name.length() > 16){
        QMessageBox::about(this, "提示", "用户名长度不能超过十六个字符!");
        return;
    }
    //电话
    if(tel.isEmpty()){
        QMessageBox::about(this, "提示", "电话不允许为空！");
        return;
    }else{
        //正则判断
        std::string std_tel = tel.toStdString();
        std::regex reg("^(13[0-9]|14[01456879]|15[0-35-9]|16[2567]|17[0-8]|18[0-9]|19[0-35-9])\\d{8}");
        std::smatch sm;
        if(!std::regex_match(std_tel, sm, reg)){
            QMessageBox::about(this, "提示", "手机号格式不正确！");
            return;
        }
    }
   //密码
    if(password.isEmpty()){
        QMessageBox::about(this, "提示", "密码不允许为空！");
        return;
    }else if(password.length() > 16){
        QMessageBox::about(this, "提示", "密码长度不能超过十六个字符!");
        return;
    }
    if(password != ui->le_register_repeat_password->text()){
        QMessageBox::about(this, "提示", "两次密码书写不一致！");
        return;
    }
    //发送注册信号
    Q_EMIT signal_register(name, tel, password);
}


void login_dialog::on_pb_register_exit_clicked()
{
    ui->le_register_password->clear();
    ui->le_register_user->clear();
    ui->le_register_repeat_password->clear();
    ui->le_register_tel->clear();
}


void login_dialog::on_pb_login_clicked()
{
    QString tel = ui->le_login_tel->text();
    QString password = ui->le_login_password->text();
    //电话
    if(tel.isEmpty()){
        QMessageBox::about(this, "提示", "电话不允许为空！");
        return;
    }else{
        //正则判断
        std::regex reg("^(13[0-9]|14[01456879]|15[0-35-9]|16[2567]|17[0-8]|18[0-9]|19[0-35-9])\\d{8}");
        std::smatch sm;
        std::string std_tel = tel.toStdString();
        if(!std::regex_match(std_tel, sm, reg)){
            QMessageBox::about(this, "提示", "手机号格式不正确！");
            return;
        }
    }
    if(password.isEmpty()){
        QMessageBox::about(this, "提示", "密码不允许为空！");
        return;
    }else if(password.length() > 16){
        QMessageBox::about(this, "提示", "密码长度不能超过十六个字符!");
        return;
    }
    Q_EMIT signal_login(tel, password);
//    Q_EMIT test();
}


void login_dialog::on_pb_login_eixt_clicked()
{
    ui->le_login_tel->clear();
    ui->le_login_password->clear();
}

