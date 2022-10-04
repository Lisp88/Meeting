#include "main_dialog.h"
#include "ui_main_dialog.h"

main_dialog::main_dialog(QWidget *parent) :
    CustomMoveDialog(parent),
    ui(new Ui::main_dialog)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::FramelessWindowHint);
}

main_dialog::~main_dialog()
{
    delete ui;
}

void main_dialog::closeEvent(QCloseEvent *event)
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

void main_dialog::setinfo(int icon, QString name)
{
    m_name = name;
    m_iconid = icon;

    ui->lb_name->setText(m_name);
    //对资源的引用 :/xx/xx.xx
    ui->pb_icon->setIcon(QIcon(QString(":/resource/tx/%1.png").arg(icon)));
}
//---------------------------------ui界面槽函数--------------------------------------
void main_dialog::on_pb_min_window_clicked()
{
    this->showMinimized();//窗口最小化
}


void main_dialog::on_pb_close_window_clicked()
{
    this->close(); //触发close事件
}

//加入会议
void main_dialog::on_pb_join_meeting_clicked()
{
    Q_EMIT signal_join_meeting();
}

//创建会议
void main_dialog::on_pb_create_meeting_clicked()
{
    Q_EMIT signal_create_meeting();
}

