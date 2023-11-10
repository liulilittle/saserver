#include <string.h>
#include <stdio.h>
#include "char.h"
#include "readmap.h"
#include "map_deal.h"
#include "char_base.h"
#include "enemy.h"
#include "mylua/base.h"
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
#include "lssproto_serv.h"

#ifdef _ALLBLUES_LUA
#ifdef _ALLBLUES_LUA_1_4

static CharBase EnemyBaseInt[] =
{
    {{"�̱��"},             ENEMY_ID}
    , {{"�����"},            ENEMY_TEMPNO}
    , {{"��С�ȼ�"},         ENEMY_LV_MIN}
    , {{"���ȼ�"},         ENEMY_LV_MAX}
    , {{"�������"},         ENEMY_CREATEMAXNUM}
    , {{"��С����"},         ENEMY_CREATEMINNUM}
};

static CharBase EnemyBaseChar[] =
{
    {{"����"},                ENEMY_NAME}
};

static CharBase EnemytempBaseInt[] =
{
    {{"���"},                E_T_TEMPNO}
    , {{"��ʼֵ"},            E_T_INITNUM}
    , {{"�ɳ���"},            E_T_LVUPPOINT}
    , {{"����"},               E_T_BASEVITAL}
    , {{"����"},               E_T_BASESTR}
    , {{"����"},               E_T_BASETGH}
    , {{"�ٶ�"},               E_T_BASEDEX}
    , {{"����"},               E_T_MODAI}
    , {{"��"},                  E_T_GET}
    , {{"��"},                  E_T_EARTHAT}
    , {{"ˮ"},                  E_T_WATERAT}
    , {{"��"},                  E_T_FIREAT}
    , {{"��"},                  E_T_WINDAT}
    , {{"����"},               E_T_POISON}
    , {{"�鿹"},               E_T_PARALYSIS}
    , {{"˯��"},               E_T_SLEEP}
    , {{"ʯ��"},               E_T_STONE}
    , {{"�ƿ�"},               E_T_DRUNK}
    , {{"�쿹"},               E_T_CONFUSION}
    , {{"����1"},          E_T_PETSKILL1}
    , {{"����2"},          E_T_PETSKILL2}
    , {{"����3"},          E_T_PETSKILL3}
    , {{"����4"},          E_T_PETSKILL4}
    , {{"����5"},          E_T_PETSKILL5}
    , {{"����6"},          E_T_PETSKILL6}
    , {{"����7"},          E_T_PETSKILL7}
    , {{"����"},               E_T_RARE}
    , {{"������"},            E_T_CRITICAL}
    , {{"������"},            E_T_COUNTER}
    , {{"������"},            E_T_SLOT}
    , {{"����"},               E_T_IMGNUMBER}
    , {{"�ȼ�����"},     E_T_LIMITLEVEL}
#ifdef _PET_FUSION
    , {{"�ںϳ����"},      E_T_FUSIONCODE}
#endif
};

static CharBase EnemytempBaseChar[] =
{
    {{"����"},                E_T_NAME}
};

static int getInt(lua_State *L)
{
    const int array = luaL_checkint(L, 1);
    const int element = getCharBaseValue(L, 2, EnemytempBaseInt, arraysizeof(EnemytempBaseInt));
    lua_pushinteger(L, ENEMYTEMP_getInt(array, element));
    return 1;
}

static int getChar(lua_State *L)
{
    const int array = luaL_checkint(L, 1);
    const int element = getCharBaseValue(L, 2, EnemytempBaseChar, arraysizeof(EnemytempBaseChar));
    lua_pushstring(L, ENEMYTEMP_getChar(array, element));
    return 1;
}

static int enemygetInt(lua_State *L)
{
    const int array = luaL_checkint(L, 1);
    const int element = getCharBaseValue(L, 2, EnemyBaseInt, arraysizeof(EnemyBaseInt));
    lua_pushinteger(L, ENEMY_getInt(array, element));
    return 1;
}

static int enemygetChar(lua_State *L)
{
    const int array = luaL_checkint(L, 1);
    const int element = getCharBaseValue(L, 2, EnemyBaseChar, arraysizeof(EnemyBaseChar));
    lua_pushstring(L, ENEMY_getChar(array, element));
    return 1;
}

static int getEnemyFromCharaindex(lua_State *L)
{
    extern int *ENEMY_getEnemyShow(int charaindex, int x, int y);
    const int charaindex = luaL_checkint(L, 1);
    int *enemytable = ENEMY_getEnemyShow(charaindex,
                                         CHAR_getInt(charaindex, CHAR_X),
                                         CHAR_getInt(charaindex, CHAR_Y));
    lua_newtable(L); //�½�һ������ѹ��ջ��
    lua_pushnumber(L, -1); //���к�����һ�У�û�㶮Ϊʲô��Ҫ�������������ִ�У�lua�л�ȡ�����ݾͻ����
    lua_rawseti(L, -2, 0);

    if (enemytable == NULL)
    {
        lua_pushinteger(L, -1);
        lua_rawseti(L, -2, 1);
    }
    else
    {
        int n;

        for (n = 0; * (enemytable + n) != -1; n++)
        {
            lua_pushinteger(L, enemytable[n]); //�������������ջ
            lua_rawseti(L, -2, n + 1); //���ո���ջ����������Ϊ����ĵ�n+1�����ݣ�ͬʱ������ݻ��Զ���ջ��pop
        }
    }

    return 1;
}

static int getEnemyCustemFromCharaindex(lua_State *L)
{
    extern int *ENEMY_getEnemyShowId(int charaindex, int x, int y);
    const int charaindex = luaL_checkint(L, 1);
    int *enemytable = ENEMY_getEnemyShowId(charaindex,
                                           CHAR_getInt(charaindex, CHAR_X),
                                           CHAR_getInt(charaindex, CHAR_Y));
    lua_newtable(L); //�½�һ������ѹ��ջ��
    lua_pushnumber(L, -1); //���к�����һ�У�û�㶮Ϊʲô��Ҫ�������������ִ�У�lua�л�ȡ�����ݾͻ����
    lua_rawseti(L, -2, 0);

    if (enemytable == NULL)
    {
        lua_pushinteger(L, -1);
        lua_rawseti(L, -2, 1);
    }
    else
    {
        int n;

        for (n = 0; * (enemytable + n) != -1; n++)
        {
            lua_pushinteger(L, enemytable[n]); //�������������ջ
            lua_rawseti(L, -2, n + 1); //���ո���ջ����������Ϊ����ĵ�n+1�����ݣ�ͬʱ������ݻ��Զ���ջ��pop
        }
    }

    return 1;
}

static int getEnemyTempArray(lua_State *L)
{
    const int enemyid = luaL_checkint(L, 1);
    lua_pushinteger(L, ENEMYTEMP_getEnemyTempArray(enemyid));
    return 1;
}


static int getEnemyTempArrayFromTempNo(lua_State *L)
{
    const int array = luaL_checkint(L, 1);
    lua_pushinteger(L, ENEMYTEMP_getEnemyTempArrayFromTempNo(array));
    return 1;
}

static int getEnemyTempArrayFromInitnum(lua_State *L)
{
    const int array = luaL_checkint(L, 1);
    lua_pushinteger(L, ENEMYTEMP_getEnemyTempArrayFromInitnum(array));
    return 1;
}

static int getEnemyTempNameFromEnemyID(lua_State *L)
{
    const int enemyid = luaL_checkint(L, 1);
    int i;

    for (i = 0; i < ENEMY_getEnemyNum(); i ++)
    {
        if (ENEMY_getInt(i, ENEMY_ID) == enemyid)
        {
            break;
        }
    }

    if (i == ENEMY_getEnemyNum())
    {
        lua_pushstring(L, "");
        return 1;
    }

    int tarray = ENEMYTEMP_getEnemyTempArray(i);

    if (!ENEMYTEMP_CHECKINDEX(tarray))
    {
        lua_pushstring(L, "");
    }
    else
    {
        lua_pushstring(L, ENEMYTEMP_getChar(tarray, E_T_NAME));
    }

    return 1;
}


static int getEnemyTempIDFromEnemyID(lua_State *L)
{
    const int enemyid = luaL_checkint(L, 1);
    int i;

    for (i = 0; i < ENEMY_getEnemyNum(); i ++)
    {
        if (ENEMY_getInt(i, ENEMY_ID) == enemyid)
        {
            break;
        }
    }

    if (i == ENEMY_getEnemyNum())
    {
        lua_pushinteger(L, -1);
        return 1;
    }

    lua_pushinteger(L, ENEMY_getInt(i, ENEMY_TEMPNO));
    return 1;
}

static int getEnemyTempNum(lua_State *L)
{
    lua_pushinteger(L, ENEMYTEMP_getEnemyNum());
    return 1;
}

static int createEnemy(lua_State *L)
{
    const int array = luaL_checkint(L, 1);
    const int level = luaL_checkint(L, 2);
    int newindex = ENEMY_createEnemy(array, level);
    lua_pushinteger(L, newindex);
    return 1;
}

static int getEnemyArrayFromId(lua_State *L)
{
    const int enemyid = luaL_checkint(L, 1);
    lua_pushinteger(L, ENEMY_getEnemyArrayFromId(enemyid));
    return 1;
}


static const luaL_Reg enemytemplib[] =
{
    {"getInt",                                              getInt},
    {"getChar",                                             getChar},
    {"enemygetInt",                                     enemygetInt},
    {"enemygetChar",                                    enemygetChar},
    {"getEnemyFromChar",                            getEnemyFromCharaindex},
    {"getEnemyCustemFromChar",                            getEnemyCustemFromCharaindex},
    {"getEnemyTempArray",                       getEnemyTempArray},
    {"getEnemyTempArrayFromTempNo",     getEnemyTempArrayFromTempNo},
    {"getEnemyTempArrayFromInitnum",    getEnemyTempArrayFromInitnum},
    {"getEnemyTempNameFromEnemyID",     getEnemyTempNameFromEnemyID},
    {"getEnemyTempIDFromEnemyID",       getEnemyTempIDFromEnemyID},
    { "getEnemyTempNum", getEnemyTempNum },
    { "createEnemy", createEnemy },
    { "getEnemyArrayFromId", getEnemyArrayFromId },
    {NULL,                                    NULL}
};

LUALIB_API int luaopen_Enemytemp(lua_State *L)
{
    luaL_register(L, "enemytemp", enemytemplib);
    return 1;
}
#endif

#endif

