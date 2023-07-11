#include "kernel.h"
#define NET_PACK_MAP(n)         net_pack_map[n-DEF_PACK_BASE]

Kernel::Kernel(QObject *parent) : QObject(parent), m_user_id(0), m_room_id(0), mp_send_video(new SendThread_Video), mp_send_audio(new SendThread_Audio)
{
    mp_login_dialog = new login_dialog;
    mp_main_dialog = new main_dialog;
    mp_client = new TcpClientMediator;
    mp_room = new room_dialog;
    mp_set_user = new set_user;
    mp_audio_read = new Audio_Read;
    mp_video_read = new Video_Read;
    mp_desk_read = new Desk_Read;
    mp_chat_dialog = new chat_dialog;
    m_tcpAV[0] = new TcpClientMediator;
    m_tcpAV[1] = new TcpClientMediator;
    //加载配置文件
    config_init();
    //绑定数据传输
    connect(mp_client, SIGNAL(SIG_ReadyData(uint,char*,int)), this, SLOT(slot_data_deal(uint,char*,int)));
    connect(m_tcpAV[0], SIGNAL(SIG_ReadyData(uint,char*,int)), this, SLOT(slot_data_deal(uint,char*,int)));
    connect(m_tcpAV[1], SIGNAL(SIG_ReadyData(uint,char*,int)), this, SLOT(slot_data_deal(uint,char*,int)));

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
    //绑定头像设置个人信息
    connect(mp_main_dialog, SIGNAL(signal_set_info()), this, SLOT(slot_set_info()));
    //绑定提交个人信息
    connect(mp_set_user, SIGNAL(signal_commit_info(int,QString,QString)), this, SLOT(slot_commit_info(int,QString,QString)));
    //绑定退出房间
    connect(mp_room, SIGNAL(signal_quit_meeting()), this, SLOT(slot_quit_meeting()));
    //音频读取发送
    connect(mp_audio_read, SIGNAL(signal_audio_frame(QByteArray&)), this, SLOT(slot_send_audio(QByteArray&)));
    //本机视频显示
    connect(mp_video_read, SIGNAL(SIG_videoFrame(QImage)), this, SLOT(slot_refresh_vedio(QImage)));
    //视频读取发送
    connect(mp_video_read, SIGNAL(SIG_videoFrameData(QByteArray)), this, SLOT(slot_send_video(QByteArray)));
    //本机桌面显示
    connect(mp_desk_read, SIGNAL(signal_screen(QImage)), this, SLOT(slot_refresh_vedio(QImage)));
    //桌面数据发送
    connect(mp_desk_read, SIGNAL(signal_screen_data(QByteArray)), this, SLOT(slot_send_video(QByteArray)));
    //音频打开
    connect(mp_room, SIGNAL(signal_audio_open()), this, SLOT(slot_audio_open()));
    //音频关闭
    connect(mp_room, SIGNAL(signal_audio_close()), this, SLOT(slot_audio_close()));
    //摄像头打开
    connect(mp_room, SIGNAL(signal_video_open()), this, SLOT(slot_video_open()));
    //摄像头关闭
    connect(mp_room, SIGNAL(signal_video_close()), this, SLOT(slot_video_close()));
    //桌面采集
    connect(mp_room, SIGNAL(signal_desk_open()), mp_desk_read, SLOT(start()));
    //采集关闭
    connect(mp_room, SIGNAL(signal_desk_close()), mp_desk_read, SLOT(pause()));
    //萌拍id
    connect(mp_room, SIGNAL(signal_pic_id(int)), this, SLOT(slot_pic_set(int)));
    //开启聊天页面
    connect(mp_room, SIGNAL(signal_open_chat()), this, SLOT(slot_open_chat()));
    //聊天发送信息
    connect(mp_chat_dialog, SIGNAL(signal_send_info(QString)), this, SLOT(slot_send_chat(QString)));
    //工作线程发送视频数据
    connect(this, SIGNAL(signal_send_video(uint,char*,int)), mp_send_video.get(), SLOT(slot_thread_send_data(uint,char*,int)));
    connect(this, SIGNAL(signal_send_audio(uint,char*,int)), mp_send_audio.get(), SLOT(slot_thread_send_data(uint,char*,int)));
    //打开网络
    set_net_pack_map();
    mp_client->OpenNet(m_server_ip.toStdString().c_str());
    m_tcpAV[0]->OpenNet(m_server_ip.toStdString().c_str());
    m_tcpAV[1]->OpenNet(m_server_ip.toStdString().c_str());
}
void Kernel::set_net_pack_map() {
    //NET_PACK_MAP(_DEF_TCP_LOGIN_RQ) = &Kernel::deal_login_data;
    //处理注册和登录服务器回复
    NET_PACK_MAP(DEF_PACK_REGISTER_RS) = &Kernel::deal_register_rs;
    NET_PACK_MAP(DEF_PACK_LOGIN_RS) = &Kernel::deal_login_rs;
    //处理加入和创建房间服务器回复
    NET_PACK_MAP(DEF_PACK_JOINROOM_RS) = &Kernel::deal_join_rs;
    NET_PACK_MAP(DEF_PACK_CREATEROOM_RS) = &Kernel::deal_create_rs;
    //房间成员请求
    NET_PACK_MAP(DEF_PACK_ROOM_MEMBER) = &Kernel::deal_member_rq;
    //处理更新头像
    NET_PACK_MAP(DEF_PACK_USER_INFO) = &Kernel::deal_update_info;
    //处理 别人离开房间
    NET_PACK_MAP(DEF_PACK_LEAVEROOM_RQ) = &Kernel::deal_other_quit;
    //处理接收音频视频
    NET_PACK_MAP(DEF_PACK_AUDIO_FRAME) = &Kernel::deal_audio_data;
    NET_PACK_MAP(DEF_PACK_VIDEO_FRAME) = &Kernel::deal_video_data;
    //聊天
    NET_PACK_MAP(DEF_PACK_CHAT_RS) = &Kernel::deal_chat_rs;
    NET_PACK_MAP(DEF_PACK_CHAT_RQ) = &Kernel::deal_chat_rq;
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

bool Kernel::Audio_SendData(unsigned int lSendIP, char *buf, int nlen)
{
    m_tcpAV[0]->SendData(lSendIP, buf, nlen);
}

bool Kernel::Video_SendData(unsigned int lSendIP, char *buf, int nlen)
{
    m_tcpAV[1]->SendData(lSendIP, buf, nlen);
}

Kernel::slot_data_deal(unsigned int lSendIP, char *buf, int nlen)
{
    int type = *(int* )buf;
    P_FUN p_fun = NET_PACK_MAP(type);
    if(p_fun)
        (this->*p_fun)(buf, nlen);
}

Kernel::slot_destory()
{
    STRU_OFFLINE rq;
    rq.m_userid = m_user_id;

    mp_client->SendData(0, (char*)&rq, sizeof(rq));

    mp_client->CloseNet();

    if(mp_client)
    delete mp_client;
    mp_client = nullptr;
    if(mp_login_dialog)
    delete mp_login_dialog ;
    mp_login_dialog = nullptr;
    if(mp_main_dialog)
    delete mp_main_dialog;
    mp_main_dialog = nullptr;
    if(mp_room)
    delete mp_room;
    mp_room = nullptr;
    if(mp_set_user)
    delete mp_set_user;
    mp_set_user = nullptr;
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
//--------------------------------------------------处理接收的数据------------------------
//处理登录回复包
void Kernel::deal_login_rs(char *buf, int nlen){
    qDebug()<<"deal_login_rs >> deal login rs package";
    STRU_LOGIN_RS* login_rs = (STRU_LOGIN_RS*)buf;
    STRU_AUDIO_REGISTER audiorq;
    STRU_VIDEO_REGISTER videorq;
    switch (login_rs->result) {
    case login_success:
        //QMessageBox::about(p_main_windows, "登陆提示", "登陆成功");
        m_user_id = login_rs->userid;

        mp_login_dialog->hide();
        mp_main_dialog->show();
        //注册音频视频

        audiorq.m_userid = m_user_id;
        Audio_SendData(0, (char*)&audiorq, sizeof(audiorq));

        videorq.m_userid = m_user_id;
        Video_SendData(0, (char*)&videorq, sizeof(videorq));

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
        m_name.clear();
        break;
    case name_is_exist:
        QMessageBox::about(mp_login_dialog, "注册提示", "该用户名已存在，请重新输入其他用户名");
        m_name.clear();
        break;
    case register_success:
        QMessageBox::about(mp_login_dialog, "注册提示", "注册成功");
        break;
    default:
        break;
    }
}

//设置加入房间
void Kernel::set_join_room(int room_id)
{
    m_room_id = room_id;
    //mp_main_dialog->hide(); 按键变灰替换隐藏窗口
    mp_main_dialog->push_button_enable(false);
    mp_room->showNormal();

    mp_room->set_room_id(m_room_id);
}

void Kernel::deal_join_rs(char *buf, int nlen)
{
    qDebug()<<"deal_join_rs >> deal join room package";
    STRU_JOINROOM_RS* rs = (STRU_JOINROOM_RS*)buf;
    if(rs->m_lResult != join_success){
        QMessageBox::about(this->mp_main_dialog, "提示", "房间不存在，加入失败");
        return;
    }
    set_join_room(rs->m_RoomID);
    if(!user_list_map.count(m_user_id)){
        //将自己加入进房间
        user_show* user = new user_show;
        connect(user, SIGNAL(signal_user_clicked(int,QString)), mp_room, SLOT(slot_set_big_image_info(int,QString)));
        user->m_icon = m_icon;
        user->slot_setInfo(m_user_id, "自己");
        mp_room->add_user(user);
        user_list_map[m_user_id] = user;
    }
}

void Kernel::deal_create_rs(char *buf, int nlen)
{
    qDebug()<<"deal_create_rs >> deal create room package";
    STRU_CREATEROOM_RS* rs = (STRU_CREATEROOM_RS*)buf;
    set_join_room(rs->m_RoomId);

    //初始化状态 清空操作
    //将自己加入进房间
    if(!user_list_map.count(m_user_id)){
        user_show* user = new user_show;
        connect(user, SIGNAL(signal_user_clicked(int,QString)), mp_room, SLOT(slot_set_big_image_info(int,QString)));
        user->m_icon = m_icon;
        user->slot_setInfo(m_user_id, "自己");
        mp_room->add_user(user);
        user_list_map[m_user_id] = user;
    }

}

void Kernel::deal_member_rq(char *buf, int nlen)
{
    qDebug()<<"deal_member_rq >> deal recv room member info package";
    //拆包
    STRU_ROOM_MEMBER_RQ* rq = (STRU_ROOM_MEMBER_RQ*)buf;
    //加入进房间
    user_show* user = new user_show;
    connect(user, SIGNAL(signal_user_clicked(int,QString)), mp_room, SLOT(slot_set_big_image_info(int,QString)));
    user->m_icon = rq->m_icon;
    user->slot_setInfo(rq->m_UserID, QString::fromStdString(rq->m_szUser));
    //已经在房间中，先把房间中的人剔除，在添加
    if(user_list_map.count(rq->m_UserID)){
        user_show* have_been = user_list_map[rq->m_UserID];
        user_list_map.erase(rq->m_UserID);
        mp_room->remove_user(have_been);
        delete have_been;
    }
    mp_room->add_user(user);
    user_list_map[rq->m_UserID] = user;
    //为新成员创建音频对象
    if(m_user_TO_audio_map.count(rq->m_UserID)){
        Audio_Write* have_been = m_user_TO_audio_map[rq->m_UserID];
        m_user_TO_audio_map.erase(rq->m_UserID);
        delete have_been;
    }
    Audio_Write* audio_w = new Audio_Write;
    m_user_TO_audio_map[rq->m_UserID] = audio_w;
}

void Kernel::deal_update_info(char *buf, int nlen)
{
    qDebug()<<"deal_update_info >> deal update user info package";
    //拆包
    STRU_USER_INFO_RQ* rq = (STRU_USER_INFO_RQ*)buf;
    //qDebug()<<"user new name : "<<rq->name;
    //qDebug()<<"user new feeling :"<<rq->feeling;
    //更新ui
    m_icon = rq->iconid;
    QString name = QString::fromStdString(rq->name);
    m_name = name;
    mp_main_dialog->setinfo(rq->iconid, name);
    //更新设置ui
    mp_set_user->set_info(rq->iconid, name, QString::fromStdString(rq->feeling));
}

//别人离开房间
void Kernel::deal_other_quit(char *buf, int nlen)
{
    qDebug()<<"deal other quit >> other quit room";
    //拆包
    STRU_LEAVEROOM_RQ* rq = (STRU_LEAVEROOM_RQ*)buf;
    //删除离开的人
    if(user_list_map.empty()) return;

    user_show* user = user_list_map[rq->m_nUserId];
    mp_room->remove_user(user);
    user_list_map.erase(rq->m_nUserId);
    m_user_TO_audio_map.erase(user->m_id);
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
    m_name = name;


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
    if(m_room_id != 0){
        QMessageBox::about(this->mp_main_dialog, "提示", "已在房间, 加入失败");
        return;
    }
    qDebug()<<"join room";
    int roomid = QInputDialog::getInt(this->mp_main_dialog, "加入房间", "房间号:");
    m_room_id = roomid;
    if(roomid < 0 || roomid > room_max_num){
        QMessageBox::about(this->mp_main_dialog, "提示", "房间号不存在");
        return;
    }

    //发送请求
    STRU_JOINROOM_RQ rq;
    rq.m_UserID = m_user_id;
    rq.m_RoomID = m_room_id;
    mp_client->SendData(0, (char*)&rq, sizeof(rq));
}

//创建房间
void Kernel::slot_create_room()
{
    STRU_CREATEROOM_RQ rq;
    rq.m_UserID = m_user_id;

    mp_client->SendData(0, (char*)&rq, sizeof(rq));
}

//弹出set框
void Kernel::slot_set_info()
{
    mp_set_user->show();
}

//提交修改的个人信息
void Kernel::slot_commit_info(int icon_id, QString name, QString feeling)
{
    //发包
    std::string str_name = name.toStdString();
    std::string str_feeling = feeling.toStdString();
    STRU_USER_INFO_RQ rq;
    rq.iconid = icon_id;
    rq.userid = m_user_id;
    strcpy(rq.name, str_name.c_str());
    strcpy(rq.feeling, str_feeling.c_str());
    m_name = name;
    SendData(0, (char*)&rq, sizeof(rq));
}

//自己退出房间
void Kernel::slot_quit_meeting()
{
    //发包
    STRU_LEAVEROOM_RQ rq;
    rq.m_nUserId = m_user_id;
    rq.m_RoomId = m_room_id;
    m_room_id = 0;
    SendData(0, (char*)&rq, sizeof(rq));
    //按钮恢复
    mp_main_dialog->push_button_enable(true);

    //关闭音视频
    mp_room->room_clear();

    //回收房间成员界面资源
    for(auto ite = user_list_map.begin(); ite != user_list_map.end(); ){
        user_show* user = ite->second;
        ite = user_list_map.erase(ite);
        mp_room->remove_user(user);
    }
    //回收音频
    for(auto ite = m_user_TO_audio_map.begin(); ite != m_user_TO_audio_map.end();){
        Audio_Write* user = ite->second;
        ite = m_user_TO_audio_map.erase(ite);
        delete user;
    }
    mp_chat_dialog->close_chat();
}

void Kernel::slot_audio_open()
{
    mp_audio_read->start();
}

void Kernel::slot_video_open()
{
    mp_video_read->start();
}

void Kernel::slot_video_close()
{
    mp_video_read->pause();
}

void Kernel::slot_audio_close()
{
    mp_audio_read->pause();
}

void Kernel::slot_desk_close()
{

}

void Kernel::slot_desk_open()
{

}



//刷新画面，用于采集
void Kernel::slot_refresh_vedio(QImage img){
    refresh_user_image(m_user_id, img);
    //qDebug()<<"refresh vedio";
}

void Kernel::slot_pic_set(int id)
{
    mp_video_read->set_pic_id(id);
}

void Kernel::slot_open_chat()
{
    mp_chat_dialog->show();
}

//发送聊天信息
void Kernel::slot_send_chat(QString text)
{
    qDebug() << "封包，发送";
    mp_chat_dialog->add_info(m_icon, m_name, text);
    STRU_CHAT_RQ rq;
    rq.userid = m_user_id;
    rq.roomid = m_room_id;
    rq.iconid = m_icon;
    strcpy_s(rq.name, m_name.toStdString().c_str());
    strcpy_s(rq.content, text.toStdString().c_str());
    SendData(0, (char*)&rq, sizeof(rq));
}

#include <QDebug>
//显示某个用户
void Kernel::refresh_user_image(int id, QImage& img)
{
    //预览图
    mp_room->set_big_image(id, img);
    //列表图
    if(user_list_map.count(id)){
        //qDebug()<<"刷新图片"<<m_user_id;
        user_show* user = user_list_map[id];
        user->slot_setImage(img);
    }
}


void Kernel::slot_send_audio(QByteArray& data)
{
    //确认协议内容 ， send发送缓冲区，底层为滑动窗口，对方读取慢导致发送阻塞，进而延时越来越大（解决方法，客户端判断延时及时丢弃数据）
    //TODO 服务器协议添加 时间

    int type = DEF_PACK_AUDIO_FRAME;
    int user_id = m_user_id;
    int room_id = m_room_id;
    QTime tm = QTime::currentTime();
    int hour = tm.hour();
    int min = tm.minute();
    int sec = tm.second();
    int msec = tm.msec();
    char* audio_data = data.data();
    int len = data.size();
    //序列化数据 对于可变音频大小，可在堆区申请空间后进行序列化
    char* buf = new char[sizeof(int)*7 + len];//int 变量 + 音频大小
    char* temp = buf;
    *(int*)temp = type;
    temp += sizeof(int);
    *(int*)temp = user_id;
    temp += sizeof(int);
    *(int*)temp = room_id;
    temp += sizeof(int);
    *(int*)temp = hour;
    temp += sizeof(int);
    *(int*)temp = min;
    temp += sizeof(int);
    *(int*)temp = sec;
    temp += sizeof(int);
    *(int*)temp = msec;
    temp += sizeof(int);
    memcpy(temp, audio_data, len);


    Audio_SendData(0, buf, sizeof(int)*7 + len);
    delete[] buf;
}


void Kernel::slot_send_video(QByteArray data)
{
    int type = DEF_PACK_VIDEO_FRAME;
    int user_id = m_user_id;
    int room_id = m_room_id;
    QTime tm = QTime::currentTime();
    int hour = tm.hour();
    int min = tm.minute();
    int sec = tm.second();
    int msec = tm.msec();
    char* audio_data = data.data();
    int len = data.size();
    //序列化数据 对于可变音频大小，可在堆区申请空间后进行序列化
    char* buf = new char[sizeof(int)*7 + len];//int 变量 + 音频大小
    char* temp = buf;
    *(int*)temp = type;
    temp += sizeof(int);
    *(int*)temp = user_id;
    temp += sizeof(int);
    *(int*)temp = room_id;
    temp += sizeof(int);
    *(int*)temp = hour;
    temp += sizeof(int);
    *(int*)temp = min;
    temp += sizeof(int);
    *(int*)temp = sec;
    temp += sizeof(int);
    *(int*)temp = msec;
    temp += sizeof(int);
    memcpy(temp, audio_data, len);
    temp += len;

//    SendData(0, buf, sizeof(int)*7 + len);
//    delete[] buf;
    //qDebug()<<"send signal";
    //替换为发送信号
    Q_EMIT signal_send_video(0, buf, sizeof(int)*7 + len);
}


void Kernel::deal_audio_data(char *buf, int nlen)
{
    //反序列化顺序
    int type;
    int user_id;
    int room_id;
    int hour;
    int mini;
    int sece;
    int msec;
    //反序列化
    char* temp = buf;
    type = *(int*)temp;
    temp += sizeof(int);
    user_id = *(int*)temp;
    temp += sizeof(int);
    room_id = *(int*)temp;
    temp += sizeof(int);
    hour = *(int*)temp;
    temp += sizeof(int);
    mini = *(int*)temp;
    temp += sizeof(int);
    sece = *(int*)temp;
    temp += sizeof(int);
    msec = *(int*)temp;
    temp += sizeof(int);
    //音频数据
    QByteArray ba(temp, nlen - (7*sizeof(int)));
    if(m_user_TO_audio_map.count(user_id)){
        Audio_Write* cur_write = m_user_TO_audio_map[user_id];
        cur_write->slot_audio_out(ba);
    }
}

void Kernel::deal_video_data(char *buf, int nlen)
{
    //反序列化顺序
    int type;
    int user_id;
    int room_id;
    int hour;
    int mini;
    int sece;
    int msec;
    //反序列化
    char* temp = buf;
    type = *(int*)temp;
    temp += sizeof(int);
    user_id = *(int*)temp;
    temp += sizeof(int);
    room_id = *(int*)temp;
    temp += sizeof(int);
    hour = *(int*)temp;
    temp += sizeof(int);
    mini = *(int*)temp;
    temp += sizeof(int);
    sece = *(int*)temp;
    temp += sizeof(int);
    msec = *(int*)temp;
    temp += sizeof(int);

    QImage img;
    img.loadFromData((const uchar*)temp, nlen - (7*sizeof(int)));
    refresh_user_image(user_id, img);
}
#include <QMessageBox>
void Kernel::deal_chat_rs(char *buf, int nlen)
{
    STRU_CHAT_RS* rs = (STRU_CHAT_RS*)buf;
    if(rs->result != 0){
        QMessageBox::about(mp_chat_dialog, "提示", "信息发送失败");
    }
}

void Kernel::deal_chat_rq(char *buf, int nlen)
{
    STRU_CHAT_RQ* rq = (STRU_CHAT_RQ*)buf;
    qDebug() << "deal chat rq ";
    if(rq->roomid == m_room_id){
        int icon_id = rq->iconid;
        QString text(rq->content);
        QString name(rq->name);
        mp_chat_dialog->add_info(icon_id, name, text);
    }
}
//问题
//1 当音频和视频同时开着时导致卡顿（视频数据过大）：原因:TCP滑动窗口阻塞，解决方法：分开进行TCP发送
//2 解决音画不同步方法：对于视频帧中时间戳进行判定<200ms则进行丢弃
#include <QTime>
void SendThread_Video::slot_thread_send_data(uint sockfd, char *buff, int nlen)
{
    char* temp = buff;
    temp += sizeof(int)*2;
    int hour = *(int*)temp;
    temp += 4;
    int minite = *(int*)temp;
    temp += 4;
    int sec = *(int*)temp;
    temp += 4;
    int msec = *(int*)temp;
    temp += 4;
    QTime time(hour, minite, sec, msec);
    if(time.msecsTo(QTime::currentTime()) <= 200)
        //Kernel::get_instance()->SendData(sockfd, buff, nlen);
        //使用新的独有的视频tcp缓冲区
        Kernel::get_instance()->Video_SendData(sockfd, buff, nlen);
    //qDebug()<<"thread send";
    delete buff;
}

Kernel* Kernel::mp_kernel = nullptr;

void SendThread_Audio::slot_thread_send_data(uint sockfd, char *buff, int nlen)
{
    char* temp = buff;
    temp += sizeof(int)*2;
    int hour = *(int*)temp;
    temp += 4;
    int minite = *(int*)temp;
    temp += 4;
    int sec = *(int*)temp;
    temp += 4;
    int msec = *(int*)temp;
    temp += 4;
    QTime time(hour, minite, sec, msec);
    if(time.msecsTo(QTime::currentTime()) <= 200)
        //Kernel::get_instance()->SendData(sockfd, buff, nlen);
        //使用新的独有的视频tcp缓冲区
        Kernel::get_instance()->Audio_SendData(sockfd, buff, nlen);
    //qDebug()<<"thread send";
    delete buff;
}
