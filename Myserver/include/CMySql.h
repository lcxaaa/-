#pragma once
#include<iostream>
#include<mysql/mysql.h>
#include <list>
#include<string>
using namespace std;

class CMySql
{
public:
    CMySql(void);
    ~CMySql(void);
public:                    //ip,用户名,密码，数据库，端口号
    bool  ConnectMySql(char *host,char *user,char *pass,char *db,short nport = 3306);
    void  DisConnect();
    bool  SelectMySql(char* szSql,int nColumn,list<string>& lstStr);
	//获得数据库中的表
    bool GetTables(char* szSql,list<string>& lstStr);
    //更新：删除、插入、修改
    bool  UpdateMySql(char* szSql);
 
private:
    MYSQL *m_sock;   
	MYSQL_RES *m_results;   
	MYSQL_ROW m_record; 
   
};

