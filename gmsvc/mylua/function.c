#include <string.h>
#include "common.h"
#include "char_base.h"
#include "item.h"
#include "configfile.h"
#include "battle.h"
#include "mylua/base.h"
#include "longzoro/version.h"
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
#include "mylua/mylua.h"

#ifdef _ALLBLUES_LUA
extern MY_Lua MYLua;

lua_State *FindLua(char *filename)
{
    MY_Lua *mylua = &MYLua;
    char newfilename[256];

    while (mylua->next != NULL)
    {
        if (strcmptail(mylua->luapath, ".allblues") == 0)
        {
            sprintf(newfilename, "%s.allblues", filename);
        }
        else
        {
            sprintf(newfilename, "%s", filename);
        }

        if (strcmp(newfilename, mylua->luapath) == 0)
        {
            return mylua->lua;
        }

        mylua = mylua->next;
    }

    return NULL;
}

#ifdef _ALLBLUES_LUA_1_9
BOOL BattleCommand(int charaindex, int battleindex)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/battlecommand.lua");

        if (lua == NULL)
        {
            return TRUE;
        }
    }

    lua_getglobal(lua, "BattleCommand");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return TRUE;
    }

    //依次放入三个参数
    lua_pushnumber(lua, charaindex);
    lua_pushnumber(lua, battleindex);
    docall(lua, 2, 1);
    return TRUE;
}


int FreeItemInslay(int charaindex, int inslayindex, int itemindex)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/iteminslay.lua");

        if (lua == NULL)
        {
            return TRUE;
        }
    }

    lua_getglobal(lua, "FreeItemInslay");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return TRUE;
    }

    //依次放入三个参数
    lua_pushnumber(lua, charaindex);
    lua_pushnumber(lua, inslayindex);
    lua_pushnumber(lua, itemindex);

    if (lua_pcall(lua, 3, 1, 0) != 0)
    {
        printf("error running function `f': %s", lua_tostring(lua, -1));
    }

    if (!lua_isnumber(lua, -1))
    {
        printf("function `f' must return a number");
    }

    int ret = lua_tonumber(lua, -1);
    lua_pop(lua, 1);
    return ret;
}
#ifdef _LOCK_PET_ITEM
#ifdef _CTRL_TRANS_DEVELOP
int FreeCtrlTransDevelop(int petindex, int type)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/ctrltransdevelop.lua");

        if (lua == NULL)
        {
            return 0;
        }
    }

    lua_getglobal(lua, "FreeCtrlTransDevelop");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return 0;
    }

    //依次放入三个参数
    lua_pushnumber(lua, petindex);
    lua_pushnumber(lua, type);

    if (lua_pcall(lua, 2, 1, 0) != 0)
    {
        printf("error running function `f': %s", lua_tostring(lua, -1));
    }

    if (!lua_isnumber(lua, -1))
    {
        printf("function `f' must return a number");
    }

    int ret = lua_tonumber(lua, -1);
    lua_pop(lua, 1);
    return ret;
}
#endif
int FreeFmPk()
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/fmpk.lua");

        if (lua == NULL)
        {
            return TRUE;
        }
    }

    lua_getglobal(lua, "FreeFmPk");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return TRUE;
    }

    //依次放入三个参数
    if (lua_pcall(lua, 0, 1, 0) != 0)
    {
        printf("error running function `f': %s", lua_tostring(lua, -1));
    }

    if (!lua_isnumber(lua, -1))
    {
        printf("function `f' must return a number");
    }

    int ret = lua_tonumber(lua, -1);
    lua_pop(lua, 1);
    return ret;
}

int FreeStreetVendorPet(int charaindex, int petindex)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/streetvendor.lua");

        if (lua == NULL)
        {
            return TRUE;
        }
    }

    lua_getglobal(lua, "FreeStreetVendorPet");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return TRUE;
    }

    //依次放入三个参数
    lua_pushnumber(lua, charaindex);
    lua_pushnumber(lua, petindex);

    if (lua_pcall(lua, 2, 1, 0) != 0)
    {
        printf("error running function `f': %s", lua_tostring(lua, -1));
    }

    if (!lua_isnumber(lua, -1))
    {
        printf("function `f' must return a number");
    }

    int ret = lua_tonumber(lua, -1);
    lua_pop(lua, 1);
    return ret;
}

int FreeStreetVendorItem(int charaindex, int itemindex)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/streetvendor.lua");

        if (lua == NULL)
        {
            return TRUE;
        }
    }

    lua_getglobal(lua, "FreeStreetVendorItem");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return TRUE;
    }

    //依次放入三个参数
    lua_pushnumber(lua, charaindex);
    lua_pushnumber(lua, itemindex);

    if (lua_pcall(lua, 2, 1, 0) != 0)
    {
        printf("error running function `f': %s", lua_tostring(lua, -1));
    }

    if (!lua_isnumber(lua, -1))
    {
        printf("function `f' must return a number");
    }

    int ret = lua_tonumber(lua, -1);
    (lua, 1);
    return ret;
}


int FreeStreetVendorName(int charaindex, char* data)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/streetvendor.lua");

        if (lua == NULL)
        {
            return TRUE;
        }
    }

    lua_getglobal(lua, "FreeStreetVendorName");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return TRUE;
    }

    //依次放入三个参数
    lua_pushnumber(lua, charaindex);
    lua_pushstring(lua, data);

    if (lua_pcall(lua, 2, 1, 0) != 0)
    {
        printf("error running function `f': %s", lua_tostring(lua, -1));
    }

    if (!lua_isnumber(lua, -1))
    {
        printf("function `f' must return a number");
    }

    int ret = lua_tonumber(lua, -1);
    lua_pop(lua, 1);
    return ret;
}

int FreeTrade(int meindex, int toindex)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/trade.lua");

        if (lua == NULL)
        {
            return TRUE;
        }
    }

    lua_getglobal(lua, "FreeTrade");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return TRUE;
    }

    //依次放入三个参数
    lua_pushnumber(lua, meindex);
    lua_pushnumber(lua, toindex);

    if (lua_pcall(lua, 2, 1, 0) != 0)
    {
        printf("error running function `f': %s", lua_tostring(lua, -1));
    }

    if (!lua_isnumber(lua, -1))
    {
        printf("function `f' must return a number");
    }

    int ret = lua_tonumber(lua, -1);
    lua_pop(lua, 1);
    return ret;
}

int FreeTradePet(int charaindex, int petindex)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/trade.lua");

        if (lua == NULL)
        {
            return TRUE;
        }
    }

    lua_getglobal(lua, "FreeTradePet");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return TRUE;
    }

    //依次放入三个参数
    lua_pushnumber(lua, charaindex);
    lua_pushnumber(lua, petindex);

    if (lua_pcall(lua, 2, 1, 0) != 0)
    {
        printf("error running function `f': %s", lua_tostring(lua, -1));
    }

    if (!lua_isnumber(lua, -1))
    {
        printf("function `f' must return a number");
    }

    int ret = lua_tonumber(lua, -1);
    lua_pop(lua, 1);
    return ret;
}

int FreeTradeItem(int charaindex, int itemindex)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/trade.lua");

        if (lua == NULL)
        {
            return TRUE;
        }
    }

    lua_getglobal(lua, "FreeTradeItem");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return TRUE;
    }

    //依次放入三个参数
    lua_pushnumber(lua, charaindex);
    lua_pushnumber(lua, itemindex);

    if (lua_pcall(lua, 2, 1, 0) != 0)
    {
        printf("error running function `f': %s", lua_tostring(lua, -1));
    }

    if (!lua_isnumber(lua, -1))
    {
        printf("function `f' must return a number");
    }

    int ret = lua_tonumber(lua, -1);
    lua_pop(lua, 1);
    return ret;
}

int FreeTradeGold(int meindex, int toindex, int gold)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/trade.lua");

        if (lua == NULL)
        {
            return TRUE;
        }
    }

    lua_getglobal(lua, "FreeTradeGold");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return TRUE;
    }

    //依次放入三个参数
    lua_pushnumber(lua, meindex);
    lua_pushnumber(lua, toindex);
    lua_pushnumber(lua, gold);

    if (lua_pcall(lua, 3, 1, 0) != 0)
    {
        printf("error running function `f': %s", lua_tostring(lua, -1));
    }

    if (!lua_isnumber(lua, -1))
    {
        printf("function `f' must return a number");
    }

    int ret = lua_tonumber(lua, -1);
    lua_pop(lua, 1);
    return ret;
}


int FreeLockPetSave(int charaindex, int petindex)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/lockpetsave.lua");

        if (lua == NULL)
        {
            return TRUE;
        }
    }

    lua_getglobal(lua, "FreeLockPetSave");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return TRUE;
    }

    //依次放入三个参数
    lua_pushnumber(lua, charaindex);
    lua_pushnumber(lua, petindex);

    if (lua_pcall(lua, 2, 1, 0) != 0)
    {
        printf("error running function `f': %s", lua_tostring(lua, -1));
    }

    if (!lua_isnumber(lua, -1))
    {
        printf("function `f' must return a number");
    }

    int ret = lua_tonumber(lua, -1);
    lua_pop(lua, 1);
    return ret;
}
#endif
void FreeAddFMAdv(int charaindex, int shiftbit)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/addfmadv.lua");

        if (lua == NULL)
        {
            return;
        }
    }

    lua_getglobal(lua, "FreeAddFMAdv");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return ;
    }

    //依次放入三个参数
    lua_pushnumber(lua, charaindex);
    lua_pushnumber(lua, shiftbit);
    docall(lua, 2, 1);
}
#ifdef _FIRST_LOCK_ITEM
void FreeFirstLockPet(int charaindex, int petindex)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/firstlock.lua");

        if (lua == NULL)
        {
            return ;
        }
    }

    lua_getglobal(lua, "FreeFirstLockPet");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return ;
    }

    //依次放入三个参数
    lua_pushnumber(lua, charaindex);
    lua_pushnumber(lua, petindex);
    docall(lua, 2, 1);
}
void FreeFirstLockItem(int charaindex, int itemindex)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/firstlock.lua");

        if (lua == NULL)
        {
            return;
        }
    }

    lua_getglobal(lua, "FreeFirstLockItem");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return ;
    }

    //依次放入三个参数
    lua_pushnumber(lua, charaindex);
    lua_pushnumber(lua, itemindex);
    docall(lua, 2, 1);
}
#endif

void FreeCharExpSave(int charindex, int exp)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/charexpsave.lua");

        if (lua == NULL)
        {
            return ;
        }
    }

    lua_getglobal(lua, "FreeCharExpSave");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return ;
    }

    //依次放入三个参数
    lua_pushnumber(lua, charindex);
    lua_pushnumber(lua, exp);
    docall(lua, 2, 1);
}
#ifdef _PET_FUSION_LUA
int FreeFusionTableForBase(int MainIndex, int Subindex1, int Subindex2)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/fusion.lua");

        if (lua == NULL)
        {
            return TRUE;
        }
    }

    lua_getglobal(lua, "FreeFusionTableForBase");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return TRUE;
    }

    //依次放入三个参数
    lua_pushnumber(lua, MainIndex);
    lua_pushnumber(lua, Subindex1);
    lua_pushnumber(lua, Subindex2);

    if (lua_pcall(lua, 3, 1, 0) != 0)
    {
        printf("error running function `f': %s", lua_tostring(lua, -1));
    }

    if (!lua_isnumber(lua, -1))
    {
        printf("function `f' must return a number");
    }

    int ret = lua_tonumber(lua, -1);
    lua_pop(lua, 1);
    return ret;
}

BOOL FreeFusionSkill(int petskill)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/fusion.lua");

        if (lua == NULL)
        {
            return TRUE;
        }
    }

    lua_getglobal(lua, "FreeFusionSkill");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return TRUE;
    }

    //依次放入一个参数
    lua_pushnumber(lua, petskill);

    if (lua_pcall(lua, 1, 1, 0) != 0)
    {
        printf("error running function `f': %s", lua_tostring(lua, -1));
    }

    if (!lua_isnumber(lua, -1))
    {
        printf("function `f' must return a number");
    }

    int ret = lua_tonumber(lua, -1);
    lua_pop(lua, 1);
    return ret;
}
#endif

BOOL FreeAction(int charaindex, int x, int y, int actionno)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/action.lua");

        if (lua == NULL)
        {
            return FALSE;
        }
    }

    lua_getglobal(lua, "FreeAction");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return FALSE;
    }

    lua_pushnumber(lua, charaindex);
    lua_pushnumber(lua, x);
    lua_pushnumber(lua, y);
    lua_pushnumber(lua, actionno);
    docall(lua, 4, 1);
    return TRUE;
}

BOOL FreeModeExp(int charaindex, int getexp, int modexp)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/modeexp.lua");

        if (lua == NULL)
        {
            return TRUE;
        }
    }

    lua_getglobal(lua, "FreeModeExp");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return TRUE;
    }

    //依次放入一个参数
    lua_pushnumber(lua, charaindex);
    lua_pushnumber(lua, getexp);
    lua_pushnumber(lua, modexp);

    if (lua_pcall(lua, 3, 1, 0) != 0)
    {
        printf("error running function `f': %s", lua_tostring(lua, -1));
    }

    if (!lua_isnumber(lua, -1))
    {
        printf("function `f' must return a number");
    }

    int ret = lua_tonumber(lua, -1);
    lua_pop(lua, 1);
    return ret;
}

BOOL FreePlayerExp(int charaindex)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/playerexp.lua");

        if (lua == NULL)
        {
            return TRUE;
        }
    }

    lua_getglobal(lua, "FreePlayerExp");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return TRUE;
    }

    //依次放入一个参数
    lua_pushnumber(lua, charaindex);

    if (lua_pcall(lua, 1, 1, 0) != 0)
    {
        printf("error running function `f': %s", lua_tostring(lua, -1));
    }

    if (!lua_isnumber(lua, -1))
    {
        printf("function `f' must return a number");
    }

    int ret = lua_tonumber(lua, -1);
    lua_pop(lua, 1);
    return ret;
}

BOOL FreeSaMenu(int charaindex, int index)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/samenu.lua");

        if (lua == NULL)
        {
            return FALSE;
        }
    }

    lua_getglobal(lua, "FreeSaMenu");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return FALSE;
    }

    lua_pushnumber(lua, charaindex);
    lua_pushnumber(lua, index);
    docall(lua, 2, 1);
    return TRUE;
}

void FreeNewSaMenu(int fd, int index, char* data)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/samenu.lua");

        if (lua == NULL)
        {
            return;
        }
    }

    lua_getglobal(lua, "FreeNewSaMenu");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return;
    }

    lua_pushnumber(lua, fd);
    lua_pushnumber(lua, index);
    lua_pushstring(lua, data);
    docall(lua, 3, 1);
}

#ifdef _DATA_INFO_SHOW
BOOL FreeDataInfoShow(int charaindex)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/datainfoshow.lua");

        if (lua == NULL)
        {
            return FALSE;
        }
    }

    lua_getglobal(lua, "FreeDataInfoShow");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return FALSE;
    }

    lua_pushnumber(lua, charaindex);
    docall(lua, 1, 1);
    return TRUE;
}
#endif
#ifdef _FAME_REG_TIME
BOOL FreeFameFeatures(int charaindex, int kind, int flg)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/famefeatures.lua");

        if (lua == NULL)
        {
            return FALSE;
        }
    }

    lua_getglobal(lua, "FreeFameFeatures");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return FALSE;
    }

    //依次放入三个参数
    lua_pushnumber(lua, charaindex);
    lua_pushnumber(lua, kind);
    lua_pushnumber(lua, flg);
    docall(lua, 3, 1);
    return TRUE;
}
#endif
BOOL FreeComplianceParameter(int charaindex)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/complianceparameter.lua");

        if (lua == NULL)
        {
            return FALSE;
        }
    }

    lua_getglobal(lua, "FreeComplianceParameter");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return FALSE;
    }

    //依次放入一个参数
    lua_pushnumber(lua, charaindex);
    docall(lua, 1, 1);
    return TRUE;
}

BOOL FreeLoginCheck(int fd, char* cdkey, char* passwd, char* ip, char* mac1, char* mac2, char* mac3
#ifdef _NEWCLIENTLOGIN
                    , int logintime, char* md5buff, char* macbuff, int logintype
#endif
                   )
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/logincheck.lua");

        if (lua == NULL)
        {
            return TRUE;
        }
    }

    lua_getglobal(lua, "FreeLoginCheck");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return TRUE;
    }

    //依次放入一个参数
    lua_pushnumber(lua, fd);
    lua_pushstring(lua, cdkey);
    lua_pushstring(lua, passwd);
    lua_pushstring(lua, ip);
    lua_pushstring(lua, mac1);
    lua_pushstring(lua, mac2);
    lua_pushstring(lua, mac3);
#ifdef _NEWCLIENTLOGIN
    lua_pushnumber(lua, logintime);
    lua_pushstring(lua, md5buff);
    lua_pushstring(lua, macbuff);
    lua_pushnumber(lua, logintype);
#endif
#ifdef _NEWCLIENTLOGIN

    if (lua_pcall(lua, 11, 1, 0) != 0)
#else
    if (lua_pcall(lua, 7, 1, 0) != 0)
#endif
    {
        printf("error running function `f': %s", lua_tostring(lua, -1));
    }

    if (!lua_isnumber(lua, -1))
    {
        printf("function `f' must return a number");
    }

    int ret = lua_tonumber(lua, -1);
    lua_pop(lua, 1);
    return ret;
}

BOOL FreeCharLogout(int charaindex)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/charlogout.lua");

        if (lua == NULL)
        {
            return TRUE;
        }
    }

    lua_getglobal(lua, "FreeCharLogout");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return TRUE;
    }

    //依次放入一个参数
    lua_pushnumber(lua, charaindex);

    if (lua_pcall(lua, 1, 1, 0) != 0)
    {
        printf("error running function `f': %s", lua_tostring(lua, -1));
    }

    if (!lua_isnumber(lua, -1))
    {
        printf("function `f' must return a number");
    }

    int ret = lua_tonumber(lua, -1);
    lua_pop(lua, 1);
    return ret;
}

/*BOOL FreeCharLogout( int charaindex )
{
    static lua_State *lua;


  if (lua == NULL){
    lua = FindLua("data/ablua/charlogout.lua");
      if (lua == NULL)return FALSE;
    }

    lua_getglobal(lua, "FreeCharLogout");

    if (!lua_isfunction(lua, -1)) {
    lua_pop(lua, 1);
    return FALSE;
  }
  //依次放入一个参数
  lua_pushnumber(lua,charaindex);
    docall(lua, 1, 1);

  return TRUE;
}*/

BOOL EquipEffectFunction(int charaindex, int id)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/equipeffectfunction.lua");

        if (lua == NULL)
        {
            return TRUE;
        }
    }

    lua_getglobal(lua, "EquipEffectFunction");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return TRUE;
    }

    //依次放入二个参数
    lua_pushnumber(lua, charaindex);
    lua_pushnumber(lua, id);

    if (lua_pcall(lua, 2, 1, 0) != 0)
    {
        printf("error running function `f': %s", lua_tostring(lua, -1));
    }

    if (!lua_isnumber(lua, -1))
    {
        printf("function `f' must return a number");
    }

    int ret = lua_tonumber(lua, -1);
    lua_pop(lua, 1);
    return ret;
}

BOOL WalkFunction(int charaindex)
{
    MY_Lua *mylua = &MYLua;

    while (mylua->lua != NULL)
    {
        lua_getglobal(mylua->lua, "WalkFunction");

        if (!lua_isfunction(mylua->lua, -1))
        {
            lua_pop(mylua->lua, 1);
            mylua = mylua->next;
            continue;
        }

        lua_pushnumber(mylua->lua, charaindex);

        if (lua_pcall(mylua->lua, 1, 1, 0) != 0)
        {
            printf("error running function `f': %s", lua_tostring(mylua->lua, -1));
        }

        if (!lua_isnumber(mylua->lua, -1))
        {
            printf("function `f' must return a number");
        }

        int ret = lua_tonumber(mylua->lua, -1);
        lua_pop(mylua->lua, 1);

        if (ret == TRUE)
        {
            return TRUE;
        }

        mylua = mylua->next;
    }

    return FALSE;
}
#ifdef _ITEM_OVER_LAP
BOOL ItemOverlapFunction(int charindex, int fromitemindex, int toitemindex)
{
    MY_Lua *mylua = &MYLua;

    while (mylua->lua != NULL)
    {
        lua_getglobal(mylua->lua, "ItemOverlapFunction");

        if (!lua_isfunction(mylua->lua, -1))
        {
            lua_pop(mylua->lua, 1);
            mylua = mylua->next;
            continue;
        }

        //依次放入二个参数
        lua_pushnumber(mylua->lua, charindex);
        lua_pushnumber(mylua->lua, fromitemindex);
        lua_pushnumber(mylua->lua, toitemindex);
        lua_pcall(mylua->lua, 3, 1, 0);
        lua_isnumber(mylua->lua, -1);
        int ret = lua_tonumber(mylua->lua, -1);
        lua_pop(mylua->lua, 1);

        if (ret == 1)
        {
            mylua = mylua->next;
            continue;
        }

        return ret;
    }

    return TRUE;
}

BOOL ItemOverlapedFunction(int charindex, int fromitemindex, int fromid, int toitemindex, int toid)
{
    MY_Lua *mylua = &MYLua;

    while (mylua->lua != NULL)
    {
        lua_getglobal(mylua->lua, "ItemOverlapedFunction");

        if (!lua_isfunction(mylua->lua, -1))
        {
            lua_pop(mylua->lua, 1);
            mylua = mylua->next;
            continue;
        }

        //依次放入二个参数
        lua_pushnumber(mylua->lua, charindex);
        lua_pushnumber(mylua->lua, fromitemindex);
        lua_pushnumber(mylua->lua, fromid);
        lua_pushnumber(mylua->lua, toitemindex);
        lua_pushnumber(mylua->lua, toid);
        docall(mylua->lua, 5, 1);
        mylua = mylua->next;
    }

    return TRUE;
}
#endif

BOOL ItemToPlayerPetFunction(int charindex, int fromitemindex, int fromid, int toitemindex, char* data)
{
    MY_Lua *mylua = &MYLua;

    while (mylua->lua != NULL)
    {
        lua_getglobal(mylua->lua, "ItemToPlayerPetFunction");

        if (!lua_isfunction(mylua->lua, -1))
        {
            lua_pop(mylua->lua, 1);
            mylua = mylua->next;
            continue;
        }

        //依次放入二个参数
        lua_pushnumber(mylua->lua, charindex);
        lua_pushnumber(mylua->lua, fromitemindex);
        lua_pushnumber(mylua->lua, fromid);
        lua_pushnumber(mylua->lua, toitemindex);
        lua_pushstring(mylua->lua, data);
        docall(mylua->lua, 5, 1);
        mylua = mylua->next;
    }

    return TRUE;
}

BOOL BattleFinishFunction(int charaindex, int battletime, int battleturn, int battletype, char* wincdkey, char* winname, char* lostcdkey, char* lostname)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/battlefinishfunction.lua");

        if (lua == NULL)
        {
            return TRUE;
        }
    }

    lua_getglobal(lua, "BattleFinishFunction");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return TRUE;
    }

    //依次放入二个参数
    lua_pushnumber(lua, charaindex);
    lua_pushnumber(lua, battletime);
    lua_pushnumber(lua, battleturn);
    lua_pushnumber(lua, battletype);
    lua_pushstring(lua, wincdkey);
    lua_pushstring(lua, winname);
    lua_pushstring(lua, lostcdkey);
    lua_pushstring(lua, lostname);
    docall(lua, 8, 1);
    return TRUE;
}

BOOL SetBattleEnmeyFunction(int meindex, int enemyindex, int id)
{
    lua_State *lua = CHAR_getLUAFunction(meindex, CHAR_BATTLESETFUNC);

    if (lua == NULL)
    {
        return FALSE;
    }

    //依次放入二个参数
    lua_pushnumber(lua, meindex);
    lua_pushnumber(lua, enemyindex);
    lua_pushnumber(lua, id);
    docall(lua, 3, 1);
    return TRUE;
}

#endif

#ifdef _ALLBLUES_LUA_1_8
BOOL CaptureOkFunction(int attackindex, int defindex)
{
    MY_Lua *mylua = &MYLua;

    while (mylua->lua != NULL)
    {
        lua_getglobal(mylua->lua, "CaptureOkFunction");

        if (!lua_isfunction(mylua->lua, -1))
        {
            lua_pop(mylua->lua, 1);
            mylua = mylua->next;
            continue;
        }

        //依次放入二个参数
        lua_pushnumber(mylua->lua, attackindex);
        lua_pushnumber(mylua->lua, defindex);
        docall(mylua->lua, 2, 1);
        mylua = mylua->next;
    }

    return TRUE;
}

BOOL CaptureCheckFunction(int attackindex, int defindex)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/capturefunction.lua");

        if (lua == NULL)
        {
            return TRUE;
        }
    }

    lua_getglobal(lua, "CaptureCheckFunction");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return TRUE;
    }

    //依次放入二个参数
    lua_pushnumber(lua, attackindex);
    lua_pushnumber(lua, defindex);

    if (lua_pcall(lua, 2, 1, 0) != 0)
    {
        printf("error running function `f': %s", lua_tostring(lua, -1));
    }

    if (!lua_isnumber(lua, -1))
    {
        printf("function `f' must return a number");
    }

    int ret = lua_tonumber(lua, -1);
    lua_pop(lua, 1);
    return ret;
}
#endif

#ifdef _ALLBLUES_LUA_1_7
BOOL CharVsEnemyFunction(int charaindex)
{
    MY_Lua *mylua = &MYLua;

    while (mylua->lua != NULL)
    {
        lua_getglobal(mylua->lua, "CharVsEnemyFunction");

        if (!lua_isfunction(mylua->lua, -1))
        {
            lua_pop(mylua->lua, 1);
            mylua = mylua->next;
            continue;
        }

        lua_pushnumber(mylua->lua, charaindex);

        if (lua_pcall(mylua->lua, 1, 1, 0) != 0)
        {
            printf("error running function `f': %s", lua_tostring(mylua->lua, -1));
        }

        if (!lua_isnumber(mylua->lua, -1))
        {
            printf("function `f' must return a number");
        }

        int ret = lua_tonumber(mylua->lua, -1);
        lua_pop(mylua->lua, 1);

        if (ret == TRUE)
        {
            return TRUE;
        }

        mylua = mylua->next;
    }

    return FALSE;
}
#endif

#ifdef _ALLBLUES_LUA_1_6
BOOL CharTalkFunction(int charaindex, char *message, int color
#ifdef _ITEM_PET_TALK_
                      , char* outitempetData
#endif
#ifdef  _PLAYER_UID
                      , char* uid
#endif
#ifdef _YAYA_VOICE
                      , int volumeLen
#endif
                     )
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/chartalkfunction.lua");

        if (lua == NULL)
        {
            return FALSE;
        }
    }

    lua_getglobal(lua, "CharTalkFunction");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return FALSE;
    }

    //依次放入二个参数
    lua_pushnumber(lua, charaindex);
    lua_pushstring(lua, message);
    lua_pushnumber(lua, color);
#ifdef _ITEM_PET_TALK_
    lua_pushstring(lua, outitempetData);
#endif
#ifdef  _PLAYER_UID
    lua_pushstring(lua, uid);
#endif
#ifdef _YAYA_VOICE
    lua_pushnumber(lua, volumeLen);
#endif
#ifdef _ITEM_PET_TALK_
#ifdef _PLAYER_UID
#ifdef _YAYA_VOICE

    if (lua_pcall(lua, 6, 1, 0) != 0)
#else
    if (lua_pcall(lua, 5, 1, 0) != 0)
#endif
#else
#ifdef _YAYA_VOICE
    if (lua_pcall(lua, 5, 1, 0) != 0)
#else
    if (lua_pcall(lua, 4, 1, 0) != 0)
#endif
#endif
#else
#ifdef  _PLAYER_UID
    if (lua_pcall(lua, 4, 1, 0) != 0)
#else
    if (lua_pcall(lua, 3, 1, 0) != 0)
#endif
#endif
    {
        printf("error running function `f': %s", lua_tostring(lua, -1));
    }

    if (!lua_isnumber(lua, -1))
    {
        printf("function `f' must return a number");
    }

    int ret = lua_tonumber(lua, -1);
    lua_pop(lua, 1);
    return ret;
}

BOOL CharTalkFunction2(int charaindex, char *message, int color)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/chartalkfunction.lua");

        if (lua == NULL)
        {
            return FALSE;
        }
    }

    lua_getglobal(lua, "CharTalkFunction2");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return FALSE;
    }

    //依次放入二个参数
    lua_pushnumber(lua, charaindex);
    lua_pushstring(lua, message);
    lua_pushnumber(lua, color);

    if (lua_pcall(lua, 3, 1, 0) != 0)
    {
        printf("error running function `f': %s", lua_tostring(lua, -1));
    }

    if (!lua_isnumber(lua, -1))
    {
        printf("function `f' must return a number");
    }

    int ret = lua_tonumber(lua, -1);
    lua_pop(lua, 1);
    return ret;
}

BOOL CharTalkFunction3(int charaindex, char *message, int color)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/chartalkfunction.lua");

        if (lua == NULL)
        {
            return FALSE;
        }
    }

    lua_getglobal(lua, "CharTalkFunction3");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return FALSE;
    }

    //依次放入二个参数
    lua_pushnumber(lua, charaindex);
    lua_pushstring(lua, message);
    lua_pushnumber(lua, color);

    if (lua_pcall(lua, 3, 1, 0) != 0)
    {
        printf("error running function `f': %s", lua_tostring(lua, -1));
    }

    if (!lua_isnumber(lua, -1))
    {
        printf("function `f' must return a number");
    }

    int ret = lua_tonumber(lua, -1);
    lua_pop(lua, 1);
    return ret;
}

BOOL CharTellFunction(int charaindex, char *message, int color, char* uid, char* petitemstr)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/chartalkfunction.lua");

        if (lua == NULL)
        {
            return FALSE;
        }
    }

    lua_getglobal(lua, "CharTellFunction");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return FALSE;
    }

    //依次放入二个参数
    lua_pushnumber(lua, charaindex);
    lua_pushstring(lua, message);
    lua_pushnumber(lua, color);
    lua_pushstring(lua, uid);
    lua_pushstring(lua, petitemstr);

    if (lua_pcall(lua, 5, 1, 0) != 0)
    {
        printf("error running function `f': %s", lua_tostring(lua, -1));
    }

    if (!lua_isnumber(lua, -1))
    {
        printf("function `f' must return a number");
    }

    int ret = lua_tonumber(lua, -1);
    lua_pop(lua, 1);
    return ret;
}

BOOL BattleWatch(int charaindex, int toindex)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/battlewatch.lua");

        if (lua == NULL)
        {
            return FALSE;
        }
    }

    lua_getglobal(lua, "BattleWatch");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return FALSE;
    }

    //依次放入二个参数
    lua_pushnumber(lua, charaindex);
    lua_pushnumber(lua, toindex);

    if (lua_pcall(lua, 2, 1, 0) != 0)
    {
        printf("error running function `f': %s", lua_tostring(lua, -1));
    }

    if (!lua_isnumber(lua, -1))
    {
        printf("function `f' must return a number");
    }

    int ret = lua_tonumber(lua, -1);
    lua_pop(lua, 1);
    return ret;
}

BOOL FamilyRideFunction(int meindex, int petindex, int petid)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/familyridefunction.lua");

        if (lua == NULL)
        {
            return FALSE;
        }
    }

    lua_getglobal(lua, "FamilyRideFunction");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return FALSE;
    }

    //依次放入二个参数
    lua_pushnumber(lua, meindex);
    lua_pushnumber(lua, petindex);
    lua_pushnumber(lua, petid);

    if (lua_pcall(lua, 3, 1, 0) != 0)
    {
        printf("error running function `f': %s", lua_tostring(lua, -1));
    }

    if (!lua_isnumber(lua, -1))
    {
        printf("function `f' must return a number");
    }

    int ret = lua_tonumber(lua, -1);
    lua_pop(lua, 1);
    return ret;
}
#endif

#ifdef _ALLBLUES_LUA_1_5
BOOL NetLoopFunction(void)
{
    MY_Lua *mylua = &MYLua;

    while (mylua->lua != NULL)
    {
        lua_getglobal(mylua->lua, "NetLoopFunction");

        if (!lua_isfunction(mylua->lua, -1))
        {
            lua_pop(mylua->lua, 1);
            mylua = mylua->next;
            continue;
        }

        docall(mylua->lua, 0, 1);
        mylua = mylua->next;
    }

    return TRUE;
}

BOOL FreeCharCreate(int charaindex, int hometown, int clifd)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/charcreate.lua");

        if (lua == NULL)
        {
            return FALSE;
        }
    }

    lua_getglobal(lua, "FreeCharCreate");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return FALSE;
    }

    //依次放入二个参数
    lua_pushnumber(lua, charaindex);
    lua_pushnumber(lua, hometown);
    lua_pushnumber(lua, clifd);
    docall(lua, 3, 1);
    return TRUE;
}

BOOL FreeCharLogin(int charaindex)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/charlogin.lua");

        if (lua == NULL)
        {
            return FALSE;
        }
    }

    lua_getglobal(lua, "FreeCharLogin");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return FALSE;
    }

    //依次放入二个参数
    lua_pushnumber(lua, charaindex);
    docall(lua, 1, 1);
    return TRUE;
}

BOOL FreeVsPlayer(int charaindex, int toindex)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/vsplayer.lua");

        if (lua == NULL)
        {
            return FALSE;
        }
    }

    lua_getglobal(lua, "FreeVsPlayer");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return FALSE;
    }

    //依次放入二个参数
    lua_pushnumber(lua, charaindex);
    lua_pushnumber(lua, toindex);

    if (lua_pcall(lua, 2, 1, 0) != 0)
    {
        printf("error running function `f': %s", lua_tostring(lua, -1));
    }

    if (!lua_isnumber(lua, -1))
    {
        printf("function `f' must return a number");
    }

    int ret = lua_tonumber(lua, -1);
    lua_pop(lua, 1);
    return ret;
}

BOOL FreePartyJoin(int charaindex, int toindex)
{
    MY_Lua *mylua = &MYLua;

    while (mylua->lua != NULL)
    {
        lua_getglobal(mylua->lua, "FreePartyJoin");

        if (!lua_isfunction(mylua->lua, -1))
        {
            lua_pop(mylua->lua, 1);
            mylua = mylua->next;
            continue;
        }

        //依次放入二个参数
        lua_pushnumber(mylua->lua, charaindex);
        lua_pushnumber(mylua->lua, toindex);

        if (lua_pcall(mylua->lua, 2, 1, 0) != 0)
        {
            printf("error running function `f': %s", lua_tostring(mylua->lua, -1));
        }

        if (!lua_isnumber(mylua->lua, -1))
        {
            printf("function `f' must return a number");
        }

        int ret = lua_tonumber(mylua->lua, -1);
        lua_pop(mylua->lua, 1);

        if (ret == FALSE)
        {
            return FALSE;
        }

        mylua = mylua->next;
    }

    return TRUE;
}
#endif

#ifdef _ALLBLUES_LUA_1_4

BOOL RunCharLogOutEvent(int charaindex)
{
    lua_State *lua = CHAR_getLUAFunction(charaindex, CHAR_LOGINOUTFUNC);

    if (lua == NULL)
    {
        return FALSE;
    }

    //依次放入二个参数
    lua_pushnumber(lua, charaindex);
    docall(lua, 1, 1);
    return TRUE;
}

BOOL BattleFinish(int battleindex, int charaindex)
{
    lua_State *lua = BATTLE_getLUAFunction(battleindex, BATTLE_FINISH);

    if (lua == NULL)
    {
        return FALSE;
    }

    //依次放入二个参数
    lua_pushnumber(lua, charaindex);
    docall(lua, 1, 1);
    return TRUE;
}

BOOL BattleEscape(int battleindex, int charaindex)
{
    lua_State *lua = BATTLE_getLUAFunction(battleindex, BATTLE_ESCAPE);

    if (lua == NULL)
    {
        return FALSE;
    }

    //依次放入二个参数
    lua_pushnumber(lua, charaindex);
    docall(lua, 1, 1);
    return TRUE;
}
#endif

#ifdef _OFFLINE_SYSTEM
BOOL OffLineCommand(int battleindex, int charaindex, int side)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/offlinecommand.lua");

        if (lua == NULL)
        {
            return FALSE;
        }
    }

    lua_getglobal(lua, "OffLineCommand");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return TRUE;
    }

    //依次放入三个参数
    lua_pushnumber(lua, battleindex);
    lua_pushnumber(lua, charaindex);
    lua_pushnumber(lua, side);
    docall(lua, 3, 1);
    return TRUE;
}
#endif

BOOL FreeBattleCommand(int battleindex, int charaindex, int side)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/battlecommand.lua");

        if (lua == NULL)
        {
            return FALSE;
        }
    }

    lua_getglobal(lua, "FreeBattleCommand");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return TRUE;
    }

    //依次放入三个参数
    lua_pushnumber(lua, battleindex);
    lua_pushnumber(lua, charaindex);
    lua_pushnumber(lua, side);
    docall(lua, 3, 1);
    return TRUE;
}

#ifdef _PETSKILL_SHOP_LUA
BOOL FreePetSkillShop(int talkerindex, int petindex, int oldSkillID, int newSkillID)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/petskillshop.lua");

        if (lua == NULL)
        {
            return TRUE;
        }
    }

    lua_getglobal(lua, "FreePetSkillShop");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return TRUE;
    }

    //依次放入三个参数
    lua_pushnumber(lua, talkerindex);
    lua_pushnumber(lua, petindex);
    lua_pushnumber(lua, oldSkillID);
    lua_pushnumber(lua, newSkillID);

    if (lua_pcall(lua, 4, 1, 0) != 0)
    {
        printf("error running function `f': %s", lua_tostring(lua, -1));
    }

    if (!lua_isnumber(lua, -1))
    {
        printf("function `f' must return a number");
    }

    int ret = lua_tonumber(lua, -1);
    lua_pop(lua, 1);
    return ret;
}
#endif

BOOL FreeWarpMap(int charaindex, int floorid, int x, int y)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/warpmap.lua");

        if (lua == NULL)
        {
            return TRUE;
        }
    }

    lua_getglobal(lua, "FreeWarpMap");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return TRUE;
    }

    //依次放入一个参数
    lua_pushnumber(lua, charaindex);
    lua_pushnumber(lua, floorid);
    lua_pushnumber(lua, x);
    lua_pushnumber(lua, y);

    if (lua_pcall(lua, 4, 1, 0) != 0)
    {
        printf("error running function `f': %s", lua_tostring(lua, -1));
    }

    if (!lua_isnumber(lua, -1))
    {
        printf("function `f' must return a number");
    }

    int ret = lua_tonumber(lua, -1);
    lua_pop(lua, 1);
    return ret;
}

BOOL FreeDropItem(int charaindex, int itemindex, int num)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/dropitem.lua");

        if (lua == NULL)
        {
            return TRUE;
        }
    }

    lua_getglobal(lua, "FreeDropItem");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return TRUE;
    }

    //依次放入一个参数
    lua_pushnumber(lua, charaindex);
    lua_pushnumber(lua, itemindex);
    lua_pushnumber(lua, num);

    if (lua_pcall(lua, 3, 1, 0) != 0)
    {
        printf("error running function `f': %s", lua_tostring(lua, -1));
    }

    if (!lua_isnumber(lua, -1))
    {
        printf("function `f' must return a number");
    }

    int ret = lua_tonumber(lua, -1);
    lua_pop(lua, 1);
    return ret;
}

BOOL FreeDropPet(int charaindex, int havepetindex)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/droppet.lua");

        if (lua == NULL)
        {
            return TRUE;
        }
    }

    lua_getglobal(lua, "FreeDropPet");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return TRUE;
    }

    //依次放入一个参数
    lua_pushnumber(lua, charaindex);
    lua_pushnumber(lua, havepetindex);

    if (lua_pcall(lua, 2, 1, 0) != 0)
    {
        printf("error running function `f': %s", lua_tostring(lua, -1));
    }

    if (!lua_isnumber(lua, -1))
    {
        printf("function `f' must return a number");
    }

    int ret = lua_tonumber(lua, -1);
    lua_pop(lua, 1);
    return ret;
}

BOOL FreeDropPetFollow(int charaindex, int havepetindex)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/droppetfollow.lua");

        if (lua == NULL)
        {
            return TRUE;
        }
    }

    lua_getglobal(lua, "FreeDropPetFollow");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return TRUE;
    }

    //依次放入一个参数
    lua_pushnumber(lua, charaindex);
    lua_pushnumber(lua, havepetindex);

    if (lua_pcall(lua, 2, 1, 0) != 0)
    {
        printf("error running function `f': %s", lua_tostring(lua, -1));
    }

    if (!lua_isnumber(lua, -1))
    {
        printf("function `f' must return a number");
    }

    int ret = lua_tonumber(lua, -1);
    lua_pop(lua, 1);
    return ret;
}

BOOL FreeBattlePer(int charaindex, int iultimate)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/battlefly.lua");

        if (lua == NULL)
        {
            return TRUE;
        }
    }

    lua_getglobal(lua, "FreeBattlePer");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return TRUE;
    }

    //依次放入一个参数
    lua_pushnumber(lua, charaindex);
    lua_pushnumber(lua, iultimate);

    if (lua_pcall(lua, 2, 1, 0) != 0)
    {
        printf("error running function `f': %s", lua_tostring(lua, -1));
    }

    if (!lua_isnumber(lua, -1))
    {
        printf("function `f' must return a number");
    }

    int ret = lua_tonumber(lua, -1);
    lua_pop(lua, 1);
    return ret;
}

int FreeBattleDuck(int attackindex, int defindex, int per)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/battleduck.lua");

        if (lua == NULL)
        {
            return per;
        }
    }

    lua_getglobal(lua, "FreeBattleDuck");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return per;
    }

    //依次放入一个参数
    lua_pushnumber(lua, attackindex);
    lua_pushnumber(lua, defindex);
    lua_pushnumber(lua, per);

    if (lua_pcall(lua, 3, 1, 0) != 0)
    {
        printf("error running function `f': %s", lua_tostring(lua, -1));
    }

    if (!lua_isnumber(lua, -1))
    {
        printf("function `f' must return a number");
    }

    int ret = lua_tonumber(lua, -1);
    lua_pop(lua, 1);
    return ret;
}

int BattleDamage(int charaindex, int defindex, int damage)
{
    MY_Lua *mylua = &MYLua;

    while (mylua->lua != NULL)
    {
        lua_getglobal(mylua->lua, "BattleDamage");

        if (!lua_isfunction(mylua->lua, -1))
        {
            lua_pop(mylua->lua, 1);
            mylua = mylua->next;
            continue;
        }

        lua_pushnumber(mylua->lua, charaindex);
        lua_pushnumber(mylua->lua, defindex);
        lua_pushnumber(mylua->lua, damage);

        if (lua_pcall(mylua->lua, 3, 1, 0) != 0)
        {
            printf("error running function `f': %s", lua_tostring(mylua->lua, -1));
        }

        if (!lua_isnumber(mylua->lua, -1))
        {
            printf("function `f' must return a number");
        }

        int ret = lua_tonumber(mylua->lua, -1);
        lua_pop(mylua->lua, 1);
        damage = ret;
        mylua = mylua->next;
    }

    return damage;
}


BOOL BattleJoin(int charaindex, int battleindex)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/battlejoin.lua");

        if (lua == NULL)
        {
            return TRUE;
        }
    }

    lua_getglobal(lua, "FreeBattleJoin");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return TRUE;
    }

    //依次放入一个参数
    lua_pushnumber(lua, charaindex);
    lua_pushnumber(lua, battleindex);

    if (lua_pcall(lua, 2, 1, 0) != 0)
    {
        printf("error running function `f': %s", lua_tostring(lua, -1));
    }

    if (!lua_isnumber(lua, -1))
    {
        printf("function `f' must return a number");
    }

    int ret = lua_tonumber(lua, -1);
    lua_pop(lua, 1);
    return ret;
}


BOOL BattleFly(int charaindex)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/battlefly.lua");

        if (lua == NULL)
        {
            return TRUE;
        }
    }

    lua_getglobal(lua, "FreeBattleFly");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return TRUE;
    }

    //依次放入一个参数
    lua_pushnumber(lua, charaindex);

    if (lua_pcall(lua, 1, 1, 0) != 0)
    {
        printf("error running function `f': %s", lua_tostring(lua, -1));
    }

    if (!lua_isnumber(lua, -1))
    {
        printf("function `f' must return a number");
    }

    int ret = lua_tonumber(lua, -1);
    lua_pop(lua, 1);
    return ret;
}

BOOL FreeBattleEscape(int charaindex)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/battleescape.lua");

        if (lua == NULL)
        {
            return TRUE;
        }
    }

    lua_getglobal(lua, "FreeBattleEscape");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return TRUE;
    }

    //依次放入一个参数
    lua_pushnumber(lua, charaindex);

    if (lua_pcall(lua, 1, 1, 0) != 0)
    {
        printf("error running function `f': %s", lua_tostring(lua, -1));
    }

    if (!lua_isnumber(lua, -1))
    {
        printf("function `f' must return a number");
    }

    int ret = lua_tonumber(lua, -1);
    lua_pop(lua, 1);
    return ret;
}

#ifdef _RIDEFLG_
BOOL FamilyRideCheck(int meindex, int petindex, int petid)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/familyridefunction.lua");

        if (lua == NULL)
        {
            return FALSE;
        }
    }

    lua_getglobal(lua, "FamilyRideCheck");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return FALSE;
    }

    //依次放入二个参数
    lua_pushnumber(lua, meindex);
    lua_pushnumber(lua, petindex);
    lua_pushnumber(lua, petid);

    if (lua_pcall(lua, 3, 1, 0) != 0)
    {
        printf("error running function `f': %s", lua_tostring(lua, -1));
    }

    if (!lua_isnumber(lua, -1))
    {
        printf("function `f' must return a number");
    }

    int ret = lua_tonumber(lua, -1);
    lua_pop(lua, 1);
    return ret;
}
#endif

BOOL FreeRideQuery(int charaindex)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/familyridefunction.lua");

        if (lua == NULL)
        {
            return TRUE;
        }
    }

    lua_getglobal(lua, "FreeRideQuery");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return TRUE;
    }

    //依次放入二个参数
    lua_pushnumber(lua, charaindex);

    if (lua_pcall(lua, 1, 1, 0) != 0)
    {
        printf("error running function `f': %s", lua_tostring(lua, -1));
    }

    if (!lua_isnumber(lua, -1))
    {
        printf("function `f' must return a number");
    }

    int ret = lua_tonumber(lua, -1);
    lua_pop(lua, 1);
    return ret;
}

#ifdef _ALLBLUES_LUA_1_2
BOOL RunUseChatMagic(int charaindex, char *data, lua_State *lua)
{
    if (lua == NULL)
    {
        return FALSE;
    }

    //依次放入二个参数
    lua_pushnumber(lua, charaindex);
    lua_pushstring(lua, data);
    docall(lua, 2, 1);
    return TRUE;
}


BOOL RunItemUseEvent(int itemindex, int charaindex, int toindex, int haveitemindex)
{
    lua_State *lua = ITEM_getLUAFunction(itemindex, ITEM_USEFUNC);

    if (lua == NULL)
    {
        return FALSE;
    }

    //依次放入四个参数
    lua_pushnumber(lua, itemindex);
    lua_pushnumber(lua, charaindex);
    lua_pushnumber(lua, toindex);
    lua_pushnumber(lua, haveitemindex);
    docall(lua, 4, 1);
    return TRUE;
}

BOOL RunItemDieReLifeEvent(int charaindex, int itemindex, int haveitemindex)
{
    lua_State *lua = ITEM_getLUAFunction(itemindex, ITEM_DIERELIFEFUNC);

    if (lua == NULL)
    {
        return FALSE;
    }

    //依次放入三个参数
    lua_pushnumber(lua, charaindex);
    lua_pushnumber(lua, itemindex);
    lua_pushnumber(lua, haveitemindex);
    docall(lua, 3, 1);
    return TRUE;
}

BOOL RunItemDetachEvent(int charaindex, int itemindex)
{
    lua_State *lua = ITEM_getLUAFunction(itemindex, ITEM_DETACHFUNC);

    if (lua == NULL)
    {
        return FALSE;
    }

    //依次放入二个参数
    lua_pushnumber(lua, charaindex);
    lua_pushnumber(lua, itemindex);
    docall(lua, 2, 1);
    return TRUE;
}

BOOL RunItemAttachEvent(int charaindex, int itemindex)
{
    lua_State *lua = ITEM_getLUAFunction(itemindex, ITEM_ATTACHFUNC);

    if (lua == NULL)
    {
        return FALSE;
    }

    //依次放入二个参数
    lua_pushnumber(lua, charaindex);
    lua_pushnumber(lua, itemindex);
    docall(lua, 2, 1);
    return TRUE;
}

BOOL RunItemPickupEvent(int charaindex, int itemindex)
{
    lua_State *lua = ITEM_getLUAFunction(itemindex, ITEM_PICKUPFUNC);

    if (lua == NULL)
    {
        return FALSE;
    }

    //依次放入二个参数
    lua_pushnumber(lua, charaindex);
    lua_pushnumber(lua, itemindex);
    docall(lua, 2, 1);
    return TRUE;
}

BOOL RunItemPostOverEvent(int itemindex, int charaindex)
{
    lua_State *lua = ITEM_getLUAFunction(itemindex, ITEM_POSTOVERFUNC);

    if (lua == NULL)
    {
        return FALSE;
    }

    //依次放入二个参数
    lua_pushnumber(lua, charaindex);
    lua_pushnumber(lua, itemindex);
    docall(lua, 2, 1);
    return TRUE;
}

BOOL RunItemPreOverEvent(int itemindex, int charaindex)
{
    lua_State *lua = ITEM_getLUAFunction(itemindex, ITEM_PREOVERFUNC);

    if (lua == NULL)
    {
        return FALSE;
    }

    //依次放入二个参数
    lua_pushnumber(lua, charaindex);
    lua_pushnumber(lua, itemindex);
    docall(lua, 2, 1);
    return TRUE;
}

BOOL RunItemDropEvent(int charaindex, int itemindex)
{
    lua_State *lua = ITEM_getLUAFunction(itemindex, ITEM_DROPFUNC);

    if (lua == NULL)
    {
        return FALSE;
    }

    //依次放入二个参数
    lua_pushnumber(lua, charaindex);
    lua_pushnumber(lua, itemindex);
    docall(lua, 2, 1);
    return TRUE;
}

#endif


BOOL RunCharTalkedEvent(int meindex, int toindex, char *messageeraseescape, int color, int channel)
{
    lua_State *lua = CHAR_getLUAFunction(meindex, CHAR_TALKEDFUNC);

    if (lua == NULL)
    {
        return FALSE;
    }

    //依次放入五个参数
    lua_pushnumber(lua, meindex);
    lua_pushnumber(lua, toindex);
    lua_pushstring(lua, messageeraseescape);
    lua_pushnumber(lua, color);
    lua_pushnumber(lua, channel);
    docall(lua, 5, 1);
    return TRUE;
}


BOOL RunCharLoopEvent(int meindex)
{
    lua_State *lua = CHAR_getLUAFunction(meindex, CHAR_LOOPFUNC);

    if (lua == NULL)
    {
        return FALSE;
    }

    //依次放入一个参数
    lua_pushnumber(lua, meindex);
    docall(lua, 1, 1);
    return TRUE;
}

BOOL RunCharOverlapEvent(int meindex, int toindex)
{
    lua_State *lua = CHAR_getLUAFunction(meindex, CHAR_OVERLAPEDFUNC);

    if (lua == NULL)
    {
        return FALSE;
    }

    //依次放入二个参数
    lua_pushnumber(lua, meindex);
    lua_pushnumber(lua, toindex);
    docall(lua, 2, 1);
    return TRUE;
}

BOOL RunCharBattleOverEvent(int meindex, int battleindex, int iswin)
{
    lua_State *lua = CHAR_getLUAFunction(meindex, CHAR_BATTLEOVERDFUNC);

    if (lua == NULL)
    {
        return FALSE;
    }

    //依次放入三个参数
    lua_pushnumber(lua, meindex);
    lua_pushnumber(lua, battleindex);
    lua_pushnumber(lua, iswin);
    docall(lua, 3, 1);
    return TRUE;
}

BOOL RunCharWindowTalked(int meindex, int talkerindex, int seqno, int select, char *data)
{
    lua_State *lua = CHAR_getLUAFunction(meindex, CHAR_WINDOWTALKEDFUNC);

    if (lua == NULL)
    {
        return FALSE;
    }

    //依次放入五个参数
    lua_pushnumber(lua, meindex);
    lua_pushnumber(lua, talkerindex);
    lua_pushnumber(lua, seqno);
    lua_pushnumber(lua, select);
    lua_pushstring(lua, data);
    docall(lua, 5, 1);
    return TRUE;
}

#endif

#ifdef _RED_MEMOY_
BOOL RedMoneyFunction(int charaindex, char* data)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/redmoneyfunction.lua");

        if (lua == NULL)
        {
            return TRUE;
        }
    }

    lua_getglobal(lua, "RedMoneyFunction");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return TRUE;
    }

    //依次放入二个参数
    lua_pushnumber(lua, charaindex);
    lua_pushstring(lua, data);
    docall(lua, 2, 1);
    return TRUE;
}
#endif

#ifdef _FAMILYBADGE_
BOOL LoadFamilyBadge(int fd)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/other/familybadge/familybadge.lua");

        if (lua == NULL)
        {
            return FALSE;
        }
    }

    lua_getglobal(lua, "LoadFamilyBadge");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return FALSE;
    }

    lua_pushnumber(lua, fd);
    docall(lua, 1, 1);
    return TRUE;
}
#endif

BOOL FreePlayerTrans(int charaindex)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/playertrans.lua");

        if (lua == NULL)
        {
            return TRUE;
        }
    }

    lua_getglobal(lua, "FreePlayerTrans");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return TRUE;
    }

    //依次放入二个参数
    lua_pushnumber(lua, charaindex);

    if (lua_pcall(lua, 1, 1, 0) != 0)
    {
        printf("error running function `f': %s", lua_tostring(lua, -1));
    }

    if (!lua_isnumber(lua, -1))
    {
        printf("function `f' must return a number");
    }

    int ret = lua_tonumber(lua, -1);
    lua_pop(lua, 1);
    return ret;
}

BOOL FreePlayerTransFlgCls(int charaindex)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/playertrans.lua");

        if (lua == NULL)
        {
            return FALSE;
        }
    }

    lua_getglobal(lua, "FreePlayerTransFlgCls");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return FALSE;
    }

    //依次放入二个参数
    lua_pushnumber(lua, charaindex);

    if (lua_pcall(lua, 1, 1, 0) != 0)
    {
        printf("error running function `f': %s", lua_tostring(lua, -1));
    }

    if (!lua_isnumber(lua, -1))
    {
        printf("function `f' must return a number");
    }

    int ret = lua_tonumber(lua, -1);
    lua_pop(lua, 1);
    return ret;
}

#ifdef  _DAMMAGE_CALC
int FreeBattleDamage(int attackindex, int defindex, int damage)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/battledamage.lua");

        if (lua == NULL)
        {
            return damage;
        }
    }

    lua_getglobal(lua, "FreeBattleDamage");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return damage;
    }

    //依次放入二个参数
    lua_pushnumber(lua, attackindex);
    lua_pushnumber(lua, defindex);
    lua_pushnumber(lua, damage);

    if (lua_pcall(lua, 3, 1, 0) != 0)
    {
        printf("error running function `f': %s", lua_tostring(lua, -1));
    }

    if (!lua_isnumber(lua, -1))
    {
        printf("function `f' must return a number");
    }

    int ret = lua_tonumber(lua, -1);
    lua_pop(lua, 1);
    return ret;
}
#endif

int FreeBattleCounter(int attackindex, int defindex, int counter)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/battlecounter.lua");

        if (lua == NULL)
        {
            return counter;
        }
    }

    lua_getglobal(lua, "FreeBattleCounter");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return counter;
    }

    //依次放入二个参数
    lua_pushnumber(lua, attackindex);
    lua_pushnumber(lua, defindex);
    lua_pushnumber(lua, counter);

    if (lua_pcall(lua, 3, 1, 0) != 0)
    {
        printf("error running function `f': %s", lua_tostring(lua, -1));
    }

    if (!lua_isnumber(lua, -1))
    {
        printf("function `f' must return a number");
    }

    int ret = lua_tonumber(lua, -1);
    lua_pop(lua, 1);
    return ret;
}

int FreeBattleCritical(int attackindex, int defindex, int critical)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/battlecritical.lua");

        if (lua == NULL)
        {
            return critical;
        }
    }

    lua_getglobal(lua, "FreeBattleCritical");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return critical;
    }

    //依次放入二个参数
    lua_pushnumber(lua, attackindex);
    lua_pushnumber(lua, defindex);
    lua_pushnumber(lua, critical);

    if (lua_pcall(lua, 3, 1, 0) != 0)
    {
        printf("error running function `f': %s", lua_tostring(lua, -1));
    }

    if (!lua_isnumber(lua, -1))
    {
        printf("function `f' must return a number");
    }

    int ret = lua_tonumber(lua, -1);
    lua_pop(lua, 1);
    return ret;
}

#ifdef  _FMBATTLE_POINT
void FmBattlePointFunction(int meid, int flg)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/fmbattlepoint.lua");

        if (lua == NULL)
        {
            return;
        }
    }

    lua_getglobal(lua, "FmBattlePoint");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return;
    }

    //依次放入二个参数
    lua_pushnumber(lua, meid);
    lua_pushnumber(lua, flg);
    docall(lua, 2, 1);
    return;
}
#endif

int FreeCharLock(int charaindex)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/charlock.lua");

        if (lua == NULL)
        {
            return TRUE;
        }
    }

    lua_getglobal(lua, "FreeCharLock");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return TRUE;
    }

    //依次放入三个参数
    lua_pushnumber(lua, charaindex);

    if (lua_pcall(lua, 1, 1, 0) != 0)
    {
        printf("error running function `f': %s", lua_tostring(lua, -1));
    }

    if (!lua_isnumber(lua, -1))
    {
        printf("function `f' must return a number");
    }

    int ret = lua_tonumber(lua, -1);
    lua_pop(lua, 1);
    return ret;
}

int FreeStayEncount(int charaindex)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/stayencount.lua");

        if (lua == NULL)
        {
            return TRUE;
        }
    }

    lua_getglobal(lua, "FreeStayEncount");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return TRUE;
    }

    //依次放入三个参数
    lua_pushnumber(lua, charaindex);

    if (lua_pcall(lua, 1, 1, 0) != 0)
    {
        printf("error running function `f': %s", lua_tostring(lua, -1));
    }

    if (!lua_isnumber(lua, -1))
    {
        printf("function `f' must return a number");
    }

    int ret = lua_tonumber(lua, -1);
    lua_pop(lua, 1);
    return ret;
}

int FreeGetFame(int charaindex, int famepoint, int level)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/getfame.lua");

        if (lua == NULL)
        {
            return famepoint;
        }
    }

    lua_getglobal(lua, "FreeGetFame");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return famepoint;
    }

    //依次放入三个参数
    lua_pushnumber(lua, charaindex);
    lua_pushnumber(lua, famepoint);
    lua_pushnumber(lua, level);

    if (lua_pcall(lua, 3, 1, 0) != 0)
    {
        printf("error running function `f': %s", lua_tostring(lua, -1));
    }

    if (!lua_isnumber(lua, -1))
    {
        printf("function `f' must return a number");
    }

    int ret = lua_tonumber(lua, -1);
    lua_pop(lua, 1);
    return ret;
}

int FreePetMail(int charaindex, int petindex)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/petmail.lua");

        if (lua == NULL)
        {
            return TRUE;
        }
    }

    lua_getglobal(lua, "FreePetMail");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return TRUE;
    }

    //依次放入三个参数
    lua_pushnumber(lua, charaindex);
    lua_pushnumber(lua, petindex);

    if (lua_pcall(lua, 2, 1, 0) != 0)
    {
        printf("error running function `f': %s", lua_tostring(lua, -1));
    }

    if (!lua_isnumber(lua, -1))
    {
        printf("function `f' must return a number");
    }

    int ret = lua_tonumber(lua, -1);
    lua_pop(lua, 1);
    return ret;
}

#ifdef _MO_LUA_GOLRYCALLBACK
BOOL GloryCallBack(int charaindex, int point)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/glorycallback.lua");

        if (lua == NULL)
        {
            return FALSE;
        }
    }

    lua_getglobal(lua, "GloryCallBack");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return FALSE;
    }

    lua_pushnumber(lua, charaindex);
    lua_pushnumber(lua, point);
// printf("GloryCallBack:%d,%d\n", charaindex, point);
    docall(lua, 2, 1);
    return TRUE;
}

#endif

#ifdef _GETTANKPET_
BOOL FreeGetTransPet(int charaindex, int petindex)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/getTransPet.lua");

        if (lua == NULL)
        {
            return FALSE;
        }
    }

    lua_getglobal(lua, "getTransPet");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return FALSE;
    }

    //依次放入二个参数
    lua_pushnumber(lua, charaindex);
    lua_pushnumber(lua, petindex);
    docall(lua, 2, 1);
    return TRUE;
}

#endif

#ifdef _NEWPETSKILL_
int FreeusePetSkill(int charaindex, int petindex, int index)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/item/usePetSkill.lua");

        if (lua == NULL)
        {
            return -1;
        }
    }

    lua_getglobal(lua, "usePetSkill");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return -1;
    }

    //依次放入一个参数
    lua_pushnumber(lua, charaindex);
    lua_pushnumber(lua, petindex);
    lua_pushnumber(lua, index);

    if (lua_pcall(lua, 3, 1, 0) != 0)
    {
        printf("error running function `f': %s", lua_tostring(lua, -1));
    }

    if (!lua_isnumber(lua, -1))
    {
        printf("function `f' must return a number");
    }

    int ret = lua_tonumber(lua, -1);
    lua_pop(lua, 1);
    return ret;
}
#endif

#ifdef _USESTATUSMULTI_
int FreeuseStatusItem(int charaindex, int Success)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/getItemStatusSuccess.lua");

        if (lua == NULL)
        {
            return -1;
        }
    }

    lua_getglobal(lua, "getItemStatusSuccess");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return -1;
    }

    //依次放入一个参数
    lua_pushnumber(lua, charaindex);
    lua_pushnumber(lua, Success);

    if (lua_pcall(lua, 2, 1, 0) != 0)
    {
        printf("error running function `f': %s", lua_tostring(lua, -1));
    }

    if (!lua_isnumber(lua, -1))
    {
        printf("function `f' must return a number");
    }

    int ret = lua_tonumber(lua, -1);
    lua_pop(lua, 1);
    return ret;
}
#endif

#ifdef _ADDFMBATTLE_LUA_
//获取下书的时间
void FreeGetFmBattleTime(int meindex, int *rettime)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/getFmBattleTime.lua");

        if (lua == NULL)
        {
            return -1;
        }
    }

    lua_getglobal(lua, "getFmBattleTime");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return -1;
    }

    //依次放入一个参数
    lua_pushnumber(lua, meindex);

    if (lua_pcall(lua, 1, 1, 0) != 0)
    {
        printf("error running function `f': %s", lua_tostring(lua, -1));
    }

    if (!lua_isnumber(lua, -1))
    {
        printf("function `f' must return a number");
    }

    *rettime = lua_tonumber(lua, -1);
    lua_pop(lua, 1);
    return;
}
#endif

#ifdef _SPEED_CHECK_
void FreeSpeedCheck(int charaindex, int num)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/speedcheck.lua");

        if (lua == NULL)
        {
            return -1;
        }
    }

    lua_getglobal(lua, "speedcheck");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return -1;
    }

    //依次放入一个参数
    lua_pushnumber(lua, charaindex);
    lua_pushnumber(lua, num);

    if (lua_pcall(lua, 2, 1, 0) != 0)
    {
        printf("error running function `f': %s", lua_tostring(lua, -1));
    }

    if (!lua_isnumber(lua, -1))
    {
        printf("function `f' must return a number");
    }

    int reg = lua_tonumber(lua, -1);
    lua_pop(lua, 1);
    return;
}
#endif

#ifdef _LC_CHECKNAME_
int FreeCheckCharName(int charaindex, char *name)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/checkCharName.lua");

        if (lua == NULL)
        {
            return -1;
        }
    }

    lua_getglobal(lua, "checkCharName");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return -1;
    }

    //依次放入一个参数
    lua_pushnumber(lua, charaindex);
    lua_pushstring(lua, name);

    if (lua_pcall(lua, 2, 1, 0) != 0)
    {
        printf("error running function `f': %s", lua_tostring(lua, -1));
    }

    if (!lua_isnumber(lua, -1))
    {
        printf("function `f' must return a number");
    }

    int ret = lua_tonumber(lua, -1);
    lua_pop(lua, 1);
    return ret;
}

int FreeCheckFamilyName(int charaindex, char *name)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/checkCharName.lua");

        if (lua == NULL)
        {
            return -1;
        }
    }

    lua_getglobal(lua, "checkFamilyName");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return -1;
    }

    //依次放入一个参数
    lua_pushnumber(lua, charaindex);
    lua_pushstring(lua, name);

    if (lua_pcall(lua, 2, 1, 0) != 0)
    {
        printf("error running function `f': %s", lua_tostring(lua, -1));
    }

    if (!lua_isnumber(lua, -1))
    {
        printf("function `f' must return a number");
    }

    int ret = lua_tonumber(lua, -1);
    lua_pop(lua, 1);
    return ret;
}
#endif

BOOL FreeGetFamilyBattleNum(int id)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/getFamilyBattleNum.lua");

        if (lua == NULL)
        {
            return TRUE;
        }
    }

    lua_getglobal(lua, "getFamilyBattleNum");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return TRUE;
    }

    lua_pushnumber(lua, id);

    if (lua_pcall(lua, 1, 1, 0) != 0)
    {
        printf("error running function `f': %s", lua_tostring(lua, -1));
    }

    if (!lua_isnumber(lua, -1))
    {
        printf("function `f' must return a number");
    }

    int ret = lua_tonumber(lua, -1);
    lua_pop(lua, 1);
    return ret;
}









#ifdef _BATTLE_LOOK_
int FreeLookBattleFlag(int charaindex)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/getLookBattleFlag.lua");

        if (lua == NULL)
        {
            return -1;
        }
    }

    lua_getglobal(lua, "FreeLookBattleFlag");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return -1;
    }

    //依次放入一个参数
    lua_pushnumber(lua, charaindex);

    if (lua_pcall(lua, 1, 1, 0) != 0)
    {
        printf("error running function `f': %s", lua_tostring(lua, -1));
    }

    if (!lua_isnumber(lua, -1))
    {
        printf("function `f' must return a number");
    }

    int ret = lua_tonumber(lua, -1);
    lua_pop(lua, 1);
    return ret;
}
#endif
#ifdef _NEW_SHOP_
void FreeShopRecv(int charaindex)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/shop/shop.lua");

        if (lua == NULL)
        {
            return;
        }
    }

    lua_getglobal(lua, "OpenShop");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return;
    }

    lua_pushnumber(lua, charaindex);
    docall(lua, 1, 1);
    return;
}
#endif

int FreeSellPet(int meindex, int charaindex, int petindex)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/sellpet.lua");

        if (lua == NULL)
        {
            return TRUE;
        }
    }

    lua_getglobal(lua, "FreeSellPet");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return TRUE;
    }

    //依次放入三个参数
    lua_pushnumber(lua, meindex);
    lua_pushnumber(lua, charaindex);
    lua_pushnumber(lua, petindex);

    if (lua_pcall(lua, 3, 1, 0) != 0)
    {
        printf("error running function `f': %s", lua_tostring(lua, -1));
    }

    if (!lua_isnumber(lua, -1))
    {
        printf("function `f' must return a number");
    }

    int ret = lua_tonumber(lua, -1);
    lua_pop(lua, 1);
    return ret;
}

#ifdef  _BATTLE_CLI_TIME
int FreeBattleTime(int charaindex, int battletime)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/getBattleTime.lua");

        if (lua == NULL)
        {
            return battletime;
        }
    }

    lua_getglobal(lua, "FreegetBattleTime");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return TRUE;
    }

    //依次放入三个参数
    lua_pushnumber(lua, charaindex);
    lua_pushnumber(lua, battletime);

    if (lua_pcall(lua, 2, 1, 0) != 0)
    {
        printf("error running function `f': %s", lua_tostring(lua, -1));
    }

    if (!lua_isnumber(lua, -1))
    {
        printf("function `f' must return a number");
    }

    int ret = lua_tonumber(lua, -1);
    lua_pop(lua, 1);
    return ret;
}
#endif

BOOL FreeSetQqData(int charaindex, char* data)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/setQqData.lua");

        if (lua == NULL)
        {
            return FALSE;
        }
    }

    lua_getglobal(lua, "setQqData");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return FALSE;
    }

    //依次放入二个参数
    lua_pushnumber(lua, charaindex);
    lua_pushstring(lua, data);
    docall(lua, 2, 1);
    return TRUE;
}

BOOL FreeWalk(int charaindex)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/walk.lua");

        if (lua == NULL)
        {
            return FALSE;
        }
    }

    lua_getglobal(lua, "FreeWalk");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return FALSE;
    }

    //依次放入二个参数
    lua_pushnumber(lua, charaindex);
    docall(lua, 1, 1);
    return TRUE;
}
#ifdef _OFFLINE_BATTLE_RETEUN
int FreeOfflineBattleTime()
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/getBattleTime.lua");

        if (lua == NULL)
        {
            return 500;
        }
    }

    lua_getglobal(lua, "FreeOfflineBattleTime");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return TRUE;
    }

    if (lua_pcall(lua, 0, 1, 0) != 0)
    {
        printf("error running function `f': %s", lua_tostring(lua, -1));
    }

    if (!lua_isnumber(lua, -1))
    {
        printf("function `f' must return a number");
    }

    int ret = lua_tonumber(lua, -1);
    lua_pop(lua, 1);
    return ret;
}
#endif

BOOL FreeMail(int charaindex, int flg)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/mail.lua");

        if (lua == NULL)
        {
            return FALSE;
        }
    }

    lua_getglobal(lua, "FreeMail");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return FALSE;
    }

    //依次放入二个参数
    lua_pushnumber(lua, charaindex);
    lua_pushnumber(lua, flg);
    docall(lua, 2, 1);
    return TRUE;
}

BOOL FreeStreet(int fd, char* data)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/street.lua");

        if (lua == NULL)
        {
            return FALSE;
        }
    }

    lua_getglobal(lua, "FreeStreet");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return FALSE;
    }

    //依次放入二个参数
    lua_pushnumber(lua, fd);
    lua_pushstring(lua, data);
    docall(lua, 2, 1);
    return TRUE;
}

void FreeEchoRecv(int charaindex)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/echo.lua");

        if (lua == NULL)
        {
            return;
        }
    }

    lua_getglobal(lua, "Echo");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return;
    }

    lua_pushnumber(lua, charaindex);
    docall(lua, 1, 1);
    return;
}

int FreePlayerRideCtrl(int charaindex, int petindex, int workindex, int action, int throwweapon)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/playerridectrl.lua");

        if (lua == NULL)
        {
            return -9999;
        }
    }

    lua_getglobal(lua, "FreePlayerRideCtrl");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return -9999;
    }

    //依次放入二个参数
    lua_pushnumber(lua, charaindex);
    lua_pushnumber(lua, petindex);
    lua_pushnumber(lua, workindex);
    lua_pushnumber(lua, action);
    lua_pushnumber(lua, throwweapon);

    if (lua_pcall(lua, 5, 1, 0) != 0)
    {
        printf("error running function `f': %s", lua_tostring(lua, -1));
    }

    if (!lua_isnumber(lua, -1))
    {
        printf("function `f' must return a number");
    }

    int ret = lua_tonumber(lua, -1);
    lua_pop(lua, 1);
    return ret;
}

int FreePetLevelUpRand(int petindex)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/petleveluprand.lua");

        if (lua == NULL)
        {
            return FALSE;
        }
    }

    lua_getglobal(lua, "FreePetLevelUpRand");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return FALSE;
    }

    //依次放入二个参数
    lua_pushnumber(lua, petindex);

    if (lua_pcall(lua, 1, 1, 0) != 0)
    {
        printf("error running function `f': %s", lua_tostring(lua, -1));
    }

    if (!lua_isnumber(lua, -1))
    {
        printf("function `f' must return a number");
    }

    int ret = lua_tonumber(lua, -1);
    lua_pop(lua, 1);
    return ret;
}

int FreeMergeItem(int charaindex, char* data)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/mergeitem.lua");

        if (lua == NULL)
        {
            return FALSE;
        }
    }

    lua_getglobal(lua, "FreeMergeItem");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return FALSE;
    }

    //依次放入二个参数
    lua_pushnumber(lua, charaindex);
    lua_pushstring(lua, data);

    if (lua_pcall(lua, 2, 1, 0) != 0)
    {
        printf("error running function `f': %s", lua_tostring(lua, -1));
    }

    if (!lua_isnumber(lua, -1))
    {
        printf("function `f' must return a number");
    }

    int ret = lua_tonumber(lua, -1);
    lua_pop(lua, 1);
    return ret;
}

void FreeMergeItemOk(int charaindex)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/mergeitem.lua");

        if (lua == NULL)
        {
            return;
        }
    }

    lua_getglobal(lua, "FreeMergeItemOk");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return;
    }

    lua_pushnumber(lua, charaindex);
    docall(lua, 1, 1);
    return;
}

void FreeMergeItemFalse(int charaindex)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/mergeitem.lua");

        if (lua == NULL)
        {
            return;
        }
    }

    lua_getglobal(lua, "FreeMergeItemFalse");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return;
    }

    lua_pushnumber(lua, charaindex);
    docall(lua, 1, 1);
    return;
}


void FreeNewPlayer(int charaindex)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/newplayer.lua");

        if (lua == NULL)
        {
            return;
        }
    }

    lua_getglobal(lua, "FreeNewPlayer");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return;
    }

    lua_pushnumber(lua, charaindex);
    docall(lua, 1, 1);
    return;
}

#ifdef _NEWPLAYER_FLG
void FreeNewPlayerFlg(int charaindex, int flg1, int flg2)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/newplayer.lua");

        if (lua == NULL)
        {
            return;
        }
    }

    lua_getglobal(lua, "FreeNewPlayerFlg");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return;
    }

    lua_pushnumber(lua, charaindex);
    lua_pushnumber(lua, flg1);
    lua_pushnumber(lua, flg2);
    docall(lua, 3, 1);
    return;
}
#endif

void FreeFmTop(int charaindex)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/fmtop.lua");

        if (lua == NULL)
        {
            return;
        }
    }

    lua_getglobal(lua, "FreeFmTop");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return;
    }

    lua_pushnumber(lua, charaindex);
    docall(lua, 1, 1);
    return;
}

void FreeSecurity(int charaindex, int type, int codetype, int index, char* codedata)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/security.lua");

        if (lua == NULL)
        {
            return;
        }
    }

    lua_getglobal(lua, "FreeSecurity");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return;
    }

    lua_pushnumber(lua, charaindex);
    lua_pushnumber(lua, type);
    lua_pushnumber(lua, codetype);
    lua_pushnumber(lua, index);
    lua_pushstring(lua, codedata);
    docall(lua, 5, 1);
    return;
}

#ifdef _ADDRESS_ONLINE
int FreeServerId(int serverid)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/serverid.lua");

        if (lua == NULL)
        {
            return serverid;
        }
    }

    lua_getglobal(lua, "FreeServerId");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return serverid;
    }

    //依次放入二个参数
    lua_pushnumber(lua, serverid);

    if (lua_pcall(lua, 1, 1, 0) != 0)
    {
        printf("error running function `f': %s", lua_tostring(lua, -1));
    }

    if (!lua_isnumber(lua, -1))
    {
        printf("function `f' must return a number");
    }

    int ret = lua_tonumber(lua, -1);
    lua_pop(lua, 1);
    return ret;
}
#endif // _ADDRESS_ONLINE

#ifdef _NEWUI_MAGICUSE
char* FreeMagicName(int charaindex, int marray)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/magicname.lua");

        if (lua == NULL)
        {
            return "NULL";
        }
    }

    lua_getglobal(lua, "FreeMagicName");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return "NULL";
    }

    //依次放入二个参数
    lua_pushnumber(lua, charaindex);
    lua_pushnumber(lua, marray);

    if (lua_pcall(lua, 2, 1, 0) != 0)
    {
        printf("error running function `f': %s", lua_tostring(lua, -1));
    }

    if (!lua_isstring(lua, -1))
    {
        printf("function `f' must return a string");
    }

    char *ret = lua_tostring(lua, -1);
    lua_pop(lua, 1);
    return ret;
}

int FreeBattleItemUse(int charaindex, int toindex, int haveitemindex)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/battleitemuse.lua");

        if (lua == NULL)
        {
            return FALSE;
        }
    }

    lua_getglobal(lua, "FreeBattleItemUse");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return FALSE;
    }

    //依次放入二个参数
    lua_pushnumber(lua, charaindex);
    lua_pushnumber(lua, toindex);
    lua_pushnumber(lua, haveitemindex);

    if (lua_pcall(lua, 3, 1, 0) != 0)
    {
        printf("error running function `f': %s", lua_tostring(lua, -1));
    }

    if (!lua_isnumber(lua, -1))
    {
        printf("function `f' must return a number");
    }

    int ret = lua_tonumber(lua, -1);
    lua_pop(lua, 1);
    return ret;
}
#endif

int FreeMagicUse(int charaindex, int toindex, int magicid, int mp)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/magicuse.lua");

        if (lua == NULL)
        {
            return TRUE;
        }
    }

    lua_getglobal(lua, "FreeMagicUse");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return TRUE;
    }

    //依次放入二个参数
    lua_pushnumber(lua, charaindex);
    lua_pushnumber(lua, toindex);
    lua_pushnumber(lua, magicid);
    lua_pushnumber(lua, mp);

    if (lua_pcall(lua, 4, 1, 0) != 0)
    {
        printf("error running function `f': %s", lua_tostring(lua, -1));
    }

    if (!lua_isnumber(lua, -1))
    {
        printf("function `f' must return a number");
    }

    int ret = lua_tonumber(lua, -1);
    lua_pop(lua, 1);
    return ret;
}

void FreePlayerLevelUp(int charaindex)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/playerlevelup.lua");

        if (lua == NULL)
        {
            return;
        }
    }

    lua_getglobal(lua, "FreePlayerLevelUp");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return;
    }

    lua_pushnumber(lua, charaindex);
    docall(lua, 1, 1);
    return;
}

int FreeShowFmPkWindows(int charaindex)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/showfmpkwindows.lua");

        if (lua == NULL)
        {
            return FALSE;
        }
    }

    lua_getglobal(lua, "FreeShowFmPkWindows");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return FALSE;
    }

    //依次放入二个参数
    lua_pushnumber(lua, charaindex);

    if (lua_pcall(lua, 1, 1, 0) != 0)
    {
        printf("error running function `f': %s", lua_tostring(lua, -1));
    }

    if (!lua_isnumber(lua, -1))
    {
        printf("function `f' must return a number");
    }

    int ret = lua_tonumber(lua, -1);
    lua_pop(lua, 1);
    return ret;
}

void FreeShowPlayerPet(int charaindex, int toindex)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/showplayerpet.lua");

        if (lua == NULL)
        {
            return;
        }
    }

    lua_getglobal(lua, "FreeShowPlayerPet");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return;
    }

    lua_pushnumber(lua, charaindex);
    lua_pushnumber(lua, toindex);
    docall(lua, 2, 1);
    return;
}

void TeacherExt(int charaindex, int toindex)
{
    MY_Lua *mylua = &MYLua;

    while (mylua->lua != NULL)
    {
        lua_getglobal(mylua->lua, "TeacherExt");

        if (!lua_isfunction(mylua->lua, -1))
        {
            lua_pop(mylua->lua, 1);
            mylua = mylua->next;
            continue;
        }

        lua_pushnumber(mylua->lua, charaindex);
        lua_pushnumber(mylua->lua, toindex);
        docall(mylua->lua, 2, 1);
        mylua = mylua->next;
    }
}

void FreeDischargeParty(int charaindex)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/dischargeparty.lua");

        if (lua == NULL)
        {
            return;
        }
    }

    lua_getglobal(lua, "FreeDischargeParty");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return;
    }

    lua_pushnumber(lua, charaindex);
    docall(lua, 1, 1);
    return;
}

void FreeSetInt(int charaindex, int datatype, int olddata, int newdata, char* file, int line)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/setint.lua");

        if (lua == NULL)
        {
            return;
        }
    }

    lua_getglobal(lua, "FreeSetInt");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return;
    }

    lua_pushnumber(lua, charaindex);
    lua_pushnumber(lua, datatype);
    lua_pushnumber(lua, olddata);
    lua_pushnumber(lua, newdata);
    lua_pushstring(lua, file);
    lua_pushnumber(lua, line);
    docall(lua, 6, 1);
    return;
}

void FreeSetWorkInt(int charaindex, int datatype, int olddata, int newdata, char* file, int line)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/setworkint.lua");

        if (lua == NULL)
        {
            return;
        }
    }

    lua_getglobal(lua, "FreeSetWorkInt");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return;
    }

    lua_pushnumber(lua, charaindex);
    lua_pushnumber(lua, datatype);
    lua_pushnumber(lua, olddata);
    lua_pushnumber(lua, newdata);
    lua_pushstring(lua, file);
    lua_pushnumber(lua, line);
    docall(lua, 6, 1);
    return;
}

int FreeDispatchMessage(int fd, int func)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/dispatchmessage.lua");

        if (lua == NULL)
        {
            return FALSE;
        }
    }

    lua_getglobal(lua, "FreeDispatchMessage");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return FALSE;
    }

    //依次放入二个参数
    lua_pushnumber(lua, fd);
    lua_pushnumber(lua, func);

    if (lua_pcall(lua, 2, 1, 0) != 0)
    {
        printf("error running function `f': %s", lua_tostring(lua, -1));
    }

    if (!lua_isnumber(lua, -1))
    {
        printf("function `f' must return a number");
    }

    int ret = lua_tonumber(lua, -1);
    lua_pop(lua, 1);
    return ret;
}

void FreeFdError(int fd, int fucnid)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/fderror.lua");

        if (lua == NULL)
        {
            return;
        }
    }

    lua_getglobal(lua, "FreeFdError");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return;
    }

    lua_pushnumber(lua, fd);
    lua_pushnumber(lua, fucnid);
    docall(lua, 2, 1);
    return;
}

#ifdef _NEW_NETSENDRECV
void FreeCharList(int fd, char* resault, char* data)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/charlist.lua");

        if (lua == NULL)
        {
            return;
        }
    }

    lua_getglobal(lua, "FreeCharList");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return;
    }

    lua_pushnumber(lua, fd);
    lua_pushstring(lua, resault);
    lua_pushstring(lua, data);
    docall(lua, 3, 1);
    return;
}
#endif

void FreeEvent(int charaindex, int eventid, int eventid2)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/event.lua");

        if (lua == NULL)
        {
            return;
        }
    }

    lua_getglobal(lua, "FreeEvent");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return;
    }

    lua_pushnumber(lua, charaindex);
    lua_pushnumber(lua, eventid);
    lua_pushnumber(lua, eventid2);
    docall(lua, 3, 1);
    return;
}

void FreeGmRecv(char* buff, char* ip)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/gmrecv.lua");

        if (lua == NULL)
        {
            return;
        }
    }

    lua_getglobal(lua, "FreeGmRecv");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return;
    }

    lua_pushstring(lua, buff);
    lua_pushstring(lua, ip);
    docall(lua, 2, 1);
    return;
}

char* FreeShowItemEffect(int itemindex)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/showitemeffect.lua");

        if (lua == NULL)
        {
            return ITEM_getChar(itemindex, ITEM_EFFECTSTRING);
        }
    }

    lua_getglobal(lua, "FreeShowItemEffect");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return ITEM_getChar(itemindex, ITEM_EFFECTSTRING);
    }

    //依次放入二个参数
    lua_pushnumber(lua, itemindex);

    if (lua_pcall(lua, 1, 1, 0) != 0)
    {
        printf("error running function `f': %s", lua_tostring(lua, -1));
    }

    if (!lua_isstring(lua, -1))
    {
        printf("function `f' must return a string");
    }

    char* ret = lua_tostring(lua, -1);
    lua_pop(lua, 1);
    return ret;
}

int FreeBattleComboCheck(int charaindex, int per)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/battlecombocheck.lua");

        if (lua == NULL)
        {
            return per;
        }
    }

    lua_getglobal(lua, "FreeBattleComboCheck");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return per;
    }

    //依次放入二个参数
    lua_pushnumber(lua, charaindex);
    lua_pushnumber(lua, per);

    if (lua_pcall(lua, 2, 1, 0) != 0)
    {
        printf("error running function `f': %s", lua_tostring(lua, -1));
    }

    if (!lua_isnumber(lua, -1))
    {
        printf("function `f' must return a number");
    }

    int ret = lua_tonumber(lua, -1);
    lua_pop(lua, 1);
    return ret;
}

BOOL FreeCharShowType(int charaindex1, int charaindex2)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/charshowtype.lua");

        if (lua == NULL)
        {
            return TRUE;
        }
    }

    lua_getglobal(lua, "FreeCharShowType");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return TRUE;
    }

    //依次放入二个参数
    lua_pushnumber(lua, charaindex1);
    lua_pushnumber(lua, charaindex2);

    if (lua_pcall(lua, 2, 1, 0) != 0)
    {
        printf("error running function `f': %s", lua_tostring(lua, -1));
    }

    if (!lua_isnumber(lua, -1))
    {
        printf("function `f' must return a number");
    }

    int ret = lua_tonumber(lua, -1);
    lua_pop(lua, 1);
    return ret;
}

BOOL FreeBattleHelp(int charaindex, int flg)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/battlehelp.lua");

        if (lua == NULL)
        {
            return TRUE;
        }
    }

    lua_getglobal(lua, "FreeBattleHelp");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return TRUE;
    }

    //依次放入二个参数
    lua_pushnumber(lua, charaindex);
    lua_pushnumber(lua, flg);

    if (lua_pcall(lua, 2, 1, 0) != 0)
    {
        printf("error running function `f': %s", lua_tostring(lua, -1));
    }

    if (!lua_isnumber(lua, -1))
    {
        printf("function `f' must return a number");
    }

    int ret = lua_tonumber(lua, -1);
    lua_pop(lua, 1);
    return ret;
}


void FreeBattleDie(int battleindex, int charaindex, int no)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/battledie.lua");

        if (lua == NULL)
        {
            return;
        }
    }

    lua_getglobal(lua, "FreeBattleDie");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return;
    }

    lua_pushnumber(lua, battleindex);
    lua_pushnumber(lua, charaindex);
    lua_pushnumber(lua, no);
    docall(lua, 3, 1);
    return;
}

int CapturePer(int attackindex, int defindex, int per)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/capturefunction.lua");

        if (lua == NULL)
        {
            return per;
        }
    }

    lua_getglobal(lua, "CapturePer");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return per;
    }

    //依次放入二个参数
    lua_pushnumber(lua, attackindex);
    lua_pushnumber(lua, defindex);
    lua_pushnumber(lua, per);

    if (lua_pcall(lua, 3, 1, 0) != 0)
    {
        printf("error running function `f': %s", lua_tostring(lua, -1));
    }

    if (!lua_isnumber(lua, -1))
    {
        printf("function `f' must return a number");
    }

    int ret = lua_tonumber(lua, -1);
    lua_pop(lua, 1);
    return ret;
}

int getlevelupexp(int charaindex, int level)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/levelupexp.lua");

        if (lua == NULL)
        {
            return 0;
        }
    }

    lua_getglobal(lua, "getlevelupexp");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return 0;
    }

    //依次放入二个参数
    lua_pushnumber(lua, charaindex);
    lua_pushnumber(lua, level);

    if (lua_pcall(lua, 2, 1, 0) != 0)
    {
        printf("error running function `f': %s", lua_tostring(lua, -1));
    }

    if (!lua_isnumber(lua, -1))
    {
        printf("function `f' must return a number");
    }

    int ret = lua_tonumber(lua, -1);
    lua_pop(lua, 1);
    return ret;
}


BOOL FreeFmSetPoint(int charaindex, int fmpointindex)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/fmsetpoint.lua");

        if (lua == NULL)
        {
            return TRUE;
        }
    }

    lua_getglobal(lua, "FreeFmSetPoint");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return TRUE;
    }

    //依次放入二个参数
    lua_pushnumber(lua, charaindex);
    lua_pushnumber(lua, fmpointindex);

    if (lua_pcall(lua, 2, 1, 0) != 0)
    {
        printf("error running function `f': %s", lua_tostring(lua, -1));
    }

    if (!lua_isnumber(lua, -1))
    {
        printf("function `f' must return a number");
    }

    int ret = lua_tonumber(lua, -1);
    lua_pop(lua, 1);
    return ret;
}

#ifdef _SMART_ENEMY_LUA
struct B_AI_RESULT
{
    BATTLE_COM  command;
    int         target;
};

int NewBattleAiCommand(int turn, int charaindex, struct B_AI_RESULT *result, int battleindex)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/battlecommand.lua");

        if (lua == NULL)
        {
            return NULL;
        }
    }

    lua_getglobal(lua, "NewBattleAiCommand");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return NULL;
    }

    //依次放入二个参数
    lua_pushnumber(lua, turn);
    lua_pushnumber(lua, charaindex);
    lua_pushnumber(lua, battleindex);

    if (lua_pcall(lua, 3, 2, 0) != 0)
    {
        printf("error running function `f': %s", lua_tostring(lua, -1));
    }

    int ret1 = lua_tonumber(lua, -1);
    int ret2 = lua_tonumber(lua, -2);
    lua_pop(lua, 1);
    result->command = ret2;
    result->target = ret1;
    lua_pop(lua, 1);
    return TRUE;
}
#endif

void FreeGmLog(char* gmdata)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/gmlog.lua");

        if (lua == NULL)
        {
            return;
        }
    }

    lua_getglobal(lua, "FreeGmLog");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return;
    }

    lua_pushstring(lua, gmdata);
    docall(lua, 1, 1);
    return;
}

int FreeBattleStatus(int attackindex, int defindex, int status, int per)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/battlestatus.lua");

        if (lua == NULL)
        {
            return per;
        }
    }

    lua_getglobal(lua, "FreeBattleStatus");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return per;
    }

    //依次放入二个参数
    lua_pushnumber(lua, attackindex);
    lua_pushnumber(lua, defindex);
    lua_pushnumber(lua, status);
    lua_pushnumber(lua, per);

    if (lua_pcall(lua, 4, 1, 0) != 0)
    {
        printf("error running function `f': %s", lua_tostring(lua, -1));
    }

    if (!lua_isnumber(lua, -1))
    {
        printf("function `f' must return a number");
    }

    int ret = lua_tonumber(lua, -1);
    lua_pop(lua, 1);
    return ret;
}

void FreeBattleRound(int battleindex)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/battle.lua");

        if (lua == NULL)
        {
            return;
        }
    }

    lua_getglobal(lua, "FreeBattleRound");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return;
    }

    lua_pushnumber(lua, battleindex);
    docall(lua, 1, 1);
    return;
}

void FreeOtherCommand(int fromid, char* cmdcommand, char* cmddata, char* cmddata2, char* cmddata3)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/saac.lua");

        if (lua == NULL)
        {
            return;
        }
    }

    lua_getglobal(lua, "FreeOtherCommand");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return;
    }

    lua_pushnumber(lua, fromid);
    lua_pushstring(lua, cmdcommand);
    lua_pushstring(lua, cmddata);
    lua_pushstring(lua, cmddata2);
    lua_pushstring(lua, cmddata3);
    docall(lua, 5, 1);
    return;
}


#ifdef _NEW_LUA_FAMILY_
int FamilyDetailRecv(int charaindex, char* data)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/familyfunction.lua");

        if (lua == NULL)
        {
            return 0;
        }
    }

    lua_getglobal(lua, "FamilyDetailRecv");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return 0;
    }

    //依次放入二个参数
    lua_pushnumber(lua, charaindex);
    lua_pushstring(lua, data);

    if (lua_pcall(lua, 2, 1, 0) != 0)
    {
        error(lua, "error running function `f': %s", lua_tostring(lua, -1));
    }

    if (!lua_isnumber(lua, -1))
    {
        error(lua, "function `f' must return a number");
    }

    int ret = lua_tonumber(lua, -1);
    lua_pop(lua, 1);
    return ret;
}

int FamilyBankRecv(int charaindex, int count)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/familyfunction.lua");

        if (lua == NULL)
        {
            return 0;
        }
    }

    lua_getglobal(lua, "FamilyBankRecv");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return 0;
    }

    //依次放入二个参数
    lua_pushnumber(lua, charaindex);
    lua_pushnumber(lua, count);

    if (lua_pcall(lua, 2, 1, 0) != 0)
    {
        error(lua, "error running function `f': %s", lua_tostring(lua, -1));
    }

    if (!lua_isnumber(lua, -1))
    {
        error(lua, "function `f' must return a number");
    }

    int ret = lua_tonumber(lua, -1);
    lua_pop(lua, 1);
    return ret;
}

#ifdef _FM_WIN_CALLBACK
void FmPkWinFunction(int manorid, char* data)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/familyfunction.lua");

        if (lua == NULL)
        {
            return;
        }
    }

    lua_getglobal(lua, "FmPkWinFunction");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return;
    }

    //依次放入二个参数
    lua_pushnumber(lua, manorid);
    lua_pushstring(lua, data);
    docall(lua, 2, 1);
    return;
}

#endif
#endif

#ifdef _NEWPLAYER_FUNC

void FreeNewPlayerRecv(int charaindex, char* data)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/newplayerfunction.lua");

        if (lua == NULL)
        {
            return;
        }
    }

    lua_getglobal(lua, "newPlayerDataRecv");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return;
    }

    //依次放入二个参数
    lua_pushnumber(lua, charaindex);
    lua_pushstring(lua, data);
    docall(lua, 2, 1);
    return;
}
#endif


#ifdef _NEWSAMENU_WITH_CALLBACK
void FreeNewSaMenu2(int fd, int index, char* data, char* func)
{
    static lua_State *lua;

    if (lua == NULL)
    {
        lua = FindLua("data/ablua/samenu.lua");

        if (lua == NULL)
        {
            return;
        }
    }

    lua_getglobal(lua, "FreeNewSaMenu2");

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return;
    }

    lua_pushnumber(lua, fd);
    lua_pushnumber(lua, index);
    lua_pushstring(lua, data);
    lua_pushstring(lua, func);
    docall(lua, 4, 1);
}
#endif
