#include "kernel.h"
#define NET_PACK_MAP(n)         net_pack_map[n-DEF_PACK_BASE]

Kernel::Kernel(QObject *parent) : QObject(parent), user_id(0), room_id(0)
{
    mp_login_dialog = new login_dialog;
    mp_main_dialog = new main_dialog;
    mp_client = new TcpClientMediator;
    mp_room = new room_dialog;
    //加载配置文件
    config_init();
    //绑定数据传输
    connect(mp_client, SIGNAL(SIG_ReadyData(uint,char*,int)), this, SLOT(slot_data_deal(uint,char*,int)));
    //绑定页面关闭
    connect(mp_login_dialog, SIGNAL(signal_close()), this, SLOT(slot_destory()));
    connect(mp_main_dialog, SIGNAL(signal_close()), this, SLOT(slot_destory()));
    //显示登录页面
    mp_login_dialog->show();
    //绑定 登录注册信号槽
    connect(mp_login_dialog, SIGNAL(signal_login(QString,QString)), this, SLOT(slot_login(QString,QString)));
    connect(mp_login_dialog, SIGNAL(signal_register(QString,QString,QString)), this, SLOT(slot_register(QString,QString,QString)));
    //绑定 房间系统
    connect(mp_main_dialog, SIGNAL(signal_join_meeting()), this, SLOT(slot_join_room()));
    connect(mp_main_dialog, SIGNAL(signal_create_meeting()), this, SLOT(slot_create_room()));
    //打开网络
    set_net_pack_map();
    mp_client->OpenNet(m_server_ip.toStdString().c_str());

}

void Kernel::set_net_pack_map() {
    //NET_PACK_MAP(_DEF_TCP_LOGIN_RQ) = &Kernel::deal_login_data;
    //处理注册和登录服务器回复
    NET_PACK_MAP(DEF_PACK_REGISTER_RS) = &Kernel::deal_register_rs;
    NET_PACK_MAP(DEF_PACK_LOGIN_RS) = &Kernel::deal_login_rs;
    //处理加入和创建房间服务器回复
    NET_PACK_MAP(DEF_PACK_JOINROOM_RS) = &Kernel::deal_join_rs;
    NET_PACK_MAP(DEF_PACK_CREATEROOM_RS) = &Kernel::deal_create_rs;
}
#define _MD5_KEY  "6666"

std::string Kernel::GetMD5(QString password)
{
    std::string str = QString("%1_%2").arg(password).arg(_MD5_KEY).toStdString();
    //md5自定义拼凑
    MD5 md5(str);
    return md5.toString();
}

Kernel::~Kernel()
{
    slot_destory();
}

bool Kernel::SendData(unsigned int lSendIP, char *buf, int nlen)
{
    return mp_client->SendData(lSendIP, buf, nlen);
}

Kernel::slot_data_deal(unsigned int lSendIP, char *buf, int nlen)
{
    int type = *(int* )buf;
    qDebug()<<"recv data type: "<<type;
    P_FUN p_fun = NET_PACK_MAP(type);
    if(p_fun)
        (this->*p_fun)(buf, nlen);
}

Kernel::slot_destory()
{
    mp_client->CloseNet();
    delete mp_client;
    mp_client = nullptr;

    delete mp_login_dialog ;
    mp_login_dialog = nullptr;

    delete mp_main_dialog;
    mp_main_dialog = nullptr;

    delete mp_room;
    mp_room = nullptr;
}



#include <QSettings>
#include <QApplication>
#include <QFileInfo>
#include <QDebug>
//初始化设置
void Kernel::config_init()
{
    m_server_ip = _DEF_SERVER_IP;
    //exe同级目录
    QString path = QApplication::applicationDirPath() + "/config.ini";
    QFileInfo info(path);
    //有则打开，没有创建
    QSettings settings(path, QSettings::IniFormat, nullptr);
    if(info.exists()){
        //加载配置文件 ip 设置为配置文件
        settings.beginGroup("Net");
        QVariant ip = settings.value("ip");
        QString qstr_ip = ip.toString();
        settings.endGroup();
        if(!qstr_ip.isEmpty()){
            m_server_ip = qstr_ip;
        }

    }else{
        //没有配置文件则创建
        settings.beginGroup("Net");
        settings.setValue("ip", m_server_ip);
        settings.endGroup();
    }

    qDebug()<<"read config file ip"<<m_server_ip;
}
//-------------------------处理接收的数据------------------------
//处理登录回复包
void Kernel::deal_login_rs(char *buf, int nlen){
    qDebug()<<"deal_login_rs >> deal login rs package";
    STRU_LOGIN_RS* login_rs = (STRU_LOGIN_RS*)buf;

    switch (login_rs->result) {
    case login_success:
        //QMessageBox::about(p_main_windows, "登陆提示", "登陆成功");
        user_id = login_rs->userid;
        //mp_windows_dialog->p_self_info->userid = user_id;
        //界面切换，使用模态对话框，点完之后切换页面

        mp_login_dialog->hide();
        mp_main_dialog->show();
        break;
    case user_not_exist:
        QMessageBox::about(mp_login_dialog, "登陆提示", "该用户不存在");
        break;
    case password_error:
        QMessageBox::about(mp_login_dialog, "登陆提示", "密码错误");
    default:
        break;
    }
}

//处理注册回复包
void Kernel::deal_register_rs(char *buf, int nlen)
{
    qDebug()<<"deal_register_rs >> deal register rs package";
    //拆包
    STRU_REGISTER_RS *register_rs = (STRU_REGISTER_RS*)buf;

    switch (register_rs->result) {
    case tel_is_exist:
        QMessageBox::about(mp_login_dialog, "注册提示", "该用户已存在，请重新输入其他手机号");
        break;
    case name_is_exist:
        QMessageBox::about(mp_login_dialog, "注册提示", "该用户名已存在，请重新输入其他用户名");
        break;
    case register_success:
        QMessageBox::about(mp_login_dialog, "注册提示", "注册成功");
        break;
    default:
        break;
    }
}

void Kernel::deal_join_rs(char *buf, int nlen)
{
    STRU_JOINROOM_RS* rs = (STRU_JOINROOM_RS*)buf;
    if(rs->m_lResult != join_success){
        QMessageBox::about(this->mp_main_dialog, "提示", "房间不存在，加入失败");
        return;
    }
    room_id = rs->m_RoomID;
    mp_main_dialog->hide();
    mp_room->showNormal();

}

void Kernel::deal_create_rs(char *buf, int nlen)
{
    STRU_CREATEROOM_RS* rs = (STRU_CREATEROOM_RS*)buf;
    rs->m_RoomId = room_id;

    mp_main_dialog->hide();
    mp_room->showNormal();

    //初始化状态 清空操作
}


//-------------------------------注册和登录槽函数----------------------------------------------
void Kernel::slot_register(QString name, QString tel, QString password)
{
    std::string std_tel = tel.toStdString();
    std::string std_name = name.toStdString();
    //MD5生成密文
    std::string std_password = GetMD5(password);

    STRU_REGISTER_RQ register_request;
    strcpy(register_request.name, std_name.c_str());
    strcpy(register_request.tel, std_tel.c_str());
    strcpy(register_request.password, std_password.c_str());



    SendData(0, (char*)&register_request, sizeof(register_request));

    qDebug()<<"slot_register >> send register info to server";
}

//登录
void Kernel::slot_login(QString tel, QString password)
{
    std::string std_tel = tel.toStdString();
    std::string std_password = GetMD5(password);

    STRU_LOGIN_RQ login_request;
    strcpy(login_request.tel, std_tel.c_str());
    strcpy(login_request.password, std_password.c_str());
    //
    SendData(0, (char*)&login_request, sizeof(login_request));
    qDebug()<<"slot_login >> send login info to server";
}

#include <QInputDialog>
#define room_max_num 999999
//加入房间
void Kernel::slot_join_room()
{
    if(room_id != 0){
        QMessageBox::about(this->mp_main_dialog, "提示", "已在房间, 加入失败");
        return;
    }
    qDebug()<<"join room";
    int roomid = QInputDialog::getInt(this->mp_main_dialog, "加入房间", "房间号:");
    if(roomid < 0 || roomid > room_max_num){
        QMessageBox::about(this->mp_main_dialog, "提示", "房间号不存在");
        return;
    }

    //发送请求
    STRU_JOINROOM_RQ rq;
    rq.m_UserID = user_id;
    rq.m_RoomID = room_id;
    mp_client->SendData(0, (char*)&rq, sizeof(rq));
}
//创建房间
void Kernel::slot_create_room()
{
    STRU_CREATEROOM_RQ rq;
    rq.m_UserID = user_id;

    mp_client->SendData(0, (char*)&rq, sizeof(rq));
}
