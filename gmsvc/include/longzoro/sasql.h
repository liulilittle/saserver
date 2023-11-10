#ifndef _SASQL_H_
#define _SASQL_H_

#include "version.h"
#ifdef _SASQL

#define BOOL int
#define FALSE 0
#define TRUE 1

void sasql_close(void);
BOOL sasql_init(void);
int sasql_mysql_query(char *sqlstr);
int sasql_query(char *nm, char *pas
               );
BOOL sasql_online(char *ID, char *NM, char *IP, char *MAC1, char *MAC2, char *MAC3, int flag);
int sasql_queryonline(char *ID);
int sasql_queryoffline(char *ID);
BOOL sasql_register(char *id, char *ps, char *ip, char *uid);
BOOL sasql_chehk_lock(char *idip);
BOOL sasql_add_lock(char *idip);
BOOL sasql_del_lock(char *idip);
#ifdef _ITEM_PET_LOCKED
char *sasql_ItemPetLocked(char *id, char *safepasswd);
char *sasql_ItemPetLocked_Passwd(char *id, char *safepasswd);
int sasql_ItemPetLocked_Char(char *id, char *safepasswd);
char* sasql_getItemPetLocked(char *id);
#endif
int sasql_getVipPoint(int charaindex);
int sasql_setVipPoint(int charaindex, int point);
int sasql_getVipPointForCdkey(char *id);
int sasql_setVipPointForCdkey(char *id, int point);
BOOL sasql_addPauctionInfo(char *cdkey, char *name, char *effect, int cost, int type, char *info, char *string);
int sasql_getPauctionList(char *List, int start, int type, char *cdkey);
BOOL sasql_getPauctionSurvey(char *token, int id);
int sasql_getMyPauction(char *token, int id, char *cdkey);
BOOL sasql_delPauctionBuy(int id, char *cdkey);
int sasql_PauctionOK(int id, char *cdkey);

int sasql_getPayPoint(int charaindex);
int sasql_setPayPoint(int charaindex, int point);
int sasql_getPayPointForCdkey(char *id);
int sasql_setPayPointForCdkey(char *id, int point);

int sasql_getRmbPoint(int charaindex);
int sasql_setRmbPoint(int charaindex, int point);
int sasql_getRmbPointForCdkey(char *id);
int sasql_setRmbPointForCdkey(char *id, int point);
int sasql_getPetPoint(int charaindex);
int sasql_setPetPoint(int charaindex, int point);
int sasql_getPetPointForCdkey(char *id);
int sasql_setPetPointForCdkey(char *id, int point);
void sasql_updateCharLockMac(int charaindex, char *cdkey);
void sasql_updateFmPointData(int fmid, int fmtime, int fmnum, int flg);
int sasql_queryFmPointData(int fmid, int flg);
#ifdef _RED_MEMOY_
int sasql_queryRedMoneyData(char* cdkey, int flg);
void sasql_insertRedMoneyData(char* cdkey, int type, int num, int total);
void sasql_deleteRedMoneyData(char* cdkey);
#endif
#ifdef _UNIQUE_NAME
int sasql_isNameExist(char *charname);
#endif
#ifdef _UPDATESERVERPLAY_
void sasql_updateServerStatus(int id);
#endif
char *sasql_getQqData(char *id);
void sasql_setQqData(char *id, char *qqdata);
char *sasql_getMyCdkeyPasswd(char *id, char *token);
char *sasql_get_cdkey(int id);
int sasql_set_cdkey(int serverid, char *cdkey);
int sasql_getCostPoint(int charaindex);
int sasql_setCostPoint(int charaindex, int point);
int sasql_getPayTotalPoint(int charaindex);
#ifdef _PLAYER_QQFLG
char *sasql_get_qq(int charaindex);
#endif
enum
{
    INSERT,
    SELECT,
    UPDATE,
    DELETE,
};

#endif
#endif



