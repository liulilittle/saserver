#include <string.h>
#include "char_base.h"
#include "object.h"
#include "longzoro/version.h"
#include "readmap.h"
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
#include "mylua/base.h"

#ifdef _ALLBLUES_LUA

static CharBase CharBaseObj[] =
{
    {{"角色"},            OBJTYPE_CHARA}
    , {{"道具"},       OBJTYPE_ITEM}
    , {{"宠物"},       OBJTYPE_GOLD}
    , {{"跳点"},       OBJTYPE_WARPPOINT}
};

static int check(lua_State *L)
{
    const int index = luaL_checkint(L, 1);
    lua_pushinteger(L, CHECKOBJECT(index));
    return 1;
}

static int getType(lua_State *L)
{
    const int index = luaL_checkint(L, 1);
    lua_pushinteger(L, OBJECT_getType(index));
    return 1;
}

static int setType(lua_State *L)
{
    const int index = luaL_checkint(L, 1);
    const int element = getCharBaseValue(L, 2, CharBaseObj, arraysizeof(CharBaseObj));
    OBJECT_setType(index, element);
    return 1;
}

static int getFloor(lua_State *L)
{
    const int index = luaL_checkint(L, 1);
    lua_pushinteger(L, OBJECT_getFloor(index));
    return 1;
}

static int setFloor(lua_State *L)
{
    const int index = luaL_checkint(L, 1);
    const int floor = luaL_checkint(L, 2);
    OBJECT_setFloor(index, floor);
    return 1;
}

static int getX(lua_State *L)
{
    const int index = luaL_checkint(L, 1);
    lua_pushinteger(L, OBJECT_getX(index));
    return 1;
}

static int setX(lua_State *L)
{
    const int index = luaL_checkint(L, 1);
    const int x = luaL_checkint(L, 2);
    OBJECT_setX(index, x);
    return 1;
}

static int getY(lua_State *L)
{
    const int index = luaL_checkint(L, 1);
    lua_pushinteger(L, OBJECT_getY(index));
    return 1;
}

static int setY(lua_State *L)
{
    const int index = luaL_checkint(L, 1);
    const int y = luaL_checkint(L, 2);
    OBJECT_setY(index, y);
    return 1;
}

static int getIndex(lua_State *L)
{
    const int index = luaL_checkint(L, 1);
    lua_pushinteger(L, OBJECT_getIndex(index));
    return 1;
}

static int getNum(lua_State *L)
{
    lua_pushinteger(L, OBJECT_getNum());
    return 1;
}
static int setIndex(lua_State *L)
{
    const int index = luaL_checkint(L, 1);
    const int charaindex = luaL_checkint(L, 2);
    OBJECT_setIndex(index, charaindex);
    return 1;
}

static int getchartype(lua_State *L)
{
    const int index = luaL_checkint(L, 1);
    lua_pushinteger(L, OBJECT_getchartype(index));
    return 1;
}

static int setchartype(lua_State *L)
{
    const int index = luaL_checkint(L, 1);
    const int flg = luaL_checkint(L, 2);
    OBJECT_setchartype(index, flg);
    return 1;
}

static int DelObjectOne(lua_State *L)
{
    const int index = luaL_checkint(L, 1);
    endObjectOne(index);
    return 1;
}

static int getObjFromName(lua_State *L)
{
    size_t l;
    const int charaindex = luaL_checkint(L, 1);
    const int num = luaL_checkint(L, 2);
    const char *name = luaL_checklstring(L, 3, &l);
    int floor = CHAR_getInt(charaindex, CHAR_FLOOR);
    int x = CHAR_getInt(charaindex, CHAR_X);
    int y = CHAR_getInt(charaindex, CHAR_Y);
    int i, j;

    for (i = x - num; i <= x + num; i++)
    {
        for (j = y - num; j <= y + num; j++)
        {
            OBJECT object;

            for (object = MAP_getTopObj(floor, i, j); object;
                    object = NEXT_OBJECT(object))
            {
                int receiveobjindex = GET_OBJINDEX(object);
                int recvindex = OBJECT_getIndex(receiveobjindex);

                switch (OBJECT_getType(receiveobjindex))
                {
                    case OBJTYPE_CHARA:
                        if (CHAR_CHECKINDEX(recvindex))
                        {
                            if (strcmp(CHAR_getChar(recvindex, CHAR_NAME), name) == 0)
                            {
                                lua_pushinteger(L, 1);
                                return 1;
                            }
                        }

                    default:
                        break;
                }
            }
        }
    }

    lua_pushinteger(L, 0);
    return 1;
}

static int getObjFromType(lua_State *L)
{
    const int charaindex = luaL_checkint(L, 1);
    const int num = luaL_checkint(L, 2);
    const int type = luaL_checkint(L, 3);
    int floor = CHAR_getInt(charaindex, CHAR_FLOOR);
    int x = CHAR_getInt(charaindex, CHAR_X);
    int y = CHAR_getInt(charaindex, CHAR_Y);
    int i, j;

    for (i = x - num; i <= x + num; i++)
    {
        for (j = y - num; j <= y + num; j++)
        {
            OBJECT object;

            for (object = MAP_getTopObj(floor, i, j); object;
                    object = NEXT_OBJECT(object))
            {
                int receiveobjindex = GET_OBJINDEX(object);
                int recvindex = OBJECT_getIndex(receiveobjindex);

                switch (OBJECT_getType(receiveobjindex))
                {
                    case OBJTYPE_CHARA:
                        if (CHAR_CHECKINDEX(recvindex))
                        {
                            if (CHAR_getInt(recvindex, CHAR_WHICHTYPE) == type)
                            {
                                lua_pushinteger(L, 1);
                                return 1;
                            }
                        }

                    default:
                        break;
                }
            }
        }
    }

    lua_pushinteger(L, 0);
    return 1;
}


static const luaL_Reg objectlib[] =
{
    {"check",                   check},
    {"getType",               getType},
    {"setType",               setType},
    {"getFloor",              getFloor},
    {"setFloor",              setFloor},
    {"getX",                      getX},
    {"setX",                      setX},
    {"getY",                      getY},
    {"setY",                      setY},
    {"getIndex",              getIndex},
    {"setIndex",              setIndex},
    {"getNum",                    getNum},
    {"getchartype",       getchartype},
    {"setchartype",       setchartype},
    {"DelObjectOne",      DelObjectOne},
    { "getObjFromName", getObjFromName },
    { "getObjFromType", getObjFromType },
    {NULL, NULL}
};


LUALIB_API int luaopen_Object(lua_State *L)
{
    luaL_register(L, "obj", objectlib);
    return 1;
}

#endif

