#include <string.h>
#include "lua.h"
#include "longzoro/version.h"
#include "lauxlib.h"
#include "lualib.h"
#include "net.h"
#include "log.h"

#ifdef _ALLBLUES_LUA

static int endOne(lua_State *L)
{
    const int sockfd = luaL_checkint(L, 1);
    CONNECT_endOne_debug(sockfd);
    return 1;
}

static int userip(lua_State *L)
{
    int sockfd = luaL_checkint(L, 1);
    unsigned long ip;
    int a, b, c, d;
    char strIP[32];
    ip = CONNECT_get_userip(sockfd);
    a = (ip % 0x100);
    ip = ip / 0x100;
    b = (ip % 0x100);
    ip = ip / 0x100;
    c = (ip % 0x100);
    ip = ip / 0x100;
    d = (ip % 0x100);
    sprintf(strIP, "%d.%d.%d.%d", a, b, c, d);
    lua_pushstring(L, strIP);
    return 1;
}
#ifdef _NEWCLISETSERVID
static int getServid(lua_State *L)
{
    int charaindex = luaL_checkint(L, 1);
    int sockfd = getfdFromCharaIndex(charaindex);
    lua_pushinteger(L, CONNECT_getServid(sockfd));
    return 1;
}
#endif
#ifdef _NEWCLISETMAC
static int getMac(lua_State *L)
{
    int sockfd = luaL_checkint(L, 1);
    char mac[128];
    CONNECT_getMAC(sockfd, mac, sizeof(mac));
    lua_pushstring(L, mac);
    return 1;
}
static int getMac2(lua_State *L)
{
    int sockfd = luaL_checkint(L, 1);
    char mac[128];
    CONNECT_getMAC2(sockfd, mac, sizeof(mac));
    lua_pushstring(L, mac);
    return 1;
}
static int getMac3(lua_State *L)
{
    int sockfd = luaL_checkint(L, 1);
    char mac[128];
    CONNECT_getMAC3(sockfd, mac, sizeof(mac));
    lua_pushstring(L, mac);
    return 1;
}
#endif

static int getIP(lua_State *L)
{
    int sockfd = luaL_checkint(L, 1);
    unsigned long ip;
    int a, b, c, d;
    char strIP[32];
    ip = CONNECT_get_userip(sockfd);
    a = (ip % 0x100);
    ip = ip / 0x100;
    b = (ip % 0x100);
    ip = ip / 0x100;
    c = (ip % 0x100);
    ip = ip / 0x100;
    d = (ip % 0x100);
    sprintf(strIP, "%d.%d.%d.%d", a, b, c, d);
    lua_pushstring(L, strIP);
    return 1;
}



static int getUse(lua_State *L)
{
    int fd = luaL_checkint(L, 1);
    lua_pushinteger(L, CONNECT_getUse(fd));
    return 1;
}
static int getCharaindex(lua_State *L)
{
    int fd = luaL_checkint(L, 1);
    lua_pushinteger(L, CONNECT_getCharaindex(fd));
    return 1;
}

static int getCdkey(lua_State *L)
{
    int fd = luaL_checkint(L, 1);
    char cdkey[16];
    CONNECT_getCdkey(fd, cdkey, sizeof(cdkey));
    lua_pushstring(L, cdkey);
    return 1;
}
static int getPasswd(lua_State *L)
{
    int fd = luaL_checkint(L, 1);
    char passwd[16];
    CONNECT_getPasswd(fd, passwd, sizeof(passwd));
    lua_pushstring(L, passwd);
    return 1;
}
static int getSocketType(lua_State *L)
{
    int fd = luaL_checkint(L, 1);
    lua_pushinteger(L, CONNECT_getState(fd));
    return 1;
}
static int setCloseRequest(lua_State *L)
{
    const int sockfd = luaL_checkint(L, 1);
    const int cnt = luaL_checkint(L, 2);
    CONNECT_setCloseRequest(sockfd, cnt);
    return 1;
}
static int setCdkey(lua_State *L)
{
    size_t l;
    int sockfd = luaL_checkint(L, 1);
    char *data = luaL_checklstring(L, 2, &l);
    CONNECT_setCdkey(sockfd, data);
    return 1;
}
static int setPasswd(lua_State *L)
{
    size_t l;
    int sockfd = luaL_checkint(L, 1);
    char *data = luaL_checklstring(L, 2, &l);
    CONNECT_setPasswd(sockfd, data);
    return 1;
}

static int setCtype(lua_State *L)
{
    int fd = luaL_checkint(L, 1);
    int index = luaL_checkint(L, 2);
    CONNECT_setCtype(fd, index);
    return 1;
}

static int setServid(lua_State *L)
{
    int fd = luaL_checkint(L, 1);
    int index = luaL_checkint(L, 2);
    CONNECT_setServid(fd, index);
    return 1;
}

static int setMAC1(lua_State *L)
{
    size_t l;
    int sockfd = luaL_checkint(L, 1);
    char *data = luaL_checklstring(L, 2, &l);
    CONNECT_setMAC(sockfd, data);
    return 1;
}
static int setMAC2(lua_State *L)
{
    size_t l;
    int sockfd = luaL_checkint(L, 1);
    char *data = luaL_checklstring(L, 2, &l);
    CONNECT_setMAC2(sockfd, data);
    return 1;
}
static int setMAC3(lua_State *L)
{
    size_t l;
    int sockfd = luaL_checkint(L, 1);
    char *data = luaL_checklstring(L, 2, &l);
    CONNECT_setMAC3(sockfd, data);
    return 1;
}

#ifdef _IPHONE_MARK_
static int setloginmark(lua_State *L)
{
    int fd = luaL_checkint(L, 1);
    int index = luaL_checkint(L, 2);
    CONNECT_setloginmark(fd, index);
    return 1;
}

static int getloginmark(lua_State *L)
{
    int fd = luaL_checkint(L, 1);
    lua_pushinteger(L, CONNECT_getloginmark(fd));
    return 1;
}
#endif
static int getconnecttime(lua_State *L)
{
    int fd = luaL_checkint(L, 1);
    lua_pushinteger(L, CONNECT_getConnectTime(fd));
    return 1;
}
#ifdef _NEW_NETSENDRECV
static int setFuncNum(lua_State *L)
{
    int fd = luaL_checkint(L, 1);
    int index = luaL_checkint(L, 2);
    CONNECT_setFuncNum(fd, index);
    return 1;
}
static int setNewDefaultKey(lua_State *L)
{
    size_t l;
    int fd = luaL_checkint(L, 1);
    char *data = luaL_checklstring(L, 2, &l);
    CONNECT_setNewDefaultKey(fd, data);
    return 1;
}
#endif
static int getState(lua_State *L)
{
    int fd = luaL_checkint(L, 1);
    lua_pushinteger(L, CONNECT_getState(fd));
    return 1;
}
static int setState(lua_State *L)
{
    int fd = luaL_checkint(L, 1);
    int type = luaL_checkint(L, 2);

    if (CONNECT_checkfd(fd) == FALSE)
    {
        return 1;
    }

    CONNECT_setState(fd, type);
    return 1;
}
#ifdef _NEW_PACKET_CODE
static int LUAsetNewDefaultKey(lua_State *L)
{
    size_t l;
    const int id = luaL_checkint(L, 1);
    const int type = luaL_checkint(L, 2);
    char *data = luaL_checklstring(L, 3, &l);
    setNewPacketData(id, type, data);
    return 1;
}
#endif
static int getFdid(lua_State *L)
{
    int fd = luaL_checkint(L, 1);
    lua_pushinteger(L, CONNECT_getFdid(fd));
    return 1;
}
static const luaL_Reg netlib[] =
{
    {"endOne",                    endOne},
    {"userip",                    userip},
#ifdef _NEWCLISETSERVID
    {"getServid",             getServid},
#endif
#ifdef _NEWCLISETMAC
    {"getMac",                    getMac},
    {"getMac2",                   getMac2},
    {"getMac3",                   getMac3},
#endif
    {"getIP",                   getIP},
    {"getUse",                  getUse},
    {"getCharaindex",   getCharaindex},
    {"getCdkey",                getCdkey},
    {"getPasswd",           getPasswd},
    {"getSocketType",   getSocketType},
    {"setCloseRequest",     setCloseRequest},
#ifdef _IPHONE_MARK_
    { "setloginmark", setloginmark },
    { "getloginmark", getloginmark },
#endif
    { "setCdkey", setCdkey },
    { "setPasswd", setPasswd },
    { "setCtype", setCtype },
    { "setServid", setServid },
    { "setMAC1", setMAC1 },
    { "setMAC2", setMAC2 },
    { "setMAC3", setMAC3 },
    { "getconnecttime", getconnecttime },
#ifdef _NEW_NETSENDRECV
    { "setFuncNum", setFuncNum },
    { "setNewDefaultKey", setNewDefaultKey },
#endif
    { "getState", getState },
    { "setState", setState },
#ifdef _NEW_PACKET_CODE
    { "LUAsetNewDefaultKey", LUAsetNewDefaultKey },
#endif
    { "getFdid", getFdid },
    {NULL,                          NULL}
};


LUALIB_API int luaopen_Net(lua_State *L)
{
    luaL_register(L, "net", netlib);
    return 1;
}

#endif
