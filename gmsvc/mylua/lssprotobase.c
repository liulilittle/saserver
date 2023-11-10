#include <string.h>
#include <stdio.h>
#include <math.h>
#include "char.h"
#include "char_base.h"
#include "mylua/base.h"
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
#include "lssproto_serv.h"

#ifdef _ALLBLUES_LUA


#define     WINDOW_BUTTONTYPE_NONE      (0)
#define     WINDOW_BUTTONTYPE_OK        (1 << 0)
#define     WINDOW_BUTTONTYPE_CANCEL    (1 << 1)
#define     WINDOW_BUTTONTYPE_YES       (1 << 2)
#define     WINDOW_BUTTONTYPE_NO        (1 << 3)
#define     WINDOW_BUTTONTYPE_PREV      (1 << 4)
#define     WINDOW_BUTTONTYPE_NEXT      (1 << 5)

#define     WINDOW_BUTTONTYPE_OKCANCEL  (WINDOW_BUTTONTYPE_OK | WINDOW_BUTTONTYPE_CANCEL)
#define     WINDOW_BUTTONTYPE_YESNO (WINDOW_BUTTONTYPE_YES | WINDOW_BUTTONTYPE_NO)


static CharBase CharBaseWindow[] =
{
    {{"对话框"},                     WINDOW_MESSAGETYPE_MESSAGE}
    , {{"输入框"},                WINDOW_MESSAGETYPE_MESSAGEANDLINEINPUT}
    , {{"选择框"},                    WINDOW_MESSAGETYPE_SELECT}
    , {{"宠物框"},                WINDOW_MESSAGETYPE_PETSELECT}
    , {{"卖道具框"},             WINDOW_MESSAGETYPE_ITEMSHOPMENU}
    , {{"买道具框"},             WINDOW_MESSAGETYPE_ITEMSHOPMAIN}
    , {{"学技能框"},                 WINDOW_MESSAGETYPE_PETSKILLSHOP}
#ifdef _NEW_MANOR_LAW
    , {{"十大气势家族"},   WINDOW_FMMESSAGETYPE_10_MEMONTUM}
    , {{"家族气势列表"},   WINDOW_FMMESSAGETYPE_FM_MEMONTUM}
#endif
    , {{"家族成员列表"},   WINDOW_FMMESSAGETYPE_DENGON}
    , {{"庄园列表"},             WINDOW_FMMESSAGETYPE_POINTLIST}
    , {{"前三大列表"},      WINDOW_FMMESSAGETYPE_TOP30DP}
    , {{"银行框"},                WINDOW_MESSAGETYPE_BANK}
    , {{"宽对话框"},             WINDOW_MESSAGETYPE_WIDEMESSAGE}
    , {{"宽输入框"},             WINDOW_MESSAGETYPE_WIDEMESSAGEANDLINEINPUT}
    , {{"拍卖框"},                WINDOW_MESSAGETYPE_AUCTIONNEW}
    , {{"拍卖买框"},             WINDOW_MESSAGETYPE_AUCTIONLIST_BUY}
    , {{"拍卖查看框"},      WINDOW_MESSAGETYPE_AUCTIONSURVEY}
    , {{"拍卖修改框"},      WINDOW_MESSAGETYPE_AUCTIONMODIFY}
    , {{"拍卖列表修改框"}, WINDOW_MESSAGETYPE_AUCTIONLIST_MODIFY}
    , {{"拍卖输入框"},      WINDOW_MESSAGETYPE_WIDEMESSAGEANDLINEINPUT}
    , { { "仓库道具框" }, WINDOW_MESSAGETYPE_POOLITEM }
    , { { "仓库宠物框" }, WINDOW_MESSAGETYPE_POOLPET }
    , { { "摊位框" }, WINDOW_MESSAGETYPE_NEWSTREET }
    , { { "摊位样式框" }, WINDOW_MESSAGETYPE_NEWSTREETTYPE }
    , { { "新选择框" }, WINDOW_MESSAGETYPE_NEWSELECT }
};

static CharBase CharBaseButton[] =
{
    {{"无按钮"},                     WINDOW_BUTTONTYPE_NONE}
    , {{"确定"},                   WINDOW_BUTTONTYPE_OK}
    , {{"取消"},                       WINDOW_BUTTONTYPE_CANCEL}
    , {{"YES"},                      WINDOW_BUTTONTYPE_YES}
    , {{"NO"},                       WINDOW_BUTTONTYPE_NO}
    , {{"上一页"},                WINDOW_BUTTONTYPE_PREV | WINDOW_BUTTONTYPE_CANCEL}
    , {{"下一页"},                WINDOW_BUTTONTYPE_NEXT | WINDOW_BUTTONTYPE_CANCEL}
};

static CharBase CharBaseSeqNo[] =
{
    {{""},                              -1}
#ifdef _NEW_STREET_VENDOR
    , {{"摆摊类型"},             CHAR_WINDOWTYPE_STREET_VENDOR_TYPE}
#endif
#ifdef _PETSKILL_CANNEDFOOD
    , {{"宠物技能选择"},   ITEM_WINDOWTYPE_SELECTPETSKILL_SELECT}
#endif
#ifdef _ITEM_OVER_LAP
    , {{"道具重叠"},             CHAR_WINDOWTYPE_ITEMOVERLAP}
#endif
    , {{"道具人物宠物"},   CHAR_WINDOWTYPE_ITETOPLAYERPET}
#ifdef _ITEM_PET_LOCKED
    , {{"安全锁"},    CHAR_WINDOWTYPE_ITEM_PET_LOCKED}
#endif
};

static CharBase CharBaseColor[] =
{
    { { "白色" }, CHAR_COLORWHITE }
    , { { "青色" }, CHAR_COLORCYAN }
    , { { "粉色" }, CHAR_COLORPURPLE }
    , { { "蓝色" }, CHAR_COLORBLUE }
    , { { "黄色" }, CHAR_COLORYELLOW }
    , { { "绿色" }, CHAR_COLORGREEN }
    , { { "红色" }, CHAR_COLORRED }
    , { { "灰白色" }, CHAR_COLORGRAY }
    , { { "灰蓝色" }, CHAR_COLORBLUE2 }
    , { { "灰绿色" }, CHAR_COLORGREEN2 }

#ifdef _NEW_FONT // <<<<<<------- 
    , { { "紫色" }, CHAR_COLORVIOLET }
    , { { "橙色" }, CHAR_COLORORANGE }
    , { { "暗黃色" }, CHAR_COLORYELLOW2 }
    , { { "金色" }, CHAR_COLORGOLDEN }
    , { { "暗橙色" }, CHAR_COLORORANGE2 }
#endif
    , { { "黑色" }, CHAR_COLORBLACK }
    , { { "随机色" }, CHAR_COLORNUM }
};

//
//static int windows_validation(lua_State *L)
//{
//  size_t l;
//  const int index = luaL_checkint(L, 1);
//  if(CHAR_CHECKINDEX(index) == FALSE) return 0;
//  int fd = getfdFromCharaIndex( index);
//  const int windowtype = getCharBaseValue(L, 2, CharBaseWindow, arraysizeof(CharBaseWindow));
//  const int buttontype = getCharBaseValue(L, 3, CharBaseButton, arraysizeof(CharBaseButton));
//  const int seqno = getCharBaseValue(L, 4, CharBaseSeqNo, arraysizeof(CharBaseSeqNo));
//  const int objindex=luaL_checkint(L, 5);
//  char *data=luaL_checklstring(L, 6, &l);
//  char *validation=luaL_checklstring(L, 7, &l);
//
///*
//  char *str=luaL_checklstring(L, 7, &l);
//
//  int validation = 0;
//  int i;
//  for(i=0; i < strlen(str); i ++){
//      validation += (((str[i] - 'A') + 1) * pow(26, i));
//  }
//
//  validation = validation << 4;
//*/
//  lssproto_CAPTCHA_send(fd,validation);
//  lssproto_WN_send(fd, windowtype, buttontype + 2048, seqno, objindex, data);
//  return 1;
//}

static int windows_send(lua_State *L)
{
    size_t l;
    const int index = luaL_checkint(L, 1);

    if (CHAR_CHECKINDEX(index) == FALSE)
    {
        return 0;
    }

    int fd = getfdFromCharaIndex(index);
    const int windowtype = getCharBaseValue(L, 2, CharBaseWindow, arraysizeof(CharBaseWindow));
    const int buttontype = getCharBaseValue(L, 3, CharBaseButton, arraysizeof(CharBaseButton));
    const int seqno = getCharBaseValue(L, 4, CharBaseSeqNo, arraysizeof(CharBaseSeqNo));
    const int objindex = luaL_checkint(L, 5);
    char *data = luaL_checklstring(L, 6, &l);
    lssproto_WN_send(fd, windowtype, buttontype, seqno, objindex, data);
    return 1;
}

static int windows_update(lua_State *L)
{
    size_t l;
    const int index = luaL_checkint(L, 1);

    if (CHAR_CHECKINDEX(index) == FALSE)
    {
        return 0;
    }

    int fd = getfdFromCharaIndex(index);
    const int windowtype = getCharBaseValue(L, 2, CharBaseWindow, arraysizeof(CharBaseWindow));
    const int buttontype = getCharBaseValue(L, 3, CharBaseButton, arraysizeof(CharBaseButton));
    const int seqno = getCharBaseValue(L, 4, CharBaseSeqNo, arraysizeof(CharBaseSeqNo));
    const int objindex = luaL_checkint(L, 5);
    char *data = luaL_checklstring(L, 6, &l);
    lssproto_WNUpdate_send(fd, windowtype, buttontype, seqno, objindex, data);
    return 1;
}

static int show(lua_State *L)
{
    size_t l;
    const int index = luaL_checkint(L, 1);

    if (CHAR_CHECKINDEX(index) == FALSE)
    {
        return 0;
    }

    int fd = getfdFromCharaIndex(index);
    char *data = luaL_checklstring(L, 2, &l);
    lssproto_C_send(fd, data);
    return 1;
}

static int FM(lua_State *L)
{
    size_t l;
    const int index = luaL_checkint(L, 1);

    if (CHAR_CHECKINDEX(index) == FALSE)
    {
        return 0;
    }

    int fd = getfdFromCharaIndex(index);
    char *data = luaL_checklstring(L, 2, &l);
    lssproto_FM_send(fd, data);
    return 1;
}
#ifdef _ADD_STATUS_2
static int S2(lua_State *L)
{
    size_t l;
    const int index = luaL_checkint(L, 1);

    if (CHAR_CHECKINDEX(index) == FALSE)
    {
        return 0;
    }

    int fd = getfdFromCharaIndex(index);
    char *data = luaL_checklstring(L, 2, &l);
    lssproto_S2_send(fd, data);
    return 1;
}
#endif

static int dengon(lua_State *L)
{
    size_t l;
    const int index = luaL_checkint(L, 1);

    if (CHAR_CHECKINDEX(index) == FALSE)
    {
        return 0;
    }

    int fd = getfdFromCharaIndex(index);
    char *data = luaL_checklstring(L, 2, &l);
    const int color = luaL_checkint(L, 3);
    const int num = luaL_checkint(L, 4);
    lssproto_DENGON_send(fd, data, color, num);
    return 1;
}

static int CharList(lua_State *L)
{
    size_t l;
    const int fd = luaL_checkint(L, 1);
    char *data = luaL_checklstring(L, 2, &l);
    lssproto_CharList_send(fd, FAILED, data);
    return 1;
}

static int ClientLogin(lua_State *L)
{
    size_t l;
    const int fd = luaL_checkint(L, 1);
    char *data = luaL_checklstring(L, 2, &l);
    lssproto_ClientLogin_send(fd, data);
    return 1;
}

static int SaMenu(lua_State *L)
{
    const int charaindex = luaL_checkint(L, 1);

    if (CHAR_CHECKINDEX(charaindex) == FALSE)
    {
        return 0;
    }

    int fd = getfdFromCharaIndex(charaindex);
    const int index = luaL_checkint(L, 2);
    size_t l;
    char *data = luaL_checklstring(L, 3, &l);
    lssproto_SAMENU_send(fd, index, data);
    return 1;
}

static int NewSaMenu(lua_State *L)
{
    const int fd = luaL_checkint(L, 1);
    const int index = luaL_checkint(L, 2);
    size_t l;
    const char *data = luaL_checklstring(L, 3, &l);
    lssproto_NewSaMenu_send(fd, index, data);
    return 1;
}
#ifdef _NEWSAMENU_WITH_CALLBACK
static int NewSaMenu2(lua_State *L)
{
    const int fd = luaL_checkint(L, 1);
    const int index = luaL_checkint(L, 2);
    size_t l;
    const char *data = luaL_checklstring(L, 3, &l);
    const char *func = luaL_checklstring(L, 4, &l);
    lssproto_NewSaMenu2_send(fd, index, data,func);
    return 1;
}
#endif
static int W2(lua_State *L)
{
    const int charaindex = luaL_checkint(L, 1);

    if (CHAR_CHECKINDEX(charaindex) == FALSE)
    {
        return 0;
    }

    int fd = getfdFromCharaIndex(charaindex);
    const int x = luaL_checkint(L, 2);
    const int y = luaL_checkint(L, 3);
    size_t l;
    char *direction = luaL_checklstring(L, 4, &l);
    lssproto_W2_recv(fd, x, y, direction);
    return 1;
}

static int B(lua_State *L)
{
    size_t l;
    const int charaindex = luaL_checkint(L, 1);

    if (CHAR_CHECKINDEX(charaindex) == FALSE)
    {
        return 0;
    }

    int fd = getfdFromCharaIndex(charaindex);
    char *data = luaL_checklstring(L, 2, &l);
    lssproto_B_recv(fd, data);
    return 1;
}

static int MI(lua_State *L)
{
    size_t l;
    const int charaindex = luaL_checkint(L, 1);
    const int fromid = luaL_checkint(L, 2);
    const int toid = luaL_checkint(L, 3);

    if (CHAR_CHECKINDEX(charaindex) == FALSE)
    {
        return 0;
    }

    int fd = getfdFromCharaIndex(charaindex);
    lssproto_MI_recv(fd, fromid, toid);
    return 1;
}
//static int MacSend(lua_State *L)
//{
//  const int charaindex = luaL_checkint(L, 1);
//  if(CHAR_CHECKINDEX(charaindex) == FALSE) return 0;
//  int fd = getfdFromCharaIndex( charaindex);
//  size_t l;
//  char *data1=luaL_checklstring(L, 2, &l);
//  char *data2=luaL_checklstring(L, 3, &l);
//  lssproto_MAC_send(fd, data1,data2);
//  return 1;
//}
#ifdef _FAMILYBADGE_
int FamilyBadgeData[200];
int FamilyBadgeDataNum;
int FamilyBadgePrice;

static int FamilyBadge(lua_State *L)
{
    size_t l;
    const int fd = luaL_checkint(L, 1);
    char *data = luaL_checklstring(L, 2, &l);
    lssproto_FamilyBadge_send(fd, data);
    return 1;
}

static int GetFBData(lua_State *L)
{
    FamilyBadgeDataNum = -1;
    int len = luaL_getn(L, 1);

    if (len > 0)
    {
        int i = 1;

        for (i; i <= len; i++)
        {
            FamilyBadgeDataNum++;
            lua_rawgeti(L, 1, i);
            FamilyBadgeData[FamilyBadgeDataNum] = (int)lua_tointeger(L, -1);
            lua_pop(L, 1);
        }
    }

    FamilyBadgePrice = luaL_checkint(L, 2);
    return 1;
}
#endif
#ifdef _WINLUATO_
extern void lssproto_WinLuaTo_send(int fd, char *funcName, char *data);
static int WinLuaToSend(lua_State *L)
{
    size_t l;
    const int fd = luaL_checkint(L, 1);
    char *funcName = luaL_checklstring(L, 2, &l);
    char *data = luaL_checklstring(L, 3, &l);
    lssproto_WinLuaTo_send(fd, funcName, data);
    return 1;
}
#endif

#ifdef _LONIN_ERROR_
extern void lssproto_LoninError_send(int fd, char *data);
static int LoninError(lua_State *L)
{
    size_t l;
    const int fd = luaL_checkint(L, 1);
    char *data = luaL_checklstring(L, 2, &l);
    lssproto_LoninError_send(fd, data);
    return 1;
}
#endif
#ifdef _IPHONE_SHOP_FLAG_
static int ShopFlag(lua_State *L)
{
    extern void lssproto_ShopFlag_send(int fd, int data);
    const int fd = luaL_checkint(L, 1);
    const int data = luaL_checkint(L, 2);
    lssproto_ShopFlag_send(fd, data);
    return 1;
}
#endif
#ifdef _SPEED_CHECK_
static int sendSpeedCheck(lua_State *L)
{
    extern void lssproto_SpeedCheck_send(int fd, int num);
    size_t l;
    const int fd = luaL_checkint(L, 1);
    const int num = luaL_checkint(L, 2);
    lssproto_SpeedCheck_send(fd, num);
    return 1;
}
#endif
#ifdef _ADDJIETU_
extern void lssproto_jiandu_send(int fd);
static int sendJianDu(lua_State *L)
{
    const int fd = luaL_checkint(L, 1);
    lssproto_jiandu_send(fd);
    return 1;
}
#endif
#ifdef _OFFLINERETURN_LOGINCLI
static int sendOfflineReturn(lua_State *L)
{
    const int fd = luaL_checkint(L, 1);
    lssproto_OfflineReturn_send(fd);
    return 1;
}
#endif
static int TK(lua_State *L)
{
    size_t l;
    const int fd = luaL_checkint(L, 1);
    const int talkchar = luaL_checkint(L, 2);
    const char* token = luaL_checklstring(L, 3, &l);
    const int color = luaL_checkint(L, 4);
#ifdef _ITEM_PET_TALK_
    const char* outitempetData = luaL_checklstring(L, 5, &l);
#ifdef _PLAYER_UID
    const char* uid = luaL_checklstring(L, 6, &l);
#endif // _PLAYER_UID
#ifdef _YAYA_VOICE
    const int volumeLen = luaL_checkint(L, 7);
#endif
#else
#ifdef _PLAYER_UID
    const char* uid = luaL_checklstring(L, 5, &l);
#endif // _PLAYER_UID
#endif
    lssproto_TK_send(fd, talkchar, token, color
#ifdef _ITEM_PET_TALK_
                     , outitempetData
#endif
#ifdef _PLAYER_UID
                     , uid
#endif
#ifdef _YAYA_VOICE
                     , volumeLen
#endif
#ifdef _TK_PLAYERFACE
                     , CHAR_getInt(OBJECT_getIndex(talkchar), CHAR_FACEIMAGENUMBER)
                     , CHAR_getInt(OBJECT_getIndex(talkchar), CHAR_LV)
#endif // _TK_PLAYERFACE
                    );
    return 1;
}

static int sendNewMessage(lua_State *L)
{
    size_t l;
    const int fd = luaL_checkint(L, 1);
    const char* token = luaL_checklstring(L, 2, &l);
    const int color = luaL_checkint(L, 3);
    lssproto_NewMessage_send(fd, token, color);
    return 1;
}
static int sendMail(lua_State *L)
{
    size_t l;
    const int fd = luaL_checkint(L, 1);
    const int id = luaL_checkint(L, 2);
    const int type = luaL_checkint(L, 3);
    const char* zhuti = luaL_checklstring(L, 4, &l);
    const char* neirong = luaL_checklstring(L, 5, &l);
    const char* itembuff = luaL_checklstring(L, 6, &l);
    lssproto_Mail_send(fd, id, type, zhuti, neirong, itembuff);
    return 1;
}
static int deint(lua_State *L)
{
    const int fd = luaL_checkint(L, 1);
    const int num = luaL_checkint(L, 2);
    int number;
    int count = util_deint(fd, num, &number);
    lua_pushinteger(L, number);
    lua_pushinteger(L, count);
    return 2;
}
static int destring(lua_State *L)
{
    const int fd = luaL_checkint(L, 1);
    const int num = luaL_checkint(L, 2);
    char data[1024 * 64];
    memset(data, 0, sizeof(data));
    int count = util_destring(fd, num, data);
    lua_pushstring(L, data);
    lua_pushinteger(L, count);
    return 2;
}

static char buffer[65500];
static int mkbuffer(lua_State *L)
{
    const int fd = luaL_checkint(L, 1);
    CONNECT_getCdkey(fd, PersonalKey, 4096);
    strcat(PersonalKey, getRandDefaultKey(fd));
    memset(buffer, 0, sizeof(buffer));
    return 0;
}
static int mkint(lua_State *L)
{
    const int fd = luaL_checkint(L, 1);
    const int number = luaL_checkint(L, 2);
    int count = util_mkint(fd, buffer, number);
    lua_pushinteger(L, count);
    return 1;
}
static int mkstring(lua_State *L)
{
    size_t l;
    const int fd = luaL_checkint(L, 1);
    const char *data = luaL_checklstring(L, 2, &l);
    int count = util_mkstring(fd, buffer, (char *)data);
    lua_pushinteger(L, count);
    return 1;
}
static int SendMesg(lua_State *L)
{
    const int fd = luaL_checkint(L, 1);
    const int id = luaL_checkint(L, 2);
    util_SendMesg(fd, id, buffer);
    return 0;
}
static int sendNewStreet(lua_State *L)
{
    size_t l;
    const int fd = luaL_checkint(L, 1);
    const char* data = luaL_checklstring(L, 2, &l);
    lssproto_NewStreet_send(fd, data);
    return 1;
}
static int sendGetGold(lua_State *L)
{
    const int fd = luaL_checkint(L, 1);
    const int gold = luaL_checkint(L, 2);
    const int vippoint = luaL_checkint(L, 3);
    const int fame = luaL_checkint(L, 4);
    const int vigor = luaL_checkint(L, 5);
    const int petpoint = luaL_checkint(L, 6);
    lssproto_GetGold_send(fd, gold, vippoint, fame, vigor, petpoint);
    return 1;
}
#ifdef _NEW_MAPBATTLEINFO
static int sendNewMapBattleInfo(lua_State *L)
{
    size_t l;
    const int fd = luaL_checkint(L, 1);
    const int type = luaL_checkint(L, 2);
    const char* battleinfo = luaL_checklstring(L, 3, &l);
    lssproto_NewMapBattleInfo_send(fd, type, battleinfo);
    return 1;
}
#endif
static int sendAnnounce(lua_State *L)
{
    size_t l;
    const int fd = luaL_checkint(L, 1);
    const char* announcebuff = luaL_checklstring(L, 2, &l);
    const int color = luaL_checkint(L, 3);
    const int announcenum = luaL_checkint(L, 4);
    lssproto_Announce_send(fd, announcebuff, color, announcenum);
    return 1;
}
static int sendBuyVigor(lua_State *L)
{
    size_t l;
    const int fd = luaL_checkint(L, 1);
    const int buytype = luaL_checkint(L, 2);
    const char* announcebuff = luaL_checklstring(L, 3, &l);
    lssproto_BuyVigor_send(fd, buytype, announcebuff);
    return 1;
}
static int sendNewPrompt(lua_State *L)
{
    size_t l;
    const int fd = luaL_checkint(L, 1);
    const char* buffer = luaL_checklstring(L, 2, &l);
    int color = getCharBaseValue(L, 3, CharBaseColor, arraysizeof(CharBaseColor));

    if (color == CHAR_COLORNUM)
    {
        color = RAND(0, CHAR_COLORNUM - 2);
    }

    const int timer = luaL_checkint(L, 4);
    lssproto_NewPrompt_send(fd, buffer, color, timer);
    return 1;
}
static int S(lua_State *L)
{
    size_t l;
    const int index = luaL_checkint(L, 1);

    if (CHAR_CHECKINDEX(index) == FALSE)
    {
        return 0;
    }

    int fd = getfdFromCharaIndex(index);
    char *data = luaL_checklstring(L, 2, &l);
    lssproto_S_send(fd, data);
    return 1;
}

#ifdef _NEWPLAYER_FUNC
static int NewPlayerSend(lua_State *L)
{
	size_t l;
	const int fd = luaL_checkint(L, 1);
	const int flg = luaL_checkint(L, 2);
	char *data=luaL_checklstring(L, 3, &l);
	lssproto_NewPlayer_send(fd,flg,data);
	return 1;
}
#endif

#ifdef _UNITY_CLIENT_UPDATE
static int UCLientUpSend(lua_State *L)
{
	size_t l;
	const int fd = luaL_checkint(L, 1);
	const int windowid = luaL_checkint(L, 2);
	lssproto_UClientUpdate_send(fd,windowid);
	return 1;
}
#endif
#ifdef _NEW_515SA

static int OfflineSend(lua_State *L){
		size_t l;
		const int fd = luaL_checkint(L, 1);
		char *data=luaL_checklstring(L, 2, &l);
		lssproto_Offline_send(fd, data);
		return 1;
}
#endif

static const luaL_Reg lssprotolib[] =
{
    {"windows",             windows_send},
    { "windowsupdate", windows_update },
    {"show",                    show},
    {"FM",                      FM},
#ifdef _ADD_STATUS_2
    {"S2",                      S2},
#endif
    {"dengon",              dengon},
    {"CharList",            CharList},
    {"ClientLogin",     ClientLogin},
    {"SaMenu",              SaMenu},
    { "NewSaMenu", NewSaMenu },
    {"W2",                      W2},
    {"B",                       B},
    {"MI",                      MI},
    /*{"windows_validation",        windows_validation},*/
    /*{"MacSend",           MacSend},*/
#ifdef _FAMILYBADGE_
    {"FamilyBadge",         FamilyBadge},
    {"GetFBData",           GetFBData},
#endif
#ifdef _WINLUATO_
    {"WinLuaToSend",        WinLuaToSend},
#endif
#ifdef _LONIN_ERROR_
    {"LoninError", LoninError},
#endif
#ifdef _IPHONE_SHOP_FLAG_
    {"ShopFlag",            ShopFlag},
#endif
#ifdef _SPEED_CHECK_
    { "sendSpeedCheck", sendSpeedCheck },
#endif
#ifdef _ADDJIETU_
    { "sendJianDu", sendJianDu },
#endif
#ifdef _OFFLINERETURN_LOGINCLI
    { "sendOfflineReturn", sendOfflineReturn },
#endif
    { "TK", TK },
    { "sendNewMessage", sendNewMessage },
    { "sendMail", sendMail },
    { "deint", deint },
    { "destring", destring },
    { "mkbuffer", mkbuffer },
    { "mkint", mkint },
    { "mkstring", mkstring },
    { "SendMesg", SendMesg },
    { "sendNewStreet", sendNewStreet },
    { "sendGetGold", sendGetGold },
#ifdef _NEW_MAPBATTLEINFO
    { "sendNewMapBattleInfo", sendNewMapBattleInfo },
#endif
    { "sendAnnounce", sendAnnounce },
    { "sendBuyVigor", sendBuyVigor },
    { "sendNewPrompt", sendNewPrompt },
    { "S", S },
#ifdef _NEWPLAYER_FUNC
	{"NewPlayerSend", 		NewPlayerSend},
#endif
#ifdef _UNITY_CLIENT_UPDATE
	{"UCLientUpSend", 		UCLientUpSend},
#endif

#ifdef _NEWSAMENU_WITH_CALLBACK
    { "NewSaMenu2", NewSaMenu2 },
#endif
#ifdef _NEW_515SA
	{"OfflineSend", 		OfflineSend},
#endif
    {NULL,                        NULL}
};

LUALIB_API int luaopen_Lssproto(lua_State *L)
{
    luaL_register(L, "lssproto", lssprotolib);
    return 1;
}

#endif

