#include "Mysql.h"



int CMysql::ConnectMysql(const char* server,const char* user,const char* password, const char* database)
{
    conn = nullptr;
    conn = mysql_init(nullptr);//初始化 mysql 句柄
    mysql_set_character_set(conn,"utf8");//设置数据库编码
    if(!mysql_real_connect(conn,server,user,password,database,0,nullptr,0))
    {//连接数据库
        return FALSE;
    }
    return TRUE;
}

int CMysql::SelectMysql(char* szSql,int nColumn,list<string>& lst)
{
    MYSQL_RES * results = nullptr;
    if(mysql_query(conn,szSql))return FALSE;//查询sql语句
    results = mysql_store_result(conn);//从上一次操作中返回结果集
    if(nullptr == results)return FALSE;
    MYSQL_ROW record;
    while((record = mysql_fetch_row(results)))//从结果集中获取一行数据
    {
        for(int i=0; i<nColumn; i++)
        {
            lst.push_back( record[i] );
        }
    }
    return TRUE;
}

int CMysql::UpdataMysql(char *szsql)
{
    if(!szsql)return FALSE;
    if(mysql_query(conn,szsql))return FALSE;

    return TRUE;
}

void CMysql::DisConnect()
{
    mysql_close(conn);
}
