#include "clogic.h"

void CLogic::setNetPackMap() {
    NetPackMap(DEF_PACK_REGISTER_RQ) = &CLogic::RegisterRq;
    NetPackMap(DEF_PACK_LOGIN_RQ) = &CLogic::LoginRq;
    NetPackMap(DEF_PACK_CREATEROOM_RQ) = &CLogic::create_room;
    NetPackMap(DEF_PACK_JOINROOM_RQ) = &CLogic::join_room;
    NetPackMap(DEF_PACK_USER_INFO) = &CLogic::update_info;
    NetPackMap(DEF_PACK_LEAVEROOM_RQ) = &CLogic::quit_room;
    NetPackMap(DEF_PACK_AUDIO_FRAME) = &CLogic::audio_frame;
    NetPackMap(DEF_PACK_VIDEO_FRAME) = &CLogic::video_frame;
    NetPackMap(DEF_PACK_AUDIO_REGISTER) = &CLogic::audio_reg;
    NetPackMap(DEF_PACK_VIDEO_REGISTER) = &CLogic::video_reg;
    NetPackMap(DEF_PACK_USER_OFFLINE) = &CLogic::user_offline;
}

void CLogic::audio_reg(sock_fd clientfd, char *szbuf, int nlen) {
    printf("clientfd %d audio register\n", clientfd);
    STRU_AUDIO_REGISTER* rq = (STRU_AUDIO_REGISTER*)szbuf;
    UserInfo * user = nullptr;
    if(!m_map_id_to_userinfo.find(rq->m_userid, user)) return;
    user->m_sockfd = clientfd;
}

void CLogic::video_reg(sock_fd clientfd, char *szbuf, int nlen) {
    printf("clientfd %d video register\n", clientfd);
    STRU_VIDEO_REGISTER* rq = (STRU_VIDEO_REGISTER*)szbuf;
    UserInfo * user = nullptr;
    if(!m_map_id_to_userinfo.find(rq->m_userid, user)) return;
    user->m_sockfd = clientfd;
}

//注册
void CLogic::RegisterRq(sock_fd clientfd, char *szbuf, int nlen) {
    printf("clientfd:%d RegisterRq\n", clientfd);
    STRU_REGISTER_RS register_rs;
    STRU_REGISTER_RQ *register_rq = (STRU_REGISTER_RQ *) szbuf;
    //查询
    char sql_szbuf[1024]{};
    list<string> query_result;
    sprintf(sql_szbuf, "select telephone "
                       "from user "
                       "where telephone = '%s';", register_rq->m_tel);
    if (!m_sql->SelectMysql(sql_szbuf, 1, query_result)) {
        std::cout << "select tel fail" << std::endl;
    }

    if (query_result.size()) {
        register_rs.m_lResult = tel_is_exist;
    } else {
        query_result.clear();
        sprintf(sql_szbuf, "select name from user where name = '%s';", register_rq->m_name);
        if (!m_sql->SelectMysql(sql_szbuf, 1, query_result)) {
            std::cout << "select name fail" << std::endl;
        }
        if (query_result.size()) {
            register_rs.m_lResult = name_is_exist;
        } else {
            register_rs.m_lResult = register_success;
//            std::uniform_int_distribution<int> u(0, 35);
            srand((int) time(0));
            sprintf(sql_szbuf, "insert into user(telephone, password, name, icon, feeling) "
                               "values('%s', '%s', '%s', %d, '普通用户');",
                    register_rq->m_tel, register_rq->m_password, register_rq->m_name, rand() % 35);
            if (!m_sql->UpdataMysql(sql_szbuf)) {
                std::cout << "update fail : " << sql_szbuf << std::endl;
            }
        }
    }
    m_tcp->SendData(clientfd, (char *) &register_rs, sizeof(register_rs));
}

//登录
void CLogic::LoginRq(sock_fd clientfd, char *szbuf, int nlen) {
    printf("clientfd:%d LoginRq\n", clientfd);
    //拆包
    STRU_LOGIN_RQ *login_rq = (STRU_LOGIN_RQ *) szbuf;
    std::cout << login_rq->m_tel << ": login_rq" << std::endl;
    STRU_LOGIN_RS login_rs;
    list<string> do_result;
    char sql_buff[1024] = "";
    sprintf(sql_buff, "select id, password from user where telephone = '%s';", login_rq->m_tel);
    if (!m_sql->SelectMysql(sql_buff, 2, do_result)) {
        cout << "deal_login_data: >> query tel and password fail" << endl;
    }
    if (do_result.size()) {
        int id = atoi(do_result.front().c_str());
        do_result.pop_front();
        string password = do_result.front();
        do_result.pop_front();
        if (strcmp(password.c_str(), login_rq->m_password) == 0) {
            login_rs.m_lResult = login_success;
            login_rs.m_userid = id;

            UserInfo *user = nullptr;
            if (m_map_id_to_userinfo.find(id, user)) {
                //查到
                goto label;
                //强制让人下线
                //delete user;
            }
            //没查到
            user = new UserInfo;

            user->m_id = id;
            user->m_sockfd = clientfd;

            m_map_id_to_userinfo.insert(id, user);
            m_tcp->SendData(clientfd, (char *) &login_rs, sizeof(login_rs));
            //发送个人信息 更新状态
            get_user_info(id);
            return;
        } else {
            login_rs.m_lResult = password_error;
        }
    } else {
        //查无此人
        label : login_rs.m_lResult = user_not_exist;
    }
    //返回包信息
    m_tcp->SendData(clientfd, (char *) &login_rs, sizeof(login_rs));

}

//创建房间
void CLogic::create_room(sock_fd clientfd, char *szbuf, int nlen) {
    printf("clientfd:%d create_room\n", clientfd);
    STRU_JOINROOM_RQ *rq = (STRU_JOINROOM_RQ *) szbuf;
    default_random_engine e;
    uniform_int_distribution<unsigned> u(100000, 999999);
    int room_id = 0;
    do {
        room_id = u(e);
        printf("room id %d\n", room_id);
    } while (m_map_room_to_userlist.IsExist(room_id));

    list<int> lst;
    lst.push_back(rq->m_UserID);
    m_map_room_to_userlist.insert(room_id, lst);

    //回复
    STRU_CREATEROOM_RS rs;
    rs.m_RoomId = room_id;
    rs.m_lResult = create_success;
    SendData(clientfd, (char *) &rs, sizeof(rs));
}

//加入房间
void CLogic::join_room(sock_fd clientfd, char *szbuf, int nlen) {
    printf("client : %d Join Room rq\n", clientfd);
    //拆包
    STRU_JOINROOM_RQ *rq = (STRU_JOINROOM_RQ *) szbuf;
    STRU_JOINROOM_RS rs;
    //查看房间是否存在
    list<int> lst;
    if (!m_map_room_to_userlist.find(rq->m_RoomID, lst)) {
        rs.m_lResult = room_no_exist;
    } else {
        rs.m_lResult = join_success;
        rs.m_RoomID = rq->m_RoomID;
    }
    SendData(clientfd, (char *) &rs, sizeof(rs));
    //查看自身信息
    UserInfo *p_joinuser = nullptr;
    if (!m_map_id_to_userinfo.find(rq->m_UserID, p_joinuser)) return;
    //构建房间请求
    STRU_ROOM_MEMBER_RQ room_joiner_rq;
    room_joiner_rq.m_UserID = rq->m_UserID;
    strcpy(room_joiner_rq.m_szUser, p_joinuser->m_userName);
    //获取房间成员列表
    for (auto ite = lst.begin(); ite != lst.end(); ++ite) {
        int member_id = *ite;
        UserInfo *p_user = nullptr;
        if (!m_map_id_to_userinfo.find(member_id, p_user))
            continue;
        STRU_ROOM_MEMBER_RQ room_member_rq;
        room_member_rq.m_UserID = member_id;
        strcpy(room_member_rq.m_szUser, p_user->m_userName);
        //发送加入者信息给房间成员
        SendData(p_user->m_sockfd, (char *) &room_joiner_rq, sizeof(room_joiner_rq));
        //发送房间成员信息给加入者
        SendData(p_joinuser->m_sockfd, (char *) &room_member_rq, sizeof(room_member_rq));
    }
    //将加入者添加到房间列表中
    lst.push_back(p_joinuser->m_id);
    m_map_room_to_userlist.insert(rq->m_RoomID, lst);
}

//修改信息
void CLogic::update_info(sock_fd clientfd, char *szbuf, int nlen) {
    printf("client : %d update info rq\n", clientfd);

    //拆包
    STRU_USER_INFO_RQ *rq = (STRU_USER_INFO_RQ *) szbuf;
    printf("新名字: %s\n", rq->name);
    printf("新签名: %s\n", rq->feeling);

    //mysql 修改信息
    char sql_buff[1024] = "";
    sprintf(sql_buff, "update user set icon = %d, name = '%s', feeling = '%s' where id = %d;",
            rq->iconid, rq->name, rq->feeling, rq->userid);
    if (!m_sql->UpdataMysql(sql_buff)) {
        printf("update info >> update db fail\n");
        return;
    }
    //发送信息给客户端
    get_user_info(rq->userid);
}

void CLogic::get_user_info(int id) {
    //从数据库获取信息
    list<string> lst;
    char sql_buff[1024]{""};
    sprintf(sql_buff, "select name, icon, feeling from user where id = %d;", id);
    if (!m_sql->SelectMysql(sql_buff, 3, lst)) {
        printf("get user info >> select db fail\n");
        return;
    }
    if (lst.size() != 3) return;
    string name = lst.front();
    lst.pop_front();
    int icon = stoi(lst.front());
    lst.pop_front();
    string feeling = lst.front();
    lst.pop_front();
    //设置缓存信息
    UserInfo *user;
    if (m_map_id_to_userinfo.find(id, user)) {
        strcpy(user->m_userName, name.c_str());
    } else return;
    //发送客户端
    STRU_USER_INFO_RQ rq;
    rq.userid = id;
    strcpy(rq.feeling, feeling.c_str());
    strcpy(rq.name, name.c_str());
    rq.iconid = icon;

    SendData(user->m_sockfd, (char *) &rq, sizeof(rq));
}

//处理退出房间
void CLogic::quit_room(sock_fd clientfd, char *szbuf, int nlen) {
    printf("client : %d quit room rq\n", clientfd);
    STRU_LEAVEROOM_RQ *rq = (STRU_LEAVEROOM_RQ *) szbuf;
    list<int> lst;
    if (!m_map_room_to_userlist.find(rq->m_RoomId, lst)) return;

    //根据房间id 转发列表
    for (auto ite = lst.begin(); ite != lst.end();) {
        int id = *ite;
        if (id == rq->m_nUserId) {
            ite = lst.erase(ite);
        } else {
            //转发
            UserInfo *user = nullptr;
            if (!m_map_id_to_userinfo.find(id, user)) continue;
            SendData(user->m_sockfd, (char *) rq, nlen);
            ++ite;
        }
    }
    //更新缓存map
    if (lst.empty()) {
        m_map_room_to_userlist.erase(rq->m_RoomId);
    } else {
        m_map_room_to_userlist.insert(rq->m_RoomId, lst);
    }
}

//音频数据转发
void CLogic::audio_frame(sock_fd clientfd, char *szbuf, int nlen) {
    printf("client : %d audio frame \n", clientfd);
    //拆包，反序列化
    char *temp = szbuf;
    temp += sizeof(int);
    int user_id = *(int *) temp;
    temp += sizeof(int);
    int room_id = *(int *) temp;
    //temp += sizeof(int);
    list<int> lst;
    if (!m_map_room_to_userlist.find(room_id, lst)) return;
    for (auto ite = lst.begin(); ite != lst.end(); ++ite) {
        int id = *ite;
        if (id != user_id) {
            //转发
            UserInfo *user = nullptr;
            if (!m_map_id_to_userinfo.find(id, user)) return;
            SendData(user->m_sockfd, szbuf, nlen);
        }
    }
}

//视频数据转发
void CLogic::video_frame(sock_fd clientfd, char *szbuf, int nlen) {
    printf("client : %d video frame \n", clientfd);
    //拆包，反序列化
    char *temp = szbuf;
    temp += sizeof(int);
    int user_id = *(int *) temp;
    temp += sizeof(int);
    int room_id = *(int *) temp;
    //temp += sizeof(int);
    list<int> lst;
    if (!m_map_room_to_userlist.find(room_id, lst)) return;
    for (auto ite = lst.begin(); ite != lst.end(); ++ite) {
        int id = *ite;
        if (id != user_id) {
            //转发
            UserInfo *user = nullptr;
            if (!m_map_id_to_userinfo.find(id, user)) return;
            SendData(user->m_sockfd, szbuf, nlen);
        }
    }
}

void CLogic::user_offline(sock_fd clientfd, char *szbuf, int nlen) {
    printf("client : %d user offline \n", clientfd);
    STRU_OFFLINE *request_rq = (STRU_OFFLINE*)szbuf;
    m_map_id_to_userinfo.erase(request_rq->m_userid);
}
