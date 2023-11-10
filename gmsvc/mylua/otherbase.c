#include <string.h>
#include <time.h>
#include <sys/time.h>
#include "char.h"
#include "char_base.h"
#include "mylua/base.h"
#include "util.h"
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
#include "lssproto_serv.h"
#include "mylua/mylua.h"
//#include "md5_encrypt.h"
#include "npc_scheduleman.h"

extern char firstrandToken[900000][12];
extern char secondrandToken[900000][20];

#ifdef _ALLBLUES_LUA
extern MY_Lua MYLua;

static int nowtime(lua_State *L)
{
    lua_pushinteger(L, (int) time(NULL));
    return 1;
}

static int strtoi(lua_State *L)
{
    size_t l;
    char *data = luaL_checklstring(L, 1, &l);
    lua_pushinteger(L, atoi(data));
    return 1;
}

static int getString(lua_State *L)
{
    size_t l;
    char *data = luaL_checklstring(L, 1, &l);
    char *delim = luaL_checklstring(L, 2, &l);
    const int index = luaL_checkint(L, 3);
    char token[256];

    if (getStringFromIndexWithDelim(data, delim, index, token, sizeof(token)))
    {
        lua_pushstring(L, token);
    }
    else
    {
        lua_pushstring(L, "");
    }

    return 1;
}

static int CallFunction(lua_State *L)
{
    size_t l;
    int i;
    const char *funcname = luaL_checklstring(L, 1, &l);
    const char *filename = luaL_checklstring(L, 2, &l);

    if (funcname == NULL || filename == NULL)
    {
        return luaL_error(L, "CallFunction error! file or func is null!\n");
    }

    luaL_checktype(L, 3, LUA_TTABLE);
    int n = luaL_getn(L, 3);
    lua_State *lua = NULL;
    MY_Lua *mylua = &MYLua;

    while (mylua->next != NULL)
    {
        if (strcmp(filename, mylua->luapath) == 0)
        {
            lua = mylua->lua;
            break;
        }

        mylua = mylua->next;
    }

    if (lua == NULL)
    {
        return 0;
    }

    //print("\ngettop1 =%d, =%s", lua_gettop(lua), lua_typename(lua, lua_type(lua, -1)));
    lua_settop(lua, 0);
    lua_getglobal(lua, funcname);

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return luaL_error(L, "file[%s] has no func[%s]!", filename, funcname);
    }

    if (n > 0)
    {
        for (i = 0; i < n; i++)
        {
            lua_pushnumber(L, i + 1);
            lua_gettable(L, -2);

            if (lua_isnumber(L, -1))
            {
                int ret = lua_tonumber(L, -1);
                lua_pop(L, 1);
                lua_pushnumber(lua, ret);
            }
            else if (lua_isstring(L, -1))
            {
                const char *ret = lua_tolstring(L, -1, &l);
                lua_pop(L, 1);
                lua_pushlstring(lua, ret, l);
            }
            else if (lua_isuserdata(L, -1))
            {
                void * ret = lua_touserdata(L, -1);
                lua_pop(L, 1);
                lua_pushlightuserdata(lua, ret);
            }
            else
            {
                lua_settop(lua, 0);
                const char* para = lua_typename(L, lua_type(L, -1));
                return luaL_error(L, "invalid type[%s]! arg[%d]", para, i);
            }
        }
    }

    /**
     * \brief for error print
     *
     */
    //print("\ngettop2 =%d", lua_gettop(lua));

    if (docall(lua, n, 0) != 0)
    {
        return 0;
    }

    /* int TM_Ret = lua_pcall(lua, n, 1, 0);

     if (TM_Ret != 0)
     {
         lua_pop(lua, 1);
         return 0;
     }*/
    lua_settop(L, 0);
    int luatop = lua_gettop(lua);
    //print("\ngettop3 =%d,filename=%s,funcname=%s", luatop, filename, funcname);

    for (i = 0; i < luatop; i++)
    {
        if (lua_isnumber(lua, -1))
        {
            int ret = lua_tonumber(lua, -1);
            lua_pushnumber(L, ret);
        }
        else if (lua_isstring(lua, -1))
        {
            const char *ret = lua_tolstring(lua, -1, &l);
            lua_pushlstring(L, ret, l);
        }
        else if (lua_isuserdata(lua, -1))
        {
            void * ret = lua_touserdata(lua, -1);
            lua_pushlightuserdata(L, ret);
        }
        else
        {
            lua_pushnil(L);
        }

        //print("\ni=%d", i);
        lua_insert(L, 1);
        lua_pop(lua, 1);
    }

    //lua_settop(lua, 0);
    return lua_gettop(L);
}


static int c10to62(lua_State *L)
{
    const int index = luaL_checkint(L, 1);
    char token[256];
    cnv10to62(index,    token, sizeof(token));
    lua_pushstring(L, token);
    return 1;
}
#ifdef _OFFLINE_SYSTEM
int luaplayernum = 0;
static int setLuaPLayerNum(lua_State *L)
{
    const int num = luaL_checkint(L, 1);
    luaplayernum = num;
    return 1;
}

static int getLuaPLayerNum(lua_State *L)
{
    lua_pushinteger(L, luaplayernum);
    return 1;
}
extern playeronlinenum;

static int getOnlinePlayer(lua_State *L)
{
    lua_pushinteger(L, playeronlinenum + luaplayernum + getPlayerNum());
    return 1;
}
#endif

static int NumToAlpha(lua_State *L)
{
    const int Num = luaL_checkint(L, 1);
    /*
        char Alpha[10];
        int i;
        for(i = 0; i < 9; i ++){
            int validation = Num;
            validation /= pow((double)26, i);
            if(validation == 0)break;
            Alpha[i] = 'A' + (validation- 1) % 26;
        }
        Alpha[i] = '\0';
        lua_pushstring(L, Alpha);
    */
    int a  = Num * Num - 213;
    a = a << 1;
    lua_pushinteger(L, a);
    return 1;
}

static int AlphaToNum(lua_State *L)
{
    size_t l;
    char *Alpha = luaL_checklstring(L, 1, &l);
    int Num = 0;
    int i;

    for (i = 0; i < strlen(Alpha); i ++)
    {
        if (Alpha[i] >= 'A' && Alpha[i] <= 'Z')
        {
            Num += (((Alpha[i] - 'A') + 1) * pow(26, i));
        }
        else if (Alpha[i] >= 'a' && Alpha[i] <= 'z')
        {
            Num += (((Alpha[i] - 'a') + 1) * pow(26, i));
        }
    }

    lua_pushinteger(L, Num);
    return 1;
}

static int DataAndData(lua_State *L)
{
    const int data1 = luaL_checkint(L, 1);
    const int data2 = luaL_checkint(L, 2);
    lua_pushinteger(L, (data1 & 1 << data2));
    return 1;
}

static int NumLeftToNum(lua_State *L)
{
    const int value = luaL_checkint(L, 1);
    const int flg = luaL_checkint(L, 2);
    lua_pushinteger(L, (value << flg));
    return 1;
}

static int NumRightToNum(lua_State *L)
{
    const int value = luaL_checkint(L, 1);
    const int flg = luaL_checkint(L, 2);
    lua_pushinteger(L, ((value >> flg) & 0xFF));
    return 1;
}

static int NumOrNum(lua_State *L)
{
    const int value = luaL_checkint(L, 1);
    const int value2 = luaL_checkint(L, 2);
    lua_pushinteger(L, value | value2);
    return 1;
}

static int NumAndNum(lua_State *L)
{
    const int value = luaL_checkint(L, 1);
    const int value2 = luaL_checkint(L, 2);
    lua_pushinteger(L, value & value2);
    return 1;
}

static int getIntDesRand(lua_State *L)
{
    const int value = luaL_checkint(L, 1);

    if (value < 1 || value > 900000)
    {
        lua_pushinteger(L, 100000);
    }
    else
    {
        lua_pushinteger(L, atoi(firstrandToken[value - 1]));
    }

    return 1;
}

static int getCharDesRand(lua_State *L)
{
    const int value = luaL_checkint(L, 1);

    if (value < 1 || value > 900000)
    {
        lua_pushstring(L, "598B64586FAA4C15");
    }
    else
    {
        lua_pushstring(L, secondrandToken[value - 1]);
    }

    return 1;
}

static int getMd5String(lua_State *L)
{
    size_t l;
    char *data = luaL_checklstring(L, 1, &l);
    char *md5 = MD5String(data);
    lua_pushstring(L, md5);
    return 1;
}

static int luagettimeofday(lua_State *L)
{
    struct timeval tv;
    struct timezone tz;
    gettimeofday(&tv, &tz);
    lua_pushinteger(L, tv.tv_usec);
    return 1;
}

static int DataOrData(lua_State *L)
{
    const int data1 = luaL_checkint(L, 1);
    const int data2 = luaL_checkint(L, 2);
    lua_pushinteger(L, (data1 | 1 << data2));
    return 1;
}
static int DataNotData(lua_State *L)
{
    const int data1 = luaL_checkint(L, 1);
    const int data2 = luaL_checkint(L, 2);
    lua_pushinteger(L, (data1 & (~(1 << data2))));
    return 1;
}


static int getFmPKsGindex(lua_State *L)
{
    const int fmpks_pos = luaL_checkint(L, 1) * MAX_SCHEDULE;
    lua_pushinteger(L, (fmpks[fmpks_pos + 1].guest_index));
    return 1;
}
static int getFmPKsHindex(lua_State *L)
{
    const int fmpks_pos = luaL_checkint(L, 1) * MAX_SCHEDULE;
    lua_pushinteger(L, (fmpks[fmpks_pos + 1].host_index));
    return 1;
}

static int getFmPKsDueltime(lua_State *L)
{
    const int fmpks_pos = luaL_checkint(L, 1) * MAX_SCHEDULE;
    lua_pushinteger(L, (fmpks[fmpks_pos + 1].dueltime));
    return 1;
}

static int getFmPKsPreparetime(lua_State *L)
{
    const int fmpks_pos = luaL_checkint(L, 1) * MAX_SCHEDULE;
    lua_pushinteger(L, (fmpks[fmpks_pos + 1].prepare_time));
    return 1;
}


static int NumLeftToNum1(lua_State *L)
{
    const int data1 = luaL_checkint(L, 1);
    const int data2 = luaL_checkint(L, 2);
    lua_pushinteger(L, (data1 << data2));
    return 1;
}
static int Random(lua_State *L)
{
    const int rand1 = luaL_checkint(L, 1);
    const int rand2 = luaL_checkint(L, 2);
    lua_pushinteger(L, RAND(rand1, rand2));
    return 1;
}
static int RandItemId(lua_State *L)
{
    size_t l;
    char *TM_ItemIdData = luaL_checklstring(L, 1, &l);
    int TM_Num = 1;
    int TM_Int = -1;
    int TM_ItemId = -1;
    int TM_ItemIdStart = -1;
    int TM_ItemIdEnd = -1;
    char ItemIdBuf[32];

    if (getStringFromIndexWithDelim(TM_ItemIdData, ",", 2, ItemIdBuf, sizeof(ItemIdBuf)) != FALSE)
    {
        int TM_ItemNum = 0;

        while (1)
        {
            if (getStringFromIndexWithDelim(TM_ItemIdData, ",", TM_ItemNum + 1, ItemIdBuf, sizeof(ItemIdBuf)) == FALSE)
            {
                break;
            }

            TM_ItemNum++;
        }

        TM_ItemNum = RAND(1, TM_ItemNum);
        getStringFromIndexWithDelim(TM_ItemIdData, ",", TM_ItemNum, ItemIdBuf, sizeof(ItemIdBuf));
        char ItemIdBuf2[16];

        if (getStringFromIndexWithDelim(ItemIdBuf, "-", 2, ItemIdBuf2, sizeof(ItemIdBuf2)) != FALSE)
        {
            int TM_ItemIdStart, TM_ItemIdEnd;
            getStringFromIndexWithDelim(ItemIdBuf, "-", 1, ItemIdBuf2, sizeof(ItemIdBuf2));
            TM_ItemIdStart = atoi(ItemIdBuf2);
            getStringFromIndexWithDelim(ItemIdBuf, "-", 2, ItemIdBuf2, sizeof(ItemIdBuf2));
            TM_ItemIdEnd = atoi(ItemIdBuf2);
            TM_ItemId = RAND(TM_ItemIdStart, TM_ItemIdEnd);
        }
        else
        {
            TM_ItemId = atoi(ItemIdBuf);
        }
    }
    else if (getStringFromIndexWithDelim(TM_ItemIdData, "-", 2, ItemIdBuf, sizeof(ItemIdBuf)) != FALSE)
    {
        if (getStringFromIndexWithDelim(TM_ItemIdData, "-", 1, ItemIdBuf, sizeof(ItemIdBuf)) != FALSE)
        {
            TM_ItemIdStart = atoi(ItemIdBuf);
        }

        if (getStringFromIndexWithDelim(TM_ItemIdData, "-", 2, ItemIdBuf, sizeof(ItemIdBuf)) != FALSE)
        {
            TM_ItemIdEnd = atoi(ItemIdBuf);
        }

        if (TM_ItemIdStart <= TM_ItemIdEnd)
        {
            TM_ItemId = RAND(TM_ItemIdStart, TM_ItemIdEnd);
        }
    }
    else
    {
        TM_ItemId = atoi(TM_ItemIdData);
    }

    lua_pushinteger(L, TM_ItemId);
    return 1;
}
static int LUAencryptStr(lua_State *L)
{
    size_t l;
    char *data = luaL_checklstring(L, 1, &l);
    lua_pushstring(L, encryptStr(data));
    return 1;
}
static int LUAdecryptStr(lua_State *L)
{
    size_t l;
    char *data = luaL_checklstring(L, 1, &l);
    lua_pushstring(L, decryptStr(data));
    return 1;
}
static int getStringFromEscaped(lua_State *L)
{
    size_t l;
    char *data = luaL_checklstring(L, 1, &l);
    lua_pushstring(L, makeStringFromEscaped(data));
    return 1;
}
static int checkString(lua_State *L)
{
    size_t l;
    char *data = luaL_checklstring(L, 1, &l);
    lua_pushinteger(L, checkStringErr(data));
    return 1;
}

static int LuagetHash(lua_State *L)
{
    extern int getHash(char* s);
    size_t l;
    char *data = luaL_checklstring(L, 1, &l);
    lua_pushinteger(L, getHash(data));
    return 1;
}

static const luaL_Reg otherlib[] =
{
    {"time",                    nowtime},
    {"atoi",                    strtoi},
    {"getString",       getString},
    {"CallFunction",    CallFunction},
    {"c10to62",             c10to62},
    {"NumToAlpha",      NumToAlpha},
    {"AlphaToNum",      AlphaToNum},
#ifdef _OFFLINE_SYSTEM
    {"setLuaPLayerNum",             setLuaPLayerNum},
    {"getLuaPLayerNum",             getLuaPLayerNum},
    {"getOnlinePlayer",             getOnlinePlayer},
#endif
    {"DataAndData",             DataAndData},
    {"NumLeftToNum",                        NumLeftToNum},
    {"NumRightToNum",                     NumRightToNum},
    {"NumOrNum",                  NumOrNum},
    {"NumAndNum",                 NumAndNum},
    {"getIntDesRand",                     getIntDesRand},
    {"getCharDesRand",                    getCharDesRand},
    {"md5",                   getMd5String},
    {"gettimeofday",  luagettimeofday},
    { "DataOrData", DataOrData },
    { "DataNotData", DataNotData },
    { "getFmPKsGindex", getFmPKsGindex },
    { "getFmPKsHindex", getFmPKsHindex },
    { "getFmPKsDueltime", getFmPKsDueltime },
    { "getFmPKsPreparetime", getFmPKsPreparetime },
    { "NumLeftToNum1", NumLeftToNum1 },
    { "Random", Random },
    { "RandItemId", RandItemId },
    { "encryptStr", LUAencryptStr },
    { "decryptStr", LUAdecryptStr },
    { "getStringFromEscaped", getStringFromEscaped },
    { "checkString", checkString },
    { "getHash", LuagetHash },
    {NULL,                        NULL}
};

LUALIB_API int luaopen_Other(lua_State *L)
{
    luaL_register(L, "other", otherlib);
    return 1;
}

#endif

