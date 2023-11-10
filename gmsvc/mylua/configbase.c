#include <string.h>
#include "char.h"
#include "char_base.h"
#include "mylua/base.h"
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
#include "configfile.h"
extern char _sa_version_type;
#ifdef _ALLBLUES_LUA

#ifdef _ALLBLUES_LUA_1_1
static int set(lua_State *L)
{
    size_t l;
    char *data = luaL_checklstring(L, 1, &l);
    luareadconfigfile(data);
    return 1;
}

static int checkIp(lua_State *L)
{
    size_t l;
    char *ip = luaL_checklstring(L, 1, &l);
    lua_pushinteger(L, checkServerIpChar(ip));
    return 1;
}

static int getGameID(lua_State *L)
{
    lua_pushstring(L, getGameserverID());
    return 1;
}

static int getGamename(lua_State *L)
{
    lua_pushstring(L, getGameservername());
    return 1;
}

static int getnumber(lua_State *L)
{
    lua_pushinteger(L, getServernumber());
    return 1;
}
#ifdef _GET_BATTLE_EXP
static int getexp(lua_State *L)
{
    lua_pushinteger(L, getBattleexp());
    return 1;
}
#endif

#ifdef _VIP_BATTLE_EXP
int getVipexp(lua_State *L)
{
    lua_pushinteger(L, getVipBattleexp());
    return 1;
}
#endif

int getFdMaxNnum(lua_State *L)
{
    lua_pushinteger(L, getFdnum());
    return 1;
}

int PlayerNum(lua_State *L)
{
    int num = luaL_checkint(L, 1);
    setPlayerNum(num);
    return 1;
}
int getBattleNum(lua_State *L)
{
    lua_pushinteger(L, getBattlenum());
    return 1;
}
int getBattleexptime(lua_State *L)
{
    lua_pushinteger(L, getBattleExpTime());
    return 1;
}
static int setsaversion(lua_State *L)
{
    size_t l;
    char *ip = luaL_checklstring(L, 1, &l);
    _sa_version_type = ip[0];
    return 1;
}
static const luaL_Reg configlib[] =
{
    {"set",                                 set},
    {"checkIp",                         checkIp},
    {"getGameserverID",         getGameID},
    {"getGameservername",   getGamename},
    {"getServernumber",         getnumber},
#ifdef _GET_BATTLE_EXP
    {"getBattleexp",                getexp},
#endif
#ifdef _VIP_BATTLE_EXP
    {"getVipBattleexp",         getVipexp},
#endif
    {"getFdnum",                        getFdMaxNnum},
    {"PlayerNum",                       PlayerNum},
    { "getBattleNum", getBattleNum },
    { "getBattleexptime", getBattleexptime },
    { "setsaversion", setsaversion },
    {NULL,                NULL}
};

LUALIB_API int luaopen_Config(lua_State *L)
{
    luaL_register(L, "config", configlib);
    return 1;
}
#endif

#endif

