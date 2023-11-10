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
    {{"短编号"},             ENEMY_ID}
    , {{"长编号"},            ENEMY_TEMPNO}
    , {{"最小等级"},         ENEMY_LV_MIN}
    , {{"最大等级"},         ENEMY_LV_MAX}
    , {{"最大数量"},         ENEMY_CREATEMAXNUM}
    , {{"最小数量"},         ENEMY_CREATEMINNUM}
};

static CharBase EnemyBaseChar[] =
{
    {{"名字"},                ENEMY_NAME}
};

static CharBase EnemytempBaseInt[] =
{
    {{"编号"},                E_T_TEMPNO}
    , {{"初始值"},            E_T_INITNUM}
    , {{"成长率"},            E_T_LVUPPOINT}
    , {{"体力"},               E_T_BASEVITAL}
    , {{"腕力"},               E_T_BASESTR}
    , {{"耐力"},               E_T_BASETGH}
    , {{"速度"},               E_T_BASEDEX}
    , {{"智能"},               E_T_MODAI}
    , {{"给"},                  E_T_GET}
    , {{"地"},                  E_T_EARTHAT}
    , {{"水"},                  E_T_WATERAT}
    , {{"火"},                  E_T_FIREAT}
    , {{"风"},                  E_T_WINDAT}
    , {{"毒抗"},               E_T_POISON}
    , {{"麻抗"},               E_T_PARALYSIS}
    , {{"睡抗"},               E_T_SLEEP}
    , {{"石抗"},               E_T_STONE}
    , {{"酒抗"},               E_T_DRUNK}
    , {{"混抗"},               E_T_CONFUSION}
    , {{"技能1"},          E_T_PETSKILL1}
    , {{"技能2"},          E_T_PETSKILL2}
    , {{"技能3"},          E_T_PETSKILL3}
    , {{"技能4"},          E_T_PETSKILL4}
    , {{"技能5"},          E_T_PETSKILL5}
    , {{"技能6"},          E_T_PETSKILL6}
    , {{"技能7"},          E_T_PETSKILL7}
    , {{"罕见"},               E_T_RARE}
    , {{"暴击率"},            E_T_CRITICAL}
    , {{"反击率"},            E_T_COUNTER}
    , {{"技能数"},            E_T_SLOT}
    , {{"形象"},               E_T_IMGNUMBER}
    , {{"等级限制"},     E_T_LIMITLEVEL}
#ifdef _PET_FUSION
    , {{"融合宠编码"},      E_T_FUSIONCODE}
#endif
};

static CharBase EnemytempBaseChar[] =
{
    {{"名字"},                E_T_NAME}
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
    lua_newtable(L); //新建一个表，并压入栈顶
    lua_pushnumber(L, -1); //这行和下面一行，没搞懂为什么需要这样，但如果不执行，lua中获取的数据就会出错
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
            lua_pushinteger(L, enemytable[n]); //将数组的数据入栈
            lua_rawseti(L, -2, n + 1); //将刚刚入栈的数据设置为数组的第n+1个数据，同时这个数据会自动从栈里pop
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
    lua_newtable(L); //新建一个表，并压入栈顶
    lua_pushnumber(L, -1); //这行和下面一行，没搞懂为什么需要这样，但如果不执行，lua中获取的数据就会出错
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
            lua_pushinteger(L, enemytable[n]); //将数组的数据入栈
            lua_rawseti(L, -2, n + 1); //将刚刚入栈的数据设置为数组的第n+1个数据，同时这个数据会自动从栈里pop
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

