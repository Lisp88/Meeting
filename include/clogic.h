#ifndef CLOGIC_H
#define CLOGIC_H

#include"TCPKernel.h"
#include <random>
class CLogic
{
public:
    CLogic( TcpKernel* pkernel )
    {
        m_pKernel = pkernel;
        m_sql = pkernel->m_sql;
        m_tcp = pkernel->m_tcp;
    }
public:
    /************** 工具函数*********************/
    void get_user_info(int id);
    //设置协议映射
    void setNetPackMap();
    /************** 发送数据*********************/
    void SendData( sock_fd clientfd, char*szbuf, int nlen )
    {
        m_pKernel->SendData( clientfd ,szbuf , nlen );
    }
    /************** 网络处理 *********************/
    //注册
    void RegisterRq(sock_fd clientfd, char*szbuf, int nlen);
    //登录
    void LoginRq(sock_fd clientfd, char*szbuf, int nlen);
    //创建房间
    void create_room(sock_fd clientfd, char*szbuf, int nlen);
    //加入房间
    void join_room(sock_fd clientfd, char*szbuf, int nlen);
    //更改信息
    void update_info(sock_fd clientfd, char*szbuf, int nlen);
    //接收退出包
    void quit_room(sock_fd clientfd, char*szbuf, int nlen);
    //接收音频
    void audio_frame(sock_fd clientfd, char*szbuf, int nlen);
    //接收视频
    void video_frame(sock_fd clientfd, char*szbuf, int nlen);
    //音频注册
    void audio_reg(sock_fd clientfd, char* szbuf, int nlen);
    //视频注册
    void video_reg(sock_fd clientfd, char* szbuf, int nlen);
    //用户离线
    void user_offline(sock_fd clientfd, char* szbuf, int nlen);
    /*******************************************/

private:
    TcpKernel* m_pKernel;
    CMysql * m_sql;
    Block_Epoll_Net * m_tcp;

    MyMap<int, UserInfo*> m_map_id_to_userinfo;
    MyMap<int, list<int>> m_map_room_to_userlist;
};

#endif // CLOGIC_H
