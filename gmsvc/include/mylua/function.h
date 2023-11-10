
#ifndef __MYLUA_FUNCTION_H__
#define __MYLUA_FUNCTION_H__

BOOL RunCharTalkedEvent(int meindex, int toindex, char *messageeraseescape, int color, int channel);
BOOL RunCharLoopEvent(int meindex);
BOOL RunCharOverlapEvent(int meindex, int toindex);
BOOL RunCharBattleOverEvent(int meindex, int toindex, int iswin);
BOOL RunCharWindowTalked(int meindex, int talkerindex, int seqno, int select, char *data);

#ifdef _ALLBLUES_LUA_1_2
BOOL RunItemUseEvent(int itemindex, int charaindex, int toindex, int haveitemindex);
BOOL RunItemDieReLifeEvent(int toindex, int itemindex, int haveitemindex);
BOOL RunItemDetachEvent(int charaindex, int itemindex);
BOOL RunItemAttachEvent(int charaindex, int itemindex);
BOOL RunItemPickupEvent(int charaindex, int itemindex);
BOOL RunItemPostOverEvent(int itemindex, int charaindex);
BOOL RunItemPreOverEvent(int itemindex, int charaindex);
BOOL RunItemDropEvent(int charaindex, int itemindex);
BOOL RunUseChatMagic(int charaindex, char *data, lua_State *lua);
#endif
#ifdef _PETSKILL_SHOP_LUA
BOOL FreePetSkillShop(int talkerindex, int petindex, int oldSkillID, int newSkillID);
#endif
#ifdef _PETSKILL_SHOP_LUA
BOOL OffLineCommand(int battleindex, int charindex, int side);
#endif
#ifdef _ALLBLUES_LUA_1_4
BOOL BattleFinish(int battleindex, int charaindex);
BOOL BattleEscape(int battleindex, int charaindex);
BOOL RunCharLogOutEvent(int charaindex);
#endif
#ifdef _ALLBLUES_LUA_1_5
BOOL FreePartyJoin(int charaindex, int toindex);
BOOL FreeVsPlayer(int charaindex, int toindex);
BOOL FreeCharLogin(int charaindex);
BOOL FreeCharCreate(int charaindex, int hometown, int clifd);
BOOL NetLoopFunction(void);
#endif
#ifdef _ALLBLUES_LUA_1_6
BOOL FamilyRideFunction(int meindex, int petindex, int petid);
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
                     );
#endif
#ifdef _ALLBLUES_LUA_1_7
BOOL CharVsEnemyFunction(int charaindex);
#endif
#ifdef _ALLBLUES_LUA_1_8
BOOL CaptureOkFunction(int attackindex, int defindex);
BOOL CaptureCheckFunction(int attackindex, int defindex);
int CapturePer(int attackindex, int defindex, int per);
#endif
#ifdef _ALLBLUES_LUA_1_9
BOOL SetBattleEnmeyFunction(int meindex, int enemyindex, int id);
BOOL BattleFinishFunction(int charaindex, int battletime, int battleturn, int battletype, char* wincdkey, char* winname, char* lostcdkey, char* lostname);
void FreeCharExpSave(int charindex, int exp);
BOOL BattleCommand(int charaindex, int battleindex);
#endif
#ifdef _GETTANKPET_
BOOL FreeGetTransPet(int charaindex, int petindex);
#endif
#endif
BOOL FreeSetQqData(int charaindex, char* data);
int FreeBattleDuck(int attackindex, int defindex, int per);
#ifdef _OFFLINE_BATTLE_RETEUN
int FreeOfflineBattleTime();
#endif
int FreeBattleCounter(int attackindex, int defindex, int counter);
int FreeBattleCritical(int attackindex, int defindex, int critical);
BOOL FreeMail(int charaindex, int flg);
BOOL FreeStreet(int fd, char* data);
void FreeEchoRecv(int charaindex);
int FreePlayerRideCtrl(int charaindex, int petindex, int workindex, int action, int throwweapon);
int FreePetLevelUpRand(int petindex);
int FreeMergeItem(int charaindex, char* data);
void FreeMergeItemOk(int charaindex);
void FreeMergeItemFalse(int charaindex);
void FreeNewPlayer(int charaindex);
#ifdef _NEWPLAYER_FLG
void FreeNewPlayerFlg(int charaindex, int flg1, int flg2);
#endif
void FreeFmTop(int charaindex);
void FreeSecurity(int charaindex, int type, int codetype, int index, char* codedata);
#ifdef _ADDRESS_ONLINE
int FreeServerId(int serverid);
#endif
#ifdef _NEWUI_MAGICUSE
char* FreeMagicName(int charaindex, int marray);
int FreeBattleItemUse(int charaindex, int toindex, int haveitemindex);
#endif
int FreeMagicUse(int charaindex, int toindex, int magicid, int mp);
void FreePlayerLevelUp(int charaindex);
int FreeShowFmPkWindows(int fd);
void FreeShowPlayerPet(int charaindex, int toindex);
void TeacherExt(int charaindex, int toindex);
void FreeDischargeParty(int charaindex);
void FreeNewSaMenu(int charaindex, int index, char* data);
void FreeSetInt(int charaindex, int datatype, int olddata, int newdata, char* file, int line);
void FreeSetWorkInt(int charaindex, int datatype, int olddata, int newdata, char* file, int line);
int FreeDispatchMessage(int fd, int func);
BOOL FreePlayerTrans(int charaindex);
BOOL FreePlayerTransFlgCls(int charaindex);
void FreeFdError(int fd, int fucnid);
#ifdef _NEW_NETSENDRECV
void FreeCharList(int fd, char* resault, char* data);
#endif
void FreeEvent(int charaindex, int eventid, int eventid2);
void FreeGmRecv(char* buff, char* ip);
char* FreeShowItemEffect(int itemindex);
int FreeBattleComboCheck(int charaindex, int per);
BOOL FreeCharShowType(int charaindex1, int charaindex2);
BOOL FreeBattleHelp(int charaindex, int flg);
void FreeBattleDie(int battleindex, int charaindex, int no);
int getlevelupexp(int charaindex, int level);
BOOL FreeFmSetPoint(int charaindex, int fmpointindex);
int FreeTrade(int meindex, int toindex);
void FreeGmLog(char* gmdata);
int FreeBattleStatus(int attackindex, int defindex, int status, int per);
void FreeBattleRound(int battleindex);
void FreeOtherCommand(int fromid, char* cmdcommand, char* cmddata, char* cmddata2, char* cmddata3);
BOOL CharTellFunction(int charaindex, char *message, int color, char* uid, char* petitemstr);