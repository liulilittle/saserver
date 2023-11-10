#include "version.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "common.h"
#include "util.h"
#include "lssproto_serv.h"
#include "saacproto_cli.h"
#include "net.h"
#include "char.h"
#include "char_talk.h"
#include "object.h"
#include "readmap.h"
#include "addressbook.h"
#include "handletime.h"
#include "configfile.h"
#include "event.h"
#include "pet.h"
#include "battle.h"
#include "battle_command.h"
#include "magic.h"
#include "petmail.h"
#include "item_gen.h"
#include "pet_skill.h"
#include "log.h"  //add this because the second had it
#include "map_deal.h" // CoolFish: 2001/4/18
#include "trade.h" // CoolFish: Trade 2001/4/18
#include "family.h" // CoolFish: Family 2001/5/24
#include "item_event.h" // shan: blackmarket
#ifdef _AUTO_PK
#include "npc_autopk.h"
#endif
#include "npc_healer.h"
#ifdef _PROFESSION_SKILL            // WON ADD 人物职业技能
#include "profession_skill.h"
#endif
#ifdef _CHATROOMPROTOCOL            // (不可开) Syu ADD 聊天室频道
#include "chatroom.h"
#endif
#ifdef _ONLINE_SHOP
#include "longzoro/newshop.h"
#endif
#ifdef _ALLBLUES_LUA
#include "mylua/function.h"
#endif
#ifdef _SERVER_NP_
#include "ggsrv30.h"
#endif
#include "longzoro/sasql.h"
#include "magic_base.h"

// shan add
extern struct FM_PKFLOOR fmpkflnum[FAMILY_FMPKFLOOR];
int flybus[128] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
                    , -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
                    , -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
                    , -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
                    , -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
                    , -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
                    , -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
                    , -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
                    , -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
                    , -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
                    , -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
                    , -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
                    - 1, -1, -1, -1, -1, -1, -1, -1
                  };

/* -----------------------------------------------------------------------
 * 联买した筛弄の戎规からキャラインデックスを评る
 * ----------------------------------------------------------------------*/
static int Callfromcli_Util_getTargetCharaindex(int fd, int toindex)
{
    int to_charaindex = -1;
    int fd_charaindex = CONNECT_getCharaindex(fd);

    /* 极尸极咳 */
    if (toindex == 0)
    {
        to_charaindex = fd_charaindex;
    }
    /* ペット 1 ×5 */
    else if (toindex > 0 && toindex < 6)
    {
        to_charaindex = CHAR_getCharPet(fd_charaindex, toindex - 1);

        if (!CHAR_CHECKINDEX(to_charaindex))
        {
            to_charaindex = -1;
        }
    }
    /* 苗粗 6 ×10 */
    else if (toindex > 5 && toindex < 11)
    {
        to_charaindex = CHAR_getPartyIndex(fd_charaindex, toindex - 6);
    }

    return to_charaindex;
}
/*----------------------------------------
 * クライアントがログインする でもメモリに委めるだけなのでチェックはない
 * これを钙ぶと CLI になる。
 ----------------------------------------*/
void lssproto_ClientLogin_recv(int fd, char* cdkey, char* passwd, char* mac1, char* mac2, int servid
#ifdef _NEWCLIENTLOGIN
                               , int logintime, char* md5buff, char* macbuff
#endif
                              )
{
    if (CONNECT_getState(fd) == NULLCONNECT)
    {
        CONNECT_setState(fd, NOTLOGIN);
    }

    /*  2脚にこれが钙ばれるのはいい    */
    /*  パスワ〖ド恃构をして喇根だったら浩びこれを钙ぶ祸。*/
    {
        //ttom avoid the restore 2001/01/09
        if (CONNECT_isNOTLOGIN(fd) == FALSE)
        {
            print("\n the Client had  Logined fd=%d", fd);
            return;
        }
    }

    if (checkStringErr(cdkey))
    {
        return;
    }

    if (strlen(mac1) > 128)
    {
        printf("\n %s 的mac1 超出长度", cdkey);
        return;
    }

    if (strlen(mac2) > 128)
    {
        printf("\n %s 的mac2 超出长度", cdkey);
        return;
    }

    if (checkStringErr(passwd))
    {
        return;
    }

    if (strlen(cdkey) < 1 || strlen(passwd) < 1)
    {
        lssproto_ClientLogin_send(fd, "账号或密码不得为空。");
        CONNECT_setCloseRequest(fd, 1);
        return;
    }

    //print( "CliLogin cdkey=%s\n" , cdkey );
    /* connectにコピ〖する */
    CONNECT_setCdkey(fd, cdkey);
    CONNECT_setPasswd(fd, passwd);
    CONNECT_setCtype(fd, CLI);
#ifdef _NEWCLISETSERVID
    CONNECT_setServid(fd, servid);
#endif
#ifdef _NEWCLISETMAC

    if (checkStringErr(mac1))
    {
        strcpy(mac1, "");
    }

    if (checkStringErr(mac2))
    {
        strcpy(mac2, "");
    }

    CONNECT_setMAC(fd, mac1);
    CONNECT_setMAC2(fd, mac2);
    CONNECT_setMAC3(fd, "");
#endif
    char ip[32];
    {
        unsigned long tmpip;
        int a, b, c, d;
        tmpip = CONNECT_get_userip(fd);
        a = (tmpip % 0x100);
        tmpip = tmpip / 0x100;
        b = (tmpip % 0x100);
        tmpip = tmpip / 0x100;
        c = (tmpip % 0x100);
        tmpip = tmpip / 0x100;
        d = (tmpip % 0x100);
        sprintf(ip, "%d.%d.%d.%d", a, b, c, d);
    }
#ifdef _STOPLOGININFORMATION
    print("\n登陆账号=%s \n", cdkey);
#else
    print("\n登陆账号=%s 密码=%s IP=%s MAC1=%s MAC2=%s SERVID=%d\n", cdkey, passwd, ip, mac1, mac2, servid);
#endif

    if (sasql_chehk_lock(cdkey))
    {
        lssproto_ClientLogin_send(fd, "您的游戏账号已被锁定");
        CONNECT_setCloseRequest(fd, 1);
        return;
    }

    if (sasql_chehk_lock(ip) || sasql_chehk_lock(mac1))
    {
        lssproto_ClientLogin_send(fd, "您的IP已被锁定");
        CONNECT_setCloseRequest(fd, 1);
        return;
    }

    int res = sasql_query(cdkey, passwd
                         );

    if (res == 2)
    {
        lssproto_ClientLogin_send(fd, "您的密码输入错误");
        CONNECT_setCloseRequest(fd, 1);
        return;
    }
    else if (res == 4)
    {
        lssproto_ClientLogin_send(fd, "异常字符的用户名");
        CONNECT_setCloseRequest(fd, 1);
        return;
    }
    else if (res == 3)
    {
#ifdef _UNIFIDE_MALINASI_2
        lssproto_ClientLogin_send(fd, "如无法登陆，请退出游戏，重新打开游戏登陆。");
#else
        lssproto_ClientLogin_send(fd, "您的账号尚未注册");
#endif // _UNIFIDE_MALINASI_2
        CONNECT_setCloseRequest(fd, 1);
        return;
    }
    else if (res == 1)
    {
#ifdef _ALLBLUES_LUA_1_9
#ifdef _NEWCLISETMAC

        if (FreeLoginCheck(fd, cdkey, passwd, ip, mac1, mac2, ""
#ifdef _NEWCLIENTLOGIN
                           , logintime, md5buff, macbuff, 1
#endif
                          ) == 0)
        {
            CONNECT_setCloseRequest(fd, 1);
            saacproto_ACKick_send(acfd, cdkey, -1, 1);
            return;
        }

#endif
#endif
        lssproto_ClientLogin_send(fd, "ok");
        sasql_online(cdkey, NULL, ip, mac1, mac2, "", 1);
    }

    /*
    #ifdef _NEWCLISETMAC
        saacproto_ACCharLogin_send( acfd, fd, cdkey, passwd, ip, mac1 ,mac2,mac3);
    #else
        saacproto_ACCharLogin_send( acfd, fd, cdkey, passwd, ip);
    #endif
    */
}
#ifdef _NEWCLIENTLOGIN
void lssproto_NewClientLogin_recv(int fd, char* cdkey, char* passwd, char* mac1, char* mac2, int servid, int logintime, char* md5buff)
{
    if (CONNECT_getState(fd) == NULLCONNECT)
    {
        CONNECT_setState(fd, NOTLOGIN);
    }

    /*  2脚にこれが钙ばれるのはいい    */
    /*  パスワ〖ド恃构をして喇根だったら浩びこれを钙ぶ祸。*/
    {
        //ttom avoid the restore 2001/01/09
        if (CONNECT_isNOTLOGIN(fd) == FALSE)
        {
            print("\n the Client had  Logined fd=%d", fd);
            return;
        }
    }

    if (checkStringErr(cdkey))
    {
        return;
    }

    if (strlen(mac1) > 128)
    {
        printf("\n %s 的mac1 超出长度", cdkey);
        return;
    }

    if (strlen(mac2) > 128)
    {
        printf("\n %s 的mac2 超出长度", cdkey);
        return;
    }

    if (checkStringErr(passwd))
    {
        return;
    }

    if (strlen(cdkey) < 1 || strlen(passwd) < 1)
    {
        lssproto_ClientLogin_send(fd, "账号或密码不得为空。");
        CONNECT_setCloseRequest(fd, 1);
        return;
    }

    //print( "CliLogin cdkey=%s\n" , cdkey );
    /* connectにコピ〖する */
    CONNECT_setCdkey(fd, cdkey);
    CONNECT_setPasswd(fd, passwd);
    CONNECT_setCtype(fd, CLI);
#ifdef _NEWCLISETSERVID
    CONNECT_setServid(fd, servid);
#endif
#ifdef _NEWCLISETMAC

    if (checkStringErr(mac1))
    {
        strcpy(mac1, "");
    }

    if (checkStringErr(mac2))
    {
        strcpy(mac2, "");
    }

    CONNECT_setMAC(fd, mac1);
    CONNECT_setMAC2(fd, mac2);
    CONNECT_setMAC3(fd, "");
#endif
    char ip[32];
    {
        unsigned long tmpip;
        int a, b, c, d;
        tmpip = CONNECT_get_userip(fd);
        a = (tmpip % 0x100);
        tmpip = tmpip / 0x100;
        b = (tmpip % 0x100);
        tmpip = tmpip / 0x100;
        c = (tmpip % 0x100);
        tmpip = tmpip / 0x100;
        d = (tmpip % 0x100);
        sprintf(ip, "%d.%d.%d.%d", a, b, c, d);
    }
#ifdef _STOPLOGININFORMATION
    print("\n登陆账号=%s \n", cdkey);
#else
    print("\n登陆账号=%s 密码=%s IP=%s MAC1=%s MAC2=%s SERVID=%d\n", cdkey, passwd, ip, mac1, mac2, servid);
#endif

    if (sasql_chehk_lock(cdkey))
    {
        lssproto_ClientLogin_send(fd, "您的游戏账号已被锁定");
        CONNECT_setCloseRequest(fd, 1);
        return;
    }

    if (sasql_chehk_lock(ip) || sasql_chehk_lock(mac1))
    {
        lssproto_ClientLogin_send(fd, "您的IP已被锁定");
        CONNECT_setCloseRequest(fd, 1);
        return;
    }

    int res = sasql_query(cdkey, passwd
                         );

    if (res == 2)
    {
        lssproto_ClientLogin_send(fd, "您的密码输入错误");
        CONNECT_setCloseRequest(fd, 1);
        return;
    }
    else if (res == 4)
    {
        lssproto_ClientLogin_send(fd, "异常字符的用户名");
        CONNECT_setCloseRequest(fd, 1);
        return;
    }
    else if (res == 3)
    {
#ifdef _UNIFIDE_MALINASI_2
        lssproto_ClientLogin_send(fd, "如无法登陆，请退出游戏，重新打开游戏登陆。");
#else
        lssproto_ClientLogin_send(fd, "您的账号尚未注册");
#endif // _UNIFIDE_MALINASI_2
        CONNECT_setCloseRequest(fd, 1);
        return;
    }
    else if (res == 1)
    {
#ifdef _ALLBLUES_LUA_1_9
#ifdef _NEWCLISETMAC

        if (FreeLoginCheck(fd, cdkey, passwd, ip, mac1, mac2, "", logintime, md5buff, "", 2) == 0)
        {
            CONNECT_setCloseRequest(fd, 1);
            saacproto_ACKick_send(acfd, cdkey, -1, 1);
            return;
        }

#endif
#endif
        lssproto_ClientLogin_send(fd, "ok");
        sasql_online(cdkey, NULL, ip, mac1, mac2, "", 1);
    }

    /*
    #ifdef _NEWCLISETMAC
    saacproto_ACCharLogin_send( acfd, fd, cdkey, passwd, ip, mac1 ,mac2,mac3);
    #else
    saacproto_ACCharLogin_send( acfd, fd, cdkey, passwd, ip);
    #endif
    */
}
#endif
void lssproto_QQLogin_recv(int fd, int qdtype, char* uid, char* token, char* mac1, char* mac2, char* mac3, int servid)
{
    if (CONNECT_getState(fd) == NULLCONNECT)
    {
        CONNECT_setState(fd, NOTLOGIN);
    }

    /*  2脚にこれが钙ばれるのはいい    */
    /*  パスワ〖ド恃构をして喇根だったら浩びこれを钙ぶ祸。*/
    {
        //ttom avoid the restore 2001/01/09
        if (CONNECT_isNOTLOGIN(fd) == FALSE)
        {
            print("\n the Client had  Logined fd=%d", fd);
            return;
        }
    }

    if (checkStringErr(uid))
    {
        return;
    }

    if (strlen(mac1) > 128)
    {
        return;
    }

    if (strlen(mac2) > 128)
    {
        return;
    }

    if (strlen(mac3) > 128)
    {
        return;
    }

    char ip[32];
    unsigned long tmpip;
    int a, b, c, d;
    tmpip = CONNECT_get_userip(fd);
    a = (tmpip % 0x100);
    tmpip = tmpip / 0x100;
    b = (tmpip % 0x100);
    tmpip = tmpip / 0x100;
    c = (tmpip % 0x100);
    tmpip = tmpip / 0x100;
    d = (tmpip % 0x100);
    sprintf(ip, "%d.%d.%d.%d", a, b, c, d);
    //print( "CliLogin cdkey=%s\n" , cdkey );
    /* connectにコピ〖する */
    char cdkey[CDKEYLEN];
    char passwd[PASSWDLEN];
    memset(cdkey, 0, sizeof(cdkey));
    memset(passwd, 0, sizeof(passwd));
    char myuid[64];
    memset(myuid, 0, sizeof(myuid));

    if (qdtype == 1)
    {
        sprintf(myuid, "vivo%s", uid);
    }
    else if (qdtype == 2)
    {
        sprintf(myuid, "oppo%s", uid);
    }
    else if (qdtype == 3)
    {
        sprintf(myuid, "U8-%s", uid);
    }
    else if (qdtype == 4)
    {
        sprintf(myuid, "MT-%s", uid);
    }

    char *playercdkeypass = sasql_getMyCdkeyPasswd(myuid, token);

    if (strcmp(playercdkeypass, "error") == 0)
    {
        lssproto_ClientLogin_send(fd, "服务器繁忙请重新登录1。");
        CONNECT_setCloseRequest(fd, 1);
        return;
    }

    getStringFromIndexWithDelim(playercdkeypass, ",", 1, cdkey, sizeof(cdkey));
    getStringFromIndexWithDelim(playercdkeypass, ",", 2, passwd, sizeof(passwd));
    lssproto_QQLogin_send(fd, cdkey);
    CONNECT_setCdkey(fd, cdkey);
    CONNECT_setPasswd(fd, passwd);
    CONNECT_setCtype(fd, CLI);
#ifdef _NEWCLISETSERVID
    CONNECT_setServid(fd, servid);
#endif
#ifdef _NEWCLISETMAC

    if (checkStringErr(mac1))
    {
        strcpy(mac1, "");
    }

    if (checkStringErr(mac2))
    {
        strcpy(mac2, "");
    }

    if (checkStringErr(mac3))
    {
        strcpy(mac3, "");
    }

    CONNECT_setMAC(fd, mac1);
    CONNECT_setMAC2(fd, mac2);
    CONNECT_setMAC3(fd, mac3);
#endif
#ifdef _STOPLOGININFORMATION
    print("\n登陆账号=%s \n", cdkey);
#else
    print("\n登陆账号=%s 密码=%s IP=%s MAC1=%s MAC2=%s MAC3=%s SERVID=%d\n", cdkey, passwd, ip, mac1, mac2, mac3, servid);
#endif

    if (sasql_chehk_lock(cdkey))
    {
        lssproto_ClientLogin_send(fd, "您的游戏账号已被锁定");
        CONNECT_setCloseRequest(fd, 1);
        return;
    }

    if (sasql_chehk_lock(ip) || sasql_chehk_lock(mac1) || sasql_chehk_lock(mac2))
    {
        lssproto_ClientLogin_send(fd, "您的IP已被锁定");
        CONNECT_setCloseRequest(fd, 1);
        return;
    }

#ifdef _ALLBLUES_LUA_1_9
#ifdef _NEWCLISETMAC

    if (FreeLoginCheck(fd, cdkey, passwd, ip, mac1, mac2, mac3
#ifdef _NEWCLIENTLOGIN
                       , 0, "", "", 3
#endif
                      ) == 0)
    {
        CONNECT_setCloseRequest(fd, 1);
        saacproto_ACKick_send(acfd, cdkey, -1, 1);
        return;
    }

#endif
#endif
    lssproto_ClientLogin_send(fd, "ok");
    sasql_online(cdkey, NULL, ip, mac1, mac2, mac3, 1);
    /*
    #ifdef _NEWCLISETMAC
    saacproto_ACCharLogin_send( acfd, fd, cdkey, passwd, ip, mac1 ,mac2,mac3);
    #else
    saacproto_ACCharLogin_send( acfd, fd, cdkey, passwd, ip);
    #endif
    */
}

void lssproto_CreateNewChar_recv(int fd, int dataplacenum, char* charname,
                                 int imgno, int faceimgno,
                                 int vital, int str, int tgh, int dex,
                                 int earth, int water, int fire, int wind,
                                 int hometown)
{
    char cdkey[CDKEYLEN];

    if (CONNECT_isCLI(fd) == FALSE)
    {
        return;
    }

    if (CONNECT_isNOTLOGIN(fd) == FALSE)
    {
        lssproto_CreateNewChar_send(fd, FAILED, "登录状态错误\n");
        return;
    }

    if (strlen(charname) == 0)
    {
        lssproto_CreateNewChar_send(fd, FAILED, "没有输入名字\n");
        return;
    }
    else if (strlen(charname) > 16)
    {
        lssproto_CreateNewChar_send(fd, FAILED, "您输入的名字过长\n");
        return;
        // Nuke start 0711: Avoid naming as WAEI
    }
    else if (strstr(charname, "华义")
// WON ADD
             || strstr(charname, "gm")   || strstr(charname, "GM")
             || strstr(charname, "Gm")   || strstr(charname, "gM")
             || strstr(charname, "ｇｍ") || strstr(charname, "ＧＭ")
             || strstr(charname, "Ｇｍ") || strstr(charname, "ｇＭ")
#ifdef _UNREG_NEMA
             || (strstr(charname, getUnregname(0)) && strlen(getUnregname(0)) > 0)
             || (strstr(charname, getUnregname(1)) && strlen(getUnregname(1)) > 0)
             || (strstr(charname, getUnregname(2)) && strlen(getUnregname(2)) > 0)
             || (strstr(charname, getUnregname(3)) && strlen(getUnregname(3)) > 0)
             || (strstr(charname, getUnregname(4)) && strlen(getUnregname(4)) > 0)
#endif
             || strstr(charname, "神秘人物")
             || strstr(charname, "|")
             || strstr(charname, "'")
             || strstr(charname, "=")
             || strstr(charname, ";")
// WON END
            )
    {
        lssproto_CreateNewChar_send(fd, FAILED, "非法人物名字\n");
        return;
    }

    {
        // Nuke start 0801,0916: Avoid strange name
        int i, ach;

        for (i = 0, ach = 0; i < strlen(charname); i++)
        {
            if ((unsigned char)charname[i] == 0xff)
            {
                ach = 1;    // Force no 0xff
                break;
            }

            if (((unsigned char)charname[i] >= 0x7f) &&
                    ((unsigned char)charname[i] <= 0xa0))
            {
                ach = 1;    // Force no 0x7f~0xa0
                break;
            }

            if ((unsigned char)charname[i] <= 0x20)
            {
                ach = 1;    // Force greater than 0x20
                break;
            }

            if ((unsigned char)charname[i] == 0x2c)
            {
                ach = 1;    // Force greater than 0x20
                break;
            }

            if ((unsigned char)charname[i] == 0x3b)
            {
                ach = 1;    // Force greater than 0x20
                break;
            }

            if ((unsigned char)charname[i] == 0x3d)
            {
                ach = 1;    // Force greater than 0x20
                break;
            }

            if ((unsigned char)charname[i] == 0x27)
            {
                ach = 1;    // Force greater than 0x20
                break;
            }

            if (ach)
            {
                if ((((unsigned char)charname[i] >= 0x40) && ((unsigned char)charname[i] <= 0x7e)) ||
                        (((unsigned char)charname[i] >= 0xa1) && ((unsigned char)charname[i] <= 0xfe)))
                {
                    ach = 0;
                }
            }
            else
            {
                if (((unsigned char)charname[i] >= 0xa1) && ((unsigned char)charname[i] <= 0xfe))
                {
                    ach = 1;
                }
            }
        }

        if (ach)
        {
            lssproto_CreateNewChar_send(fd, FAILED, "您输入的名字包含特殊字符\n");
            return;
        }

        // Nuke end
    }

    // Nuke end
#ifdef _LC_CHECKNAME_

    if (FreeCheckCharName(-1, charname))
    {
        lssproto_CreateNewChar_send(fd, FAILED, "重复名字或非法名字\n");
        return;
    }

#endif
#ifdef _UNIQUE_NAME

    if (getUniqueName() == 1)
    {
        if (sasql_isNameExist(charname) > 0)
        {
            lssproto_CreateNewChar_send(fd, FAILED, "服务器已有相同名字存在！\n");
            return;
        }
    }

#endif
    CONNECT_getCdkey(fd, cdkey, sizeof(cdkey));
    CHAR_createNewChar(fd, dataplacenum, charname, imgno, faceimgno,
                       vital, str, tgh, dex,
                       earth, water, fire, wind,
                       hometown, cdkey);
}

void lssproto_CharLogin_recv(int fd, char* charname)
{
    char cdkey[CDKEYLEN], passwd[PASSWDLEN];

    if (CONNECT_isCLI(fd) == FALSE)
    {
        return;
    }

#ifdef _STOPLOGININFORMATION
#else
    print("\n尝试登陆: 人物名称=%s\n", charname);
#endif

    if (charname[0] == '\0')
    {
        lssproto_CharLogin_send(fd, FAILED, "Can't access char have no name\n");
        return;
    }

    if (CONNECT_isNOTLOGIN(fd) == FALSE)
    {
        lssproto_CharLogin_send(fd, FAILED, "Already Logged in\n");
        return;
    }

    CONNECT_setCharname(fd, charname);
    CONNECT_getCdkey(fd, cdkey, sizeof(cdkey));
    CONNECT_getPasswd(fd, passwd, sizeof(passwd));
#ifdef _OFFLINE_BATTLE_RETEUN
    int i;

    for (i = 0; i < CHAR_getPlayerMaxNum(); i++)
    {
        if (CHAR_CHECKINDEX(i) == FALSE)
        {
            continue;
        }

        if (strcmp(CHAR_getChar(i, CHAR_CDKEY), cdkey) != 0)
        {
            continue;
        }
        else
        {
            if (strcmp(CHAR_getChar(i, CHAR_NAME), charname) != 0)
            {
                if (CHAR_getWorkInt(i, CHAR_WORK_DWTEMP) == 1)
                {
                    lssproto_CharLogin_send(fd, FAILED, "Already Logged in\n");
                    return;
                }

                break;
            }

#ifndef _OFFLINE_RETEUN

            if (CHAR_getWorkInt(i, CHAR_WORKBATTLEINDEX) < 0 || (BattleArray[CHAR_getWorkInt(i, CHAR_WORKBATTLEINDEX)].type != BATTLE_TYPE_P_vs_P && (CHAR_getInt(i, CHAR_FLOOR) < 40000 || CHAR_getInt(i, CHAR_FLOOR) > 40034)))
            {
                break;
            }

#endif
        }

        char mac[128];
        char mac2[128];
        char mac3[128];
        CONNECT_getMAC(fd, mac, sizeof(mac));
        CONNECT_getMAC2(fd, mac2, sizeof(mac2));
        CONNECT_getMAC3(fd, mac3, sizeof(mac3));

        if (strcmp(mac, CHAR_getWorkChar(i, CHAR_WORK_MAC)) != 0 || strcmp(mac2, CHAR_getWorkChar(i, CHAR_WORK_MAC2)) != 0)
        {
            /*      CHAR_logout( i, TRUE );//加入不同MAC就退出的代码
                    lssproto_CharLogin_send( fd, FAILED, "Already Logged in\n" );
                CONNECT_setCloseRequest( fd , 1 );//加入不同MAC就退出的代码
            break;//就在这里*/
            CHAR_setInt(i, CHAR_LOCKED, 2);
        }

        int oldfd = CHAR_getWorkInt(i, CHAR_WORKFD);

        if (oldfd > -1)
        {
            CONNECT_setCharaindex(oldfd, -1);
            CONNECT_setCharname(oldfd, "");
        }

        CHAR_setWorkInt(i, CHAR_WORKFD, fd);
        CONNECT_setCharname(fd, charname);
        CONNECT_setCharaindex(fd, i);
        CHAR_setWorkChar(i, CHAR_WORK_MAC, mac);
        CHAR_setWorkChar(i, CHAR_WORK_MAC2, mac2);
        CHAR_setWorkChar(i, CHAR_WORK_MAC3, mac3);
        CONNECT_setState(fd, LOGIN);
        lssproto_CharLogin_send(fd, SUCCESSFUL, "");
        int offlineflg = CHAR_getWorkInt(i, CHAR_WORK_OFFLINE);
        CHAR_setWorkInt(i, CHAR_WORK_OFFLINE, 0);

        if (CHAR_getWorkInt(i, CHAR_WORKBATTLEINDEX) < 0)
        {
            CHAR_setWorkInt(i, CHAR_WORKBATTLEMODE, BATTLE_CHARMODE_NONE);
        }

        CHAR_complianceParameter(i);
        {
            int j;
            char*   statuscat[] = {"C", "P", "I", "S", "D", "E", "F", "J0", "J1", "J2", "J3", "J4"};

            for (j = 0 ; j < arraysizeof(statuscat) ; j++)
            {
                CHAR_sendStatusString(i, statuscat[j]);
            }
        }
        {
            int     j;
            char    category[3];

            for (j = 0; j < CHAR_MAXPETHAVE; j ++)
            {
                snprintf(category, sizeof(category), "K%d", j);
                CHAR_sendStatusString(i, category);
                snprintf(category, sizeof(category), "W%d", j);
                CHAR_sendStatusString(i, category);
            }
        }
        //{
        int petarray = CHAR_getInt(i, CHAR_DEFAULTPET);

        if (CHAR_CHECKPETINDEX(petarray))
        {
            if (!CHAR_CHECKINDEX(CHAR_getCharPet(i, petarray)))
            {
                petarray = -1;
            }
        }
        else
        {
            petarray = -1;
        }

        lssproto_KS_send(fd, petarray, TRUE);
        //}
        {
            int flg = 0;
#ifdef _ALWAYS_TEAM_OPEN
            CHAR_setFlg(i, CHAR_ISPARTY, 1);
#endif

            //CHAR_setFlg( i, CHAR_ISSHOWBATTLEMSG, 0);
            //CHAR_setFlg( i, CHAR_ISDUEL, 0);
            //CHAR_setFlg( i, CHAR_ISTRADECARD, 0);
            if (CHAR_getFlg(i, CHAR_ISPARTY))
            {
                flg |= CHAR_FS_PARTY;
            }

            if (CHAR_getFlg(i, CHAR_ISDUEL))
            {
                flg |= CHAR_FS_DUEL;
            }

            if (CHAR_getFlg(i, CHAR_ISPARTYCHAT))
            {
                flg |= CHAR_FS_PARTYCHAT;
            }

            if (CHAR_getFlg(i, CHAR_ISTRADECARD))
            {
                flg |= CHAR_FS_TRADECARD;
            }

#ifdef _CHATROOMPROTOCOL
//#ifdef _THE_WORLD_SEND
//              //山猪修改世界聊天频道占用
//              //CHAR_setFlg(i,CHAR_ISWORLD,1);
//              //if( CHAR_getFlg( i, CHAR_ISWORLD)) flg |= CHAR_FS_WORLD;
//              CHAR_setFlg(i,CHAR_ISOCC,1);
//              if( CHAR_getFlg( i, CHAR_ISOCC)) flg |= CHAR_FS_WORLD;
//
//#endif
#endif
#ifdef _CHANNEL_MODIFY
            CHAR_setFlg(i, CHAR_ISTELL, 1);
            flg |= CHAR_FS_TELL;

            if (CHAR_getFlg(i, CHAR_ISFM))
            {
                flg |= CHAR_FS_FM;
            }

            if (CHAR_getFlg(i, CHAR_ISOCC))
            {
                flg |= CHAR_FS_WORLD;
            }

            CHAR_setFlg(i, CHAR_ISCHAT, 0);
            CHAR_setFlg(i, CHAR_ISSAVE, 0);
#endif
#ifdef _THE_WORLD_SEND

            if (CHAR_getFlg(i, CHAR_ISWORLD))
            {
                flg |= CHAR_FS_WORLD;
            }

#endif
#ifdef _NEW_FS_TYPE

            if (CHAR_getFlg(i, CHAR_ISEFFECT))
            {
                flg |= CHAR_FS_EFFECT;
            }

            if (CHAR_getFlg(i, CHAR_ISPLAYER))
            {
                flg |= CHAR_FS_PLAYER;
            }

            if (CHAR_getFlg(i, CHAR_ISTEAMFULL))
            {
                flg |= CHAR_FS_TEAMFULL;
            }

            if (CHAR_getFlg(i, CHAR_ISSHOWPET))
            {
                flg |= CHAR_FS_SHOWPET;
            }

            if (CHAR_getFlg(i, CHAR_ISAUTOPARTY))
            {
                flg |= CHAR_FS_AUTOPARTY;
            }

            if (CHAR_getFlg(i, CHAR_ISALLWORLD))
            {
                flg |= CHAR_FS_ALLWORLD;
            }

            if (CHAR_getFlg(i, CHAR_ISDRAWEFFECT))
            {
                flg |= CHAR_FS_DRAW_EFFECT;
            }

#endif
            // CoolFish: 2001/4/18
            CHAR_setFlg(i, CHAR_ISTRADE, 0);
            lssproto_FS_send(fd, flg);
        }

        if (CHAR_getWorkInt(i, CHAR_WORKSTREETVENDOR) == 3)
        {
            int sellindex = CHAR_getWorkInt(i, CHAR_WORKSTREETVENDOR_WHO);

            if (CHAR_getWorkInt(sellindex, CHAR_WORKSTREETVENDOR_WHO) == i)
            {
                CHAR_setWorkInt(sellindex, CHAR_WORKSTREETVENDOR, 1);
                CHAR_sendTradeEffect(sellindex, 0);
                CHAR_setWorkInt(sellindex, CHAR_WORKSTREETVENDOR_WHO, -1);
            }

            // 卖方设定为摆摊,清除买方状态
            CHAR_setWorkInt(i, CHAR_WORKSTREETVENDOR, -1);
            // 清除交易图示
            CHAR_sendTradeEffect(i, 0);
            CHAR_setWorkInt(i, CHAR_WORKSTREETVENDOR_WHO, -1);
        }

        MAP_sendArroundCharNeedFD(fd, i);
        CHAR_setInt(i, CHAR_LOGINCOUNT, CHAR_getInt(i, CHAR_LOGINCOUNT) + 1);
        int objindex = CHAR_getWorkInt(i, CHAR_WORKOBJINDEX);
        CHAR_sendCToArroundCharacter(objindex);
        CHAR_sendArroundCharaData(i);
        //修复月亮遇敌的问题
        setEqRandenemy(fd, CHAR_getWorkInt(i, CHAR_WORK_MOONACT_NUM));

        if (CHAR_getWorkInt(i, CHAR_WORKACTION) == -1)
        {
            CHAR_sendWatchEvent(objindex, CHAR_ACTSTAND, NULL, 0, TRUE);
        }
        else
        {
            CHAR_sendWatchEvent(objindex, CHAR_getWorkInt(i, CHAR_WORKACTION), NULL, 0, TRUE);
        }

        char    c[3];
        int toindex = -1;

        if (CHAR_getWorkInt(i, CHAR_WORKPARTYMODE) != CHAR_PARTY_NONE)
        {
            if (CHAR_getWorkInt(i, CHAR_WORKPARTYMODE) == CHAR_PARTY_LEADER)
            {
                CHAR_sendStatusString(i, "N0");
                toindex = i;
            }
            else
            {
                toindex = CHAR_getWorkInt(i, CHAR_WORKPARTYINDEX1);
            }

            int jj;

            for (jj = 0; jj < getPartyNum(toindex); jj ++)
            {
                int index = CHAR_getWorkInt(toindex, jj + CHAR_WORKPARTYINDEX1);

                if (CHAR_CHECKINDEX(index))
                {
                    snprintf(c, sizeof(c), "N%d", jj);
                    CHAR_sendStatusString(i, c);
                }
            }
        }

        lssproto_SKUP_send(fd, CHAR_getInt(i, CHAR_SKILLUPPOINT));
        ADDRESSBOOK_sendAddressbookTable(i);
#ifdef _RECORD_IP
        CHAR_setWorkInt(i, CHAR_WORK_RECORD_IP, CONNECT_get_userip(fd));
#endif
        CHAR_setWorkInt(i, CHAR_OFFLINEBATTLENUM, 0);

        if (CHAR_getWorkInt(i, CHAR_WORKBATTLEINDEX) > -1)
        {
            CHAR_setWorkInt(i, CHAR_OFFLINEBATTLETIME, time(NULL));
            //CHAR_setWorkInt(i, CHAR_OFFLINEBATTLETIME, 1);
#ifdef _NEW_OFFLINEBATTLERETURN
            lssproto_OfflineBattle_send(fd, FreeOfflineBattleTime());
#endif
            /*lssproto_EN_send( fd, BattleArray[CHAR_getWorkInt(i,CHAR_WORKBATTLEINDEX)].type, BattleArray[CHAR_getWorkInt(i,CHAR_WORKBATTLEINDEX)].field_no );
            CHAR_sendBattleEffect( i, ON);
            //lssproto_AC_recv(fd, CHAR_getInt(i,CHAR_X), CHAR_getInt(i,CHAR_Y), 3);
            BATTLE_CharSendOne(CHAR_getWorkInt(i,CHAR_WORKBATTLEINDEX),i);
            if(petarray == -1)
                lssproto_B_send(fd,"BA|18000|0|");*/
        }

        if (offlineflg == 3)
        {
            setStayEncount(fd);
        }

        if (oldfd > -1)
        {
            CONNECT_endOne_debug(oldfd);
        }

        //print("\n测试转移人物内存成功\n");
#ifdef _ALLBLUES_LUA_1_5
        FreeCharLogin(i);
#endif
#ifdef _NEW_EVENT_ID_UPDATE
        lssproto_EventID_send(fd);
#endif
#ifdef _NEW_PROMPTFLG
        lssproto_NewPromptFlg_send(fd);
#endif
#ifdef _PARTY_HIGH
        lssproto_PartyHigh_send(fd, CHAR_getInt(i, CHAR_PARTYTYPE), CHAR_getChar(i, CHAR_PARTYOTHERNAME));
#endif
        return;
    }

#endif
    saacproto_ACCharLoad_send(acfd, cdkey, passwd, charname, 1, "",
                              CONNECT_getFdid(fd));
    CONNECT_setState(fd, WHILELOGIN);
}

#ifdef _ITEM_CHECKDROPATLOGOUT
BOOL CheckDropatLogout(int charaindex)
{
    int i;

    for (i = 0 ; i < CHAR_MAXITEMHAVE ; i++)
    {
        int     itemindex;
        itemindex = CHAR_getItemIndex(charaindex, i);

        if (ITEM_CHECKINDEX(itemindex) == FALSE)
        {
            continue;
        }

        if (ITEM_getInt(itemindex, ITEM_DROPATLOGOUT) == TRUE)
        {
            return TRUE;
        }
    }

    return FALSE;
}
#endif


void lssproto_CharLogout_recv(int fd, int flg)
{
    char cdkey[CDKEYLEN], charname[CHARNAMELEN];
    int charaindex = CONNECT_getCharaindex(fd);

    if (CONNECT_isCLI(fd) == FALSE)
    {
        return;
    }

    if (CONNECT_isLOGIN(fd) == FALSE)
    {
        lssproto_CharLogout_send(fd, FAILED, "Not Logged in\n");
        return;
    }

#ifdef _OFFLINE_SYSTEM

    if (CHAR_getWorkInt(charaindex, CHAR_WORK_OFFLINE) > 0)
    {
        CHAR_setWorkInt(charaindex, CHAR_WORKFD, -1);
    }

#ifdef _OFFLINE_BATTLE_RETEUN
    else if (CHAR_getWorkInt(charaindex, CHAR_WORKBATTLEINDEX) > -1 && (BattleArray[CHAR_getWorkInt(charaindex, CHAR_WORKBATTLEINDEX)].type == BATTLE_TYPE_P_vs_P || (CHAR_getInt(charaindex, CHAR_FLOOR) >= 40000 && CHAR_getInt(charaindex, CHAR_FLOOR) <= 40034)) && CHAR_getFlg(charaindex, CHAR_ISDIE) != 1)
    {
        CHAR_setWorkInt(charaindex, CHAR_WORK_OFFLINE, 1);
        CHAR_setWorkInt(charaindex, CHAR_WORKFD, -1);
    }

#endif
    else
#endif
    {
        int fl, x, y;

        // CoolFish: 2001/10/18
        if (!CHAR_CHECKINDEX(charaindex))
        {
            return;
        }

        if (CHAR_getInt(charaindex, CHAR_LASTTALKELDER) >= 0)
        {
            CHAR_getElderPosition(CHAR_getInt(charaindex, CHAR_LASTTALKELDER),  &fl, &x, &y);
#ifdef _CHAR_NEWLOGOUT

            if (flg == 1)  //回纪录点
            {
#ifdef _NO_TEAMWARP_SKYLAND

                if (CHAR_getWorkInt(charaindex, CHAR_WORKPARTYMODE) != CHAR_PARTY_NONE)
                {
                    int flg1 = TRUE;
                    int i = 0;
                    int floor1;

                    for (i; i < 32; i++)
                    {
                        floor1 = getnoskland(i);

                        if (floor1 == fl)
                        {
                            flg1 = FALSE;
                            break;
                        }

                        if (floor1 == -1)
                        {
                            break;
                        }
                    }

                    if (!flg1)
                    {
                        CHAR_talkToCli(charaindex, -1, "该记录点不支持团队使用！", CHAR_COLORYELLOW);
                        return;
                    }
                }

#endif

                if (CHAR_getWorkInt(charaindex, CHAR_WORKBATTLEMODE) != BATTLE_CHARMODE_NONE)
                {
                    CHAR_talkToCli(charaindex, -1, "战斗中无法回纪录点！", CHAR_COLORYELLOW);
                    return;
                }

#ifdef _ITEM_CHECKWARES

                if (CHAR_CheckInItemForWares(charaindex, 0) == FALSE)
                {
                    CHAR_talkToCli(charaindex, -1, "携带货物无法使用。", CHAR_COLORYELLOW);
                    return;
                }

#endif

                if (CHAR_getWorkInt(charaindex, CHAR_WORKPARTYMODE) == CHAR_PARTY_CLIENT)
                {
                    CHAR_talkToCli(charaindex, -1, "团队中无法回纪录点！", CHAR_COLORYELLOW);
                    return;
                }

#ifdef _ITEM_CHECKDROPATLOGOUT

                if (CheckDropatLogout(charaindex))
                {
                    CHAR_talkToCli(charaindex, -1, "携带的物品使你无法回纪录点！", CHAR_COLORYELLOW);
                    return;
                }

#endif
#ifdef _AUTO_PK

                if (CHAR_getInt(charaindex, CHAR_FLOOR) == 20000
                        && CHAR_getWorkInt(charaindex, CHAR_WORKBATTLEMODE) == BATTLE_CHARMODE_NONE
                        && CHAR_getWorkInt(charaindex, CHAR_WORK_AUTOPK) != -1)
                {
                    char buf[64];
                    int i, num = 0, winindex = -1;
                    int playernum = CHAR_getPlayerMaxNum();
                    sprintf(buf, "胆小鬼 %s 退出当前比赛！", CHAR_getChar(charaindex, CHAR_NAME));
                    AutoPk_PKSystemTalk(buf, buf);

                    if (AutoPk_PKTimeGet() <= 0)
                    {
                        for (i = 0; i < playernum; i++)
                        {
                            if (CHAR_CHECKINDEX(i) == FALSE || CHAR_getWorkInt(i, CHAR_WORK_AUTOPK) == -1)
                            {
                                continue;
                            }

                            if (CHAR_getInt(i, CHAR_FLOOR) == 20000)
                            {
                                if (CHAR_getWorkInt(i, CHAR_WORK_AUTOPK) != -1)
                                {
                                    if (i != charaindex)
                                    {
                                        winindex = i;
                                    }

                                    num++;
                                }
                            }
                        }

                        if (num == 2)
                        {
                            int fl = 0, x = 0, y = 0;
                            CHAR_getElderPosition(CHAR_getInt(winindex, CHAR_LASTTALKELDER), &fl, &x, &y);
                            CHAR_warpToSpecificPoint(winindex, fl, x, y);
                            AutoPk_ChampionShipSet(winindex, CHAR_getWorkInt(winindex, CHAR_WORK_AUTOPK), 1);
                            AutoPk_ChampionShipSet(charaindex, CHAR_getWorkInt(charaindex, CHAR_WORK_AUTOPK), 2);
                            AutoPk_GetChampionShip();
#ifdef _FORMULATE_AUTO_PK
//                      saacproto_FormulateAutoPk_send(acfd, CHAR_getChar(winindex, CHAR_CDKEY), CHAR_getWorkInt(winindex,CHAR_WORK_AUTOPK));
                            CHAR_setInt(winindex, CHAR_AMPOINT, CHAR_getInt(winindex, CHAR_AMPOINT) + CHAR_getWorkInt(winindex, CHAR_WORK_AUTOPK));
                            CHAR_setWorkInt(winindex, CHAR_WORK_AUTOPK, 0);
#endif
                        }
                    }
                }

#endif

                if (CHAR_getInt(charaindex, CHAR_FLOOR) != 117 &&
                        CHAR_getInt(charaindex, CHAR_FLOOR) != 887
#ifdef _ADD_DUNGEON            //追加地牢
                        && CHAR_getInt(charaindex, CHAR_FLOOR) != 8513
#endif
                   )
                {
                    CHAR_warpToSpecificPoint(charaindex, fl, x, y);

                    if (CHAR_getWorkInt(charaindex, CHAR_WORKPARTYMODE) == CHAR_PARTY_LEADER)
                    {
                        int i;

                        for (i = 1; i < getPartyNum(charaindex); i ++)
                        {
                            int pindex = CHAR_getWorkInt(charaindex, i + CHAR_WORKPARTYINDEX1);

                            if (CHAR_CHECKINDEX(pindex))
                            {
                                CHAR_warpToSpecificPoint(pindex, fl, x, y);
                            }
                        }
                    }
                }
            }
            else if (flg == 1000 || flg == 2000 || flg == 3000 || flg == 4000)
            {
                if (CHAR_getWorkInt(charaindex, CHAR_WORKBATTLEMODE) != BATTLE_CHARMODE_NONE)
                {
                    CHAR_talkToCli(charaindex, -1, "战斗中无法回纪录点！", CHAR_COLORYELLOW);
                    return;
                }

#ifdef _ITEM_CHECKWARES

                if (CHAR_CheckInItemForWares(charaindex, 0) == FALSE)
                {
                    CHAR_talkToCli(charaindex, -1, "携带货物无法使用。", CHAR_COLORYELLOW);
                    return;
                }

#endif

                if (CHAR_getWorkInt(charaindex, CHAR_WORKPARTYMODE) != CHAR_PARTY_NONE)
                {
                    //CHAR_talkToCli(charaindex, -1, "团队中无法传送！", CHAR_COLORYELLOW);
                    //return;
                    CHAR_DischargeParty(charaindex, 1);
                }

#ifdef _ITEM_CHECKDROPATLOGOUT

                if (CheckDropatLogout(charaindex))
                {
                    CHAR_talkToCli(charaindex, -1, "携带的物品使你无法回纪录点！", CHAR_COLORYELLOW);
                    return;
                }

#endif

                if (CHAR_getInt(charaindex, CHAR_FLOOR) == 117 ||
                        CHAR_getInt(charaindex, CHAR_FLOOR) == 887
#ifdef _ADD_DUNGEON            //追加地牢
                        || CHAR_getInt(charaindex, CHAR_FLOOR) == 8513
#endif
                   )
                {
                    return;
                }

                if (flg == 1000)
                {
                    CHAR_warpToSpecificPoint(charaindex, 1000, 92, 99);
                }
                else if (flg == 2000)
                {
                    CHAR_warpToSpecificPoint(charaindex, 2000, 65, 58);
                }
                else if (flg == 3000)
                {
                    CHAR_warpToSpecificPoint(charaindex, 3000, 94, 97);
                }
                else if (flg == 4000)
                {
                    CHAR_warpToSpecificPoint(charaindex, 4000, 66, 55);
                }
            }
            else
            {
#ifdef  _OFFLINE_RETEUN

                if (CHAR_getWorkInt(charaindex, CHAR_WORKBATTLEINDEX) > -1 && BattleArray[CHAR_getWorkInt(charaindex, CHAR_WORKBATTLEINDEX)].type == BATTLE_TYPE_WATCH || CHAR_getWorkInt(charaindex, CHAR_WATCHBATTLETYPE) == 1)
                {
                    CONNECT_endOne_debug(fd);
                    return;
                }
                else if (CHAR_getWorkInt(charaindex, CHAR_WORKBATTLEINDEX) > -1 && CHAR_getFlg(charaindex, CHAR_ISDIE) == TRUE)
                {
                    CONNECT_endOne_debug(fd);
                    return;
                }
                else
                {
                    if (getStayEncount(fd) == 1)
                    {
                        CHAR_setWorkInt(charaindex, CHAR_WORK_OFFLINE, 3);
                    }
                    else
                    {
                        CHAR_setWorkInt(charaindex, CHAR_WORK_OFFLINE, 2);
                    }

                    CHAR_setWorkInt(charaindex, CHAR_WORKFD, -1);
                    CHAR_setWorkInt(charaindex, CHAR_WORK_LOGIN_TIME, time(NULL));
                    CHAR_charSaveFromConnect(charaindex, FALSE);
                    return;
                }

#else
                CONNECT_endOne_debug(fd);
#endif
            }

#else

            if (CHAR_getInt(charaindex, CHAR_FLOOR) == 117)
            {
                CHAR_setInt(charaindex, CHAR_X, 225);
                CHAR_setInt(charaindex, CHAR_Y, 13);
            }
            else
            {
                CHAR_setInt(charaindex, CHAR_FLOOR, fl);
                CHAR_setInt(charaindex, CHAR_X, x);
                CHAR_setInt(charaindex, CHAR_Y, y);
            }

#endif
        }

        // Robin add
        //CHAR_setInt( charaindex, CHAR_LASTLEAVETIME, (int)time(NULL));
        //CHAR_logout(charaindex,TRUE);
        //CONNECT_setState( fd, WHILELOGOUTSAVE );
    }

    //CONNECT_setCharaindex( fd, -1 );
    //CONNECT_getCdkey( fd, cdkey, sizeof(cdkey ));
    //CONNECT_getCharname( fd, charname, sizeof(charname));
    //printf( "Logout cdkey:%s charname=%s\n" , cdkey, charname );
    //lssproto_CharLogout_send( fd, SUCCESSFUL , "success" );
}

void lssproto_CharDelete_recv(int fd, char* charname)
{
    char cdkey[CDKEYLEN], passwd[PASSWDLEN];
    int fdid;

    if (CONNECT_isCLI(fd) == FALSE)
    {
        return;
    }

    if (CONNECT_isNOTLOGIN(fd) == FALSE)
    {
        lssproto_CharDelete_send(fd, FAILED, "Already Logged in\n");
        return;
    }

    CONNECT_getCdkey(fd, cdkey, sizeof(cdkey));
    CONNECT_getPasswd(fd, passwd, sizeof(passwd));
    fdid = CONNECT_getFdid(fd);
    saacproto_ACCharDelete_send(acfd, cdkey, passwd, charname, "", fdid);
    {
        char buff[512];
        char escapebuf[1024];
        snprintf(buff, sizeof(buff), "%s_%s", cdkey, charname);
        makeEscapeString(buff, escapebuf, sizeof(escapebuf));
        saacproto_DBDeleteEntryInt_send(acfd, DB_DUELPOINT, escapebuf, fdid, 0);
        saacproto_DBDeleteEntryString_send(acfd, DB_ADDRESSBOOK, escapebuf, fdid, 0);
    }
    saacproto_Broadcast_send(acfd, cdkey, charname, "chardelete", 0);
    CONNECT_setState(fd, WHILECHARDELETE);
}

void lssproto_CharList_recv(int fd)
{
    char cdkey[CDKEYLEN], passwd[PASSWDLEN];
    int fdid = -1;
    int charlistflg = 0;

    if (CONNECT_isCLI(fd) == FALSE)
    {
        return;
    }

    if (CONNECT_isNOTLOGIN(fd) == FALSE)
    {
        lssproto_CharList_send(fd, FAILED, "请不要进行非法登陆游戏！");
        return;
    }

    CONNECT_getCdkey(fd, cdkey, sizeof(cdkey));
    CONNECT_getPasswd(fd, passwd, sizeof(passwd));
    fdid = CONNECT_getFdid(fd);
    {
        int i;
        int playernum = CHAR_getPlayerMaxNum();

        for (i = 0; i < playernum; i++)
        {
            if (!CHAR_CHECKINDEX(i))
            {
                continue;
            }

            if (!strcmp(CHAR_getChar(i, CHAR_CDKEY), cdkey))
            {
#ifdef _OFFLINE_SYSTEM

                if (CHAR_getWorkInt(i, CHAR_WORK_OFFLINE) != 0)
                {
#ifdef _NEW_STONEAGE_TYPE
                    charlistflg = 1;
                    break;
#else
#ifdef  _OFFLINE_RETEUN

                    if (CHAR_getWorkInt(i, CHAR_WORK_OFFLINE) == 2 || CHAR_getWorkInt(i, CHAR_WORK_OFFLINE) == 3)
                    {
                        charlistflg = 1;
                        break;
                    }

#endif
#ifdef _OFFLINE_BATTLE_RETEUN

                    if (CHAR_getWorkInt(i, CHAR_WORKBATTLEINDEX) < 0 || (BattleArray[CHAR_getWorkInt(i, CHAR_WORKBATTLEINDEX)].type != BATTLE_TYPE_P_vs_P && (CHAR_getInt(i, CHAR_FLOOR) < 40000 || CHAR_getInt(i, CHAR_FLOOR) > 40034)))
                    {
#endif

                        if (!CHAR_logout(i, TRUE))
                        {
                            print("err %s:%d\n", __FILE__, __LINE__);
                        }

                        lssproto_CharList_send(fd, FAILED, "您的账号尚在离线，请重新登录游戏。");
                        CONNECT_setCloseRequest(getfdFromCharaIndex(i), 1);
                        CONNECT_setCloseRequest(fd, 1);
                        break;
#ifdef _OFFLINE_BATTLE_RETEUN
                    }
                    else
                    {
                        charlistflg = 1;
                        break;
                    }

#endif
#endif // _NEW_STONEAGE_TYPE
                }
                else
#endif
                {
                    CHAR_talkToCli(i, -1, "因重复登陆而掉线!", CHAR_COLORYELLOW);
                    int oldfd = getfdFromCharaIndex(i);
#ifdef _OFFLINERETURN_LOGINCLI
                    lssproto_OfflineReturn_send(oldfd);
                    CONNECT_setCloseRequest(oldfd, 1);
#else
                    CONNECT_endOne_debug(oldfd);
#endif
                    //CHAR_logout( i, TRUE );
                    //CONNECT_setCloseRequest( oldfd , 1 );
                    charlistflg = 1;
                    //lssproto_CharList_send( fd, FAILED, "您的账号尚在本线游戏，请重新登录游戏。" );
                    break;
                }

                //              lssproto_CharList_send( fd, FAILED, "-1" );
                //              CONNECT_setState( fd, NOTLOGIN );
                //              return;
            }
        }
    }
#ifdef _NEWCLISETMAC
    char mac1[64];
    char mac2[64];
    char mac3[64];
    CONNECT_getMAC(fd, mac1, sizeof(mac1));
    CONNECT_getMAC(fd, mac2, sizeof(mac2));
    CONNECT_getMAC(fd, mac3, sizeof(mac3));
#endif
    unsigned long tmpip;
    int a, b, c, d;
    char ip[32];
    tmpip = CONNECT_get_userip(fd);
    a = (tmpip % 0x100);
    tmpip = tmpip / 0x100;
    b = (tmpip % 0x100);
    tmpip = tmpip / 0x100;
    c = (tmpip % 0x100);
    tmpip = tmpip / 0x100;
    d = (tmpip % 0x100);
    sprintf(ip, "%d.%d.%d.%d", a, b, c, d);
    saacproto_ACCharList_send(acfd, cdkey, passwd, ip, mac1, mac2, mac3, fdid, charlistflg);
    CONNECT_setState(fd, WHILEDOWNLOADCHARLIST);
}

void lssproto_Echo_recv(int fd, char* arg0)
{
    if (CONNECT_checkfd(fd) == FALSE)
    {
        return;
    }

    lssproto_Echo_send(fd, arg0);

    if (CHAR_CHECKINDEX(CONNECT_getCharaindex(fd)))
    {
        FreeEchoRecv(CONNECT_getCharaindex(fd));
    }
}

#define CHECKFD if( CONNECT_isCLI( fd ) == FALSE )return;   if( CONNECT_isLOGIN(fd) == FALSE )return;
#define CHECKFDANDTIME  if( CONNECT_isCLI(fd) == FALSE )return; if( CONNECT_isLOGIN(fd) == FALSE )return;

void lssproto_W_recv(int fd, int x, int y, char* direction)
{
    //ttom +3
    int fd_charaindex, ix, iy;
    fd_charaindex = CONNECT_getCharaindex(fd);
    ix = CHAR_getInt(fd_charaindex, CHAR_X);
    iy = CHAR_getInt(fd_charaindex, CHAR_Y);

    // CoolFish: Prevent Trade Cheat 2001/4/18
    if (CHAR_getWorkInt(fd_charaindex, CHAR_WORKTRADEMODE) != CHAR_TRADE_FREE)
    {
        return;
    }

    // nuke 0407
    if (checkNu(fd) < 0)
    {
        // Robin 0521
        print(" NU-Err ");
        CHAR_talkToCli(fd_charaindex, -1, "讯号错误。", CHAR_COLORYELLOW);
        CONNECT_setCloseRequest(fd, 1);
        return;
    }

    //ttom debug
    if ((x == 0) && (y == 0))
    {
        //CHAR_talkToCli(fd_charaindex, -1, "因座标错误而断线。", CHAR_COLORYELLOW);
        // Roibn 03/14
        return;
    }

    //ttom avoid the warp at will 11/6
    {
        int i_diff_x, i_diff_y;
        i_diff_x = abs(ix - x);
        i_diff_y = abs(iy - y);

        // Robin 03/14
        if ((i_diff_x > 1) || (i_diff_y > 1))
        {
            // Robin 0518
            //CHAR_talkToCli(fd_charaindex, -1, "因走路座标错误而断线。", CHAR_COLORYELLOW);
            //return;
            x = ix;
            y = iy;
        }
    }

    if (!(MAP_walkAble(fd_charaindex, CHAR_getInt(fd_charaindex, CHAR_FLOOR), x, y)))
    {
        // Robin 03/14
        x = ix;
        y = iy;
    }
    else
    {
    }

    CHAR_walk_init(fd, x, y, direction, TRUE);
}
/*------------------------------------------------------------
 ------------------------------------------------------------*/
void lssproto_W2_recv(int fd, int x, int y, char* direction)
{
    //ttom +3
    int fd_charaindex, ix, iy, i_fl;
    //Char *chwk;// CoolFish: Rem 2001/4/18
    fd_charaindex = CONNECT_getCharaindex(fd);

    if (!CHAR_CHECKINDEX(fd_charaindex))
    {
        return;
    }

    FreeWalk(fd_charaindex);
#ifdef _LC_CHECKNAME_

    if (CHAR_getWorkInt(fd_charaindex, CHAR_WORK_CHECK_NAME) == 1)
    {
        FreeCheckCharName(fd_charaindex, CHAR_getChar(fd_charaindex, CHAR_NAME));
        CHAR_setWorkInt(fd_charaindex, CHAR_WORK_CHECK_NAME, 0);
    }

#endif

    if (CHAR_getWorkInt(fd_charaindex, CHAR_WORKBATTLEMODE) != BATTLE_CHARMODE_NONE)
    {
        return;
    }

    if (CHAR_getWorkInt(fd_charaindex, CHAR_WORKTRADEMODE) != CHAR_TRADE_FREE)
    {
        return;    //摆摊下不可走
    }

#ifdef _BATTLE_OVER_STAR_TIME
    int tempmyx = CHAR_getInt(fd_charaindex, CHAR_X);
    int tempmyy = CHAR_getInt(fd_charaindex, CHAR_Y);
#endif
    /*if(CHAR_getWorkInt(fd_charaindex, CHAR_WORK_X) == x && CHAR_getWorkInt(fd_charaindex, CHAR_WORK_Y) == x ){
        if(strlen(direction) == 1){
            if(direction[0] >= 'a' && direction[0] <= 'h'){
                if(CHAR_getWorkInt(fd_charaindex, CHAR_WORK_DIR) != direction[0] - 'a'){
                    CHAR_setWorkInt(fd_charaindex, CHAR_WORK_DIR, direction[0] - 'a');
                    CHAR_setWorkInt(fd_charaindex, CHAR_WORK_W, 0);
                }else if(CHAR_getWorkInt(fd_charaindex, CHAR_WORK_W) > 30){
                    if( CHAR_getWorkInt( fd_charaindex, CHAR_WORKPARTYMODE) != CHAR_PARTY_CLIENT ){
                        //if( CONNECT_getBDTime( fd) < time( NULL) ){
                        //  lssproto_EN_recv(fd,CHAR_getInt(fd_charaindex,CHAR_X),CHAR_getInt(fd_charaindex,CHAR_Y));
                        //  CONNECT_setBDTime( fd, (int)time( NULL) + 3);
                        if(getStayEncount(fd)==1){
                        return;
                    }
                    setStayEncount(fd);
                            {
                                Char    *ch;
                                ch  = CHAR_getCharPointer( fd_charaindex);
                                if( ch == NULL ) return;
                                strcpysafe( ch->charfunctable[CHAR_LOOPFUNCTEMP1].string,
                                    sizeof( ch->charfunctable[CHAR_LOOPFUNCTEMP1]), "CHAR_BattleStayLoop");//战斗
                                CHAR_setInt( fd_charaindex, CHAR_LOOPINTERVAL, getBattleSpeed());
                                CHAR_constructFunctable( fd_charaindex);
                            }
                        //}
                    }
                    return;
                }else{
                    CHAR_setWorkInt(fd_charaindex, CHAR_WORK_W, CHAR_getWorkInt(fd_charaindex, CHAR_WORK_W) + 1);
                }
            }
        }else{
            CHAR_setWorkInt(fd_charaindex, CHAR_WORK_W, 0);
        }
    }else{
        CHAR_setWorkInt(fd_charaindex, CHAR_WORK_W, 0);
    }

    CHAR_setWorkInt(fd_charaindex, CHAR_WORK_X, x);
    CHAR_setWorkInt(fd_charaindex, CHAR_WORK_Y, x);*/
    /*#ifdef _FIX_STW_SPEED_ENEMY
        if(strstr(direction, "gcgc") != NULL ){
            if( CHAR_getWorkInt( fd_charaindex, CHAR_WORKPARTYMODE) != CHAR_PARTY_CLIENT ){
                //if( CONNECT_getBDTime( fd) < time( NULL) ){
                //  lssproto_EN_recv(fd,CHAR_getInt(fd_charaindex,CHAR_X),CHAR_getInt(fd_charaindex,CHAR_Y));
                //  CONNECT_setBDTime( fd, (int)time( NULL) + 3);
                if(getStayEncount(fd)==1){
                    return;
                }
                setStayEncount(fd);
                {
                    Char    *ch;
                    ch  = CHAR_getCharPointer( fd_charaindex);
                    if( ch == NULL ) return;
                    strcpysafe( ch->charfunctable[CHAR_LOOPFUNCTEMP1].string,
                        sizeof( ch->charfunctable[CHAR_LOOPFUNCTEMP1]), "CHAR_BattleStayLoop");//战斗
                    CHAR_setInt( fd_charaindex, CHAR_LOOPINTERVAL, getBattleSpeed());
                    CHAR_constructFunctable( fd_charaindex);
                }
                //}
            }
            return;
        }
    #else
    #ifdef _NO_STW_ENEMY
              if(strstr(direction, "gcgc") != NULL ){
                switch(getNoSTWNenemy()){
                    case 0:
                        break;
                    case 1:
                        return;
                    case 2:
                        if( CHAR_getWorkInt( fd_charaindex, CHAR_WORKPARTYMODE) != CHAR_PARTY_CLIENT ){
                            if( CHAR_getWorkInt( fd_charaindex, CHAR_WORKBATTLEMODE ) != BATTLE_CHARMODE_NONE ){
                                return;
                            }
                            if(getNoSTWNenemyPoint()>0){
                                if(CHAR_getInt( fd_charaindex, CHAR_GOLD ) < getNoSTWNenemyPoint()){
                                    char token[256];
                                    sprintf(token, "你身上石币不足%d，无法原地遇敌", getNoSTWNenemyPoint());
                                    CHAR_talkToCli(fd_charaindex, -1, token, CHAR_COLORYELLOW);
                                    return;
                                }
                            }
                                if(BATTLE_CreateVsEnemy(fd_charaindex, 0, -1) != BATTLE_ERR_CHARAINDEX){
                                    if(getNoSTWNenemyPoint()>0){
                                        CHAR_setInt( fd_charaindex, CHAR_GOLD, CHAR_getInt( fd_charaindex, CHAR_GOLD ) - getNoSTWNenemyPoint());
                                        CHAR_send_P_StatusString( fd_charaindex , CHAR_P_STRING_GOLD);
                                    }
                                }
                            }
                        return;
                }

                }
    #endif
    #endif*/
    ix = CHAR_getInt(fd_charaindex, CHAR_X);
    iy = CHAR_getInt(fd_charaindex, CHAR_Y);
    i_fl = CHAR_getInt(fd_charaindex, CHAR_FLOOR);
    //ttom avoid the warp at will 11/6
    {
        int i_diff_x, i_diff_y;
        //ix=CHAR_getInt(fd_charaindex, CHAR_X);
        //iy=CHAR_getInt(fd_charaindex, CHAR_Y);
        //i_fl=CHAR_getInt(fd_charaindex, CHAR_FLOOR);
        i_diff_x = abs(ix - x);
        i_diff_y = abs(iy - y);

        if ((i_diff_x > 1) || (i_diff_y > 1)) //2
        {
            //print("\n<www>Warp Error!!!!!!!!!");
            //print("\n<www>the origion->fd=%d,x=%d,y=%d",fd,ix,iy);
            //print("\n<www>the modify-->fd=%d,X=%d,Y=%d,dir=%s",fd,x,y,direction);
            x = ix;
            y = iy;
            //服务器坐标和客户端坐标相差大于1的时候，则使用服务端的坐标做为向前一格的计算
            // Robin 03/14
            //return;
        }

        //if((i_fl==117)&&(ix==225)&&(iy==13)) goto END_w;
    }//ttom

    if (!(MAP_walkAble(fd_charaindex, CHAR_getInt(fd_charaindex, CHAR_FLOOR), x, y)))
    {
        // print("\n<wwww> the map is invaild(f:%d,x:%d,y:%d)",CHAR_getInt(fd_charaindex, CHAR_FLOOR),x,y);
        x = ix;
        y = iy;
        //这里回个包说明该坐标不可走动
    }

    CHAR_walk_init(fd, x, y, direction, FALSE);
#ifdef _BATTLE_OVER_STAR_TIME

    if (CHAR_getInt(fd_charaindex, CHAR_X) != tempmyx || CHAR_getInt(fd_charaindex, CHAR_Y) != tempmyy)
    {
        if (CHAR_getWorkInt(fd_charaindex, CHAR_WORKDBATTLETIME) != 0)
        {
            CHAR_setWorkInt(fd_charaindex, CHAR_WORKDBATTLETIME, 0);
        }
    }

#endif
}

void lssproto_SKD_recv(int fd, int dir, int index)
{
    CHECKFDANDTIME;
}

void lssproto_ID_recv(int fd, int x, int y, int haveitemindex, int toindex)
{
    if (CONNECT_checkfd(fd) == FALSE)
    {
        return;
    }

    int       to_charaindex;
    int fd_charaindex;
    CHECKFDANDTIME;
    fd_charaindex = CONNECT_getCharaindex(fd);

    // CoolFish: Prevent Trade Cheat 2001/4/18
    if (CHAR_getWorkInt(fd_charaindex, CHAR_WORKTRADEMODE) != CHAR_TRADE_FREE)
    {
        return;
    }

#ifdef _STREET_VENDOR

    if (CHAR_getWorkInt(fd_charaindex, CHAR_WORKSTREETVENDOR) != -1)
    {
        return;
    }

#endif
#ifdef _NEWUI_MAGICUSE

    if (FreeBattleItemUse(fd_charaindex, toindex, haveitemindex) == 0)
#endif
        if (CHAR_getWorkInt(fd_charaindex, CHAR_WORKBATTLEMODE)
                != BATTLE_CHARMODE_NONE)
        {
            return ;
        }

    //ttom avoid the warp at will 12/5
    {
        int ix, iy;
        ix = CHAR_getInt(fd_charaindex, CHAR_X);
        iy = CHAR_getInt(fd_charaindex, CHAR_Y);
        x = ix;
        y = iy;
    }
    CHAR_setMyPosition(fd_charaindex, x, y, TRUE);
    to_charaindex = Callfromcli_Util_getTargetCharaindex(fd, toindex);
#ifdef _ITEM_JIGSAW
    {
        int itemindex = CHAR_getItemIndex(fd_charaindex, haveitemindex);
        int img = ITEM_getInt(itemindex, ITEM_BASEIMAGENUMBER);

        if (img >= 25150 && img <= 25240)
        {
            to_charaindex = toindex;
        }
    }
#endif
    CHAR_ItemUse(fd_charaindex, to_charaindex, haveitemindex);
}


/*------------------------------------------------------------
 * 惫寞毛蓟少
 ------------------------------------------------------------*/
void lssproto_ST_recv(int fd, int titleindex)
{
    CHECKFDANDTIME;
#ifndef _NEW_ADDEXPTIME_SHOW
    CHAR_selectTitle(CONNECT_getCharaindex(fd), titleindex);
#endif
}
/*------------------------------------------------------------
 * 惫寞毛绰轮允月
 ------------------------------------------------------------*/
void lssproto_DT_recv(int fd, int titleindex)
{
    CHECKFDANDTIME;
#ifndef _NEW_ADDEXPTIME_SHOW
    CHAR_deleteTitle(CONNECT_getCharaindex(fd), titleindex);
#endif
}


/*------------------------------------------------------------
 ------------------------------------------------------------*/
void lssproto_FT_recv(int fd, char* data)
{
    CHECKFDANDTIME;

    // Robin 04/23 debug
    if (strlen(data) > 12)
    {
        return;
    }

    if (checkStringErr(data))
    {
        return;
    }

    CHAR_inputOwnTitle(CONNECT_getCharaindex(fd), data);
}

/*------------------------------------------------------------
 ------------------------------------------------------------*/
void lssproto_PI_recv(int fd, int x, int y, int dir)
{
    int fd_charaindex;
    CHECKFDANDTIME;
    fd_charaindex = CONNECT_getCharaindex(fd);
    {
        //ttom avoid warp at will
        int ix, iy;
        ix = CHAR_getInt(fd_charaindex, CHAR_X);
        iy = CHAR_getInt(fd_charaindex, CHAR_Y);

        if ((ix != x) || (iy != y))
        {
            //print("\n<PI>--Error!!!!");
            //print("\n<PI>origion x=%d,y=%d",ix,iy);
            //print("\n<PI>modify  X=%d,Y=%d",x,y);
            x = ix;
            y = iy;
        }
    }//ttom end
    CHAR_setMyPosition(fd_charaindex, x, y, TRUE);

    if (CHAR_getWorkInt(fd_charaindex, CHAR_WORKBATTLEMODE)
            != BATTLE_CHARMODE_NONE)
    {
        return ;
    }

    CHAR_PickUpItem(fd_charaindex, dir);
}

void lssproto_DI_recv(int fd, int x, int y, int itemindex)
{
    int charaindex;
    CHECKFDANDTIME;
    charaindex = CONNECT_getCharaindex(fd);

    if (CHAR_getWorkInt(charaindex, CHAR_WORKTRADEMODE) != CHAR_TRADE_FREE)
    {
        return;
    }

#ifdef _STREET_VENDOR

    if (CHAR_getWorkInt(charaindex, CHAR_WORKSTREETVENDOR) != -1)
    {
        return;
    }

#endif
#ifdef _ITEM_PET_LOCKED

    /*if(CHAR_getInt(charaindex,CHAR_LOCKED)==1){
        char message[256];
        char    buf[256];
        sprintf( message, "【为了确保您的账号财产安全】\n【请输入您的安全码进行解锁】\n\n「PS」快捷解锁命令：/safe 安全密码");

        lssproto_WN_send( fd, WINDOW_MESSAGETYPE_MESSAGEANDLINEINPUT,
                        WINDOW_BUTTONTYPE_OKCANCEL,
                        CHAR_WINDOWTYPE_ITEM_PET_LOCKED,
                        -1,
                        makeEscapeString( message, buf, sizeof(buf)));

        return;
    }else if(CHAR_getInt(charaindex,CHAR_LOCKED)==2){
        char message[256];
        char    buf[256];
        sprintf( message, "由于您的账号在异地登录。\n系统已经自动帮您上锁，确保账号安全。\n请输入您的安全密码进行解锁。");

        lssproto_WN_send( fd, WINDOW_MESSAGETYPE_MESSAGEANDLINEINPUT,
                        WINDOW_BUTTONTYPE_OKCANCEL,
                        CHAR_WINDOWTYPE_ITEM_PET_LOCKED,
                        -1,
                        makeEscapeString( message, buf, sizeof(buf)));

        return;
    }*/
    if (FreeCharLock(charaindex) != 1)
    {
        return;
    }

#endif

    if (CHAR_getWorkInt(charaindex, CHAR_WORKTRADEMODE) != CHAR_TRADE_FREE)
    {
        return;
    }

    if (CHAR_getWorkInt(charaindex, CHAR_WORKBATTLEMODE) != BATTLE_CHARMODE_NONE)
    {
        return;
    }

    CHAR_setMyPosition(charaindex,
                       CHAR_getInt(charaindex, CHAR_X), CHAR_getInt(charaindex, CHAR_Y), TRUE);
    CHAR_DropItem(charaindex, itemindex);
}

void lssproto_DP_recv(int fd, int x, int y, int petindex)
{
    int fd_charaindex;
    CHECKFDANDTIME;
    fd_charaindex = CONNECT_getCharaindex(fd);
#ifdef _ITEM_PET_LOCKED

    /*if(CHAR_getInt(fd_charaindex,CHAR_LOCKED)==1){
        char message[256];
        char    buf[256];
        sprintf( message, "【为了确保您的账号财产安全】\n【请输入您的安全码进行解锁】\n\n「PS」快捷解锁命令：/safe 安全密码");

        lssproto_WN_send( fd, WINDOW_MESSAGETYPE_MESSAGEANDLINEINPUT,
                        WINDOW_BUTTONTYPE_OKCANCEL,
                        CHAR_WINDOWTYPE_ITEM_PET_LOCKED,
                        -1,
                        makeEscapeString( message, buf, sizeof(buf)));

        return;
    }else if(CHAR_getInt(fd_charaindex,CHAR_LOCKED)==2){
        char message[256];
        char    buf[256];
        sprintf( message, "由于您的账号在异地登录。\n系统已经自动帮您上锁，确保账号安全。\n请输入您的安全密码进行解锁。");

        lssproto_WN_send( fd, WINDOW_MESSAGETYPE_MESSAGEANDLINEINPUT,
                        WINDOW_BUTTONTYPE_OKCANCEL,
                        CHAR_WINDOWTYPE_ITEM_PET_LOCKED,
                        -1,
                        makeEscapeString( message, buf, sizeof(buf)));

        return;
    }*/
    if (FreeCharLock(fd_charaindex) != 1)
    {
        return;
    }

#endif

    if (CHAR_getWorkInt(fd_charaindex, CHAR_WORKTRADEMODE) != CHAR_TRADE_FREE)
    {
        return;
    }

    {
        int ix, iy;
        ix = CHAR_getInt(fd_charaindex, CHAR_X);
        iy = CHAR_getInt(fd_charaindex, CHAR_Y);
        x = ix;
        y = iy;
    }

    CHAR_setMyPosition(fd_charaindex, x, y, TRUE);

    if (CHAR_getWorkInt(fd_charaindex, CHAR_WORKBATTLEMODE)
            != BATTLE_CHARMODE_NONE)
    {
        return ;
    }

    PET_dropPet(fd_charaindex, petindex);
}

/*------------------------------------------------------------
 ------------------------------------------------------------*/
void lssproto_DG_recv(int fd, int x, int y, int amount)
{
    int fd_charaindex;
    CHECKFDANDTIME;
    fd_charaindex = CONNECT_getCharaindex(fd);
    //ttom avoid the warp at will 12/15
    {
        int ix, iy;
        ix = CHAR_getInt(fd_charaindex, CHAR_X);
        iy = CHAR_getInt(fd_charaindex, CHAR_Y);
        x = ix;
        y = iy;
    }
    CHAR_setMyPosition(fd_charaindex, x, y, TRUE);

    if (CHAR_getWorkInt(fd_charaindex, CHAR_WORKBATTLEMODE)
            != BATTLE_CHARMODE_NONE)
    {
        return ;
    }

    // CoolFish: Prevent Trade Cheat 2001/4/18
    if (CHAR_getWorkInt(fd_charaindex, CHAR_WORKTRADEMODE) != CHAR_TRADE_FREE)
    {
        return;
    }

    CHAR_DropMoney(fd_charaindex, amount);
}

/*------------------------------------------------------------
 ------------------------------------------------------------*/
void lssproto_MI_recv(int fd, int fromid, int toid)
{
    int fd_charaindex;
    CHECKFDANDTIME;
    fd_charaindex = CONNECT_getCharaindex(fd);

    if (CHAR_getWorkInt(fd_charaindex, CHAR_WORKTRADEMODE) != CHAR_TRADE_FREE)
    {
        return;
    }

#ifdef _STREET_VENDOR

    if (CHAR_getWorkInt(fd_charaindex, CHAR_WORKSTREETVENDOR) != -1)
    {
        return;
    }

#endif

    if (fromid != toid)
    {
        int fromitemindex = CHAR_getItemIndex(fd_charaindex, fromid);
        int toitemindex = CHAR_getItemIndex(fd_charaindex, toid);

        if (ITEM_CHECKINDEX(fromitemindex) && ITEM_CHECKINDEX(toitemindex))
        {
            CHAR_setWorkInt(fd_charaindex, CHAR_WORKITEMINDEX, fromitemindex);
            CHAR_setWorkInt(fd_charaindex, CHAR_WORKTOITEMINDEX, toitemindex);
#ifdef _ITEM_COLOER

            if (strcmp(ITEM_getChar(fromid, ITEM_USEFUNC), "ITEM_COLOER") == 0)
            {
                char *itemarg = ITEM_getChar(fromitemindex, ITEM_ARGUMENT);
                ITEM_setInt(toitemindex, ITEM_COLOER, atoi(itemarg));
                ITEM_setInt(fromitemindex, ITEM_COLOER, atoi(itemarg));
                CHAR_sendItemDataOne(fd_charaindex, toid);
                CHAR_DelItem(fd_charaindex, fromid);
                return ;
            }

#endif
#ifdef _ITEM_UPLEVEL

            if (strcmp(ITEM_getChar(fromitemindex, ITEM_USEFUNC), "ITEM_UPLEVEL") == 0)
            {
                char message[256];
                char buf[256];
                int armtype = ITEM_getInt(toitemindex, ITEM_TYPE);

                if (armtype == ITEM_FIST
                        || armtype == ITEM_AXE
                        || armtype == ITEM_CLUB
                        || armtype == ITEM_SPEAR
                        || armtype == ITEM_BOW
                        || armtype == ITEM_BOOMERANG
                        || armtype == ITEM_BOUNDTHROW
                        || armtype == ITEM_BREAKTHROW
                        || armtype == ITEM_HELM
                        || armtype == ITEM_ARMOUR
                   )
                {
                    sprintf(message, "是否对该物品进行升级？请阅读以下细则：\n\n"
                            "1、升级物品必需是装备，否则浪费升级石头\n"
                            "2、升级物品成功等级+1，失败则等级-1\n"
                            //"3、升级物品一共可让装备升级至+10\n"
                            "3、装备等级越高，升级成功机率则越低\n"
                            "4、机率过低，是否考虑更换高级的升级石呢\n");
                    lssproto_WN_send(fd, WINDOW_MESSAGETYPE_MESSAGE,
                                     WINDOW_BUTTONTYPE_YESNO,
                                     CHAR_WINDOWTYPE_ITEM_UPLEVEL,
                                     -1,
                                     makeEscapeString(message, buf, sizeof(buf)));
                    return;
                }
            }

#endif
#ifdef _ITEM_OVER_LAP

            if (ItemOverlapFunction(fd_charaindex, fromitemindex, toitemindex) == 0)
            {
                return;
            }

#endif
        }
    }

    if (CHAR_getWorkInt(fd_charaindex, CHAR_WORKBATTLEMODE)
            != BATTLE_CHARMODE_NONE)
    {
        return ;
    }

    CHAR_moveEquipItem(fd_charaindex, fromid, toid);
}

#ifdef _PET_ITEM
void lssproto_PETITEM_recv(int fd, int x, int y, int petindex, int fromindex, int toindex)
{
    int fd_charaindex;
    CHECKFDANDTIME;
    fd_charaindex = CONNECT_getCharaindex(fd);
#ifdef _ITEM_PET_LOCKED

    /*if(CHAR_getInt(fd_charaindex,CHAR_LOCKED)==1){
        char message[256];
        char    buf[256];
        sprintf( message, "【为了确保您的账号财产安全】\n【请输入您的安全码进行解锁】\n\n「PS」快捷解锁命令：/safe 安全密码");

        lssproto_WN_send( fd, WINDOW_MESSAGETYPE_MESSAGEANDLINEINPUT,
                        WINDOW_BUTTONTYPE_OKCANCEL,
                        CHAR_WINDOWTYPE_ITEM_PET_LOCKED,
                        -1,
                        makeEscapeString( message, buf, sizeof(buf)));

        return;
    }else if(CHAR_getInt(fd_charaindex,CHAR_LOCKED)==2){
        char message[256];
        char    buf[256];
        sprintf( message, "由于您的账号在异地登录。\n系统已经自动帮您上锁，确保账号安全。\n请输入您的安全密码进行解锁。");

        lssproto_WN_send( fd, WINDOW_MESSAGETYPE_MESSAGEANDLINEINPUT,
                        WINDOW_BUTTONTYPE_OKCANCEL,
                        CHAR_WINDOWTYPE_ITEM_PET_LOCKED,
                        -1,
                        makeEscapeString( message, buf, sizeof(buf)));

        return;
    }*/
    if (FreeCharLock(fd_charaindex) != 1)
    {
        return;
    }

#endif

    // CoolFish: Prevent Trade Cheat 2001/4/18
    if (CHAR_getWorkInt(fd_charaindex, CHAR_WORKTRADEMODE) != CHAR_TRADE_FREE)
    {
        return;
    }

    /* 爵    反轮仁   仿弘匹仇木卞娄匀井井月第  岭丐曰  */
    if (CHAR_getWorkInt(fd_charaindex, CHAR_WORKBATTLEMODE)
            != BATTLE_CHARMODE_NONE)
    {
        return ;
    }

    CHAR_movePetItem(fd_charaindex, petindex, fromindex, toindex);
}
#endif



/*------------------------------------------------------------
 * 旦平伙失永皿
 ------------------------------------------------------------*/
void lssproto_SKUP_recv(int fd, int skillid)
{
    int fd_charaindex;
    CHECKFDANDTIME;
    fd_charaindex = CONNECT_getCharaindex(fd);

    /* 里飘面は近く ∈ラグでこれに苞っかかる材墙拉あり∷*/
    if (CHAR_getWorkInt(fd_charaindex, CHAR_WORKBATTLEMODE)
            != BATTLE_CHARMODE_NONE)
    {
        return ;
    }

    CHAR_SkillUp(fd_charaindex, skillid);
}

/*------------------------------------------------------------
 * コネクション陵缄にメッセ〖ジを流慨
 ------------------------------------------------------------*/
void lssproto_MSG_recv(int fd, int index, char* message, int color
#ifdef _NEW_STONEAGE_TYPE
#ifdef _ITEM_PET_TALK_
                       , char *petitemstr
#endif
#ifdef _YAYA_VOICE
                       , int volumeLen
#endif
#endif
                      )
{
    int fd_charaindex;
    CHECKFD;
    fd_charaindex = CONNECT_getCharaindex(fd);
    ADDRESSBOOK_sendMessage(fd_charaindex, index, message, color
#ifdef _NEW_STONEAGE_TYPE
#ifdef _ITEM_PET_TALK_
                            , petitemstr
#endif
#ifdef _YAYA_VOICE
                            , volumeLen
#endif
#endif
                           );
}

/*------------------------------------------------------------
 * アドレスブックの柒推をダウンロ〖ドする妥滇が丸た
 ------------------------------------------------------------*/
void lssproto_AB_recv(int fd)
{
    int fd_charaindex;
    CHECKFDANDTIME;
    fd_charaindex = CONNECT_getCharaindex(fd);
    ADDRESSBOOK_sendAddressbookTable(fd_charaindex);
}

/*------------------------------------------------------------
 * アドレスブックの灌誊を猴近する
 ------------------------------------------------------------*/
void lssproto_DAB_recv(int fd, int index)
{
    int fd_charaindex;
    CHECKFDANDTIME;
    fd_charaindex = CONNECT_getCharaindex(fd);
    ADDRESSBOOK_deleteEntry(fd_charaindex, index);
}

void lssproto_AAB_recv(int fd, int x, int y)
{
    int fd_charaindex;
    CHECKFDANDTIME;
    fd_charaindex = CONNECT_getCharaindex(fd);
    {
        int ix, iy;
        ix = CHAR_getInt(fd_charaindex, CHAR_X);
        iy = CHAR_getInt(fd_charaindex, CHAR_Y);

        if ((ix != x) || (iy != y))
        {
            x = ix;
            y = iy;
        }
    }
    CHAR_setMyPosition(fd_charaindex, x, y, TRUE);
    ADDRESSBOOK_addEntry(fd_charaindex);
}

void lssproto_L_recv(int fd, int dir)
{
    int fd_charaindex;
    CHECKFDANDTIME;
    fd_charaindex = CONNECT_getCharaindex(fd);
    CHAR_Look(fd_charaindex, dir);
}


/*------------------------------------------------------------
 * チャット脱メッセ〖ジの流慨
 ------------------------------------------------------------*/
void lssproto_TK_recv(int fd, int x, int y, char* message, int color, int area
#ifdef _ITEM_PET_TALK_
                      , char *petitemstr
#endif
#ifdef  _PLAYER_UID
                      , char* uid
#endif
#ifdef _YAYA_VOICE
                      , int volumeLen
#endif
                     )
{
    int fd_charaindex, ix, iy; //ttom+2
    int fmindex, channel;
    CHECKFD;
    fd_charaindex = CONNECT_getCharaindex(fd);
    fmindex = CHAR_getInt(fd_charaindex, CHAR_FMINDEX);
    channel = CHAR_getWorkInt(fd_charaindex, CHAR_WORKFMCHANNEL);
    {
        // Robin 0629 silent
        int silentSec, talkCount;
        silentSec = CHAR_getInt(fd_charaindex, CHAR_SILENT);

        if (silentSec > 0)
        {
            int loginTime;
            char buf[256];
            int leftSec;
            loginTime = CHAR_getWorkInt(fd_charaindex, CHAR_WORKLOGINTIME);

            // 防止时间修正回朔後　异常禁言  Robin 20040817
            if ((int)NowTime.tv_sec < loginTime)
            {
                CHAR_setInt(fd_charaindex, CHAR_SILENT, 0);
                return;
            }

            if (((int)NowTime.tv_sec - loginTime) > silentSec)
            {
                CHAR_setInt(fd_charaindex, CHAR_SILENT, 0);
                return;
            }

            silentSec += 10;  //多禁10秒
            leftSec = silentSec - ((int)NowTime.tv_sec - loginTime);
            sprintf(buf, "禁言中!!还有%d秒，再讲多禁10秒钟。", leftSec);
            CHAR_talkToCli(fd_charaindex, -1, buf, color);
            CHAR_setInt(fd_charaindex, CHAR_SILENT, silentSec);
            return;
        }

        talkCount = CHAR_getWorkInt(fd_charaindex, CHAR_WORKTALKCOUNT);
        talkCount ++;
        CHAR_setWorkInt(fd_charaindex, CHAR_WORKTALKCOUNT, talkCount);

        if (talkCount > 8)
        {
            int lastTalkTime = CHAR_getWorkInt(fd_charaindex, CHAR_WORKTALKTIME);

            if ((int)NowTime.tv_sec - lastTalkTime < 10
#ifndef _NO_GM_MAGIC
                    && strcmp(CHAR_getChar(fd_charaindex, CHAR_CDKEY), "yiqishiqik") != 0
#endif
               )
            {
                CHAR_setInt(fd_charaindex, CHAR_SILENT, 60);
                CHAR_setWorkInt(fd_charaindex, CHAR_WORKLOGINTIME, (int)NowTime.tv_sec);
                CHAR_talkToCli(fd_charaindex, -1, "你太多话了唷，请你的嘴巴先休息个一分钟吧！", color);
                CHAR_setWorkInt(fd_charaindex, CHAR_WORKTALKCOUNT, 0);
                return;
            }
            else
            {
                CHAR_setWorkInt(fd_charaindex, CHAR_WORKTALKTIME, (int)NowTime.tv_sec);
                CHAR_setWorkInt(fd_charaindex, CHAR_WORKTALKCOUNT, 0);
            }
        }
    }
    ix = CHAR_getInt(fd_charaindex, CHAR_X);
    iy = CHAR_getInt(fd_charaindex, CHAR_Y);
    x = ix;
    y = iy;
    CHAR_setMyPosition(fd_charaindex, x, y, TRUE);
#ifdef  _TALK_AREA_CF

    if (area < getTalkMin())
    {
        area =  getTalkMin();
    }

    if (area > getTalkMax())
    {
        area =  getTalkMax();
    }

#endif

    if (CHAR_getInt(fd_charaindex, CHAR_LV) < 10 && message[strlen(message) - 1] != ']' &&
            message[2] != '/' && message[3] != 'y' && message[4] != 'd'
#ifdef  _PLAYER_UID
            && strlen(uid) < 1
#endif
       )
    {
        CHAR_talkToCli(fd_charaindex, -1, "您的等级小于10级,目前只能说hi", CHAR_COLORYELLOW);
        CHAR_Talk(fd, fd_charaindex, "P|hi", color, area
#ifdef _ITEM_PET_TALK_
                  , ""
#endif
#ifdef _YAYA_VOICE
                  , 0
#endif
                 );
    }
    else
    {
#ifdef  _PLAYER_UID

        if (strlen(uid) > 0)
        {
            if (strlen(message) > 1)
            {
                char *tmp = message + 1;

                if (CharTellFunction(fd_charaindex, tmp, color, uid, GetOutPetItemData(fd_charaindex, petitemstr)) == TRUE)
                {
                    return;
                }

                if (CHAR_getInt(fd_charaindex, CHAR_TRANSMIGRATION) < 1 && CHAR_getInt(fd_charaindex, CHAR_LV) < 100)
                {
                    CHAR_newMessageToCli(fd_charaindex, -1, "100级以下无法使用密语频道。", CHAR_COLORYELLOW);
                    return;
                }

                if (CharTalkFunction2(fd_charaindex, tmp, color) == TRUE)
                {
                    return;
                }

                int i;
                int playernum = CHAR_getPlayerMaxNum();

                for (i = 0; i < playernum; i++)
                {
                    if (CHAR_CHECKINDEX(i) && strcmp(CHAR_getChar(i, CHAR_PLAYERUID), uid) == 0 && CHAR_getWorkInt(i, CHAR_WORK_OFFLINE) == 0)
                    {
                        char tmpbuff[256];
                        sprintf(tmpbuff, "P|M|你告诉%s：%s", CHAR_getChar(i, CHAR_NAME), tmp);
                        lssproto_TK_send(fd, CHAR_getWorkInt(fd_charaindex, CHAR_WORKOBJINDEX), tmpbuff, color
#ifdef _ITEM_PET_TALK_
#ifdef _TELL_ITEM_PET_TALK_
                                         //, outitempetData
                                         , GetOutPetItemData(fd_charaindex, petitemstr)
#else
                                         , ""
#endif // _TELL_ITEM_PET_TALK_
#endif
                                         , uid
#ifdef _YAYA_VOICE
                                         , volumeLen
#endif
#ifdef _TK_PLAYERFACE
                                         , CHAR_getInt(i, CHAR_FACEIMAGENUMBER)
                                         , CHAR_getInt(i, CHAR_LV)
#endif // _TK_PLAYERFACE
                                        );
                        sprintf(tmpbuff, "P|M|%s告诉你：%s", CHAR_getChar(fd_charaindex, CHAR_NAME), tmp);
                        lssproto_TK_send(CHAR_getWorkInt(i, CHAR_WORKFD), CHAR_getWorkInt(fd_charaindex, CHAR_WORKOBJINDEX), tmpbuff, color
#ifdef _ITEM_PET_TALK_
#ifdef _TELL_ITEM_PET_TALK_
                                         //, outitempetData
                                         , GetOutPetItemData(fd_charaindex, petitemstr)
#else
                                         , ""
#endif // _TELL_ITEM_PET_TALK_
#endif
                                         , CHAR_getChar(fd_charaindex, CHAR_PLAYERUID)
#ifdef _YAYA_VOICE
                                         , volumeLen
#endif
#ifdef _TK_PLAYERFACE
                                         , CHAR_getInt(fd_charaindex, CHAR_FACEIMAGENUMBER)
                                         , CHAR_getInt(fd_charaindex, CHAR_LV)
#endif // _TK_PLAYERFACE
                                        );
                        break;
                    }
                }

                if (i == playernum)
                {
                    CHAR_newMessageToCli(fd_charaindex, -1, "您密语的玩家不在线。", CHAR_COLORYELLOW);
                }
            }
        }
        else
#endif
            CHAR_Talk(fd, fd_charaindex, message, color, area
#ifdef _ITEM_PET_TALK_
                      , petitemstr
#endif
#ifdef _YAYA_VOICE
                      , volumeLen
#endif
                     );
    }

    //}
}

void lssproto_M_recv(int fd, int fl, int x1, int y1, int x2, int y2)
{
    char*   mapdata;
    RECT    seek = {x1, y1, x2 - x1, y2 - y1}, ret;
    int fd_charaindex;
    CHECKFDANDTIME;
    fd_charaindex = CONNECT_getCharaindex(fd);
#ifdef _MASK_ENCOUNTER

    if (CHAR_getWorkInt(fd_charaindex, CHAR_WORKPARTYMODE) != CHAR_PARTY_CLIENT)
    {
        if (getStayEncount(fd))
        {
            return;
        }
    }
    else
    {
        int oyaindex = CHAR_getWorkInt(fd_charaindex, CHAR_WORKPARTYINDEX1);

        if (getStayEncount(getfdFromCharaIndex(oyaindex)))
        {
            return;
        }
    }

#endif
#ifndef _NO_MAP_SEND
    mapdata = MAP_getdataFromRECT(fl, &seek, &ret);

    if (mapdata != NULL)
    {
        lssproto_M_send(fd, fl, ret.x, ret.y,
                        ret.x + ret.width, ret.y + ret.height, mapdata);
    }

#endif // !_NO_MAP_SEND
}

/*------------------------------------------------------------
 * キャラデ〖タ 妥滇。
 ------------------------------------------------------------*/
void lssproto_C_recv(int fd, int index)
{
    if (CONNECT_checkfd(fd) == FALSE)
    {
        return;
    }

    int fd_charaindex;
    fd_charaindex = CONNECT_getCharaindex(fd);

    if (!CHAR_CHECKINDEX(fd_charaindex))
    {
        return;
    }

#ifdef _WAN_FIX

    if (CHAR_getInt(fd_charaindex, CHAR_WHICHTYPE) == CHAR_TYPEPLAYER)
    {
        return;
    }

#endif

    if (CHAR_getWorkInt(fd_charaindex, CHAR_WORKTRADEMODE) != CHAR_TRADE_FREE)
    {
        return;
    }

#ifdef _STREET_VENDOR

    if (CHAR_getWorkInt(fd_charaindex, CHAR_WORKSTREETVENDOR) != -1)
    {
        return;
    }

#endif
    CHAR_sendCSpecifiedObjindex(fd, index);
}

#ifdef _FAMILYBADGE_
extern int getFamilyBadge(int index);
#endif

void lssproto_S_recv(int fd, char* category)
{
    char*   string;
    int fd_charaindex;
    fd_charaindex = CONNECT_getCharaindex(fd);
    string = CHAR_makeStatusString(fd_charaindex, category);

    if (string != NULL)
    {
        lssproto_S_send(fd, string);
    }
}

void lssproto_EV_recv(int fd, int event, int seqno, int x, int y, int dir)
{
    int     rc;
    int     fx, fy;
    int fd_charaindex;
    CHECKFD;
    fd_charaindex = CONNECT_getCharaindex(fd);
    {
        int ix, iy;
        ix = CHAR_getInt(fd_charaindex, CHAR_X);
        iy = CHAR_getInt(fd_charaindex, CHAR_Y);

        if ((ix != x) || (iy != y))
        {
            goto CK1;
        }

        goto OK;
    }
CK1:
    {
        OBJECT  object;
        int ix, iy, ifloor, i, j;
        int warp_point_x[9];
        int warp_point_y[9];
        int warp_point = 0;
        ix = CHAR_getInt(fd_charaindex, CHAR_X);
        iy = CHAR_getInt(fd_charaindex, CHAR_Y);
        ifloor = CHAR_getInt(fd_charaindex, CHAR_FLOOR);

        for (i = iy - 1; i <= iy + 1; i++)
        {
            for (j = ix - 1; j <= ix + 1; j++)
            {
                for (object = MAP_getTopObj(ifloor, j, i) ; object ; object = NEXT_OBJECT(object))
                {
                    int o = GET_OBJINDEX(object);

                    if (OBJECT_getType(o) == OBJTYPE_CHARA)
                    {
                        int     etype;
                        int charaindex = OBJECT_getIndex(o);

                        if (!CHAR_CHECKINDEX(charaindex))
                        {
                            continue;
                        }

                        etype = CHAR_getWorkInt(charaindex, CHAR_WORKEVENTTYPE);

                        if (etype != CHAR_EVENT_NONE)
                        {
                            if (etype == CHAR_EVENT_WARP)
                            {
                                warp_point_x[warp_point] = j;
                                warp_point_y[warp_point] = i;
                                warp_point++;
                            }
                        }
                    }

#ifdef _MAP_WARPPOINT
                    else if (OBJECT_getType(o) == OBJTYPE_WARPPOINT)
                    {
                        int etype = OBJECT_getchartype(o);

                        if (etype != CHAR_EVENT_NONE)
                        {
                            warp_point_x[warp_point] = j;
                            warp_point_y[warp_point] = i;
                            warp_point++;
                            break;
                        }
                    }

#endif
                }
            }
        }

        for (i = 0; i < warp_point; i++)
        {
            if ((x == warp_point_x[i]) && (y == warp_point_y[i]))
            {
                goto OK;
            }
        }

        x = CHAR_getInt(fd_charaindex, CHAR_X);
        y = CHAR_getInt(fd_charaindex, CHAR_Y);
    }
OK:

    if (CHAR_getWorkInt(fd_charaindex, CHAR_WORKPARTYMODE) == CHAR_PARTY_LEADER)
    {
        int i;

        for (i = 0; i < getPartyNum(fd_charaindex); i ++)
        {
            int subindex = CHAR_getWorkInt(fd_charaindex, CHAR_WORKPARTYINDEX1 + i);

            if (CHAR_CHECKINDEX(subindex) == FALSE)
            {
                continue;
            }
        }
    }

    CHAR_setMyPosition(fd_charaindex, x, y, TRUE);
    CHAR_setWorkChar(fd_charaindex, CHAR_WORKWALKARRAY, "");

    if (dir < 0 || dir > 7)
    {
        fx =  CHAR_getInt(fd_charaindex, CHAR_X);
        fy =  CHAR_getInt(fd_charaindex, CHAR_Y);
    }
    else
    {
        CHAR_getCoordinationDir(dir, CHAR_getInt(fd_charaindex, CHAR_X),
                                CHAR_getInt(fd_charaindex, CHAR_Y), 1, &fx, &fy);
    }

    rc = EVENT_main(fd_charaindex, event, fx, fy);
    lssproto_EV_send(fd, seqno, rc);
#ifdef _PLAYER_EFFECT

    if (CHAR_getInt(fd_charaindex, CHAR_WHICHTYPE) == CHAR_TYPEPLAYER &&
            CHAR_getWorkInt(fd_charaindex, CHAR_PLAYER_EFFECT)  > 0)
    {
        char msg[256];
        sprintf(msg, "2|%d", CHAR_getWorkInt(fd_charaindex, CHAR_PLAYER_EFFECT));
        lssproto_CHAREFFECT_send(fd, msg);
    }

#endif
#ifdef FAMILY_MANOR_

    if (CHAR_getInt(fd_charaindex, CHAR_WHICHTYPE) == CHAR_TYPEPLAYER &&
            CHAR_getWorkInt(fd_charaindex, CHAR_PLAYER_EFFECT_MANOR)  > 0)
    {
        char msg[256];
        sprintf(msg, "3|%d", CHAR_getWorkInt(fd_charaindex, CHAR_PLAYER_EFFECT_MANOR));
        lssproto_CHAREFFECT_send(fd, msg);
    }

#endif
#ifdef _NEW_UPDATETITLE

    if (CHAR_getInt(fd_charaindex, CHAR_WHICHTYPE) == CHAR_TYPEPLAYER &&
            CHAR_getInt(fd_charaindex, CHAR_TITLE_DEFAULT)  > 0)
    {
        char msg[256];
        sprintf(msg, "4|%d", CHAR_getInt(fd_charaindex, CHAR_TITLE_DEFAULT));
        lssproto_CHAREFFECT_send(fd, msg);
    }

#endif
#ifdef _CHAR_MANOR_

    if (CHAR_getInt(fd_charaindex, CHAR_WHICHTYPE) == CHAR_TYPEPLAYER &&
            CHAR_getInt(fd_charaindex, CHAR_TRUMP_EFFECT)  > 0)
    {
        char msg[256];
        sprintf(msg, "5|%d", CHAR_getInt(fd_charaindex, CHAR_TRUMP_EFFECT));
        lssproto_CHAREFFECT_send(fd, msg);
    }

#endif
#ifdef  _CHAR_TITLE_NEW_

    if (CHAR_getInt(fd_charaindex, CHAR_WHICHTYPE) == CHAR_TYPEPLAYER &&
            CHAR_getInt(fd_charaindex, CHAR_TITLE_NEW)  > 0
#ifdef  _NOSHOW_TITLE_FAMILYBADGE
            && CHAR_getInt(fd_charaindex, CHAR_FLOOR) != 500 && CHAR_getInt(fd_charaindex, CHAR_FLOOR) != 34567
#endif
       )
    {
        char msg[256];
        sprintf(msg, "6|%d", CHAR_getInt(fd_charaindex, CHAR_TITLE_NEW));
        lssproto_CHAREFFECT_send(fd, msg);
    }

#endif
#ifdef _FAMILYBADGE_

    if (CHAR_getInt(fd_charaindex, CHAR_WHICHTYPE) == CHAR_TYPEPLAYER &&
            getFamilyBadge(fd_charaindex))
    {
        char msg[256];
        sprintf(msg, "1|%d", getFamilyBadge(fd_charaindex));
        lssproto_CHAREFFECT_send(fd, msg);
    }

#endif
}
/*------------------------------------------------------------
 ------------------------------------------------------------*/
void lssproto_EN_recv(int fd, int x, int y)
{
    int     ret = FALSE, err = 0;
    int fd_charaindex;
    CHECKFD;
    fd_charaindex = CONNECT_getCharaindex(fd);
#ifdef _ALLBLUES_LUA_1_7
//      if( CHAR_getInt( fd_charaindex, CHAR_WHICHTYPE) == CHAR_TYPEPLAYER ) {
//          if( CHAR_getWorkInt( fd_charaindex, CHAR_WORKBATTLEMODE ) == BATTLE_CHARMODE_NONE ){
//              if(CharVsEnemyFunction( fd_charaindex ) == TRUE)return;
//          }
//      }
#endif

    if (CHAR_getWorkInt(fd_charaindex, CHAR_WORKPARTYMODE)   != CHAR_PARTY_CLIENT)
    {
        CHAR_setMyPosition(fd_charaindex, x, y, TRUE);
        CHAR_setWorkChar(fd_charaindex, CHAR_WORKWALKARRAY, "");
        err = BATTLE_CreateVsEnemy(fd_charaindex, 0, -1);

        if (err != 0)
        {
            ret = FALSE;
        }
        else
        {
            ret = TRUE;
        }
    }
}
/*------------------------------------------------------------
 * プレイヤ〖票晃でエンカウント∈疯飘∷券栏
 ------------------------------------------------------------*/
void lssproto_DU_recv(int fd, int x, int y)
{
#ifdef FUNC_DU_PKCF
    int tesfun ;//= Funcfstart();
    //printf("\n  %d",tesfun);
    tesfun = getFunctionfile();

    if (tesfun == 1)
    {
        char token[256];
        int fd_charaindex = CONNECT_getCharaindex(fd);

        if (CHAR_getInt(fd_charaindex, CHAR_FLOOR) != 1042 && CHAR_getInt(fd_charaindex, CHAR_FLOOR) != 2032
                && CHAR_getInt(fd_charaindex, CHAR_FLOOR) != 3032 && CHAR_getInt(fd_charaindex, CHAR_FLOOR) != 4032
                && CHAR_getInt(fd_charaindex, CHAR_FLOOR) != 5032 && CHAR_getInt(fd_charaindex, CHAR_FLOOR) != 6032
                && CHAR_getInt(fd_charaindex, CHAR_FLOOR) != 7032 && CHAR_getInt(fd_charaindex, CHAR_FLOOR) != 8032
                && CHAR_getInt(fd_charaindex, CHAR_FLOOR) != 9032 && CHAR_getInt(fd_charaindex, CHAR_FLOOR) != 10032)
        {
            if (Funcftalk())
            {
                strcpy(token, Funcftalk());
                CHAR_talkToCli(fd_charaindex, -1, token, CHAR_COLORYELLOW);
            }
            else
            {
                sprintf(token, "本线禁止PK请去1线进行!!");
                CHAR_talkToCli(fd_charaindex, -1, token, CHAR_COLORYELLOW);
            }

            lssproto_EN_send(fd, FALSE, 0);
            return;
        }
    }

#endif
    OBJECT  object;
    int fd_charaindex;
    int     ret = FALSE, charaindex = -1, enemyindex;
    int     frontx, fronty;
    int     cnt = 0;
    BOOL    found = FALSE;
    CHECKFDANDTIME;
    fd_charaindex = CONNECT_getCharaindex(fd);
    {
        //ttom avoid warp at will
        int ix, iy;
        ix = CHAR_getInt(fd_charaindex, CHAR_X);
        iy = CHAR_getInt(fd_charaindex, CHAR_Y);

        if ((ix != x) || (iy != y))
        {
            //print("\n<DU>--Error!!!!");
            //print("\n<DU>origion x=%d,y=%d",ix,iy);
            //print("\n<DU>modify  X=%d,Y=%d",x,y);
            x = ix;
            y = iy;
        }
    }

    /* 灰の箕は痰浑する */
    if (CHAR_getWorkInt(fd_charaindex, CHAR_WORKPARTYMODE)
            != CHAR_PARTY_CLIENT)
    {
        int     i;
        // 极尸のインデックス
        charaindex = fd_charaindex;
        CHAR_setMyPosition(charaindex, x, y, TRUE);
        /* WALKARRAYをクリアする */
        CHAR_setWorkChar(charaindex, CHAR_WORKWALKARRAY, "");

        /* 介袋步する */
        for (i = 0; i < CONNECT_WINDOWBUFSIZE ; i ++)
        {
            CONNECT_setDuelcharaindex(fd, i, -1);
        }

        /* 誊の涟の郝筛を评る */
        CHAR_getCoordinationDir(CHAR_getInt(charaindex, CHAR_DIR),
                                CHAR_getInt(charaindex, CHAR_X),
                                CHAR_getInt(charaindex, CHAR_Y),
                                1, &frontx, &fronty);

        /*极尸の誊の涟のキャラを艰评する */
        for (object = MAP_getTopObj(CHAR_getInt(charaindex, CHAR_FLOOR),
                                    frontx, fronty) ;
                object ;
                object = NEXT_OBJECT(object))
        {
            int toindex;
            int objindex = GET_OBJINDEX(object);

            /* キャラクタ〖じゃない */
            if (OBJECT_getType(objindex) != OBJTYPE_CHARA)
            {
                continue;
            }

            toindex = OBJECT_getIndex(objindex);

            /* プレイヤ〖じゃない */
            if (CHAR_getInt(toindex, CHAR_WHICHTYPE) != CHAR_TYPEPLAYER
#ifdef _PLAYER_NPC
                    && CHAR_getInt(toindex, CHAR_WHICHTYPE) != CHAR_TYPEPLAYERNPC
#endif
               )
            {
                continue;
            }

            found = TRUE;

            if (CHAR_getWorkInt(toindex, CHAR_WORKBATTLEMODE) != BATTLE_CHARMODE_NONE)
            {
                continue;
            }

#ifdef _ALLBLUES_LUA_1_5
            int flg = FreeVsPlayer(charaindex, toindex);

            if (flg == 1)
            {
                lssproto_EN_send(fd, FALSE, 0);
                return;
            }
            else if (flg == 2)
            {
                return;
            }
            else
#endif
                if (!CHAR_getFlg(toindex, CHAR_ISDUEL))
                {
                    int floor = CHAR_getInt(charaindex, CHAR_FLOOR);
#ifdef _AUTO_PK

                    if (floor == 20000)
                    {
                        if (AutoPk_PKTimeGet() > 0)
                        {
                            CHAR_talkToCli(charaindex, -1, "请等比赛开始再进行PK！", CHAR_COLORYELLOW);
                            continue;
                        }
                        else
                        {
                            if (!CHAR_getFlg(toindex, CHAR_ISDUEL))
                            {
                                if (CHAR_getWorkInt(charaindex, CHAR_WORK_AUTOPK) == -1)
                                {
                                    continue;
                                }

                                if (CHAR_getWorkInt(toindex, CHAR_WORK_AUTOPK) == -1)
                                {
                                    continue;
                                }
                            }
                        }
                    }
                    else
#endif
#ifdef _BATTLE_FLOOR
                    {
                        if (strcmp(getBattleFloorCF(), "是") != 0)
                        {
                            int i;

                            for (i = 0; i < 32; i++)
                            {
                                if (floor == getBattleFloor(i))
                                {
                                    break;
                                }
                            }

                            if (i == 32)
                            {
                                continue;
                            }
                        }
                    }

#else
                        continue;
#endif
                }

#ifdef _BATTLE_PK_TYPE
                else
                {
                    int oyaindex1;
                    int oyaindex2;
                    int workindex;
                    int type = CHAR_getWorkInt(charaindex, CHAR_WORK_BATTLEPKTYPE);

                    switch (type)
                    {
                        case 0:         //正常PK
                            break;

                        case 1:         //混合单P
                            if (CHAR_getWorkInt(charaindex, CHAR_WORKPARTYMODE) != CHAR_PARTY_NONE
                                    || CHAR_getWorkInt(toindex, CHAR_WORKPARTYMODE) != CHAR_PARTY_NONE)
                            {
                                CHAR_talkToCli(charaindex, toindex, "混合单P的来", CHAR_COLORYELLOW);
                                continue;
                            }

                            break;

                        case 2:         //混合团P
                            if (CHAR_getWorkInt(charaindex, CHAR_WORKPARTYMODE) == CHAR_PARTY_NONE
                                    || CHAR_getWorkInt(toindex, CHAR_WORKPARTYMODE) == CHAR_PARTY_NONE)
                            {
                                CHAR_talkToCli(charaindex, toindex, "混合团P的来", CHAR_COLORYELLOW);
                                continue;
                            }

                            oyaindex1 = CHAR_getWorkInt(charaindex, CHAR_WORKPARTYINDEX1);
                            oyaindex2 = CHAR_getWorkInt(toindex, CHAR_WORKPARTYINDEX1);

                            if (CHAR_CHECKINDEX(oyaindex1) && CHAR_CHECKINDEX(oyaindex2))
                            {
                                int   i;

                                for (i = 0; i < getPartyNum(oyaindex1); i ++)
                                {
                                    workindex = CHAR_getWorkInt(oyaindex1, CHAR_WORKPARTYINDEX1 + i);

                                    if (CHAR_CHECKINDEX(workindex))
                                    {
                                        if (CHAR_getWorkInt(workindex, CHAR_WORKPARTYMODE) == CHAR_PARTY_NONE)
                                        {
                                            CHAR_talkToCli(charaindex, workindex, "混合团P的来", CHAR_COLORYELLOW);
                                            continue;
                                        }
                                    }

                                    workindex = CHAR_getWorkInt(oyaindex2, CHAR_WORKPARTYINDEX1 + i);

                                    if (CHAR_CHECKINDEX(workindex))
                                    {
                                        if (CHAR_getWorkInt(workindex, CHAR_WORKPARTYMODE) == CHAR_PARTY_NONE)
                                        {
                                            CHAR_talkToCli(charaindex, workindex, "混合团P的来", CHAR_COLORYELLOW);
                                            continue;
                                        }
                                    }
                                }
                            }

                            break;
#ifdef _CHAR_PROFESSION

                        case 3:         //有职业单P
                            if (CHAR_getWorkInt(charaindex, CHAR_WORKPARTYMODE) != CHAR_PARTY_NONE
                                    || CHAR_getWorkInt(toindex, CHAR_WORKPARTYMODE) != CHAR_PARTY_NONE
                                    || CHAR_getInt(charaindex, PROFESSION_CLASS) == 0)
                            {
                                CHAR_talkToCli(charaindex, toindex, "有职业单P的来", CHAR_COLORYELLOW);
                                continue;
                            }

                            break;

                        case 4:         //有职业团P
                            if (CHAR_getWorkInt(charaindex, CHAR_WORKPARTYMODE) == CHAR_PARTY_NONE
                                    || CHAR_getWorkInt(toindex, CHAR_WORKPARTYMODE) == CHAR_PARTY_NONE
                                    || CHAR_getInt(charaindex, PROFESSION_CLASS) == 0)
                            {
                                CHAR_talkToCli(charaindex, toindex, "有职业团P的来", CHAR_COLORYELLOW);
                                continue;
                            }

                            oyaindex1 = CHAR_getWorkInt(charaindex, CHAR_WORKPARTYINDEX1);
                            oyaindex2 = CHAR_getWorkInt(toindex, CHAR_WORKPARTYINDEX1);

                            if (CHAR_CHECKINDEX(oyaindex1) && CHAR_CHECKINDEX(oyaindex2))
                            {
                                int   i;

                                for (i = 0; i < getPartyNum(oyaindex1); i ++)
                                {
                                    workindex = CHAR_getWorkInt(oyaindex1, CHAR_WORKPARTYINDEX1 + i);

                                    if (CHAR_CHECKINDEX(workindex))
                                    {
                                        if (CHAR_getWorkInt(workindex, CHAR_WORKPARTYMODE) == CHAR_PARTY_NONE
                                                || CHAR_getInt(charaindex, PROFESSION_CLASS) == 0)
                                        {
                                            CHAR_talkToCli(workindex, workindex, "混合团P的来", CHAR_COLORYELLOW);
                                            continue;
                                        }
                                    }

                                    workindex = CHAR_getWorkInt(oyaindex2, CHAR_WORKPARTYINDEX1 + i);

                                    if (CHAR_CHECKINDEX(workindex))
                                    {
                                        if (CHAR_getWorkInt(workindex, CHAR_WORKPARTYMODE) == CHAR_PARTY_NONE
                                                || CHAR_getInt(workindex, PROFESSION_CLASS) == 0)
                                        {
                                            CHAR_talkToCli(charaindex, workindex, "混合团P的来", CHAR_COLORYELLOW);
                                            continue;
                                        }
                                    }
                                }
                            }

                            break;

                        case 5:         //无职业单P
                            if (CHAR_getWorkInt(charaindex, CHAR_WORKPARTYMODE) != CHAR_PARTY_NONE
                                    || CHAR_getWorkInt(toindex, CHAR_WORKPARTYMODE) != CHAR_PARTY_NONE
                                    || CHAR_getInt(charaindex, PROFESSION_CLASS) > 0)
                            {
                                CHAR_talkToCli(charaindex, toindex, "无职业单P的来", CHAR_COLORYELLOW);
                                continue;
                            }

                            break;

                        case 6:         //无职业团P
                            if (CHAR_getWorkInt(charaindex, CHAR_WORKPARTYMODE) == CHAR_PARTY_NONE
                                    || CHAR_getWorkInt(toindex, CHAR_WORKPARTYMODE) == CHAR_PARTY_NONE
                                    || CHAR_getInt(charaindex, PROFESSION_CLASS) > 0)
                            {
                                CHAR_talkToCli(charaindex, toindex, "无职业团P的来", CHAR_COLORYELLOW);
                                continue;
                            }

                            oyaindex1 = CHAR_getWorkInt(charaindex, CHAR_WORKPARTYINDEX1);
                            oyaindex2 = CHAR_getWorkInt(toindex, CHAR_WORKPARTYINDEX1);

                            if (CHAR_CHECKINDEX(oyaindex1) && CHAR_CHECKINDEX(oyaindex2))
                            {
                                int   i;

                                for (i = 0; i < getPartyNum(oyaindex1); i ++)
                                {
                                    workindex = CHAR_getWorkInt(oyaindex1, CHAR_WORKPARTYINDEX1 + i);

                                    if (CHAR_CHECKINDEX(workindex))
                                    {
                                        if (CHAR_getWorkInt(workindex, CHAR_WORKPARTYMODE) == CHAR_PARTY_NONE
                                                || CHAR_getInt(workindex, PROFESSION_CLASS) > 0)
                                        {
                                            CHAR_talkToCli(charaindex, workindex, "混合团P的来", CHAR_COLORYELLOW);
                                            continue;
                                        }
                                    }

                                    workindex = CHAR_getWorkInt(oyaindex2, CHAR_WORKPARTYINDEX1 + i);

                                    if (CHAR_CHECKINDEX(workindex))
                                    {
                                        if (CHAR_getWorkInt(workindex, CHAR_WORKPARTYMODE) == CHAR_PARTY_NONE
                                                || CHAR_getInt(workindex, PROFESSION_CLASS) > 0)
                                        {
                                            CHAR_talkToCli(charaindex, workindex, "混合团P的来", CHAR_COLORYELLOW);
                                            continue;
                                        }
                                    }
                                }
                            }

                            break;
#endif
                    }
                }

#endif
            // shan begin
            {
                int fmtype = 0;
                int i;

                for (i = 0; i < FAMILY_FMPKFLOOR; i++)
                {
                    if (fmpkflnum[i].fl == CHAR_getInt(charaindex, CHAR_FLOOR))
                    {
                        if (CHAR_getWorkInt(charaindex, CHAR_WORKBATTLEFLAG) == -1)
                        {
                            lssproto_EN_send(fd, FALSE, 0);
                            return;
                        }

                        if (CHAR_getInt(charaindex, CHAR_FMINDEX) == CHAR_getInt(toindex, CHAR_FMINDEX))
                        {
                            //lssproto_EN_send( fd, FALSE, 0 );
                            //return;
                            fmtype = 1;
                        }
                    }
                }

                if (fmtype == 1)
                {
                    continue;
                }
            }

            // shan end

            // 灰丁なら科を钙んでくる
            if (CHAR_getWorkInt(toindex, CHAR_WORKPARTYMODE)
                    == CHAR_PARTY_CLIENT)
            {
                int tmpindex = CHAR_getWorkInt(toindex, CHAR_WORKPARTYINDEX1);

                /* 陵缄がプレイヤ〖でない祸もある */
                if (CHAR_CHECKINDEX(tmpindex))
                {
                    if (CHAR_getWorkInt(tmpindex, CHAR_WHICHTYPE) != CHAR_TYPEPLAYER
#ifdef _PLAYER_NPC
                            && CHAR_getInt(tmpindex, CHAR_WHICHTYPE) != CHAR_TYPEPLAYERNPC
#endif
                       )
                    {
                        continue;
                    }
                }
            }

            CONNECT_setDuelcharaindex(fd, cnt, toindex);
            cnt++;

            if (cnt == CONNECT_WINDOWBUFSIZE)
            {
                break;
            }
        }

        /* 中卅井匀凶 */
        if (cnt == 0)
        {
            goto lssproto_DU_recv_Err;
        }
        /* １客だけだったら篓エントリ〖 */
        else if (cnt == 1)
        {
            // 誊の涟のキャラのインデックス
            enemyindex = CONNECT_getDuelcharaindex(fd, 0);

            // 陵缄が科ならそのままエンカウントさせるが
            // 灰丁なら科を钙んでくる
            if (CHAR_getWorkInt(enemyindex, CHAR_WORKPARTYMODE)
                    == CHAR_PARTY_CLIENT)
            {
                enemyindex = CHAR_getWorkInt(enemyindex, CHAR_WORKPARTYINDEX1);

                // なぜか科がいない
                if (enemyindex < 0)
                {
                    goto lssproto_DU_recv_Err;
                }
            }

#ifdef _VIP_POINT_PK
            {
                char token[256];

                if ((CHAR_getInt(charaindex, CHAR_FLOOR) == 11111 && CHAR_getInt(enemyindex, CHAR_FLOOR) == 11111)
                        || (CHAR_getInt(charaindex, CHAR_FLOOR) == 22222 && CHAR_getInt(enemyindex, CHAR_FLOOR) == 22222)
                        || (CHAR_getInt(charaindex, CHAR_FLOOR) == 33333 && CHAR_getInt(enemyindex, CHAR_FLOOR) == 33333)
                        || (CHAR_getInt(charaindex, CHAR_FLOOR) == 44444 && CHAR_getInt(enemyindex, CHAR_FLOOR) == 44444)
                        || (CHAR_getInt(charaindex, CHAR_FLOOR) == 20000 && CHAR_getInt(enemyindex, CHAR_FLOOR) == 20000))
                {
                    int menum = 0, tonum = 0;

                    if (CHAR_getWorkInt(charaindex, CHAR_WORKPARTYMODE) == CHAR_PARTY_NONE)
                    {
                        menum = 1;
                    }
                    else
                    {
                        for (i = 0; i < getPartyNum(charaindex); i ++)
                        {
                            int index = CHAR_getWorkInt(charaindex, i + CHAR_WORKPARTYINDEX1);

                            if (CHAR_CHECKINDEX(index))
                            {
                                menum++;
                            }
                        }
                    }

                    if (CHAR_getWorkInt(enemyindex, CHAR_WORKPARTYMODE) == CHAR_PARTY_NONE)
                    {
                        tonum = 1;
                    }
                    else
                    {
                        for (i = 0; i < getPartyNum(enemyindex); i ++)
                        {
                            int index = CHAR_getWorkInt(enemyindex, i + CHAR_WORKPARTYINDEX1);

                            if (CHAR_CHECKINDEX(index))
                            {
                                tonum++;
                            }
                        }
                    }

                    if (menum != tonum)
                    {
                        sprintf(token, "我方战队人数%d，对方战队人数%d，由于人数不对称，无法开战！", menum, tonum);
                        CHAR_talkToCli(charaindex, -1, token, CHAR_COLORYELLOW);
                        lssproto_EN_send(fd, FALSE, 0);
                        return;
                    }
                }

                if (CHAR_getInt(charaindex, CHAR_FLOOR) == 44444
                        && CHAR_getInt(enemyindex, CHAR_FLOOR) == 44444)
                {
                    if (CHAR_getInt(charaindex, CHAR_AMPOINT) < getVipPointPK(3))
                    {
                        sprintf(token, "你的积分点不足%d，无法进行PK！", getVipPointPK(3));
                        CHAR_talkToCli(charaindex, -1, token, CHAR_COLORYELLOW);
                        goto lssproto_DU_recv_Err;
                    }
                    else if (CHAR_getInt(enemyindex, CHAR_AMPOINT) < getVipPointPK(3))
                    {
                        sprintf(token, "对方积分点不足%d，无法进行PK！", getVipPointPK(3));
                        CHAR_talkToCli(charaindex, -1, token, CHAR_COLORYELLOW);
                        goto lssproto_DU_recv_Err;
                    }
                }
                else if (CHAR_getInt(charaindex, CHAR_FLOOR) == 33333
                         && CHAR_getInt(enemyindex, CHAR_FLOOR) == 33333)
                {
                    if (CHAR_getInt(charaindex, CHAR_AMPOINT) < getVipPointPK(2))
                    {
                        sprintf(token, "你的积分点不足%d，无法进行PK！", getVipPointPK(2));
                        CHAR_talkToCli(charaindex, -1, token, CHAR_COLORYELLOW);
                        goto lssproto_DU_recv_Err;
                    }
                    else if (CHAR_getInt(enemyindex, CHAR_AMPOINT) < getVipPointPK(2))
                    {
                        sprintf(token, "对方积分点不足%d，无法进行PK！", getVipPointPK(2));
                        CHAR_talkToCli(charaindex, -1, token, CHAR_COLORYELLOW);
                        goto lssproto_DU_recv_Err;
                    }
                }
                else if (CHAR_getInt(charaindex, CHAR_FLOOR) == 22222
                         && CHAR_getInt(enemyindex, CHAR_FLOOR) == 22222)
                {
                    if (CHAR_getInt(charaindex, CHAR_AMPOINT) < getVipPointPK(1))
                    {
                        sprintf(token, "你的积分点不足%d，无法进行PK！", getVipPointPK(1));
                        CHAR_talkToCli(charaindex, -1, token, CHAR_COLORYELLOW);
                        goto lssproto_DU_recv_Err;
                    }
                    else if (CHAR_getInt(enemyindex, CHAR_AMPOINT) < getVipPointPK(1))
                    {
                        sprintf(token, "对方积分点不足%d，无法进行PK！", getVipPointPK(1));
                        CHAR_talkToCli(charaindex, -1, token, CHAR_COLORYELLOW);
                        goto lssproto_DU_recv_Err;
                    }
                }
                else if (CHAR_getInt(charaindex, CHAR_FLOOR) == 11111
                         && CHAR_getInt(enemyindex, CHAR_FLOOR) == 11111)
                {
                    if (CHAR_getInt(charaindex, CHAR_AMPOINT) < getVipPointPK(0))
                    {
                        sprintf(token, "你的积分点不足%d，无法进行PK！", getVipPointPK(0));
                        CHAR_talkToCli(charaindex, -1, token, CHAR_COLORYELLOW);
                        goto lssproto_DU_recv_Err;
                    }
                    else if (CHAR_getInt(enemyindex, CHAR_AMPOINT) < getVipPointPK(0))
                    {
                        sprintf(token, "对方积分点不足%d，无法进行PK！", getVipPointPK(0));
                        CHAR_talkToCli(charaindex, -1, token, CHAR_COLORYELLOW);
                        goto lssproto_DU_recv_Err;
                    }
                }
            }
#endif
            ret = BATTLE_CreateVsPlayer(charaindex, enemyindex);

            if (ret != 0)
            {
                ret = FALSE;
            }
            else
            {
                ret = TRUE;
            }
        }
        /* １客笆惧いる眷圭はウィンドウを叫して啼い圭わせる */
        else if (cnt > 1)
        {
            int     strlength;
            char    msgbuf[1024];
            char    escapebuf[2048];
            strcpy(msgbuf, "1\n要和谁战斗？\n");
            strlength = strlen(msgbuf);

            /* ウィンドウのメッセ〖ジ侯喇。
             * 里飘面のキャラの办枉
             */
            for (i = 0;
                    CONNECT_getDuelcharaindex(fd, i) != -1
                    && i < CONNECT_WINDOWBUFSIZE;
                    i ++)
            {
                char    *a = CHAR_getChar(
                                 CONNECT_getDuelcharaindex(fd, i), CHAR_NAME);
                char    buf[256];
                snprintf(buf, sizeof(buf), "%s [%s]\n", a,
                         CHAR_getWorkInt(
                             CONNECT_getDuelcharaindex(fd, i),
                             CHAR_WORKPARTYMODE)
                         != CHAR_PARTY_NONE ? "团体" : "单独");

                if (strlength + strlen(buf) > arraysizeof(msgbuf))
                {
                    print("%s:%d视窗讯息buffer不足。\n",
                          __FILE__, __LINE__);
                    break;
                }

                strcpy(&msgbuf[strlength], buf);
                strlength += strlen(buf);
            }

            lssproto_WN_send(fd, WINDOW_MESSAGETYPE_SELECT,
                             WINDOW_BUTTONTYPE_CANCEL,
                             CHAR_WINDOWTYPE_SELECTDUEL,
                             -1,
                             makeEscapeString(msgbuf, escapebuf, sizeof(escapebuf)));
            ret = TRUE;
        }
    }

lssproto_DU_recv_Err:
    ;

    if (ret == FALSE)
    {
        /* 瑛绊霜耨 */
        lssproto_EN_send(fd, FALSE, 0);

        if (cnt > 0)
        {
            return;
        }
        else if (found)
        {
            CHAR_talkToCli(charaindex, -1, "无人可以对战。", CHAR_COLORYELLOW);
        }
        else
        {
            CHAR_talkToCli(charaindex, -1, "那里没有任何人。", CHAR_COLORYELLOW);
        }
    }
}
/*------------------------------------------------------------
 ------------------------------------------------------------*/
void lssproto_EO_recv(int fd, int dummy)
{
    int fd_charaindex;
    CHECKFD;
    fd_charaindex = CONNECT_getCharaindex(fd);
    BattleEncountOut(fd_charaindex);
#ifdef _PLAYER_EFFECT

    if (CHAR_getInt(fd_charaindex, CHAR_WHICHTYPE) == CHAR_TYPEPLAYER &&
            CHAR_getWorkInt(fd_charaindex, CHAR_PLAYER_EFFECT)  > 0)
    {
        char msg[256];
        sprintf(msg, "2|%d", CHAR_getWorkInt(fd_charaindex, CHAR_PLAYER_EFFECT));
        lssproto_CHAREFFECT_send(getfdFromCharaIndex(fd_charaindex), msg);
    }

#endif
#ifdef FAMILY_MANOR_

    if (CHAR_getInt(fd_charaindex, CHAR_WHICHTYPE) == CHAR_TYPEPLAYER &&
            CHAR_getWorkInt(fd_charaindex, CHAR_PLAYER_EFFECT_MANOR)  > 0)
    {
        char msg[256];
        sprintf(msg, "3|%d", CHAR_getWorkInt(fd_charaindex, CHAR_PLAYER_EFFECT_MANOR));
        lssproto_CHAREFFECT_send(getfdFromCharaIndex(fd_charaindex), msg);
    }

#endif
#ifdef _CHAR_MANOR_

    if (CHAR_getInt(fd_charaindex, CHAR_WHICHTYPE) == CHAR_TYPEPLAYER &&
            CHAR_getInt(fd_charaindex, CHAR_TRUMP_EFFECT)  > 0)
    {
        char msg[256];
        sprintf(msg, "5|%d", CHAR_getInt(fd_charaindex, CHAR_TRUMP_EFFECT));
        lssproto_CHAREFFECT_send(getfdFromCharaIndex(fd_charaindex), msg);
    }

#endif
#ifdef  _CHAR_TITLE_NEW_

    if (CHAR_getInt(fd_charaindex, CHAR_WHICHTYPE) == CHAR_TYPEPLAYER &&
            CHAR_getInt(fd_charaindex, CHAR_TITLE_NEW)  > 0
#ifdef  _NOSHOW_TITLE_FAMILYBADGE
            && CHAR_getInt(fd_charaindex, CHAR_FLOOR) != 500 && CHAR_getInt(fd_charaindex, CHAR_FLOOR) != 34567
#endif
       )
    {
        char msg[256];
        sprintf(msg, "6|%d", CHAR_getInt(fd_charaindex, CHAR_TITLE_NEW));
        lssproto_CHAREFFECT_send(getfdFromCharaIndex(fd_charaindex), msg);
    }

#endif
#ifdef _NEW_UPDATETITLE

    if (CHAR_getInt(fd_charaindex, CHAR_WHICHTYPE) == CHAR_TYPEPLAYER &&
            CHAR_getInt(fd_charaindex, CHAR_TITLE_DEFAULT)  > 0)
    {
        char msg[256];
        sprintf(msg, "4|%d", CHAR_getInt(fd_charaindex, CHAR_TITLE_DEFAULT));
        lssproto_CHAREFFECT_send(getfdFromCharaIndex(fd_charaindex), msg);
    }

#endif
#ifdef _FAMILYBADGE_

    if (CHAR_getInt(fd_charaindex, CHAR_WHICHTYPE) == CHAR_TYPEPLAYER &&
            getFamilyBadge(fd_charaindex))
    {
        char msg[256];
        sprintf(msg, "1|%d", getFamilyBadge(fd_charaindex));
        lssproto_CHAREFFECT_send(fd, msg);
    }

#endif
}

/*------------------------------------------------------------
 ------------------------------------------------------------*/
void lssproto_BU_recv(int fd, int dummy)
{
    int fd_charaindex;
    CHECKFD;
    fd_charaindex = CONNECT_getCharaindex(fd);
    // shan 2001/12/25
    //BATTLE_WatchStop( fd_charaindex );
}

void lssproto_B_recv(int fd, char *command)
{
    int fd_charaindex;
    int battle_index;//ttom++
    CHECKFD;
    fd_charaindex = CONNECT_getCharaindex(fd);
    BattleCommandDispach(fd, command);
    // Nuke +1 0827: Battle acceleration
    battle_index = CHAR_getWorkInt(fd_charaindex, CHAR_WORKBATTLEINDEX);

    if (BATTLE_CHECKINDEX(battle_index) == FALSE)
    {
        return;
    }
}

void lssproto_FS_recv(int fd, int flg)
{
    int fd_charaindex;
    CHECKFDANDTIME;
    fd_charaindex = CONNECT_getCharaindex(fd);
    /* 椭瘀反竣卞六内卞公及引引白仿弘凳蕙 */
    CHAR_setFlg(fd_charaindex, CHAR_ISPARTY,
                (flg & CHAR_FS_PARTY) ? TRUE : FALSE);
//  CHAR_setFlg( fd_charaindex, CHAR_ISBATTLE,
//              (flg & CHAR_FS_BATTLE )? TRUE:FALSE);
#ifdef _BATTLE_PK_TYPE

    if ((CHAR_getFlg(fd_charaindex, CHAR_ISDUEL) == FALSE) && (flg & CHAR_FS_DUEL))
    {
        CHAR_setWorkInt(fd_charaindex, CHAR_WORK_BATTLEPKTYPE, 0);
        char message[256];
        char    buf[256];
        sprintf(message, "请选择你所希望的PK类型\n"
                "正常PK\n"
                "混合单P\n"
                "混合团P\n"
#ifdef _CHAR_PROFESSION
                "有职业单P\n"
                "有职业团P\n"
                "无职业单P\n"
                "无职业团P\n"
#endif
               );
        lssproto_WN_send(fd, WINDOW_MESSAGETYPE_SELECT,
                         WINDOW_BUTTONTYPE_OKCANCEL,
                         CHAR_WINDOWTYPE_BATTLEPKTYPE,
                         -1,
                         makeEscapeString(message, buf, sizeof(buf)));
    }

#endif
    CHAR_setFlg(fd_charaindex, CHAR_ISDUEL,
                (flg & CHAR_FS_DUEL) ? TRUE : FALSE);
    CHAR_setFlg(fd_charaindex, CHAR_ISPARTYCHAT,
                (flg & CHAR_FS_PARTYCHAT) ? TRUE : FALSE);
    CHAR_setFlg(fd_charaindex, CHAR_ISTRADECARD,
                (flg & CHAR_FS_TRADECARD) ? TRUE : FALSE);
#ifdef _CHANNEL_MODIFY
    //密语频道开关
    CHAR_setFlg(fd_charaindex, CHAR_ISTELL, (flg & CHAR_FS_TELL) ? TRUE : FALSE);
    //家族频道开关
    CHAR_setFlg(fd_charaindex, CHAR_ISFM, (flg & CHAR_FS_FM) ? TRUE : FALSE);
    //职业频道开关
    CHAR_setFlg(fd_charaindex, CHAR_ISOCC, (flg & CHAR_FS_OCC) ? TRUE : FALSE);
    //储存对话开关
    CHAR_setFlg(fd_charaindex, CHAR_ISSAVE, (flg & CHAR_FS_SAVE) ? TRUE : FALSE);
#ifdef _THE_WORLD_SEND
    //世界频道开关    //山猪修改世界聊天频道占用
    CHAR_setFlg(fd_charaindex, CHAR_ISWORLD, (flg & CHAR_FS_WORLD) ? TRUE : FALSE);
    //if(CHAR_getFlg(i, CHAR_ISOCC)==TRUE)
    //{
    //  CHAR_talkToCli( fd_charaindex, -1, "世界频道开启成功!", CHAR_COLORRED);
    //}
    //else
    //{
    //  CHAR_talkToCli( fd_charaindex, -1, "世界频道关闭成功!", CHAR_COLORRED);
    //}
#endif
#endif
    // CoolFish: Trade 2001/4/18
    CHAR_setFlg(fd_charaindex, CHAR_ISTRADE,
                (flg & CHAR_FS_TRADE) ? TRUE : FALSE);
#ifdef _NEW_FS_TYPE
    CHAR_setFlg(fd_charaindex, CHAR_ISEFFECT,
                (flg & CHAR_FS_EFFECT) ? TRUE : FALSE);
    CHAR_setFlg(fd_charaindex, CHAR_ISPLAYER,
                (flg & CHAR_FS_PLAYER) ? TRUE : FALSE);
    CHAR_setFlg(fd_charaindex, CHAR_ISTEAMFULL,
                (flg & CHAR_FS_TEAMFULL) ? TRUE : FALSE);
    CHAR_setFlg(fd_charaindex, CHAR_ISSHOWPET,
                (flg & CHAR_FS_SHOWPET) ? TRUE : FALSE);
    CHAR_setFlg(fd_charaindex, CHAR_ISAUTOPARTY,
                (flg & CHAR_FS_AUTOPARTY) ? TRUE : FALSE);
    CHAR_setFlg(fd_charaindex, CHAR_ISALLWORLD,
                (flg & CHAR_FS_ALLWORLD) ? TRUE : FALSE);
    CHAR_setFlg(fd_charaindex, CHAR_ISDRAWEFFECT,
                (flg & CHAR_FS_DRAW_EFFECT) ? TRUE : FALSE);
#endif // _NEW_FS_TYPE
    /*
    if (CHAR_getFlg(fd_charaindex, CHAR_ISTRADECARD) == TRUE)
        CHAR_setFlg(fd_charaindex, CHAR_ISTRADE, FALSE);
    if (CHAR_getFlg(fd_charaindex, CHAR_ISTRADE) == TRUE)
        CHAR_setFlg(fd_charaindex, CHAR_ISTRADECARD, FALSE);
    */
    lssproto_FS_send(fd, flg);
}
/*------------------------------------------------------------
 ------------------------------------------------------------*/
void lssproto_PR_recv(int fd, int x, int y, int request)
{
    int result = FALSE;
    int fd_charaindex;
    CHECKFDANDTIME;
    fd_charaindex = CONNECT_getCharaindex(fd);

    if (!CHAR_CHECKINDEX(fd_charaindex))
    {
        return;
    }

    if (CHAR_getWorkInt(fd_charaindex, CHAR_WORKBATTLEMODE) != BATTLE_CHARMODE_NONE)
    {
        return;
    }

    if (CHAR_getWorkInt(fd_charaindex, CHAR_WORKTRADEMODE) != CHAR_TRADE_FREE)
    {
        return;
    }

#if 1 // 禁止组队区域

    if (request == 1)
    {
        int nowFloor;
        nowFloor = CHAR_getInt(fd_charaindex, CHAR_FLOOR);

        if (nowFloor == 31706
                || nowFloor == 10204
                || (10601 <= nowFloor && nowFloor <= 10605)
                || nowFloor == 10919 || nowFloor == 10920
                || nowFloor == 20711 || nowFloor == 20712
                || nowFloor == 1008 || nowFloor == 1021
                || nowFloor == 3008 || nowFloor == 3021
                || (nowFloor <= 8213 && nowFloor >= 8200)
                || (nowFloor >= 30017 && nowFloor <= 30021)
#ifdef _TIME_TICKET
                || check_TimeTicketMap(nowFloor)
#endif
           )
        {
            print("\n 改封包!禁止组队区域!!:%s ", CHAR_getChar(fd_charaindex, CHAR_CDKEY));
            return;
        }
    }

#endif
    {
        //ttom avoid warp at will
        int ix, iy;
        ix = CHAR_getInt(fd_charaindex, CHAR_X);
        iy = CHAR_getInt(fd_charaindex, CHAR_Y);

        if ((ix != x) || (iy != y))
        {
            //print("\n<PR>--Error!!!!");
            //print("\n<PR>origion x=%d,y=%d",ix,iy);
            //print("\n<PR>modify  X=%d,Y=%d",x,y);
            x = ix;
            y = iy;
        }
    }
    CHAR_setMyPosition(fd_charaindex, x, y, TRUE);

    if (request == 0)
    {
        /* 轮迩允月 */
        result = CHAR_DischargeParty(fd_charaindex, 0);
    }
    else if (request == 1)
    {
        result = CHAR_JoinParty(fd_charaindex);
    }
}
/*------------------------------------------------------------
 ------------------------------------------------------------*/
void lssproto_KS_recv(int fd, int petarray)
{
    int ret, fd_charaindex;
    CHECKFDANDTIME;
    fd_charaindex = CONNECT_getCharaindex(fd);

    if (!CHAR_CHECKINDEX(fd_charaindex))
    {
        return;
    }

    /*if(petarray != -1 && CHAR_getInt( fd_charaindex, CHAR_RIDEPET) == petarray ){
      lssproto_KS_send( fd, petarray, FALSE);
      return;
    }*/
    ret = PET_SelectBattleEntryPet(fd_charaindex, petarray);
    lssproto_KS_send(fd, petarray, ret);
}

void lssproto_SPET_recv(int fd, int standbypet)
{
    int fd_charaindex;
    int i, s_pet = 0, cnt = 0;
    CHECKFDANDTIME;
    fd_charaindex = CONNECT_getCharaindex(fd);

    if (!CHAR_CHECKINDEX(fd_charaindex))
    {
        return;
    }

    //if( CHAR_getWorkInt( fd_charaindex, CHAR_WORKBATTLEMODE) != BATTLE_CHARMODE_NONE
    //  && standbypet >= CHAR_getWorkInt( fd_charaindex, CHAR_WORKSTANDBYPET) ) {
    //  print("\n 改封包!??战斗中用SPET增加待机宠!!:%s ", CHAR_getChar( fd_charaindex, CHAR_CDKEY) );
    //  return;
    //}

    //if( CHAR_getInt( fd_charaindex, CHAR_RIDEPET) == petarray ) {
    //  lssproto_SPET_send( fd, petarray, FALSE);
    //}

    for (i = 0; i < CHAR_MAXPETHAVE; i++)
    {
        if (standbypet & (1 << i))
        {
            if (CHAR_getInt(fd_charaindex, CHAR_RIDEPET) == i)
            {
                continue;
            }

            cnt++;
            //if( cnt > 4 ) {
            //  print("\n 改封包!待机宠超过数量!!:%s ", CHAR_getChar( fd_charaindex, CHAR_CDKEY) );
            //lssproto_SPET_send( fd, s_pet, FALSE);
            //  break;
            //}
            s_pet |= (1 << i);
        }

#ifdef  _PETSTANDY_ABDUCT
        else
        {
            if (CHAR_getInt(fd_charaindex, CHAR_RIDEPET) == i)
            {
                continue;
            }

            int pindex = CHAR_getCharPet(fd_charaindex, i);

            if (CHAR_CHECKINDEX(pindex))
            {
                if (CHAR_getWorkInt(pindex, CHAR_WORK_PETABDUCTTYPE) == 1)
                {
                    s_pet |= (1 << i);
                    CHAR_setWorkInt(pindex, CHAR_WORK_PETABDUCTTYPE, 0);
                }
            }
        }

#endif
    }

    CHAR_setWorkInt(fd_charaindex, CHAR_WORKSTANDBYPET, s_pet);
    lssproto_SPET_send(fd, s_pet, TRUE);
}

/*------------------------------------------------------------
 ------------------------------------------------------------*/
void lssproto_AC_recv(int fd, int x, int y, int actionno)
{
    if (CONNECT_checkfd(fd) == FALSE)
    {
        return;
    }

    int fd_charaindex;
    CHECKFDANDTIME;
    fd_charaindex = CONNECT_getCharaindex(fd);
    {
        //ttom avoid the warp at will
        Char *ch;
        ch = CHAR_getCharPointer(fd_charaindex);

        // CoolFish: +1 2001/11/05
        if (!ch)
        {
            return;
        }

        if ((ch->data[CHAR_X] != x) || (ch->data[CHAR_Y] != y))
        {
            return;
        }
    }
#ifdef _ALLBLUES_LUA_1_9
    FreeAction(fd_charaindex, x, y, actionno);
#endif
    CHAR_setMyPosition(fd_charaindex, x, y, TRUE);
    CHAR_sendAction(fd_charaindex, actionno, FALSE);
    return;
}
/*------------------------------------------------------------
 ------------------------------------------------------------*/
void lssproto_MU_recv(int fd, int x, int y, int array, int toindex)
{
    int     to_charaindex = -1, fd_charaindex;
    CHECKFDANDTIME;
    fd_charaindex = CONNECT_getCharaindex(fd);
    {
        //ttom avoid warp at will
        int ix, iy;
        ix = CHAR_getInt(fd_charaindex, CHAR_X);
        iy = CHAR_getInt(fd_charaindex, CHAR_Y);

        if ((ix != x) || (iy != y))
        {
            //print("\n<MU>--Error!!!!");
            //print("\n<MU>origion x=%d,y=%d",ix,iy);
            //print("\n<MU>modify  X=%d,Y=%d",x,y);
            x = ix;
            y = iy;
        }
    }
    CHAR_setMyPosition(fd_charaindex, x, y, TRUE);
    to_charaindex = Callfromcli_Util_getTargetCharaindex(fd, toindex);
    MAGIC_Use(fd_charaindex, array, to_charaindex);
}

void lssproto_JB_recv(int fd, int x, int y)
{
    int charaindex, floor;
    CHECKFDANDTIME;
    charaindex = CONNECT_getCharaindex(fd);
    {
        int ix, iy;
        ix = CHAR_getInt(charaindex, CHAR_X);
        iy = CHAR_getInt(charaindex, CHAR_Y);

        if ((ix != x) || (iy != y))
        {
            x = ix;
            y = iy;
        }
    }
    CHAR_setMyPosition(charaindex, x, y, TRUE);

    if (CHAR_CHECKINDEX(charaindex) == FALSE)
    {
        return;
    }

#ifdef _BATTLE_LOOK_
    BATTLE_RescueTry(charaindex);
#else
    floor = CHAR_getInt(charaindex, CHAR_FLOOR);

    if (floor == 1007
            || floor == 2007
            || floor == 3007
            || floor == 4007
            || floor == 130
#ifdef _AUTO_PK
            || (floor == 20000 && CHAR_getWorkInt(charaindex, CHAR_WORK_AUTOPK) == -1)
#endif
#ifdef _WATCH_FLOOR
            || floor == getWatchFloor(1)
            || floor == getWatchFloor(2)
            || floor == getWatchFloor(3)
            || floor == getWatchFloor(4)
            || floor == getWatchFloor(5)
            || !strcmp(getWatchFloorCF(), "是")
#endif
       )
    {
        BATTLE_WatchTry(charaindex);
    }
    else
    {
        if (BATTLE_RescueTry(charaindex) == FALSE)
        {
            BATTLE_WatchTry(charaindex);
        }
    }

#endif
}

void lssproto_KN_recv(int fd, int havepetindex, char* data)
{
    int fd_charaindex;
    CHECKFD;
    fd_charaindex = CONNECT_getCharaindex(fd);

    // Robin 04/26 debug
    if (strlen(data) > 16)
    {
        return;
    }

    // CoolFish: Prevent Trade Cheat 2001/4/18
    if (CHAR_getWorkInt(fd_charaindex, CHAR_WORKTRADEMODE) != CHAR_TRADE_FREE)
    {
        return;
    }

    if (checkStringErr(data))
    {
        return;
    }

#ifdef _LC_CHECKNAME_

    if (FreeCheckCharName(-1, data))
    {
        CHAR_talkToCli(fd_charaindex, -1, "宠物名中请不要带有非法字符！", CHAR_COLORRED);
        return;
    }

#endif
    CHAR_inputUserPetName(fd_charaindex, havepetindex, data);
}
/*------------------------------------------------------------
 ------------------------------------------------------------*/
void lssproto_WN_recv(int fd, int x, int y, int seqno,
                      int objindex, int select, char* data)
{
    int fd_charaindex;
    CHECKFDANDTIME;

    if (checkStringErr(data))
    {
        return;
    }

    fd_charaindex = CONNECT_getCharaindex(fd);

    // CoolFish: Prevent Trade Cheat 2001/4/18
    if (CHAR_getWorkInt(fd_charaindex, CHAR_WORKTRADEMODE) != CHAR_TRADE_FREE)
    {
        return;
    }

    // Robin
    if (checkStringErr(data))
    {
        return;
    }

#ifdef _ANGEL_SUMMON

    if (seqno == CHAR_WINDOWTYPE_ANGEL_ASK)
    {
        print("\n CHAR_WINDOWTYPE_ANGEL_ASK objindex:%d select:%d data:%s ",
              objindex, select, data);

        if (select == WINDOW_BUTTONTYPE_YES)
        {
            if (AngelCreate(fd_charaindex) == FALSE)
            {
                sendAngelCleanToCli(fd);
            }
        }
        else if (select == WINDOW_BUTTONTYPE_NO)
        {
            int mindex;
            char nameinfo[64];
            mindex = checkIfAngel(fd_charaindex);
            print(" ====不接受召唤任务==== ");
            getMissionNameInfo(fd_charaindex, nameinfo);
            saacproto_ACMissionTable_send(acfd, mindex, 3, nameinfo, "");
            lssproto_WN_send(fd, WINDOW_MESSAGETYPE_MESSAGE,
                             WINDOW_BUTTONTYPE_OK, -1, -1,
                             "真是遗憾。\n少了你的帮助，看来魔族会继续危害大陆的人民了。");
            //CHAR_talkToCli( fd_charaindex, -1, "天之声：真是可惜，这可是难得的机会呀。", CHAR_COLORYELLOW);
            sendAngelCleanToCli(fd);
        }
    }

#endif
#ifdef _ITEM_PET_TALK_

    if (seqno == CHAR_WINDOWTYPE_PETTALK_MENU)
    {
        if (select == WINDOW_BUTTONTYPE_OK)
        {
            char token1[256], token2[1024];
            extern int sasql_getVipPoint(int charaindex);
            extern int sasql_setVipPoint(int charaindex, int point);
            int vipPoint = sasql_getVipPoint(fd_charaindex);

            if (vipPoint > getAllServSend())
            {
                CONNECT_getPetTalk(fd, token1, token2);
                saacproto_AllServSend_send(token1, token2
#ifdef _PLAYER_UID
                                           , ""
#endif
#ifdef _YAYA_VOICE
                                           , 0
#endif
                                          );
                sasql_setVipPoint(fd_charaindex, vipPoint - getAllServSend());
            }
            else
            {
                sprintf(token1, "【金币不足】使用星球频道要求:每次消耗%d点金币", getAllServSend());
                CHAR_talkToCli(fd_charaindex, -1, token1, CHAR_COLORRED);
            }
        }

        return;
    }

#endif
    {
        //ttom avoid the warp at will
        Char *ch;
        ch = CHAR_getCharPointer(fd_charaindex);

        // CoolFish: +1 2001/11/05
        if (!ch)
        {
            return;
        }

        if ((ch->data[CHAR_X] != x) || (ch->data[CHAR_Y] != y))
        {
            // Robin 04/20 test
            //return;
            x = ch->data[CHAR_X];
            y = ch->data[CHAR_Y];
        }

        //ttom avoid WN at will
        if (seqno == CHAR_WINDOWTYPE_NPCENEMY_START)
        {
//      Char *ch;
            OBJECT  object;
            int ix, iy, ifloor, i, j;
            int     whichtype = -1;
            int enemy = 0;
            int enemy_index;

            if (!CHECKOBJECT(objindex))
            {
                goto  FIRST;
            }

            enemy_index = OBJECT_getIndex(objindex); //ttom 11/15/2000
            ix    = ch->data[CHAR_X];
            iy    = ch->data[CHAR_Y];
            ifloor = ch->data[CHAR_FLOOR];

            for (i = iy - 1; i <= iy + 1; i++)
            {
                for (j = ix - 1; j <= ix + 1; j++)
                {
                    for (object = MAP_getTopObj(ifloor, j, i) ; object ;
                            object = NEXT_OBJECT(object))
                    {
                        int objindex = GET_OBJINDEX(object);

                        switch (OBJECT_getType(objindex))
                        {
                            case OBJTYPE_CHARA:
                                whichtype =  CHAR_getInt(OBJECT_getIndex(objindex), CHAR_WHICHTYPE);

                                if (whichtype == CHAR_TYPENPCENEMY)
                                {
                                    int i_ene_temp;
                                    i_ene_temp = OBJECT_getIndex(objindex);

                                    //print("\n<WN>--enetemp=%d",i_ene_temp);
                                    if (i_ene_temp == enemy_index)
                                    {
                                        goto START_WN;
                                    }
                                    else
                                    {
                                        enemy = 0;
                                    }
                                }

                                break;

                            case OBJTYPE_ITEM:
                                break;

                            case OBJTYPE_GOLD:
                                break;

                            default:
                                break;
                        }
                    }
                }
            }

            if (enemy == 0)
            {
FIRST:
                lssproto_EN_send(fd, FALSE, 0);
                CHAR_talkToCli(fd_charaindex, -1, "事件错误。", CHAR_COLORYELLOW);
                goto END_WN;
            }
        }
    }
    //ttom end
START_WN:
    CHAR_setMyPosition(fd_charaindex, x, y, TRUE);

    //if (CHAR_getWorkInt(fd_charaindex, CHAR_WORKBATTLEMODE) == BATTLE_CHARMODE_NONE)
    //{
    if (seqno == CHAR_WINDOWTYPE_WINDOWWARPMAN_MAIN)
    {
        if (!CONNECT_get_first_warp(fd))
        {
            select = 1;
        }
    }

    CHAR_processWindow(fd_charaindex, seqno,
                       select, objindex, makeStringFromEscaped(data));
    //}
//ttom+1
END_WN:
    return;
    //CONNECT_set_pass(fd,TRUE);//ttom
}

/*------------------------------------------------------------
 ------------------------------------------------------------*/
void lssproto_HL_recv(int fd, int flg)
{
    char    msgbuf[128];
    int     i, fd_charaindex;
    CHECKFD;
    fd_charaindex = CONNECT_getCharaindex(fd);

    if (CHAR_getWorkInt(fd_charaindex, CHAR_WORKBATTLEMODE)
            == BATTLE_CHARMODE_NONE)
    {
        return;
    }

#ifdef _LOCKHELP_OK             // (不可开) Syu ADD 锁定不可加入战斗

    if ((CHAR_getInt(fd_charaindex, CHAR_FLOOR) >= 8200 && CHAR_getInt(fd_charaindex, CHAR_FLOOR) <= 8213) ||
            (CHAR_getInt(fd_charaindex, CHAR_FLOOR) >= 30017 && CHAR_getInt(fd_charaindex, CHAR_FLOOR) <= 30021)
            || CHAR_getInt(fd_charaindex, CHAR_FLOOR) == 20000
       )
    {
        return ;
    }

#endif
#ifdef _NO_HELP_MAP
    {
        int floor = CHAR_getInt(fd_charaindex, CHAR_FLOOR);
        int i;

        for (i = 0; i < 32; i++)
        {
            if (floor == getNoHelpMap(i))
            {
                CHAR_talkToCli(fd_charaindex, -1, "【求助失败】该地图无法HELP请求小伙伴的帮助 ~囧~ 加油哦 你行的！", CHAR_COLORYELLOW);
                return;
            }
        }
    }
#endif

    if (FreeBattleHelp(fd_charaindex, flg) == FALSE)
    {
        return;
    }

    if (flg == TRUE)
    {
        BattleArray[CHAR_getWorkInt(fd_charaindex,
                                    CHAR_WORKBATTLEINDEX)].Side[
                                        CHAR_getWorkInt(fd_charaindex,
                                                CHAR_WORKBATTLESIDE)].flg |= BSIDE_FLG_HELP_OK;
        snprintf(msgbuf, sizeof(msgbuf),
                 "%s 在求救！",
                 CHAR_getChar(fd_charaindex, CHAR_NAME));
    }
    else
    {
        BattleArray[CHAR_getWorkInt(fd_charaindex,
                                    CHAR_WORKBATTLEINDEX)].Side[
                                        CHAR_getWorkInt(fd_charaindex,
                                                CHAR_WORKBATTLESIDE)].flg &= ~BSIDE_FLG_HELP_OK;
        snprintf(msgbuf, sizeof(msgbuf),
                 "%s 决定拒绝帮助。",
                 CHAR_getChar(fd_charaindex, CHAR_NAME));
    }

    for (i = 0; i < 5; i ++)
    {
        int toindex = BattleArray[CHAR_getWorkInt(
                                      fd_charaindex, CHAR_WORKBATTLEINDEX)].Side[
                          CHAR_getWorkInt(fd_charaindex,
                                          CHAR_WORKBATTLESIDE)].Entry[i].charaindex;

        if (CHAR_CHECKINDEX(toindex))
        {
            int tofd = getfdFromCharaIndex(toindex);

            if (tofd != -1)
            {
                lssproto_HL_send(tofd, flg);
            }

            /* 丢永本□斥霜耨 */
            CHAR_talkToCli(toindex, -1, msgbuf, CHAR_COLORYELLOW);
            CHAR_sendBattleEffect(toindex, ON);
        }
    }
}
/*------------------------------------------------------------
 ------------------------------------------------------------*/
void lssproto_ProcGet_recv(int fd)
{
    outputNetProcLog(fd, 1);
}
/*------------------------------------------------------------
 ------------------------------------------------------------*/
void lssproto_PlayerNumGet_recv(int fd)
{
    /*
        int     i;
        int     clicnt  =0;
        int     playercnt = 0;
        for( i = 0; i < ConnectLen; i ++ ) {
            if( CONNECT_getUse_debug(i,1017) ){
                if( CONNECT_getCtype(i) == CLI) {
                    clicnt ++;
                    if( CONNECT_getCharaindex(i) >= 0 ) playercnt++;
                }
            }
        }
        lssproto_PlayerNumGet_send( fd, clicnt * 0.6, playercnt * 0.6);
    */
}


/*------------------------------------------------------------
 ------------------------------------------------------------*/
void lssproto_LB_recv(int fd, int x, int y)
{
    int fd_charaindex;
    CHECKFDANDTIME;
    fd_charaindex = CONNECT_getCharaindex(fd);
    {
        //ttom avoid warp at will
        int ix, iy;
        ix = CHAR_getInt(fd_charaindex, CHAR_X);
        iy = CHAR_getInt(fd_charaindex, CHAR_Y);

        if ((ix != x) || (iy != y))
        {
            //print("\n<LB>--Error!!!!");
            //print("\n<LB>origion x=%d,y=%d",ix,iy);
            //print("\n<LB>modify  X=%d,Y=%d",x,y);
            x = ix;
            y = iy;
        }
    }
    CHAR_setMyPosition(fd_charaindex, x, y, TRUE);
    BATTLE_WatchTry(fd_charaindex);
}
/*------------------------------------------------------------
 ------------------------------------------------------------*/
void lssproto_Shutdown_recv(int fd, char* passwd, int min)
{
    char    buff[32];

    if (strcmp(passwd, "hogehoge") == 0)
    {
        int     i;
        int     playernum = CHAR_getPlayerMaxNum();
        snprintf(buff, sizeof(buff), "%s公告。", getGameservername());

        for (i = 0 ; i < playernum ; i++)
        {
            if (CHAR_getCharUse(i) != FALSE)
            {
                CHAR_talkToCli(i, -1, buff, CHAR_COLORYELLOW);
            }
        }

        SERVSTATE_setLimittime(min);
        SERVSTATE_setShutdown(NowTime.tv_sec);
        SERVSTATE_setDsptime(0);
    }
}
void lssproto_PMSG_recv(int fd, int index, int petindex, int itemindex,
                        char* message, int color)
{
    // CoolFish: Prevent Trade Cheat 2001/4/18
    int fd_charaindex;
    fd_charaindex = CONNECT_getCharaindex(fd);

    if (CHAR_getWorkInt(fd_charaindex, CHAR_WORKTRADEMODE) != CHAR_TRADE_FREE)
    {
        return;
    }

#ifdef _STREET_VENDOR

    if (CHAR_getWorkInt(fd_charaindex, CHAR_WORKSTREETVENDOR) != -1)
    {
        return;
    }

#endif
#ifdef _ITEM_PET_LOCKED

    if (CHAR_getInt(fd_charaindex, CHAR_LOCKED) == 1)
    {
        char message[256];
        char    buf[256];
        sprintf(message, "【为了确保您的账号财产安全】\n【请输入您的安全码进行解锁】\n\n「PS」快捷解锁命令：/safe 安全密码");
        lssproto_WN_send(fd, WINDOW_MESSAGETYPE_MESSAGEANDLINEINPUT,
                         WINDOW_BUTTONTYPE_OKCANCEL,
                         CHAR_WINDOWTYPE_ITEM_PET_LOCKED,
                         -1,
                         makeEscapeString(message, buf, sizeof(buf)));
        return;
    }
    else if (CHAR_getInt(fd_charaindex, CHAR_LOCKED) == 2)
    {
        char message[256];
        char    buf[256];
        sprintf(message, "由于您的账号在异地登录。\n系统已经自动帮您上锁，确保账号安全。\n请输入您的安全密码进行解锁。");
        lssproto_WN_send(fd, WINDOW_MESSAGETYPE_MESSAGEANDLINEINPUT,
                         WINDOW_BUTTONTYPE_OKCANCEL,
                         CHAR_WINDOWTYPE_ITEM_PET_LOCKED,
                         -1,
                         makeEscapeString(message, buf, sizeof(buf)));
        return;
    }

#endif

    if (CHAR_getWorkInt(fd_charaindex, CHAR_WORKTRADEMODE) != CHAR_TRADE_FREE)
    {
        return;
    }

    PETMAIL_sendPetMail(CONNECT_getCharaindex(fd),
                        index, petindex, itemindex, message, color);
}
/*------------------------------------------------------------
 ------------------------------------------------------------*/
void lssproto_PS_recv(int fd, int havepetindex, int havepetskill, int toindex, char* data)
{
    int charaindex = CONNECT_getCharaindex(fd);

    if (CHAR_getWorkInt(charaindex, CHAR_WORKBATTLEMODE) != BATTLE_CHARMODE_NONE)
    {
        return;
    }

    int to_charaindex = Callfromcli_Util_getTargetCharaindex(fd, toindex);
    int petindex;
    BOOL    ret;
    petindex = CHAR_getCharPet(charaindex, havepetindex);

    if (!CHAR_CHECKINDEX(petindex))
    {
        return;
    }

    ret = PETSKILL_Use(petindex, havepetskill, to_charaindex, data);
    lssproto_PS_send(fd, ret, havepetindex, havepetskill, toindex);
}
/*------------------------------------------------------------
 ------------------------------------------------------------*/
void lssproto_SP_recv(int fd, int x, int y, int dir)
{
    int fd_charaindex;
    fd_charaindex = CONNECT_getCharaindex(fd);
    {
        //ttom avoid the warp at will
        int i_x, i_y;
        i_x = CHAR_getInt(fd_charaindex, CHAR_X);
        i_y = CHAR_getInt(fd_charaindex, CHAR_Y);

        if ((i_x != x) || (i_y != y))
        {
            x = i_x;
            y = i_y;
        }
    }//ttom
    CHAR_setMyPosition_main(fd_charaindex, x, y, dir, TRUE);
}

/*------------------------------------------------------------
 * CoolFish: Trade Command 2001/4/18
 ------------------------------------------------------------*/
void lssproto_TD_recv(int fd, char* message)
{
    int fd_charaindex;
    CHECKFDANDTIME;
    fd_charaindex = CONNECT_getCharaindex(fd);
//      print(" MAP_TRADEPICKUP_check0 ");

    if (CHAR_getWorkInt(fd_charaindex, CHAR_WORKSTREETVENDOR)  != -1)
    {
        CHAR_talkToCli(fd_charaindex, -1, "摆摊状态下不能交易", CHAR_COLORYELLOW);
        return;
    }

    CHAR_Trade(fd, fd_charaindex, message);
}

/*------------------------------------------------------------
 * CoolFish: Family Command 2001/5/24
 ------------------------------------------------------------*/
void lssproto_FM_recv(int fd, char* message)
{
    int fd_charaindex;
    struct timeval recvtime;
    CHECKFDANDTIME;
    // add code by shan
    CONNECT_getLastrecvtime(fd, &recvtime);

    if (time_diff(NowTime, recvtime) < 0.5)
    {
        return;
    }

    CONNECT_setLastrecvtime(fd, &NowTime);
    fd_charaindex = CONNECT_getCharaindex(fd);

    if (checkStringErr(message))
    {
        return;
    }

    CHAR_Family(fd, fd_charaindex, message);
}

// shan 2002/01/10
void lssproto_PETST_recv(int fd,  int nPet, int sPet)
{
    int charaindex;
    int i, nums = 0;
    CHECKFDANDTIME;
    charaindex = CONNECT_getCharaindex(fd);

    if (!CHAR_CHECKINDEX(charaindex))
    {
        return;
    }

    if (CHAR_getWorkInt(charaindex, CHAR_WORKBATTLEMODE)
            != BATTLE_CHARMODE_NONE)
    {
        return ;
    }

    for (i = 0; i < 5; i++)
    {
        if (CHAR_getWorkInt(charaindex, CHAR_WORK_PET0_STAT + i) == TRUE)
        {
            nums++;
        }
    }

    if (nPet < 0)
    {
        nPet = 0;
    }

    if (nPet > 4)
    {
        nPet = 4;
    }

    if (nums <= 3)
    {
        CHAR_setWorkInt(charaindex, CHAR_WORK_PET0_STAT + nPet, sPet);
    }

    if (nPet != CHAR_getInt(charaindex, CHAR_DEFAULTPET))
    {
        lssproto_PETS_send(fd, nPet, sPet);
    }
}

#ifdef _MIND_ICON
void lssproto_MA_recv(int fd, int x, int y, int nMind)
{
    int charaindex;
    CHECKFDANDTIME;
    charaindex = CONNECT_getCharaindex(fd);

    if (!CHAR_CHECKINDEX(charaindex))
    {
        return;
    }

    if (CHAR_getWorkInt(charaindex, CHAR_WORKBATTLEMODE) != BATTLE_CHARMODE_NONE)
    {
        return ;
    }

    {
        int i_x, i_y;
        i_x = CHAR_getInt(charaindex, CHAR_X);
        i_y = CHAR_getInt(charaindex, CHAR_Y);

        if ((i_x != x) || (i_y != y))
        {
            x = i_x;
            y = i_y;
        }
    }

    //print("\nshan------------------>mind action->%d x->%d y->%d", nMind, x, y);
    CHAR_setMyPosition(charaindex, x, y, TRUE);
    CHAR_setWorkInt(charaindex, CHAR_MIND_NUM, nMind);
    CHAR_sendMindEffect(charaindex, CHAR_getWorkInt(charaindex, CHAR_MIND_NUM));

    if (CHAR_getWorkInt(charaindex, CHAR_MIND_NUM) != 101290 &&
            CHAR_getWorkInt(charaindex, CHAR_MIND_NUM) != 101294   &&
            CHAR_getWorkInt(charaindex, CHAR_MIND_NUM) != 101288)
    {
        CHAR_setWorkInt(charaindex, CHAR_MIND_NUM, 0);
    }

    //print("\nshan------------------>end");
    return;
}
#endif


#ifdef _TEAM_KICKPARTY
void lssproto_KTEAM_recv(int fd, int si)
{
    int charaindex = -1, pindex;

    if (si < 0 || si > 5)
    {
        return;
    }

    charaindex = CONNECT_getCharaindex(fd);

    if (!CHAR_CHECKINDEX(charaindex))
    {
        return;
    }

    if (CHAR_getWorkInt(charaindex, CHAR_WORKPARTYMODE) != CHAR_PARTY_LEADER)
    {
        return;
    }

    pindex = CHAR_getWorkInt(charaindex, si + CHAR_WORKPARTYINDEX1);

    if (!CHAR_CHECKINDEX(pindex))
    {
        return;
    }

    if (CHAR_getWorkInt(pindex, CHAR_WORKPARTYMODE) == CHAR_PARTY_LEADER)
    {
        return;
    }

    if (CHAR_DischargeParty(pindex, 0) == FALSE)
    {
        CHAR_talkToCli(charaindex, -1, "踢除失败！", CHAR_COLORYELLOW);
    }
    else
    {
        char buf1[256];
        sprintf(buf1, "队长[%s]将你踢除！", CHAR_getUseName(charaindex));
        CHAR_talkToCli(pindex, -1, buf1, CHAR_COLORYELLOW);
        sprintf(buf1, "将[%s]踢除出团队！", CHAR_getUseName(pindex));
        CHAR_talkToCli(charaindex, -1, buf1, CHAR_COLORYELLOW);
    }
}
#endif

#ifdef _CHATROOMPROTOCOL            // (不可开) Syu ADD 聊天室频道
void lssproto_CHATROOM_recv(int fd, char *data)
{
    ChatRoom_recvall(fd, data) ;
}
#endif

#ifdef _NEWREQUESTPROTOCOL          // (不可开) Syu ADD 新增Protocol要求细项
void lssproto_RESIST_recv(int fd)
{
    int charindex = -1 ;
    char token[256];
    charindex = CONNECT_getCharaindex(fd);

    if (!CHAR_CHECKINDEX(charindex))
    {
        return;
    }

    sprintf(token, "%d|%d|%d|%d|%d|%d|%d|%d",
            CHAR_getInt(charindex, CHAR_EARTH_RESIST),
            CHAR_getInt(charindex, CHAR_WATER_RESIST),
            CHAR_getInt(charindex, CHAR_FIRE_RESIST),
            CHAR_getInt(charindex, CHAR_WIND_RESIST),
            CHAR_getInt(charindex, CHAR_EARTH_EXP),
            CHAR_getInt(charindex, CHAR_WATER_EXP),
            CHAR_getInt(charindex, CHAR_FIRE_EXP),
            CHAR_getInt(charindex, CHAR_WIND_EXP)
           );
    lssproto_RESIST_send(fd, token) ;
}
#endif

#ifdef _OUTOFBATTLESKILL            // (不可开) Syu ADD 非战斗时技能Protocol
void lssproto_BATTLESKILL_recv(int fd, int iNum)
{
#ifdef _CHAR_PROFESSION         // WON ADD 人物职业
    int charaindex = CONNECT_getCharaindex(fd);
#ifndef _PROSKILL_OPTIMUM
    int skillindex = -1, char_pskill = -1, profession_skill = -1;
#endif

    if (!CHAR_CHECKINDEX(charaindex))
    {
        return;
    }

    if (CHAR_getWorkInt(charaindex, CHAR_WORKBATTLEMODE) != BATTLE_CHARMODE_NONE)
    {
        return;
    }

#ifndef _PROSKILL_OPTIMUM   // Robin fix cancel 此处略过职业检查, 改在 PROFESSION_SKILL_Use 中检查
    // 人物的职业
    char_pskill = CHAR_getInt(charaindex, PROFESSION_CLASS);
    skillindex = PROFESSION_SKILL_GetArray(charaindex, iNum);
    int Pskillid = skillindex;
    // 技能的职业
    profession_skill = PROFESSION_SKILL_getInt(Pskillid, PROFESSION_SKILL_PROFESSION_CLASS);

    if ((char_pskill > 0) && (char_pskill == profession_skill))
    {
#else

    if (1)
    {
#endif

        if (PROFESSION_SKILL_Use(charaindex, iNum, 0, NULL) != 1)
        {
            if (CHAR_getWorkInt(charaindex, CHAR_ENCOUNT_NUM) - (int)time(NULL) > 0)
            {
//              print("\n 玩家(%s)使用技能错误!", CHAR_getUseName( charaindex ) );
#ifdef _PROSKILL_ERR_KICK
                CONNECT_setCloseRequest(fd, 1);
#endif
            }
        }
    }

#endif
}

#endif

#ifdef _STREET_VENDOR
void lssproto_STREET_VENDOR_recv(int fd, char *message)
{
    int charaindex = CONNECT_getCharaindex(fd);

    if (!CHAR_CHECKINDEX(charaindex))
    {
        return;
    }

#ifdef _ITEM_PET_LOCKED

    /*if(CHAR_getInt(charaindex,CHAR_LOCKED)==1){
        char message[256];
        char    buf[256];
        sprintf( message, "【为了确保您的账号财产安全】\n【请输入您的安全码进行解锁】\n\n「PS」快捷解锁命令：/safe 安全密码");

        lssproto_WN_send( fd, WINDOW_MESSAGETYPE_MESSAGEANDLINEINPUT,
                        WINDOW_BUTTONTYPE_OKCANCEL,
                        CHAR_WINDOWTYPE_ITEM_PET_LOCKED,
                        -1,
                        makeEscapeString( message, buf, sizeof(buf)));

        return;
    }else if(CHAR_getInt(charaindex,CHAR_LOCKED)==2){
        char message[256];
        char    buf[256];
        sprintf( message, "由于您的账号在异地登录。\n系统已经自动帮您上锁，确保账号安全。\n请输入您的安全密码进行解锁。");

        lssproto_WN_send( fd, WINDOW_MESSAGETYPE_MESSAGEANDLINEINPUT,
                        WINDOW_BUTTONTYPE_OKCANCEL,
                        CHAR_WINDOWTYPE_ITEM_PET_LOCKED,
                        -1,
                        makeEscapeString( message, buf, sizeof(buf)));

        return;
    }*/
    if (FreeCharLock(charaindex) != 1)
    {
        return;
    }

#endif

    if (CHAR_getWorkInt(charaindex, CHAR_WORKBATTLEMODE) != BATTLE_CHARMODE_NONE)
    {
        return;
    }

    if (CHAR_getWorkInt(charaindex, CHAR_WORKPARTYMODE) != CHAR_PARTY_NONE)
    {
        CHAR_talkToCli(charaindex, -1, "组队状态下不能交易", CHAR_COLORYELLOW);
        return;
    }

    CHAR_sendStreetVendor(charaindex, message);
}
#endif

#ifdef _RIGHTCLICK
void lssproto_RCLICK_recv(int fd, int type, char* data)
{
    print("\n RCLICK_recv( type=%d data=%s) ", type, data);
}
#endif

#ifdef _JOBDAILY
void lssproto_JOBDAILY_recv(int fd, char *data)
{
    int charaindex = CONNECT_getCharaindex(fd);

    if (!CHAR_CHECKINDEX(charaindex))
    {
        return;
    }

    CHAR_JobDaily(charaindex, data);
}
#endif

#ifdef _TEACHER_SYSTEM
void lssproto_TEACHER_SYSTEM_recv(int fd, char *data)
{
    int charaindex = CONNECT_getCharaindex(fd);

    if (!CHAR_CHECKINDEX(charaindex))
    {
        return;
    }

    CHAR_Teacher_system(charaindex, data);
}
#endif

#ifdef _ASSESS_ABILITY
void lssproto_ASSESS_ABILITY_recv(int fd)
{
    int charaindex = CONNECT_getCharaindex(fd);

    if (!CHAR_CHECKINDEX(charaindex))
    {
        return;
    }

    char data[512];
    char tmp[512];
    memset(data, 0, sizeof(data));
    int i;

    for (i = 0; i < 25; i++)
    {
        sprintf(tmp, "%d|", CHAR_getInt(charaindex, CHAR_MATERIAL01 + i));
        strcat(data, tmp);
    }

    lssproto_ASSESS_ABILITY_send(fd, data);
}
#endif

#ifdef _ONLINE_SHOP
void lssproto_VIP_SHOP_recv(int fd, int type, int page)
{
    int charaindex = CONNECT_getCharaindex(fd);

    if (!CHAR_CHECKINDEX(charaindex))
    {
        return;
    }

    OnlineShop_ShowList(fd, charaindex, type, page);
}

void lssproto_VIP_SHOP_buy_recv(int fd, int type, int page, int id, int num)
{
    int charaindex = CONNECT_getCharaindex(fd);

    if (!CHAR_CHECKINDEX(charaindex))
    {
        return;
    }

    OnlineShop_Buy(fd, charaindex, type, page, id, num);
}
#endif

void lssproto_SaMenu_recv(int fd, int index)
{
    int charaindex;
    int i, nums = 0;
    CHECKFDANDTIME;
    charaindex = CONNECT_getCharaindex(fd);

    if (!CHAR_CHECKINDEX(charaindex))
    {
        return;
    }

#ifndef _NO_WAIGUA

    if (CHAR_getWorkInt(charaindex, CHAR_WORKBATTLEMODE)
            != BATTLE_CHARMODE_NONE && index != 999)
    {
        return ;
    }

#endif
    FreeSaMenu(charaindex, index);
}

void lssproto_NewCharDelete_recv(int fd, char* charname, char* passwd)
{
    char cdkey[CDKEYLEN];
    int fdid;

    if (CONNECT_isCLI(fd) == FALSE)
    {
        return;
    }

    if (CONNECT_isNOTLOGIN(fd) == FALSE)
    {
        lssproto_CharDelete_send(fd, FAILED, "Already Logged in\n");
        return;
    }

    if (strlen(passwd) < 6)
    {
        lssproto_CharDelete_send(fd, FAILED, "您输入的安全码小于6位");
        return;
    }

    CONNECT_getCdkey(fd, cdkey, sizeof(cdkey));
    fdid = CONNECT_getFdid(fd);

    if (strcmp(sasql_ItemPetLocked(cdkey, passwd), "安全锁已经成功解锁！") != 0)
    {
        lssproto_CharDelete_send(fd, FAILED, "您的安全码错误，不能删除人物！");
        return;
    }

    saacproto_ACCharDelete_send(acfd, cdkey, passwd, charname, "", fdid);
    {
        char buff[512];
        char escapebuf[1024];
        snprintf(buff, sizeof(buff), "%s_%s", cdkey, charname);
        makeEscapeString(buff, escapebuf, sizeof(escapebuf));
        saacproto_DBDeleteEntryInt_send(acfd, DB_DUELPOINT, escapebuf, fdid, 0);
        saacproto_DBDeleteEntryString_send(acfd, DB_ADDRESSBOOK, escapebuf, fdid, 0);
    }
    saacproto_Broadcast_send(acfd, cdkey, charname, "chardelete", 0);
    CONNECT_setState(fd, WHILECHARDELETE);
}

#ifdef  _OFFLINE_RETEUN
void lssproto_NEWLOGOUT_recv(int fd, int flg)
{
    if (flg == 213)
    {
        lssproto_WN_send(fd, WINDOW_MESSAGETYPE_MESSAGE,
                         WINDOW_BUTTONTYPE_YESNO,
                         CHAR_WINDOWTYPE_NEWLOGOUT,
                         -1,
                         "是否确定要原地登出？");
    }
}
#endif

#ifdef  _OFFLINE_RETEUN
void lssproto_NEWLOGOUT2_recv(int fd, int flg)
{
    if (flg == 213)
    {
        int charaindex = CONNECT_getCharaindex(fd);

        if (!CHAR_CHECKINDEX(charaindex))
        {
            return;
        }

        CHAR_logout(charaindex, TRUE);
        lssproto_CharLogout_send(fd, SUCCESSFUL, "success");
        //CONNECT_setCloseRequest( fd , 1 );
    }
}
#endif

void lssproto_MAC_recv(int fd, int flg)
{
    if (flg == 213)
    {
        int charaindex = CONNECT_getCharaindex(fd);

        if (!CHAR_CHECKINDEX(charaindex))
        {
            return;
        }

        CHAR_logout(charaindex, TRUE);
        CONNECT_setCloseRequest(fd, 1);
    }
}

#ifdef _FAMILYBADGE_
void lssproto_FamilyBadge_recv(int fd)
{
    int charaindex = CONNECT_getCharaindex(fd);

    if (!CHAR_CHECKINDEX(charaindex))
    {
        return;
    }

    LoadFamilyBadge(fd);
}
#endif

void lssproto_FsUpdate_recv(int fd)
{
    int charaindex = CONNECT_getCharaindex(fd);

    if (!CHAR_CHECKINDEX(charaindex))
    {
        return;
    }

    int flg = 0;

    if (CHAR_getFlg(charaindex, CHAR_ISPARTY))
    {
        flg |= CHAR_FS_PARTY;
    }

    if (CHAR_getFlg(charaindex, CHAR_ISDUEL))
    {
        flg |= CHAR_FS_DUEL;
    }

    if (CHAR_getFlg(charaindex, CHAR_ISPARTYCHAT))
    {
        flg |= CHAR_FS_PARTYCHAT;
    }

    if (CHAR_getFlg(charaindex, CHAR_ISTRADECARD))
    {
        flg |= CHAR_FS_TRADECARD;
    }

//#ifdef _CHATROOMPROTOCOL
//
//#ifdef _THE_WORLD_SEND
//              //山猪修改世界聊天频道占用
//              //CHAR_setFlg(i,CHAR_ISWORLD,1);
//              //if( CHAR_getFlg( i, CHAR_ISWORLD)) flg |= CHAR_FS_WORLD;
//              CHAR_setFlg(charaindex,CHAR_ISOCC,1);
//              if( CHAR_getFlg( charaindex, CHAR_ISOCC)) flg |= CHAR_FS_WORLD;
//
//#endif
//#endif
#ifdef _CHANNEL_MODIFY
    CHAR_setFlg(charaindex, CHAR_ISTELL, 1);
    flg |= CHAR_FS_TELL;

    if (CHAR_getFlg(charaindex, CHAR_ISFM))
    {
        flg |= CHAR_FS_FM;
    }

#ifdef _VERSION_NEW
    CHAR_setFlg(charaindex, CHAR_ISOCC, 1);

    if (CHAR_getFlg(charaindex, CHAR_ISOCC))
    {
        flg |= CHAR_FS_WORLD;
    }

#endif
    CHAR_setFlg(charaindex, CHAR_ISCHAT, 0);
    CHAR_setFlg(charaindex, CHAR_ISSAVE, 0);
#endif
    // CoolFish: 2001/4/18
    CHAR_setFlg(charaindex, CHAR_ISTRADE, 0);
    lssproto_FS_send(fd, flg);
}

#ifdef _PETITEM_TALKSHOW
void lssproto_PetItemTalkShow_recv(int fd, int posindex, char* posname)
{
    if (posindex < 1 || posindex > 100)
    {
        return;
    }

    char tempposname[64];
    memset(tempposname, 0, sizeof(tempposname));
    sprintf(tempposname, "[%s]", CHAR_PetItemShow[posindex - 1].name);

    if (strcmp(tempposname, posname) != 0)
    {
        return;
    }

    if (CHAR_PetItemShow[posindex - 1].postype == 1)
    {
        lssproto_ItemTalkShow_send(fd, CHAR_PetItemShow[posindex - 1].name, CHAR_PetItemShow[posindex - 1].itembuff, CHAR_PetItemShow[posindex - 1].posbmp);
    }
    else if (CHAR_PetItemShow[posindex - 1].postype == 2)
    {
        lssproto_PetTalkShow_send(fd, CHAR_PetItemShow[posindex - 1].name, CHAR_PetItemShow[posindex - 1].posbmp, CHAR_PetItemShow[posindex - 1].oldhp, CHAR_PetItemShow[posindex - 1].oldatk, CHAR_PetItemShow[posindex - 1].olddef, CHAR_PetItemShow[posindex - 1].olddex, CHAR_PetItemShow[posindex - 1].oldlv,
                                  CHAR_PetItemShow[posindex - 1].nowhp, CHAR_PetItemShow[posindex - 1].nowatk, CHAR_PetItemShow[posindex - 1].nowdef, CHAR_PetItemShow[posindex - 1].nowdex, CHAR_PetItemShow[posindex - 1].level, CHAR_PetItemShow[posindex - 1].trans, CHAR_PetItemShow[posindex - 1].earth, CHAR_PetItemShow[posindex - 1].water, CHAR_PetItemShow[posindex - 1].fire,   CHAR_PetItemShow[posindex - 1].wind, CHAR_PetItemShow[posindex - 1].zhong,
                                  CHAR_PetItemShow[posindex - 1].skillname[0], CHAR_PetItemShow[posindex - 1].skillname[1], CHAR_PetItemShow[posindex - 1].skillname[2], CHAR_PetItemShow[posindex - 1].skillname[3], CHAR_PetItemShow[posindex - 1].skillname[4], CHAR_PetItemShow[posindex - 1].skillname[5], CHAR_PetItemShow[posindex - 1].skillname[6]);
    }
}
#endif

#ifdef  _NEW_MIYU_
void lssproto_NewChat_recv(int fd, char* uid)
{
    int charaindex = CONNECT_getCharaindex(fd);

    if (!CHAR_CHECKINDEX(charaindex))
    {
        return;
    }

    int i;
    int playernum = CHAR_getPlayerMaxNum();

    for (i = 0; i < playernum; i++)
    {
        if (CHAR_CHECKINDEX(i))
        {
#ifdef _PLAYER_UID

            if (strcmp(CHAR_getChar(i, CHAR_PLAYERUID), uid) == 0)
#else
            if (strcmp(CHAR_getChar(i, CHAR_NAME), uid) == 0)
#endif
            {
                lssproto_NewChat_send(fd, uid, CHAR_getInt(i, CHAR_FACEIMAGENUMBER), CHAR_getInt(i, CHAR_TRANSMIGRATION), CHAR_getInt(i, CHAR_LV));
                return;
            }
        }
    }

    CHAR_talkToCli(charaindex, -1, "您密语的玩家不在线。", CHAR_COLORYELLOW);
}
#endif

#ifdef  _NEW_UI
void lssproto_NewUi_recv(int fd, int flg, char* data)
{
    int charaindex = CONNECT_getCharaindex(fd);

    if (!CHAR_CHECKINDEX(charaindex))
    {
        return;
    }

    char newuibuff[20480];
    memset(newuibuff, 0, sizeof(newuibuff));
    int floorid = CHAR_getInt(charaindex, CHAR_FLOOR);
    int x = CHAR_getInt(charaindex, CHAR_X);
    int y = CHAR_getInt(charaindex, CHAR_Y);
    int maxplayernum = CHAR_getPlayerMaxNum();
    int i;

    if (flg == 3)
    {
        if (CHAR_getInt(charaindex, CHAR_FLOOR) != 2005 && CHAR_getInt(charaindex, CHAR_FLOOR) != 1042
                && CHAR_getInt(charaindex, CHAR_FLOOR) != 2032 && CHAR_getInt(charaindex, CHAR_FLOOR) != 3032
                && CHAR_getInt(charaindex, CHAR_FLOOR) != 4032 && CHAR_getInt(charaindex, CHAR_FLOOR) != 5032
                && CHAR_getInt(charaindex, CHAR_FLOOR) != 6032 && CHAR_getInt(charaindex, CHAR_FLOOR) != 7032
                && CHAR_getInt(charaindex, CHAR_FLOOR) != 8032 && CHAR_getInt(charaindex, CHAR_FLOOR) != 9032)
        {
            CHAR_talkToCli(charaindex, -1, "此功能需要在渔村医院或族战中使用。", CHAR_COLORYELLOW);
            return;
        }
    }

    if ((flg >= 1 && flg <= 7) || (flg >= 21 && flg <= 27))
    {
        if (flg == 2)
        {
            if (CHAR_getWorkInt(charaindex, CHAR_WORKPARTYMODE) > 0)
            {
                CHAR_DischargeParty(charaindex, 1);
                return;
            }
        }

        if (flg == 26)
        {
            if (FreeCharLock(charaindex) != 1)
            {
                CHAR_talkToCli(charaindex, -1, "很遗憾，您目前账号尚未解锁，无法和周围进行交易。", CHAR_COLORYELLOW);
                return;
            }
        }

        if (CHAR_getWorkInt(charaindex, CHAR_WORKPARTYMODE) == 2)
        {
            if (flg % 10 == 2)
            {
                CHAR_talkToCli(charaindex, -1, "很遗憾，您目前在组队模式下，无法查看周围队伍。", CHAR_COLORYELLOW);
                return;
            }
            else if (flg % 10 == 3)
            {
                CHAR_talkToCli(charaindex, -1, "很遗憾，您目前在队员模式下，无法与周围人PK。", CHAR_COLORYELLOW);
                return;
            }
            else if (flg % 10 == 4)
            {
                CHAR_talkToCli(charaindex, -1, "很遗憾，您目前在组队模式下，无法加入求救队伍。", CHAR_COLORYELLOW);
                return;
            }
            else if (flg % 10 == 5)
            {
                CHAR_talkToCli(charaindex, -1, "很遗憾，您目前在队员模式下，无法观看周围战斗。", CHAR_COLORYELLOW);
                return;
            }
            else if (flg % 10 == 6)
            {
                CHAR_talkToCli(charaindex, -1, "很遗憾，您目前在组队模式下，无法与周围人交易。", CHAR_COLORYELLOW);
                return;
            }
            else if (flg % 10 == 7)
            {
                CHAR_talkToCli(charaindex, -1, "很遗憾，您目前在组队模式下，无法观战周围摊位。", CHAR_COLORYELLOW);
                return;
            }
        }
        else if (CHAR_getWorkInt(charaindex, CHAR_WORKPARTYMODE) == 1)
        {
            if (flg % 10 == 2)
            {
                CHAR_talkToCli(charaindex, -1, "很遗憾，您目前在组队模式下，无法查看周围队伍。", CHAR_COLORYELLOW);
                return;
            }
            else if (flg % 10 == 4)
            {
                CHAR_talkToCli(charaindex, -1, "很遗憾，您目前在组队模式下，无法加入求救队伍。", CHAR_COLORYELLOW);
                return;
            }
            else if (flg % 10 == 6)
            {
                CHAR_talkToCli(charaindex, -1, "很遗憾，您目前在组队模式下，无法与周围人交易。", CHAR_COLORYELLOW);
                return;
            }
            else if (flg % 10 == 7)
            {
                CHAR_talkToCli(charaindex, -1, "很遗憾，您目前在组队模式下，无法观战周围摊位。", CHAR_COLORYELLOW);
                return;
            }
        }

        char luafunname[7][32] = { "cardDataCall", "partyDataCall", "pkDataCall", "battleDataCall", "watchDataCall", "tradeDataCall", "streetDataCall"  };

        for (i = 0; i < maxplayernum; i++)
        {
            if (!CHAR_CHECKINDEX(i))
            {
                continue;
            }

            if (i == charaindex)
            {
                continue;
            }

            int tofloorid = CHAR_getInt(i, CHAR_FLOOR);
            int tox = CHAR_getInt(i, CHAR_X);
            int toy = CHAR_getInt(i, CHAR_Y);

            if (floorid != tofloorid)
            {
                continue;
            }

            if (flg % 10 != 3)
            {
                if (flg % 10 == 2 || flg % 10 == 4 || flg % 10 == 5)
                {
                    if (tox < x - 6 || tox > x + 6)
                    {
                        continue;
                    }

                    if (toy < y - 6 || toy > y + 6)
                    {
                        continue;
                    }
                }
                else
                {
                    if (tox < x - 3 || tox > x + 3)
                    {
                        continue;
                    }

                    if (toy < y - 3 || toy > y + 3)
                    {
                        continue;
                    }
                }
            }

            switch (flg % 10)
            {
                case 1://换片
                    {
                        if (CHAR_getFlg(i, CHAR_ISTRADECARD))
                        {
                            sprintf(newuibuff, "%s%s"
#ifdef _PLAYER_UID
                                    ",%s"
#endif // _PLAYER_UID
                                    ",%d,%d,%d|", newuibuff, CHAR_getChar(i, CHAR_NAME)
#ifdef _PLAYER_UID
                                    , CHAR_getChar(i, CHAR_PLAYERUID)
#endif // _PLAYER_UID
                                    , CHAR_getInt(i, CHAR_TRANSMIGRATION), CHAR_getInt(i, CHAR_LV), CHAR_getInt(i, CHAR_BASEIMAGENUMBER));
                        }

                        break;
                    }

                case 2://组队
                    {
                        if (CHAR_getWorkInt(charaindex, CHAR_WORKPARTYMODE) > 0)
                        {
                            CHAR_DischargeParty(charaindex, 1);
                            return;
                        }

                        if (CHAR_getWorkInt(i, CHAR_WORKPARTYMODE) <= 1)
                        {
                            if (CHAR_getFlg(i, CHAR_ISPARTY))
                            {
                                int partynum = 0;

                                if (CHAR_getWorkInt(i, CHAR_WORKPARTYMODE) == 1)
                                {
                                    int j;

                                    for (j = 0; j < 5; j++)
                                    {
                                        int partyindex = CHAR_getWorkInt(i, CHAR_WORKPARTYINDEX1 + j);

                                        if (CHAR_CHECKINDEX(partyindex))
                                        {
                                            partynum++;
                                        }
                                    }
                                }
                                else
                                {
                                    partynum = 1;
                                }

                                sprintf(newuibuff, "%s%s"
#ifdef _PLAYER_UID
                                        ",%s"
#endif // _PLAYER_UID
                                        ",%d,%d,%d,%d|", newuibuff, CHAR_getChar(i, CHAR_NAME)
#ifdef _PLAYER_UID
                                        , CHAR_getChar(i, CHAR_PLAYERUID)
#endif // _PLAYER_UID
                                        , CHAR_getInt(i, CHAR_TRANSMIGRATION), CHAR_getInt(i, CHAR_LV), CHAR_getInt(i, CHAR_BASEIMAGENUMBER), partynum);
                            }
                        }

                        break;
                    }

                case 3://PK
                    {
                        if (CHAR_getWorkInt(charaindex, CHAR_WORKBATTLEMODE) != BATTLE_CHARMODE_NONE)
                        {
                            CHAR_talkToCli(charaindex, -1, "您已经在战斗中，无法使用此功能。", CHAR_COLORYELLOW);
                            return;
                        }

                        if (CHAR_getFlg(i, CHAR_ISDUEL))
                        {
                            int partynum = 0;

                            if (CHAR_getWorkInt(i, CHAR_WORKPARTYMODE) == 1)
                            {
                                int j;

                                for (j = 0; j < 5; j++)
                                {
                                    int partyindex = CHAR_getWorkInt(i, CHAR_WORKPARTYINDEX1 + j);

                                    if (CHAR_CHECKINDEX(partyindex))
                                    {
                                        partynum++;
                                    }
                                }
                            }
                            else
                            {
                                partynum = 1;
                            }

                            sprintf(newuibuff, "%s%s"
#ifdef _PLAYER_UID
                                    ",%s"
#endif // _PLAYER_UID
                                    ",%d,%d,%d,%d|", newuibuff, CHAR_getChar(i, CHAR_NAME)
#ifdef _PLAYER_UID
                                    , CHAR_getChar(i, CHAR_PLAYERUID)
#endif // _PLAYER_UID
                                    , CHAR_getInt(i, CHAR_TRANSMIGRATION), CHAR_getInt(i, CHAR_LV), CHAR_getInt(i, CHAR_BASEIMAGENUMBER), partynum);
                        }

                        break;
                    }

                case 4://加入战斗
                    {
                        if (CHAR_getWorkInt(i, CHAR_WORKBATTLEMODE) != BATTLE_CHARMODE_NONE)
                        {
                            if (BattleArray[CHAR_getWorkInt(i,
                                                            CHAR_WORKBATTLEINDEX)].Side[
                                        CHAR_getWorkInt(i,
                                                        CHAR_WORKBATTLESIDE)].flg & BSIDE_FLG_HELP_OK)
                            {
                                int partynum = 0;

                                if (CHAR_getWorkInt(i, CHAR_WORKPARTYMODE) == 1)
                                {
                                    int j;

                                    for (j = 0; j < 5; j++)
                                    {
                                        int partyindex = CHAR_getWorkInt(i, CHAR_WORKPARTYINDEX1 + j);

                                        if (CHAR_CHECKINDEX(partyindex))
                                        {
                                            partynum++;
                                        }
                                    }
                                }
                                else
                                {
                                    partynum = 1;
                                }

                                sprintf(newuibuff, "%s%s"
#ifdef _PLAYER_UID
                                        ",%s"
#endif // _PLAYER_UID
                                        ",%d,%d,%d,%d|", newuibuff, CHAR_getChar(i, CHAR_NAME)
#ifdef _PLAYER_UID
                                        , CHAR_getChar(i, CHAR_PLAYERUID)
#endif // _PLAYER_UID
                                        , CHAR_getInt(i, CHAR_TRANSMIGRATION), CHAR_getInt(i, CHAR_LV), CHAR_getInt(i, CHAR_BASEIMAGENUMBER), partynum);
                            }
                        }

                        break;
                    }

                case 5://观战
                    {
                        if (CHAR_getWorkInt(i, CHAR_WORKBATTLEMODE) != BATTLE_CHARMODE_NONE)
                        {
                            int partynum = 0;

                            if (CHAR_getWorkInt(i, CHAR_WORKPARTYMODE) == 1)
                            {
                                int j;

                                for (j = 0; j < 5; j++)
                                {
                                    int partyindex = CHAR_getWorkInt(i, CHAR_WORKPARTYINDEX1 + j);

                                    if (CHAR_CHECKINDEX(partyindex))
                                    {
                                        partynum++;
                                    }
                                }
                            }
                            else
                            {
                                partynum = 1;
                            }

                            sprintf(newuibuff, "%s%s"
#ifdef _PLAYER_UID
                                    ",%s"
#endif // _PLAYER_UID
                                    ",%d,%d,%d,%d|", newuibuff, CHAR_getChar(i, CHAR_NAME)
#ifdef _PLAYER_UID
                                    , CHAR_getChar(i, CHAR_PLAYERUID)
#endif // _PLAYER_UID
                                    , CHAR_getInt(i, CHAR_TRANSMIGRATION), CHAR_getInt(i, CHAR_LV), CHAR_getInt(i, CHAR_BASEIMAGENUMBER), partynum);
                        }

                        break;
                    }

                case 6://交易
                    {
                        if (FreeCharLock(charaindex) != 1)
                        {
                            return;
                        }

                        if (CHAR_getFlg(i, CHAR_ISTRADE))
                        {
                            sprintf(newuibuff, "%s%s"
#ifdef _PLAYER_UID
                                    ",%s"
#endif // _PLAYER_UID
                                    ",%d,%d,%d|", newuibuff, CHAR_getChar(i, CHAR_NAME)
#ifdef _PLAYER_UID
                                    , CHAR_getChar(i, CHAR_PLAYERUID)
#endif // _PLAYER_UID
                                    , CHAR_getInt(i, CHAR_TRANSMIGRATION), CHAR_getInt(i, CHAR_LV), CHAR_getInt(i, CHAR_BASEIMAGENUMBER));
                        }

                        break;
                    }

                case 7://摆摊
                    {
                        if (FreeCharLock(charaindex) != 1)
                        {
                            return;
                        }

#ifdef _NEW_STREET_VENDOR
                        extern char streetvendor[STREET_VENDOR_NUM][32];
#endif

                        if (CHAR_getWorkInt(i, CHAR_WORKSTREETVENDOR) == 1)
                        {
                            sprintf(newuibuff, "%s%s"
#ifdef _PLAYER_UID
                                    ",%s"
#endif // _PLAYER_UID
                                    ",%s,%s|", newuibuff, CHAR_getChar(i, CHAR_NAME)
#ifdef _PLAYER_UID
                                    , CHAR_getChar(i, CHAR_PLAYERUID)
#endif // _PLAYER_UID
                                    , CHAR_getWorkChar(i, CHAR_STREETVENDOR_NAME),
#ifdef _NEW_STREET_VENDOR
                                    streetvendor[CHAR_getWorkInt(i, CHAR_WORKSTREETVENDORTYPE)]
#else
                                    "石币"
#endif // _NEW_STREET_VENDOR
                                   );
                        }

                        break;
                    }

                default:
                    {
                        break;
                    }
            }
        }

        if (flg % 10 == 2)
        {
            for (i = 0; i < 128; i++)
            {
                if (flybus[i] != -1)
                {
                    if (CHAR_getInt(flybus[i], CHAR_FLOOR) == CHAR_getInt(charaindex, CHAR_FLOOR)
                            && CHAR_getInt(flybus[i], CHAR_X) >= x - 10 && CHAR_getInt(flybus[i], CHAR_X) <= x + 10
                            && CHAR_getInt(flybus[i], CHAR_Y) >= y - 10 && CHAR_getInt(flybus[i], CHAR_Y) <= y + 10
                            && CHAR_getWorkInt(flybus[i], CHAR_NPCWORKINT5) == 0)
                    {
                        int partynum = 0;

                        if (CHAR_getWorkInt(flybus[i], CHAR_WORKPARTYMODE) == 1)
                        {
                            int j;

                            for (j = 0; j < 5; j++)
                            {
                                int partyindex = CHAR_getWorkInt(flybus[i], CHAR_WORKPARTYINDEX1 + j);

                                if (CHAR_CHECKINDEX(partyindex))
                                {
                                    partynum++;
                                }
                            }
                        }
                        else
                        {
                            partynum = 1;
                        }

                        sprintf(newuibuff, "%s%s"
#ifdef _PLAYER_UID
                                ",%s"
#endif // _PLAYER_UID
                                ",%d,%d,%d,%d|", newuibuff, CHAR_getChar(flybus[i], CHAR_NAME)
#ifdef _PLAYER_UID
                                , CHAR_getChar(flybus[i], CHAR_PLAYERUID)
#endif // _PLAYER_UID
                                , 0, 0, CHAR_getInt(flybus[i], CHAR_BASEIMAGENUMBER), partynum);
                    }
                }
                else
                {
                    break;
                }
            }
        }

        lssproto_WinLuaTo_send(fd, luafunname[(flg % 10) - 1], newuibuff);
    }
    else if (flg >= 11 && flg <= 16)
    {
        if (strlen(data) < 1)
        {
            return;
        }

        switch (flg)
        {
            case 11:
                {
                    for (i = 0; i < maxplayernum; i++)
                    {
                        if (CHAR_CHECKINDEX(i) && charaindex != i)
                        {
                            if (strcmp(data,
#ifdef _PLAYER_UID
                                       CHAR_getChar(i, CHAR_PLAYERUID)
#else
                                       CHAR_getChar(i, CHAR_NAME)
#endif // _PLAYER_UID
                                      ) == 0)
                            {
                                /*if (CHAR_getInt(charaindex, CHAR_FLOOR) != CHAR_getInt(i, CHAR_FLOOR)){
                                    CHAR_talkToCli(charaindex, -1, "对方已经不在本地图，无法换片。", CHAR_COLORYELLOW);
                                    return;
                                }
                                else{*/
                                ADDRESSBOOK_addEntryFromToindex(charaindex, i);
                                return;
                                //}
                            }
                        }
                    }

                    CHAR_talkToCli(charaindex, -1, "对方已经下线，无法换片。", CHAR_COLORYELLOW);
                    break;
                }

            case 12:
                {
                    if (CHAR_getWorkInt(charaindex, CHAR_WORKPARTYMODE) > 0)
                    {
                        CHAR_DischargeParty(charaindex, 1);
                        return;
                    }

                    for (i = 0; i < maxplayernum; i++)
                    {
                        if (CHAR_CHECKINDEX(i) && charaindex != i)
                        {
                            if (strcmp(data,
#ifdef _PLAYER_UID
                                       CHAR_getChar(i, CHAR_PLAYERUID)
#else
                                       CHAR_getChar(i, CHAR_NAME)
#endif // _PLAYER_UID
                                      ) == 0)
                            {
                                if (CHAR_getInt(charaindex, CHAR_FLOOR) != CHAR_getInt(i, CHAR_FLOOR))
                                {
                                    CHAR_talkToCli(charaindex, -1, "对方已经不在本地图，无法加入组队。", CHAR_COLORYELLOW);
                                    return;
                                }
                                else
                                {
                                    if (CHAR_getWorkInt(i, CHAR_WORKPARTYMODE) > 1)
                                    {
                                        CHAR_talkToCli(charaindex, -1, "对方不是单人或队长，无法加入组队。", CHAR_COLORYELLOW);
                                        return;
                                    }

                                    if (!CHAR_getFlg(i, CHAR_ISPARTY))
                                    {
                                        CHAR_talkToCli(charaindex, -1, "对方没有开启组队，无法加入组队。", CHAR_COLORYELLOW);
                                        return;
                                    }

                                    int partynum = 0;

                                    if (CHAR_getWorkInt(i, CHAR_WORKPARTYMODE) == 1)
                                    {
                                        int j;

                                        for (j = 0; j < 5; j++)
                                        {
                                            int partyindex = CHAR_getWorkInt(i, CHAR_WORKPARTYINDEX1 + j);

                                            if (CHAR_CHECKINDEX(partyindex))
                                            {
                                                partynum++;
                                            }
                                        }
                                    }
                                    else
                                    {
                                        partynum = 1;
                                    }

                                    if (partynum == 5)
                                    {
                                        CHAR_talkToCli(charaindex, -1, "对方组队人数已满，无法加入组队。", CHAR_COLORYELLOW);
                                        return;
                                    }

                                    int WarpRand = RAND(1, 8);
                                    int WarpX = CHAR_getInt(i, CHAR_X);
                                    int WarpY = CHAR_getInt(i, CHAR_Y);

                                    if (WarpRand == 1)
                                    {
                                        WarpX = WarpX + 1;
                                    }
                                    else if (WarpRand == 2)
                                    {
                                        WarpY = WarpY + 1;
                                    }
                                    else if (WarpRand == 3)
                                    {
                                        WarpX = WarpX - 1;
                                    }
                                    else if (WarpRand == 4)
                                    {
                                        WarpY = WarpY - 1;
                                    }
                                    else if (WarpRand == 5)
                                    {
                                        WarpX = WarpX + 1;
                                        WarpY = WarpY + 1;
                                    }
                                    else if (WarpRand == 6)
                                    {
                                        WarpX = WarpX - 1;
                                        WarpY = WarpY - 1;
                                    }
                                    else if (WarpRand == 7)
                                    {
                                        WarpX = WarpX + 1;
                                        WarpY = WarpY - 1;
                                    }
                                    else if (WarpRand == 8)
                                    {
                                        WarpX = WarpX - 1;
                                        WarpY = WarpY + 1;
                                    }

                                    if (CHAR_JoinParty_Main(charaindex, i) == TRUE)
                                    {
                                        CHAR_warpToSpecificPoint(charaindex, CHAR_getInt(charaindex, CHAR_FLOOR), WarpX, WarpY);
                                    }

                                    return;
                                }
                            }
                        }
                    }

                    for (i = 0; i < 128; i++)
                    {
                        if (flybus[i] != -1)
                        {
                            if (CHAR_getInt(flybus[i], CHAR_FLOOR) == CHAR_getInt(charaindex, CHAR_FLOOR)
                                    && CHAR_getInt(flybus[i], CHAR_X) >= x - 10 && CHAR_getInt(flybus[i], CHAR_X) <= x + 10
                                    && CHAR_getInt(flybus[i], CHAR_Y) >= y - 10 && CHAR_getInt(flybus[i], CHAR_Y) <= y + 10
                                    && CHAR_getWorkInt(flybus[i], CHAR_NPCWORKINT5) == 0
                                    && strcmp(data,
#ifdef _PLAYER_UID
                                              CHAR_getChar(flybus[i], CHAR_PLAYERUID)
#else
                                              CHAR_getChar(i, CHAR_NAME)
#endif // _PLAYER_UID
                                             ) == 0)
                            {
                                int partynum = 0;

                                if (CHAR_getWorkInt(flybus[i], CHAR_WORKPARTYMODE) == 1)
                                {
                                    int j;

                                    for (j = 0; j < 5; j++)
                                    {
                                        int partyindex = CHAR_getWorkInt(flybus[i], CHAR_WORKPARTYINDEX1 + j);

                                        if (CHAR_CHECKINDEX(partyindex))
                                        {
                                            partynum++;
                                        }
                                    }
                                }
                                else
                                {
                                    partynum = 1;
                                }

                                if (partynum == 5)
                                {
                                    CHAR_talkToCli(charaindex, -1, "对方组队人数已满，无法加入组队。", CHAR_COLORYELLOW);
                                    return;
                                }

                                int WarpRand = RAND(1, 8);
                                int WarpX = CHAR_getInt(flybus[i], CHAR_X);
                                int WarpY = CHAR_getInt(flybus[i], CHAR_Y);

                                if (WarpRand == 1)
                                {
                                    WarpX = WarpX + 1;
                                }
                                else if (WarpRand == 2)
                                {
                                    WarpY = WarpY + 1;
                                }
                                else if (WarpRand == 3)
                                {
                                    WarpX = WarpX - 1;
                                }
                                else if (WarpRand == 4)
                                {
                                    WarpY = WarpY - 1;
                                }
                                else if (WarpRand == 5)
                                {
                                    WarpX = WarpX + 1;
                                    WarpY = WarpY + 1;
                                }
                                else if (WarpRand == 6)
                                {
                                    WarpX = WarpX - 1;
                                    WarpY = WarpY - 1;
                                }
                                else if (WarpRand == 7)
                                {
                                    WarpX = WarpX + 1;
                                    WarpY = WarpY - 1;
                                }
                                else if (WarpRand == 8)
                                {
                                    WarpX = WarpX - 1;
                                    WarpY = WarpY + 1;
                                }

                                if (CHAR_JoinParty_Main(charaindex, flybus[i]) == TRUE)
                                {
                                    CHAR_warpToSpecificPoint(charaindex, CHAR_getInt(charaindex, CHAR_FLOOR), WarpX, WarpY);
                                }

                                return;
                            }
                        }
                        else
                        {
                            break;
                        }
                    }

                    CHAR_talkToCli(charaindex, -1, "对方已经下线，无法加入组队。", CHAR_COLORYELLOW);
                    break;
                }

            case 13:
                {
                    if (CHAR_getWorkInt(charaindex, CHAR_WORKBATTLEMODE) != BATTLE_CHARMODE_NONE)
                    {
                        CHAR_talkToCli(charaindex, -1, "您已经在战斗中，无法使用此功能。", CHAR_COLORYELLOW);
                        return;
                    }

                    for (i = 0; i < maxplayernum; i++)
                    {
                        if (CHAR_CHECKINDEX(i) && charaindex != i)
                        {
                            if (strcmp(data,
#ifdef _PLAYER_UID
                                       CHAR_getChar(i, CHAR_PLAYERUID)
#else
                                       CHAR_getChar(i, CHAR_NAME)
#endif // _PLAYER_UID
                                      ) == 0)
                            {
                                if (CHAR_getInt(charaindex, CHAR_FLOOR) != CHAR_getInt(i, CHAR_FLOOR))
                                {
                                    CHAR_talkToCli(charaindex, -1, "对方已经不在本地图，无法进行PK。", CHAR_COLORYELLOW);
                                    return;
                                }
                                else
                                {
                                    if (CHAR_getWorkInt(i, CHAR_WORKBATTLEMODE) != BATTLE_CHARMODE_NONE)
                                    {
                                        CHAR_talkToCli(charaindex, -1, "对方已经在战斗中，无法进行PK。", CHAR_COLORYELLOW);
                                        return;
                                    }

                                    if (CHAR_getFlg(i, CHAR_ISDUEL))
                                    {
                                        if (CHAR_getWorkInt(i, CHAR_WORKPARTYMODE) > 1)
                                        {
                                            CHAR_talkToCli(charaindex, -1, "对方不是单人或队长，无法进行PK。", CHAR_COLORYELLOW);
                                            return;
                                        }

                                        if (CHAR_getInt(i, CHAR_WHICHTYPE) != CHAR_TYPEPLAYER
#ifdef _PLAYER_NPC
                                                && CHAR_getInt(i, CHAR_WHICHTYPE) != CHAR_TYPEPLAYERNPC
#endif
                                           )
                                        {
                                            continue;
                                        }

                                        if (CHAR_getWorkInt(i, CHAR_WORKBATTLEMODE) != BATTLE_CHARMODE_NONE)
                                        {
                                            return;
                                        }

#ifdef _ALLBLUES_LUA_1_5
                                        int flg = FreeVsPlayer(charaindex, i);

                                        if (flg == 1)
                                        {
                                            lssproto_EN_send(fd, FALSE, 0);
                                            return;
                                        }
                                        else if (flg == 2)
                                        {
                                            return;
                                        }
                                        else
#endif
                                            if (!CHAR_getFlg(i, CHAR_ISDUEL))
                                            {
                                                int floor = CHAR_getInt(charaindex, CHAR_FLOOR);
#ifdef _AUTO_PK

                                                if (floor == 20000)
                                                {
                                                    if (AutoPk_PKTimeGet() > 0)
                                                    {
                                                        CHAR_talkToCli(charaindex, -1, "请等比赛开始再进行PK！", CHAR_COLORYELLOW);
                                                        return;
                                                    }
                                                    else
                                                    {
                                                        if (!CHAR_getFlg(i, CHAR_ISDUEL))
                                                        {
                                                            if (CHAR_getWorkInt(charaindex, CHAR_WORK_AUTOPK) == -1)
                                                            {
                                                                return;
                                                            }

                                                            if (CHAR_getWorkInt(i, CHAR_WORK_AUTOPK) == -1)
                                                            {
                                                                return;
                                                            }
                                                        }
                                                    }
                                                }
                                                else
#endif
#ifdef _BATTLE_FLOOR
                                                {
                                                    if (strcmp(getBattleFloorCF(), "是") != 0)
                                                    {
                                                        int j;

                                                        for (j = 0; j < 32; j++)
                                                        {
                                                            if (floor == getBattleFloor(j))
                                                            {
                                                                break;
                                                            }
                                                        }

                                                        if (j == 32)
                                                        {
                                                            return;
                                                        }
                                                    }
                                                }

#else
                                                    return;
#endif
                                            }

                                        // shan begin
                                        {
                                            int fmtype = 0;
                                            int j;

                                            for (j = 0; j < FAMILY_FMPKFLOOR; j++)
                                            {
                                                if (fmpkflnum[j].fl == CHAR_getInt(charaindex, CHAR_FLOOR))
                                                {
                                                    if (CHAR_getWorkInt(charaindex, CHAR_WORKBATTLEFLAG) == -1)
                                                    {
                                                        lssproto_EN_send(fd, FALSE, 0);
                                                        return;
                                                    }

                                                    if (CHAR_getInt(charaindex, CHAR_FMINDEX) == CHAR_getInt(i, CHAR_FMINDEX))
                                                    {
                                                        //lssproto_EN_send( fd, FALSE, 0 );
                                                        //return;
                                                        fmtype = 1;
                                                    }
                                                }
                                            }

                                            if (fmtype == 1)
                                            {
                                                return;
                                            }
                                        }
                                        BATTLE_CreateVsPlayer(charaindex, i);
                                        return;
                                    }
                                    else
                                    {
                                        CHAR_talkToCli(charaindex, -1, "对方没有开启对战，无法进行PK。", CHAR_COLORYELLOW);
                                        return;
                                    }
                                }
                            }
                        }
                    }

                    CHAR_talkToCli(charaindex, -1, "对方已经下线，无法进行PK。", CHAR_COLORYELLOW);
                    break;
                }

            case 14:
                {
                    if (CHAR_getWorkInt(charaindex, CHAR_WORKBATTLEMODE) != BATTLE_CHARMODE_NONE)
                    {
                        CHAR_talkToCli(charaindex, -1, "您已经在战斗中，无法使用此功能。", CHAR_COLORYELLOW);
                        return;
                    }

                    for (i = 0; i < maxplayernum; i++)
                    {
                        if (CHAR_CHECKINDEX(i) && charaindex != i)
                        {
                            if (strcmp(data,
#ifdef _PLAYER_UID
                                       CHAR_getChar(i, CHAR_PLAYERUID)
#else
                                       CHAR_getChar(i, CHAR_NAME)
#endif // _PLAYER_UID
                                      ) == 0)
                            {
                                if (CHAR_getInt(charaindex, CHAR_FLOOR) != CHAR_getInt(i, CHAR_FLOOR))
                                {
                                    CHAR_talkToCli(charaindex, -1, "对方已经不在本地图，无法加入战斗。", CHAR_COLORYELLOW);
                                    return;
                                }
                                else
                                {
                                    if (CHAR_getWorkInt(i, CHAR_WORKBATTLEMODE) == BATTLE_CHARMODE_NONE)
                                    {
                                        CHAR_talkToCli(charaindex, -1, "对方不在战斗中，无法加入战斗。", CHAR_COLORYELLOW);
                                        return;
                                    }

                                    if (BattleArray[CHAR_getWorkInt(i,
                                                                    CHAR_WORKBATTLEINDEX)].Side[
                                                CHAR_getWorkInt(i,
                                                                CHAR_WORKBATTLESIDE)].flg & BSIDE_FLG_HELP_OK)
                                    {
                                        int WarpRand = RAND(1, 8);
                                        int WarpX = CHAR_getInt(i, CHAR_X);
                                        int WarpY = CHAR_getInt(i, CHAR_Y);

                                        if (WarpRand == 1)
                                        {
                                            WarpX = WarpX + 1;
                                        }
                                        else if (WarpRand == 2)
                                        {
                                            WarpY = WarpY + 1;
                                        }
                                        else if (WarpRand == 3)
                                        {
                                            WarpX = WarpX - 1;
                                        }
                                        else if (WarpRand == 4)
                                        {
                                            WarpY = WarpY - 1;
                                        }
                                        else if (WarpRand == 5)
                                        {
                                            WarpX = WarpX + 1;
                                            WarpY = WarpY + 1;
                                        }
                                        else if (WarpRand == 6)
                                        {
                                            WarpX = WarpX - 1;
                                            WarpY = WarpY - 1;
                                        }
                                        else if (WarpRand == 7)
                                        {
                                            WarpX = WarpX + 1;
                                            WarpY = WarpY - 1;
                                        }
                                        else if (WarpRand == 8)
                                        {
                                            WarpX = WarpX - 1;
                                            WarpY = WarpY + 1;
                                        }

                                        CHAR_warpToSpecificPoint(charaindex, CHAR_getInt(charaindex, CHAR_FLOOR), WarpX, WarpY);

                                        if (BATTLE_RescueEntry(charaindex, i) == 0)
                                        {
                                            return;
                                        }
                                        else
                                        {
                                            CHAR_talkToCli(charaindex, -1, "无法加入战斗。", CHAR_COLORYELLOW);
                                            return;
                                        }
                                    }
                                    else
                                    {
                                        CHAR_talkToCli(charaindex, -1, "对方拒绝HELP，无法加入战斗。", CHAR_COLORYELLOW);
                                        return;
                                    }
                                }
                            }
                        }
                    }

                    CHAR_talkToCli(charaindex, -1, "对方已经下线，无法加入战斗。", CHAR_COLORYELLOW);
                    break;
                }

            case 15:
                {
                    if (CHAR_getWorkInt(charaindex, CHAR_WORKBATTLEMODE) != BATTLE_CHARMODE_NONE)
                    {
                        CHAR_talkToCli(charaindex, -1, "您已经在战斗中，无法使用此功能。", CHAR_COLORYELLOW);
                        return;
                    }

                    for (i = 0; i < maxplayernum; i++)
                    {
                        if (CHAR_CHECKINDEX(i) && charaindex != i)
                        {
                            if (strcmp(data,
#ifdef _PLAYER_UID
                                       CHAR_getChar(i, CHAR_PLAYERUID)
#else
                                       CHAR_getChar(i, CHAR_NAME)
#endif // _PLAYER_UID
                                      ) == 0)
                            {
                                if (CHAR_getInt(charaindex, CHAR_FLOOR) != CHAR_getInt(i, CHAR_FLOOR))
                                {
                                    CHAR_talkToCli(charaindex, -1, "对方已经不在本地图，无法观战。", CHAR_COLORYELLOW);
                                    return;
                                }
                                else
                                {
                                    if (CHAR_getWorkInt(i, CHAR_WORKBATTLEMODE) == BATTLE_CHARMODE_NONE)
                                    {
                                        CHAR_talkToCli(charaindex, -1, "对方不在战斗中，无法观战。", CHAR_COLORYELLOW);
                                        return;
                                    }

                                    BATTLE_WatchEntry(charaindex, i);
                                    return;
                                }
                            }
                        }
                    }

                    CHAR_talkToCli(charaindex, -1, "对方已经下线，无法进行PK。", CHAR_COLORYELLOW);
                    break;
                }

            case 16:
                {
                    if (FreeCharLock(charaindex) != 1)
                    {
                        return;
                    }

                    if (CHAR_getWorkInt(charaindex, CHAR_WORKTRADEMODE) != 0)
                    {
                        CHAR_talkToCli(charaindex, -1, "您已经在交易了，无法再交易了。", CHAR_COLORYELLOW);
                        return;
                    }

                    for (i = 0; i < maxplayernum; i++)
                    {
                        if (CHAR_CHECKINDEX(i) && charaindex != i)
                        {
                            if (strcmp(data,
#ifdef _PLAYER_UID
                                       CHAR_getChar(i, CHAR_PLAYERUID)
#else
                                       CHAR_getChar(i, CHAR_NAME)
#endif // _PLAYER_UID
                                      ) == 0)
                            {
                                if (CHAR_getInt(charaindex, CHAR_FLOOR) != CHAR_getInt(i, CHAR_FLOOR))
                                {
                                    CHAR_talkToCli(charaindex, -1, "对方已经不在本地图，无法交易。", CHAR_COLORYELLOW);
                                    return;
                                }
                                else
                                {
                                    if (CHAR_getWorkInt(i, CHAR_WORKBATTLEMODE) != BATTLE_CHARMODE_NONE)
                                    {
                                        CHAR_talkToCli(charaindex, -1, "对方已经在战斗中，无法交易。", CHAR_COLORYELLOW);
                                        return;
                                    }

                                    if (CHAR_getWorkInt(i, CHAR_WORKTRADEMODE) != 0)
                                    {
                                        CHAR_talkToCli(charaindex, -1, "对方已经在交易了，无法再交易了。", CHAR_COLORYELLOW);
                                        return;
                                    }

                                    TRADE_SearchFromIndex(fd, charaindex, i);
                                    return;
                                }
                            }
                        }
                    }

                    CHAR_talkToCli(charaindex, -1, "对方已经下线，无法交易。", CHAR_COLORYELLOW);
                    break;
                }

            default:
                {
                    break;
                }
        }
    }
}
#endif

void lssproto_Qq_recv(int fd, char* data)
{
    int charaindex = CONNECT_getCharaindex(fd);

    if (!CHAR_CHECKINDEX(charaindex))
    {
        return;
    }

    //FreeSetQqData(charaindex,data);
    if (strlen(data) > 1)
    {
        char myqqdata[1024];
        memset(myqqdata, 0, sizeof(myqqdata));
        sprintf(myqqdata, "%s", sasql_getQqData(CHAR_getChar(charaindex, CHAR_CDKEY)));

        if (strcmp(myqqdata, "error") != 0)
        {
            char buff1[16];
            char buff2[16];
            memset(buff1, 0, sizeof(buff1));
            int i = 1;

            while (getStringFromIndexWithDelim(data, "|", i, buff1, sizeof(buff1)) != FALSE)
            {
                if (strlen(buff1) > 1)
                {
                    memset(buff2, 0, sizeof(buff2));
                    sprintf(buff2, "%s|", buff1);

                    if (strstr(myqqdata, buff2) == NULL)
                    {
                        sprintf(myqqdata, "%s%s", myqqdata, buff2);
                    }
                }

                i++;
            }
        }

        if (strcmp(myqqdata, "error") != 0)
        {
            sasql_setQqData(CHAR_getChar(charaindex, CHAR_CDKEY), myqqdata);
        }
    }
}
#ifdef _SERVER_NP_
void lssproto_NP_recv(int fd, char* data)
{
    if (getServerNp() == 0)
    {
        return;
    }

    int charaindex = CONNECT_getCharaindex(fd);

    if (!CHAR_CHECKINDEX(charaindex))
    {
        CONNECT_endOne_debug(fd);
        return;
    }

    char packet[4096];
    memset(packet, 0, sizeof(packet));
    hexstrtostring(packet, data);
    UINT32 uReturn = CONNECT_getNpCheck(fd, packet, strlen(data) / 2);

    if (uReturn >= 3000)
    {
        // 断开玩家连接
        //printf("\n校验NP数据失败");
        if (uReturn == 3309)
        {
            int npcnt = CONNECT_getNpCnt(fd);

            if (npcnt >= 3)
            {
                lssproto_OfflineReturn_send(fd);
                CHAR_logout(charaindex, TRUE);
                CONNECT_setCloseRequest(fd, 1);
            }
            else
            {
                CONNECT_closeNp(fd);
                CONNECT_createNp(fd);
                CONNECT_setNpCnt(fd, npcnt + 1);
            }
        }
        else
        {
            lssproto_OfflineReturn_send(fd);
            CHAR_logout(charaindex, TRUE);
            CONNECT_setCloseRequest(fd, 1);
            //CONNECT_endOne_debug(fd);
        }
    }
    else
    {
        CONNECT_setNpTime(fd, time(NULL));
        CONNECT_setNpCnt(fd, 0);
    }
}
#endif
#ifdef _NEW_OFFLINEBATTLERETURN
void lssproto_OfflineBattle_recv(int fd, int flg)
{
    int fd_charaindex = CONNECT_getCharaindex(fd);

    if (CHAR_CHECKINDEX(fd_charaindex) == FALSE)
    {
        return;
    }

    if (CHAR_getWorkInt(fd_charaindex, CHAR_OFFLINEBATTLETIME) > 0)
    {
        if (CHAR_getWorkInt(fd_charaindex, CHAR_WORKBATTLEINDEX) > -1)
        {
            lssproto_EN_send(fd, BattleArray[CHAR_getWorkInt(fd_charaindex, CHAR_WORKBATTLEINDEX)].type, BattleArray[CHAR_getWorkInt(fd_charaindex, CHAR_WORKBATTLEINDEX)].field_no);
            CHAR_sendBattleEffect(fd_charaindex, ON);
            //lssproto_AC_recv(fd, CHAR_getInt(i,CHAR_X), CHAR_getInt(i,CHAR_Y), 3);
            BATTLE_CharSendOne(CHAR_getWorkInt(fd_charaindex, CHAR_WORKBATTLEINDEX), fd_charaindex);
            int petarray = CHAR_getInt(fd_charaindex, CHAR_DEFAULTPET);

            if (CHAR_CHECKPETINDEX(petarray))
            {
                if (!CHAR_CHECKINDEX(CHAR_getCharPet(fd_charaindex, petarray)))
                {
                    petarray = -1;
                }
            }
            else
            {
                petarray = -1;
            }

            if (petarray == -1)
            {
                if (fd != -1 && fd != acfd
                   )
                {
                    lssproto_B_send(fd, "BA|18000|0|");
                }
            }
        }

        CHAR_setWorkInt(fd_charaindex, CHAR_OFFLINEBATTLETIME, -1);
    }
    else
    {
        FreeNewPlayer(fd_charaindex);
    }
}
#endif
#ifdef _RED_MEMOY_
void lssproto_NewRedMoney_recv(int charaindex, int flg, char* buff)
{
    if (CHAR_CHECKINDEX(charaindex) == FALSE)
    {
        return;
    }

    if (flg == 1)
    {
        lssproto_NewRedMoney_send(CHAR_getWorkInt(charaindex, CHAR_WORKFD), sasql_getVipPoint(charaindex));
    }
    else if (flg == 2)
    {
        extern void CHAR_NewRedMemoy_WindowResult(int charaindex, char * data);
        CHAR_NewRedMemoy_WindowResult(charaindex, buff);
    }
}
#endif

void lssproto_GetGold_recv(int fd, int flg)
{
    int charaindex = CONNECT_getCharaindex(fd);

    if (CHAR_CHECKINDEX(charaindex) == FALSE)
    {
        return;
    }

    int gold = CHAR_getInt(charaindex, CHAR_GOLD);
    int fame = CHAR_getInt(charaindex, CHAR_FAME) / 100;
    int vigor = CHAR_getInt(charaindex, CHAR_ACTIVE);
    int vippoint = sasql_getVipPoint(charaindex);
    int petpoint = sasql_getPetPoint(charaindex);
    lssproto_GetGold_send(fd, gold, vippoint, fame, vigor, petpoint);
}

void lssproto_PlayerExt_recv(int fd, int objindex, int flg)
{
    int charaindex = CONNECT_getCharaindex(fd);

    if (CHAR_CHECKINDEX(charaindex) == FALSE)
    {
        return;
    }

    if (!CHECKOBJECT(objindex))
    {
        return;
    }

    if (flg >= 1 && flg <= 7)
    {
        int toindex = OBJECT_getIndex(objindex);

        if (CHAR_CHECKINDEX(toindex) == FALSE)
        {
            return;
        }

        if (flg == 1)
        {
            int partyindex = toindex;

            if (CHAR_getWorkInt(charaindex, CHAR_WORKPARTYMODE) > 0)
            {
                CHAR_DischargeParty(charaindex, 1);
                return;
            }

            if (CHAR_getInt(toindex, CHAR_WHICHTYPE) == CHAR_TYPEPLAYER)
            {
                if (CHAR_getInt(charaindex, CHAR_FLOOR) != CHAR_getInt(toindex, CHAR_FLOOR))
                {
                    CHAR_newMessageToCli(charaindex, -1, "对方已经不在本地图，无法加入组队。", CHAR_COLORWHITE);
                    return;
                }

                if (CHAR_getWorkInt(toindex, CHAR_WORKPARTYMODE) == 2)
                {
                    partyindex = CHAR_getWorkInt(toindex, CHAR_WORKPARTYINDEX1);
                }

                if (!CHAR_getFlg(partyindex, CHAR_ISPARTY))
                {
                    CHAR_newMessageToCli(charaindex, -1, "对方没有开启组队，无法加入组队。", CHAR_COLORWHITE);
                    return;
                }

                int partynum = 0;

                if (CHAR_getWorkInt(partyindex, CHAR_WORKPARTYMODE) == 1)
                {
                    int j;

                    for (j = 0; j < 5; j++)
                    {
                        int partyindextemp = CHAR_getWorkInt(partyindex, CHAR_WORKPARTYINDEX1 + j);

                        if (CHAR_CHECKINDEX(partyindextemp))
                        {
                            partynum++;
                        }
                    }
                }
                else
                {
                    partynum = 1;
                }

                if (partynum == 5)
                {
                    CHAR_newMessageToCli(charaindex, -1, "对方组队人数已满，无法加入组队。", CHAR_COLORWHITE);
                    return;
                }

                int WarpRand = RAND(1, 8);
                int WarpX = CHAR_getInt(partyindex, CHAR_X);
                int WarpY = CHAR_getInt(partyindex, CHAR_Y);

                if (WarpRand == 1)
                {
                    WarpX = WarpX + 1;
                }
                else if (WarpRand == 2)
                {
                    WarpY = WarpY + 1;
                }
                else if (WarpRand == 3)
                {
                    WarpX = WarpX - 1;
                }
                else if (WarpRand == 4)
                {
                    WarpY = WarpY - 1;
                }
                else if (WarpRand == 5)
                {
                    WarpX = WarpX + 1;
                    WarpY = WarpY + 1;
                }
                else if (WarpRand == 6)
                {
                    WarpX = WarpX - 1;
                    WarpY = WarpY - 1;
                }
                else if (WarpRand == 7)
                {
                    WarpX = WarpX + 1;
                    WarpY = WarpY - 1;
                }
                else if (WarpRand == 8)
                {
                    WarpX = WarpX - 1;
                    WarpY = WarpY + 1;
                }

                if (!MAP_walkAble(charaindex, CHAR_getInt(charaindex, CHAR_FLOOR), WarpX, WarpY))
                {
                    WarpX = CHAR_getInt(partyindex, CHAR_X);
                    WarpY = CHAR_getInt(partyindex, CHAR_Y);
                }

#ifdef _ALLBLUES_LUA_1_5

                if (FreePartyJoin(charaindex, partyindex) == FALSE)
                {
                    return;
                }

#endif

                if ((CHAR_getInt(charaindex, CHAR_X) - 3 >= CHAR_getInt(partyindex, CHAR_X) && CHAR_getInt(charaindex, CHAR_Y) - 3 >= CHAR_getInt(partyindex, CHAR_Y))
                        || (CHAR_getInt(charaindex, CHAR_X) + 3 <= CHAR_getInt(partyindex, CHAR_X) && CHAR_getInt(charaindex, CHAR_Y) + 3 <= CHAR_getInt(partyindex, CHAR_Y))
                        || (CHAR_getInt(charaindex, CHAR_X) - 3 >= CHAR_getInt(partyindex, CHAR_X) && CHAR_getInt(charaindex, CHAR_Y) + 3 <= CHAR_getInt(partyindex, CHAR_Y))
                        || (CHAR_getInt(charaindex, CHAR_X) + 3 <= CHAR_getInt(partyindex, CHAR_X) && CHAR_getInt(charaindex, CHAR_Y) - 3 >= CHAR_getInt(partyindex, CHAR_Y)))
                {
                }
                else
                {
                    CHAR_warpToSpecificPoint(charaindex, CHAR_getInt(charaindex, CHAR_FLOOR), WarpX, WarpY);
                }

                CHAR_JoinParty_Main(charaindex, partyindex);
                return;
            }
            else if (CHAR_getInt(toindex, CHAR_WHICHTYPE) == CHAR_TYPEBUS)
            {
                if (CHAR_getInt(toindex, CHAR_FLOOR) == CHAR_getInt(charaindex, CHAR_FLOOR)
                        && CHAR_getInt(toindex, CHAR_X) >= CHAR_getInt(charaindex, CHAR_X) - 10 && CHAR_getInt(toindex, CHAR_X) <= CHAR_getInt(charaindex, CHAR_X) + 10
                        && CHAR_getInt(toindex, CHAR_Y) >= CHAR_getInt(charaindex, CHAR_Y) - 10 && CHAR_getInt(toindex, CHAR_Y) <= CHAR_getInt(charaindex, CHAR_Y) + 10
                        && CHAR_getWorkInt(toindex, CHAR_NPCWORKINT5) == 0)
                {
                    int partynum = 0;

                    if (CHAR_getWorkInt(toindex, CHAR_WORKPARTYMODE) == 1)
                    {
                        int j;

                        for (j = 0; j < 5; j++)
                        {
                            int partyindex = CHAR_getWorkInt(toindex, CHAR_WORKPARTYINDEX1 + j);

                            if (CHAR_CHECKINDEX(partyindex))
                            {
                                partynum++;
                            }
                        }
                    }
                    else
                    {
                        partynum = 1;
                    }

                    if (partynum == 5)
                    {
                        CHAR_newMessageToCli(charaindex, -1, "对方组队人数已满，无法加入组队。", CHAR_COLORWHITE);
                        return;
                    }

                    int WarpRand = RAND(1, 8);
                    int WarpX = CHAR_getInt(toindex, CHAR_X);
                    int WarpY = CHAR_getInt(toindex, CHAR_Y);

                    if (WarpRand == 1)
                    {
                        WarpX = WarpX + 1;
                    }
                    else if (WarpRand == 2)
                    {
                        WarpY = WarpY + 1;
                    }
                    else if (WarpRand == 3)
                    {
                        WarpX = WarpX - 1;
                    }
                    else if (WarpRand == 4)
                    {
                        WarpY = WarpY - 1;
                    }
                    else if (WarpRand == 5)
                    {
                        WarpX = WarpX + 1;
                        WarpY = WarpY + 1;
                    }
                    else if (WarpRand == 6)
                    {
                        WarpX = WarpX - 1;
                        WarpY = WarpY - 1;
                    }
                    else if (WarpRand == 7)
                    {
                        WarpX = WarpX + 1;
                        WarpY = WarpY - 1;
                    }
                    else if (WarpRand == 8)
                    {
                        WarpX = WarpX - 1;
                        WarpY = WarpY + 1;
                    }

                    if (!MAP_walkAble(charaindex, CHAR_getInt(charaindex, CHAR_FLOOR), WarpX, WarpY))
                    {
                        WarpX = CHAR_getInt(toindex, CHAR_X);
                        WarpY = CHAR_getInt(toindex, CHAR_Y);
                    }

                    if ((CHAR_getInt(charaindex, CHAR_X) - 3 >= CHAR_getInt(toindex, CHAR_X) && CHAR_getInt(charaindex, CHAR_Y) - 3 >= CHAR_getInt(toindex, CHAR_Y))
                            || (CHAR_getInt(charaindex, CHAR_X) + 3 <= CHAR_getInt(toindex, CHAR_X) && CHAR_getInt(charaindex, CHAR_Y) + 3 <= CHAR_getInt(toindex, CHAR_Y))
                            || (CHAR_getInt(charaindex, CHAR_X) - 3 >= CHAR_getInt(toindex, CHAR_X) && CHAR_getInt(charaindex, CHAR_Y) + 3 <= CHAR_getInt(toindex, CHAR_Y))
                            || (CHAR_getInt(charaindex, CHAR_X) + 3 <= CHAR_getInt(toindex, CHAR_X) && CHAR_getInt(charaindex, CHAR_Y) - 3 >= CHAR_getInt(toindex, CHAR_Y)))
                    {
                    }
                    else
                    {
                        CHAR_warpToSpecificPoint(charaindex, CHAR_getInt(charaindex, CHAR_FLOOR), WarpX, WarpY);
                    }

                    int busimg = CHAR_getInt(toindex, CHAR_BASEIMAGENUMBER);

                    if ((busimg != 100355) && (busimg != 100461))
                    {
#ifdef _SHIP_MATEMO

                        if (busimg == 100348)
                        {
                            int metamo = (CHAR_getInt(charaindex, CHAR_BASEBASEIMAGENUMBER) - 100000) / 20;

                            if (metamo < 0)
                            {
                                metamo = 0;
                            }
                            else if (metamo > 11)
                            {
                                metamo = 11;
                            }

                            CHAR_setInt(charaindex, CHAR_BASEIMAGENUMBER, 102062 + metamo);
                        }
                        else
#endif
                            CHAR_setInt(charaindex, CHAR_BASEIMAGENUMBER, busimg);

                        CHAR_setInt(charaindex, CHAR_RIDEPET, -1);
                        CHAR_sendCToArroundCharacter(CHAR_getWorkInt(charaindex,
                                                     CHAR_WORKOBJINDEX));

                        if (CHAR_getInt(charaindex, CHAR_RIDEPET) != -1)
                        {
                            CHAR_setInt(charaindex, CHAR_RIDEPET, -1);
                            CHAR_send_P_StatusString(charaindex, CHAR_P_STRING_RIDEPET);
                        }
                    }

                    CHAR_JoinParty_Main(charaindex, toindex);
                    return;
                }
                else
                {
                    CHAR_newMessageToCli(charaindex, -1, "无法加入组队", CHAR_COLORWHITE);
                    return;
                }
            }
        }
        else if (flg == 2)
        {
            ADDRESSBOOK_addEntryFromToindex(charaindex, toindex);
            return;
        }
        else if (flg == 3)
        {
            if (CHAR_getWorkInt(charaindex, CHAR_WORKBATTLEMODE) != BATTLE_CHARMODE_NONE)
            {
                CHAR_newMessageToCli(charaindex, -1, "您已经在战斗中，无法使用此功能。", CHAR_COLORWHITE);
                return;
            }

            if (CHAR_getInt(charaindex, CHAR_FLOOR) != CHAR_getInt(toindex, CHAR_FLOOR))
            {
                CHAR_newMessageToCli(charaindex, -1, "对方已经不在本地图，无法进行PK。", CHAR_COLORWHITE);
                return;
            }

            if (CHAR_getWorkInt(toindex, CHAR_WORKBATTLEMODE) != BATTLE_CHARMODE_NONE)
            {
                CHAR_newMessageToCli(charaindex, -1, "对方已经在战斗中，无法进行PK。", CHAR_COLORWHITE);
                return;
            }

            if (CHAR_getWorkInt(charaindex, CHAR_WORKPARTYMODE) > 1)
            {
                return;
            }

            if (CHAR_getFlg(toindex, CHAR_ISDUEL))
            {
                if (CHAR_getWorkInt(toindex, CHAR_WORKPARTYMODE) > 1)
                {
                    CHAR_newMessageToCli(charaindex, -1, "对方不是单人或队长，无法进行PK。", CHAR_COLORWHITE);
                    return;
                }

                if (CHAR_getInt(toindex, CHAR_WHICHTYPE) != CHAR_TYPEPLAYER
#ifdef _PLAYER_NPC
                        && CHAR_getInt(toindex, CHAR_WHICHTYPE) != CHAR_TYPEPLAYERNPC
#endif
                   )
                {
                    return;
                }

                if (CHAR_getWorkInt(toindex, CHAR_WORKBATTLEMODE) != BATTLE_CHARMODE_NONE)
                {
                    return;
                }

#ifdef _ALLBLUES_LUA_1_5
                int flgtemp = FreeVsPlayer(charaindex, toindex);

                if (flgtemp == 1)
                {
                    lssproto_EN_send(fd, FALSE, 0);
                    return;
                }
                else if (flgtemp == 2)
                {
                    return;
                }
                else
#endif
                    if (!CHAR_getFlg(toindex, CHAR_ISDUEL))
                    {
                        int floor = CHAR_getInt(charaindex, CHAR_FLOOR);
#ifdef _AUTO_PK

                        if (floor == 20000)
                        {
                            if (AutoPk_PKTimeGet() > 0)
                            {
                                CHAR_newMessageToCli(charaindex, -1, "请等比赛开始再进行PK！", CHAR_COLORWHITE);
                                return;
                            }
                            else
                            {
                                if (!CHAR_getFlg(toindex, CHAR_ISDUEL))
                                {
                                    if (CHAR_getWorkInt(charaindex, CHAR_WORK_AUTOPK) == -1)
                                    {
                                        return;
                                    }

                                    if (CHAR_getWorkInt(toindex, CHAR_WORK_AUTOPK) == -1)
                                    {
                                        return;
                                    }
                                }
                            }
                        }
                        else
#endif
#ifdef _BATTLE_FLOOR
                        {
                            if (strcmp(getBattleFloorCF(), "是") != 0)
                            {
                                int j;

                                for (j = 0; j < 32; j++)
                                {
                                    if (floor == getBattleFloor(j))
                                    {
                                        break;
                                    }
                                }

                                if (j == 32)
                                {
                                    return;
                                }
                            }
                        }

#else
                            return;
#endif
                    }

                // shan begin
                {
                    int fmtype = 0;
                    int j;

                    for (j = 0; j < FAMILY_FMPKFLOOR; j++)
                    {
                        if (fmpkflnum[j].fl == CHAR_getInt(charaindex, CHAR_FLOOR))
                        {
                            if (CHAR_getWorkInt(charaindex, CHAR_WORKBATTLEFLAG) == -1)
                            {
                                lssproto_EN_send(fd, FALSE, 0);
                                return;
                            }

                            if (CHAR_getInt(charaindex, CHAR_FMINDEX) == CHAR_getInt(toindex, CHAR_FMINDEX))
                            {
                                //lssproto_EN_send( fd, FALSE, 0 );
                                //return;
                                fmtype = 1;
                            }
                        }
                    }

                    if (fmtype == 1)
                    {
                        return;
                    }
                }
                BATTLE_CreateVsPlayer(charaindex, toindex);
                return;
            }
            else
            {
                CHAR_newMessageToCli(charaindex, -1, "对方没有开启对战，无法进行PK。", CHAR_COLORWHITE);
                return;
            }
        }
        else if (flg == 4)
        {
            if (CHAR_getWorkInt(charaindex, CHAR_WORKBATTLEMODE) != BATTLE_CHARMODE_NONE)
            {
                CHAR_newMessageToCli(charaindex, -1, "您已经在战斗中，无法使用此功能。", CHAR_COLORWHITE);
                return;
            }

            if (CHAR_getInt(charaindex, CHAR_FLOOR) != CHAR_getInt(toindex, CHAR_FLOOR))
            {
                CHAR_newMessageToCli(charaindex, -1, "对方已经不在本地图，无法观战。", CHAR_COLORWHITE);
                return;
            }
            else
            {
                if (CHAR_getWorkInt(toindex, CHAR_WORKBATTLEMODE) == BATTLE_CHARMODE_NONE)
                {
                    CHAR_newMessageToCli(charaindex, -1, "对方不在战斗中，无法观战。", CHAR_COLORWHITE);
                    return;
                }

                BATTLE_WatchEntry(charaindex, toindex);
                return;
            }
        }
        else if (flg == 5)
        {
            if (CHAR_getWorkInt(charaindex, CHAR_WORKBATTLEMODE) != BATTLE_CHARMODE_NONE)
            {
                CHAR_newMessageToCli(charaindex, -1, "您已经在战斗中，无法使用此功能。", CHAR_COLORWHITE);
                return;
            }

            if (CHAR_getInt(charaindex, CHAR_FLOOR) != CHAR_getInt(toindex, CHAR_FLOOR))
            {
                CHAR_newMessageToCli(charaindex, -1, "对方已经不在本地图，无法加入战斗。", CHAR_COLORWHITE);
                return;
            }
            else
            {
                if (CHAR_getWorkInt(toindex, CHAR_WORKBATTLEMODE) == BATTLE_CHARMODE_NONE)
                {
                    CHAR_newMessageToCli(charaindex, -1, "对方不在战斗中，无法加入战斗。", CHAR_COLORWHITE);
                    return;
                }

                if (BattleArray[CHAR_getWorkInt(toindex,
                                                CHAR_WORKBATTLEINDEX)].Side[
                            CHAR_getWorkInt(toindex,
                                            CHAR_WORKBATTLESIDE)].flg & BSIDE_FLG_HELP_OK)
                {
                    int WarpRand = RAND(1, 8);
                    int WarpX = CHAR_getInt(toindex, CHAR_X);
                    int WarpY = CHAR_getInt(toindex, CHAR_Y);

                    if (WarpRand == 1)
                    {
                        WarpX = WarpX + 1;
                    }
                    else if (WarpRand == 2)
                    {
                        WarpY = WarpY + 1;
                    }
                    else if (WarpRand == 3)
                    {
                        WarpX = WarpX - 1;
                    }
                    else if (WarpRand == 4)
                    {
                        WarpY = WarpY - 1;
                    }
                    else if (WarpRand == 5)
                    {
                        WarpX = WarpX + 1;
                        WarpY = WarpY + 1;
                    }
                    else if (WarpRand == 6)
                    {
                        WarpX = WarpX - 1;
                        WarpY = WarpY - 1;
                    }
                    else if (WarpRand == 7)
                    {
                        WarpX = WarpX + 1;
                        WarpY = WarpY - 1;
                    }
                    else if (WarpRand == 8)
                    {
                        WarpX = WarpX - 1;
                        WarpY = WarpY + 1;
                    }

                    if (!MAP_walkAble(charaindex, CHAR_getInt(charaindex, CHAR_FLOOR), WarpX, WarpY))
                    {
                        WarpX = CHAR_getInt(toindex, CHAR_X);
                        WarpY = CHAR_getInt(toindex, CHAR_Y);
                    }

                    CHAR_warpToSpecificPoint(charaindex, CHAR_getInt(charaindex, CHAR_FLOOR), WarpX, WarpY);

                    if (BATTLE_RescueEntry(charaindex, toindex) == 0)
                    {
                        return;
                    }
                    else
                    {
                        CHAR_newMessageToCli(charaindex, -1, "无法加入战斗。", CHAR_COLORWHITE);
                        return;
                    }
                }
                else
                {
                    CHAR_newMessageToCli(charaindex, -1, "对方拒绝HELP，无法加入战斗。", CHAR_COLORWHITE);
                    return;
                }
            }
        }
        else if (flg == 6)
        {
            if (FreeCharLock(charaindex) != 1)
            {
                return;
            }

            if (CHAR_getWorkInt(charaindex, CHAR_WORKTRADEMODE) != 0)
            {
                CHAR_newMessageToCli(charaindex, -1, "您已经在交易了，无法再交易了。", CHAR_COLORWHITE);
                return;
            }

            if (CHAR_getInt(charaindex, CHAR_FLOOR) != CHAR_getInt(toindex, CHAR_FLOOR))
            {
                CHAR_newMessageToCli(charaindex, -1, "对方已经不在本地图，无法交易。", CHAR_COLORWHITE);
                return;
            }
            else
            {
                if (CHAR_getWorkInt(toindex, CHAR_WORKBATTLEMODE) != BATTLE_CHARMODE_NONE)
                {
                    CHAR_newMessageToCli(charaindex, -1, "对方已经在战斗中，无法交易。", CHAR_COLORWHITE);
                    return;
                }

                if (CHAR_getWorkInt(toindex, CHAR_WORKTRADEMODE) != 0)
                {
                    CHAR_newMessageToCli(charaindex, -1, "对方已经在交易了，无法再交易了。", CHAR_COLORWHITE);
                    return;
                }

                if (TRADE_SearchFromIndex(fd, charaindex, toindex) == FALSE)
                {
                    CHAR_newMessageToCli(charaindex, -1, "组队，摆摊，战斗中不能交易", CHAR_COLORWHITE);
                }

                return;
            }
        }
        else if (flg == 7)
        {
            char message[64];
#ifdef _NEW_STONEAGE_TYPE
            sprintf(message, "O|%d", objindex);
            FreeStreet(fd, message);
#else
            sprintf(message, "O|%s", CHAR_getChar(toindex, CHAR_PLAYERUID));
            CHAR_sendStreetVendor(charaindex, message);
#endif // _NEW_STONEAGE_TYPE
        }
    }
    else if (flg >= 8 && flg <= 10)
    {
        CHAR_PickUpItemObj(charaindex, objindex);
    }
    else if (flg == 11)
    {
        int toindex = OBJECT_getIndex(objindex);

        if (CHAR_CHECKINDEX(toindex) == FALSE)
        {
            return;
        }

        FreeShowPlayerPet(charaindex, toindex);
    }
    else if (flg == 12)
    {
        int toindex = OBJECT_getIndex(objindex);

        if (CHAR_CHECKINDEX(toindex) == FALSE)
        {
            return;
        }

        TeacherExt(charaindex, toindex);
    }
}

void lssproto_Mail_recv(int fd, int flg)
{
    int charaindex = CONNECT_getCharaindex(fd);

    if (CHAR_CHECKINDEX(charaindex) == FALSE)
    {
        return;
    }

    if (flg <= 0)
    {
        return;
    }

    FreeMail(charaindex, flg);
}

#ifdef _NEW_PROMPTFLG
void lssproto_NewPromptFlg_recv(int fd, int flg1, int flg2)
{
    int charaindex = CONNECT_getCharaindex(fd);

    if (CHAR_CHECKINDEX(charaindex) == FALSE)
    {
        return;
    }

    CHAR_setInt(charaindex, CHAR_NEWPROMPTFLG1, flg1);
    CHAR_setInt(charaindex, CHAR_NEWPROMPTFLG2, flg2);
}
#endif

#ifdef _PARTY_HIGH
void lssproto_PartyHigh_recv(int fd, int partytype, char* partybuff)
{
    int charaindex = CONNECT_getCharaindex(fd);

    if (CHAR_CHECKINDEX(charaindex) == FALSE)
    {
        return;
    }

    CHAR_setInt(charaindex, CHAR_PARTYTYPE, partytype);
    CHAR_setChar(charaindex, CHAR_PARTYOTHERNAME, partybuff);
    lssproto_PartyHigh_send(fd, partytype, partybuff);
}
#endif

#ifdef _TALK_PLAYER_SHOW
void lssproto_TalkPlayerShow_recv(int fd, int playerobj, char* playername)
{
    if (CHECKOBJECTUSE(playerobj))
    {
        int charaindex = OBJECT_getIndex(playerobj);

        if (CHAR_CHECKINDEX(charaindex))
        {
            if (strcmp(playername, CHAR_getChar(charaindex, CHAR_NAME)) == 0)
            {
                lssproto_TalkPlayerShow_send(fd, CHAR_getChar(charaindex, CHAR_NAME), CHAR_getInt(charaindex, CHAR_FACEIMAGENUMBER), CHAR_getInt(charaindex, CHAR_LV), CHAR_getInt(charaindex, CHAR_TRANSMIGRATION), CHAR_getInt(charaindex, CHAR_FMINDEX), CHAR_getChar(charaindex, CHAR_FMNAME), CHAR_getInt(charaindex, CHAR_FMSPRITE));
                return;
            }
            else
            {
                lssproto_TalkPlayerShow_send(fd, "", 0, 0, 0, 0, "", 0);
                CHAR_newMessageToCli(CONNECT_getCharaindex(fd), -1, "对方已下线", CHAR_COLORWHITE);
                return;
            }
        }
        else
        {
            lssproto_TalkPlayerShow_send(fd, "", 0, 0, 0, 0, "", 0);
            CHAR_newMessageToCli(CONNECT_getCharaindex(fd), -1, "对方已下线", CHAR_COLORWHITE);
            return;
        }
    }
    else
    {
        lssproto_TalkPlayerShow_send(fd, "", 0, 0, 0, 0, "", 0);
        CHAR_newMessageToCli(CONNECT_getCharaindex(fd), -1, "对方已下线", CHAR_COLORWHITE);
        return;
    }
}
#endif

void lssproto_DIN_recv(int fd, int x, int y, int itemindex, int num)
{
    int charaindex;
    CHECKFDANDTIME;
    charaindex = CONNECT_getCharaindex(fd);

    if (CHAR_getWorkInt(charaindex, CHAR_WORKTRADEMODE) != CHAR_TRADE_FREE)
    {
        return;
    }

#ifdef _STREET_VENDOR

    if (CHAR_getWorkInt(charaindex, CHAR_WORKSTREETVENDOR) != -1)
    {
        return;
    }

#endif
#ifdef _ITEM_PET_LOCKED

    if (FreeCharLock(charaindex) != 1)
    {
        return;
    }

#endif

    if (CHAR_getWorkInt(charaindex, CHAR_WORKTRADEMODE) != CHAR_TRADE_FREE)
    {
        return;
    }

    if (CHAR_getWorkInt(charaindex, CHAR_WORKBATTLEMODE) != BATTLE_CHARMODE_NONE)
    {
        return;
    }

    if (num < 1)
    {
        return;
    }

    CHAR_setMyPosition(charaindex,
                       CHAR_getInt(charaindex, CHAR_X), CHAR_getInt(charaindex, CHAR_Y), TRUE);
    CHAR_DropItem_Num(charaindex, itemindex, num);
}


#ifdef _NEWPLAYER_FUNC
void lssproto_NewPlayer_recv(int fd,char* data){
	int charaindex = CONNECT_getCharaindex(fd);
	if (!CHAR_CHECKINDEX(charaindex))
		return;
	FreeNewPlayerRecv(charaindex, data);
}
#endif
