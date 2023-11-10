#include "longzoro/version.h"

#ifdef _SASQL//新添加

#include "longzoro/sasql.h"
#include "char_base.h"
#include "configfile.h"

#include <mysql/mysql.h>
#include <stdio.h>
#include <string.h>
MYSQL mysql;
MYSQL_RES *mysql_result;
MYSQL_ROW mysql_row;
MYSQL_FIELD *fields;

typedef struct tagSQLConfig
{
    char    sql_IP[32];

    int     sql_Port;

    char    sql_Port1[16];

    char    sql_ID[16];

    char    sql_PS[32];

    char    sql_DataBase[16];

    char    sql_Table[16];

    char    sql_LOCK[16];

    char    sql_NAME[16];

    char    sql_PASS[16];

    int         AutoReg;

    int         openbackground;
} SQLConfig;

SQLConfig sqlconfig;

int AutoReg;
int openbackground;

int getHash(char* s)
{
    int i;
    int h = 0;

    for (i = 0;; i++)
    {
        if (s[i] == 0)
        {
            break;
        }

        h += s[i];
    }

    return h;
}

static int readSqlConfig(char *path)
{
    char buf[255];
    FILE *fp;
    fp = fopen(path, "r");

    if (fp == NULL)
    {
        return -2;
    }

    while (fgets(buf, sizeof(buf), fp))
    {
        char command[255];
        char param[255];
        chop(buf);
        easyGetTokenFromString(buf, 1, command, sizeof(command));
        easyGetTokenFromString(buf, 2, param, sizeof(param));

        if (strcmp(command, "sql_IP") == 0)
        {
            strcmp(sqlconfig.sql_IP, param) ;
            snprintf(sqlconfig.sql_IP, sizeof(sqlconfig.sql_IP), param);
#ifdef _STOPLOGININFORMATION
#else
            printf("数据库地址：  %s\n", sqlconfig.sql_IP);
#endif
        }
        else if (strcmp(command, "sql_Port") == 0)
        {
            sqlconfig.sql_Port = atoi(param);
            snprintf(sqlconfig.sql_Port1, sizeof(sqlconfig.sql_Port1), param);
#ifdef _STOPLOGININFORMATION
#else
            printf("数据库端口：  %d\n", sqlconfig.sql_Port);
#endif
        }
        else if (strcmp(command, "sql_ID") == 0)
        {
            strcmp(sqlconfig.sql_ID, param) ;
            snprintf(sqlconfig.sql_ID, sizeof(sqlconfig.sql_ID), param);
#ifdef _STOPLOGININFORMATION
#else
            printf("数据库用户：  %s\n", sqlconfig.sql_ID);
#endif
        }
        else if (strcmp(command, "sql_PS") == 0)
        {
            strcmp(sqlconfig.sql_PS, param) ;
            snprintf(sqlconfig.sql_PS, sizeof(sqlconfig.sql_PS), param);
#ifdef _STOPLOGININFORMATION
#else
            printf("数据库密码：  %s\n", sqlconfig.sql_PS);
#endif
        }
        else if (strcmp(command, "sql_DataBase") == 0)
        {
            strcmp(sqlconfig.sql_DataBase, param) ;
            snprintf(sqlconfig.sql_DataBase, sizeof(sqlconfig.sql_DataBase), param);
#ifdef _STOPLOGININFORMATION
#else
            printf("登陆数据库名：%s\n", sqlconfig.sql_DataBase);
#endif
        }
        else if (strcmp(command, "sql_Table") == 0)
        {
            strcmp(sqlconfig.sql_Table, param) ;
            snprintf(sqlconfig.sql_Table, sizeof(sqlconfig.sql_Table), param);
#ifdef _STOPLOGININFORMATION
#else
            printf("用户信息表名：  %s\n", sqlconfig.sql_Table);
#endif
        }
        else if (strcmp(command, "sql_LOCK") == 0)
        {
            snprintf(sqlconfig.sql_LOCK, sizeof(sqlconfig.sql_LOCK), param);
#ifdef _STOPLOGININFORMATION
#else
            printf("\n用户锁定表名：  %s", sqlconfig.sql_LOCK);
#endif
        }
        else if (strcmp(command, "sql_NAME") == 0)
        {
            strcmp(sqlconfig.sql_NAME, param) ;
            snprintf(sqlconfig.sql_NAME, sizeof(sqlconfig.sql_NAME), param);
#ifdef _STOPLOGININFORMATION
#else
            printf("账号字段名称：  %s\n", sqlconfig.sql_NAME);
#endif
        }
        else if (strcmp(command, "sql_PASS") == 0)
        {
            strcmp(sqlconfig.sql_PASS, param) ;
            snprintf(sqlconfig.sql_PASS, sizeof(sqlconfig.sql_PASS), param);
#ifdef _STOPLOGININFORMATION
#else
            printf("密码字段名称：  %s\n", sqlconfig.sql_PASS);
#endif
        }
        else if (strcmp(command, "openbackground") == 0)
        {
            sqlconfig.openbackground = atoi(param);

            if (sqlconfig.openbackground == 1)
            {
#ifdef _STOPLOGININFORMATION
#else
                printf("\n后台功能：    YES");
#endif
            }
            else
            {
#ifdef _STOPLOGININFORMATION
#else
                printf("\n后台功能：    NO");
#endif
            }
        }
        else if (strcmp(command, "AutoReg") == 0)
        {
            sqlconfig.AutoReg = atoi(param);

            if (sqlconfig.AutoReg)
            {
#ifdef _STOPLOGININFORMATION
#else
                printf("\n开放自动注册：YES");
#endif
            }
            else
            {
#ifdef _STOPLOGININFORMATION
#else
                printf("\n开放自动注册：NO");
#endif
            }
        }
    }

    fclose(fp);
    return 0;
}


BOOL sasql_init(void)
{
    if ((mysql_init(&mysql) == NULL) & readSqlConfig("sql.cf"))
    {
        printf("\n数据库初始化失败！\n");
        exit(1);
        return FALSE;
    }

    if (!mysql_real_connect(&mysql,
                            sqlconfig.sql_IP,
                            sqlconfig.sql_ID,//帐号
                            sqlconfig.sql_PS,//密码
                            sqlconfig.sql_DataBase,//选择的资料库
                            sqlconfig.sql_Port,
                            NULL,
                            0))
    {
        printf("\n数据库连接失败！\n");
        return FALSE;
    }

#ifdef _UTF8
    mysql_query(&mysql, "set names 'utf8'");
#else
    mysql_query(&mysql, "set names 'gbk'");
#endif // _UTF8
    //printf("\n数据库连接成功！\n");
#ifdef _STOPLOGININFORMATION
#else
    printf("\n数据库连接成功！\n");
#endif
    return TRUE;
}

int sasql_mysql_query(char *sqlstr)
{
#ifdef _UTF8
    mysql_query(&mysql, "set names 'utf8'");
#else
    mysql_query(&mysql, "set names 'gbk'");
#endif // _UTF8
    int ret = mysql_query(&mysql, sqlstr);

    if (ret)
    {
        //printf("重新连接数据库...");
#ifdef _STOPLOGININFORMATION
#else
        printf("\n数据库查找失败！\n");
        printf("重新连接数据库...");
#endif
        sasql_close();
        sasql_init();
        ret = mysql_query(&mysql, sqlstr);
    }

    return ret;
}

void sasql_close(void)
{
    mysql_close(&mysql);
}



BOOL sasql_ckeckStrint(char *str)
{
    int i;

    for (i = 0; i < strlen(str); i++)
    {
        if ((str[i] < 'a' || str[i] > 'z') && (str[i] < 'A' || str[i] > 'Z') && (str[i] < '0' || str[i] > '9') && (str[i] != '.') && (str[i] != '-'))
        {
            return FALSE;
        }
    }

    return TRUE;
}

int sasql_query(char *nm, char *pas
               )
{
    char sqlstr[1024];

    if (sasql_ckeckStrint(nm) == FALSE)
    {
        //printf("异常字符的用户名%s\n",nm);
        return 4;
    }

    //sprintf(sqlstr,"select `%s`, `salt` from %s where %s=BINARY'%s'",config.sql_PASS, config.sql_Table,config.sql_NAME,nm);
    sprintf(sqlstr, "select `%s` from %s where %s=BINARY'%s'", sqlconfig.sql_PASS, sqlconfig.sql_Table, sqlconfig.sql_NAME, nm);

    if (!sasql_mysql_query(sqlstr))
    {
        int num_row = 0;
        mysql_free_result(mysql_result);
        mysql_result = mysql_store_result(&mysql);
        num_row = mysql_num_rows(mysql_result);

        if (num_row > 0)
        {
            char token[64];
            mysql_row = mysql_fetch_row(mysql_result);
            strcpy(token, mysql_row[0]);
#ifdef _OLDPS_TO_MD5PS

            if (strlen(token) < 32)
            {
                if (strcmp(pas, token) == 0)
                {
                    sprintf(sqlstr, "update `%s` set `%s`='%s' where %s=BINARY'%s'", sqlconfig.sql_Table, sqlconfig.sql_PASS, MD5String(token), sqlconfig.sql_NAME, nm);
                    sasql_mysql_query(sqlstr);
                    return 1;
                }
                else
                {
                    sprintf(sqlstr, "update `%s` set `%s`='%s' where %s=BINARY'%s'", sqlconfig.sql_Table, sqlconfig.sql_PASS, MD5String(token), sqlconfig.sql_NAME, nm);
                    sasql_mysql_query(sqlstr);
                    return 2;
                }
            }
            else
            {
                if (strcmp(MD5String(pas), token) == 0)
                {
                    return 1;
                }
                else
                {
                    return 2;
                }
            }

#else

            if (strcmp(pas, token) == 0)
            {
                return 1;
            }
            else
            {
                printf("用户%s密码错误！\n", nm);
                return 2;
            }

#endif
        }
        else
        {
            printf("用户%s未注册！\n", nm);
            return 3;
        }
    }
    else
    {
#ifdef _STOPLOGININFORMATION
#else
        printf("\n数据库查找失败！\n");
        printf("重新连接数据库...");
#endif
        sasql_close();
        sasql_init();
        printf("完成\n");
        return 0;
    }
}


BOOL sasql_online(char *ID, char *NM, char *IP, char *MAC1, char *MAC2, char *MAC3, int flag)
{
    if (sqlconfig.openbackground == 1)
    {
        if (ID != NULL)
        {
            if (sasql_ckeckStrint(ID) == FALSE)
            {
                //printf("异常字符的用户名%s\n",ID);
                return TRUE;
            }
        }

        char sqlstr[256];
        memset(sqlstr, 0, sizeof(sqlstr));

        if (flag == 0)
        {
            sprintf(sqlstr, "update %s set Online=0,Offline=0 where %s=BINARY'%s'", sqlconfig.sql_Table, sqlconfig.sql_NAME, ID);
        }
        else if (flag == 1)
        {
            sprintf(sqlstr, "update %s set LoginTime=NOW(), IP='%s',MAC1='%s',MAC2='%s',MAC3='%s', Online=%d where %s=BINARY'%s'", sqlconfig.sql_Table, IP, MAC1, MAC2, MAC3, getServernumber(), sqlconfig.sql_NAME, ID);
        }
        else if (flag == 2)
        {
            sprintf(sqlstr, "update %s set OnlineName='%s', Path='char/0x%x' where %s=BINARY'%s'", sqlconfig.sql_Table, NM, getHash(ID) & 0xff, sqlconfig.sql_NAME, ID);
        }
        else if (flag == 3)
        {
            sprintf(sqlstr, "update %s set Online=0,Offline=0 where Online=%d", sqlconfig.sql_Table, getServernumber());
        }

        if (!sasql_mysql_query(sqlstr))
        {
            return TRUE;
        }

        printf("\n更新数据库失败%s\n", sqlstr);
#ifdef _STOPLOGININFORMATION
#else
        printf("重新连接数据库...");
#endif
        sasql_close();
        sasql_init();
        printf("完成\n");
    }

    return TRUE;
}

int sasql_queryonline(char *ID)
{
    if (sqlconfig.openbackground == 1)
    {
        if (ID != NULL)
        {
            if (sasql_ckeckStrint(ID) == FALSE)
            {
                //printf("异常字符的用户名%s\n",ID);
                return TRUE;
            }
        }

        char sqlstr[256];
        sprintf(sqlstr, "select Online from `%s` where %s=BINARY'%s'", sqlconfig.sql_Table, sqlconfig.sql_NAME, ID);

        if (!sasql_mysql_query(sqlstr))
        {
            int num_row = 0;
            mysql_free_result(mysql_result);
            mysql_result = mysql_store_result(&mysql);
            num_row = mysql_num_rows(mysql_result);

            if (num_row > 0)
            {
                mysql_row = mysql_fetch_row(mysql_result);
                return atoi(mysql_row[0]);
            }
            else
            {
                return -1;
            }
        }

#ifdef _STOPLOGININFORMATION
#else
        printf("\n数据库查找失败！\n");
        printf("重新连接数据库...");
#endif
        sasql_close();
        sasql_init();
        printf("完成\n");
        return -1;
    }

    return -1;
}

int sasql_queryoffline(char *ID)
{
    if (sqlconfig.openbackground == 1)
    {
        if (ID != NULL)
        {
            if (sasql_ckeckStrint(ID) == FALSE)
            {
                //printf("异常字符的用户名%s\n",ID);
                return TRUE;
            }
        }

        char sqlstr[256];
        sprintf(sqlstr, "select `Offline` from `%s` where %s=BINARY'%s'", sqlconfig.sql_Table, sqlconfig.sql_NAME, ID);

        if (!sasql_mysql_query(sqlstr))
        {
            int num_row = 0;
            mysql_free_result(mysql_result);
            mysql_result = mysql_store_result(&mysql);
            num_row = mysql_num_rows(mysql_result);

            if (num_row > 0)
            {
                mysql_row = mysql_fetch_row(mysql_result);
                return atoi(mysql_row[0]);
            }
            else
            {
                return -1;
            }
        }

#ifdef _STOPLOGININFORMATION
#else
        printf("\n数据库查找失败！\n");
        printf("重新连接数据库...");
#endif
        sasql_close();
        sasql_init();
        printf("完成\n");
        return -1;
    }

    return -1;
}

BOOL sasql_register(char *id, char *ps, char *ip, char *uid)
{
    char sqlstr[256];
    //if(sqlconfig.AutoReg!=1)return FALSE;

    if (sasql_ckeckStrint(id) == FALSE)
    {
        //printf("异常字符的用户名%s\n",id);
        return FALSE;
    }

    if (sasql_ckeckStrint(ps) == FALSE)
    {
        //printf("异常字符的游戏密码%s\n",ps);
        return FALSE;
    }

#ifdef _OLDPS_TO_MD5PS
    sprintf(sqlstr, "INSERT INTO %s (%s,%s,RegTime,Path) VALUES (BINARY'%s','%s',NOW(),'char/0x%x')", sqlconfig.sql_Table, sqlconfig.sql_NAME, sqlconfig.sql_PASS, id, MD5String(ps), getHash(id) & 0xff);
#else
    sprintf(sqlstr, "INSERT INTO %s (%s,%s,RegIP,RegTime,Path,uid) VALUES (BINARY'%s','%s','%s',NOW(),'char/0x%x','%s')", sqlconfig.sql_Table, sqlconfig.sql_NAME, sqlconfig.sql_PASS, id, ps, ip, getHash(id) & 0xff, uid);
#endif

    if (!sasql_mysql_query(sqlstr))
    {
        printf("\n新用户注册成功！\n");
        return TRUE;
    }

    printf("\n新用户注册失败！\n");
    return FALSE;
}

BOOL sasql_chehk_lock(char *idip)
{
    if (sasql_ckeckStrint(idip) == FALSE)
    {
        //printf("异常字符%s\n",idip);
        return FALSE;
    }

    char sqlstr[256];
    sprintf(sqlstr, "select * from `%s` where Name='%s'", sqlconfig.sql_LOCK, idip);

    if (!sasql_mysql_query(sqlstr))
    {
        int num_row = 0;
        mysql_free_result(mysql_result);
        mysql_result = mysql_store_result(&mysql);
        num_row = mysql_num_rows(mysql_result);

        if (num_row > 0)
        {
            return TRUE;
        }
    }

    return FALSE;
}

BOOL sasql_add_lock(char *idip)
{
    if (sasql_ckeckStrint(idip) == FALSE)
    {
        //printf("异常字符%s\n",idip);
        return FALSE;
    }

    char sqlstr[256];
    sprintf(sqlstr, "INSERT INTO `%s` (Name) VALUES (BINARY'%s')", sqlconfig.sql_LOCK, idip);

    if (!sasql_mysql_query(sqlstr))
    {
        printf("\n添加锁定%s成功！\n", idip);
        return TRUE;
    }

    return FALSE;
}

BOOL sasql_del_lock(char *idip)
{
    if (sasql_ckeckStrint(idip) == FALSE)
    {
        //printf("异常字符%s\n",idip);
        return FALSE;
    }

    char sqlstr[256];
    sprintf(sqlstr, "delete from `%s` where Name=BINARY'%s'", sqlconfig.sql_LOCK, idip);

    if (!sasql_mysql_query(sqlstr))
    {
        printf("\n解除锁定%s成功！\n", idip);
        return TRUE;
    }

    return FALSE;
}

#ifdef _ITEM_PET_LOCKED
char *sasql_ItemPetLocked(char *id, char *safepasswd)
{
    if (sasql_ckeckStrint(id) == FALSE)
    {
        //printf("异常字符%s\n",id);
        return "无法解锁，请与本服管理员联系！";
    }

    if (sasql_ckeckStrint(safepasswd) == FALSE)
    {
        //printf("异常字符%s\n",safepasswd);
        return "无法解锁，请与本服管理员联系！";
    }

    char sqlstr[256];
    sprintf(sqlstr, "select SafePasswd from `%s` where %s=BINARY'%s'", sqlconfig.sql_Table, sqlconfig.sql_NAME, id);

    if (!sasql_mysql_query(sqlstr))
    {
        int num_row = 0;
        mysql_free_result(mysql_result);
        mysql_result = mysql_store_result(&mysql);
        num_row = mysql_num_rows(mysql_result);

        if (num_row > 0)
        {
            mysql_row = mysql_fetch_row(mysql_result);

            if (mysql_row[0] != NULL && strlen(mysql_row[0]) > 0)
            {
                if (strcmp(safepasswd, mysql_row[0]) == 0)
                {
                    return "安全锁已经成功解锁！";
                }
                else
                {
                    return "密码错误，安全锁无法解开！";
                }
            }
            else
            {
                return "您还未设置安全锁解锁密码，为了确保安全，请输入一次六位以上密码做为安全锁密码并劳劳记住！";
            }
        }
    }

    return "无法解锁，请与本服管理员联系！";
}

char *sasql_ItemPetLocked_Passwd(char *id, char *safepasswd)
{
    if (sasql_ckeckStrint(id) == FALSE)
    {
        //printf("异常字符%s\n",id);
        return "安全密码修改失败，请与本服管理员联系！";
    }

    if (sasql_ckeckStrint(safepasswd) == FALSE)
    {
        //printf("异常字符%s\n",safepasswd);
        return "安全密码修改失败，请与本服管理员联系！";
    }

    char sqlstr[256];
    sprintf(sqlstr, "select SafePasswd from %s where %s=BINARY'%s'", sqlconfig.sql_Table, sqlconfig.sql_NAME, id);

    if (!sasql_mysql_query(sqlstr))
    {
        int num_row = 0;
        mysql_free_result(mysql_result);
        mysql_result = mysql_store_result(&mysql);
        num_row = mysql_num_rows(mysql_result);

        if (num_row > 0)
        {
            mysql_row = mysql_fetch_row(mysql_result);

            if (mysql_row[0] != NULL && strlen(mysql_row[0]) > 0)
            {
                return "安全密码已存在,无法再进行修改！";
            }
            else
            {
                char sqlstr[256];
                sprintf(sqlstr, "update %s set SafePasswd=BINARY'%s' where %s=BINARY'%s'", sqlconfig.sql_Table, safepasswd, sqlconfig.sql_NAME, id);

                if (!sasql_mysql_query(sqlstr))
                {
                    return "安全密码修改成功，请妥善保管你的安全密码！";
                }

                return "安全密码修改失败，请与本服管理员联系！";
            }
        }
        else
        {
            return "账号不存在安全密码已存在！";
        }
    }
    else
    {
#ifdef _STOPLOGININFORMATION
#else
        printf("\n数据库查找失败！\n");
        printf("重新连接数据库...");
#endif
        sasql_close();
        sasql_init();
        printf("完成\n");
        return "安全密码修改失败，请与本服管理员联系！";
    }
}

int sasql_ItemPetLocked_Char(char *id, char *safepasswd)
{
    if (sasql_ckeckStrint(id) == FALSE)
    {
        //printf("异常字符%s\n",id);
        return 0;
    }

    if (sasql_ckeckStrint(safepasswd) == FALSE)
    {
        //printf("异常字符%s\n",safepasswd);
        return 0;
    }

    char sqlstr[256];
    sprintf(sqlstr, "select SafePasswd from %s where %s=BINARY'%s'", sqlconfig.sql_Table, sqlconfig.sql_NAME, id);

    if (!sasql_mysql_query(sqlstr))
    {
        int num_row = 0;
        mysql_free_result(mysql_result);
        mysql_result = mysql_store_result(&mysql);
        num_row = mysql_num_rows(mysql_result);

        if (num_row > 0)
        {
            mysql_row = mysql_fetch_row(mysql_result);

            if (mysql_row[0] != NULL && strlen(mysql_row[0]) > 0)
            {
                if (strcmp(safepasswd, mysql_row[0]) == 0)
                {
                    return 1;
                }
                else
                {
                    printf("用户%s安全密码错误！\n", id);
                    return -1;
                }
            }
            else
            {
                return 0;
            }
        }
        else
        {
            printf("用户%s未注册！\n", id);
            return -1;
        }
    }
    else
    {
#ifdef _STOPLOGININFORMATION
#else
        printf("\n数据库查找失败！\n");
        printf("重新连接数据库...");
#endif
        sasql_close();
        sasql_init();
        printf("完成\n");
        return -1;
    }
}
char* sasql_getItemPetLocked(char *id)
{
    if (sasql_ckeckStrint(id) == FALSE)
    {
        //printf("异常字符%s\n",id);
        return "error";
    }

    char sqlstr[256];
    sprintf(sqlstr, "select SafePasswd from %s where %s=BINARY'%s'", sqlconfig.sql_Table, sqlconfig.sql_NAME, id);

    if (!sasql_mysql_query(sqlstr))
    {
        int num_row = 0;
        mysql_free_result(mysql_result);
        mysql_result = mysql_store_result(&mysql);
        num_row = mysql_num_rows(mysql_result);

        if (num_row > 0)
        {
            mysql_row = mysql_fetch_row(mysql_result);

            if (mysql_row[0] != NULL && strlen(mysql_row[0]) > 0)
            {
                return mysql_row[0];
            }
            else
            {
                return "error";
            }
        }
        else
        {
            printf("用户%s未注册！\n", id);
            return "error";
        }
    }
    else
    {
#ifdef _STOPLOGININFORMATION
#else
        printf("\n数据库查找失败！\n");
        printf("重新连接数据库...");
#endif
        sasql_close();
        sasql_init();
        printf("完成\n");
        return "error";
    }
}
#endif

int sasql_getVipPoint(int charaindex)
{
    char sqlstr[256];
    char *id = CHAR_getChar(charaindex, CHAR_CDKEY);
    sprintf(sqlstr, "select VipPoint from `%s` where %s=BINARY'%s'", sqlconfig.sql_Table, sqlconfig.sql_NAME, id);

    if (!sasql_mysql_query(sqlstr))
    {
        int num_row = 0;
        mysql_free_result(mysql_result);
        mysql_result = mysql_store_result(&mysql);
        num_row = mysql_num_rows(mysql_result);

        if (num_row > 0)
        {
            mysql_row = mysql_fetch_row(mysql_result);
            return atoi(mysql_row[0]);
        }
    }

#ifdef _STOPLOGININFORMATION
#else
    printf("\n数据库查找失败！\n");
    printf("重新连接数据库...");
#endif
    sasql_close();
    sasql_init();
    printf("完成\n");
    return -1;
}

int sasql_setVipPoint(int charaindex, int point)
{
    char sqlstr[256];
    char *id = CHAR_getChar(charaindex, CHAR_CDKEY);
    sprintf(sqlstr, "update %s set VipPoint=%d where %s=BINARY'%s'", sqlconfig.sql_Table, point, sqlconfig.sql_NAME, id);

    if (!sasql_mysql_query(sqlstr))
    {
        //printf("用户%s当前重回币%d！\n",id,point);
        return point;
    }

#ifdef _STOPLOGININFORMATION
#else
    printf("\n数据库查找失败！\n");
    printf("重新连接数据库...");
#endif
    sasql_close();
    sasql_init();
    printf("完成\n");
    return -1;
}

int sasql_getVipPointForCdkey(char *id)
{
    if (sasql_ckeckStrint(id) == FALSE)
    {
        //printf("异常字符的用户名%s\n",id);
        return -1;
    }

    char sqlstr[256];
    sprintf(sqlstr, "select VipPoint from `%s` where %s=BINARY'%s'", sqlconfig.sql_Table, sqlconfig.sql_NAME, id);

    if (!sasql_mysql_query(sqlstr))
    {
        int num_row = 0;
        mysql_free_result(mysql_result);
        mysql_result = mysql_store_result(&mysql);
        num_row = mysql_num_rows(mysql_result);

        if (num_row > 0)
        {
            mysql_row = mysql_fetch_row(mysql_result);
            return atoi(mysql_row[0]);
        }
    }

#ifdef _STOPLOGININFORMATION
#else
    printf("\n数据库查找失败！\n");
    printf("重新连接数据库...");
#endif
    sasql_close();
    sasql_init();
    printf("完成\n");
    return -1;
}

int sasql_setVipPointForCdkey(char *id, int point)
{
    if (sasql_ckeckStrint(id) == FALSE)
    {
        //printf("异常字符的用户名%s\n",id);
        return -1;
    }

    char sqlstr[256];
    sprintf(sqlstr, "update %s set VipPoint=%d where %s=BINARY'%s'", sqlconfig.sql_Table, point, sqlconfig.sql_NAME, id);

    if (!sasql_mysql_query(sqlstr))
    {
        //printf("用户%s当前重回币%d！\n",id,point);
        return point;
    }

#ifdef _STOPLOGININFORMATION
#else
    printf("\n数据库查找失败！\n");
    printf("重新连接数据库...");
#endif
    sasql_close();
    sasql_init();
    printf("完成\n");
    return -1;
}


int sasql_getPayPoint(int charaindex)
{
    char sqlstr[256];
    char *id = CHAR_getChar(charaindex, CHAR_CDKEY);
    sprintf(sqlstr, "select PayPoint from `%s` where %s=BINARY'%s'", sqlconfig.sql_Table, sqlconfig.sql_NAME, id);

    if (!sasql_mysql_query(sqlstr))
    {
        int num_row = 0;
        mysql_free_result(mysql_result);
        mysql_result = mysql_store_result(&mysql);
        num_row = mysql_num_rows(mysql_result);

        if (num_row > 0)
        {
            mysql_row = mysql_fetch_row(mysql_result);
            return atoi(mysql_row[0]);
        }
    }

#ifdef _STOPLOGININFORMATION
#else
    printf("\n数据库查找失败！\n");
    printf("重新连接数据库...");
#endif
    sasql_close();
    sasql_init();
    printf("完成\n");
    return -1;
}

int sasql_setPayPoint(int charaindex, int point)
{
    char sqlstr[256];
    char *id = CHAR_getChar(charaindex, CHAR_CDKEY);
    sprintf(sqlstr, "update %s set PayPoint=%d where %s=BINARY'%s'", sqlconfig.sql_Table, point, sqlconfig.sql_NAME, id);

    if (!sasql_mysql_query(sqlstr))
    {
        //printf("用户%s当前重回币%d！\n",id,point);
        return point;
    }

#ifdef _STOPLOGININFORMATION
#else
    printf("\n数据库查找失败！\n");
    printf("重新连接数据库...");
#endif
    sasql_close();
    sasql_init();
    printf("完成\n");
    return -1;
}

int sasql_getPayPointForCdkey(char *id)
{
    if (sasql_ckeckStrint(id) == FALSE)
    {
        //printf("异常字符的用户名%s\n",id);
        return -1;
    }

    char sqlstr[256];
    sprintf(sqlstr, "select PayPoint from `%s` where %s=BINARY'%s'", sqlconfig.sql_Table, sqlconfig.sql_NAME, id);

    if (!sasql_mysql_query(sqlstr))
    {
        int num_row = 0;
        mysql_free_result(mysql_result);
        mysql_result = mysql_store_result(&mysql);
        num_row = mysql_num_rows(mysql_result);

        if (num_row > 0)
        {
            mysql_row = mysql_fetch_row(mysql_result);
            return atoi(mysql_row[0]);
        }
    }

#ifdef _STOPLOGININFORMATION
#else
    printf("\n数据库查找失败！\n");
    printf("重新连接数据库...");
#endif
    sasql_close();
    sasql_init();
    printf("完成\n");
    return -1;
}

int sasql_setPayPointForCdkey(char *id, int point)
{
    if (sasql_ckeckStrint(id) == FALSE)
    {
        //printf("异常字符的用户名%s\n",id);
        return -1;
    }

    char sqlstr[256];
    sprintf(sqlstr, "update %s set PayPoint=%d where %s=BINARY'%s'", sqlconfig.sql_Table, point, sqlconfig.sql_NAME, id);

    if (!sasql_mysql_query(sqlstr))
    {
        //printf("用户%s当前重回币%d！\n",id,point);
        return point;
    }

#ifdef _STOPLOGININFORMATION
#else
    printf("\n数据库查找失败！\n");
    printf("重新连接数据库...");
#endif
    sasql_close();
    sasql_init();
    printf("完成\n");
    return -1;
}


int sasql_getRmbPoint(int charaindex)
{
    char sqlstr[256];
    char *id = CHAR_getChar(charaindex, CHAR_CDKEY);
    sprintf(sqlstr, "select RmbPoint from `%s` where %s=BINARY'%s'", sqlconfig.sql_Table, sqlconfig.sql_NAME, id);

    if (!sasql_mysql_query(sqlstr))
    {
        int num_row = 0;
        mysql_free_result(mysql_result);
        mysql_result = mysql_store_result(&mysql);
        num_row = mysql_num_rows(mysql_result);

        if (num_row > 0)
        {
            mysql_row = mysql_fetch_row(mysql_result);
            return atoi(mysql_row[0]);
        }
    }

#ifdef _STOPLOGININFORMATION
#else
    printf("\n数据库查找失败！\n");
    printf("重新连接数据库...");
#endif
    sasql_close();
    sasql_init();
    printf("完成\n");
    return -1;
}

int sasql_setRmbPoint(int charaindex, int point)
{
    char sqlstr[256];
    char *id = CHAR_getChar(charaindex, CHAR_CDKEY);
    sprintf(sqlstr, "update %s set RmbPoint=%d where %s=BINARY'%s'", sqlconfig.sql_Table, point, sqlconfig.sql_NAME, id);

    if (!sasql_mysql_query(sqlstr))
    {
        return point;
    }

#ifdef _STOPLOGININFORMATION
#else
    printf("\n数据库查找失败！\n");
    printf("重新连接数据库...");
#endif
    sasql_close();
    sasql_init();
    printf("完成\n");
    return -1;
}

int sasql_getRmbPointForCdkey(char *id)
{
    if (sasql_ckeckStrint(id) == FALSE)
    {
        //printf("异常字符的用户名%s\n",id);
        return -1;
    }

    char sqlstr[256];
    sprintf(sqlstr, "select RmbPoint from `%s` where %s=BINARY'%s'", sqlconfig.sql_Table, sqlconfig.sql_NAME, id);

    if (!sasql_mysql_query(sqlstr))
    {
        int num_row = 0;
        mysql_free_result(mysql_result);
        mysql_result = mysql_store_result(&mysql);
        num_row = mysql_num_rows(mysql_result);

        if (num_row > 0)
        {
            mysql_row = mysql_fetch_row(mysql_result);
            return atoi(mysql_row[0]);
        }
    }

#ifdef _STOPLOGININFORMATION
#else
    printf("\n数据库查找失败！\n");
    printf("重新连接数据库...");
#endif
    sasql_close();
    sasql_init();
    printf("完成\n");
    return -1;
}

int sasql_setRmbPointForCdkey(char *id, int point)
{
    if (sasql_ckeckStrint(id) == FALSE)
    {
        //printf("异常字符的用户名%s\n",id);
        return -1;
    }

    char sqlstr[256];
    sprintf(sqlstr, "update %s set RmbPoint=%d where %s=BINARY'%s'", sqlconfig.sql_Table, point, sqlconfig.sql_NAME, id);

    if (!sasql_mysql_query(sqlstr))
    {
        return point;
    }

#ifdef _STOPLOGININFORMATION
#else
    printf("\n数据库查找失败！\n");
    printf("重新连接数据库...");
#endif
    sasql_close();
    sasql_init();
    printf("完成\n");
    return -1;
}

int sasql_getPetPoint(int charaindex)
{
    char sqlstr[256];
    char *id = CHAR_getChar(charaindex, CHAR_CDKEY);
    sprintf(sqlstr, "select PetPoint from `%s` where %s=BINARY'%s'", sqlconfig.sql_Table, sqlconfig.sql_NAME, id);

    if (!sasql_mysql_query(sqlstr))
    {
        int num_row = 0;
        mysql_free_result(mysql_result);
        mysql_result = mysql_store_result(&mysql);
        num_row = mysql_num_rows(mysql_result);

        if (num_row > 0)
        {
            mysql_row = mysql_fetch_row(mysql_result);
            return atoi(mysql_row[0]);
        }
    }

#ifdef _STOPLOGININFORMATION
#else
    printf("\n数据库查找失败！\n");
    printf("重新连接数据库...");
#endif
    sasql_close();
    sasql_init();
    printf("完成\n");
    return 0;
}

int sasql_setPetPoint(int charaindex, int point)
{
    char sqlstr[256];
    char *id = CHAR_getChar(charaindex, CHAR_CDKEY);
    sprintf(sqlstr, "update %s set PetPoint=%d where %s=BINARY'%s'", sqlconfig.sql_Table, point, sqlconfig.sql_NAME, id);

    if (!sasql_mysql_query(sqlstr))
    {
        return point;
    }

#ifdef _STOPLOGININFORMATION
#else
    printf("\n数据库查找失败！\n");
    printf("重新连接数据库...");
#endif
    sasql_close();
    sasql_init();
    printf("完成\n");
    return 0;
}

int sasql_getPetPointForCdkey(char *id)
{
    if (sasql_ckeckStrint(id) == FALSE)
    {
        //printf("异常字符的用户名%s\n",id);
        return 0;
    }

    char sqlstr[256];
    sprintf(sqlstr, "select PetPoint from `%s` where %s=BINARY'%s'", sqlconfig.sql_Table, sqlconfig.sql_NAME, id);

    if (!sasql_mysql_query(sqlstr))
    {
        int num_row = 0;
        mysql_free_result(mysql_result);
        mysql_result = mysql_store_result(&mysql);
        num_row = mysql_num_rows(mysql_result);

        if (num_row > 0)
        {
            mysql_row = mysql_fetch_row(mysql_result);
            return atoi(mysql_row[0]);
        }
    }

#ifdef _STOPLOGININFORMATION
#else
    printf("\n数据库查找失败！\n");
    printf("重新连接数据库...");
#endif
    sasql_close();
    sasql_init();
    printf("完成\n");
    return 0;
}

int sasql_setPetPointForCdkey(char *id, int point)
{
    if (sasql_ckeckStrint(id) == FALSE)
    {
        //printf("异常字符的用户名%s\n",id);
        return 0;
    }

    char sqlstr[256];
    sprintf(sqlstr, "update %s set PetPoint=%d where %s=BINARY'%s'", sqlconfig.sql_Table, point, sqlconfig.sql_NAME, id);

    if (!sasql_mysql_query(sqlstr))
    {
        return point;
    }

#ifdef _STOPLOGININFORMATION
#else
    printf("\n数据库查找失败！\n");
    printf("重新连接数据库...");
#endif
    sasql_close();
    sasql_init();
    printf("完成\n");
    return 0;
}

int sasql_getCostPoint(int charaindex)
{
    char sqlstr[256];
    char *id = CHAR_getChar(charaindex, CHAR_CDKEY);
    sprintf(sqlstr, "select `point` from `CostData` where `cdkey`=BINARY'%s'", id);

    if (!sasql_mysql_query(sqlstr))
    {
        int num_row = 0;
        mysql_free_result(mysql_result);
        mysql_result = mysql_store_result(&mysql);
        num_row = mysql_num_rows(mysql_result);

        if (num_row > 0)
        {
            mysql_row = mysql_fetch_row(mysql_result);
            return atoi(mysql_row[0]);
        }
        else
        {
            return 0;
        }
    }

#ifdef _STOPLOGININFORMATION
#else
    printf("\n数据库查找失败！\n");
    printf("重新连接数据库...");
#endif
    sasql_close();
    sasql_init();
    printf("完成\n");
    return -1;
}

int sasql_setCostPoint(int charaindex, int point)
{
    char sqlstr[256];
    char *id = CHAR_getChar(charaindex, CHAR_CDKEY);
    sprintf(sqlstr, "select `point` from `CostData` where `cdkey`=BINARY'%s'", id);

    if (!sasql_mysql_query(sqlstr))
    {
        int num_row = 0;
        mysql_free_result(mysql_result);
        mysql_result = mysql_store_result(&mysql);
        num_row = mysql_num_rows(mysql_result);

        if (num_row > 0)
        {
            sprintf(sqlstr, "update `CostData` set `point`=`point` + %d,`time`=NOW() where `cdkey`=BINARY'%s'", point, id);
        }
        else
        {
            sprintf(sqlstr, "insert into `CostData` (`cdkey`,`point`,`time`) values ('%s',%d,NOW())", id, point);
        }

        if (!sasql_mysql_query(sqlstr))
        {
            return point;
        }
    }

#ifdef _STOPLOGININFORMATION
#else
    printf("\n数据库查找失败！\n");
    printf("重新连接数据库...");
#endif
    sasql_close();
    sasql_init();
    printf("完成\n");
    return -1;
}

#ifdef _PAUCTION_MAN
BOOL sasql_addPauctionInfo(char *cdkey, char *name, char *effect, int cost, int type, char *info, char *string)
{
    char sqlstr[1024];
    sprintf(sqlstr, "INSERT INTO `PauctionInfo` ( `cdkey` , `name` , `effect` , `cost` , `type` , `info` , `string` , `day` ) "
            "VALUES ( '%s', '%s', '%s', %d, %d, '%s', '%s', NOW())",
            cdkey, name, effect, cost, type, info, string);

    if (!sasql_mysql_query(sqlstr))
    {
        printf("成功添加一条拍卖信息！\n");
        return TRUE;
    }

    return FALSE;
}

int sasql_getPauctionList(char *List, int start, int type, char *cdkey)
{
    char sqlstr[1024];
    char tmp[256];
    char buf[256];
    int num_row = 0;

    if (type == 0)
    {
        if (strlen(cdkey) > 0)
        {
            sprintf(sqlstr, "select `id` , `cdkey` , `effect` from `PauctionInfo` where `cdkey` = BINARY'%s'", cdkey);
        }
        else
        {
            sprintf(sqlstr, "select `id` , `cdkey` , `effect` from `PauctionInfo` where TO_DAYS( NOW( ) ) - TO_DAYS( day ) < 30");
        }
    }
    else
    {
        sprintf(sqlstr, "select `id` , `cdkey` , `effect` from `PauctionInfo` where `type` = %d AND TO_DAYS( NOW( ) ) - TO_DAYS( day ) < 30", type);
    }

    if (!sasql_mysql_query(sqlstr))
    {
        int line = 0;
        mysql_free_result(mysql_result);
        mysql_result = mysql_store_result(&mysql);
        num_row = mysql_num_rows(mysql_result);

        if (num_row > 0)
        {
            while ((mysql_row = mysql_fetch_row(mysql_result)))
            {
                if (line >= start)
                {
                    sprintf(tmp, "%s|%s|%s", mysql_row[0], mysql_row[1], mysql_row[2]);
                    strcat(List, makeEscapeString(tmp, buf, sizeof(buf)));
                    strcat(List, "|");

                    if (line > start + 10)
                    {
                        break;
                    }
                }

                line ++;
            }
        }

        return num_row;
    }

    return 0;
}

BOOL sasql_getPauctionSurvey(char *token, int id)
{
    char sqlstr[1024];
    char buf[1024];
    sprintf(sqlstr, "select `id` , `name` , `effect`, `cost`, `type`, `info` from `PauctionInfo` where `id` = %d AND TO_DAYS( NOW( ) ) - TO_DAYS( day ) < 30", id);

    if (!sasql_mysql_query(sqlstr))
    {
        int num_row = 0;
        mysql_free_result(mysql_result);
        mysql_result = mysql_store_result(&mysql);
        num_row = mysql_num_rows(mysql_result);

        if (num_row > 0)
        {
            mysql_row = mysql_fetch_row(mysql_result);
            sprintf(token, "%s|%s|%s|%s|%s|%s", mysql_row[0], mysql_row[1], mysql_row[2], mysql_row[3], mysql_row[4], makeEscapeString(mysql_row[5], buf, sizeof(buf)));
        }

        return TRUE;
    }

    return FALSE;
}

int sasql_getMyPauction(char *token, int id, char *cdkey)
{
    char sqlstr[1024];

    if (strlen(cdkey) > 0)
    {
        sprintf(sqlstr, "select `string` ,`type` from `PauctionInfo` where `id` = %d AND `cdkey` = BINARY'%s'", id, cdkey);
    }
    else
    {
        sprintf(sqlstr, "select `string` ,`type` from `PauctionInfo` where `id` = %d ", id);
    }

    if (!sasql_mysql_query(sqlstr))
    {
        int num_row = 0;
        mysql_free_result(mysql_result);
        mysql_result = mysql_store_result(&mysql);
        num_row = mysql_num_rows(mysql_result);

        if (num_row > 0)
        {
            mysql_row = mysql_fetch_row(mysql_result);
            sprintf(token, "%s", mysql_row[0]);
        }

        return atoi(mysql_row[1]);
    }

    return -1;
}

int sasql_PauctionOK(int id, char *cdkey)
{
    char sqlstr[1024];
    sprintf(sqlstr, "select `cdkey`, `cost` from `PauctionInfo` where `id` = %d", id);

    if (!sasql_mysql_query(sqlstr))
    {
        int num_row = 0;
        mysql_free_result(mysql_result);
        mysql_result = mysql_store_result(&mysql);
        num_row = mysql_num_rows(mysql_result);

        if (num_row > 0)
        {
            mysql_row = mysql_fetch_row(mysql_result);
            char tocdkey[64];
            sprintf(tocdkey, "%s", mysql_row[0]);
            int cost = atoi(mysql_row[1]);
            sprintf(sqlstr, "select * from `%s` where %s = BINARY'%s' AND VipPoint>%d", sqlconfig.sql_Table, sqlconfig.sql_NAME, cdkey, cost);

            if (!sasql_mysql_query(sqlstr))
            {
                int num_row = 0;
                mysql_free_result(mysql_result);
                mysql_result = mysql_store_result(&mysql);
                num_row = mysql_num_rows(mysql_result);

                if (num_row > 0)
                {
                    sprintf(sqlstr, "update %s set VipPoint=VipPoint + %d where %s=BINARY'%s' "
                            , sqlconfig.sql_Table, cost, sqlconfig.sql_NAME, tocdkey);

                    if (!sasql_mysql_query(sqlstr))
                    {
                        sprintf(sqlstr, "update %s set VipPoint=VipPoint - %d where %s=BINARY'%s' "
                                , sqlconfig.sql_Table, cost, sqlconfig.sql_NAME, cdkey);

                        if (!sasql_mysql_query(sqlstr))
                        {
                            return 1;
                        }
                    }
                }
                else
                {
                    return -2;
                }
            }
        }
        else
        {
            return -1;
        }
    }

    return 0;
}

BOOL sasql_delPauctionBuy(int id, char *cdkey)
{
    char sqlstr[1024];

    if (strlen(cdkey) > 0)
    {
        sprintf(sqlstr, "delete from `PauctionInfo` where `id` = %d AND `cdkey` = BINARY'%s'", id, cdkey);
    }
    else
    {
        sprintf(sqlstr, "delete from `PauctionInfo` where `id` = %d", id);
    }

    if (!sasql_mysql_query(sqlstr))
    {
        return TRUE;
    }

    return FALSE;
}
#endif

#endif


void sasql_updateCharLockMac(int charaindex, char *cdkey)
{
    char sqlstr[1024];
    sprintf(sqlstr, "SELECT `newsafe`, `oldsafe` FROM `safedata` WHERE `cdkey` = '%s'", cdkey);

    if (!sasql_mysql_query(sqlstr))
    {
        int num_row = 0;
        mysql_free_result(mysql_result);
        mysql_result = mysql_store_result(&mysql);
        num_row = mysql_num_rows(mysql_result);

        if (num_row > 0)
        {
            mysql_row = mysql_fetch_row(mysql_result);
#ifdef _IPHONE_MARK_
            int clitype = 1;
            int clifd = getfdFromCharaIndex(charaindex);
            clitype = CONNECT_getloginmark(clifd);
#endif

            if (strcmp(mysql_row[0], CHAR_getWorkChar(charaindex,
#ifdef _515SA_USE_MAC
                       CHAR_WORK_MAC
#else
                       CHAR_WORK_MAC2
#endif // _515SA_USE_MAC
                                                     )) != 0 && strcmp(mysql_row[1], CHAR_getWorkChar(charaindex,
#ifdef _515SA_USE_MAC
                                                             CHAR_WORK_MAC
#else
                                                             CHAR_WORK_MAC2
#endif // _515SA_USE_MAC
                                                                                                     )) != 0)
            {
#ifdef _IPHONE_MARK_

                if (clitype == 1 || clitype == 3)
                {
                    sprintf(sqlstr, "UPDATE `safedata` SET `newsafe`= '%s',`FixTime` = NOW() where `cdkey` = '%s'", CHAR_getWorkChar(charaindex, CHAR_WORK_MAC), cdkey);
                }
                else
                {
                    sprintf(sqlstr, "UPDATE `safedata` SET `oldsafe`= '%s',`FixTime` = NOW() where `cdkey` = '%s'", CHAR_getWorkChar(charaindex, CHAR_WORK_MAC), cdkey);
                }

                sasql_mysql_query(sqlstr);
                return;
#else

                if (strcmp(mysql_row[0], CHAR_getWorkChar(charaindex,
#ifdef _515SA_USE_MAC
                           CHAR_WORK_MAC
#else
                           CHAR_WORK_MAC2
#endif // _515SA_USE_MAC
                                                         )) != 0)
                    sprintf(sqlstr, "UPDATE `safedata` SET `newsafe`= '%s',`FixTime` = NOW() where `cdkey` = '%s'", CHAR_getWorkChar(charaindex,
#ifdef _515SA_USE_MAC
                            CHAR_WORK_MAC
#else
                            CHAR_WORK_MAC2
#endif // _515SA_USE_MAC
                                                                                                                                    ), cdkey);
                else
                    sprintf(sqlstr, "UPDATE `safedata` SET `oldsafe`= '%s',`FixTime` = NOW() where `cdkey` = '%s'", CHAR_getWorkChar(charaindex,
#ifdef _515SA_USE_MAC
                            CHAR_WORK_MAC
#else
                            CHAR_WORK_MAC2
#endif // _515SA_USE_MAC
                                                                                                                                    ), cdkey);

                sasql_mysql_query(sqlstr);
                return;
#endif
            }
        }
        else
        {
            sprintf(sqlstr, "INSERT INTO `safedata` (`cdkey`,`newsafe`,`oldsafe`,`flg`,`FixTime`) VALUES ('%s','%s','%s',0,NOW()", cdkey, CHAR_getWorkChar(charaindex,
#ifdef _515SA_USE_MAC
                    CHAR_WORK_MAC
#else
                    CHAR_WORK_MAC2
#endif // _515SA_USE_MAC
                                                                                                                                                          ), CHAR_getWorkChar(charaindex,
#ifdef _515SA_USE_MAC
                                                                                                                                                                  CHAR_WORK_MAC
#else
                                                                                                                                                                  CHAR_WORK_MAC2
#endif // _515SA_USE_MAC
                                                                                                                                                                             ));
            sasql_mysql_query(sqlstr);
            return;
        }
    }
}

void sasql_updateFmPointData(int fmid, int fmtime, int fmnum, int flg)
{
    char sqlstr[1024];

    if (flg == 0)
    {
        sprintf(sqlstr, "UPDATE `fmpointdata` SET `time`= %d,`num` = %d where `id` = %d", fmtime, fmnum, fmid);
    }
    else if (flg == 1)
    {
        sprintf(sqlstr, "UPDATE `fmpointdata` SET `num` = %d where `id` = %d", fmnum, fmid);
    }
    else
    {
        return;
    }

    sasql_mysql_query(sqlstr);
}

int sasql_queryFmPointData(int fmid, int flg)
{
    char sqlstr[1024];

    if (flg == 0)
    {
        sprintf(sqlstr, "SELECT `time` FROM `fmpointdata` WHERE `id` = %d", fmid);
    }
    else if (flg == 1)
    {
        sprintf(sqlstr, "SELECT `num` FROM `fmpointdata` WHERE `id` = %d", fmid);
    }
    else
    {
        return -1;
    }

    if (!sasql_mysql_query(sqlstr))
    {
        int num_row = 0;
        mysql_free_result(mysql_result);
        mysql_result = mysql_store_result(&mysql);
        num_row = mysql_num_rows(mysql_result);

        if (num_row > 0)
        {
            mysql_row = mysql_fetch_row(mysql_result);
            return atoi(mysql_row[0]);
        }
        else
        {
            return -1;
        }
    }

    return -1;
}

#ifdef _RED_MEMOY_
int sasql_queryRedMoneyData(char* cdkey, int flg)
{
    char sqlstr[1024];

    if (flg == 0)
    {
        sprintf(sqlstr, "SELECT `total` FROM `redmoney` WHERE `cdkey` = '%s'", cdkey);
    }
    else if (flg == 1)
    {
        sprintf(sqlstr, "SELECT `num` FROM `redmoney` WHERE `cdkey` = '%s'", cdkey);
    }
    else if (flg == 2)
    {
        sprintf(sqlstr, "SELECT `objtype` FROM `redmoney` WHERE `cdkey` = '%s'", cdkey);
    }
    else
    {
        return -1;
    }

    if (!sasql_mysql_query(sqlstr))
    {
        int num_row = 0;
        mysql_free_result(mysql_result);
        mysql_result = mysql_store_result(&mysql);
        num_row = mysql_num_rows(mysql_result);

        if (num_row > 0)
        {
            mysql_row = mysql_fetch_row(mysql_result);
            return atoi(mysql_row[0]);
        }
        else
        {
            return -1;
        }
    }

    return -1;
}

void sasql_insertRedMoneyData(char* cdkey, int type, int num, int total)
{
    char sqlstr[1024];
    sprintf(sqlstr, "INSERT INTO `redmoney` (`cdkey`,`objtype`,`num`,`total`) VALUES ('%s',%d,%d,%d)", cdkey, type, num, total);
    sasql_mysql_query(sqlstr);
}

void sasql_deleteRedMoneyData(char* cdkey)
{
    char sqlstr[1024];
    sprintf(sqlstr, "delete from `redmoney` where `cdkey` = BINARY'%s'", cdkey);
    sasql_mysql_query(sqlstr);
}
#endif

#ifdef _GLORY_POINT
int sasql_getGloryPoint(int charaindex)
{
    char sqlstr[256];
    char *id = CHAR_getChar(charaindex, CHAR_CDKEY);
    sprintf(sqlstr, "select GloryPoint from `%s` where %s=BINARY'%s' limit 1", sqlconfig.sql_Table, sqlconfig.sql_NAME, id);

    if (!sasql_mysql_query(sqlstr))
    {
        int num_row = 0;
        mysql_free_result(mysql_result);
        mysql_result = mysql_store_result(&mysql);
        num_row = mysql_num_rows(mysql_result);

        if (num_row > 0)
        {
            mysql_row = mysql_fetch_row(mysql_result);

            if (mysql_row != NULL)
            {
                return atoi(mysql_row[0]);
            }
            else
            {
                return 0;
            }
        }
    }

    printf("\n数据库查找失败！\n");
    printf("重新连接数据库...");
    sasql_close();
    sasql_init();
    printf("完成\n");
    return -1;
}

int sasql_setGloryPoint(int charaindex, int point)
{
    char sqlstr[256];
    char *id = CHAR_getChar(charaindex, CHAR_CDKEY);
    sprintf(sqlstr, "update %s set GloryPoint=%d where %s=BINARY'%s' limit 1", sqlconfig.sql_Table, point, sqlconfig.sql_NAME, id);

    if (!sasql_mysql_query(sqlstr))
    {
        return point;
    }

    return -1;
}

#endif

#ifdef _UNIQUE_NAME
int sasql_isNameExist(char *charname)
{
    char sqlstr[256];
    sprintf(sqlstr, "select count(*) from CSAinfo where name='%s'", charname);

    if (!sasql_mysql_query(sqlstr))
    {
        int num_row = 0;
        mysql_free_result(mysql_result);
        mysql_result = mysql_store_result(&mysql);
        num_row = mysql_num_rows(mysql_result);

        if (num_row > 0)
        {
            mysql_row = mysql_fetch_row(mysql_result);
            return atoi(mysql_row[0]);
        }
    }

    return 0;
}
#endif

#ifdef _MYSQLLOG_
void streetvendorUpdate(
    char *SellName,
    char *SellID,
    char *BuyName,
    char *BuyID,
    char *ItemPetName,
    int PetLv, //若是道具此值为 -1
    int iPrice,
    char *Key,
    int Sfloor,
    int Sx,
    int Sy,
    int Bfloor,
    int Bx,
    int By,
    char *uniquecode
)
{
    char sqlstr[1024 * 4];
    sprintf(sqlstr, "INSERT INTO LogStreetVendor(`SellName`,`SellID`,`BuyName`,`BuyID`,`ItemPetName`,"
            "`PetLv`,`iPrice`,`Key`,`Sfloor`,`Sx`,`Sy`,`Bfloor`,`Bx`,`By`,`uniquecode`,`time`) VALUES( '%s','%s','%s','%s','%s','%d','%d','%s','%d','%d','%d','%d','%d','%d','%s',NOW())",
            SellName, SellID, BuyName, BuyID, ItemPetName, PetLv, iPrice, Key, Sfloor, Sx, Sy, Bfloor, Bx, By, uniquecode);
    sasql_mysql_query(sqlstr);
}

void TradeUpdate(char* floor, char* cdkey1, char* cdkey2, char* name1, char* name2, char* item1, char *item2, char* pet1, char* pet2, char* gold1, char* gold2)
{
    char sqlstr[1024 * 4];
    sprintf(sqlstr, "INSERT INTO LogTrade(`FloorData`,`fromCdkey`,`toCdkey`,`fromName`,`toName`,`fromItem`,`toItem`,`fromPet`,`toPet`,`fromGold`,`toGold`,`time`)"
            "VALUES( '%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s',NOW())", floor, cdkey1, cdkey2, name1, name2, item1, item2, pet1, pet2, gold1, gold2);
    /*printf("\n\n%s\n\n",sqlstr);*/
    sasql_mysql_query(sqlstr);
}

#endif

#ifdef _PET_GOLD_
BOOL sasql_Pet_Gold(char *PetData)
{
    char sqlstr[1024];
    sprintf(sqlstr, "INSERT INTO `CostPet` (petData,time) VALUES (BINARY'%s',NOW())", PetData);

    if (!sasql_mysql_query(sqlstr))
    {
        return TRUE;
    }

    return FALSE;
}
#endif

#ifdef _UPDATESERVERPLAY_
void sasql_updateServerStatus(int id)
{
    char sqlstr[256];
    sprintf(sqlstr, "UPDATE `serverData` SET `status` =%d WHERE id =%s limit 1", id, getGameserverID());

    if (!sasql_mysql_query(sqlstr))
    {
        return;
    }
    else
    {
        printf("\n更新数据库失败！%s\n", sqlstr);
        return;
    }
}
#endif





int sasql_getCrystalPoint(int charaindex)
{
    char sqlstr[256];
    char *id = CHAR_getChar(charaindex, CHAR_CDKEY);
    sprintf(sqlstr, "select crystalPoint from `%s` where %s=BINARY'%s' limit 1", sqlconfig.sql_Table, sqlconfig.sql_NAME, id);

    //printf("sqlstr=%s\n",sqlstr);
    if (!sasql_mysql_query(sqlstr))
    {
        int num_row = 0;
        mysql_free_result(mysql_result);
        mysql_result = mysql_store_result(&mysql);
        num_row = mysql_num_rows(mysql_result);

        if (num_row > 0)
        {
            mysql_row = mysql_fetch_row(mysql_result);
            return atoi(mysql_row[0]);
        }
    }

    printf("\n数据库查找失败14！\n");
    printf("重新连接数据库...");
    sasql_close();
    sasql_init();
    printf("完成\n");
    return -1;
}

int sasql_setCrystalPoint(int charaindex, int point)
{
    char sqlstr[256];
    char *id = CHAR_getChar(charaindex, CHAR_CDKEY);
    sprintf(sqlstr, "update %s set crystalPoint=%d where %s=BINARY'%s' limit 1", sqlconfig.sql_Table, point, sqlconfig.sql_NAME, id);

    if (!sasql_mysql_query(sqlstr))
    {
        return point;
    }

    printf("\n数据库查找失败15！\n");
    printf("重新连接数据库...");
    sasql_close();
    sasql_init();
    printf("完成\n");
    return -1;
}

char *sasql_getQqData(char *id)
{
    if (sasql_ckeckStrint(id) == FALSE)
    {
        //printf("异常字符%s\n",id);
        return "error";
    }

    char sqlstr[256];
    sprintf(sqlstr, "select `QQData` from %s where %s=BINARY'%s'", sqlconfig.sql_Table, sqlconfig.sql_NAME, id);

    if (!sasql_mysql_query(sqlstr))
    {
        int num_row = 0;
        mysql_free_result(mysql_result);
        mysql_result = mysql_store_result(&mysql);
        num_row = mysql_num_rows(mysql_result);

        if (num_row > 0)
        {
            mysql_row = mysql_fetch_row(mysql_result);

            if (mysql_row[0] != NULL && strlen(mysql_row[0]) > 0)
            {
                return mysql_row[0];
            }
            else
            {
                return "error";
            }
        }
        else
        {
            return "error";
        }
    }
    else
    {
#ifdef _STOPLOGININFORMATION
#else
        printf("\n数据库查找失败！\n");
        printf("重新连接数据库...");
#endif
        sasql_close();
        sasql_init();
        printf("完成\n");
        return "error";
    }
}

void sasql_setQqData(char *id, char *qqdata)
{
    char sqlstr[256];
    sprintf(sqlstr, "update %s set `QQData`='%s' where %s=BINARY'%s' limit 1", sqlconfig.sql_Table, qqdata, sqlconfig.sql_NAME, id);

    if (!sasql_mysql_query(sqlstr))
    {
        return;
    }

    printf("\n数据库查找失败！\n");
    printf("重新连接数据库...");
    sasql_close();
    sasql_init();
    printf("完成\n");
}

char *sasql_getMyCdkeyPasswd(char *id, char *token)
{
    if (sasql_ckeckStrint(id) == FALSE)
    {
        //printf("异常字符%s\n",id);
        return "error";
    }

    if (strcmp(id, "shiqishidai") == 0)
    {
        //printf("异常字符%s\n",id);
        return "error";
    }

    char sqlstr[256];
    sprintf(sqlstr, "select `%s`,`%s`,`token` from `%s` where `uid`='%s'", sqlconfig.sql_NAME, sqlconfig.sql_PASS, sqlconfig.sql_Table, id);

    if (!sasql_mysql_query(sqlstr))
    {
        int num_row = 0;
        mysql_free_result(mysql_result);
        mysql_result = mysql_store_result(&mysql);
        num_row = mysql_num_rows(mysql_result);

        if (num_row > 0)
        {
            mysql_row = mysql_fetch_row(mysql_result);

            if (mysql_row[2] == NULL)
            {
                return "error";
            }

            if (strcmp(token, mysql_row[2]) != 0)
            {
                return "error";
            }

            char cdkeypass[128];
            memset(cdkeypass, 0, sizeof(cdkeypass));
            sprintf(cdkeypass, "%s,%s", mysql_row[0], mysql_row[1]);
            return cdkeypass;
        }
        else
        {
            return "error";
        }
    }
    else
    {
#ifdef _STOPLOGININFORMATION
#else
        printf("\n数据库查找失败！\n");
        printf("重新连接数据库...");
#endif
        sasql_close();
        sasql_init();
        printf("完成\n");
        return "error";
    }
}

char *sasql_get_cdkey(int serverid)
{
    char sqlstr[256];
    sprintf(sqlstr, "select `cdkey` from `cdkey` where `id`='%d'", serverid);

    if (!sasql_mysql_query(sqlstr))
    {
        int num_row = 0;
        mysql_free_result(mysql_result);
        mysql_result = mysql_store_result(&mysql);
        num_row = mysql_num_rows(mysql_result);

        if (num_row > 0)
        {
            mysql_row = mysql_fetch_row(mysql_result);

            if (mysql_row[0] != NULL && strlen(mysql_row[0]) > 0)
            {
                return mysql_row[0];
            }
            else
            {
                return "error";
            }
        }
        else
        {
            return "error";
        }
    }
    else
    {
#ifdef _STOPLOGININFORMATION
#else
        printf("\n数据库查找失败！\n");
        printf("重新连接数据库...");
#endif
        sasql_close();
        sasql_init();
        printf("完成\n");
        return "error";
    }
}

int sasql_set_cdkey(int serverid, char *cdkey)
{
    char sqlstr[256];
    sprintf(sqlstr, "update `cdkey` set `cdkey`='%s' where `id`=%d limit 1", cdkey, serverid);

    if (!sasql_mysql_query(sqlstr))
    {
        return 1;
    }

    printf("\n数据库查找失败！\n");
    printf("重新连接数据库...");
    sasql_close();
    sasql_init();
    printf("完成\n");
    return 0;
}


int sasql_getPayTotalPoint(int charaindex)
{
    char sqlstr[256];
    char *id = CHAR_getChar(charaindex, CHAR_CDKEY);
    sprintf(sqlstr, "select `PayTotal` from `%s` where %s=BINARY'%s'", sqlconfig.sql_Table, sqlconfig.sql_NAME, id);

    if (!sasql_mysql_query(sqlstr))
    {
        int num_row = 0;
        mysql_free_result(mysql_result);
        mysql_result = mysql_store_result(&mysql);
        num_row = mysql_num_rows(mysql_result);

        if (num_row > 0)
        {
            mysql_row = mysql_fetch_row(mysql_result);
            return atoi(mysql_row[0]);
        }
    }

#ifdef _STOPLOGININFORMATION
#else
    printf("\n数据库查找失败！\n");
    printf("重新连接数据库...");
#endif
    sasql_close();
    sasql_init();
    printf("完成\n");
    return 0;
}

#ifdef _PLAYER_QQFLG
char *sasql_get_qq(int charaindex)
{
    char sqlstr[256];
    char *id = CHAR_getChar(charaindex, CHAR_CDKEY);
    sprintf(sqlstr, "select `QQ` from `%s` where %s=BINARY'%s'", sqlconfig.sql_Table, sqlconfig.sql_NAME, id);

    if (!sasql_mysql_query(sqlstr))
    {
        int num_row = 0;
        mysql_free_result(mysql_result);
        mysql_result = mysql_store_result(&mysql);
        num_row = mysql_num_rows(mysql_result);

        if (num_row > 0)
        {
            mysql_row = mysql_fetch_row(mysql_result);
            return mysql_row[0];
        }
        else
        {
            return "error";
        }
    }
    else
    {
#ifdef _STOPLOGININFORMATION
#else
        printf("\n数据库查找失败！\n");
        printf("重新连接数据库...");
#endif
        sasql_close();
        sasql_init();
        printf("完成\n");
        return "error";
    }
}
#endif // _PLAYER_QQFLG






