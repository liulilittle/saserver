#include <string.h>
#include "char.h"
#include "battle.h"
#include "object.h"
#include "char_base.h"
#include "enemy.h"
#include "mylua/base.h"
#include "npcutil.h"
#include "readmap.h"
#include "log.h"
#include "pet.h"
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
#include "longzoro/version.h"
#include "handletime.h"
#include "item_event.h"
#include "lssproto_serv.h"
#ifdef _ALLBLUES_LUA
#include "mylua/mylua.h"
extern MY_Lua MYLua;
static CharBase CharBaseWorkInt[] =
{
    {{"对象"},                        CHAR_WORKOBJINDEX}
#ifdef _FONT_SIZE
    , {{"字号"},                   CHAR_WORKFONTSIZE}
#endif
    , { { "AI" }, CHAR_WORKFIXAI }
    , {{"战斗"},                   CHAR_WORKBATTLEMODE}
    , {{"组队"},                   CHAR_WORKPARTYMODE}
    , {{"战斗索引"},             CHAR_WORKBATTLEINDEX}
    , {{"NPC临时1"},           CHAR_NPCWORKINT1}
    , {{"NPC临时2"},             CHAR_NPCWORKINT2}
    , {{"NPC临时3"},             CHAR_NPCWORKINT3}
    , {{"NPC临时4"},             CHAR_NPCWORKINT4}
    , {{"NPC临时5"},             CHAR_NPCWORKINT5}
    , {{"NPC临时6"},             CHAR_NPCWORKINT6}
    , {{"NPC临时7"},             CHAR_NPCWORKINT7}
    , {{"NPC临时8"},             CHAR_NPCWORKINT8}
    , {{"NPC临时9"},             CHAR_NPCWORKINT9}
    , {{"NPC临时10"},            CHAR_NPCWORKINT10}
    , {{"NPC临时11"},            CHAR_NPCWORKINT11}
    , {{"NPC临时12"},          CHAR_NPCWORKINT12}
    , {{"NPC临时13"},          CHAR_NPCWORKINT13}
    , { { "NPC临时14" }, CHAR_NPCWORKINT14 }
    , { { "NPC临时15" }, CHAR_NPCWORKINT15 }
    , { { "NPC临时16" }, CHAR_NPCWORKINT16 }
    , { { "NPC临时17" }, CHAR_NPCWORKINT17 }
    , { { "NPC临时18" }, CHAR_NPCWORKINT18 }
    , { { "NPC临时19" }, CHAR_NPCWORKINT19 }
    , { { "NPC临时20" }, CHAR_NPCWORKINT20 }
    , {{"捡起模式"},             CHAR_WORKPETFOLLOWMODE}
    , {{"队员1"},                    CHAR_WORKPARTYINDEX1}
    , {{"队员2"},                    CHAR_WORKPARTYINDEX2}
    , {{"队员3"},                    CHAR_WORKPARTYINDEX3}
    , {{"队员4"},                  CHAR_WORKPARTYINDEX4}
    , {{"队员5"},                  CHAR_WORKPARTYINDEX5}
#ifdef _AIRCRAFT_PLUS_
    , {{"队员6"},                  CHAR_WORKPARTYINDEX6}
    , {{"队员7"},                  CHAR_WORKPARTYINDEX7}
    , {{"队员8"},                  CHAR_WORKPARTYINDEX8}
#endif
    , {{"最大HP"},                 CHAR_WORKMAXHP}
    , {{"最大MP"},                 CHAR_WORKMAXMP}
    , {{"攻击力"},                    CHAR_WORKATTACKPOWER}
    , {{"防御力"},                    CHAR_WORKDEFENCEPOWER}
    , {{"敏捷力"},                    CHAR_WORKQUICK}
    , {{"修正体力"},             CHAR_WORKFIXVITAL}
    , {{"修正腕力"},             CHAR_WORKFIXSTR}
    , {{"修正耐力"},             CHAR_WORKFIXTOUGH}
    , {{"修正速度"},             CHAR_WORKFIXDEX}
    , {{"家族地图"},             CHAR_WORKFMFLOOR}
    , {{"攻击"},                       CHAR_WORKATTACKPOWER}
    , {{"防御"},                       CHAR_WORKDEFENCEPOWER}
    , {{"敏捷"},                       CHAR_WORKQUICK}
    , {{"逃跑"},                       CHAR_WORKDBATTLEESCAPE}
    , {{"经验时间"},             CHAR_WORKITEM_ADDEXPTIME}
    , {{"经验加成"},             CHAR_WORKITEM_ADDEXP}
#ifdef _OFFLINE_SYSTEM
    , {{"离线"},                   CHAR_WORK_OFFLINE}
#endif
    , {{"获得经验"},             CHAR_WORKGETEXP}

#ifdef _AUTO_PK
    , {{"自动PK点"},              CHAR_WORK_AUTOPK}
    , {{"自动PK死亡"},           CHAR_WORK_AUTOPK_DEAD}
    , {{"自动PK对手"},           CHAR_WORK_AUTOPK_DEFINDEX}
#endif
#ifdef _SPECIAL_COUNTER
    , {{"计数器"},                CHAR_WORK_COUNTER}
#endif
#ifdef _SPECIAL_TIMER
    , {{"计时器"},                CHAR_WORK_TIMER}
#endif
#ifdef _STREET_VENDOR
    , {{"摆摊"},                   CHAR_WORKSTREETVENDOR}
#endif
#ifdef _RECORD_IP
    , {{"IP"},                       CHAR_WORK_RECORD_IP}
#endif
#ifdef _ONLINE_TIME
    , {{"登陆时间"},             CHAR_WORK_LOGIN_TIME}
#endif
#ifdef _NEWCLISETSERVID
    , {{"服务器ID"},          CHAR_WORK_SERVID}
#endif
    , {{"PK时间"},             CHAR_WORKDBATTLETIME}
    , {{"使用道具索引"},                       CHAR_WORKUSEINDEX}
    , {{"被使用道具索引"},                    CHAR_WORKTOPPINDEX}
    , {{"战斗模式"},                         CHAR_WORKBATTLEFLAG}
    , {{"族战标识"},                         CHAR_WORKFMPKFLAG}
    , {{"忠诚"},                       CHAR_WORKFIXAI}
#ifdef _NPC_EVENT_NOTICE
    , {{"NOTICE"},               CHAR_WORK_NOTICE}
#endif
    , {{"毒抗性"},            CHAR_WORKMODPOISON}
    , {{"麻抗性"},            CHAR_WORKMODPARALYSIS}
    , {{"睡抗性"},            CHAR_WORKMODSLEEP}
    , {{"石抗性"},            CHAR_WORKMODSTONE}
    , {{"酒抗性"},            CHAR_WORKMODDRUNK}
    , {{"混抗性"},            CHAR_WORKMODCONFUSION}
    , {{"虚抗性"},            CHAR_WORKMODWEAKEN}
    , {{"剧抗性"},            CHAR_WORKMODDEEPPOISON}
    , {{"障抗性"},            CHAR_WORKMODBARRIER}
    , {{"默抗性"},            CHAR_WORKMODNOCAST}
#ifdef _LUA_RESIST
    , {{"毒抗附加"},         CHAR_WORK_RESIST_POISON}
    , {{"麻抗附加"},         CHAR_WORK_RESIST_PARALYSIS}
    , {{"睡抗附加"},         CHAR_WORK_RESIST_SLEEP}
    , {{"石抗附加"},         CHAR_WORK_RESIST_STONE}
    , {{"酒抗附加"},         CHAR_WORK_RESIST_DRUNK}
    , {{"混抗附加"},         CHAR_WORK_RESIST_CONFUSION}
    , {{"虚抗附加"},         CHAR_WORK_RESIST_WEAKEN}
    , {{"剧抗附加"},         CHAR_WORK_RESIST_DEEPPOISON}
    , {{"障抗附加"},         CHAR_WORK_RESIST_BARRIER}
    , {{"默抗附加"},         CHAR_WORK_RESIST_NOCAST}
#endif
#ifdef  _FMBATTLE_POINT
    , {{"族战积分标识"},   CHAR_WORKFMBATTLETYPE}
#endif
#ifdef  _NEW_TEACHER_SYSTEM
    , {{"新师徒临时索引"},    CHAR_WORK_NEWTEACHER_INDEX}
#endif
    , {{"族战标志"},                     CHAR_WORKFMPKFLAG}
    , { { "族战战标" }, CHAR_WORKBATTLEFLAG }
    , { { "传送标识" }, CHAR_WORKWARPCHECK }
    , {{"家族临时索引"},   CHAR_WORKFMINDEXI}
    , {{"段位临时"}, CHAR_WORK_DWTEMP}
    , {{"宠物主人索引"},   CHAR_WORKPLAYERINDEX}
#ifdef _IPHONEADDJIKOU_
    , {{"手机资料标志"},                   CHAR_WORK_IPHONE_INFO}
#endif
#ifdef _IPHONE_SHOP_FLAG_
    , {{"苹果商城标志"},                   CHAR_WORK_IPHONE_FLAG}
#endif
#ifdef _SPEED_CHECK_
    , { { "加速计次" }, CHAR_WORK_CHECK_SPEED_NUM }
#endif
#ifdef FAMILY_MANOR_
    , { { "庄园特效" }, CHAR_PLAYER_EFFECT_MANOR }
#endif
#ifdef _PLAYER_EFFECT
    , { { "人物效果" }, CHAR_PLAYER_EFFECT }
#endif
    , { { "商店变量" }, CHAR_WORKSHOPRELEVANT }
    , { { "战斗特效" }, CHAR_WORKBATTLEEFFECTS }
    , { { "安全模式" }, CHAR_WORKSAFEMODE }
#ifdef _YAYA_VOICE
    , { { "语音模式" }, CHAR_WORKYAYAVOICE }
#endif
    , { { "附加伤害" }, CHAR_WORKDAMAGE }
    , { { "增加堆叠数" }, CHAR_WORKATTACHPILE }
    , { { "心跳时间" }, CHAR_WORKECHOTIME }
    , { { "心跳次数" }, CHAR_WORKECHONUM }
    , { { "超时回合数" }, CHAR_WORKBATTLEWAITCNT }
    , { { "交易状态" }, CHAR_WORKTRADEMODE }
    , { { "自动组队" }, CHAR_WORKAUTOPARTY }
#ifdef _PLAYER_QUESTION
    , { { "玩家答题" }, CHAR_WORKPLAYERQUESTION }
#endif
    , { { "皮肤索引" }, CHAR_WORKSKININDEX }
    , { { "皮肤时间" }, CHAR_WORKSKINTIME }
    , { { "驯兽模式" }, CHAR_PickAllPet }
    , { { "复活动画" }, CHAR_WORK_RELIFE }
    , { { "战斗边" }, CHAR_WORKBATTLESIDE }
    , { { "战斗状态标识" }, CHAR_WORKBATTLEFLG }
    , { { "骑证时间" }, CHAR_WORKRIDETIME }
    , { { "EVENT类型" }, CHAR_WORKEVENTTYPE }
    , { { "掉线时间" }, CHAR_WORKKICKTIME }
#ifdef _SMART_ENEMY_LUA
    , { {"狂暴标记"},           CHAR_WORK_CRAZYFLG }
    , { {"狂暴计数"},           CHAR_WORK_CRAZYCNT }
    , { {"狂暴回合"},           CHAR_WORK_CRAZYROUND }
#endif
#ifdef _TRUE_DAMAGE_MAGIC
    , { {"真实伤害"},           CHAR_WORK_DAMGEFLG }
#endif
    , { {"战斗接口1"},          CHAR_WORKBATTLECOM1 }
    , { {"战斗接口2"},          CHAR_WORKBATTLECOM2 }
    , { {"战斗接口3"},          CHAR_WORKBATTLECOM3 }
    , { {"战斗接口4"},          CHAR_WORKBATTLECOM4 }
#ifdef _PROFESSION_SKILL            // WON ADD 人物职业技能
    , { {"火抗性"},          CHAR_WORK_F_RESIST }
    , { {"冰抗性"},          CHAR_WORK_I_RESIST }
    , { {"电抗性"},          CHAR_WORK_T_RESIST }
#endif
    , { {"族长换位索引"},          CHAR_WORKLEADERCHANGE }
#ifdef _VARY_WOLF_JOINBATTLE
    , { {"宠物临时数据"},          CHAR_WORKPETDATA }
#endif
};

static CharBase CharBaseWorkChar[] =
{
    {{"NPC临时1"},                CHAR_NPCWORKCHAR1}
    , {{"NPC临时2"},             CHAR_NPCWORKCHAR2}
    , {{"NPC临时3"},             CHAR_NPCWORKCHAR3}
    , {{"NPC临时4"},             CHAR_NPCWORKCHAR4}
    , {{"NPC临时5"},             CHAR_NPCWORKCHAR5}
    , {{"NPC临时6"},             CHAR_NPCWORKCHAR6}
#ifdef _STREET_VENDOR
    , {{"摆摊"},                       CHAR_STREETVENDOR_NAME}
#endif
#ifdef _NEWCLISETMAC
    , {{"MAC"},                      CHAR_WORK_MAC}
    , {{"MAC2"},                         CHAR_WORK_MAC2}
    , {{"MAC3"},                         CHAR_WORK_MAC3}
#endif
#ifdef _ITEM_MMEXP
    , { { "MM道具" }, CHAR_WORK_MMEXPITEM }
#endif
};

static CharBase CharBaseValue[] =
{
    {{"北"},                   0}
    , {{"东北"},           1}
    , {{"东"},              2}
    , {{"东南"},           3}
    , {{"南"},              4}
    , {{"西南"},           5}
    , {{"西"},              6}
    , {{"西北"},           7}
    , {{"无类型"},        CHAR_TYPENONE}
    , {{"玩家"},           CHAR_TYPEPLAYER}
    , {{"敌人"},           CHAR_TYPEENEMY}
    , {{"宠物"},           CHAR_TYPEPET}
#ifdef _ALLBLUES_LUA
    , {{"LUA"},          CHAR_TYPELUANPC}
#endif
#ifdef _PLAYER_NPC
    , {{"帮托"},           CHAR_TYPEPLAYERNPC}
    , {{"帮宠"},           CHAR_TYPEPLAYERPETNPC}
#endif
};

static CharBase CharBaseWorkValue[] =
{
    {{"无"},                                   CHAR_PARTY_NONE}
    , {{"队长"},                           CHAR_PARTY_LEADER}
    , {{"队员"},                           CHAR_PARTY_CLIENT}
    , {{"无战斗"},                        BATTLE_CHARMODE_NONE}
    , {{"战斗初始化"},              BATTLE_CHARMODE_INIT}
    , {{"等待战斗"},                     BATTLE_CHARMODE_C_WAIT}
    , {{"确认战斗"},                     BATTLE_CHARMODE_C_OK}
    , {{"帮助战斗"},                     BATTLE_CHARMODE_RESCUE}
    , {{"结束战斗"},                     BATTLE_CHARMODE_FINAL}
    , {{"观战初始化"},              BATTLE_CHARMODE_WATCHINIT}
    , {{"没有家族"},                     FMMEMBER_NONE}
    , {{"申请入族"},                   FMMEMBER_APPLY}
    , {{"族长"},                             FMMEMBER_LEADER}
    , {{"成员"},                             FMMEMBER_MEMBER}
    , {{"长老"},                             FMMEMBER_ELDER}
};

static CharBase CharBaseInt[] =
{
    {{"图像号"},             CHAR_BASEIMAGENUMBER}
    , {{"原图像号"},     CHAR_BASEBASEIMAGENUMBER}
    , {{"头像号"},    CHAR_FACEIMAGENUMBER}
    , {{"存档"},               CHAR_SAVEINDEXNUMBER}
    , {{"地图号"},            CHAR_FLOOR}
    , {{"坐标X"},          CHAR_X}
    , {{"坐标Y"},          CHAR_Y}
    , {{"方向"},           CHAR_DIR}
    , {{"等级"},           CHAR_LV}
    , {{"石币"},           CHAR_GOLD}
    , {{"HP"},               CHAR_HP}
    , {{"MP"},               CHAR_MP}
    , {{"最大MP"},         CHAR_MAXMP}
    , {{"体力"},           CHAR_VITAL}
    , {{"腕力"},           CHAR_STR}
    , {{"耐力"},               CHAR_TOUGH}
    , {{"速度"},               CHAR_DEX}
    , {{"地"},                  CHAR_EARTHAT}
    , {{"水"},                  CHAR_WATERAT}
    , {{"火"},              CHAR_FIREAT}
    , {{"风"},              CHAR_WINDAT}
    , {{"战宠"},           CHAR_DEFAULTPET}
    , {{"魅力"},           CHAR_CHARM}
    , {{"运气"},           CHAR_LUCK}
    , {{"死亡次数"},     CHAR_DEADCOUNT}
    , {{"走路步数"},     CHAR_WALKCOUNT}
    , {{"说话次数"},     CHAR_TALKCOUNT}
    , {{"损坏次数"},     CHAR_DAMAGECOUNT}
    , {{"给宠次数"},     CHAR_GETPETCOUNT}
    , {{"杀宠次数"},     CHAR_KILLPETCOUNT}
    , {{"死宠次数"},     CHAR_DEADPETCOUNT}
    , {{"发邮件数"},     CHAR_SENDMAILCOUNT}
    , {{"合成次数"},     CHAR_MERGEITEMCOUNT}
    , {{"PK次数"},         CHAR_DUELBATTLECOUNT}
    , {{"PK赢数"},         CHAR_DUELWINCOUNT}
    , {{"PK败数"},         CHAR_DUELLOSECOUNT}
    , {{"PK连胜"},         CHAR_DUELSTWINCOUNT}
    , {{"PK连败"},         CHAR_DUELSTLOSECOUNT}
    , {{"PK最高连胜"},   CHAR_DUELMAXSTWINCOUNT}
    , {{"类型"},           CHAR_WHICHTYPE}
    , { { "抓捕等级" }, CHAR_PETGETLV }
    , {{"循环事件时间"},   CHAR_LOOPINTERVAL}
#ifdef _NEWOPEN_MAXEXP
    , {{"总经验"},        CHAR_OLDEXP}
#endif
    , {{"当前经验"},     CHAR_EXP}
    , {{"技能点"},        CHAR_SKILLUPPOINT}
    , {{"升级点"},        CHAR_LEVELUPPOINT}
    , {{"运气时间"},     CHAR_LASTTIMESETLUCK}
    , {{"DP"},               CHAR_DUELPOINT}
    , {{"经验"},           CHAR_EXP}
    , {{"出生地"},        CHAR_LASTTALKELDER}
    , {{"转数"},           CHAR_TRANSMIGRATION}
    , {{"禁言"},           CHAR_SILENT}
    , {{"家族索引"},     CHAR_FMINDEX}
    , {{"家族地位"},     CHAR_FMLEADERFLAG}
    , {{"家族类型"},     CHAR_FMSPRITE}
#ifdef _NEW_RIDEPETS
    , {{"证书骑宠"},     CHAR_LOWRIDEPETS}
#endif
#ifdef _RIDE_CF
    , {{"证书骑宠1"},    CHAR_LOWRIDEPETS1}
#ifdef _ADD_RIDE_CF
    , {{"证书骑宠2"},    CHAR_HIGHRIDEPET2}
#endif
#endif
    , {{"个人银行"},     CHAR_BANKGOLD}
    , {{"骑宠"},           CHAR_RIDEPET}
    , {{"暴击"},           CHAR_CRITIAL}
    , {{"反击"},           CHAR_COUNTER}
    , {{"下线时间"},     CHAR_LASTLEAVETIME}
#ifdef _GAMBLE_BANK
//  ,{{"赌场银行"},     CHAR_RIDEPET}
#endif
#ifdef _DROPSTAKENEW
    , {{"赌场积分"},     CHAR_GAMBLENUM}
#endif
#ifdef _PERSONAL_FAME   // Arminius 8.30: 家族个人声望
    , {{"声望"},           CHAR_FAME}
#endif
#ifdef _ATTACK_EFFECT
    , { { "攻击特效" }, CHAR_ATTACK_EFFECT }
#endif
#ifdef _ATTACK_MAGIC
    , {{"地魔法熟练度"},           CHAR_EARTH_EXP}
    , {{"水魔法熟练度"},           CHAR_WATER_EXP}
    , {{"火魔法熟练度"},           CHAR_FIRE_EXP}
    , {{"风魔法熟练度"},           CHAR_WIND_EXP}
    , {{"地魔法抗性"},              CHAR_EARTH_RESIST}
    , {{"水魔法抗性"},              CHAR_WATER_RESIST}
    , {{"火魔法抗性"},              CHAR_FIRE_RESIST}
    , {{"风魔法抗性"},              CHAR_WIND_RESIST}
    , {{"地魔法熟练度经验"},     CHAR_EARTH_ATTMAGIC_EXP}
    , {{"水魔法熟练度经验"},     CHAR_WATER_ATTMAGIC_EXP}
    , {{"火魔法熟练度经验"},     CHAR_FIRE_ATTMAGIC_EXP}
    , {{"风魔法熟练度经验"},     CHAR_WIND_ATTMAGIC_EXP}
    , {{"地魔法抗性经验值"},     CHAR_EARTH_DEFMAGIC_EXP}
    , {{"水魔法抗性经验值"},     CHAR_WATER_DEFMAGIC_EXP}
    , {{"火魔法抗性经验值"},     CHAR_FIRE_DEFMAGIC_EXP}
    , {{"风魔法抗性经验值"},     CHAR_WIND_DEFMAGIC_EXP}
#endif
#ifdef _CHAR_PROFESSION         // WON ADD 人物职业
    , {{"职业类别"},    PROFESSION_CLASS}
    , {{"职业等级"},     PROFESSION_LEVEL}
    , { {"就职次数"},     PROFESSION_CNT }
    , {{"技能点数"},     PROFESSION_SKILL_POINT}
    , {{"增加堆叠"},     ATTACHPILE}
    , {{"火熟练度"},     PROFESSION_FIRE_P}
    , {{"冰熟练度"},     PROFESSION_ICE_P}
    , {{"雷熟练度"},     PROFESSION_THUNDER_P}
    , {{"火抗性"},    PROFESSION_FIRE_R}
    , {{"冰抗性"},    PROFESSION_ICE_R}
    , {{"雷抗性"},    PROFESSION_THUNDER_R}
#endif
#ifdef _NEW_MANOR_LAW
    , {{"气势"},       CHAR_MOMENTUM}
#endif
#ifdef _VIP_SERVER
    , {{"积分"},       CHAR_AMPOINT}
#endif
#ifdef _VIP_RIDE
    , {{"会员"},           CHAR_VIPRIDE}
    , {{"会员有效期"}, CHAR_VIPTIME}
#endif
#ifdef _ITEM_PET_LOCKED
    , {{"安全锁"},        CHAR_LOCKED}
#endif
#ifdef _ONLINE_SHOP
    , {{"贝币"},           CHAR_BJ}
#endif
    , {{"模式AI"},     CHAR_MODAI}
    , {{"可变AI"},     CHAR_VARIABLEAI}
    , {{"宠技位"},    CHAR_SLOT}
    , {{"宠ID"},        CHAR_PETID}
    , {{"守护兽"},    CHAR_PETFAMILY}
    , {{"能力值"},    CHAR_ALLOCPOINT}
    , {{"成长区间"}, CHAR_PETRANK}
    , {{"限制等级"}, CHAR_LIMITLEVEL}
#ifdef _PET_BEATITUDE
    , {{"提升值"},    CHAR_BEATITUDE}
#endif
#ifdef _CAMEO_MONEY
    , {{"贝壳"},       CHAR_CAMEO}
#endif
#ifdef _MISSION_TRAIN
    , {{"任务链"},    CHAR_MISSIONTRAIN_NUM}
    , {{"任务链时间"},  CHAR_MISSIONTRAIN_TIME}
#endif
#ifdef _PET_EVOLVE
    , {{"进化"},       CHAR_EVOLVE}
#endif
#ifdef _ONLINE_TIME
    , {{"在线时间"},         CHAR_ONLINE_TIME}
#endif

#ifdef _FLOOR_DIFI_FIELD
    , {{"楼层时间"},         CHAR_FLOORDIFI_TIME}
#endif
#ifdef _ACTIVE_GAME
    , {{"活力"},       CHAR_ACTIVE}
#endif
#ifdef _SUPER
    , {{"极品"},       CHAR_SUPER}
#endif
#ifdef _ALLDOMAN
    , {{"英雄称号"},         CHAR_HEROFLOOR}
#endif
#ifdef _MISSION_TIME
    , {{"任务计时"},         CHAR_MISSION_TIME}
#endif
#ifdef _PET_FUSION
    , {{"融合宠"},        CHAR_FUSIONBEIT}
    , {{"融合蛋"},        CHAR_FUSIONRAISE}
#endif
#ifdef _REGISTRATION_TIME
    , {{"签到FLG"},        CHAR_REGISTRATION_FLG}
    , {{"签到时间"},         CHAR_REGISTRATION_TIME}
    , {{"签到次数"},         CHAR_REGISTRATION_COUNT}
    , {{"签到在线时间"}, CHAR_REGISTRATION_ONLINE_TIME}
#endif
#ifdef _PETCOM_
    , { { "初体力" }, CHAR_YHP }
    , { { "初攻击" }, CHAR_YATK }
    , { { "初防御" }, CHAR_YDEF }
    , { { "初速度" }, CHAR_YQUICK }
    , { { "初等级" }, CHAR_YLV }
#endif
#ifdef _SPECIAL_COUNTER
    , {{"计数器"},        CHAR_SPCOUNTER}
#endif
#ifdef _SPECIAL_TIMER
    , {{"计时器"},        CHAR_SPTIMER}
#endif
#ifdef _OFFLINE_TIME
    , {{"离线时间"},     CHAR_OFFLINE_TIME}
#endif
#ifdef _FAME_REG_TIME
    , {{"家族签到时间"},   CHAR_FAME_REG_TIME}
#endif
#ifdef _PET_VALIDITY
    , {{"宠物有效期"},      CHAR_PETVALIDITY}
#endif
    , {{"刷楼积分"},     CHAR_SHUALOU_POINT}
#ifdef  _NEW_MAP_NO_FAME
    , {{"无声望模式"},      CHAR_NOFAME}
#endif
    , {{"世界日期次数"},       CHAR_DATEWORLDCNT}
    , {{"世界时间"},     CHAR_WORLDTIME}
    , {{"骑宠名字模式"},       CHAR_RIDENAMEFLG}
    , {{"活动积分"},     CHAR_HDNUM}
    , {{"伤害积分"},     CHAR_SHNUM}
    , {{"活动日期"},     CHAR_HDDATE}
#ifdef  _ENEMY_MIN_ADD
    , {{"遇敌几率时间"},       CHAR_ENEMY_MIN_TIME}
    , {{"遇敌几率倍数"},       CHAR_ENEMY_MIN_BASE}
#endif
#ifdef  _FMBATTLE_POINT
    , {{"族战积分"},     CHAR_FMBATTLE_POINT}
#endif
#ifdef _CHAR_TITLE_STR_
    , { { "称号状态1" }, CHAR_TITLE1 }
    , { { "称号状态2" }, CHAR_TITLE2 }
    , { { "称号状态3" }, CHAR_TITLE3 }
    , {{"文字称号"},     CHAR_TITLE_}
    , { { "称号类型" }, CHAR_TITLE_TYPE }
#endif
#ifdef _CHAR_MANOR_
    , {{"人物光环"},     CHAR_TRUMP_EFFECT}
#endif
#ifdef _CHAR_TITLE_NEW_
    , {{"新文字称号"},      CHAR_TITLE_NEW}
#endif
#ifdef  _NEW_TEACHER_SYSTEM
    , {{"新师徒点数"},  CHAR_NEW_TEACHER_POINT}
#endif
    , {{"补偿日期"}, CHAR_BUCHANG_DATE}
    , {{"段位积分"}, CHAR_DWPOINT}
    , {{"段位模式"}, CHAR_DWTYPE}
    , {{"段位时间"}, CHAR_DWTIME}
#ifdef _ADDLOGINTIME_
    , {{"登陆新时间"},      CHAR_LOGIN_TIME}
    , {{"登陆新日期"},      CHAR_LOGIN_DATA}
#endif
#ifdef _PAIPAISONG_
    , {{"象卷数量"},         CHAR_PPSNUM}
    , {{"象卷幸运值"},      CHAR_PPSLUCKY}
#endif
#ifdef _NEWPAIHANG_
    , { { "132称号" }, CHAR_132TITLE_FLG }
#endif
#ifdef _PETCOM_
    , { { "预测血量" }, CHAR_YHP }
    , { { "预测攻击" }, CHAR_YATK }
    , { { "预测防御" }, CHAR_YDEF }
    , { { "预测敏捷" }, CHAR_YQUICK }
    , { { "预测等级" }, CHAR_YLV }
#endif
    , { { "人物显示颜色" }, CHAR_POPUPNAMECOLOR }
    , { { "经验时间" }, CHAR_ADDEXPTIME }
    , { { "经验加成" }, CHAR_ADDEXPPOWER }
#ifdef _BUY_VIGORDATA
    , { { "购买活力数据" }, CHAR_VIGORDATA }
#endif
    , { { "骑宠等级" }, CHAR_LEARNRIDE }
#ifdef _NEWPLAYER_FLG
    , { { "新玩家旗标" }, CHAR_NEWPLAYERFLG }
#endif
    , { { "签到双倍时间" }, CHAR_DAYITEMEXPTIME }
#ifdef _PARTY_HIGH
    , { { "组队增强模式" }, CHAR_PARTYTYPE }
#endif
#ifdef _OFFLINE_EXP
    , { { "离线经验" }, CHAR_OFFLINE_EXP }
#endif // _OFFLINE_EXP

#ifdef _U8_QUESTION
    , { { "U8问卷调查" }, CHAR_U8QUESTION }
#endif // _U8_QUESTION
    , { { "完成任务旗标1" }, CHAR_ENDEVENT }
    , { { "完成任务旗标2" }, CHAR_ENDEVENT2 }
    , { { "完成任务旗标3" }, CHAR_ENDEVENT3 }
    , { { "完成任务旗标4" }, CHAR_ENDEVENT4 }
    , { { "完成任务旗标5" }, CHAR_ENDEVENT5 }
    , { { "完成任务旗标6" }, CHAR_ENDEVENT6 }
    , { { "完成任务旗标7" }, CHAR_ENDEVENT7 }
    , { { "完成任务旗标8" }, CHAR_ENDEVENT8 }
    , { { "完成任务旗标9" }, CHAR_ENDEVENT9 }
    , { { "完成任务旗标10" }, CHAR_ENDEVENT10 }
    , { { "完成任务旗标11" }, CHAR_ENDEVENT11 }
    , { { "完成任务旗标12" }, CHAR_ENDEVENT12 }
    , { { "完成任务旗标13" }, CHAR_ENDEVENT13 }
    , { { "完成任务旗标14" }, CHAR_ENDEVENT14 }
    , { { "完成任务旗标15" }, CHAR_ENDEVENT15 }
    , { { "完成任务旗标16" }, CHAR_ENDEVENT16 }
    , { { "完成任务旗标17" }, CHAR_ENDEVENT17 }
    , { { "完成任务旗标18" }, CHAR_ENDEVENT18 }
    , { { "完成任务旗标19" }, CHAR_ENDEVENT19 }
    , { { "完成任务旗标20" }, CHAR_ENDEVENT20 }
    , { { "完成任务旗标21" }, CHAR_ENDEVENT21 }
    , { { "完成任务旗标22" }, CHAR_ENDEVENT22 }
    , { { "完成任务旗标23" }, CHAR_ENDEVENT23 }
    , { { "完成任务旗标24" }, CHAR_ENDEVENT24 }
    , { { "完成任务旗标25" }, CHAR_ENDEVENT25 }
    , { { "完成任务旗标26" }, CHAR_ENDEVENT26 }
    , { { "完成任务旗标27" }, CHAR_ENDEVENT27 }
    , { { "完成任务旗标28" }, CHAR_ENDEVENT28 }
    , { { "完成任务旗标29" }, CHAR_ENDEVENT29 }
    , { { "完成任务旗标30" }, CHAR_ENDEVENT30 }
    , { { "完成任务旗标31" }, CHAR_ENDEVENT31 }
    , { { "完成任务旗标32" }, CHAR_ENDEVENT32 }

    , { { "进行任务旗标1" }, CHAR_NOWEVENT }
    , { { "进行任务旗标2" }, CHAR_NOWEVENT2 }
    , { { "进行任务旗标3" }, CHAR_NOWEVENT3 }
    , { { "进行任务旗标4" }, CHAR_NOWEVENT4 }
    , { { "进行任务旗标5" }, CHAR_NOWEVENT5 }
    , { { "进行任务旗标6" }, CHAR_NOWEVENT6 }
    , { { "进行任务旗标7" }, CHAR_NOWEVENT7 }
    , { { "进行任务旗标8" }, CHAR_NOWEVENT8 }
    , { { "进行任务旗标9" }, CHAR_NOWEVENT9 }
    , { { "进行任务旗标10" }, CHAR_NOWEVENT10 }
    , { { "进行任务旗标11" }, CHAR_NOWEVENT11 }
    , { { "进行任务旗标12" }, CHAR_NOWEVENT12 }
    , { { "进行任务旗标13" }, CHAR_NOWEVENT13 }
    , { { "进行任务旗标14" }, CHAR_NOWEVENT14 }
    , { { "进行任务旗标15" }, CHAR_NOWEVENT15 }
    , { { "进行任务旗标16" }, CHAR_NOWEVENT16 }
    , { { "进行任务旗标17" }, CHAR_NOWEVENT17 }
    , { { "进行任务旗标18" }, CHAR_NOWEVENT18 }
    , { { "进行任务旗标19" }, CHAR_NOWEVENT19 }
    , { { "进行任务旗标20" }, CHAR_NOWEVENT20 }
    , { { "进行任务旗标21" }, CHAR_NOWEVENT21 }
    , { { "进行任务旗标22" }, CHAR_NOWEVENT22 }
    , { { "进行任务旗标23" }, CHAR_NOWEVENT23 }
    , { { "进行任务旗标24" }, CHAR_NOWEVENT24 }
    , { { "进行任务旗标25" }, CHAR_NOWEVENT25 }
    , { { "进行任务旗标26" }, CHAR_NOWEVENT26 }
    , { { "进行任务旗标27" }, CHAR_NOWEVENT27 }
    , { { "进行任务旗标28" }, CHAR_NOWEVENT28 }
    , { { "进行任务旗标29" }, CHAR_NOWEVENT29 }
    , { { "进行任务旗标30" }, CHAR_NOWEVENT30 }
    , { { "进行任务旗标31" }, CHAR_NOWEVENT31 }
    , { { "进行任务旗标32" }, CHAR_NOWEVENT32 }
    , { { "宠物附加数据" }, CHAR_PETDATA }
    , { { "宠物附加数据2" }, CHAR_PETDATA2 }
    , { { "宠物附加数据3" }, CHAR_PETDATA3 }
    , { { "宠物附加数据4" }, CHAR_PETDATA4 }
    , { { "宠物附加数据5" }, CHAR_PETDATA5 }
#ifdef _PET_IMAGE
    , { { "宠物图鉴1" }, CHAR_PETIMAGE1 }
    , { { "宠物图鉴2" }, CHAR_PETIMAGE2 }
    , { { "宠物图鉴3" }, CHAR_PETIMAGE3 }
    , { { "宠物图鉴4" }, CHAR_PETIMAGE4 }
    , { { "宠物图鉴5" }, CHAR_PETIMAGE5 }
    , { { "宠物图鉴6" }, CHAR_PETIMAGE6 }
    , { { "宠物图鉴7" }, CHAR_PETIMAGE7 }
    , { { "宠物图鉴8" }, CHAR_PETIMAGE8 }
    , { { "宠物图鉴9" }, CHAR_PETIMAGE9 }
    , { { "宠物图鉴10" }, CHAR_PETIMAGE10 }
#endif
    , { { "储存点" }, CHAR_SAVEPOINT }
#ifdef _479SA
    , { {"月卡期限"},       CHAR_CARD_TIME }
    , { {"历史评分"},       CHAR_PET_OLD_NUM }
#endif // _479SA
#ifdef _BATTLE_EFFECTS
    , { { "战斗特效" }, CHAR_BATTLEEFFECTS }
    , { { "战斗特效2" }, CHAR_BATTLEEFFECTS2 }
#endif // _BATTLE_EFFECTS
#ifdef _NEWPLAYER_FUNC
    , {{"新手状态"},     CHAR_NEWFLG}
    , {{"主流程ID"},      CHAR_MAINID}
    , {{"子流程ID"},      CHAR_SUBID}
#endif
#ifdef __NEW_CONSIGNMENT
    , {{"售价"},               CHAR_PRICE}
    , {{"寄售"},               CHAR_CONSIGN}
#endif
};

static CharBase CharBaseChar[] =
{
    {{"名字"},                CHAR_NAME}
    , {{"昵称"},           CHAR_OWNTITLE}
    , {{"主人"},               CHAR_USERPETNAME}
    , {{"账号"},           CHAR_CDKEY}
    , {{"家族"},           CHAR_FMNAME}
    , {{"唯一编号"},     CHAR_UNIQUECODE}
#ifdef _TEACHER_SYSTEM
    , {{"导师帐号"},     CHAR_TEACHER_ID}
    , {{"导师名字"},     CHAR_TEACHER_NAME}
#endif
#ifdef _ITEM_SETLOVER
    , {{"爱人账号"},     CHAR_LOVERID}
    , {{"爱人名字"},     CHAR_LOVERNAME}
#endif
#ifdef _NEW_NAME
    , {{"称号"},               CHAR_NEWNAME}
#endif
#ifdef _LOTTERY_SYSTEM
    , {{"彩票"},               CHAR_LOTTERY_VALUE}
#endif
    , {{"主人账号"},     CHAR_OWNERCDKEY}
    , {{"主人名字"},     CHAR_OWNERCHARANAME}
#ifdef _POWER_UP
    , {{"能力提升"},     CHAR_POWER_UP}
#endif
    , {{"伤害BUFF"},       CHAR_DAMAGE_BUFF}
#ifdef _PET_LV1_4V
    , {{"宠物四围"},     CHAR_PET_4V}
    , {{"宠物转生四围"},     CHAR_PETTRN_4V}
#endif
#ifdef _PAOHUAN
    , {{"跑环数据"},     CHAR_PAOHUAN}
#endif
#ifdef _CHAR_TITLE_NEW_
    , {{"新称号数据"},  CHAR_TITLENEW_DATA}
#endif
#ifdef  _NEW_TEACHER_SYSTEM
    , {{"新师徒数据"},  CHAR_NEW_TEACHER_DATA}
#endif
#ifdef  _PLAYER_UID
    , {{"UID"},  CHAR_PLAYERUID}
#endif
#ifdef _NEW_EVENT_ID_UPDATE
    , { { "任务小标" }, CHAR_EVENTID }
#endif // _NEW_EVENT_ID_UPDATE
#ifdef _PARTY_HIGH
    , { { "组队增强名字" }, CHAR_PARTYOTHERNAME }
#endif
#ifdef _ACTIVE_GAME
    , { { "活力时间" }, CHAR_ACTIVE_TIME }
#endif
#ifdef _CAPTURE_DATA
    , { { "抓宠数据" }, CHAR_CAPTURE_DATA }
#endif // _CAPTURE_DATA
#ifdef _479SA
    , { { "月卡状态" }, CHAR_CARD_TYPE }
    , { { "历史四围" }, CHAR_PET_OLD_4V }
    , { { "历史四围初" }, CHAR_PET_OLD_4V_FIRST }
    , { { "历史祝福" }, CHAR_PET_OLD_ZF }
    , { {"活跃度"},      CHAR_ACTIVE_MAIN }
    , { {"活动1"},        CHAR_ACTIVE_ACT1 }
    , { {"活动2"},        CHAR_ACTIVE_ACT2 }
    , { {"活动3"},        CHAR_ACTIVE_ACT3 }
    , { {"活动4"},        CHAR_ACTIVE_ACT4 }
    , { {"活动5"},        CHAR_ACTIVE_ACT5 }
    , { {"活动6"},        CHAR_ACTIVE_ACT6 }
    , { {"活动7"},        CHAR_ACTIVE_ACT7 }
    , { {"活动8"},        CHAR_ACTIVE_ACT8 }
    , { {"活动9"},        CHAR_ACTIVE_ACT9 }
    , { {"活动10"},       CHAR_ACTIVE_ACT10 }
    , { {"活动11"},       CHAR_ACTIVE_ACT11 }
    , { {"活动12"},       CHAR_ACTIVE_ACT12 }
    , { {"活动13"},       CHAR_ACTIVE_ACT13 }
    , { {"活动14"},       CHAR_ACTIVE_ACT14 }
    , { {"活动15"},       CHAR_ACTIVE_ACT15 }
    , { {"活动16"},       CHAR_ACTIVE_ACT16 }
    , { {"月卡期限"},       CHAR_CARD_TIME }
    , { {"历史评分"},       CHAR_PET_OLD_NUM }
#endif // _479SA
    , { {"刷楼数据"},       CHAR_SHUALOU_DATA },
};


static CharBase CharBaseEvent[] =
{
    {{"初始化事件"},       CHAR_INITFUNC}
    //,{{"行走事件"},       CHAR_WALKPREFUNC}
    //,{{"行走后事件"},        CHAR_WALKPOSTFUNC}
    //,{{"未知事件"},       CHAR_PREOVERFUNC}
    //,{{"未知事件"},       CHAR_POSTOVERFUNC}
    //,{{"观看事件"},       CHAR_WATCHFUNC}
    , {{"循环事件"},         CHAR_LOOPFUNC}
    //,{{"死亡事件"},       CHAR_DYINGFUNC}
    , {{"对话事件"},         CHAR_TALKEDFUNC}
    //,{{"攻击事件"},           CHAR_PREATTACKEDFUNC}
    //,{{"攻击后事件"},    CHAR_POSTATTACKEDFUNC}
    //,{{"离开事件"},       CHAR_OFFFUNC}
    //,{{"看事件"},          CHAR_LOOKEDFUNC}
    //,{{"丢弃事件"},           CHAR_ITEMPUTFUNC}
    //,{{"特殊对话事件"}, CHAR_SPECIALTALKEDFUNC}
    , {{"窗口事件"},         CHAR_WINDOWTALKEDFUNC}
#ifdef _USER_CHARLOOPS
    //,{{"循环事件1"},      CHAR_LOOPFUNCTEMP1}
    //,{{"循环事件2"},      CHAR_LOOPFUNCTEMP2}
    //,{{"未知事件"},       CHAR_BATTLEPROPERTY}
#endif
    , {{"重叠事件"},         CHAR_OVERLAPEDFUNC}
    , {{"战后事件"},         CHAR_BATTLEOVERDFUNC}
#ifdef _ALLBLUES_LUA_1_4
    , {{"登出事件"},         CHAR_LOGINOUTFUNC}
#endif
#ifdef _ALLBLUES_LUA_1_9
    , {{"战斗设置事件"}, CHAR_BATTLESETFUNC}
#endif
};

//static CharBase CharBaseColor[] = {
//  {{"白色"},            CHAR_COLORWHITE}
//  ,{{"青色"},       CHAR_COLORCYAN}
//  ,{{"粉色"},           CHAR_COLORPURPLE}
//  ,{{"蓝色"},       CHAR_COLORBLUE}
//  ,{{"黄色"},       CHAR_COLORYELLOW}
//  ,{{"绿色"},       CHAR_COLORGREEN}
//  ,{{"红色"},       CHAR_COLORRED}
//  ,{{"灰白色"},    CHAR_COLORGRAY}
//  ,{{"灰蓝色"},    CHAR_COLORBLUE2}
//  ,{{"灰绿色"},    CHAR_COLORGREEN2}
//
//
//#ifdef _NEW_FONT // <<<<<<-------
//  ,{{"紫色"},     CHAR_COLORVIOLET}
//  ,{{"橙色"},     CHAR_COLORORANGE}
//  ,{{"暗黃色"},    CHAR_COLORYELLOW2}
//  ,{{"金色"},     CHAR_COLORGOLDEN}
//  ,{{"暗橙色"},    CHAR_COLORORANGE2}
//#endif
//  ,{{"黑色"},   CHAR_COLORBLACK}
//  ,{{"随机色"},   CHAR_COLORNUM}
//
//};

static CharBase CharBaseColor[] =
{
    {{"白色"},            CHAR_COLORWHITE}
    , {{"青色"},       CHAR_COLORCYAN}
    , {{"粉色"},           CHAR_COLORPURPLE}
    , {{"蓝色"},       CHAR_COLORBLUE}
    , {{"黄色"},       CHAR_COLORYELLOW}
    , {{"绿色"},       CHAR_COLORGREEN}
    , {{"红色"},       CHAR_COLORRED}
    , {{"灰白色"},   CHAR_COLORGRAY}
    , {{"灰蓝色"},   CHAR_COLORBLUE2}
    , {{"灰绿色"},   CHAR_COLORGREEN2}

#ifdef _NEW_FONT // <<<<<<------- 
    , {{"紫色"},     CHAR_COLORVIOLET}
    , {{"橙色"},     CHAR_COLORORANGE}
    , {{"暗黃色"},    CHAR_COLORYELLOW2}
    , {{"金色"},     CHAR_COLORGOLDEN}
    , {{"暗橙色"},    CHAR_COLORORANGE2}
#endif
    , {{"黑色"},   CHAR_COLORBLACK}
    , {{"随机色"},   CHAR_COLORNUM}
};

static CharBase CharBaseUpdata[] =
{
    {{"HP"},                        CHAR_P_STRING_HP}
    , {{"MAXHP"},                CHAR_P_STRING_MAXHP}
    , {{"MP"},                       CHAR_P_STRING_MP}
    , {{"MAXMP"},                CHAR_P_STRING_MAXMP}
    , {{"体力"},                   CHAR_P_STRING_VITAL}
    , {{"腕力"},                   CHAR_P_STRING_STR}
    , {{"耐力"},                   CHAR_P_STRING_TOUGH}
    , {{"速度"},                   CHAR_P_STRING_DEX}
    , {{"经验"},                   CHAR_P_STRING_EXP}
    , {{"下一级经验"},      CHAR_P_STRING_NEXTEXP}
    , {{"等级"},                   CHAR_P_STRING_LV}
    , {{"攻击力"},                CHAR_P_STRING_ATK}
    , {{"防御力"},                CHAR_P_STRING_DEF}
    , {{"敏捷力"},                CHAR_P_STRING_QUICK}
    , {{"魅力"},                   CHAR_P_STRING_CHARM}
    , {{"运气"},                   CHAR_P_STRING_LUCK}
    , {{"地"},                      CHAR_P_STRING_EARTH}
    , {{"水"},                      CHAR_P_STRING_WATER}
    , {{"火"},                      CHAR_P_STRING_FIRE}
    , {{"风"},                      CHAR_P_STRING_WIND}
    , {{"石币"},                   CHAR_P_STRING_GOLD}
    //,{{"标题"},                 CHAR_P_STRING_TITLE}
    , {{"DP"},                       CHAR_P_STRING_DUELPOINT}
    , {{"转数"},                   CHAR_P_STRING_TRANSMIGRATION}
    , {{"名字"},                   CHAR_P_STRING_NAME}
    , {{"昵称"},                   CHAR_P_STRING_OWNTITLE}
    , {{"骑宠"},                   CHAR_P_STRING_RIDEPET}
    , {{"学骑"},                   CHAR_P_STRING_LEARNRIDE}
    , {{"图像"},                   CHAR_P_STRING_BASEBASEIMAGENUMBER}
    , {{"天行者"},                CHAR_P_STRING_SKYWALKER}
    , {{"调试模式"},         CHAR_P_STRING_DEBUGMODE}
};

static CharBase CharBaseFlg[] =
{
    {{"可见"},                        CHAR_ISVISIBLE}
    , {{"透明"},               CHAR_ISTRANSPARENT}
    , {{"飞行"},             CHAR_ISFLYING}
    , {{"死亡"},             CHAR_ISDIE}
    , {{"组队"},             CHAR_ISPARTY}
    , {{"决斗"},             CHAR_ISDUEL}
    , {{"名片"},             CHAR_ISTRADECARD}
    , {{"交易"},             CHAR_ISTRADE}
#ifdef _CHANNEL_MODIFY
    , {{"密语频道"},       CHAR_ISTELL}
    , {{"家族频道"},       CHAR_ISFM}
    , {{"职业频道"},       CHAR_ISOCC}
    , {{"对话储存"},       CHAR_ISSAVE}
    , {{"聊天室"},          CHAR_ISCHAT}
#ifdef _CHATROOMPROTOCOL
#ifdef _THE_WORLD_SEND
    , {{"世界频道"},       CHAR_ISWORLD}
#endif
#endif
#ifdef _NEW_FS_TYPE
    , { { "特效" }, CHAR_ISEFFECT }
    , { { "屏蔽" }, CHAR_ISPLAYER }
    , { { "队伍满员" }, CHAR_ISTEAMFULL }
    , { { "查看宠物" }, CHAR_ISSHOWPET }
    , { { "自动组队" }, CHAR_ISAUTOPARTY }
    , { { "星球频道" }, CHAR_ISALLWORLD }
#endif
#endif

};

static CharBase CharBaseAction[] =
{
    {{"站立"},                    CHAR_ACTSTAND}
    , {{"走动"},                   CHAR_ACTWALK}
    , {{"攻击"},                   CHAR_ACTATTACK}
    , {{"投掷"},                   CHAR_ACTTHROW}
    , {{"受伤"},                   CHAR_ACTDAMAGE}
    , {{"死亡"},                   CHAR_ACTDEAD}
    , {{"魔法"},                   CHAR_ACTMAGIC}
    , {{"道具"},                   CHAR_ACTITEM}
    , {{"效果"},                   CHAR_ACTEFFECT}
    , {{"下"},                      CHAR_ACTDOWN}
    , {{"坐"},                      CHAR_ACTSIT}
    , {{"挥手"},                   CHAR_ACTHAND}
    , {{"高兴"},                   CHAR_ACTPLEASURE}
    , {{"发怒"},                   CHAR_ACTANGRY}
    , {{"悲哀"},                   CHAR_ACTSAD}
    , {{"防守"},                   CHAR_ACTGUARD}
    , {{"走路"},                   CHAR_ACTACTIONWALK}
    , {{"点头"},                   CHAR_ACTNOD}
    , {{"站立"},                   CHAR_ACTACTIONSTAND}
    , {{"战斗"},                   CHAR_ACTBATTLE}
    , {{"带队"},                   CHAR_ACTLEADER}
    , {{"观战"},                   CHAR_ACTBATTLEWATCH}
    , {{"未知"},                   CHAR_ACTPOPUPNAME}
    , {{"旋转"},                   CHAR_ACTTURN}
    , {{"变形"},                   CHAR_ACTWARP}
    , {{"交易"},                   CHAR_ACTTRADE}
#ifdef _ANGEL_SUMMON
    , {{"天使"},                   CHAR_ACTANGEL}
#endif

#ifdef _MIND_ICON
    , {{"心思"},                   CHAR_MIND}
#endif
#ifdef _STREET_VENDOR
    , {{"打开摊滩"},         CHAR_STREETVENDOR_OPEN}
    , {{"关闭摊滩"},         CHAR_STREETVENDOR_CLOSE}
#endif
};


static int getCharNum(lua_State *L)
{
    lua_pushinteger(L, CHAR_getCharNum());
    return 1;
}

static int getPlayerMaxNum(lua_State *L)
{
    lua_pushinteger(L, CHAR_getPlayerMaxNum());
    return 1;
}

static int getPetMaxNum(lua_State *L)
{
    lua_pushinteger(L, CHAR_getPetMaxNum());
    return 1;
}

static int getOthersMaxNum(lua_State *L)
{
    lua_pushinteger(L, CHAR_getOthersMaxNum());
    return 1;
}


static int check(lua_State *L)
{
    const int index = luaL_checkint(L, 1);
    lua_pushinteger(L, CHAR_CHECKINDEX(index));
    return 1;
}

static int setFlg(lua_State *L)
{
    const int index = luaL_checkint(L, 1);
    const int element = getCharBaseValue(L, 2, CharBaseFlg, arraysizeof(CharBaseFlg));
    const int data = luaL_checkint(L, 3);

    if (CHAR_setFlg(index, element, data) == -1)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

static int getFlg(lua_State *L)
{
    const int index = luaL_checkint(L, 1);
    const int element = getCharBaseValue(L, 2, CharBaseFlg, arraysizeof(CharBaseFlg));
    lua_pushinteger(L, CHAR_getFlg(index, element));
    return 1;
}

static int setWorkChar(lua_State *L)
{
    size_t l;
    const int index = luaL_checkint(L, 1);
    const int element = getCharBaseValue(L, 2, CharBaseWorkChar, arraysizeof(CharBaseWorkChar));
    char *data = luaL_checklstring(L, 3, &l);

    if (CHAR_setWorkChar(index, element, data) == -1)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

static int getWorkChar(lua_State *L)
{
    const int index = luaL_checkint(L, 1);
    const int element = getCharBaseValue(L, 2, CharBaseWorkChar, arraysizeof(CharBaseWorkChar));
    lua_pushstring(L, CHAR_getWorkChar(index, element));
    return 1;
}


static int setWorkInt(lua_State *L)
{
    const int index = luaL_checkint(L, 1);
    const int element = getCharBaseValue(L, 2, CharBaseWorkInt, arraysizeof(CharBaseWorkInt));
    const int data = luaL_checkint(L, 3);

    if (CHAR_setWorkInt(index, element, data) == -1)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

static int getWorkInt(lua_State *L)
{
    const int index = luaL_checkint(L, 1);
    const int element = getCharBaseValue(L, 2, CharBaseWorkInt, arraysizeof(CharBaseWorkInt));
    lua_pushinteger(L, CHAR_getWorkInt(index, element));
    return 1;
}

static int setChar(lua_State *L)
{
    size_t l;
    const int index = luaL_checkint(L, 1);
    const int element = getCharBaseValue(L, 2, CharBaseChar, arraysizeof(CharBaseChar));
    char *data = luaL_checklstring(L, 3, &l);

    if (CHAR_setChar(index, element, data) == -1)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

static int getChar(lua_State *L)
{
    const int index = luaL_checkint(L, 1);
    const int element = getCharBaseValue(L, 2, CharBaseChar, arraysizeof(CharBaseChar));
    lua_pushstring(L, CHAR_getChar(index, element));
    return 1;
}

static int setInt(lua_State *L)
{
    const int index = luaL_checkint(L, 1);
    const int element = getCharBaseValue(L, 2, CharBaseInt, arraysizeof(CharBaseInt));
    const int data = getCharBaseValue(L, 3, CharBaseValue, arraysizeof(CharBaseValue));

    if (CHAR_setInt(index, element, data) == -1)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

static int getInt(lua_State *L)
{
    const int index = luaL_checkint(L, 1);
    const int element = getCharBaseValue(L, 2, CharBaseInt, arraysizeof(CharBaseInt));
    lua_pushinteger(L, CHAR_getInt(index, element));
    return 1;
}

static int setFunctionPointer(lua_State *L)
{
    size_t l;
    const int index = luaL_checkint(L, 1);
    const int functype = getCharBaseValue(L, 2, CharBaseEvent, arraysizeof(CharBaseEvent));
    char *luafunctable = luaL_checklstring(L, 3, &l);
    char *luafunctablepath = luaL_checklstring(L, 4, &l);
    lua_getglobal(L, luafunctable);

    if (!lua_isfunction(L, -1))
    {
        return luaL_error(L, "no function! %s", luafunctable);
    }

    lua_pop(L, 1);

    if (strlen(luafunctablepath) > 0)
    {
        MY_Lua *mylua = &MYLua;

        while (mylua->next != NULL)
        {
            if (strcmp(mylua->luapath, luafunctablepath) == 0)
            {
                return CHAR_setLUAFunction(index, functype, mylua->lua, luafunctable);
            }

            mylua = mylua->next;
        }
    }
    else
    {
        return CHAR_setLUAFunction(index, functype, L, luafunctable);
    }

    return 1;
}

static int delFunctionPointer(lua_State *L)
{
    const int index = luaL_checkint(L, 1);
    const int functype = getCharBaseValue(L, 2, CharBaseEvent, arraysizeof(CharBaseEvent));
    return CHAR_delLUAFunction(index, functype);
}

static int TalkToCli(lua_State *L)
{
    size_t l;
    const int talkedcharaindex = luaL_checkint(L, 1);
    const int talkcharaindex = luaL_checkint(L, 2);
    char *message = luaL_checklstring(L, 3, &l);
    int color = getCharBaseValue(L, 4, CharBaseColor, arraysizeof(CharBaseColor));

    if (color == CHAR_COLORNUM)
    {
        color = RAND(0, CHAR_COLORNUM - 2);
    }

    CHAR_talkToCli(talkedcharaindex, talkcharaindex, message, color);
    return 1;
}

static int TalkToRound(lua_State *L)
{
    size_t l;
    const int talkedcharaindex = luaL_checkint(L, 1);
    char *message = luaL_checklstring(L, 2, &l);
    int color = getCharBaseValue(L, 3, CharBaseColor, arraysizeof(CharBaseColor));

    if (color == CHAR_COLORNUM)
    {
        color = RAND(0, CHAR_COLORNUM - 2);
    }

    int fl = CHAR_getInt(talkedcharaindex, CHAR_FLOOR) ;
    int x  = CHAR_getInt(talkedcharaindex, CHAR_X) ;
    int y  = CHAR_getInt(talkedcharaindex, CHAR_Y) ;
    int i, j;
    int range = 8;

    for (i = x - range ; i <= x + range ; i++)
    {
        for (j = y - range ; j <= y + range ; j++)
        {
            OBJECT  object;

            for (object = MAP_getTopObj(fl, i, j) ; object ; object = NEXT_OBJECT(object))
            {
                int objindex = GET_OBJINDEX(object);
                int toindex = OBJECT_getIndex(objindex);

                if (OBJECT_getType(objindex) == OBJTYPE_CHARA && toindex != talkedcharaindex)
                {
                    if (CHAR_getInt(toindex, CHAR_WHICHTYPE) == CHAR_TYPEPLAYER && CHAR_getWorkInt(toindex, CHAR_WORKBATTLEINDEX) < 0)
                    {
                        typedef void (*TALKF)(int, int, char*, int, int
#ifdef _ITEM_PET_TALK_
                                              , char *
#endif
#ifdef _YAYA_VOICE
                                              , int
#endif
                                             );
                        TALKF   talkedfunc = NULL;
                        talkedfunc = (TALKF)CHAR_getFunctionPointer(toindex, CHAR_TALKEDFUNC);

                        if (talkedfunc)
                        {
                            talkedfunc(toindex, talkedcharaindex, message, color, -1
#ifdef _ITEM_PET_TALK_
                                       , ""
#endif
#ifdef _YAYA_VOICE
                                       , 0
#endif
                                      );
                        }
                    }
                }
            }
        }
    }

    return 1;
}

static int WalkPoint(lua_State *L)
{
    const int index = luaL_checkint(L, 1);

    if (CHAR_getWorkInt(index, CHAR_WORKBATTLEMODE) != BATTLE_CHARMODE_NONE)
    {
        return 0;
    }

    const int x = luaL_checkint(L, 2);
    const int y = luaL_checkint(L, 3);
    POINT   start, end;
    start.x = CHAR_getInt(index, CHAR_X);
    start.y = CHAR_getInt(index, CHAR_Y);
    end.x = x;
    end.y = y;
    int pos = NPC_Util_getDirFromTwoPoint(&start, &end);

    if (pos != - 1)
    {
        CHAR_walk(index, pos, 0);
    }

    return 1;
}

static int Walk(lua_State *L)
{
    const int index = luaL_checkint(L, 1);

    if (CHAR_getWorkInt(index, CHAR_WORKBATTLEMODE) != BATTLE_CHARMODE_NONE)
    {
        return 0;
    }

    const int dir = luaL_checkint(L, 2);
    const int step = luaL_checkint(L, 3);
    POINT   start, end;
    start.x = CHAR_getInt(index, CHAR_X);
    start.y = CHAR_getInt(index, CHAR_Y);
    end.x = start.x;
    end.y = start.y;

    switch (dir)
    {
        case 0:
            {
                end.x = start.x - step;
                break;
            }

        case 1:
            {
                end.x = start.x + step;
                end.y = start.y - step;
                break;
            }

        case 2:
            {
                end.x = start.x + step;
                break;
            }

        case 3:
            {
                end.x = start.x + step;
                end.y = start.y + step;
                break;
            }

        case 4:
            {
                end.y = start.y + step;
                break;
            }

        case 5:
            {
                end.x = start.x - step;
                end.y = start.y + step;
                break;
            }

        case 6:
            {
                end.y = start.y - step;
                break;
            }

        case 7:
            {
                end.x = start.x - step;
                end.y = start.y - step;
                break;
            }
    }

    int pos = NPC_Util_getDirFromTwoPoint(&start, &end);

    if (dir != - 1)
    {
        CHAR_walk(index, pos, 0);
    }

    return 1;
}

static int BoundRandWalk(lua_State *L)
{
    const int index = luaL_checkint(L, 1);

    if (CHAR_getWorkInt(index, CHAR_WORKBATTLEMODE) != BATTLE_CHARMODE_NONE)
    {
        return 0;
    }

    const int minX = min(luaL_checkint(L, 2), luaL_checkint(L, 4));
    const int minY = min(luaL_checkint(L, 3), luaL_checkint(L, 5));
    const int maxX = max(luaL_checkint(L, 2), luaL_checkint(L, 4));
    const int maxY = max(luaL_checkint(L, 3), luaL_checkint(L, 5));
    POINT   start, end;
    int stepX = (rand() % 10) - 5;
    int stepY = (rand() % 10) - 5;
    start.x = CHAR_getInt(index, CHAR_X);
    start.y = CHAR_getInt(index, CHAR_Y);
    end.x = start.x + stepX;

    if (end.x > maxX)
    {
        end.x = maxX;
    }
    else if (end.x < minX)
    {
        end.x = minX;
    }

    end.y = start.y + stepY;

    if (end.y > maxY)
    {
        end.y = maxY;
    }
    else if (end.y < minY)
    {
        end.y = minY;
    }

    int dir = NPC_Util_getDirFromTwoPoint(&start, &end);

    if (dir != - 1)
    {
        CHAR_walk(index, dir, 0);
    }

    return 1;
}

static int RandRandWalk(lua_State *L)
{
    const int index = luaL_checkint(L, 1);

    if (CHAR_getWorkInt(index, CHAR_WORKBATTLEMODE) != BATTLE_CHARMODE_NONE)
    {
        return 0;
    }

    POINT   start, end;
    int stepX = (RAND(0, 1000) % 9) - 4;
    int stepY = (RAND(0, 1000) % 9) - 4;
    start.x = CHAR_getInt(index, CHAR_X);
    start.y = CHAR_getInt(index, CHAR_Y);
    end.x = start.x + stepX;
    end.y = start.y + stepY;
    int dir = NPC_Util_getDirFromTwoPoint(&start, &end);

    if (dir != - 1)
    {
        CHAR_walk(index, dir, 0);
    }

    return 1;
}

static int ToAroundChar(lua_State *L)
{
    const int index = luaL_checkint(L, 1);
    int objindex = CHAR_getWorkInt(index, CHAR_WORKOBJINDEX);
    CHAR_sendCToArroundCharacter(objindex);
    return 1;
}

static int AroundChar(lua_State *L)
{
    const int index = luaL_checkint(L, 1);
    CHAR_sendArroundCharaData(index);
    return 1;
}

static int sendCMeToMe(lua_State *L)
{
    const int index = luaL_checkint(L, 1);
    CHAR_sendCMeToMe(index);
    return 1;
}

static int talkToFloor(lua_State *L)
{
    size_t l;
    const int floor = luaL_checkint(L, 1);
    const int talkindex = luaL_checkint(L, 2);
    char *message = luaL_checklstring(L, 3, &l);
    int color = getCharBaseValue(L, 4, CharBaseColor, arraysizeof(CharBaseColor));

    if (color == CHAR_COLORNUM)
    {
        color = RAND(0, CHAR_COLORNUM - 2);
    }

    CHAR_talkToFloor(floor, talkindex, message, color);
    return 1;
}

static int talkToParty(lua_State *L)
{
    size_t l;
    const int talkedcharaindex = luaL_checkint(L, 1);
    const int talkcharaindex = luaL_checkint(L, 2);
    char *message = luaL_checklstring(L, 3, &l);
    int color = getCharBaseValue(L, 4, CharBaseColor, arraysizeof(CharBaseColor));

    if (color == CHAR_COLORNUM)
    {
        color = RAND(0, CHAR_COLORNUM - 2);
    }

    CHAR_talkToCliAndParty(talkedcharaindex, talkcharaindex, message, color);
    return 1;
}

static int talkToServer(lua_State *L)
{
    size_t l;
    const int talkcharaindex = luaL_checkint(L, 1);
    char *message = luaL_checklstring(L, 2, &l);
    int color = getCharBaseValue(L, 3, CharBaseColor, arraysizeof(CharBaseColor));

    if (color == CHAR_COLORNUM)
    {
        color = RAND(0, CHAR_COLORNUM - 2);
    }

    CHAR_talkToAll(talkcharaindex, message, color);
    return 1;
}

static int talkToWorld(lua_State *L)
{
    size_t l;
    const int talkcharaindex = luaL_checkint(L, 1);
    char *message = luaL_checklstring(L, 2, &l);
    int color = getCharBaseValue(L, 3, CharBaseColor, arraysizeof(CharBaseColor));
#ifdef  _ITEM_PET_TALK_
    char *outitempetData = luaL_checklstring(L, 4, &l);
#ifdef  _PLAYER_UID
    char *uid = luaL_checklstring(L, 5, &l);
#ifdef _YAYA_VOICE
    int volumeLen = luaL_checkint(L, 6);
#endif
#else
#ifdef _YAYA_VOICE
    int volumeLen = luaL_checkint(L, 5);
#endif
#endif
#else
#ifdef  _PLAYER_UID
    char *uid = luaL_checklstring(L, 4, &l);
#endif
#endif

    if (color == CHAR_COLORNUM)
    {
        color = RAND(0, CHAR_COLORNUM - 2);
    }

    int i = 0;
    int playernum = CHAR_getPlayerMaxNum();

    for (i = 0; i < playernum; i++)
    {
        if (CHAR_getCharUse(i) == FALSE)
        {
            continue;
        }

        if (!CHAR_CHECKINDEX(i))
        {
            continue;
        }

        if (!CHAR_getFlg(i, CHAR_ISWORLD))
        {
            continue;
        }

        if (CHAR_getWorkInt(i, CHAR_WORKFD) < 0)
        {
            continue;
        }

        char tempbuff[2048];
        memset(tempbuff, 0, sizeof(tempbuff));
        sprintf(tempbuff, "P|P|%s|0", message);
        lssproto_TK_send(CHAR_getWorkInt(i, CHAR_WORKFD), CHAR_getWorkInt(talkcharaindex, CHAR_WORKOBJINDEX), tempbuff, color
#ifdef _ITEM_PET_TALK_
                         , outitempetData
#endif
#ifdef  _PLAYER_UID
                         , uid
#endif
#ifdef _YAYA_VOICE
                         , volumeLen
#endif
#ifdef _TK_PLAYERFACE
                         , CHAR_getInt(talkcharaindex, CHAR_FACEIMAGENUMBER)
                         , CHAR_getInt(talkcharaindex, CHAR_LV)
#endif // _TK_PLAYERFACE
                        );
    }

    //CHAR_talkToCli(i, talkcharaindex, message, color);
    return 1;
}
#ifdef _ALLBLUES_LUA_1_1
static int WarpToSpecificPoint(lua_State *L)
{
    const int charaindex = luaL_checkint(L, 1);
    const int floor = luaL_checkint(L, 2);
    const int x = luaL_checkint(L, 3);
    const int y = luaL_checkint(L, 4);
    lua_pushinteger(L, CHAR_warpToSpecificPoint(charaindex, floor, x, y));
    return 1;
}
static int MapAllWarp(lua_State *L)
{
    const int oldfloor = luaL_checkint(L, 1);
    const int newfloor = luaL_checkint(L, 2);
    const int x = luaL_checkint(L, 3);
    const int y = luaL_checkint(L, 4);
    int     i;
    int     playernum = CHAR_getPlayerMaxNum();

    for (i = 0 ; i < playernum ; i++)
    {
        if (CHAR_getCharUse(i) == FALSE)
        {
            continue;
        }

        if (CHAR_getInt(i, CHAR_FLOOR) != oldfloor)
        {
            continue;
        }

        CHAR_warpToSpecificPoint(i, newfloor, x, y);
    }

    return 1;
}
#endif


#ifdef _ALLBLUES_LUA_1_3
static int createPet(lua_State *L)
{
    const int enemyid = luaL_checkint(L, 1);
    const int UpLevel = luaL_checkint(L, 2);
    int i;

    for (i = 0; i < ENEMY_getEnemyNum(); i ++)
    {
        if (ENEMY_getInt(i, ENEMY_ID) == enemyid)
        {
            break;
        }
    }

    if (i == ENEMY_getEnemyNum())
    {
        lua_pushinteger(L, -1);
        return 1;
    }

    int petindex = ENEMY_createPet(i, -1, -1, -1, -1);

    for (i = 0; i < UpLevel; i ++)
    {
        CHAR_PetLevelUp(petindex);
        CHAR_PetAddVariableAi(petindex, AI_FIX_PETLEVELUP);
    }

    CHAR_setInt(petindex, CHAR_LV, CHAR_getInt(petindex, CHAR_LV) + UpLevel);
    CHAR_complianceParameter(petindex);
    CHAR_setInt(petindex, CHAR_HP, CHAR_getWorkInt(petindex, CHAR_WORKMAXHP));
    CHAR_LoginBesideSetWorkInt(petindex, -1);
    lua_pushinteger(L, petindex);
    return 1;
}

static int setCharPet(lua_State *L)
{
    const int index = luaL_checkint(L, 1);
    const int petindex = luaL_checkint(L, 2);
    int havepetelement = CHAR_getCharPetElement(index);

    if (havepetelement > -1)
    {
        CHAR_setCharPet(index, havepetelement, petindex);
        CHAR_setWorkInt(petindex, CHAR_WORKPLAYERINDEX, index);
    }

    lua_pushinteger(L, havepetelement);
    return 1;
}

static int setCharPetNew(lua_State *L)
{
    const int index = luaL_checkint(L, 1);
    const int pethaveid = luaL_checkint(L, 2);
    const int petindex = luaL_checkint(L, 3);
    CHAR_setCharPet(index, pethaveid, petindex);
    lua_pushinteger(L, pethaveid);
    return 1;
}

static int getCharPet(lua_State *L)
{
    const int index = luaL_checkint(L, 1);
    const int havepetid = luaL_checkint(L, 2);
    lua_pushinteger(L, CHAR_getCharPet(index, havepetid));
    return 1;
}

static int setCharPoolPet(lua_State *L)
{
    const int index = luaL_checkint(L, 1);
    const int petindex = luaL_checkint(L, 2);
    int havepetelement = CHAR_getCharPoolPetElement(index);

    if (havepetelement > -1)
    {
        CHAR_setCharPoolPet(index, havepetelement, petindex);
        CHAR_setWorkInt(petindex, CHAR_WORKPLAYERINDEX, index);
    }

    lua_pushinteger(L, havepetelement);
    return 1;
}

static int getCharPoolPet(lua_State *L)
{
    const int index = luaL_checkint(L, 1);
    const int havepetid = luaL_checkint(L, 2);
    lua_pushinteger(L, CHAR_getCharPoolPet(index, havepetid));
    return 1;
}

static int setDepotPetIndex(lua_State *L)
{
    const int index = luaL_checkint(L, 1);
    const int petindex = luaL_checkint(L, 2);
    int havepetelement = CHAR_getCharPoolPetElement(index);

    if (havepetelement > -1)
    {
        CHAR_setDepotPetIndex(index, havepetelement, petindex);
        CHAR_setWorkInt(petindex, CHAR_WORKPLAYERINDEX, index);
    }

    lua_pushinteger(L, havepetelement);
    return 1;
}

static int getDepotPetIndex(lua_State *L)
{
    const int index = luaL_checkint(L, 1);
    const int havepetid = luaL_checkint(L, 2);

    if (!CHAR_CheckDepotPet(index))
    {
        lua_pushinteger(L, -1);
        return 1;
    }

    lua_pushinteger(L, CHAR_getDepotPetIndex(index, havepetid));
    return 1;
}

static int DelItem(lua_State *L)
{
    const int charaindex = luaL_checkint(L, 1);
    const int haveitemid = luaL_checkint(L, 2);
    CHAR_DelItem(charaindex, haveitemid);
    return 1;
}

static int DelPileItemMess(lua_State *L)
{
    const int charaindex = luaL_checkint(L, 1);
    const int haveitemid = luaL_checkint(L, 2);
    const int itemindex = CHAR_getItemIndex(charaindex, haveitemid);
    CHAR_DelPileItemMess(charaindex, haveitemid, ITEM_getInt(itemindex, ITEM_USEPILENUMS), 1);
    return 1;
}

static int getFd(lua_State *L)
{
    const int charaindex = luaL_checkint(L, 1);
    lua_pushinteger(L, getfdFromCharaIndex(charaindex));
    return 1;
}

static int Updata(lua_State *L)
{
    const int charaindex = luaL_checkint(L, 1);
    const int indextable = getCharBaseValue(L, 2, CharBaseUpdata, arraysizeof(CharBaseUpdata));
    CHAR_send_P_StatusString(charaindex, indextable);
    return 1;
}

static int Additem(lua_State *L)
{
    const int charaindex = luaL_checkint(L, 1);
    const int itemid = luaL_checkint(L, 2);
    int itemindex = -1;
    int emptyitemindexinchara = CHAR_findEmptyItemBox(charaindex);

    if (emptyitemindexinchara >= 0)
    {
        itemindex = ITEM_makeItemAndRegist(itemid);

        if (itemindex > -1)
        {
            CHAR_setItemIndex(charaindex, emptyitemindexinchara, itemindex);
            ITEM_setWorkInt(itemindex, ITEM_WORKOBJINDEX, -1);
            ITEM_setWorkInt(itemindex, ITEM_WORKCHARAINDEX, charaindex);
            CHAR_sendItemDataOne(charaindex, emptyitemindexinchara);
            LogItem(
                CHAR_getChar(charaindex, CHAR_NAME),
                CHAR_getChar(charaindex, CHAR_CDKEY),
#ifdef _add_item_log_name  // WON ADD 在item的log中增加item名称
                itemindex,
#else
                atoi(message),
#endif
                "AddItem(制作道具LUA)",
                CHAR_getInt(charaindex, CHAR_FLOOR),
                CHAR_getInt(charaindex, CHAR_X),
                CHAR_getInt(charaindex, CHAR_Y),
                ITEM_getChar(itemindex, ITEM_UNIQUECODE),
                ITEM_getChar(itemindex, ITEM_NAME),
                ITEM_getInt(itemindex, ITEM_ID)
            );
        }
    }

    lua_pushinteger(L, itemindex);
    return 1;
}

static int Finditem(lua_State *L)
{
    const int charaindex = luaL_checkint(L, 1);
    const int itemid = luaL_checkint(L, 2);
    int i;
    int itemindex = -1;
    int id;

    for (i = CHAR_STARTITEMARRAY; i < CHAR_MAXITEMHAVE; i++)
    {
        itemindex = CHAR_getItemIndex(charaindex, i);

        if (ITEM_CHECKINDEX(itemindex))
        {
            id = ITEM_getInt(itemindex, ITEM_ID);

            if (id == itemid)
            {
                lua_pushinteger(L, itemindex);
                return 1;
            }
        }
    }

    lua_pushinteger(L, -1);
    return 1;
}

static int AddPet(lua_State *L)
{
    const int index = luaL_checkint(L, 1);
    const int enemyid = luaL_checkint(L, 2);
    const int UpLevel = luaL_checkint(L, 3);
    int i, havepetelement;

    for (i = 0; i < ENEMY_getEnemyNum(); i ++)
    {
        if (ENEMY_getInt(i, ENEMY_ID) == enemyid)
        {
            break;
        }
    }

    if (i == ENEMY_getEnemyNum())
    {
        lua_pushinteger(L, -1);
        return 1;
    }

    for (havepetelement = 0; havepetelement < CHAR_MAXPETHAVE; havepetelement ++)
    {
        int petindex = CHAR_getCharPet(index, havepetelement);

        if (!CHAR_CHECKINDEX(petindex))
        {
            break;
        }
    }

    if (havepetelement == CHAR_MAXPETHAVE)
    {
        lua_pushinteger(L, -1);
        return 1;
    }

    int petindex = ENEMY_createPet(i, -1, -1, -1, -1);

    if (!CHAR_CHECKINDEX(petindex))
    {
        lua_pushinteger(L, -1);
        return 1;
    }

    for (i = CHAR_getInt(petindex, CHAR_LV); i < UpLevel; i ++)
    {
        CHAR_PetLevelUp(petindex);
        CHAR_PetAddVariableAi(petindex, AI_FIX_PETLEVELUP);
        CHAR_setInt(petindex, CHAR_LV, CHAR_getInt(petindex, CHAR_LV) + 1);
    }

    CHAR_setCharPet(index, havepetelement, petindex);
    CHAR_setWorkInt(petindex, CHAR_WORKPLAYERINDEX, index);
    char token[64];
    CHAR_setInt(petindex, CHAR_VARIABLEAI, 10000);
    CHAR_complianceParameter(petindex);
    CHAR_setInt(petindex, CHAR_HP, CHAR_getWorkInt(petindex, CHAR_WORKMAXHP));

    if (CHAR_getInt(index, CHAR_WHICHTYPE) == CHAR_TYPEPLAYER)
    {
        CHAR_setChar(petindex, CHAR_OWNERCDKEY, CHAR_getChar(index, CHAR_CDKEY));
        CHAR_setChar(petindex, CHAR_OWNERCHARANAME, CHAR_getChar(index, CHAR_NAME));
    }

    snprintf(token, sizeof(token), "K%d", havepetelement);
    CHAR_sendStatusString(index, token);
    snprintf(token, sizeof(token), "W%d", havepetelement);
    CHAR_sendStatusString(index, token);
    LogPet(
        CHAR_getChar(index, CHAR_NAME),
        CHAR_getChar(index, CHAR_CDKEY),
        CHAR_getChar(petindex, CHAR_NAME),
        CHAR_getInt(petindex, CHAR_LV),
        "来至LUA的char.AddPet",
        CHAR_getInt(index, CHAR_FLOOR),
        CHAR_getInt(index, CHAR_X),
        CHAR_getInt(index, CHAR_Y),
        CHAR_getChar(petindex, CHAR_UNIQUECODE)    // shan 2001/12/14
    );
    lua_pushinteger(L, petindex);
    return 1;
}

static int AddPetTempNo(lua_State *L)
{
    const int index = luaL_checkint(L, 1);
    const int tempno = luaL_checkint(L, 2);
    const int UpLevel = luaL_checkint(L, 3);
    int i, havepetelement;

    for (havepetelement = 0; havepetelement < CHAR_MAXPETHAVE; havepetelement++)
    {
        int petindex = CHAR_getCharPet(index, havepetelement);

        if (!CHAR_CHECKINDEX(petindex))
        {
            break;
        }
    }

    if (havepetelement == CHAR_MAXPETHAVE)
    {
        lua_pushinteger(L, -1);
        return 1;
    }

    int petindex = ENEMY_createPetFromTempNo(tempno);

    if (!CHAR_CHECKINDEX(petindex))
    {
        lua_pushinteger(L, -1);
        return 1;
    }

    for (i = CHAR_getInt(petindex, CHAR_LV); i < UpLevel; i++)
    {
        CHAR_PetLevelUp(petindex);
        CHAR_PetAddVariableAi(petindex, AI_FIX_PETLEVELUP);
        CHAR_setInt(petindex, CHAR_LV, CHAR_getInt(petindex, CHAR_LV) + 1);
    }

    CHAR_setCharPet(index, havepetelement, petindex);
    CHAR_setWorkInt(petindex, CHAR_WORKPLAYERINDEX, index);
    char token[64];
    CHAR_setInt(petindex, CHAR_VARIABLEAI, 10000);
    CHAR_complianceParameter(petindex);
    CHAR_setInt(petindex, CHAR_HP, CHAR_getWorkInt(petindex, CHAR_WORKMAXHP));

    if (CHAR_getInt(index, CHAR_WHICHTYPE) == CHAR_TYPEPLAYER)
    {
        CHAR_setChar(petindex, CHAR_OWNERCDKEY, CHAR_getChar(index, CHAR_CDKEY));
        CHAR_setChar(petindex, CHAR_OWNERCHARANAME, CHAR_getChar(index, CHAR_NAME));
    }

    snprintf(token, sizeof(token), "K%d", havepetelement);
    CHAR_sendStatusString(index, token);
    snprintf(token, sizeof(token), "W%d", havepetelement);
    CHAR_sendStatusString(index, token);
    LogPet(
        CHAR_getChar(index, CHAR_NAME),
        CHAR_getChar(index, CHAR_CDKEY),
        CHAR_getChar(petindex, CHAR_NAME),
        CHAR_getInt(petindex, CHAR_LV),
        "来至LUA的char.AddPet",
        CHAR_getInt(index, CHAR_FLOOR),
        CHAR_getInt(index, CHAR_X),
        CHAR_getInt(index, CHAR_Y),
        CHAR_getChar(petindex, CHAR_UNIQUECODE)   // shan 2001/12/14
    );
    lua_pushinteger(L, petindex);
    return 1;
}


static int AddPetCf515(lua_State *L)
{
    const int index = luaL_checkint(L, 1);
    const int enemyid = luaL_checkint(L, 2);
    const int UpLevel = luaL_checkint(L, 3);
    const int trn = luaL_checkint(L, 4);
    const int vital = luaL_checkint(L, 5);
    const int str = luaL_checkint(L, 6);
    const int tgh = luaL_checkint(L, 7);
    const int dex = luaL_checkint(L, 8);
    int i, havepetelement;

    for (i = 0; i < ENEMY_getEnemyNum(); i++)
    {
        if (ENEMY_getInt(i, ENEMY_ID) == enemyid)
        {
            break;
        }
    }

    if (i == ENEMY_getEnemyNum())
    {
        lua_pushinteger(L, -1);
        return 1;
    }

    for (havepetelement = 0; havepetelement < CHAR_MAXPETHAVE; havepetelement++)
    {
        int petindex = CHAR_getCharPet(index, havepetelement);

        if (!CHAR_CHECKINDEX(petindex))
        {
            break;
        }
    }

    if (havepetelement == CHAR_MAXPETHAVE)
    {
        lua_pushinteger(L, -1);
        return 1;
    }

    int petindex = ENEMY_createPetFromEnemyIndex(index, i);

    if (!CHAR_CHECKINDEX(petindex))
    {
        lua_pushinteger(L, -1);
        return 1;
    }

    int LevelUpPoint = (vital << 24) + (str << 16) + (tgh << 8) + (dex << 0);

    for (i = CHAR_getInt(petindex, CHAR_LV); i < UpLevel; i++)
    {
        CHAR_setInt(petindex, CHAR_ALLOCPOINT, LevelUpPoint);
        CHAR_PetLevelUp(petindex);
        CHAR_PetAddVariableAi(petindex, AI_FIX_PETLEVELUP);
        CHAR_setInt(petindex, CHAR_LV, CHAR_getInt(petindex, CHAR_LV) + 1);
    }

    CHAR_setInt(petindex, CHAR_TRANSMIGRATION, trn);
    //CHAR_setCharPet( index, havepetelement, petindex);
    //  CHAR_setChar( petindex, CHAR_OWNERCDKEY, CHAR_getChar( index, CHAR_CDKEY));
    //  CHAR_setChar( petindex, CHAR_OWNERCHARANAME, CHAR_getChar( index, CHAR_NAME));
    CHAR_setWorkInt(petindex, CHAR_WORKPLAYERINDEX, index);
    char token[64];
    CHAR_setInt(petindex, CHAR_VARIABLEAI, 10000);
    CHAR_complianceParameter(petindex);
    CHAR_setInt(petindex, CHAR_HP, CHAR_getWorkInt(petindex, CHAR_WORKMAXHP));
    snprintf(token, sizeof(token), "K%d", havepetelement);
    CHAR_sendStatusString(index, token);
    snprintf(token, sizeof(token), "W%d", havepetelement);
    CHAR_sendStatusString(index, token);
    LogPet(
        CHAR_getChar(index, CHAR_NAME),
        CHAR_getChar(index, CHAR_CDKEY),
        CHAR_getChar(petindex, CHAR_NAME),
        CHAR_getInt(petindex, CHAR_LV),
        "来至LUA的char.AddPetCf",
        CHAR_getInt(index, CHAR_FLOOR),
        CHAR_getInt(index, CHAR_X),
        CHAR_getInt(index, CHAR_Y),
        CHAR_getChar(petindex, CHAR_UNIQUECODE)   // shan 2001/12/14
    );
    lua_pushinteger(L, petindex);
    return 1;
}





static int AddPetCf(lua_State *L)
{
    const int index = luaL_checkint(L, 1);
    const int enemyid = luaL_checkint(L, 2);
    const int UpLevel = luaL_checkint(L, 3);
    const int trn = luaL_checkint(L, 4);
    const int vital = luaL_checkint(L, 5);
    const int str = luaL_checkint(L, 6);
    const int tgh = luaL_checkint(L, 7);
    const int dex = luaL_checkint(L, 8);
    int i, havepetelement;

    for (i = 0; i < ENEMY_getEnemyNum(); i ++)
    {
        if (ENEMY_getInt(i, ENEMY_ID) == enemyid)
        {
            break;
        }
    }

    if (i == ENEMY_getEnemyNum())
    {
        lua_pushinteger(L, -1);
        return 1;
    }

    for (havepetelement = 0; havepetelement < CHAR_MAXPETHAVE; havepetelement ++)
    {
        int petindex = CHAR_getCharPet(index, havepetelement);

        if (!CHAR_CHECKINDEX(petindex))
        {
            break;
        }
    }

    if (havepetelement == CHAR_MAXPETHAVE)
    {
        lua_pushinteger(L, -1);
        return 1;
    }

    int petindex = ENEMY_createPet(i, vital, str, tgh, dex);

    if (!CHAR_CHECKINDEX(petindex))
    {
        lua_pushinteger(L, -1);
        return 1;
    }

    for (i = CHAR_getInt(petindex, CHAR_LV); i < UpLevel; i ++)
    {
        CHAR_PetLevelUp(petindex);
        CHAR_PetAddVariableAi(petindex, AI_FIX_PETLEVELUP);
        CHAR_setInt(petindex, CHAR_LV, CHAR_getInt(petindex, CHAR_LV) + 1);
    }

    CHAR_setInt(petindex, CHAR_TRANSMIGRATION, trn);
    CHAR_setCharPet(index, havepetelement, petindex);
//  CHAR_setChar( petindex, CHAR_OWNERCDKEY, CHAR_getChar( index, CHAR_CDKEY));
//  CHAR_setChar( petindex, CHAR_OWNERCHARANAME, CHAR_getChar( index, CHAR_NAME));
    CHAR_setWorkInt(petindex, CHAR_WORKPLAYERINDEX, index);
    char token[64];
    CHAR_setInt(petindex, CHAR_VARIABLEAI, 10000);
    CHAR_complianceParameter(petindex);
    CHAR_setInt(petindex, CHAR_HP, CHAR_getWorkInt(petindex, CHAR_WORKMAXHP));
    snprintf(token, sizeof(token), "K%d", havepetelement);
    CHAR_sendStatusString(index, token);
    snprintf(token, sizeof(token), "W%d", havepetelement);
    CHAR_sendStatusString(index, token);
    LogPet(
        CHAR_getChar(index, CHAR_NAME),
        CHAR_getChar(index, CHAR_CDKEY),
        CHAR_getChar(petindex, CHAR_NAME),
        CHAR_getInt(petindex, CHAR_LV),
        "来至LUA的char.AddPetCf",
        CHAR_getInt(index, CHAR_FLOOR),
        CHAR_getInt(index, CHAR_X),
        CHAR_getInt(index, CHAR_Y),
        CHAR_getChar(petindex, CHAR_UNIQUECODE)    // shan 2001/12/14
    );
    lua_pushinteger(L, petindex);
    return 1;
}

#ifdef _PLAYER_NPC
static int setPlayerNpc(lua_State *L)
{
    const int charaindex = luaL_checkint(L, 1);
    const int petindex = luaL_checkint(L, 2);
    int i;

    for (i = 1; i < getPartyNum(charaindex); i ++)
    {
        if (CHAR_getWorkInt(charaindex, i + CHAR_WORKPARTYINDEX1) == -1)
        {
            break;
        }
    }

    if (i == getPartyNum(charaindex))
    {
        CHAR_endCharOneArray(petindex);
    }
    else
    {
        if (CHAR_CHECKINDEX(petindex) == TRUE)
        {
            Object object;
            int objindex;
            CHAR_setInt(petindex, CHAR_FLOOR, CHAR_getInt(charaindex, CHAR_FLOOR));
            CHAR_setInt(petindex, CHAR_X, CHAR_getInt(charaindex, CHAR_X));
            CHAR_setInt(petindex, CHAR_Y, CHAR_getInt(charaindex, CHAR_Y));
            object.type = OBJTYPE_CHARA;
            object.index = petindex;
            object.x = CHAR_getInt(petindex, CHAR_X);
            object.y = CHAR_getInt(petindex, CHAR_Y);
            object.floor = CHAR_getInt(petindex, CHAR_FLOOR);
            objindex = initObjectOne(&object);

            if (objindex == -1)
            {
                CHAR_endCharOneArray(petindex);
            }
            else
            {
                CHAR_setWorkInt(petindex, CHAR_WORKOBJINDEX, objindex);

                if (CHAR_JoinParty_Main(petindex, charaindex) == FALSE)
                {
                    CHAR_CharaDelete(petindex);
                    return 1;
                }

                CHAR_sendCToArroundCharacter(objindex);
            }
        }
    }

    return 1;
}
#endif
#endif
#ifdef _PETSKILL_SHOP_LUA
static int FreePetSkill(lua_State *L)
{
    const int petindex = luaL_checkint(L, 1);
    const int petskill = luaL_checkint(L, 2);
    int i;

    for (i = 0; i < CHAR_MAXPETSKILLHAVE; i++)
    {
        if (petskill == CHAR_getPetSkill(petindex, i))
        {
            lua_pushinteger(L, 1);
            return 1;
        }
    }

    lua_pushinteger(L, 0);
    return 1;
}
#endif

static int Coordinate(lua_State *L)
{
    const int index = luaL_checkint(L, 1);

    if (CHAR_getWorkInt(index, CHAR_WORKBATTLEMODE) != BATTLE_CHARMODE_NONE)
    {
        return 0;
    }

    const int X = luaL_checkint(L, 2);
    const int Y = luaL_checkint(L, 3);
    POINT   start, end;
    start.x = CHAR_getInt(index, CHAR_X);
    start.y = CHAR_getInt(index, CHAR_Y);
    end.x = X;
    end.y = Y;
    int dir = NPC_Util_getDirFromTwoPoint(&start, &end);

    if (dir != - 1)
    {
        CHAR_walk(index, dir, 0);
    }

    return 1;
}

static int setPetSkill(lua_State *L)
{
    const int index = luaL_checkint(L, 1);
    const int havepetskillindex = luaL_checkint(L, 2);
    const int skillID = luaL_checkint(L, 3);
    CHAR_setPetSkill(index, havepetskillindex, skillID);
    return 1;
}

static int getPetSkill(lua_State *L)
{
    const int index = luaL_checkint(L, 1);
    const int havepetskillindex = luaL_checkint(L, 2);
    lua_pushinteger(L, CHAR_getPetSkill(index, havepetskillindex));
    return 1;
}


static int PetLevelUp(lua_State *L)
{
    const int index = luaL_checkint(L, 1);
    CHAR_PetLevelUp(index);
    return 1;
}

static int GMPetLevelUp(lua_State *L)
{
    const int index = luaL_checkint(L, 1);
    const int zhang = luaL_checkint(L, 2);
    CHAR_GMPetLevelUp(index, zhang);
    return 1;
}

static int JoinParty(lua_State *L)
{
    const int index1 = luaL_checkint(L, 1);
    const int index2 = luaL_checkint(L, 2);
    lua_pushinteger(L, CHAR_JoinParty_Main(index2, index1));
    return 1;
}

#ifdef _ALLBLUES_LUA_1_4
static int getRightTo8(lua_State *L)
{
    const int value = luaL_checkint(L, 1);
    const int flg = luaL_checkint(L, 2);
    lua_pushinteger(L, (value >> (4 - flg) * 8) & 0xFF);
    return 1;
}

static int getLiftTo8(lua_State *L)
{
    const int value = luaL_checkint(L, 1);
    const int flg = luaL_checkint(L, 2);
    lua_pushinteger(L, (value << (4 - flg) * 8));
    return 1;
}

static int complianceParameter(lua_State *L)
{
    const int index = luaL_checkint(L, 1);
    //print("体力：%d 攻击：%d 耐力：%d  速度：%d\n", CHAR_getInt(index, CHAR_VITAL), CHAR_getInt(index, CHAR_STR), CHAR_getInt(index, CHAR_TOUGH), CHAR_getInt(index, CHAR_DEX));
    CHAR_complianceParameter(index);
    return 1;
}


static int sendStatusString(lua_State *L)
{
    size_t l;
    const int index = luaL_checkint(L, 1);
    char *data = luaL_checklstring(L, 2, &l);
    CHAR_sendStatusString(index, data);
    return 1;
}

static int sendBattleEffect(lua_State *L)
{
    const int index = luaL_checkint(L, 1);
    const int val = luaL_checkint(L, 2);
    CHAR_sendBattleEffect(index, val);
    return 1;
}

static int sendWatchEvent(lua_State *L)
{
    const int objindex = luaL_checkint(L, 1);
    const int action = getCharBaseValue(L, 2, CharBaseAction, arraysizeof(CharBaseAction));
    const int flg = luaL_checkint(L, 3);
    luaL_checktype(L, 4, LUA_TTABLE);
    int n = luaL_getn(L, 4);
    int *opt = (int *) malloc(n * sizeof(int));
    int i;

    for (i = 0; i < n; i++)
    {
        opt[i] = getArrayInt(L, i);
    }

    CHAR_sendWatchEvent(objindex,   action, opt, n, flg);
    return 1;
}

static int dropPetAbsolute(lua_State *L)
{
    const int index = luaL_checkint(L, 1);
    const int fl = luaL_checkint(L, 2);
    const int fx = luaL_checkint(L, 3);
    const int fy = luaL_checkint(L, 4);
    const int dir = luaL_checkint(L, 5);
    const int val = luaL_checkint(L, 6);
    int dirx[9], diry[9];
    int floor, x, y, i;

    for (i  = 0 ; i < 7 ; i  ++)
    {
        dirx[i + 2] = CHAR_getDX(dir + i + 1);
        diry[i + 2] = CHAR_getDY(dir + i + 1);
    }

    dirx[0] = CHAR_getDX(dir);
    diry[0] = CHAR_getDY(dir);
    dirx[1] = 0;
    diry[1] = 0;
    floor = fl;

    for (i = 0 ; i < 9 ; i ++)
    {
        int x = fx + dirx[i];
        int y = fy + diry[i];

        if (PET_isPutPoint(floor, x, y) == TRUE)
        {
            break;
        }
    }

    if (i == 9)
    {
        i = 1;
    }

    x = fx + dirx[i];
    y = fy + diry[i];
    int objindex = PET_dropPetAbsolute(index, floor, x, y, val);

    if (objindex == -1)
    {
        return 1;
    }

    CHAR_setWorkInt(index, CHAR_WORKOBJINDEX, objindex);
    CHAR_setInt(index, CHAR_FLOOR, floor);
    CHAR_setInt(index, CHAR_X, x);
    CHAR_setInt(index, CHAR_Y, y);
    CHAR_setInt(index, CHAR_PUTPETTIME, NowTime.tv_sec);
    CHAR_sendCToArroundCharacter(objindex);
    return 1;
}

static int AllWarpToSpecificPoint(lua_State *L)
{
    const int charaindex = luaL_checkint(L, 1);
    const int floor = luaL_checkint(L, 2);
    const int x = luaL_checkint(L, 3);
    const int y = luaL_checkint(L, 4);

    if (CHAR_getWorkInt(charaindex, CHAR_WORKPARTYMODE) == CHAR_PARTY_LEADER)
    {
        int i;

        for (i = 0; i < getPartyNum(charaindex); i ++)
        {
            int subindex = CHAR_getWorkInt(charaindex, CHAR_WORKPARTYINDEX1 + i);

            if (CHAR_CHECKINDEX(subindex) == FALSE)
            {
                continue;
            }

            CHAR_warpToSpecificPoint(subindex, floor, x, y);
            ITEM_WarpDelErrorItem(subindex);
        }
    }
    else if (CHAR_getWorkInt(charaindex, CHAR_WORKPARTYMODE) == CHAR_PARTY_CLIENT)
    {
        CHAR_talkToCli(charaindex, -1, "队员无法使用。", CHAR_COLORYELLOW);
        return FALSE;
    }
    else if (CHAR_getWorkInt(charaindex, CHAR_WORKPARTYMODE) == CHAR_PARTY_NONE)
    {
        ITEM_WarpDelErrorItem(charaindex);
        CHAR_warpToSpecificPoint(charaindex, floor, x, y);
    }

    return 1;
}

static int Findpet(lua_State *L)
{
    const int charaindex = luaL_checkint(L, 1);
    const int petid = luaL_checkint(L, 2);
    const int lv = luaL_checkint(L, 3);
    int i;

    for (i = 0; i < CHAR_MAXPETHAVE; i ++)
    {
        int petindex = CHAR_getCharPet(charaindex, i);

        if (!CHAR_CHECKINDEX(petindex))
        {
            continue;
        }

        if (CHAR_getInt(petindex, CHAR_PETID) == petid)
        {
            if (lv > 0)
            {
                if (CHAR_getInt(petindex, CHAR_LV) != lv)
                {
                    continue;
                }
            }

            lua_pushinteger(L, petindex);
            return 1;
        }
    }

    lua_pushinteger(L, -1);
    return 1;
}

static int FindPetFormMatemo(lua_State *L)
{
    const int charaindex = luaL_checkint(L, 1);
    const int metamo = luaL_checkint(L, 2);
    const int lv = luaL_checkint(L, 3);
    int i;

    for (i = 0; i < CHAR_MAXPETHAVE; i ++)
    {
        int petindex = CHAR_getCharPet(charaindex, i);

        if (!CHAR_CHECKINDEX(petindex))
        {
            continue;
        }

        if (CHAR_getInt(petindex, CHAR_BASEBASEIMAGENUMBER) == metamo)
        {
            if (lv > 0)
            {
                if (CHAR_getInt(petindex, CHAR_LV) != lv)
                {
                    continue;
                }
            }

            lua_pushinteger(L, petindex);
            return 1;
        }
    }

    lua_pushinteger(L, -1);
    return 1;
}

static int FindPetFormEnemyTempID(lua_State *L)
{
    size_t l;
    const int charaindex = luaL_checkint(L, 1);
    char *data = luaL_checklstring(L, 2, &l);
    char token[64];
    int enemytempid = -1, lv = 0;
    int i;

    if (getStringFromIndexWithDelim(data, "-", 1, token, sizeof(token)) == TRUE)
    {
        lv = atoi(token);
    }
    else
    {
        lua_pushinteger(L, -1);
    }

    if (getStringFromIndexWithDelim(data, "-", 2, token, sizeof(token)) == TRUE)
    {
        enemytempid = atoi(token);
    }
    else
    {
        lua_pushinteger(L, -1);
    }

    for (i = 0; i < CHAR_MAXPETHAVE; i ++)
    {
        int petindex = CHAR_getCharPet(charaindex, i);

        if (!CHAR_CHECKINDEX(petindex))
        {
            continue;
        }

        if (CHAR_getInt(petindex, CHAR_PETID) == enemytempid)
        {
            if (lv > 0)
            {
                if (CHAR_getInt(petindex, CHAR_LV) != lv)
                {
                    continue;
                }
            }

            lua_pushinteger(L, petindex);
            return 1;
        }
    }

    lua_pushinteger(L, -1);
    return 1;
}

static int DelPet(lua_State *L)
{
    const int charaindex = luaL_checkint(L, 1);
    const int petindex = luaL_checkint(L, 2);
    int i;
    char category[12];

    for (i = 0; i < CHAR_MAXPETHAVE; i ++)
    {
        if (petindex == CHAR_getCharPet(charaindex, i))
        {
            if (CHAR_getInt(charaindex, CHAR_RIDEPET) == i)
            {
                CHAR_setInt(charaindex, CHAR_RIDEPET, -1);
                CHAR_send_P_StatusString(charaindex, CHAR_P_STRING_RIDEPET);
                CHAR_complianceParameter(charaindex);
                CHAR_sendCToArroundCharacter(CHAR_getWorkInt(charaindex, CHAR_WORKOBJINDEX));
            }

            LogPet(CHAR_getChar(charaindex, CHAR_NAME),    // 平乓仿
                   CHAR_getChar(charaindex, CHAR_CDKEY),
                   CHAR_getChar(petindex, CHAR_NAME),
                   CHAR_getInt(petindex, CHAR_LV),
                   "EvnetDell(LUA任务删除)",
                   CHAR_getInt(charaindex, CHAR_FLOOR),
                   CHAR_getInt(charaindex, CHAR_X),
                   CHAR_getInt(charaindex, CHAR_Y),
                   CHAR_getChar(petindex, CHAR_UNIQUECODE)    // shan 2001/12/14
                  );
            CHAR_endCharOneArray(petindex);
            CHAR_setCharPet(charaindex, i, -1);
            snprintf(category, sizeof(category), "K%d", i);
            CHAR_sendStatusString(charaindex, category);
            break;
        }
    }

    return 1;
}

#endif

#ifdef _ALLBLUES_LUA_1_7
static int findEmptyItemBox(lua_State *L)
{
    const int charaindex = luaL_checkint(L, 1);
    lua_pushinteger(L, CHAR_findEmptyItemBox(charaindex));
    return 1;
}

static int findEmptyPetBox(lua_State *L)
{
    const int charaindex = luaL_checkint(L, 1);
    int num = 0;
    int i;

    for (i = 0; i < CHAR_MAXPETHAVE; i ++)
    {
        int petindex = CHAR_getCharPet(charaindex, i);

        if (!CHAR_CHECKINDEX(petindex))
        {
            num ++ ;
        }
    }

    lua_pushinteger(L, num);
    return 1;
}

static int dropPetFollow(lua_State *L)
{
    const int charaindex = luaL_checkint(L, 1);
    const int havepetindex = luaL_checkint(L, 2);
    PET_dropPetFollow(charaindex, havepetindex, -1, -1, -1);
    return 1;
}

static int getItemIndex(lua_State *L)
{
    const int charaindex = luaL_checkint(L, 1);
    const int haveitemindex = luaL_checkint(L, 2);
    lua_pushinteger(L, CHAR_getItemIndex(charaindex, haveitemindex));
    return 1;
}

static int setItemIndex(lua_State *L)
{
    const int charaindex = luaL_checkint(L, 1);
    const int emptyindex = luaL_checkint(L, 2);
    const int itemindex = luaL_checkint(L, 3);
    CHAR_setItemIndex(charaindex, emptyindex, itemindex);
    CHAR_sendItemDataOne(charaindex, emptyindex);
    return 1;
}

static int getPoolItemIndex(lua_State *L)
{
    const int charaindex = luaL_checkint(L, 1);
    const int haveitemindex = luaL_checkint(L, 2);
    lua_pushinteger(L, CHAR_getPoolItemIndex(charaindex, haveitemindex));
    return 1;
}

static int charSaveFromConnect(lua_State *L)
{
    const int charaindex = luaL_checkint(L, 1);

    if (CHAR_getCharUse(charaindex) != FALSE)
    {
        CHAR_charSaveFromConnect(charaindex, FALSE);
    }

    return 1;
}

#ifdef _RECORD_IP
static int userip(lua_State *L)
{
    const int charaindex = luaL_checkint(L, 1);
    unsigned long ip;
    int a, b, c, d;
    char strIP[32];
    ip = CHAR_getWorkInt(charaindex, CHAR_WORK_RECORD_IP);
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
#endif

static int DischargeParty(lua_State *L)
{
    const int charaindex = luaL_checkint(L, 1);
    const int flg = luaL_checkint(L, 2);
    CHAR_DischargeParty(charaindex, flg);
    return 1;
}


static int Skillupsend(lua_State *L)
{
    const int charaindex = luaL_checkint(L, 1);
    CHAR_Skillupsend(charaindex);
    return 1;
}
#endif

#ifdef _ALLBLUES_LUA_1_9
static int logou(lua_State *L)
{
    const int charaindex = luaL_checkint(L, 1);
    CHAR_logout(charaindex, TRUE);
    return 1;
}
static int copyChar(lua_State *L)
{
    const int fromindex = luaL_checkint(L, 1);
    const int toindex = luaL_checkint(L, 2);

    if (!CHAR_CHECKINDEX(fromindex))
    {
        return 1;
    }

    if (!CHAR_CHECKINDEX(toindex))
    {
        return 1;
    }

    CHAR_setInt(toindex, CHAR_BASEIMAGENUMBER, CHAR_getInt(fromindex, CHAR_BASEIMAGENUMBER));
    CHAR_setInt(toindex, CHAR_BASEBASEIMAGENUMBER, CHAR_getInt(fromindex, CHAR_BASEBASEIMAGENUMBER));
    CHAR_setInt(toindex, CHAR_TRANSMIGRATION, CHAR_getInt(fromindex, CHAR_TRANSMIGRATION));
    CHAR_setInt(toindex, CHAR_LV, CHAR_getInt(fromindex, CHAR_LV));
    CHAR_setInt(toindex, CHAR_HP, CHAR_getInt(fromindex, CHAR_HP));
    CHAR_setInt(toindex, CHAR_MP, CHAR_getInt(fromindex, CHAR_MP));
    CHAR_setInt(toindex, CHAR_MAXMP, CHAR_getInt(fromindex, CHAR_MAXMP));
    CHAR_setInt(toindex, CHAR_VITAL, CHAR_getInt(fromindex, CHAR_VITAL));
    CHAR_setInt(toindex, CHAR_STR, CHAR_getInt(fromindex, CHAR_STR));
    CHAR_setInt(toindex, CHAR_TOUGH, CHAR_getInt(fromindex, CHAR_TOUGH));
    CHAR_setInt(toindex, CHAR_DEX, CHAR_getInt(fromindex, CHAR_DEX));
    CHAR_setInt(toindex, CHAR_ALLOCPOINT, CHAR_getInt(fromindex, CHAR_ALLOCPOINT));
    CHAR_setInt(toindex, CHAR_PETRANK, CHAR_getInt(fromindex, CHAR_PETRANK));
    CHAR_setInt(toindex, CHAR_LIMITLEVEL, CHAR_getInt(fromindex, CHAR_LIMITLEVEL));
    CHAR_setChar(toindex, CHAR_NAME, CHAR_getChar(fromindex, CHAR_NAME));
    CHAR_setChar(toindex, CHAR_OWNTITLE, CHAR_getChar(fromindex, CHAR_OWNTITLE));
    CHAR_complianceParameter(toindex);
    return 1;
}

static int talkToAllServer(lua_State *L)
{
    size_t l;
    char *message = luaL_checklstring(L, 1, &l);
#ifdef _ITEM_PET_TALK_
#ifdef _NEW_STONEAGE_TYPE
    char *itempetbuff = luaL_checklstring(L, 2, &l);
#endif
#endif
#ifdef _PLAYER_UID
    char *uid = luaL_checklstring(L, 3, &l);
#endif
    saacproto_AllServSend_send(message
#ifdef _ITEM_PET_TALK_
#ifdef _NEW_STONEAGE_TYPE
                               , itempetbuff
#else
                               , ""
#endif // _NEW_STONEAGE_TYPE
#endif
#ifdef _PLAYER_UID
                               , uid
#endif
#ifdef _YAYA_VOICE
                               , 0
#endif
                              );
    return 1;
}

#endif

static int earnFame(lua_State *L)
{
    const int charaindex = luaL_checkint(L, 1);
    const int num = luaL_checkint(L, 2);
    CHAR_earnFame(charaindex, num);
    return 1;
}

static int Encounter(lua_State *L)
{
    const int charaindex = luaL_checkint(L, 1);

    if (CHAR_CHECKINDEX(charaindex) == FALSE)
    {
        return 0;
    }

    int fd = CHAR_getWorkInt(charaindex, CHAR_WORKFD);
    setStayEncount(fd);
#ifdef _USER_CHARLOOPS
    {
        Char    *ch;
        ch  = CHAR_getCharPointer(charaindex);

        if (ch == NULL)
        {
            return 0;
        }

        strcpysafe(ch->charfunctable[CHAR_LOOPFUNCTEMP1].string,
                   sizeof(ch->charfunctable[CHAR_LOOPFUNCTEMP1]), "CHAR_BattleStayLoop"); //战斗
        CHAR_setInt(charaindex, CHAR_LOOPINTERVAL, getBattleSpeed());
        CHAR_constructFunctable(charaindex);
    }
#endif
    return 1;
}

static int StopEncounter(lua_State *L)
{
    const int charaindex = luaL_checkint(L, 1);

    if (CHAR_CHECKINDEX(charaindex) == FALSE)
    {
        return 0;
    }

    int fd = CHAR_getWorkInt(charaindex, CHAR_WORKFD);
    clearStayEncount(fd);
    return 1;
}

static int getCharStayEncount(lua_State *L)
{
    const int charaindex = luaL_checkint(L, 1);

    if (CHAR_CHECKINDEX(charaindex) == FALSE)
    {
        lua_pushinteger(L, 0);
    }
    else
    {
        int fd = CHAR_getWorkInt(charaindex, CHAR_WORKFD);
        lua_pushinteger(L, getStayEncount(fd));
    }

    return 1;
}

static int getDX(lua_State *L)
{
    size_t l;
    char *diy = luaL_checklstring(L, 1, &l);
    lua_pushinteger(L, CHAR_getDX(diy));
    return 1;
}




static int getDY(lua_State *L)
{
    size_t l;
    char *dir = luaL_checklstring(L, 1, &l);
    lua_pushinteger(L, CHAR_getDY(dir));
    return 1;
}

static int sendAction(lua_State *L)
{
    const int charaindex = luaL_checkint(L, 1);
    const int actionno = luaL_checkint(L, 2);
    const int flg = luaL_checkint(L, 3);
    CHAR_sendAction(charaindex, actionno, flg);
    return 1;
}

static int GetOldLevelExp(lua_State *L)
{
    const int level = luaL_checkint(L, 1);
    lua_pushinteger(L, CHAR_GetOldLevelExp(level));
    return 1;
}

#ifdef _PROFESSION_SKILL            // WON ADD 人物职业技能
static int getCharSkill(lua_State *L)
{
    const int index = luaL_checkint(L, 1);
    const int sindex = luaL_checkint(L, 2);
    lua_pushinteger(L, CHAR_getCharSkill(index, sindex));
    return 1;
}


static int setCharSkill(lua_State *L)
{
    const int index = luaL_checkint(L, 1);
    const int sindex = luaL_checkint(L, 2);
    const int data = luaL_checkint(L, 3);

    if (CHAR_setCharSkill(index, sindex, data) == -1)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

static int getCharSkillLevel(lua_State *L)
{
    const int index = luaL_checkint(L, 1);
    const int sindex = luaL_checkint(L, 2);
    lua_pushinteger(L, CHAR_getCharSkillLevel(index, sindex));
    return 1;
}

static int setCharSkillLevel(lua_State *L)
{
    const int index = luaL_checkint(L, 1);
    const int sindex = luaL_checkint(L, 2);
    const int data = luaL_checkint(L, 3);
    CHAR_setCharSkillLevel(index, sindex, data);
    return 1;
}

static int ProfeesionSkillAdd(lua_State *L)
{
    const int index = luaL_checkint(L, 1);
    const int sindex = luaL_checkint(L, 2);
    const int level = luaL_checkint(L, 3);
    lua_pushinteger(L, PROFESSION_SKILL_ADDSK(index, sindex, level));
    return 1;
}

static int DelSProfeesionSkill(lua_State *L)
{
    const int index = luaL_checkint(L, 1);
    const int sindex = luaL_checkint(L, 2);
    CHAR_HaveSkill *pSkil;
    pSkil = CHAR_getCharHaveSkill(index, sindex);
    SKILL_makeSkillData(&pSkil->skill, 0, 0);
    pSkil->use = 0;
    return 1;
}


#endif

static int DropPet(lua_State *L)
{
    const int index = luaL_checkint(L, 1);
    const int havepetindex = luaL_checkint(L, 2);
    PET_dropPet(index, havepetindex);
    return 1;
}

static int getCoordinationDir(lua_State *L)
{
    const int dir = luaL_checkint(L, 1);
    const int x = luaL_checkint(L, 2);
    const int y = luaL_checkint(L, 3);
    const int c = luaL_checkint(L, 4);
    const int type = luaL_checkint(L, 5);
    int out = -1;

    if (type == 1)
    {
        out = x + CHAR_getDX(dir) * c;
    }
    else
    {
        out = y + CHAR_getDY(dir) * c;
    }

    lua_pushinteger(L, out);
    return 1;
}

static int getSameCoordinateObjects(lua_State *L)
{
    int objbuf[16];
    const int floorid = luaL_checkint(L, 1);
    const int x = luaL_checkint(L, 2);
    const int y = luaL_checkint(L, 3);
    const int type = luaL_checkint(L, 4);
    int out = CHAR_getSameCoordinateObjects(objbuf, arraysizeof(objbuf), floorid, x, y);

    if (type == 1)
    {
        lua_pushinteger(L, out);
    }
    else
    {
        char tempbuf[1024];
        memset(tempbuf, 0, sizeof(tempbuf));
        int i;

        for (i = 0; i < out; i++)
        {
            if (i == 0)
            {
                sprintf(tempbuf, "%d", objbuf[i]);
            }
            else
            {
                sprintf(tempbuf, "%s|%d", tempbuf, objbuf[i]);
            }
        }

        lua_pushstring(L, tempbuf);
    }

    return 1;
}

static int lua_getMyMaxPilenum(lua_State *L)
{
    int charaindex = luaL_checkint(L, 1);
    lua_pushinteger(L, CHAR_getMyMaxPilenum(charaindex));
    return 1;
}

static int PileItemFromItemBoxToItemBox(lua_State *L)
{
    int charaindex = luaL_checkint(L, 1);
    int from = luaL_checkint(L, 2);
    int to = luaL_checkint(L, 3);
    lua_pushinteger(L, CHAR_PileItemFromItemBoxToItemBox(charaindex, from, to));
    return 1;
}
#ifdef  _PETITEM_TALKSHOW
static int LuaMakePetItemTalkShow(lua_State *L)
{
    size_t l;
    const int charaindex = luaL_checkint(L, 1);
    char *messagebuff = luaL_checklstring(L, 2, &l);
    extern char* MakePetItemTalkShow(int charaindex, char* talkbuff);
    char *outbuff = MakePetItemTalkShow(charaindex, messagebuff);
    lua_pushstring(L, outbuff);
    return 1;
}
#endif
static int PetTrans(lua_State *L)
{
    extern int NPC_PetTransManStatusNoMM(int toindex, int petNo, int vital1, int str1, int tgh1, int dex1);
    const int index = luaL_checkint(L, 1);
    const int petindex = luaL_checkint(L, 2);
    const int vital = luaL_checkint(L, 3);
    const int str = luaL_checkint(L, 4);
    const int tgh = luaL_checkint(L, 5);
    const int dex = luaL_checkint(L, 6);
    lua_pushinteger(L, NPC_PetTransManStatusNoMM(index, petindex, vital, str, tgh, dex));
    return 1;
}
static int talkToServerEx(lua_State *L)
{
    size_t l;
    const int talkcharaindex = luaL_checkint(L, 1);
    char *message = luaL_checklstring(L, 2, &l);
    const int color = getCharBaseValue(L, 3, CharBaseColor, arraysizeof(CharBaseColor));
    const int fontsize = luaL_checkint(L, 4);
    int i = 0;
    int playernum = CHAR_getPlayerMaxNum();

    for (i = 0; i < playernum; i++)
    {
        if (CHAR_getCharUse(i) == FALSE)
        {
            continue;
        }

        if (!CHAR_CHECKINDEX(i))
        {
            continue;
        }

        CHAR_talkToCliExt(i, talkcharaindex, message, color, fontsize
#ifdef _ITEM_PET_TALK_
                          , ""
#endif
#ifdef _YAYA_VOICE
                          , 0
#endif
                         );
    }

    return 1;
}

#ifdef _ADDFMBATTLE_LUA_
static int setFmTime(lua_State *L)
{
    const int index = luaL_checkint(L, 1);
    const int day = luaL_checkint(L, 2);
    extern void setFmBattleTime(int index, int day);
    setFmBattleTime(index, day);
    lua_pushinteger(L, 0);
    return 1;
}
#endif

#ifdef _SPEED_CHECK_
EXTERN struct timeval NowTime;
static int setCharSpeed(lua_State *L)
{
    int index = luaL_checkint(L, 1);
    CHAR_setWorkInt(index, CHAR_WORK_CHECK_SPEED_SEC, NowTime.tv_sec);
    CHAR_setWorkInt(index, CHAR_WORK_CHECK_SPEED_USEC, NowTime.tv_usec);
    return 1;
}

static int getCharSpeed(lua_State *L)
{
    struct timeval old;
    unsigned int timediff_us;
    int index = luaL_checkint(L, 1);
    old.tv_sec = CHAR_getWorkInt(index, CHAR_WORK_CHECK_SPEED_SEC);
    old.tv_usec = CHAR_getWorkInt(index, CHAR_WORK_CHECK_SPEED_USEC);
    timediff_us = time_diff_us(NowTime, old);
    lua_pushinteger(L, timediff_us);
    return 1;
}
#endif

static int charSavePetToString(lua_State *L)
{
    int petindex = luaL_checkint(L, 1);

    if (CHAR_CHECKINDEX(petindex))
    {
        if (CHAR_getInt(petindex, CHAR_WHICHTYPE) == CHAR_TYPEPET)
        {
            char *petdata = CHAR_makePetStringFromPetIndex(petindex);
            lua_pushstring(L, petdata);
        }
    }

    return 1;
}

static int charLoadPetToString(lua_State *L)
{
    int charaindex = luaL_checkint(L, 1);
    int petpos = CHAR_getCharPetElement(charaindex);

    if (petpos > -1)
    {
        char * petdata = (char *)lua_tostring(L, 2);
        Char   petone;
        int petindex;
        BOOL ret = CHAR_makePetFromStringToArg(petdata, &petone, 0);

        if (ret == TRUE)
        {
            petindex = PET_initCharOneArray(&petone);

            if (petindex < 0)
            {
                return 0;
            }

            CHAR_setCharPet(charaindex, petpos, petindex);
            CHAR_setWorkInt(petindex, CHAR_WORKPLAYERINDEX, charaindex);
            CHAR_complianceParameter(petindex);
#ifdef _XIUFU_PETMYSQL_
            extern int StrReplace(char* strRes, char* from, char* to);
            char * petBlessData = CHAR_getChar(petindex, CHAR_NEWNAME);

            if (*petBlessData)
            {
                if (!strstr(petBlessData, "|"))
                {
                    StrReplace(petBlessData, "c", ",");
                    StrReplace(petBlessData, "z", "|");
                }
            }

#endif
            char category[12];
            snprintf(category, sizeof(category), "K%d", petpos);
            CHAR_sendStatusString(charaindex, category);
            return 1;
        }
    }

    return 0;
}


static int charSaveItemToString(lua_State *L)
{
    int itemindex = luaL_checkint(L, 1);

    if (ITEM_CHECKINDEX(itemindex))
    {
        char *petdata = ITEM_makeStringFromItemIndex(itemindex, 0);
        lua_pushstring(L, petdata);
    }

    return 1;
}

static int charLoadItemToString(lua_State *L)
{
    int charaindex = luaL_checkint(L, 1);
    char * itemdata = (char *)lua_tostring(L, 2);
    int ti = CHAR_findEmptyItemBox(charaindex);

    if (ti != -1)
    {
        ITEM_Item   itmone;
        int itemindex;
        BOOL ret = ITEM_makeExistItemsFromStringToArg(itemdata, &itmone, 0);

        if (ret == TRUE)
        {
            itemindex = ITEM_initExistItemsOne(&itmone);

            if (itemindex < 0)
            {
                return 0;
            }

            CHAR_setItemIndex(charaindex, ti, itemindex);
            CHAR_sendItemDataOne(charaindex, ti);
            return 1;
        }
    }

    return 1;
}


static int chargetPoolItemIndex(lua_State *L)
{
    int charaindex = luaL_checkint(L, 1);
    int index = luaL_checkint(L, 2);

    if (CHAR_CHECKINDEX(charaindex))
    {
        int itemindex = CHAR_getPoolItemIndex(charaindex, index);
        lua_pushinteger(L, itemindex);
    }
    else
    {
        lua_pushinteger(L, -1);
    }

    return 1;
}


static int pickupFollowPet(lua_State *L)
{
    int charaindex = luaL_checkint(L, 1);
    int index = luaL_checkint(L, 2);

    if (CHAR_CHECKINDEX(charaindex))
    {
        if (CHAR_pickupFollowPet(charaindex, index))
        {
            lua_pushinteger(L, 1);
            return 1;
        }
    }

    lua_pushinteger(L, 0);
    return 1;
}


static int GetEmptyItemBoxNum(lua_State *_NLL)
{
    const int TM_CharIndex = luaL_checkint(_NLL, 1);

    if (!CHAR_CHECKINDEX(TM_CharIndex))
    {
        lua_pushinteger(_NLL, -1);
        return 0;
    }

    int i = CHAR_EQUIPPLACENUM;
    int EmptyNum = 0;

    for (; i < CHAR_MAXITEMHAVE; i++)
    {
        if (CHAR_getItemIndex(TM_CharIndex, i) == -1)
        {
            EmptyNum++;
        }
    }

    lua_pushinteger(_NLL, EmptyNum);
    return 1;
}

#ifdef _NEW_SHOP_
int Lua_WriteShopData(lua_State *L)
{
    const int id = luaL_checkint(L, 1);
    size_t l;
    const char *data = luaL_checklstring(L, 2, &l);
    WriteShopData(data, id);
    return 0;
}
#endif

static int UpCahrData(lua_State *L)
{
    const int charaindex = luaL_checkint(L, 1);
    size_t l;
    const char *data = luaL_checklstring(L, 2, &l);
    CHAR_sendStatusString(charaindex, data);
    return 1;
}




int UpChar(lua_State *_NLL)
{
    int TM_Index = (int)luaL_checkint(_NLL, 1);

    if (!CHAR_CHECKINDEX(TM_Index))
    {
        return 0;
    }

    CHAR_complianceParameter(TM_Index);

    if (CHAR_getInt(TM_Index, CHAR_WHICHTYPE) == CHAR_TYPEPET)
    {
        int playindex = CHAR_getWorkInt(TM_Index, CHAR_WORKPLAYERINDEX);

        if (!CHAR_CHECKINDEX(playindex))
        {
            return 0;
        }
        else
        {
            int i;

            for (i = 0; i < CHAR_MAXPETHAVE; i++)
            {
                int workindex = CHAR_getCharPet(playindex, i);

                if (workindex == TM_Index)
                {
                    CHAR_send_K_StatusString(playindex, i, CHAR_K_STRING_BASEIMAGENUMBER |
                                             CHAR_K_STRING_HP |
                                             CHAR_K_STRING_MAXHP |
                                             CHAR_K_STRING_MP |
                                             CHAR_K_STRING_MAXMP |
                                             CHAR_K_STRING_EXP |
                                             CHAR_K_STRING_NEXTEXP |
                                             CHAR_K_STRING_LV |
                                             CHAR_K_STRING_ATK |
                                             CHAR_K_STRING_DEF |
                                             CHAR_K_STRING_QUICK |
                                             CHAR_K_STRING_AI |
                                             CHAR_K_STRING_EARTH |
                                             CHAR_K_STRING_WATER |
                                             CHAR_K_STRING_FIRE |
                                             CHAR_K_STRING_WIND |
                                             CHAR_K_STRING_SLOT |
                                             CHAR_K_STRING_CHANGENAMEFLG |
                                             CHAR_K_STRING_NAME |
                                             CHAR_K_STRING_USERPETNAME);
                    char msgbuf[128];
                    snprintf(msgbuf, sizeof(msgbuf), "W%d", i);
                    CHAR_sendStatusString(playindex, msgbuf);
                    CHAR_sendStatusString(playindex, "P");
                    break;
                }
            }
        }
    }
    else if (CHAR_getInt(TM_Index, CHAR_WHICHTYPE) == CHAR_TYPEPLAYER)
    {
        CHAR_send_P_StatusString(TM_Index, CHAR_P_STRING_GOLD);
    }

    if (CHAR_getFlg(TM_Index, CHAR_ISVISIBLE) == FALSE)
    {
        int TM_objindex = CHAR_getWorkInt(TM_Index, CHAR_WORKOBJINDEX);
        CHAR_sendCDArroundChar
        (
            OBJECT_getFloor(TM_objindex),
            OBJECT_getX(TM_objindex),
            OBJECT_getY(TM_objindex),
            TM_objindex
        );
    }
    else
    {
        CHAR_sendCToArroundCharacter(CHAR_getWorkInt(TM_Index, CHAR_WORKOBJINDEX));
    }

    if (CHAR_getWorkInt(TM_Index, CHAR_WORKPARTYMODE) == CHAR_PARTY_LEADER)
    {
        CHAR_sendLeader(CHAR_getWorkInt(TM_Index, CHAR_WORKOBJINDEX), 1);
    }

    return 1;
}

static int newMessageToCli(lua_State *L)
{
    size_t l;
    const int talkedcharaindex = luaL_checkint(L, 1);
    const int talkcharaindex = luaL_checkint(L, 2);
    char *message = luaL_checklstring(L, 3, &l);
    int color = getCharBaseValue(L, 4, CharBaseColor, arraysizeof(CharBaseColor));

    if (color == CHAR_COLORNUM)
    {
        color = RAND(0, CHAR_COLORNUM - 2);
    }

    CHAR_newMessageToCli(talkedcharaindex, talkcharaindex, message, color);
    return 1;
}

#ifdef _NEW_515SA
static int QuickAlert(lua_State *L)
{
    const int charaindex = luaL_checkint(L, 1);
    size_t l;
    const char *data = luaL_checklstring(L, 2, &l);
    CHAR_newMessageToCli(-1, charaindex, data, 0);
    return 1;
}

static int Alert(lua_State *L)
{
    const int charaindex = luaL_checkint(L, 1);
    size_t l;
    const char *data = luaL_checklstring(L, 2, &l);
    const int color = luaL_checkint(L, 3);
    const int countdown = luaL_checkint(L, 4);
    CHAR_newMessageToCli(-1, charaindex, data, color);
    return 1;
}

#endif
static int moveEquipItem(lua_State *L)
{
    const int charaindex = luaL_checkint(L, 1);
    const int fromid = luaL_checkint(L, 2);
    const int toid = luaL_checkint(L, 3);
    CHAR_moveEquipItem(charaindex, fromid, toid);
    return 1;
}

static int checkPlayerImageNumber(lua_State *L)
{
    const int imageid = luaL_checkint(L, 1);
    lua_pushinteger(L, CHAR_checkPlayerImageNumber(imageid));
    return 1;
}

static int checkFaceImageNumber(lua_State *L)
{
    const int imageid = luaL_checkint(L, 1);
    const int faceid = luaL_checkint(L, 2);
    lua_pushinteger(L, CHAR_checkFaceImageNumber(imageid, faceid));
    return 1;
}

static int getMaxHaveGold(lua_State *L)
{
    const int charaindex = luaL_checkint(L, 1);
    lua_pushinteger(L, CHAR_getMaxHaveGold(charaindex));
    return 1;
}

#ifdef _PET_NOZHUFU_TYPE
static int getPet4v(lua_State *L)
{
    const int vital = luaL_checkint(L, 1);
    const int str = luaL_checkint(L, 2);
    const int tough = luaL_checkint(L, 3);
    const int dex = luaL_checkint(L, 4);
    const int zhuhp = luaL_checkint(L, 5);
    const int zhustr = luaL_checkint(L, 6);
    const int zhutgh = luaL_checkint(L, 7);
    const int zhudex = luaL_checkint(L, 8);
    const int hp = ((vital - zhuhp * 100) * 4
                    + (str - zhustr * 100) * 1
                    + (tough - zhutgh * 100) * 1
                    + (dex - zhudex * 100) * 1) * 0.01;
    const int strint = (str - zhustr * 100) * 0.01 * 1.0
                       + (tough - zhutgh * 100) * 0.01 * 0.1
                       + (vital - zhuhp * 100) * 0.01 * 0.1
                       + (dex - zhudex * 100) * 0.01 * 0.05;
    const int tghint = (tough - zhutgh * 100) * 0.01 * 1.0
                       + (str - zhustr * 100) * 0.01 * 0.1
                       + (vital - zhuhp * 100) * 0.01 * 0.1
                       + (dex - zhudex * 100) * 0.01 * 0.05;
    const int dexint = (dex - zhudex * 100) * 0.01;
    char pet4vbuff[64];
    memset(pet4vbuff, 0, sizeof(pet4vbuff));
    sprintf(pet4vbuff, "%d|%d|%d|%d", hp, strint, tghint, dexint);
    lua_pushstring(L, pet4vbuff);
    return 1;
}
#endif

static int WarpElderPosition(lua_State *L)
{
    const int charaindex = luaL_checkint(L, 1);
    int fl = 0, x = 0, y = 0;

    if (CHAR_CHECKINDEX(charaindex))
    {
        CHAR_getElderPosition(CHAR_getInt(charaindex, CHAR_LASTTALKELDER), &fl, &x, &y);
        CHAR_warpToSpecificPoint(charaindex, fl, x, y);
    }

    return 1;
}

static int getAddressbookName(lua_State *L)
{
    const int charaindex = luaL_checkint(L, 1);
    const int i = luaL_checkint(L, 2);
    ADDRESSBOOK_entry *ae;
    ae = CHAR_getAddressbookEntry(charaindex, i);

    if (ae && ae->use == TRUE)
    {
        lua_pushstring(L, ae->charname);
    }
    else
    {
        lua_pushstring(L, "");
    }

    return 1;
}

static int getAddressbookCdkey(lua_State *L)
{
    const int charaindex = luaL_checkint(L, 1);
    const int i = luaL_checkint(L, 2);
    ADDRESSBOOK_entry *ae;
    ae = CHAR_getAddressbookEntry(charaindex, i);

    if (ae && ae->use == TRUE)
    {
        lua_pushstring(L, ae->cdkey);
    }
    else
    {
        lua_pushstring(L, "");
    }

    return 1;
}

static int getLevelExp(lua_State *L)
{
    const int charaindex = luaL_checkint(L, 1);
    const int level = luaL_checkint(L, 2);
    lua_pushinteger(L, CHAR_GetLevelExp(charaindex, level));
    return 1;
}
static int LUAgetCharBaseValue(lua_State *L)
{
    const int element = getCharBaseValue(L, 1, CharBaseInt, arraysizeof(CharBaseInt));
    lua_pushinteger(L, element);
    return 1;
}
static int ElderSetPosition(lua_State *L)
{
    const int elderindex = luaL_checkint(L, 1);
    const int fl = luaL_checkint(L, 2);
    const int x = luaL_checkint(L, 3);
    const int y = luaL_checkint(L, 4);
    CHAR_ElderSetPosition(elderindex, fl, x, y);
    return 1;
}
#ifdef _479SA
int getDateValue(int timediff)
{
    int timevalue = (int)time(NULL);
    struct tm *p;
    time_t timep;
    time(&timep);
    p = localtime(&timep); /*取得当地时间*/

    if (p->tm_hour < timediff)
    {
        timevalue = timevalue - 86400;
    }

    time_t fixtime = timevalue;
    struct tm *tm1 = localtime(&fixtime);
    int ret = (tm1->tm_year + 1900) * 10000 + (tm1->tm_mon + 1) * 100 + tm1->tm_mday;
    return ret;
}

int comPareDate(int oldvalue, int timediff)
{
    int nowvalue = getDateValue(timediff);

    if (oldvalue == nowvalue)
    {
        //      print("\ncomPareDate true old=%d new=%d",oldvalue,nowvalue);
        return TRUE;
    }

    //  print("\ncomPareDate false old=%d new=%d",oldvalue,nowvalue);
    return FALSE;
}
static int getActCount(lua_State *L)
{
    const int charaindex = luaL_checkint(L, 1);
    const int id = luaL_checkint(L, 2);
    const int timediff = luaL_checkint(L, 3);
    char value[256];
    char* data = CHAR_getChar(charaindex, CHAR_ACTIVE_MAIN + id);

    if (strlen(data) == 0)
    {
        sprintf(value, "%d|0", getDateValue(timediff));
        CHAR_setChar(charaindex, CHAR_ACTIVE_MAIN + id, value);
        lua_pushinteger(L, 0);
    }
    else
    {
        getStringFromIndexWithDelim(data, "|", 1, value, sizeof(value));
        int oldtime = atoi(value);
        int ret = getStringFromIndexWithDelim(data, "|", 2, value, sizeof(value));
        int oldcount = atoi(value);

        if (comPareDate(oldtime, timediff) == TRUE)
        {
            lua_pushinteger(L, oldcount);
        }
        else
        {
            sprintf(value, "%d|0", getDateValue(timediff));
            CHAR_setChar(charaindex, CHAR_ACTIVE_MAIN + id, value);
            lua_pushinteger(L, 0);
        }
    }

    return 1;
}

static int setActCount(lua_State *L)
{
    const int charaindex = luaL_checkint(L, 1);
    const int id = luaL_checkint(L, 2);
    const int timediff = luaL_checkint(L, 3);
    const int count = luaL_checkint(L, 4);
    char value[256];
    sprintf(value, "%d|%d", getDateValue(timediff), count);
    CHAR_setChar(charaindex, CHAR_ACTIVE_MAIN + id, value);
    return 1;
}

static int addActCount(lua_State *L)
{
    const int charaindex = luaL_checkint(L, 1);
    const int id = luaL_checkint(L, 2);
    const int timediff = luaL_checkint(L, 3);
    const int count = luaL_checkint(L, 4);
    char value[256] = { 0 };
    char* data = CHAR_getChar(charaindex, CHAR_ACTIVE_MAIN + id);

    if (strlen(data) == 0)
    {
        sprintf(value, "%d|%d", getDateValue(timediff), count);
        CHAR_setChar(charaindex, CHAR_ACTIVE_MAIN + id, value);
    }
    else
    {
        getStringFromIndexWithDelim(data, "|", 1, value, sizeof(value));
        int oldtime = atoi(value);
        getStringFromIndexWithDelim(data, "|", 2, value, sizeof(value));
        int oldcount = atoi(value);

        if (comPareDate(oldtime, timediff) == TRUE)
        {
            sprintf(value, "%d|%d", getDateValue(timediff), (count + oldcount));
            CHAR_setChar(charaindex, CHAR_ACTIVE_MAIN + id, value);
        }
        else
        {
            sprintf(value, "%d|%d", getDateValue(timediff), count);
            CHAR_setChar(charaindex, CHAR_ACTIVE_MAIN + id, value);
        }
    }

    return 1;
}

static int getActive(lua_State *L)
{
    const int charaindex = luaL_checkint(L, 1);
    const int timediff = luaL_checkint(L, 2);
    char value[256];
    char* data = CHAR_getChar(charaindex, CHAR_ACTIVE_MAIN);

    if (strlen(data) == 0)
    {
        sprintf(value, "%d|0|0|0|0|0|0", getDateValue(timediff));
        CHAR_setChar(charaindex, CHAR_ACTIVE_MAIN, value);
        lua_pushinteger(L, 0);
    }
    else
    {
        getStringFromIndexWithDelim(data, "|", 1, value, sizeof(value));
        int oldtime = atoi(value);
        getStringFromIndexWithDelim(data, "|", 2, value, sizeof(value));
        int oldcount = atoi(value);

        if (comPareDate(oldtime, timediff) == TRUE)
        {
            lua_pushinteger(L, oldcount);
        }
        else
        {
            sprintf(value, "%d|0|0|0|0|0|0", getDateValue(timediff));
            CHAR_setChar(charaindex, CHAR_ACTIVE_MAIN, value);
            lua_pushinteger(L, 0);
        }
    }

    return 1;
}

static int setActive(lua_State *L)
{
    const int charaindex = luaL_checkint(L, 1);
    const int timediff = luaL_checkint(L, 2);
    const int count = luaL_checkint(L, 3);
    char value[256];
    char* data = CHAR_getChar(charaindex, CHAR_ACTIVE_MAIN);

    if (strlen(data) == 0)
    {
        sprintf(value, "%d|%d|0|0|0|0|0", getDateValue(timediff), count);
        CHAR_setChar(charaindex, CHAR_ACTIVE_MAIN, value);
    }
    else
    {
        getStringFromIndexWithDelim(data, "|", 1, value, sizeof(value));
        int oldtime = atoi(value);
        getStringFromIndexWithDelim(data, "|", 2, value, sizeof(value));
        int oldcount = atoi(value);
        getStringFromIndexWithDelim(data, "|", 3, value, sizeof(value));
        int act1 = atoi(value);
        getStringFromIndexWithDelim(data, "|", 4, value, sizeof(value));
        int act2 = atoi(value);
        getStringFromIndexWithDelim(data, "|", 5, value, sizeof(value));
        int act3 = atoi(value);
        getStringFromIndexWithDelim(data, "|", 6, value, sizeof(value));
        int act4 = atoi(value);
        getStringFromIndexWithDelim(data, "|", 7, value, sizeof(value));
        int act5 = atoi(value);

        if (comPareDate(oldtime, timediff) == TRUE)
        {
            sprintf(value, "%d|%d|%d|%d|%d|%d|%d", getDateValue(timediff), count, act1, act2, act3, act4, act5);
            CHAR_setChar(charaindex, CHAR_ACTIVE_MAIN, value);
        }
        else
        {
            sprintf(value, "%d|%d|0|0|0|0|0", getDateValue(timediff), count);
            CHAR_setChar(charaindex, CHAR_ACTIVE_MAIN, value);
        }
    }

    return 1;
}

static int getActiveFlg(lua_State *L)
{
    const int charaindex = luaL_checkint(L, 1);
    const int timediff = luaL_checkint(L, 2);
    const int id = luaL_checkint(L, 3);
    char value[256];
    char* data = CHAR_getChar(charaindex, CHAR_ACTIVE_MAIN);

    if (strlen(data) == 0)
    {
        sprintf(value, "%d|0|0|0|0|0|0", getDateValue(timediff));
        CHAR_setChar(charaindex, CHAR_ACTIVE_MAIN, value);
        lua_pushinteger(L, 0);
    }
    else
    {
        getStringFromIndexWithDelim(data, "|", 1, value, sizeof(value));
        int oldtime = atoi(value);
        getStringFromIndexWithDelim(data, "|", 2, value, sizeof(value));
        int oldcount = atoi(value);

        if (comPareDate(oldtime, timediff) == TRUE)
        {
            getStringFromIndexWithDelim(data, "|", 2 + id, value, sizeof(value));
            lua_pushinteger(L, atoi(value));
        }
        else
        {
            sprintf(value, "%d|0|0|0|0|0|0", getDateValue(timediff));
            CHAR_setChar(charaindex, CHAR_ACTIVE_MAIN, value);
            lua_pushinteger(L, 0);
        }
    }

    return 1;
}

static int setActiveFlg(lua_State *L)
{
    const int charaindex = luaL_checkint(L, 1);
    const int timediff = luaL_checkint(L, 2);
    const int id = luaL_checkint(L, 3);
    const int count = luaL_checkint(L, 4);
    char* data = CHAR_getChar(charaindex, CHAR_ACTIVE_MAIN);
    char value[256];
    int act[5] = { 0, 0, 0, 0, 0 };
    act[id - 1] = count;

    if (strlen(data) == 0)
    {
        sprintf(value, "%d|%d|%d|%d|%d|%d|%d", getDateValue(timediff), 0, act[0], act[1], act[2], act[3], act[4]);
        CHAR_setChar(charaindex, CHAR_ACTIVE_MAIN, value);
    }
    else
    {
        getStringFromIndexWithDelim(data, "|", 1, value, sizeof(value));
        int oldtime = atoi(value);
        getStringFromIndexWithDelim(data, "|", 2, value, sizeof(value));
        int oldcount = atoi(value);

        if (comPareDate(oldtime, timediff) == TRUE)
        {
            getStringFromIndexWithDelim(data, "|", 3, value, sizeof(value));
            act[0] = atoi(value);
            getStringFromIndexWithDelim(data, "|", 4, value, sizeof(value));
            act[1] = atoi(value);
            getStringFromIndexWithDelim(data, "|", 5, value, sizeof(value));
            act[2] = atoi(value);
            getStringFromIndexWithDelim(data, "|", 6, value, sizeof(value));
            act[3] = atoi(value);
            getStringFromIndexWithDelim(data, "|", 7, value, sizeof(value));
            act[4] = atoi(value);
            act[id - 1] = count;
            sprintf(value, "%d|%d|%d|%d|%d|%d|%d", getDateValue(timediff), oldcount, act[0], act[1], act[2], act[3], act[4]);
            CHAR_setChar(charaindex, CHAR_ACTIVE_MAIN, value);
        }
        else
        {
            sprintf(value, "%d|%d|%d|%d|%d|%d|%d", getDateValue(timediff), oldcount, act[0], act[1], act[2], act[3], act[4]);
            CHAR_setChar(charaindex, CHAR_ACTIVE_MAIN, value);
        }
    }

    return 1;
}
static int ChangeCard(lua_State *L)
{
    const int charaindex = luaL_checkint(L, 1);
    const int toindex = luaL_checkint(L, 2);
    lua_pushinteger(L, ADDRESSBOOK_addEntryFromToindex(charaindex, toindex));
    return 1;
}

static int TradeForIndex(lua_State *L)
{
    const int charaindex = luaL_checkint(L, 1);
    const int toindex = luaL_checkint(L, 2);
    lua_pushinteger(L, TRADE_SearchFromIndex(CHAR_getWorkInt(charaindex, CHAR_WORKFD), charaindex, toindex));
    return 1;
}
#endif // _479SA
static const luaL_Reg charlib[] =
{
    {"getCharNum",                      getCharNum},
    {"getPlayerMaxNum",             getPlayerMaxNum},
    {"getPetMaxNum",                    getPetMaxNum},
    {"getOthersMaxNum",             getOthersMaxNum},
    {"check",                               check},
    {"setWorkChar",                     setWorkChar},
    {"getWorkChar",                     getWorkChar},
    {"setWorkInt",                      setWorkInt},
    {"getWorkInt",                      getWorkInt},
    {"setChar",                             setChar},
    {"getChar",                           getChar},
    {"setInt",                                setInt},
    {"getInt",                                getInt},
    {"setFlg",                                setFlg},
    {"getFlg",                                getFlg},
    {"setFunctionPointer",        setFunctionPointer},
    {"delFunctionPointer",        delFunctionPointer},
    {"TalkToCli",                         TalkToCli},
    {"TalkToRound",                   TalkToRound},
    {"talkToFloor",                   talkToFloor},
    {"talkToParty",                   talkToParty},
    {"talkToServer",                  talkToServer},
    {"talkToWorld",                   talkToWorld},
    {"BoundRandWalk",                 BoundRandWalk},
    {"ToAroundChar",                  ToAroundChar},
    {"AroundChar",                        AroundChar},
#ifdef _ALLBLUES_LUA_1_1
    {"Walk",                                  Walk},
    {"WalkPoint",                         WalkPoint},
    {"WarpToSpecificPoint",   WarpToSpecificPoint},
    {"MapAllWarp",                        MapAllWarp},
    {"RandRandWalk",                  RandRandWalk},
#endif
#ifdef _ALLBLUES_LUA_1_3
    {"createPet",                       createPet},
    {"setCharPet",                      setCharPet},
    {"setCharPetNew",               setCharPetNew},
    {"getCharPet",                      getCharPet},
    {"setCharPoolPet",              setCharPoolPet},
    {"getCharPoolPet",              getCharPoolPet},
    {"setDepotPetIndex",            setDepotPetIndex},
    {"getDepotPetIndex",            getDepotPetIndex},
    {"DelItem",                             DelItem},
    { "DelPileItemMess", DelPileItemMess },
    {"getFd",                               getFd},
    {"Updata",                              Updata},
    {"Additem",                             Additem},
    {"AddPet",                              AddPet},
    { "AddPetTempNo", AddPetTempNo },
    {"Finditem",                            Finditem},
#ifdef _PLAYER_NPC
    {"setPlayerNpc",                  setPlayerNpc},
#endif
    {"Coordinate",                        Coordinate},
    {"setPetSkill",                   setPetSkill},
    {"getPetSkill",                   getPetSkill},
    {"PetLevelUp",                        PetLevelUp},
    {"GMPetLevelUp",                        GMPetLevelUp},
    {"JoinParty",                         JoinParty},
#endif
#ifdef _ALLBLUES_LUA_1_4
    {"getLiftTo8",                        getLiftTo8},
    {"getRightTo8",                   getRightTo8},
    {"complianceParameter",   complianceParameter},
    {"sendStatusString",          sendStatusString},
    {"sendBattleEffect",          sendBattleEffect},
    {"dropPetAbsolute",           dropPetAbsolute},
    {"AddPetCf",                          AddPetCf},
    {"sendWatchEvent",                sendWatchEvent},
    {"AllWarpToSpecificPoint", AllWarpToSpecificPoint},
    {"Findpet",                               Findpet},
    {"DelPet",                                DelPet},
    {"FindPetFormEnemyTempID", FindPetFormEnemyTempID},
    {"FindPetFormMatemo",         FindPetFormMatemo},
#endif
#ifdef _PETSKILL_SHOP_LUA
    {"FreePetSkill",                  FreePetSkill},
#endif
#ifdef _ALLBLUES_LUA_1_7
    {"findEmptyPetBox",           findEmptyPetBox},
    {"findEmptyItemBox",          findEmptyItemBox},
    {"dropPetFollow",                 dropPetFollow},
    {"getItemIndex",                  getItemIndex},
    {"setItemIndex",                  setItemIndex},
    {"getPoolItemIndex",          getPoolItemIndex},
    {"charSaveFromConnect",   charSaveFromConnect},
    {"DischargeParty",                DischargeParty},
    {"Skillupsend",                   Skillupsend},
#ifdef _RECORD_IP
    {"userip",                                userip},
#endif
#endif
#ifdef _ALLBLUES_LUA_1_9
    {"logou",                               logou},
    {"copyChar",                            copyChar},
    {"talkToAllServer",             talkToAllServer},
#endif
    {"earnFame",                            earnFame},
    {"Encounter",                       Encounter},
    {"StopEncounter",               StopEncounter},
    {"getCharStayEncount",      getCharStayEncount},
    {"getDX",                               getDX},
    {"getDY",                               getDY},
    {"sendAction",                      sendAction},
    {"GetOldLevelExp",              GetOldLevelExp},
#ifdef _PROFESSION_SKILL            // WON ADD 人物职业技能
    {"getCharSkill",                    getCharSkill},
    {"setCharSkill",                    setCharSkill},
    { "getCharSkillLevel",                    getCharSkillLevel },
    { "setCharSkillLevel",                    setCharSkillLevel },
    { "ProfeesionSkillAdd",                    ProfeesionSkillAdd },
    {"DelSProfeesionSkill",         DelSProfeesionSkill},
#endif
    {"DropPet",         DropPet},
    {"getCoordinationDir",      getCoordinationDir},
    {"getSameCoordinateObjects",        getSameCoordinateObjects},
    {"PileItemFromItemBoxToItemBox", PileItemFromItemBoxToItemBox},
    {"getMyMaxPilenum", lua_getMyMaxPilenum},
#ifdef  _PETITEM_TALKSHOW
    {"MakePetItemTalkShow",         LuaMakePetItemTalkShow},
#endif
    {"PetTrans",            PetTrans},
    {"talkToServerEx",                  talkToServerEx},
#ifdef _ADDFMBATTLE_LUA_
    { "setFmTime", setFmTime },
#endif
#ifdef _SPEED_CHECK_
    { "setCharSpeed", setCharSpeed },
    { "getCharSpeed", getCharSpeed },
#endif
    {"charLoadItemToString", charLoadItemToString},
    {"charSaveItemToString", charSaveItemToString},
    {"charLoadPetToString", charLoadPetToString},
    {"charSavePetToString", charSavePetToString},
#ifdef _NEW_SHOP_
    { "WriteShopData", Lua_WriteShopData },
#endif
    { "pickupFollowPet", pickupFollowPet },

    { "GetEmptyItemBoxNum", GetEmptyItemBoxNum },
    { "getPoolItemIndex", chargetPoolItemIndex },
    { "UpCahrData", UpCahrData },
    { "upchar", UpChar },

    { "AddPetCf515", AddPetCf515 },
    { "newMessageToCli", newMessageToCli },
#ifdef _NEW_515SA
    { "Alert", Alert },
    { "QuickAlert", QuickAlert },
#endif
    { "moveEquipItem", moveEquipItem },
    { "checkPlayerImageNumber", checkPlayerImageNumber },
    { "checkFaceImageNumber", checkFaceImageNumber },
    { "getMaxHaveGold", getMaxHaveGold },
#ifdef _PET_NOZHUFU_TYPE
    { "getPet4v", getPet4v },
#endif
    { "WarpElderPosition", WarpElderPosition },
    { "getAddressbookName", getAddressbookName },
    { "getAddressbookCdkey", getAddressbookCdkey },
    { "getLevelExp", getLevelExp },
    { "getCharBaseValue", LUAgetCharBaseValue },
    { "ElderSetPosition", ElderSetPosition },
#ifdef _479SA
    { "getActCount",        getActCount },
    { "setActCount",        setActCount },
    { "addActCount",        addActCount },
    { "getActive",      getActive },
    { "setActive",      setActive },
    { "getActiveFlg",       getActiveFlg },
    { "setActiveFlg",       setActiveFlg },
    { "ChangeCard",         ChangeCard },
    { "TradeForIndex",      TradeForIndex },
#endif // _479SA
    { "sendCMeToMe",      sendCMeToMe },
    {NULL,                                        NULL}
};

LUALIB_API int luaopen_Char(lua_State *L)
{
    luaL_register(L, "char", charlib);
    return 1;
}

#endif
