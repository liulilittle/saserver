#include <string.h>
#include "lua.h"
#include "longzoro/version.h"
#include "lauxlib.h"
#include "lualib.h"
#include "net.h"
#include "log.h"
#include "longzoro/sasql.h"

#ifdef _ALLBLUES_LUA
#ifdef _ALLBLUES_LUA_1_4
#ifdef _SASQL
#include <mysql/mysql.h>
extern MYSQL mysql;
MYSQL_RES *mysql_result;
MYSQL_ROW mysql_row;
MYSQL_FIELD *fields;
int numrow = 0;
int numfields = 0;
static int setVipPoint(lua_State *L)
{
    const int charaindex = luaL_checkint(L, 1);
    const int point = luaL_checkint(L, 2);
    sasql_setVipPoint(charaindex, point);
    return 1;
}

static int getVipPoint(lua_State *L)
{
    const int charaindex = luaL_checkint(L, 1);
    lua_pushinteger(L, sasql_getVipPoint(charaindex));
    return 1;
}

static int setVipPointForCdkey(lua_State *L)
{
    size_t l;
    char *id = luaL_checklstring(L, 1, &l);
    const int point = luaL_checkint(L, 2);
    sasql_setVipPointForCdkey(id, point);
    return 1;
}

static int getVipPointForCdkey(lua_State *L)
{
    size_t l;
    char *id = luaL_checklstring(L, 1, &l);
    lua_pushinteger(L, sasql_getVipPointForCdkey(id));
    return 1;
}

static int setPayPoint(lua_State *L)
{
    const int charaindex = luaL_checkint(L, 1);
    const int point = luaL_checkint(L, 2);
    sasql_setPayPoint(charaindex, point);
    return 1;
}

static int getPayPoint(lua_State *L)
{
    const int charaindex = luaL_checkint(L, 1);
    lua_pushinteger(L, sasql_getPayPoint(charaindex));
    return 1;
}

static int setPayPointForCdkey(lua_State *L)
{
    size_t l;
    char *id = luaL_checklstring(L, 1, &l);
    const int point = luaL_checkint(L, 2);
    sasql_setPayPointForCdkey(id, point);
    return 1;
}

static int getPayPointForCdkey(lua_State *L)
{
    size_t l;
    char *id = luaL_checklstring(L, 1, &l);
    lua_pushinteger(L, sasql_getPayPointForCdkey(id));
    return 1;
}


static int setRmbPoint(lua_State *L)
{
    const int charaindex = luaL_checkint(L, 1);
    const int point = luaL_checkint(L, 2);
    sasql_setRmbPoint(charaindex, point);
    return 1;
}

static int getRmbPoint(lua_State *L)
{
    const int charaindex = luaL_checkint(L, 1);
    lua_pushinteger(L, sasql_getRmbPoint(charaindex));
    return 1;
}

static int setRmbPointForCdkey(lua_State *L)
{
    size_t l;
    char *id = luaL_checklstring(L, 1, &l);
    const int point = luaL_checkint(L, 2);
    sasql_setRmbPointForCdkey(id, point);
    return 1;
}

static int getRmbPointForCdkey(lua_State *L)
{
    size_t l;
    char *id = luaL_checklstring(L, 1, &l);
    lua_pushinteger(L, sasql_getRmbPointForCdkey(id));
    return 1;
}

static int setPetPoint(lua_State *L)
{
    const int charaindex = luaL_checkint(L, 1);
    const int point = luaL_checkint(L, 2);
    sasql_setPetPoint(charaindex, point);
    return 1;
}

static int getPetPoint(lua_State *L)
{
    const int charaindex = luaL_checkint(L, 1);
    lua_pushinteger(L, sasql_getPetPoint(charaindex));
    return 1;
}

static int setPetPointForCdkey(lua_State *L)
{
    size_t l;
    char *id = luaL_checklstring(L, 1, &l);
    const int point = luaL_checkint(L, 2);
    sasql_setPetPointForCdkey(id, point);
    return 1;
}

static int getPetPointForCdkey(lua_State *L)
{
    size_t l;
    char *id = luaL_checklstring(L, 1, &l);
    lua_pushinteger(L, sasql_getPetPointForCdkey(id));
    return 1;
}


static int query(lua_State *L)
{
    size_t l;
    char *data = luaL_checklstring(L, 1, &l);

    if (!sasql_mysql_query(data))
    {
        lua_pushinteger(L, 1);
    }
    else
    {
        lua_pushinteger(L, 0);
    }

    return 1;
}

static int free_result(lua_State *L)
{
    mysql_free_result(mysql_result);
    return 1;
}


static int store_result(lua_State *L)
{
    mysql_result = mysql_store_result(&mysql);
    return 1;
}

static int num_rows(lua_State *L)
{
    numrow = mysql_num_rows(mysql_result);
    lua_pushinteger(L, numrow);
    return 1;
}

static int num_fields(lua_State *L)
{
    lua_pushinteger(L, numfields);
    return 1;
}

static int fetch_row(lua_State *L)
{
    mysql_row = mysql_fetch_row(mysql_result);
    return 1;
}

static int data(lua_State *L)
{
    int id = luaL_checkint(L, 1) - 1;

    if (mysql_row[id] == NULL)
    {
        lua_pushstring(L, "");
        return 1;
    }

    lua_pushstring(L, mysql_row[id]);
    return 1;
}

#ifdef _GLORY_POINT
static int setGloryPoint(lua_State *L)
{
    const int charaindex = luaL_checkint(L, 1);
    const int point = luaL_checkint(L, 2);
    sasql_setGloryPoint(charaindex, point);
#ifdef _MO_LUA_GOLRYCALLBACK
    GloryCallBack(charaindex, point);
#endif
    return 1;
}

static int getGloryPoint(lua_State *L)
{
    const int charaindex = luaL_checkint(L, 1);
    lua_pushinteger(L, sasql_getGloryPoint(charaindex));
    return 1;
}
#endif

extern int sasql_setCrystalPoint(int charaindex, int point);

static int setCrystalPoint(lua_State *L)
{
    const int charaindex = luaL_checkint(L, 1);
    const int point = luaL_checkint(L, 2);
    sasql_setCrystalPoint(charaindex, point);
    return 1;
}
extern int sasql_getCrystalPoint(int charaindex);

static int getCrystalPoint(lua_State *L)
{
    const int charaindex = luaL_checkint(L, 1);
    lua_pushinteger(L, sasql_getCrystalPoint(charaindex));
    return 1;
}

static int updateFmPointData(lua_State *L)
{
    const int fmid = luaL_checkint(L, 1);
    const int fmtime = luaL_checkint(L, 2);
    const int fmnum = luaL_checkint(L, 3);
    const int flg = luaL_checkint(L, 4);
    sasql_updateFmPointData(fmid, fmtime, fmnum, flg);
    return 1;
}

static int queryFmPointData(lua_State *L)
{
    const int fmid = luaL_checkint(L, 1);
    const int flg = luaL_checkint(L, 2);
    lua_pushinteger(L, sasql_queryFmPointData(fmid, flg));
    return 1;
}

static int setCostPoint(lua_State *L)
{
    const int charaindex = luaL_checkint(L, 1);
    const int point = luaL_checkint(L, 2);
    sasql_setCostPoint(charaindex, point);
    return 1;
}

static int getCostPoint(lua_State *L)
{
    const int charaindex = luaL_checkint(L, 1);
    lua_pushinteger(L, sasql_getCostPoint(charaindex));
    return 1;
}

static const luaL_Reg sasqllib[] =
{
    {"setVipPoint",                       setVipPoint},
    {"getVipPoint",                       getVipPoint},
    {"query",                                   query},
    {"free_result",                         free_result},
    {"store_result",                        store_result},
    {"num_rows",                                num_rows},
    {"fetch_row",                           fetch_row},
    {"num_fields",                          num_fields},
    {"data",                                        data},
    {"setVipPointForCdkey",         setVipPointForCdkey},
    {"getVipPointForCdkey",       getVipPointForCdkey},
    {"setPayPoint",                       setPayPoint},
    {"getPayPoint",                       getPayPoint},
    {"setPayPointForCdkey",         setPayPointForCdkey},
    {"getPayPointForCdkey",       getPayPointForCdkey},
    {"setRmbPoint",                       setRmbPoint},
    {"getRmbPoint",                       getRmbPoint},
    {"setRmbPointForCdkey",         setRmbPointForCdkey},
    {"getRmbPointForCdkey",       getRmbPointForCdkey},
    {"setPetPoint",                       setPetPoint},
    {"getPetPoint",                       getPetPoint},
    {"setPetPointForCdkey",         setPetPointForCdkey},
    {"getPetPointForCdkey",       getPetPointForCdkey},
#ifdef _GLORY_POINT
    {"setGloryPoint",                         setGloryPoint},
    {"getGloryPoint",                         getGloryPoint},
#endif
    { "setCrystalPoint", setCrystalPoint },
    { "getCrystalPoint", getCrystalPoint },
    { "updateFmPointData", updateFmPointData },
    { "queryFmPointData", queryFmPointData },
    { "setCostPoint", setCostPoint },
    { "getCostPoint", getCostPoint },
    {NULL,                            NULL}
};

LUALIB_API int luaopen_Sasql(lua_State *L)
{
    luaL_register(L, "sasql", sasqllib);
    return 1;
}
#endif
#endif
#endif
