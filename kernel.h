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
//单例
class Kernel : public QObject
{
    Q_OBJECT
public:
    explicit Kernel(QObject *parent = nullptr);
    //-------------工具类函数
    //设置协议映射
    void set_net_pack_map();

    //获取MD5
    static std::string GetMD5(QString password);

    //------------处理接收数据
    void deal_login_rs(char *buf, int nlen);

    void deal_register_rs(char *buf, int nlen);

    void deal_join_rs(char *buf, int nlen);

    void deal_create_rs(char* buf, int nlen);
//private:
    ~Kernel();



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
public:

    //初始化配置文件
    void config_init();

    bool SendData(unsigned int lSendIP , char* buf , int nlen);

private:
    //static Kernel mp_kernel;
    login_dialog* mp_login_dialog;
    main_dialog* mp_main_dialog;
    INetMediator* mp_client;
    room_dialog* mp_room;
    //个人信息
    int user_id;
    int room_id;
    QString m_server_ip;
    //---协议包映射处理函数---
    typedef void (Kernel::*P_FUN)(char*, int);
    std::map<int, P_FUN> net_pack_map;

};
//Kernel* Kernel::mp_kernel = nullptr;

#endif // KERNEL_H
