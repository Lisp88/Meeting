#ifndef _PACKDEF_H
#define _PACKDEF_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <sys/epoll.h>
#include <pthread.h>
#include <signal.h>
#include <errno.h>
#include "err_str.h"
#include <malloc.h>

#include<iostream>
#include<map>
#include<list>


//边界值
#define _DEF_SIZE 45
#define _DEF_BUFFERSIZE 1000
#define _DEF_PORT 8000
#define _DEF_SERVERIP "0.0.0.0"  //TODO

/*-------------数据库信息-----------------*/
#define _DEF_DB_NAME    "meeting"
#define _DEF_DB_IP      "127.0.0.1"
#define _DEF_DB_USER    "root"
#define _DEF_DB_PWD     "88888888"
/*--------------------------------------*/

#define TRUE true
#define FALSE false


#define _DEF_LISTEN 128
#define _DEF_EPOLLSIZE 4096
#define _DEF_IPSIZE 16
#define _DEF_COUNT 10
#define _DEF_TIMEOUT 10
#define _DEF_SQLIEN 400



#define BOOL bool
#define DEF_PACK_BASE  (10000)
#define DEF_PACK_COUNT  (100)

//注册
#define  DEF_PACK_REGISTER_RQ    (DEF_PACK_BASE + 0)
#define  DEF_PACK_REGISTER_RS    (DEF_PACK_BASE + 1)
//登录
#define  DEF_PACK_LOGIN_RQ    (DEF_PACK_BASE + 2)
#define  DEF_PACK_LOGIN_RS    (DEF_PACK_BASE + 3)
//创建房间
#define DEF_PACK_CREATEROOM_RQ  (DEF_PACK_BASE + 4)
#define DEF_PACK_CREATEROOM_RS  (DEF_PACK_BASE + 5)
//加入房间
#define DEF_PACK_JOINROOM_RQ  (DEF_PACK_BASE + 6)
#define DEF_PACK_JOINROOM_RS  (DEF_PACK_BASE + 7)
//房间列表请求
#define DEF_PACK_ROOM_MEMBER    (DEF_PACK_BASE + 8)
//音频数据
#define DEF_PACK_AUDIO_FRAME    (DEF_PACK_BASE + 9)
//视频数据
#define DEF_PACK_VIDEO_FRAME    (DEF_PACK_BASE + 10)
//退出房间请求
#define DEF_PACK_LEAVEROOM_RQ   (DEF_PACK_BASE + 11)
//退出房间回复
//#define DEF_PACK_LEAVEROOM_RS   (DEF_PACK_BASE + 12)
//音频注册
#define DEF_PACK_AUDIO_REGISTER (DEF_PACK_BASE + 13)
//视频注册
#define DEF_PACK_VIDEO_REGISTER (DEF_PACK_BASE + 14)
//用户信息
#define DEF_PACK_USER_INFO       DEF_PACK_BASE + 15
//用户下线
#define DEF_PACK_USER_OFFLINE    DEF_PACK_BASE + 16
//注册请求结果
#define tel_is_exist		(0)
#define register_success	(1)
#define name_is_exist       (2)

//登录请求结果
#define user_not_exist		(0)
#define password_error		(1)
#define login_success		(2)
//#define user_online          3

//创建房间结果
#define room_is_exist        0
#define create_success       1

//加入房间结果
#define room_no_exist        0
#define join_success         1


#define MAX_PATH            (260 )
#define MAX_SIZE            (40  )
#define DEF_HOBBY_COUNT     (8  )
#define MAX_CONTENT_LEN     (4096 )


/////////////////////网络//////////////////////////////////////


#define DEF_MAX_BUF	  1024
#define DEF_BUFF_SIZE	  4096


typedef int PackType;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//登录请求
typedef struct STRU_LOGIN_RQ
{
    STRU_LOGIN_RQ()
    {
        m_nType = DEF_PACK_LOGIN_RQ;
        memset(m_tel,0,MAX_SIZE);
        memset(m_password,0,MAX_SIZE);
    }

    PackType m_nType;   //包类型
    char     m_tel[MAX_SIZE] ; //用户ID
    char     m_password[MAX_SIZE];  //密码
} STRU_LOGIN_RQ;


//登录回复
typedef struct STRU_LOGIN_RS
{
    STRU_LOGIN_RS()
    {
        m_nType= DEF_PACK_LOGIN_RS;
        m_userid = 0;
        m_lResult = 0;
        memset( m_name , 0 , MAX_SIZE );
    }
    PackType m_nType;   //包类型
    int  m_userid;
    int  m_lResult ; //注册结果
    char m_name[MAX_SIZE];  //用户名

} STRU_LOGIN_RS;

//注册请求
typedef struct STRU_REGISTER_RQ
{
    STRU_REGISTER_RQ()
    {
        m_nType = DEF_PACK_REGISTER_RQ;
        memset(m_tel,0,MAX_SIZE);
        memset(m_password,0,MAX_SIZE);
    }

    PackType m_nType;   //包类型
    char     m_tel[MAX_SIZE] ; //手机号
    char     m_name[MAX_SIZE];
    char     m_password[MAX_SIZE];  //密码

} STRU_REGISTER_RQ;

//注册回复
typedef struct STRU_REGISTER_RS
{
    STRU_REGISTER_RS()
    {
        m_nType= DEF_PACK_REGISTER_RS;
        m_lResult = 0;
    }
    PackType m_nType;   //包类型
    int  m_lResult ; //注册结果

} STRU_REGISTER_RS;



//创建房间请求
typedef struct STRU_CREATEROOM_RQ
{
    STRU_CREATEROOM_RQ()
    {
        m_nType = DEF_PACK_CREATEROOM_RQ;
        m_UserID = 0;
    }

    PackType m_nType;   //包类型
    int m_UserID;

}STRU_CREATEROOM_RQ;

//创建房间回复
typedef struct STRU_CREATEROOM_RS
{
    STRU_CREATEROOM_RS()
    {
        m_nType= DEF_PACK_CREATEROOM_RS;
        m_lResult = 0;
        m_RoomId = 0;
    }
    PackType m_nType;   //包类型
    int  m_lResult ;    //创建结果
    int  m_RoomId;

}STRU_CREATEROOM_RS;

//加入房间请求
typedef struct STRU_JOINROOM_RQ
{
    STRU_JOINROOM_RQ()
    {
        m_nType = DEF_PACK_JOINROOM_RQ;
        m_UserID = 0;
        m_RoomID = 0;
    }

    PackType m_nType;   //包类型
    int m_UserID;
    int m_RoomID;

}STRU_JOINROOM_RQ;

//加入房间回复
typedef struct STRU_JOINROOM_RS
{
    STRU_JOINROOM_RS()
    {
        m_nType= DEF_PACK_JOINROOM_RS;
        m_lResult = 0;
        m_RoomID = 0;
    }
    PackType m_nType;   //包类型
    int  m_lResult ;    //注册结果
    int m_RoomID;
}STRU_JOINROOM_RS;

//房间成员请求
typedef struct STRU_ROOM_MEMBER_RQ
{
    STRU_ROOM_MEMBER_RQ()
    {
        m_nType= DEF_PACK_ROOM_MEMBER;
        m_UserID =0;
        memset(m_szUser,0,MAX_SIZE);
    }
    PackType m_nType;   //包类型
    int m_UserID;
    int m_icon;
    char m_szUser[MAX_SIZE];

}STRU_ROOM_MEMBER_RQ;


//离开房间请求
typedef struct STRU_LEAVEROOM_RQ
{
    STRU_LEAVEROOM_RQ()
    {
        m_nType = DEF_PACK_LEAVEROOM_RQ;
        m_nUserId = 0;
        m_RoomId = 0;
        memset(szUserName,0,MAX_SIZE);
    }
    PackType   m_nType;   //包类型
    int    m_nUserId; //用户ID
    int    m_RoomId;
    char   szUserName[MAX_SIZE];
}STRU_LEAVEROOM_RQ;


// id -> userInfo
typedef struct UserInfo
{
    UserInfo()
    {
         m_sockfd = 0;
         m_id = 0;
         m_roomid = 0;
         memset(m_userName, 0 , MAX_SIZE);
        m_videofd = 0;
        m_audiofd = 0;
    }
    //struct bufferevent*  m_sockfd;  用下面的替换y
    int m_sockfd;
    int  m_id;
    int  m_roomid;
    char m_userName[MAX_SIZE];

    int  m_videofd;
    int  m_audiofd;

}UserInfo;
//信息请求结构体
typedef struct STRU_USER_INFO_RQ
{
    STRU_USER_INFO_RQ():type(DEF_PACK_USER_INFO) , userid(0) , iconid(0)
    {
        memset( name ,  0 , sizeof(name) );
        memset( feeling ,  0 , sizeof(feeling) );
    }
    //需要 用户id 头像id 昵称 签名 状态--是否在线
    PackType type;
    int userid;
    int iconid;
    char name[MAX_SIZE];
    char feeling[MAX_SIZE];
}STRU_USER_INFO_RQ;
//注册音频
struct STRU_AUDIO_REGISTER
{
    STRU_AUDIO_REGISTER():m_nType(DEF_PACK_AUDIO_REGISTER){
        m_userid = 0;
    }

    PackType m_nType;   //包类型
    int m_userid;
};

//注册视频
struct STRU_VIDEO_REGISTER
{
    STRU_VIDEO_REGISTER():m_nType(DEF_PACK_VIDEO_REGISTER){
        m_userid = 0;
    }

    PackType m_nType;   //包类型
    int m_userid;
};

//下线数据包
struct STRU_OFFLINE
{
    STRU_OFFLINE():m_nType(DEF_PACK_USER_OFFLINE) {}

    PackType m_nType;
    int m_userid;
};
///音频数据帧
/// 成员描述
/// int type;
/// int userId;
/// int roomId;
/// int hour;
/// int min;
/// int sec;
/// int msec;
/// QByteArray audioFrame;  --> char frame[]; 柔性数组
///


///视频数据帧
/// 成员描述
/// int type;
/// int userId;
/// int roomId;
/// int hour;
/// int min;
/// int sec;
/// int msec;
/// QByteArray videoFrame;
///



#endif



