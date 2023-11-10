#include <string.h>
#include "char.h"
#include "char_base.h"
#include "family.h"
#include "mylua/base.h"
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
#include "saacproto_cli.h"
#include "npc_scheduleman.h"
#include "npc_fmwarpman.h"
#include "handletime.h"
#ifdef _ALLBLUES_LUA

extern char familyListBuf[MAXFAMILYLIST];
extern struct FMMEMBER_LIST memberlist[FAMILY_MAXNUM];
extern struct FMS_DPTOP     fmdptop;
#ifdef _MO_LUA_FMPOINT
extern struct  FM_POINTLIST fmpointlist;                   // 家族据点
#endif
extern int familyNumTotal;
static int ShowFamilyList(lua_State *L)
{
    size_t l;
    const int index = luaL_checkint(L, 1);
    char subbuf[256];

    if (getStringFromIndexWithDelim(familyListBuf, "|", index, subbuf, sizeof(subbuf)) == TRUE)
    {
        lua_pushstring(L, subbuf);
    }
    else
    {
        lua_pushstring(L, "");
    }

    return 1;
}

static int ShowFamilyListNum(lua_State *L)
{
    lua_pushinteger(L, familyNumTotal);
    return 1;
}

static int ShowFamilyPkList(lua_State *L)
{
    size_t l;
    const int index = luaL_checkint(L, 1);
    char NPC_sendbuf[128 * 10];
    memset(&NPC_sendbuf, 0, sizeof(NPC_sendbuf));
    char fmpointname[5][16] = {"萨姆吉尔", "玛丽娜丝", "加加庄园", "卡鲁它那", "伊甸庄园"};
    char fmpointtype[5][16] = {"5V5", "5V5", "5V5", "1V1", "2V2"};
    int i, fmpks_pos;
#ifdef  _NO_WAIGUA

    for (i = 1; i <= 4; i++)
    {
#else

    for (i = 1; i <= 5; i++)
    {
#endif
        fmpks_pos = i * MAX_SCHEDULE;
        char tmpbuf[256] = "";
        int fmtime = sasql_queryFmPointData(i, 0);
        int fmpkmum = sasql_queryFmPointData(i, 1);
        char fmtimebuff[64];
        char fmnumbuff[64];
        memset(fmtimebuff, 0, sizeof(fmtimebuff));
        memset(fmnumbuff, 0, sizeof(fmnumbuff));

        if (fmtime > 0)
        {
            sprintf(fmtimebuff, "占领天数[%d]", (NowTime.tv_sec - fmtime) / 86400);
        }
        else
        {
            sprintf(fmtimebuff, "占领天数[0]");
        }

        if (fmpkmum >= 0)
        {
            sprintf(fmnumbuff, " 成功守庄：%d次", fmpkmum);
        }
        else
        {
            sprintf(fmnumbuff, " 成功守庄：无记录");
        }

        switch (fmpks[fmpks_pos + 1].flag)
        {
            case FMPKS_FLAG_MANOR_BATTLEBEGIN:
            case FMPKS_FLAG_MANOR_PREPARE:
            case FMPKS_FLAG_MANOR_OTHERPLANET:
                {
                    time_t dueltime = (time_t)fmpks[fmpks_pos + 1].dueltime;
                    struct tm *tm1 = localtime(&dueltime);
#ifdef  _NO_WAIGUA

                    if (i == 3)
                    {
                        sprintf(fmpointtype[i - 1], "%s", "2V2");
                    }

#endif

                    if (i == 5)
                    {
                        if (tm1->tm_wday == 0 || tm1->tm_wday % 2 != 0)
                        {
                            sprintf(fmpointtype[i - 1], "%s", "1V1");
                        }
                    }

                    if (dueltime > 0)
                        sprintf(tmpbuf, "　　「%s」%s 族战模式[%s]\n"
                                "　　　　　　　　%s ｖｓ %s\n"
                                "　　　　　　　　%02d月%02d日 %d:%d　%d ｖｓ %d\n", fmpointname[i - 1], fmtimebuff, fmpointtype[i - 1], //fmpks[fmpks_pos + 2].host_name,
                                fmpks[fmpks_pos + 1].guest_name,
                                fmpks[fmpks_pos + 1].host_name,
                                tm1->tm_mon + 1, tm1->tm_mday, tm1->tm_hour, tm1->tm_min, fmpks[fmpks_pos + 1].max_player, fmpks[fmpks_pos + 1].max_player);
                    else
                        sprintf(tmpbuf, "　　「%s」%s 族战模式[%s]\n"
                                "　　　　　　　　%s ｖｓ %s\n"
                                "　　　　　　　　「正在族战中」　%d ｖｓ %d\n", fmpointname[i - 1], fmtimebuff, fmpointtype[i - 1], //fmpks[fmpks_pos + 2].host_name,
                                fmpks[fmpks_pos + 1].guest_name,
                                fmpks[fmpks_pos + 1].host_name,
                                fmpks[fmpks_pos + 1].max_player,
                                fmpks[fmpks_pos + 1].max_player
                               );
                }
                break;

            default:
                {
#ifdef  _NO_WAIGUA

                    if (i == 3)
                    {
                        sprintf(fmpointtype[i - 1], "%s", "2V2");
                    }

#endif

                    if (i == 5)
                    {
                        sprintf(tmpbuf, "　　「%s」%s 族战模式[1V1] & [2V2]\n　　　　　　　　无挑战排程\n\n", fmpointname[i - 1], fmtimebuff);
                    }
                    else
                    {
                        sprintf(tmpbuf, "　　「%s」%s 族战模式[%s]\n　　　　　　　　无挑战排程\n\n", fmpointname[i - 1], fmtimebuff, fmpointtype[i - 1]);
                    }
                }
                break;
        }

        strcat(NPC_sendbuf, tmpbuf);
    }

#ifndef _NO_WAIGUA
    strcat(NPC_sendbuf, "　　「混乱之庄」每周六21点开启活动 无需下书 激情对决");
#endif
    lua_pushstring(L, NPC_sendbuf);
    return 1;
}

static int ShowFamilyPkNum(lua_State *L)
{
    size_t l;
    const int floorid = luaL_checkint(L, 1);
    const int fmpks_pos = luaL_checkint(L, 2) * MAX_SCHEDULE;
    char token[1024];
    int num1 = 0, num2 = 0;
    memset(token, 0, sizeof(token));
    NPC_GetPKFMNum(floorid,
                   fmpks[fmpks_pos].host_index,
                   fmpks[fmpks_pos].guest_index,
                   &num1, &num2);
    sprintf(token, "\n%s\n\n%s:%4d人\n\n%s:%4d人", "目前两个家族人数如下：",
            fmpks[fmpks_pos].host_name, num1,
            fmpks[fmpks_pos].guest_name, num2);
    lua_pushstring(L, token);
    return 1;
}

static int ShowFamilyPkNumTalk(lua_State *L)
{
    const int charaindex = luaL_checkint(L, 1);
    const int floorid = luaL_checkint(L, 2);
    const int fmpks_pos = luaL_checkint(L, 3) * MAX_SCHEDULE;
    char token[1024];
    int num1 = 0, num2 = 0;
    memset(token, 0, sizeof(token));
    NPC_GetPKFMNum(floorid,
                   fmpks[fmpks_pos].host_index,
                   fmpks[fmpks_pos].guest_index,
                   &num1, &num2);
    CHAR_talkToCli(charaindex, -1, "目前两个家族人数如下：", CHAR_COLORYELLOW);
    sprintf(token, "%s:%4d人", fmpks[fmpks_pos].host_name, num1);
    CHAR_talkToCli(charaindex, -1, token, CHAR_COLORYELLOW);
    sprintf(token, "%s:%4d人", fmpks[fmpks_pos].guest_name, num2);
    CHAR_talkToCli(charaindex, -1, token, CHAR_COLORYELLOW);
    return 1;
}

static int ShowFamilyPkTime(lua_State *L)
{
    size_t l;
    const int fmpks_pos = luaL_checkint(L, 1) * MAX_SCHEDULE;
    char token[1024];
    memset(token, 0, sizeof(token));

    if (fmpks[fmpks_pos + 1].flag == FMPKS_FLAG_MANOR_OTHERPLANET || fmpks[fmpks_pos + 1].flag == FMPKS_FLAG_MANOR_BATTLEBEGIN || fmpks[fmpks_pos + 1].flag == FMPKS_FLAG_MANOR_PREPARE)
    {
        time_t dueltime = (time_t)fmpks[fmpks_pos + 1].dueltime;
        struct tm *tm1 = localtime(&dueltime);
        sprintf(token, "%02d%02d", tm1->tm_mon + 1, tm1->tm_mday);
    }
    else
    {
        sprintf(token, "0000");
    }

    lua_pushstring(L, token);
    return 1;
}

static int DelFamily(lua_State *L)
{
    const int fmindex = luaL_checkint(L, 1) - 1;
    const int fdid = getFdidFromCharaIndex(luaL_checkint(L, 2));

    if (fmindex < 0 || fmindex >= FAMILY_MAXNUM)
    {
        return 1;
    }

    saacproto_ACNewDelFM_send(acfd, fmindex, fdid);
    return 1;
}

static int LuaACShowMemberList(lua_State *L)
{
    const int fmindex = luaL_checkint(L, 1) - 1;

    if (fmindex < 0 || fmindex >= FAMILY_MAXNUM)
    {
        return 1;
    }

    saacproto_ACShowMemberList_send(acfd, fmindex);
    return 1;
}

static int ShowMemberListData(lua_State *L)
{
    const int fmindex = luaL_checkint(L, 1) - 1;
    const int memberindex = luaL_checkint(L, 2) - 1;

    if (fmindex < 0 || fmindex >= FAMILY_MAXNUM)
    {
        lua_pushstring(L, "");
        return 1;
    }

    if (memberlist[fmindex].fmnum < memberindex + 1)
    {
        lua_pushstring(L, "");
        return 1;
    }

    lua_pushstring(L, memberlist[fmindex].numberlistarray[memberindex]);
    return 1;
}

static int ShowMemberListNum(lua_State *L)
{
    const int fmindex = luaL_checkint(L, 1) - 1;

    if (fmindex < 0 || fmindex >= FAMILY_MAXNUM)
    {
        lua_pushinteger(L, 0);
        return 1;
    }

    lua_pushinteger(L, memberlist[fmindex].fmnum);
    return 1;
}
#ifdef _MO_LUA_FMPOINT
static int ShowPointListArray(lua_State *L)
{
    size_t l;
    const int index = luaL_checkint(L, 1);

    if (index >= FAMILY_MAXHOME || index < 0)
    {
        lua_pushstring(L, fmpointlist.pointlistarray[0]);
    }

    lua_pushstring(L, fmpointlist.pointlistarray[index]);
    return 1;
}
#endif
static int ShowFamilyPkName(lua_State *L)
{
    const int floorid = luaL_checkint(L, 1);
    const int fmpks_pos = luaL_checkint(L, 2) * MAX_SCHEDULE;
    const int type = luaL_checkint(L, 3);
    char token[1024];
    int num1 = 0, num2 = 0;
    memset(token, 0, sizeof(token));

    if (type == 1)
    {
        sprintf(token, "%s", fmpks[fmpks_pos].host_name);
    }
    else if (type == 2)
    {
        sprintf(token, "%s", fmpks[fmpks_pos].guest_name);
    }

    lua_pushstring(L, token);
    return 1;
}
static int ShowFamilyPkPlayerNum(lua_State *L)
{
    const int floorid = luaL_checkint(L, 1);
    const int fmpks_pos = luaL_checkint(L, 2) * MAX_SCHEDULE;
    const int type = luaL_checkint(L, 3);
    int num1 = 0, num2 = 0;
    NPC_GetPKFMNum(floorid,
                   fmpks[fmpks_pos].host_index,
                   fmpks[fmpks_pos].guest_index,
                   &num1, &num2);

    if (type == 1)
    {
        lua_pushinteger(L, num1);
    }
    else if (type == 2)
    {
        lua_pushinteger(L, num2);
    }
    else
    {
        lua_pushinteger(L, 0);
    }

    return 1;
}

static int ShowFamilyTopNum(lua_State *L)
{
    lua_pushinteger(L, fmdptop.num);
    return 1;
}

static int ShowFamilyTop(lua_State *L)
{
    const int i = luaL_checkint(L, 1);
    lua_pushstring(L, fmdptop.topmemo[i]);
    return 1;
}

static int getFamilyPkTime(lua_State *L)
{
    const int fmpks_pos = luaL_checkint(L, 1) * MAX_SCHEDULE;

    if (fmpks[fmpks_pos + 1].flag == FMPKS_FLAG_MANOR_OTHERPLANET || fmpks[fmpks_pos + 1].flag == FMPKS_FLAG_MANOR_BATTLEBEGIN || fmpks[fmpks_pos + 1].flag == FMPKS_FLAG_MANOR_PREPARE)
    {
        lua_pushinteger(L, fmpks[fmpks_pos + 1].dueltime);
    }
    else
    {
        lua_pushinteger(L, 0);
    }

    return 1;
}
static int getFamilyPkHostName(lua_State *L)
{
    const int fmpks_pos = luaL_checkint(L, 1) * MAX_SCHEDULE;

    if (fmpks[fmpks_pos + 1].flag == FMPKS_FLAG_MANOR_OTHERPLANET || fmpks[fmpks_pos + 1].flag == FMPKS_FLAG_MANOR_BATTLEBEGIN || fmpks[fmpks_pos + 1].flag == FMPKS_FLAG_MANOR_PREPARE)
    {
        lua_pushstring(L, fmpks[fmpks_pos + 1].host_name);
    }
    else
    {
        lua_pushstring(L, "");
    }

    return 1;
}
static int getFamilyPkGuestName(lua_State *L)
{
    const int fmpks_pos = luaL_checkint(L, 1) * MAX_SCHEDULE;

    if (fmpks[fmpks_pos + 1].flag == FMPKS_FLAG_MANOR_OTHERPLANET || fmpks[fmpks_pos + 1].flag == FMPKS_FLAG_MANOR_BATTLEBEGIN || fmpks[fmpks_pos + 1].flag == FMPKS_FLAG_MANOR_PREPARE)
    {
        lua_pushstring(L, fmpks[fmpks_pos + 1].guest_name);
    }
    else
    {
        lua_pushstring(L, "");
    }

    return 1;
}
static int ShowFmJoinNum(lua_State *L)
{
    const int fmworkindex = luaL_checkint(L, 1);

    if (fmworkindex < 0 || fmworkindex >= FAMILY_MAXNUM)
    {
        lua_pushinteger(L, 0);
        return 1;
    }

    lua_pushinteger(L, memberlist[fmworkindex].fmjoinnum);
    return 1;
}
static int ShowFmRank(lua_State *L)
{
    const int fmworkindex = luaL_checkint(L, 1);

    if (fmworkindex < 0 || fmworkindex >= FAMILY_MAXNUM)
    {
        lua_pushinteger(L, -1);
        return 1;
    }

    int h;

    for (h = 0; h < FAMILY_MAXNUM; h++)
    {
        if (fmdptop.fmtopid[h] == fmworkindex)
        {
            break;
        }
    }

    lua_pushinteger(L, h);
    return 1;
}
static int ShowFmFame(lua_State *L)
{
    const int fmrank = luaL_checkint(L, 1);

    if (fmrank < 0 || fmrank >= FAMILY_MAXNUM)
    {
        lua_pushinteger(L, 0);
        return 1;
    }

    lua_pushinteger(L, fmdptop.fmtopdp[fmrank]);
    return 1;
}
static int ShowFmMomnum(lua_State *L)
{
    const int fmrank = luaL_checkint(L, 1);

    if (fmrank < 0 || fmrank >= FAMILY_MAXNUM)
    {
        lua_pushinteger(L, 0);
        return 1;
    }

    lua_pushinteger(L, fmdptop.fmMomentum[fmrank]);
    return 1;
}
static int ShowFmBadge(lua_State *L)
{
    const int fmworkindex = luaL_checkint(L, 1);

    if (fmworkindex < 0 || fmworkindex >= FAMILY_MAXNUM)
    {
        lua_pushinteger(L, 0);
        return 1;
    }

    lua_pushinteger(L, memberlist[fmworkindex].badge);
    return 1;
}
#ifdef _FAMILY_QQ
static int ShowFmQq(lua_State *L)
{
    const int fmworkindex = luaL_checkint(L, 1);

    if (fmworkindex < 0 || fmworkindex >= FAMILY_MAXNUM)
    {
        lua_pushstring(L, "");
        return 1;
    }

    lua_pushstring(L, memberlist[fmworkindex].fmqq);
    return 1;
}
static int ShowFmRule(lua_State *L)
{
    const int fmworkindex = luaL_checkint(L, 1);

    if (fmworkindex < 0 || fmworkindex >= FAMILY_MAXNUM)
    {
        lua_pushstring(L, "");
        return 1;
    }

    lua_pushstring(L, memberlist[fmworkindex].fmrule);
    return 1;
}
static int ShowFmPetName(lua_State *L)
{
    const int fmworkindex = luaL_checkint(L, 1);

    if (fmworkindex < 0 || fmworkindex >= FAMILY_MAXNUM)
    {
        lua_pushstring(L, "");
        return 1;
    }

    lua_pushstring(L, memberlist[fmworkindex].petname);
    return 1;
}
static int ShowFmPetAttr(lua_State *L)
{
    const int fmworkindex = luaL_checkint(L, 1);

    if (fmworkindex < 0 || fmworkindex >= FAMILY_MAXNUM)
    {
        lua_pushstring(L, "");
        return 1;
    }

    lua_pushstring(L, memberlist[fmworkindex].petattr);
    return 1;
}
static int ShowFmSprite(lua_State *L)
{
    const int fmworkindex = luaL_checkint(L, 1);

    if (fmworkindex < 0 || fmworkindex >= FAMILY_MAXNUM)
    {
        lua_pushinteger(L, 0);
        return 1;
    }

    lua_pushinteger(L, memberlist[fmworkindex].fmsprite);
    return 1;
}
#endif
static int ShowFmAccept(lua_State *L)
{
    const int fmindex = luaL_checkint(L, 1) - 1;

    if (fmindex < 0 || fmindex >= FAMILY_MAXNUM)
    {
        lua_pushinteger(L, -1);
        return 1;
    }

    lua_pushinteger(L, memberlist[fmindex].accept);
    return 1;
}
static int LuaACFMReadMemo(lua_State *L)
{
    const int fmindex = luaL_checkint(L, 1) - 1;

    if (fmindex < 0 || fmindex >= FAMILY_MAXNUM)
    {
        return 1;
    }

    saacproto_ACFMReadMemo_send(acfd, fmindex);
    return 1;
}
static int LuaACFMPointList(lua_State *L)
{
    saacproto_ACFMPointList_send(acfd);
    return 1;
}
static int LuaACShowTopFMList(lua_State *L)
{
    const int type = luaL_checkint(L, 1);
    saacproto_ACShowTopFMList_send(acfd, type);
    return 1;
}
static int Family(lua_State *L)
{
	size_t l;
	const int charaindex = luaL_checkint(L, 1);
	const char *data = luaL_checklstring(L, 2, &l);
	CHAR_Family(getfdFromCharaIndex(charaindex), charaindex, data);
	return 1;
}

#ifdef _NEW_LUA_FAMILY_
extern int getFamilyBadge(int index);
static int GetFamilyBadge(lua_State *L)
{
	const int index = luaL_checkint(L, 1);
	if (CHAR_CHECKINDEX(index)){
		lua_pushinteger(L,getFamilyBadge(index));
	}else lua_pushinteger(L,0);
	return 1;
}

static int getMomentum(lua_State *L)
{
	const int fmindex = luaL_checkint(L, 1);
	if(fmindex < 0 || fmindex > 999){
		lua_pushinteger(L, 0);
		return 1;
	}
	int h = -1;
	for( h=0; h<FAMILY_MAXNUM; h++)
		if( fmdptop.fmtopid[h] == fmindex ){
			lua_pushinteger(L,fmdptop.fmMomentum[h]);
			return 1;
		}
	lua_pushinteger(L,0);
	return 1;
}

static int getFame(lua_State *L)
{
	const int fmindex = luaL_checkint(L, 1);
	if(fmindex < 0 || fmindex > 999){
		lua_pushinteger(L, 0);
		return 1;
	}

	int h = -1;
	for( h=0; h<FAMILY_MAXNUM; h++)
		if( fmdptop.fmtopid[h] == fmindex ){
			lua_pushinteger(L,fmdptop.fmtopdp[h]);
			return 1;
		}
	lua_pushinteger(L,0);
	return 1;
}

static int getFamilyRank(lua_State *L)
{
	const int fmindex_wk = luaL_checkint(L, 1);
	if( fmindex_wk < 0 || fmindex_wk >= FAMILY_MAXNUM){
		lua_pushinteger(L,-1);
		return 1;
	}
	int h= 0;
	for( h=0; h<FAMILY_MAXNUM; h++){
		if( fmdptop.fmtopid[h] == fmindex_wk ){
			lua_pushinteger(L,h+1);
			return 1;
		}
	}
	lua_pushinteger(L,-1);
	return 1;
}

static int getTopFamilyData(lua_State *L)
{
	const int id = luaL_checkint(L, 1);
	lua_pushstring(L,fmdptop.topmemo[id]);
	return 1;
}



extern int familyNumTotal;
static int getfamilyNumTotal(lua_State *L)
{
	lua_pushinteger(L,familyNumTotal);
	return 1;
}

static int getFmPointTime(lua_State *L)
{
	const int pointid = luaL_checkint(L, 1);
	lua_pushinteger(L,sasql_queryFmPointData(pointid,0));
	return 1;
}

static int getFmPointCount(lua_State *L)
{
	const int pointid = luaL_checkint(L, 1);
	lua_pushinteger(L,sasql_queryFmPointData(pointid,1));
	return 1;
}

static int getFmPKData(lua_State *L)
{
	size_t l;
	const int fmpks_pos = luaL_checkint(L, 1) * MAX_SCHEDULE;
	const char *args = luaL_checklstring(L, 2, &l);
	if( strcmp( args, "比赛时间" ) == 0 ){
		lua_pushinteger(L,fmpks[fmpks_pos+1].dueltime);
	}else if( strcmp( args, "主队索引" ) == 0 ){
		lua_pushinteger(L,fmpks[fmpks_pos+1].host_index);
	}else if( strcmp( args, "主队名字" ) == 0 ){
		lua_pushstring(L,fmpks[fmpks_pos+1].host_name);
	}else if( strcmp( args, "客队索引" ) == 0 ){
		lua_pushinteger(L,fmpks[fmpks_pos+1].guest_index);
	}else if( strcmp( args, "客队名字" ) == 0 ){
		lua_pushstring(L,fmpks[fmpks_pos+1].guest_name);
	}else if( strcmp( args, "准备时间" ) == 0 ){
		lua_pushinteger(L,fmpks[fmpks_pos+1].prepare_time);
	}else if( strcmp( args, "最大人数" ) == 0 ){
		lua_pushinteger(L,fmpks[fmpks_pos+1].max_player);
	}else if( strcmp( args, "比赛状态" ) == 0 ){
		lua_pushinteger(L,fmpks[fmpks_pos+1].flag);
	}else if( strcmp( args, "胜利条件" ) == 0 ){
		lua_pushinteger(L,fmpks[fmpks_pos+1].win);
	}else if( strcmp( args, "线路名称" ) == 0 ){
		lua_pushstring(L,fmpks[fmpks_pos+1].gmsv_name);
	}
	return 1;
}

static int getMemberData(lua_State *L)
{
	size_t l;
	const int fmindex = luaL_checkint(L, 1);
	const char *args = luaL_checklstring(L, 2, &l);
	if (strcmp(args, "总人数") == 0) {
		lua_pushinteger(L, memberlist[fmindex].fmnum);
	} else if (strcmp(args, "加入人数") == 0) {
		lua_pushinteger(L, memberlist[fmindex].fmjoinnum);
	} else if (strcmp(args, "加入标记") == 0) {
		lua_pushstring(L, memberlist[fmindex].accept);
	} else if (strcmp(args, "气势") == 0) {
		lua_pushinteger(L, memberlist[fmindex].memonum);
	} else if (strcmp(args, "气势索引") == 0) {
		lua_pushstring(L, memberlist[fmindex].memoindex);
	} else if (strcmp(args, "徽章") == 0) {
		lua_pushinteger(L, memberlist[fmindex].badge);
	} else if (strcmp(args, "成员索引") == 0) {
		const int index = luaL_checkint(L, 3);
		lua_pushinteger(L, memberlist[fmindex].memberindex[index]);
	} else if (strcmp(args, "成员信息") == 0) {
		const int index = luaL_checkint(L, 3);
		lua_pushstring(L, memberlist[fmindex].numberlistarray[index]);
	}
	return 1;
}
extern int	familyMemberIndex[FAMILY_MAXNUM][FAMILY_MAXMEMBER];

static int getFamilyMemberIndex(lua_State *L)
{
	const int meindex = luaL_checkint(L, 1);
	int fmindexi = CHAR_getWorkInt( meindex, CHAR_WORKFMINDEXI );
	if (fmindexi<0 || fmindexi>999) {
		lua_pushinteger(L, -1);
		return 1;
	}
	int j;
	for( j = 0 ; j < FAMILY_MAXMEMBER ; j++ ) {
		if (meindex == familyMemberIndex[fmindexi][j]){
			lua_pushinteger(L, j);
			break;
		}
	}
	return 1;
}

static int Leave(lua_State *L)
{
	size_t l;
	const int charaindex = luaL_checkint(L, 1);
	const char *message = luaL_checklstring(L, 2, &l);

	FAMILY_Leave(getfdFromCharaIndex(charaindex), charaindex, message);

	return 1;
}

static int getBankMoney(lua_State *L)
{
	size_t l;
	const int talkerindex = luaL_checkint(L, 1);

	saacproto_ACGetFMData_send( acfd, CHAR_getChar( talkerindex, CHAR_FMNAME),
									CHAR_getInt( talkerindex, CHAR_FMINDEX ),
									CHAR_getWorkInt( talkerindex, CHAR_WORKFMINDEXI ),
									1,
									CONNECT_getFdid(getfdFromCharaIndex( talkerindex))
									);

	return 1;
}
#endif
static const luaL_Reg Familylib[] =
{
    { "ShowFamilyList", ShowFamilyList },
    { "ShowFamilyListNum", ShowFamilyListNum },
    {"ShowFamilyPkList",    ShowFamilyPkList},
    {"ShowFamilyPkNum",     ShowFamilyPkNum},
    { "ShowFamilyPkNumTalk", ShowFamilyPkNumTalk },
    {"ShowFamilyPkTime",    ShowFamilyPkTime},
    {"DelFamily",   DelFamily},
    {"ACShowMemberList",    LuaACShowMemberList},
    {"ShowMemberListData",  ShowMemberListData},
    {"ShowMemberListNum",   ShowMemberListNum},
#ifdef _MO_LUA_FMPOINT
    { "ShowPointListArray", ShowPointListArray },
#endif
    { "ShowFamilyPkName", ShowFamilyPkName },
    { "ShowFamilyPkPlayerNum", ShowFamilyPkPlayerNum },
    { "ShowFamilyTopNum", ShowFamilyTopNum },
    { "ShowFamilyTop", ShowFamilyTop },
    { "getFamilyPkTime", getFamilyPkTime },
    { "getFamilyPkHostName", getFamilyPkHostName },
    { "getFamilyPkGuestName", getFamilyPkGuestName },
    { "ShowFmJoinNum", ShowFmJoinNum },
    { "ShowFmRank", ShowFmRank },
    { "ShowFmFame", ShowFmFame },
    { "ShowFmMomnum", ShowFmMomnum },
    { "ShowFmBadge", ShowFmBadge },
#ifdef _FAMILY_QQ
    { "ShowFmQq", ShowFmQq },
    { "ShowFmRule", ShowFmRule },
    { "ShowFmPetName", ShowFmPetName },
    { "ShowFmPetAttr", ShowFmPetAttr },
    { "ShowFmSprite", ShowFmSprite },
#endif
    { "ShowFmAccept", ShowFmAccept },
    { "ACFMReadMemo", LuaACFMReadMemo },
    { "ACFMPointList", LuaACFMPointList },
    { "ACShowTopFMList", LuaACShowTopFMList },
	{ "Family", Family },

#ifdef _NEW_LUA_FAMILY_
	{"GetFamilyBadge", 	GetFamilyBadge},
	{"getMomentum", 	getMomentum},
	{"getFame", 	getFame},

	{"getfamilyNumTotal",getfamilyNumTotal},
	{"getFmPointTime",getFmPointTime},
	{"getFmPointCount",getFmPointCount},
	{"getFmPKData",getFmPKData},
	{"getTopFamilyData",getTopFamilyData},
	{"getFamilyRank",getFamilyRank},
	{"getMemberData",getMemberData},
	{"getFamilyMemberIndex",getFamilyMemberIndex},
	{"Leave",Leave},
	{"getBankMoney",getBankMoney},

#endif
    {NULL,                        NULL}
};

LUALIB_API int luaopen_Family(lua_State *L)
{
    luaL_register(L, "family", Familylib);
    return 1;
}

#endif

