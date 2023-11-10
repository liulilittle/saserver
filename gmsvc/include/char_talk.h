#ifndef __CHAR_TALK_H__
#define __CHAR_TALK_H__

#include "common.h"
#include "util.h"
#include "net.h"
#ifdef _ALLBLUES_LUA
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
#endif
#ifdef _ITEM_PET_TALK_
char* GetOutPetItemData(int fd_charaindex, char* petitemstr);
#endif
void CHAR_getMessageBody(char* message, char* kind, int kindlen,
                         char** body);

int CHAR_getChatMagicFuncLevel(char* name, BOOL isDebug);
int CHAR_getChatMagicFuncNameAndString(int ti, char* name, char *usestring, int level, BOOL isDebug);
int CHAR_getChatMagicFuncMaxNum(void);

void OneByOneTkChannel(int fd, char *tmp1, char *tmp2,
#ifdef _TELL_ITEM_PET_TALK_
                       char* itempetbuff,
#endif
                       int color) ;

typedef void (*CHATMAGICFUNC)(int, char*);
CHATMAGICFUNC CHAR_getChatMagicFuncPointer(char* name, BOOL isDebug);


#ifdef _RE_GM_COMMAND
int re_gm_command();
#endif

#ifdef  _PETITEM_TALKSHOW
typedef struct
{
    int postype;
    char name[CHARNAMELEN];
    char itembuff[128];
    int posbmp;
    int oldhp;
    int oldatk;
    int olddef;
    int olddex;
    int oldlv;
    int nowhp;
    int nowatk;
    int nowdef;
    int nowdex;
    int level;
    int trans;
    int earth;
    int water;
    int fire;
    int wind;
    int zhong;
    char skillname[7][CHARNAMELEN];
} CHAR_PetItemShowTable;

extern CHAR_PetItemShowTable CHAR_PetItemShow[100];
#endif


#ifdef _FILTER_TALK
int ReadFilterTalk();
char *getFilterTalk(int index);
int getFilterTalkNum();
#endif

#ifdef _ALLBLUES_LUA_1_2
typedef struct tagMAGIC_LuaFunc
{
    lua_State *lua;
    char *luafuncname;
    char *luafunctable;
    int gmlevel;
    char *usestring;
    struct tagMAGIC_LuaFunc   *next;
} MAGIC_LuaFunc;

BOOL MAGIC_addLUAListFunction(lua_State *L, const char *luafuncname, const char *luafunctable, int gmlevel, char *usestring);
BOOL MAGIC_getLUAListFunction(char *luafuncname, int gmlevel, int charaindex, char *data);

#endif

#endif
