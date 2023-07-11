#include "room_dialog.h"
#include "ui_room_dialog.h"

room_dialog::room_dialog(QWidget *parent) :
    CustomMoveDialog(parent),
    ui(new Ui::room_dialog)
{
    ui->setupUi(this);

    m_user_layout = new QVBoxLayout;

    //用户列表垂直布局
    ui->wdg_user_list->setLayout(m_user_layout);

    m_user_layout->setContentsMargins(0, 0, 0, 0);//距离外边缘
    m_user_layout->setSpacing(5);//控件之间距离

    //添加测试user 因为room页面包含该控件，所以不用引入头文件
//    m_chat_dialog = new chat_dialog;
}

room_dialog::~room_dialog()
{
    delete ui;
}

void room_dialog::set_room_id(int room_id)
{
    ui->lb_room_id->setText(QString("房间号 : %1").arg(room_id));
}

void room_dialog::add_user(QWidget *user)
{
    m_user_layout->addWidget(user);
}

void room_dialog::remove_user(QWidget *user)
{
    user->hide();
    m_user_layout->removeWidget(user);
    delete user;
}

void room_dialog::room_clear()
{
    //设置ui
    ui->cb_audio->setChecked(false);
    ui->cb_capture->setChecked(false);
    ui->cb_desk->setChecked(false);
    //发信号
    Q_EMIT signal_audio_close();
    Q_EMIT signal_video_close();
    Q_EMIT signal_desk_close();
}

#include <QDebug>
void room_dialog::set_big_image(int user_id, QImage& img)
{
    //qDebug()<<"输出id"<<ui->wdg_show->m_id<<user_id;
    if(ui->wdg_show->m_id == user_id){
        ui->wdg_show->slot_setImage(img);
        //qDebug()<<"刷新自己";
    }
}

void room_dialog::slot_set_big_image_info(int user_id, QString name)
{
    qDebug()<<"设置信息 name"<<name<<user_id;
    ui->wdg_show->slot_setInfo(user_id, name);
    //user_show* user = (user_show*)QObject::sender();//可以使用该方法获得发送信号的类对象的数据，不需要传参
}

void room_dialog::on_pb_min_clicked()
{
    this->slot_showMin();
}


void room_dialog::on_pb_max_clicked()
{
    this->slot_showMax();
}

#include <QMessageBox>
void room_dialog::on_pb_exit_2_clicked()
{
    if(QMessageBox::question(this, "提示", "是否退出当前会议") == QMessageBox::Yes){
        Q_EMIT signal_quit_meeting();
        this->close();
    }
}


void room_dialog::on_pb_hide_clicked()
{
    if(ui->sa_user_list->isHidden()){
        ui->sa_user_list->show();
    }else{
        ui->sa_user_list->hide();
    }
}


void room_dialog::on_pb_exit_meeting_clicked()
{
    if(QMessageBox::question(this, "提示", "是否退出当前会议") == QMessageBox::Yes){
        Q_EMIT signal_quit_meeting();
        this->close();
    }
}


void room_dialog::on_cb_capture_clicked()
{
    if(ui->cb_capture->isChecked()){
        //开启摄像头 (摄像头和桌面公用一个，屏幕，开启前禁用另一个)
        Q_EMIT signal_desk_close();
        Q_EMIT signal_video_open();
        //ui->cb_capture->setCheckable(false);

    }else{
        //ui->cb_capture->setCheckable(true);
        //关闭摄像头
        Q_EMIT signal_video_close();
    }
}


void room_dialog::on_cb_audio_clicked()
{
    if(ui->cb_audio->isChecked()){
        //ui->cb_audio->setCheckable(false);
        //开启麦克风
        Q_EMIT signal_audio_open();
    }else{
        //ui->cb_audio->setCheckable(true);
        //关闭麦克风
        Q_EMIT signal_audio_close();
    }
}


void room_dialog::on_cb_desk_clicked()
{
    if(ui->cb_desk->isChecked()){
        //ui->cb_audio->setCheckable(false);
        //开启桌面共享
        Q_EMIT signal_video_close();
        Q_EMIT signal_desk_open();
    }else{
        //ui->cb_audio->setCheckable(true);
        //关闭桌面共享
        Q_EMIT signal_desk_close();
    }
}


void room_dialog::on_cbx_lvjing_currentIndexChanged(int index)
{
    if(index) Q_EMIT signal_pic_id(index);
}


void room_dialog::on_pushButton_clicked()
{
    Q_EMIT signal_open_chat();
}

