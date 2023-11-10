#include <string.h>
#include "char.h"
#include "char_base.h"
#include "mylua/base.h"
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
#include "saacproto_cli.h"

#ifdef _ALLBLUES_LUA
static int ACFixFMPoint(lua_State *L)
{
    size_t l;
    char *winfmname = luaL_checklstring(L, 1, &l);
    const int winfmindex = luaL_checkint(L, 2);
    const int winindex = luaL_checkint(L, 3);
    char *losefmname = luaL_checklstring(L, 4, &l);
    const int losefmindex = luaL_checkint(L, 5);
    const int loseindex = luaL_checkint(L, 6);
    const int village = luaL_checkint(L, 7);
    saacproto_ACFixFMPoint_send(acfd, winfmname, winfmindex, winindex, losefmname, losefmindex, loseindex, village);
    return 1;
}

static int ACFMDetail(lua_State *L)
{
    size_t l;
    char *fmname = luaL_checklstring(L, 1, &l);
    const int fmindex = luaL_checkint(L, 2);
    const int tempindex = luaL_checkint(L, 3);
    const int fd = luaL_checkint(L, 4);
    saacproto_ACFMDetail_send(acfd, fmname, fmindex, tempindex, CONNECT_getFdid(fd));
    return 1;
}

static int ItemPetLocked(lua_State *L)
{
    size_t l;
    int charaindex = luaL_checkint(L, 1);
    int fd = getfdFromCharaIndex(charaindex);
    char *data = luaL_checklstring(L, 2, &l);
    saacproto_ItemPetLocked_send(acfd, CHAR_getChar(charaindex, CHAR_CDKEY), data);
    return 1;
}

static int ACKick(lua_State *L)
{
    size_t l;
    char *cdkey = luaL_checklstring(L, 1, &l);
    int fd = luaL_checkint(L, 2);
    int flg = luaL_checkint(L, 3);
    saacproto_ACKick_send(acfd, cdkey, fd, flg);
    return 1;
}
#ifdef _REMOTE_COMMAND_
static int GmCommand(lua_State *L)
{
    size_t l;
    char *message = luaL_checklstring(L, 1, &l);
    char magicname[128];

    if (getStringFromIndexWithDelim(message, " ", 1, magicname, sizeof(magicname)) == FALSE)
    {
        return 1;
    }

    if (strlen(message) == strlen(magicname))
    {
        saacproto_remoteCommand_send(magicname, "", -1, "");
    }
    else
    {
        saacproto_remoteCommand_send(magicname, message + strlen(magicname) + 1, -1, "");
    }

    return 1;
}
#endif

static int ACFixFMData(lua_State *L)
{
    size_t l;
    const int charaindex = luaL_checkint(L, 1);
    const int type = luaL_checkint(L, 2);
    char *data = luaL_checklstring(L, 3, &l);
    char *data2 = luaL_checklstring(L, 4, &l);
    saacproto_ACFixFMData_send(acfd,
                               CHAR_getChar(charaindex, CHAR_FMNAME), CHAR_getInt(charaindex, CHAR_FMINDEX), CHAR_getWorkInt(charaindex, CHAR_WORKFMINDEXI),
                               type, data, data2, CHAR_getWorkInt(charaindex, CHAR_WORKFMCHARINDEX), -1);
    return 1;
}

static int LUAACShowFMList(lua_State *L)
{
    saacproto_ACShowFMList_send(acfd);
    return 1;
}

static int CreateHeader(lua_State *L)
{
    size_t l;
    const char *data = luaL_checklstring(L, 1, &l);
    saacproto_CreateHeader(saacproto.work, data);
    return 1;
}

static int strcatsafeint(lua_State *L)
{
    const int data = luaL_checkint(L, 1);
    saacproto_strcatsafe(saacproto.work, saacproto_mkstr_int(data), saacproto.workbufsize);
    return 1;
}

static int strcatsafechar(lua_State *L)
{
    size_t l;
    const char *data = luaL_checklstring(L, 1, &l);
    saacproto_strcatsafe(saacproto.work, saacproto_mkstr_string(data), saacproto.workbufsize);
    return 1;
}

static int Send(lua_State *L)
{
    saacproto_Send(acfd, saacproto.work);
    return 1;
}

static int OtherCommand(lua_State *L)
{
    size_t l;
    const int toid = luaL_checkint(L, 1);
    char *cmdcomand = luaL_checklstring(L, 2, &l);
    char *cmddata = luaL_checklstring(L, 3, &l);
    char *cmddata2 = luaL_checklstring(L, 4, &l);
    char *cmddata3 = luaL_checklstring(L, 5, &l);
    saacproto_OtherCommand_send(toid, cmdcomand, cmddata, cmddata2, cmddata3);
    return 1;
}

static const luaL_Reg Saacprotolib[] =
{
    {"ACFixFMPoint",    ACFixFMPoint},
    {"ACFMDetail",      ACFMDetail},
    {"ItemPetLocked", ItemPetLocked},
    {"ACKick",      ACKick},
#ifdef _REMOTE_COMMAND_
    { "GmCommand", GmCommand },
#endif
    { "ACFixFMData", ACFixFMData },
    { "ACShowFMList", LUAACShowFMList },
    { "CreateHeader", CreateHeader },
    { "strcatsafeint", strcatsafeint },
    { "strcatsafechar", strcatsafechar },
    { "Send", Send },
    { "OtherCommand", OtherCommand },
    {NULL,                        NULL}
};

LUALIB_API int luaopen_Saacproto(lua_State *L)
{
    luaL_register(L, "saacproto", Saacprotolib);
    return 1;
}

#endif

