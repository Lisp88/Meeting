#ifndef KERNEL_H
#define KERNEL_H

#include <QObject>
#include "login_dialog.h"
#include "TcpClientMediator.h"
#include "main_dialog.h"
#include "md5.h"
#include "packdef.h"
#include <QDebug>
#include <QMessageBox>
#include "room_dialog.h"
#include "set_user.h"
#include "set"
#include "audio_read.h"
#include "audio_write.h"
#include "video_read.h"
#include "desk_read.h"
#include "QTime"
#include "ui_user_show.h"
#include "desk_read.h"
#include "work_thread.h"
#include "chat_dialog.h"

//为优化主线程send为槽函数，发送数据量大造成的 主线程卡顿 ，使用工作者线程来进行图片数据的发送
class SendThread_Video : public work_thread{
    Q_OBJECT
public:
public slots:
    void slot_thread_send_data(uint sockfd, char* buff, int nlen);
};
class SendThread_Audio : public work_thread{
    Q_OBJECT
public:
public slots:
    void slot_thread_send_data(uint sockfd, char* buff, int nlen);
};


//单例
class Kernel : public QObject
{
    Q_OBJECT
public:
//private:
    explicit Kernel(QObject *parent = nullptr);
    ~Kernel();
    Kernel(const Kernel& k);
    Kernel(const Kernel&& k);
public:
    //单例获取
    static Kernel* get_instance(){
        if(!mp_kernel)
            mp_kernel = new Kernel;
        return mp_kernel;
    }

    //-------------工具类函数
    //设置协议映射
    void set_net_pack_map();

    //刷新某人画面
    void refresh_user_image(int id, QImage& img);

    //获取MD5
    static std::string GetMD5(QString password);

    //设置加入的房间
    void set_join_room(int room_id);

    //------------处理接收数据---自定义协议转发
    void deal_login_rs(char *buf, int nlen);

    void deal_register_rs(char *buf, int nlen);

    void deal_join_rs(char *buf, int nlen);

    void deal_create_rs(char* buf, int nlen);

    void deal_member_rq(char* buf, int nlen);

    void deal_update_info(char* buf, int nlen);

    void deal_other_quit(char* buf, int nlen);

    void deal_audio_data(char* buf, int nlen);

    void deal_video_data(char* buf, int nlen);

    void deal_chat_rs(char* buf, int nlen);

    void deal_chat_rq(char* buf, int nlen);
signals:
    void signal_send_video(unsigned int lSendIP , char* buf , int nlen);
    void signal_send_audio(unsigned int lSendIP , char* buf , int nlen);
public slots:
    //------------ui槽函数-----------
    slot_data_deal(unsigned int lSendIP , char* buf , int nlen);
    //销毁
    slot_destory();
    //注册
    void slot_register(QString name, QString tel, QString password);
    //登录
    void slot_login(QString tel, QString password);
    //加入房间
    void slot_join_room();
    //创建房间
    void slot_create_room();
    //设置信息
    void slot_set_info();
    //提交信息
    void slot_commit_info(int icon_id, QString name, QString feeling);
    //退出房间
    void slot_quit_meeting();
    //打开音频
    void slot_audio_open();
    //打开视频
    void slot_video_open();
    //关闭视频
    void slot_video_close();
    //关闭音频
    void slot_audio_close();
    //关闭桌面采集
    void slot_desk_close();
    //打开桌面采集
    void slot_desk_open();
    //发送音频
    void slot_send_audio(QByteArray& data);
    //发送视频
    void slot_send_video(QByteArray data);
    //刷新采集
    void slot_refresh_vedio(QImage img);
    //设置萌拍
    void slot_pic_set(int id);
    //开启聊天页面
    void slot_open_chat();
    //发送聊天信息
    void slot_send_chat(QString text);
public:

    //初始化配置文件
    void config_init();
    //客户端发送数据
    bool SendData(unsigned int lSendIP , char* buf , int nlen);
    //音频数据发送
    bool Audio_SendData(unsigned int lSendIP , char* buf , int nlen);
    //视频数据发送
    bool Video_SendData(unsigned int lSendIP , char* buf , int nlen);
    //
    int get_icon(){return m_icon;}
private:
    //单例
    static Kernel* mp_kernel;
    //窗体类
    login_dialog*       mp_login_dialog;
    main_dialog*        mp_main_dialog;
    INetMediator*       mp_client;
    room_dialog*        mp_room;
    set_user*           mp_set_user;
    chat_dialog*        mp_chat_dialog;
    //个人信息
    int m_user_id;
    int m_room_id;
    int m_icon;
    QString m_name;
    QString m_server_ip;
    //---协议包映射处理函数---
    typedef void (Kernel::*P_FUN)(char*, int);
    std::map<int, P_FUN> net_pack_map;
    std::map<int, user_show*> user_list_map;
    //音视频部分
    Audio_Read* mp_audio_read;
    Video_Read* mp_video_read;
    Desk_Read* mp_desk_read;
    //音频播放需要多个对象和用户映射
    std::map<int, Audio_Write*> m_user_TO_audio_map;
    QSharedPointer<SendThread_Video> mp_send_video;
    QSharedPointer<SendThread_Audio> mp_send_audio;
    //SendThread* mp_send_videom;
    //解决音视频卡顿，分开创建两个TCP通道
    INetMediator* m_tcpAV[2];//0音频 1视频
};


#endif // KERNEL_H
