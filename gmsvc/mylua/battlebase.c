#include <string.h>
#include "longzoro/version.h"
#include "char_base.h"
#include "enemy.h"
#include "object.h"
#include "char.h"
#include "lssproto_serv.h"
#include "battle.h"
#include "battle_command.h"
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
#include "mylua/mylua.h"
#include "mylua/base.h"
#ifdef _ALLBLUES_LUA

static int CreateVsEnemy(lua_State *L)
{
    const int charaindex = luaL_checkint(L, 1);
    const int npcindex = luaL_checkint(L, 2);
    int enemytable[11];
    int i;

    for (i = 0; i < 11; i ++)
    {
        enemytable[i] = -1;
    }

    luaL_checktype(L, 3, LUA_TTABLE);
    int n = luaL_getn(L, 3);
    int id = 0;

    for (i = 0; i < n; i++)
    {
        int enemyid = getArrayInt(L, i);
        int curEnemy = ENEMY_getEnemyArrayFromId(enemyid);

        if (ENEMY_CHECKINDEX(curEnemy))
        {
            enemytable[id++] = curEnemy;
        }
        else
        {
            enemytable[i] = -1;
        }
    }

    int ret = BATTLE_CreateVsEnemyNew(charaindex, npcindex, enemytable);

    if (ret == 0)
    {
        if (npcindex > -1)
        {
            CHAR_setWorkInt(npcindex, CHAR_WORKBATTLEMODE, BATTLE_CHARMODE_INIT);
            CHAR_sendBattleEffect(npcindex, ON);
        }

        lua_pushinteger(L, CHAR_getWorkInt(charaindex, CHAR_WORKBATTLEINDEX));
        return 1;
    }
    else
    {
        lua_pushinteger(L, -1);
        return 1;
    }
}

static int CreateVsEnemyLv(lua_State *L)
{
    size_t l;
    const int charaindex = luaL_checkint(L, 1);
    const int npcindex = luaL_checkint(L, 2);
    char *enemytablebuf = luaL_checklstring(L, 3, &l);
    char *enemylvtablebuf = luaL_checklstring(L, 4, &l);
    char tmpenemytablebuf[10];
    char tmpenemylvtablebuf[10];
    int j = 1;
    int k = 1;
    int enemytable[11];
    int enemylvtable[11];
    int i;

    for (i = 0; i < 11; i ++)
    {
        enemytable[i] = -1;
        enemylvtable[i] = -1;
    }

    while (getStringFromIndexWithDelim(enemytablebuf, ",", j, tmpenemytablebuf, sizeof(tmpenemytablebuf)) != FALSE)
    {
        int enemyid = atoi(tmpenemytablebuf);
        int curEnemy = ENEMY_getEnemyArrayFromId(enemyid);

        if (ENEMY_CHECKINDEX(curEnemy))
        {
            enemytable[j - 1] = curEnemy;
        }
        else
        {
            enemytable[j - 1] = -1;
        }

        j++;
    }

    while (getStringFromIndexWithDelim(enemylvtablebuf, ",", k, tmpenemylvtablebuf, sizeof(tmpenemylvtablebuf)) != FALSE)
    {
        enemylvtable[k - 1] = atoi(tmpenemylvtablebuf);
        k++;
    }

    if (j != k)
    {
        lua_pushinteger(L, -1);
        return 0;
    }

    int ret = BATTLE_CreateVsEnemyLvNew(charaindex, npcindex, enemytable, enemylvtable);

    if (ret == 0)
    {
        if (npcindex > -1)
        {
            CHAR_setWorkInt(npcindex, CHAR_WORKBATTLEMODE, BATTLE_CHARMODE_INIT);
            CHAR_sendBattleEffect(npcindex, ON);
        }

        lua_pushinteger(L, CHAR_getWorkInt(charaindex, CHAR_WORKBATTLEINDEX));
        return 1;
    }
    else
    {
        lua_pushinteger(L, -1);
        return 1;
    }
}

static int getCharOne(lua_State *L)
{
    const int battle = luaL_checkint(L, 1);
    const int num = luaL_checkint(L, 2);
    const int side = luaL_checkint(L, 3);
    lua_pushinteger(L, BattleArray[battle].Side[side].Entry[num].charaindex);
    return 1;
}

static int Exit(lua_State *L)
{
    const int charaindex = luaL_checkint(L, 1);
    const int battleindex = luaL_checkint(L, 2);
    int defNo = BATTLE_Index2No(battleindex, charaindex);
    char szwork[128];
    //sprintf(szwork,"BE|e%X|",defNo);
    //BATTLESTR_ADD( szwork );
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLEMODE, BATTLE_CHARMODE_NONE);
    BATTLE_Exit(charaindex, battleindex);
    CHAR_DischargePartyNoMsg(charaindex);
    BATTLE_CommandSend(charaindex, "BU");
    //sprintf(szwork,"BQ|e%X|",charaindex);//���ܶ���
    //BATTLESTR_ADD(szwork);
    return 1;
}

#ifdef _ALLBLUES_LUA_1_4
static CharBase BattleBaseEvent[] =
{
    {{"�����¼�"},      BATTLE_FINISH}
    , {{"�����¼�"},     BATTLE_ESCAPE}
};

static int CreateRandVsPlayer(lua_State *L)
{
    const int floorid = luaL_checkint(L, 1);
    int charaindex1 = -1, charaindex2 = -1;
    int num = 0;
    int i = 0;
    int playernum = CHAR_getPlayerMaxNum();

    for (i = 0; i < playernum; i++)
    {
        if (CHAR_getCharUse(i) == FALSE)
        {
            continue;
        }

        if (!CHAR_CHECKINDEX(i))
        {
            continue;
        }

        if (CHAR_getInt(i, CHAR_FLOOR) == floorid)
        {
            if (CHAR_getWorkInt(i, CHAR_WORKBATTLEMODE) == BATTLE_CHARMODE_NONE)
            {
                if (charaindex1 == -1)
                {
                    charaindex1 = i;
                }
                else
                {
                    charaindex2 = i;
                }

                if (CHAR_CHECKINDEX(charaindex1) && CHAR_CHECKINDEX(charaindex2))
                {
                    BATTLE_CreateVsPlayer(charaindex1, charaindex2);
                    num ++;
                }
            }
        }
    }

    lua_pushinteger(L, num);
    return 1;
}

static int CreateVsPlayer(lua_State *L)
{
    const int charaindex1 = luaL_checkint(L, 1);
    const int charaindex2 = luaL_checkint(L, 2);

    if (CHAR_CHECKINDEX(charaindex1) && CHAR_CHECKINDEX(charaindex2))
    {
        if (CHAR_getWorkInt(charaindex1, CHAR_WORKBATTLEMODE) == BATTLE_CHARMODE_NONE
                && CHAR_getWorkInt(charaindex2, CHAR_WORKBATTLEMODE) == BATTLE_CHARMODE_NONE)
        {
            if (BATTLE_CreateVsPlayer(charaindex1, charaindex2) == 0)
            {
                lua_pushinteger(L, CHAR_getWorkInt(charaindex1, CHAR_WORKBATTLEINDEX));
                return 1;
            }
        }
    }

    lua_pushinteger(L, -1);
    return 1;
}

static int setLUAFunctionPointer(lua_State *L)
{
    size_t l;
    const int battleindex = luaL_checkint(L, 1);
    const int functype = getCharBaseValue(L, 2, BattleBaseEvent, arraysizeof(BattleBaseEvent));
    char *luafunctable = luaL_checklstring(L, 3, &l);
    BATTLE_setLUAFunction(battleindex, functype, L, luafunctable);
    return 1;
}

static int WatchEntry(lua_State *L)
{
    const int meindex = luaL_checkint(L, 1);
    const int toindex = luaL_checkint(L, 2);
    BATTLE_WatchEntry(meindex, toindex);
    return 1;
}

static int NewEntry(lua_State *L)
{
    const int charaindex = luaL_checkint(L, 1);
    const int battleindex = luaL_checkint(L, 2);
    const int side = luaL_checkint(L, 3);
    int fd = getfdFromCharaIndex(charaindex);
    int iRet = BATTLE_NewEntry(charaindex, battleindex, side);

    if (iRet == 0)
    {
        CHAR_setWorkInt(charaindex, CHAR_WORKBATTLEMODE, BATTLE_CHARMODE_RESCUE);
        iRet = BATTLE_PetDefaultEntry(
                   charaindex,
                   battleindex,
                   side
               );
    }

    if (iRet == 0)
    {
        int     flg;

        if (CHAR_getInt(charaindex, CHAR_DEFAULTPET) >= 0)
        {
            int pindex = CHAR_getCharPet(charaindex, CHAR_getInt(charaindex, CHAR_DEFAULTPET));
            CHAR_setWorkInt(pindex, CHAR_WORKBATTLEMODE, BATTLE_CHARMODE_RESCUE);
        }

        if (fd != -1)
        {
            lssproto_EN_send(fd, BattleArray[battleindex].type, BattleArray[battleindex].field_no);
        }

        flg = (BattleArray[battleindex].Side[side].flg & BSIDE_FLG_HELP_OK) ? TRUE : FALSE;
        lssproto_HL_send(fd, flg);
        char szBuffer[256];
        sprintf(szBuffer, "BP|%X|%X|%X",
                BATTLE_Index2No(battleindex, charaindex), BP_FLG_JOIN, CHAR_getInt(charaindex, CHAR_MP));
        BATTLE_CommandSend(charaindex, szBuffer);
        lua_pushinteger(L, 1);
    }
    else
    {
        if (fd != -1)
        {
            lssproto_EN_send(fd, FALSE, BattleArray[battleindex].field_no);
        }

        lua_pushinteger(L, 0);
    }

    return 1;
}
#endif

#ifdef _ALLBLUES_LUA_1_9
static int CreateForWatcher(lua_State *L)
{
    const int meindex = luaL_checkint(L, 1);
    const int batleindex = luaL_checkint(L, 2);
    BATTLE_CreateForWatcher(meindex, batleindex);
    return 1;
}

static int checkindex(lua_State *L)
{
    const int batleindex = luaL_checkint(L, 1);
    lua_pushinteger(L, BATTLE_CHECKINDEX(batleindex));
    return 1;
}

static int getType(lua_State *L)
{
    const int batleindex = luaL_checkint(L, 1);
    lua_pushinteger(L, BATTLE_getType(batleindex));
    return 1;
}

#ifdef _BATTLE_TIMESPEED
static int getCreateTime(lua_State *L)
{
    const int batleindex = luaL_checkint(L, 1);
    lua_pushinteger(L, BATTLE_getCreateTime(batleindex));
    return 1;
}
#endif

static int getBattleFloor(lua_State *L)
{
    const int battleindex = luaL_checkint(L, 1);
    lua_pushinteger(L, BATTLE_getBattleFloor(battleindex));
    return 1;
}
static int getBattleX(lua_State *L)
{
    const int battleindex = luaL_checkint(L, 1);
    lua_pushinteger(L, BATTLE_getBattleX(battleindex));
    return 1;
}
static int getBattleY(lua_State *L)
{
    const int battleindex = luaL_checkint(L, 1);
    lua_pushinteger(L, BATTLE_getBattleY(battleindex));
    return 1;
}
#endif

static int Ai_One(lua_State *L)
{
    const int charaindex = luaL_checkint(L, 1);
    const int battleindex = luaL_checkint(L, 2);
    const int side = luaL_checkint(L, 3);
    const int turn = luaL_checkint(L, 4);
    BATTLE_ai_one(charaindex, battleindex, side, turn);
    lua_pushinteger(L, 0);
    return 1;
}

static int RescueEntry(lua_State *L)
{
    const int charaindex = luaL_checkint(L, 1);
    const int toindex = luaL_checkint(L, 2);
    BATTLE_RescueEntry(charaindex, toindex);
    return 1;
}


#ifdef  _BATTLE_PAIDUI
static int getBattlePaiType(lua_State *L)
{
    const int battleindex = luaL_checkint(L, 1);
    lua_pushinteger(L, BATTLE_getBattlePaiType(battleindex));
    return 1;
}
static int setBattlePaiType(lua_State *L)
{
    const int battleindex = luaL_checkint(L, 1);
    const int num = luaL_checkint(L, 2);
    lua_pushinteger(L, BATTLE_setBattlePaiType(battleindex, num));
    return 1;
}
static int getBattlePaiIndex(lua_State *L)
{
    const int battleindex = luaL_checkint(L, 1);
    const int num = luaL_checkint(L, 2);
    lua_pushinteger(L, BATTLE_getBattlePaiIndex(battleindex, num));
    return 1;
}
static int setBattlePaiIndex(lua_State *L)
{
    const int battleindex = luaL_checkint(L, 1);
    const int num = luaL_checkint(L, 2);
    const int index = luaL_checkint(L, 3);
    lua_pushinteger(L, BATTLE_setBattlePaiIndex(battleindex, num, index));
    return 1;
}
#endif

static int getTopBattleIndex(lua_State *L)
{
    const int battleindex = luaL_checkint(L, 1);
    lua_pushinteger(L, BATTLE_getTopBattleIndex(battleindex));
    return 1;
}

static int getLeaderIndex(lua_State *L)
{
    const int battleindex = luaL_checkint(L, 1);
    lua_pushinteger(L, BATTLE_getLeaderIndex(battleindex));
    return 1;
}

static int setBattleEntryCharaindex(lua_State *L)
{
    const int battleindex = luaL_checkint(L, 1);
    const int side = luaL_checkint(L, 2);
    const int no = luaL_checkint(L, 3);
    const int index = luaL_checkint(L, 4);
    BATTLE_setBattleEntryCharaindex(battleindex, side, no, index);
    return 1;
}

static int ExitOne(lua_State *L)
{
    const int charaindex = luaL_checkint(L, 1);
    const int battleindex = luaL_checkint(L, 2);
    BATTLE_Exit(charaindex, battleindex);
    return 1;
}
static int getBattleHelpType(lua_State *L)
{
    const int charaindex = luaL_checkint(L, 1);
    lua_pushinteger(L, BATTLE_getHelpType(charaindex));
    return 1;
}
#ifdef _SMART_ENEMY_LUA
extern int BATTLE_FieldAttChangeNew(int charaindex, int type);
static int fieldAttChange(lua_State *L)
{
    const int charaindex = luaL_checkint(L, 1);
    const int type = luaL_checkint(L, 2);
    BATTLE_FieldAttChangeNew(charaindex, type);
    return 1;
}

static int getFieldAtt(lua_State *L)
{
    const int batleindex = luaL_checkint(L, 1);
    lua_pushinteger(L, BATTLE_getFieldAtt(batleindex));
    return 1;
}

static int getAttCount(lua_State *L)
{
    const int batleindex = luaL_checkint(L, 1);
    lua_pushinteger(L, BATTLE_getAttCount(batleindex));
    return 1;
}


static int getAttPow(lua_State *L)
{
    const int batleindex = luaL_checkint(L, 1);
    lua_pushinteger(L, BATTLE_getAttPow(batleindex));
    return 1;
}

extern int ENEMY_createEnemy(int enemyid, int level);
static int createEnemy(lua_State *L)
{
    const int enemyid = luaL_checkint(L, 1);
    const int level = luaL_checkint(L, 2);
    lua_pushinteger(L, ENEMY_createEnemy(enemyid, level));
    return 1;
}

extern int PETSKILL_Use(int charaindex, int havepetskill, int target, char* data);
static int PetskillUse(lua_State *L)
{
    const int charaindex = luaL_checkint(L, 1);
    const int havepetskill = luaL_checkint(L, 2);
    const int target = luaL_checkint(L, 3);
    lua_pushinteger(L, PETSKILL_Use(charaindex, havepetskill, target, NULL));
    return 1;
}

static int isReversed(lua_State *L)
{
    const int charaindex = luaL_checkint(L, 1);
    lua_pushinteger(L, CHAR_getWorkInt(charaindex, CHAR_WORKBATTLEFLG) & CHAR_BATTLEFLG_REVERSE);
    return 1;
}

#endif
static int getTurn(lua_State *L)
{
    const int batleindex = luaL_checkint(L, 1);
    lua_pushinteger(L, BATTLE_getTurn(batleindex));
    return 1;
}
static const luaL_Reg battlelib[] =
{
    {"CreateVsEnemy",                         CreateVsEnemy},
    {"CreateVsEnemyLv",                   CreateVsEnemyLv},
    {"getCharOne",                                getCharOne},
#ifdef _ALLBLUES_LUA_1_4
    {"CreateRandVsPlayer",                CreateRandVsPlayer},
    {"CreateVsPlayer",                        CreateVsPlayer},
    {"setLUAFunctionPointer",         setLUAFunctionPointer},
    {"WatchEntry",                                WatchEntry},
    {"NewEntry",                                    NewEntry},
#endif
#ifdef _ALLBLUES_LUA_1_9
    {"CreateForWatcher",                    CreateForWatcher},
    {"checkindex",                              checkindex},
    {"getType",                                     getType},
#ifdef _BATTLE_TIMESPEED
    {"getCreateTime",                       getCreateTime},
#endif
    {"getBattleFloor",                      getBattleFloor},
    { "getBattleX", getBattleX },
    { "getBattleY", getBattleY },
#endif
    {"Exit",                        Exit},
    {"Ai_One",                      Ai_One},
    {"RescueEntry",                         RescueEntry},
#ifdef  _BATTLE_PAIDUI
    {"getBattlePaiType",                        getBattlePaiType},
    {"setBattlePaiType",                        setBattlePaiType},
    {"getBattlePaiIndex",                       getBattlePaiIndex},
    {"setBattlePaiIndex",                       setBattlePaiIndex},
#endif
    { "getTopBattleIndex", getTopBattleIndex },
    { "getLeaderIndex", getLeaderIndex },
    { "setBattleEntryCharaindex", setBattleEntryCharaindex },
    { "ExitOne", ExitOne },
    { "getBattleHelpType", getBattleHelpType },
#ifdef _SMART_ENEMY_LUA
    {"fieldAttChange",                  fieldAttChange},
    {"getAttCount",                         getAttCount},
    {"getFieldAtt",                         getFieldAtt},
    {"getAttPow",                           getAttPow},
    {"createEnemy",                     createEnemy},
    {"PetskillUse",                     PetskillUse },
    {"isReversed",                          isReversed  },
#endif
    {"getTurn",                          getTurn  },
    {NULL, NULL}
};


LUALIB_API int luaopen_Battle(lua_State *L)
{
    luaL_register(L, "battle", battlelib);
    return 1;
}

#endif
