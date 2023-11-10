#ifndef __CHAR_BASE_H__
#define __CHAR_BASE_H__

#include "version.h"
#include "skill.h"
#include "title.h"
/*#include "magicinfo.h"*/
#include "addressbook.h"
#ifdef _ALLBLUES_LUA
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
#endif
#define CHAR_DELIMITER "\n"

#define NONCHAR_DELIMITER "|"

#define STATUSSENDDELIMITER "|"

#define CHAR_MAXGOLDHAVE    (10000*10000)
#define CHAR_MAXBANKGOLDHAVE    (10000*10000)
#define CHAR_MAXFMBANKGOLDHAVE  (10000*100000)

#ifdef _NEW_MANOR_LAW
#define MAX_PERSONAL_MOMENTUM 10000000
#endif
#ifdef _PERSONAL_FAME   // Arminius 8.30: �����������
#define MAX_PERSONALFAME 100000000
#endif
#ifdef _PROFESSION_SKILL            // WON ADD ����ְҵ����
#define CHAR_SKILLMAXHAVE   26      // ְҵ��������
#else
#define CHAR_SKILLMAXHAVE   5
#endif

#ifdef _STREET_VENDOR
#define MAX_SELL_ITEM 20                    // ���߼ӳ��ﹲ������ʮ��
#endif
#define CHAR_TITLEMAXHAVE   30

#define NULLSKILL   "0"

#define CHAR_MAXPETHAVE         5
#define CHAR_MAXPERSONAGOLD     50000000

#ifdef _ADD_POOL_ITEM              // WON ADD ���ӿɼķŵĵ���
#define CHAR_MAXPOOLPETHAVE         15
#else
#define CHAR_MAXPOOLPETHAVE         10
#endif

#define CHAR_MAXPETSKILLHAVE       7
#define CHAR_PARTYMAX           5
#ifdef _PLAYER_NPC
#define CHAR_PLAYERNPCMAX       4
#endif
#define CHAR_MAXATTRIB          100
#define CHAR_MAXUPLEVEL         200
#define CHAR_MAXDUELPOINT       100000000
#define CHAR_DEFAULTMAXAI       (60)
#define CHAR_MAXVARIABLEAI      (100*100)
#define CHAR_MINVARIABLEAI      (-100*100)
#define CHAR_POISONSTRING           "�ж�"
#define CHAR_RECOVERPOISONSTRING    "�ⶾ��"
#define CHAR_PARALYSISSTRING        "�����"
#define CHAR_RECOVERPARALYSISSTRING "��������"
#define CHAR_SILENCESTRING          "�޷���ħ����"
#define CHAR_RECOVERSILENCESTRING   "������ħ����"
#define CHAR_STONESTRING            "ʯ����"
#define CHAR_RECOVERSTONESTRING     "�������"
#define CHAR_DARKNESSSTRING         "���ܲ�����"
#define CHAR_RECOVERDARKNESSSTRING  "���ܳ�����"
#define CHAR_CONFUSIONSTRING        "����ֻ�"
#define CHAR_RECOVERCONFUSIONSTRING "�ָ�����"

#ifdef _800_600
#define CHAR_DEFAULTSEESIZ  26
#define MAP_CHAR_DEFAULTSEESIZ      (CHAR_DEFAULTSEESIZ+4)
#else
#define CHAR_DEFAULTSEESIZ  20
#define MAP_CHAR_DEFAULTSEESIZ      (CHAR_DEFAULTSEESIZ+4)
#endif


typedef enum
{
    CHAR_HEAD,     //ͷ
    CHAR_BODY, //�·�
    CHAR_ARM,//����
    CHAR_DECORATION1,//����1
    CHAR_DECORATION2,//����2
#ifdef _ITEM_EQUITSPACE
    CHAR_EQBELT,    //����
    CHAR_EQSHIELD,  //��
    CHAR_EQSHOES,   //Ь��
#endif
#ifdef _EQUIT_NEWGLOVE
    CHAR_EQGLOVE,//����
#endif
    CHAR_EQUIPPLACENUM,
} CHAR_EquipPlace;


typedef enum
{
    CHAR_PET_HEAD,      //ͷ
    CHAR_PET_TOOTH,     //��
    CHAR_PET_CLAW,      //צ
    CHAR_PET_BREAST,    //��
    CHAR_PET_BACK,      //��
    CHAR_PET_WING,      //��
    CHAR_PET_FEET,      //��

    CHAR_PETITEMNUM,
} CHAR_petitem;


typedef struct tagCharHaveSkill
{
    int     use;
    Skill   skill;
} CHAR_HaveSkill;

#ifdef _STREET_VENDOR
typedef struct STREET_VENDDOR_t
{
    int pile;                                               // ����
    int price;                                          // �۸�
    int index;                                          // ����
    int kind;                                           // ���߻��ǳ��� 0:���� 1:����
    BOOL usage;                                         // �Ƿ�ʹ����
} StreetVendor_t;

enum
{
    SV_PILE,
    SV_PRICE,
    SV_INDEX,
    SV_KIND,
    SV_USAGE,
    MAX_SV
};
#endif

typedef enum
{
    CHAR_ACTSTAND,
    CHAR_ACTWALK,
    CHAR_ACTATTACK,
    CHAR_ACTTHROW,
    CHAR_ACTDAMAGE,
    CHAR_ACTDEAD,
    CHAR_ACTMAGIC,
    CHAR_ACTITEM,
    CHAR_ACTEFFECT,
    CHAR_ACTDOWN = 10,
    CHAR_ACTSIT,
    CHAR_ACTHAND,
    CHAR_ACTPLEASURE,
    CHAR_ACTANGRY,
    CHAR_ACTSAD,
    CHAR_ACTGUARD,
    CHAR_ACTACTIONWALK,
    CHAR_ACTNOD,
    CHAR_ACTACTIONSTAND,
    CHAR_ACTBATTLE = 20,
    CHAR_ACTLEADER,
    CHAR_ACTBATTLEWATCH,
    CHAR_ACTPOPUPNAME,
    CHAR_ACTTURN = 30,
    CHAR_ACTWARP,
    CHAR_ACTTRADE,
#ifdef _ANGEL_SUMMON
    CHAR_ACTANGEL = 34,
#endif

#ifdef _MIND_ICON
    CHAR_MIND = 40,
#endif
#ifdef _STREET_VENDOR
    CHAR_STREETVENDOR_OPEN,
    CHAR_STREETVENDOR_CLOSE,
#endif
#ifdef _FAMILYBADGE_
    CHAR_FAMILYBADGE,
#endif
#ifdef _ITEM_CRACKER
    CHAR_ITEM_CRACKER = 50,
#endif
} CHAR_ACTION;

typedef enum
{
    CHAR_CLIACTATTACK,
    CHAR_CLIACTDAMAGE,
    CHAR_CLIACTDOWN,
    CHAR_CLIACTSTAND,
    CHAR_CLIACTWALK,
    CHAR_CLIACTSIT,
    CHAR_CLIACTHAND,
    CHAR_CLIACTPLEASURE,
    CHAR_CLIACTANGRY,
    CHAR_CLIACTSAD,
    CHAR_CLIACTGUARD,
    CHAR_CLIACTNOD,
    CHAR_CLIACTTHROW,

} CHAR_CLIENTACTION;

typedef enum
{
    CHAR_TYPENONE,
    CHAR_TYPEPLAYER,
    CHAR_TYPEENEMY,
    CHAR_TYPEPET,
    CHAR_TYPEDOOR,
    CHAR_TYPEBOX,
    CHAR_TYPEMSG,
    CHAR_TYPEWARP,
    CHAR_TYPESHOP,
    CHAR_TYPEHEALER,
    CHAR_TYPEOLDMAN,
    CHAR_TYPEROOMADMIN,
    CHAR_TYPETOWNPEOPLE,
    CHAR_TYPEDENGON,
    CHAR_TYPEADM,
    CHAR_TYPETEMPLE,
    CHAR_TYPESTORYTELLER,
    CHAR_TYPERANKING,
    CHAR_TYPEOTHERNPC,
    CHAR_TYPEPRINTPASSMAN,
    CHAR_TYPENPCENEMY,
    CHAR_TYPEACTION,
    CHAR_TYPEWINDOWMAN,
    CHAR_TYPESAVEPOINT,
    CHAR_TYPEWINDOWHEALER,
    CHAR_TYPEITEMSHOP,
    CHAR_TYPESTONESHOP,
    CHAR_TYPEDUELRANKING,
    CHAR_TYPEWARPMAN,
    CHAR_TYPEEVENT,
    CHAR_TYPEMIC,
    CHAR_TYPELUCKYMAN,
    CHAR_TYPEBUS,
    CHAR_TYPECHARM,
    CHAR_TYPECHECKMAN,
    CHAR_TYPEJANKEN,
    CHAR_TYPETRANSMIGRATION,
    CHAR_TYPEFMWARPMAN,         // ����У˳�����Ա
    CHAR_TYPEFMSCHEDULEMAN,     // ����У˳��Ǽ�Ա
    CHAR_TYPEMANORSCHEDULEMAN,  // ׯ԰�У˳�ԤԼ��
#ifdef _GAMBLE_BANK
    CHAR_GAMBLEBANK,
#endif
#ifdef _NEW_WARPMAN
    CHAR_NEWNPCMAN,
#endif

#ifdef _GAMBLE_ROULETTE
    CHAR_GAMBLEROULETTE,
    CHAR_GAMBLEMASTER,
#endif
#ifdef _TRANSER_MAN
    CHAR_TRANSERMANS,
#endif

#ifdef _NPC_MAKEPAIR
    CHAR_MAKEPAIR,
#endif
#ifdef _NPC_FUSION
    CHAR_PETFUSIONMAN,
#endif

#ifdef _ITEM_NPCCHANGE
    CHAR_ITEMCHANGENPC,
#endif
#ifdef _CFREE_petskill
    CHAR_FREESKILLSHOP,
#endif
#ifdef _PETRACE
    CHAR_PETRACEMASTER,         // ���ﾺ��
    CHAR_PETRACEPET,
#endif

#ifdef _ALLDOMAN // (���ɿ�) Syu ADD ���а�NPC
    CHAR_TYPEALLDOMAN,
#endif

    CHAR_TYPEPETMAKER,              // petmaker

#ifdef _NPC_WELFARE
    CHAR_TYPEWELFARE,
#endif
#ifdef _ALLBLUES_LUA
    CHAR_TYPELUANPC,
#endif
#ifdef _PLAYER_NPC
    CHAR_TYPEPLAYERNPC,
    CHAR_TYPEPLAYERPETNPC,
#endif

    CHAR_TYPENEWSTREET,
    CHAR_TYPENUM,
} CHAR_TYPE;

typedef enum
{
    CHAR_EVENT_NONE,
    CHAR_EVENT_NPC,
    CHAR_EVENT_ENEMY,
    CHAR_EVENT_WARP,
    CHAR_EVENT_DOOR,
    CHAR_EVENT_ALTERRATIVE,
    CHAR_EVENT_WARP_MORNING,
    CHAR_EVENT_WARP_NOON,
    CHAR_EVENT_WARP_NIGHT,
    CHAR_EVENTNUM,
} CHAR_EVENT;

typedef enum
{
    CHAR_PARTY_NONE,
    CHAR_PARTY_LEADER,
    CHAR_PARTY_CLIENT,
} CHAR_PARTY_MODE;

typedef enum
{
    CHAR_TRADE_FREE,      /* �ɽ��� */
    CHAR_TRADE_SENDING,   /* ���������� */
    CHAR_TRADE_TRADING,   /* ������ */
    CHAR_TRADE_LOCK,      /* ���������� */
} CHAR_TRADE_MODE;

typedef enum
{
    CHAR_PETMAIL_NONE,
    CHAR_PETMAIL_IDLE1,
    CHAR_PETMAIL_IDLE2,
    CHAR_PETMAIL_RETURNWAIT,
    CHAR_PETMAIL_IDLE3,
    CHAR_PETMAIL_IDLE4,
    CHAR_PETMAIL_IDLE5,
} CHAR_PETMAIL_MODE;

typedef enum
{
    CHAR_PETFOLLOW_NONE,
    CHAR_PETFOLLOW_NOW,
} CHAR_PETFOLLOW_MODE;

#ifdef _DROPSTAKENEW
typedef enum
{
    DICE_BIG = 1,       // ����_��
    DICE_SMALL,         // ����_С
    DICE_ALLEAT,        // ����_ͨ��
    PET_RACE1 = 5,      // ���ٳ�_����1
    PET_RACE2,          // ���ٳ�_����2
    PET_RACE3,          // ���ٳ�_����3
    ROULETTE1 = 11, ROULETTE2, ROULETTE3, ROULETTE4, ROULETTE5, ROULETTE6, ROULETTE7,
    ROULETTE8, ROULETTE9, ROULETTE10, ROULETTE11, ROULETTE12, ROULETTE13, ROULETTE14,
    ROULETTE15, ROULETTE16, ROULETTE17, ROULETTE18, ROULETTE19, ROULETTE20,
    ROULETTE21 = 41, ROULETTE22, ROULETTE23, ROULETTE24,    ROULETTE25, ROULETTE26,
    ROULETTE27, ROULETTE28, ROULETTE29, ROULETTE30, ROULETTE31, ROULETTE32,
    ROULETTE33, ROULETTE34, ROULETTE35, ROULETTE36, ROULETTE37, ROULETTE38,
    ROULETTE39, ROULETTE40,
    ROULETTE41 = 71, ROULETTE42, ROULETTE43, ROULETTE44, ROULETTE45,
    ROULETTE51 = 101, ROULETTE52, ROULETTE53,   //����
    ROULETTE61 = 111, ROULETTE62,               //1��10 10��11
    PETFIGHT_WIN1 = 120,
    PETFIGHT_WIN2,
    PETFIGHT_DUEL,
} GAMBLE_TYPE;
#endif
#if _ATTESTAION_ID == 1
#define CHAR_MAXDEPOTITEMHAVE 50
#define CHAR_MAXDEPOTPETHAVE 30
#define CHAR_MAXPOOLITEMHAVE    30

#else
#ifdef _CHAR_POOLITEM
#ifdef _NPC_DEPOTPET // ���ų���ֿ�ͬʱ���ӵ��ֿ߲�����
#define CHAR_MAXDEPOTITEMHAVE 80
#else
#define CHAR_MAXDEPOTITEMHAVE 60
#endif
#endif

#ifdef _CHAR_POOLPET
#define CHAR_MAXDEPOTPETHAVE 30
#endif


#ifdef _ADD_POOL_ITEM               // (���ɿ�) WON ADD ���ӿɼķŵĵ���
#define CHAR_MAXPOOLITEMHAVE    30
#else
#define CHAR_MAXPOOLITEMHAVE    20
#endif
#endif

#define CHAR_MAXITEMNUM  15
#define CHAR_STARTITEMARRAY     CHAR_EQUIPPLACENUM
#define CHAR_MAXITEMHAVE (CHAR_STARTITEMARRAY+CHAR_MAXITEMNUM)

#define CHAR_MAXPETITEMHAVE CHAR_PETITEMNUM

typedef enum
{
    CHAR_DATAPLACENUMBER,
    CHAR_BASEIMAGENUMBER,
    CHAR_BASEBASEIMAGENUMBER,
    CHAR_FACEIMAGENUMBER,
    CHAR_FLOOR,
    CHAR_X,
    CHAR_Y,
    CHAR_DIR,
    CHAR_LV,
    CHAR_GOLD,
    CHAR_HP,
    CHAR_MP,
    CHAR_MAXMP,
    CHAR_VITAL,
    CHAR_STR,
    CHAR_TOUGH,
    CHAR_DEX,
    CHAR_CHARM,
    CHAR_LUCK,
    CHAR_EARTHAT,
    CHAR_WATERAT,
    CHAR_FIREAT,
    CHAR_WINDAT,
    CHAR_DEFAULTPET,
    CHAR_CRITIAL,
    CHAR_COUNTER,
    CHAR_RARE,
    CHAR_RADARSTRENGTH,
    CHAR_CHATVOLUME,
    CHAR_MERCHANTLEVEL,
    CHAR_HEALERLEVEL,
    CHAR_SAVEPOINT = CHAR_HEALERLEVEL,
    CHAR_DETERMINEITEM,
    CHAR_INDEXOFEQTITLE,
    CHAR_POISON,
    CHAR_PARALYSIS,
    CHAR_SLEEP,
    CHAR_STONE,
    CHAR_DRUNK,
    CHAR_CONFUSION,
    CHAR_LOGINCOUNT,
    CHAR_NPCCREATEINDEX = CHAR_LOGINCOUNT,
    CHAR_DEADCOUNT,
    CHAR_WALKCOUNT,
    CHAR_TALKCOUNT,
    CHAR_DAMAGECOUNT,
    CHAR_GETPETCOUNT,
    CHAR_KILLPETCOUNT,
    CHAR_DEADPETCOUNT,
    CHAR_SENDMAILCOUNT,
    CHAR_MERGEITEMCOUNT,
    CHAR_DUELBATTLECOUNT,
    CHAR_DUELWINCOUNT,
    CHAR_DUELLOSECOUNT,
    CHAR_DUELSTWINCOUNT,
    CHAR_DUELSTLOSECOUNT,
    CHAR_DUELMAXSTWINCOUNT,
    CHAR_WHICHTYPE,
    CHAR_WALKINTERVAL,
    CHAR_LOOPINTERVAL,
#ifdef _NEWOPEN_MAXEXP
    CHAR_OLDEXP,
#endif
    CHAR_EXP,

    CHAR_LASTTALKELDER,
    CHAR_SKILLUPPOINT,
    CHAR_LEVELUPPOINT,
    CHAR_IMAGETYPE,
    CHAR_NAMECOLOR,
    CHAR_POPUPNAMECOLOR,
    CHAR_LASTTIMESETLUCK,
    CHAR_DUELPOINT,
    CHAR_ENDEVENT,              /* (0~31)       */
    CHAR_ENDEVENT2,             /* (32~63)      */
    CHAR_ENDEVENT3,             /* (64~96)      */
#ifdef _NEWEVENT
    CHAR_ENDEVENT4,             /* (96~127) */
    CHAR_ENDEVENT5,             /* (128~159) */
    CHAR_ENDEVENT6,             /* (160~191) */
#endif
#ifdef _ADD_NEWEVENT            // WON �����������
    CHAR_ENDEVENT7,
    CHAR_ENDEVENT8,
#endif
#ifdef _ADD_NEWEVENT_1024
    CHAR_ENDEVENT9,
    CHAR_ENDEVENT10,
    CHAR_ENDEVENT11,
    CHAR_ENDEVENT12,
    CHAR_ENDEVENT13,
    CHAR_ENDEVENT14,
    CHAR_ENDEVENT15,
    CHAR_ENDEVENT16,
    CHAR_ENDEVENT17,
    CHAR_ENDEVENT18,
    CHAR_ENDEVENT19,
    CHAR_ENDEVENT20,
    CHAR_ENDEVENT21,
    CHAR_ENDEVENT22,
    CHAR_ENDEVENT23,
    CHAR_ENDEVENT24,
    CHAR_ENDEVENT25,
    CHAR_ENDEVENT26,
    CHAR_ENDEVENT27,
    CHAR_ENDEVENT28,
    CHAR_ENDEVENT29,
    CHAR_ENDEVENT30,
    CHAR_ENDEVENT31,
    CHAR_ENDEVENT32,
#endif
    CHAR_NOWEVENT,              /* (0~31)       */
    CHAR_NOWEVENT2,             /* (32~63)      */
    CHAR_NOWEVENT3,             /* (64~96)      */
#ifdef _NEWEVENT
    CHAR_NOWEVENT4,             /* (96~127)     */
    CHAR_NOWEVENT5,             /* (128~159)    */
    CHAR_NOWEVENT6,             /* (160~191)    */
#endif
#ifdef _ADD_NEWEVENT            // WON �����������
    CHAR_NOWEVENT7,
    CHAR_NOWEVENT8,             // 224~255 �����ٻ�ר��
#endif
#ifdef _ADD_NEWEVENT_1024
    CHAR_NOWEVENT9,
    CHAR_NOWEVENT10,
    CHAR_NOWEVENT11,
    CHAR_NOWEVENT12,
    CHAR_NOWEVENT13,
    CHAR_NOWEVENT14,
    CHAR_NOWEVENT15,
    CHAR_NOWEVENT16,
    CHAR_NOWEVENT17,
    CHAR_NOWEVENT18,
    CHAR_NOWEVENT19,
    CHAR_NOWEVENT20,
    CHAR_NOWEVENT21,
    CHAR_NOWEVENT22,
    CHAR_NOWEVENT23,
    CHAR_NOWEVENT24,
    CHAR_NOWEVENT25,
    CHAR_NOWEVENT26,
    CHAR_NOWEVENT27,
    CHAR_NOWEVENT28,
    CHAR_NOWEVENT29,
    CHAR_NOWEVENT30,
    CHAR_NOWEVENT31,
    CHAR_NOWEVENT32,
#endif
    CHAR_TRANSMIGRATION,
    CHAR_TRANSEQUATION,
    CHAR_INITDATA,
    CHAR_SILENT,              /* char shutup time */
    CHAR_FMINDEX,             // ���� index
    CHAR_FMLEADERFLAG,            /* �����Ա����
                                 * FMMEMBER_NONE    :û�м����κμ���
                                 * FMMEMBER_APPLY   :������������
                                 * FMMEMBER_LEADER  :�峤
                                 * FMMEMBER_MEMBER  :һ���Ա
                                 * FMMEMBER_ELDER   :����
                                 * FMMEMBER_INVITE  :��˾         // ��ʱ����
                                 * FMMEMBER_BAILEE  :����          // ��ʱ����
                                 * FMMEMBER_VICELEADER  :���峤      // ��ʱ����
                                */
    CHAR_FMSPRITE,        // �����ػ�����

    CHAR_BANKGOLD,
    CHAR_RIDEPET,
    CHAR_LEARNRIDE,
#ifdef _NEW_RIDEPETS
    CHAR_LOWRIDEPETS,
#endif
    CHAR_LIMITLEVEL,
#ifdef _PET_FUSION
    CHAR_FUSIONCODE,        //���ֱ���
    CHAR_FUSIONINDEX,       //����������
    CHAR_FUSIONRAISE,       //  ������
    CHAR_FUSIONBEIT,        //�走���
    CHAR_FUSIONTIMELIMIT,   //  ��ʱ��
#endif

#ifdef _PERSONAL_FAME   // Arminius 8.30: �����������
    CHAR_FAME,
#endif

#ifdef _NEWSAVE
    CHAR_SAVEINDEXNUMBER,   /*  SaveFile .0.char or .1.char */
#endif

#ifdef _ATTACK_MAGIC
    CHAR_EARTH_EXP,                     // ��ҵĵ�ħ��������
    CHAR_WATER_EXP,                     // ��ҵ�ˮħ��������
    CHAR_FIRE_EXP,                      // ��ҵĻ�ħ��������
    CHAR_WIND_EXP,                      // ��ҵķ�ħ��������
    CHAR_EARTH_RESIST,                  // ��ҵĵ�ħ������
    CHAR_WATER_RESIST,                  // ��ҵ�ˮħ������
    CHAR_FIRE_RESIST,                   // ��ҵĻ�ħ������
    CHAR_WIND_RESIST,                   // ��ҵķ�ħ������
    CHAR_EARTH_ATTMAGIC_EXP,            // ��ҵĵ�ħ�������Ⱦ���ֵ
    CHAR_WATER_ATTMAGIC_EXP,            // ��ҵ�ˮħ�������Ⱦ���ֵ
    CHAR_FIRE_ATTMAGIC_EXP,             // ��ҵĻ�ħ�������Ⱦ���ֵ
    CHAR_WIND_ATTMAGIC_EXP,             // ��ҵķ�ħ�������Ⱦ���ֵ
    CHAR_EARTH_DEFMAGIC_EXP,            // ��ҵĵ�ħ�����Ծ���ֵ
    CHAR_WATER_DEFMAGIC_EXP,            // ��ҵ�ˮħ�����Ծ���ֵ
    CHAR_FIRE_DEFMAGIC_EXP,             // ��ҵĻ�ħ�����Ծ���ֵ
    CHAR_WIND_DEFMAGIC_EXP,             // ��ҵķ�ħ�����Ծ���ֵ
#endif


#ifdef _GAMBLE_BANK
    CHAR_PERSONAGOLD,   //�ĳ���������
#endif
#ifdef _DROPSTAKENEW
    CHAR_GAMBLENUM,     //�ĳ�����
#endif
#ifdef _ADD_ACTION          //npc����
    CHAR_ACTIONSTYLE,
#endif
#ifdef _PET_EVOLUTION
    CHAR_EVOLUTIONBASEVTL,
    CHAR_EVOLUTIONBASESTR,
    CHAR_EVOLUTIONBASETGH,
    CHAR_EVOLUTIONBASEDEX,
#endif

#ifdef _FM_JOINLIMIT
    CHAR_FMTIMELIMIT,
#endif

#ifdef _CHAR_PROFESSION         // WON ADD ����ְҵ
    PROFESSION_CLASS,           // ְҵ��
    PROFESSION_LEVEL,           // ְҵ�ȼ�
    PROFESSION_CNT,           // ��ְ����
//      PROFESSION_EXP,             // ְҵ����ֵ
    PROFESSION_SKILL_POINT,     // ���ܵ���
    ATTACHPILE,                 // ���Ӷѵ�
    PROFESSION_FIRE_P,          // ��������
    PROFESSION_ICE_P,           // ��������
    PROFESSION_THUNDER_P,       // ��������
    PROFESSION_FIRE_R,          // ����
    PROFESSION_ICE_R,           // ������
    PROFESSION_THUNDER_R,       // �׿���
#endif
#ifdef _ALLDOMAN // (���ɿ�) Syu ADD ���а�NPC
    CHAR_HEROFLOOR,
#endif
#ifdef _PETSKILL_BECOMEPIG
    CHAR_BECOMEPIG,
    CHAR_BECOMEPIG_BBI,//Ҫ��ɵ�ͼ��
#endif
    CHAR_LASTLEAVETIME, // Robin add ��������ʱ��

#ifdef _NEW_MANOR_LAW
    CHAR_MOMENTUM,
#endif

#ifdef _ITEM_ADDEXP2
    CHAR_ADDEXPPOWER,
    CHAR_ADDEXPTIME,
#endif

#ifdef _ANGEL_SUMMON
    CHAR_HEROCNT, // �����������Ĵ���
#endif

#ifdef _TEACHER_SYSTEM
    CHAR_TEACHER_FAME,  // ��ʦ�쵼����
#endif

#ifdef _GM_ITEM
    CHAR_GMTIME,       // ���GM�������
#endif

#ifdef _VIP_SERVER
    CHAR_AMPOINT,      // �ػر���
#endif

#ifdef _VIP_RIDE
    CHAR_VIPRIDE,      // ��Ա��ڼ�
    CHAR_VIPTIME,      // ��Ա��ڼ�
#endif

#ifdef _PET_BEATITUDE
    CHAR_BEATITUDE,
#endif

#ifdef _RIDE_CF
    CHAR_LOWRIDEPETS1,
#ifdef _ADD_RIDE_CF
    CHAR_HIGHRIDEPET2,
#endif
#endif

#ifdef _ITEM_PET_LOCKED
    CHAR_LOCKED,
#endif

#ifdef _BOUND_TIME
    CHAR_BOUNDTIME,
#endif

#ifdef _SUPER
    CHAR_SUPER,
#endif

#ifdef _ONLINE_SHOP
    CHAR_BJ,
#endif

#ifdef _LOTTERY_SYSTEM
    CHAR_LOTTERY_DATE,
    CHAR_LOTTERY_TIME,
#endif

#ifdef _COMPOUND_EXP
    CHAR_MATERIAL01,
    CHAR_MATERIAL02,
    CHAR_MATERIAL03,
    CHAR_MATERIAL04,
    CHAR_MATERIAL05,
    CHAR_MATERIAL06,
    CHAR_MATERIAL07,
    CHAR_MATERIAL08,
    CHAR_MATERIAL09,
    CHAR_MATERIAL10,
    CHAR_MATERIAL11,
    CHAR_MATERIAL12,
    CHAR_MATERIAL13,
    CHAR_MATERIAL14,
    CHAR_MATERIAL15,
    CHAR_MATERIAL16,
    CHAR_MATERIAL17,
    CHAR_MATERIAL18,
    CHAR_MATERIAL19,
    CHAR_MATERIAL20,
    CHAR_MATERIAL21,
    CHAR_MATERIAL22,
    CHAR_MATERIAL23,
    CHAR_MATERIAL24,
    CHAR_MATERIAL25,
#endif

#ifdef _ONLINE_TIME
    CHAR_ONLINE_TIME,
#endif

#ifdef _CAMEO_MONEY
    CHAR_CAMEO,
#endif

#ifdef _MISSION_TRAIN
    CHAR_MISSIONTRAIN_NUM,
    CHAR_MISSIONTRAIN_TIME,
#endif
#ifdef _PET_EVOLVE
    CHAR_EVOLVE,
#endif
#ifdef _FLOOR_DIFI_FIELD
    CHAR_FLOORDIFI_TIME,
#endif

#ifdef _ACTIVE_GAME
    CHAR_ACTIVE,
#endif
#ifdef _MISSION_TIME
    CHAR_MISSION_TIME,
#endif
#ifdef _REGISTRATION_TIME
    CHAR_REGISTRATION_FLG,
    CHAR_REGISTRATION_TIME,
    CHAR_REGISTRATION_COUNT,
    CHAR_REGISTRATION_ONLINE_TIME,
#endif
#ifdef _SPECIAL_COUNTER
    CHAR_SPCOUNTER,
#endif
#ifdef _SPECIAL_TIMER
    CHAR_SPTIMER,
#endif
#ifdef _OFFLINE_TIME
    CHAR_OFFLINE_TIME,
#endif
#ifdef _FAME_REG_TIME
    CHAR_FAME_REG_TIME,
#endif
    CHAR_SHUALOU_POINT,    //ˢ¥����
#ifdef  _NEW_MAP_NO_FAME
    CHAR_NOFAME,
#endif
    CHAR_DATEWORLDCNT,      //����Ƶ�����ڴ���
    CHAR_WORLDTIME,     //����Ƶ��ʱ��
    CHAR_RIDENAMEFLG,       //����Ƿ���ʾԭ��
    CHAR_HDNUM,                 //�����
    CHAR_SHNUM,                 //�˺�����
    CHAR_HDDATE,                    //�����
#ifdef  _ENEMY_MIN_ADD
    CHAR_ENEMY_MIN_TIME,
    CHAR_ENEMY_MIN_BASE,
#endif
#ifdef  _FMBATTLE_POINT
    CHAR_FMBATTLE_POINT,
#endif

#ifdef _CHAR_TITLE_STR_
    CHAR_TITLE1,
    CHAR_TITLE2,
    CHAR_TITLE3,
    CHAR_TITLE_,
    CHAR_TITLE_TYPE,
#endif
#ifdef _CHAR_MANOR_
    CHAR_TRUMPFLG,
    CHAR_TRUMP_EFFECT,
#endif
#ifdef _CHAR_TITLE_NEW_
    CHAR_TITLE_NEW,
#endif
#ifdef  _NEW_TEACHER_SYSTEM
    CHAR_NEW_TEACHER_POINT,
#endif
    CHAR_BUCHANG_DATE,
    CHAR_DWPOINT,
    CHAR_DWTYPE,
    CHAR_DWTIME,
#ifdef _ADDLOGINTIME_
    CHAR_LOGIN_TIME,
    CHAR_LOGIN_DATA,
#endif
#ifdef _PAIPAISONG_
    CHAR_PPSNUM,
    CHAR_PPSLUCKY,
#endif
#ifdef _NEWPAIHANG_
    CHAR_YX_TIME1,
    CHAR_YX_TIME2,
    CHAR_132TITLE_FLG,
#endif
#ifdef _ATTACK_EFFECT
    CHAR_ATTACK_EFFECT,
#endif
#ifdef _PETCOM_
    CHAR_YHP,
    CHAR_YATK,
    CHAR_YDEF,
    CHAR_YQUICK,
    CHAR_YLV,
#endif
#ifdef _BUY_VIGORDATA
    CHAR_VIGORDATA,
#endif // _BUY_VIGORDATA
#ifdef _NEW_PROMPTFLG
    CHAR_NEWPROMPTFLG1,
    CHAR_NEWPROMPTFLG2,
#endif // _NEW_PROMPTFLG
#ifdef _NEWPLAYER_FLG
    CHAR_NEWPLAYERFLG,
#endif // _NEWPLAYER_FLG
    CHAR_DAYITEMEXPTIME,    //ÿ��ǩ��˫��ʱ��
#ifdef _PARTY_HIGH
    CHAR_PARTYTYPE,
#endif // _PARTY_HIGH
#ifdef _OFFLINE_EXP
    CHAR_OFFLINE_EXP,
#endif // _OFFLINE_EXP
    CHAR_CREATETIME,
#ifdef _U8_QUESTION
    CHAR_U8QUESTION,
#endif // _U8_QUESTION
    CHAR_PETDATA,
    CHAR_PETDATA2,
    CHAR_PETDATA3,
    CHAR_PETDATA4,
    CHAR_PETDATA5,
#ifdef _PET_IMAGE
    CHAR_PETIMAGE1,
    CHAR_PETIMAGE2,
    CHAR_PETIMAGE3,
    CHAR_PETIMAGE4,
    CHAR_PETIMAGE5,
    CHAR_PETIMAGE6,
    CHAR_PETIMAGE7,
    CHAR_PETIMAGE8,
    CHAR_PETIMAGE9,
    CHAR_PETIMAGE10,
#endif
#ifdef _479SA
    CHAR_CARD_TIME,
    CHAR_PET_OLD_NUM,
#endif // _479SA
#ifdef _BATTLE_EFFECTS
    CHAR_BATTLEEFFECTS,
    CHAR_BATTLEEFFECTS2,
#endif // _BATTLE_EFFECTS
#ifdef _NEWPLAYER_FUNC
	CHAR_NEWFLG,
	CHAR_MAINID,
	CHAR_SUBID,
#endif
#ifdef __NEW_CONSIGNMENT
	CHAR_PRICE,
	CHAR_CONSIGN,
#endif
    CHAR_DATAINTNUM,
} CHAR_DATAINT;

typedef enum
{
    CHAR_MODAI                = CHAR_CHARM,           /*  ʸ����  ���������Ĥ�Ұ�    */
    CHAR_VARIABLEAI         = CHAR_LUCK,            /*  ʸ�����������  ����  ��ߨ�  ���װ�ƥ  �Ȼ�����   */
    CHAR_SLOT                 = CHAR_DEFAULTPET,      /* ��  ë��������  �µ���������  ʸ����������  */
    CHAR_PETGETLV         = CHAR_CHATVOLUME,      /* ��������ľ����ì��  ʸ������������   */
    CHAR_PUTPETTIME           = CHAR_LOGINCOUNT,      /*  ʸ��������  ��  ��ľ������ */
    CHAR_MAILMODE           = CHAR_RADARSTRENGTH,   /*   ʸ������������  ������˪������ */
    CHAR_ALLOCPOINT           = CHAR_LEVELUPPOINT,    /* ��ì��ʧ�����ݼ��Ի��Ի���̼���(ʸ������) */
    CHAR_PETMAILBUFINDEX  = CHAR_GETPETCOUNT,     /* ʸ����������(offmsg)�߼�index */
    CHAR_PETMAILIDLETIME    = CHAR_SENDMAILCOUNT,   /* ʸ���������Ｐ�м�����ئ�� */
    CHAR_PETMAILFROMFLOOR   = CHAR_DUELBATTLECOUNT, /* ������ë˪�����ݼ��׷�ʧ */
    CHAR_PETMAILFROMX       = CHAR_DUELWINCOUNT,    /* ������ë˪�����ݼ�  ��   */
    CHAR_PETMAILFROMY       = CHAR_DUELLOSECOUNT,   /* ������ë˪�����ݼ�  ��   */
    CHAR_PETMAILEFFECT      = CHAR_DUELSTWINCOUNT,  /* ʸ���������Ｐ���ؼ��ް�������  į */
    CHAR_PETSENDMAILCOUNT   = CHAR_TALKCOUNT,       /* ʸ������������ë���Ϸּ��� */
    CHAR_PETRANK          = CHAR_LASTTIMESETLUCK, /* ʸ�������¼���(ʸ������  ��  ��ۨ߷��޷¡��� */
    CHAR_PETID              = CHAR_DUELMAXSTWINCOUNT,/* ʸ������  ������������     */
    CHAR_PETFAMILY        = CHAR_FMLEADERFLAG,        // CoolFish: Family �ж��Ƿ�Ϊ�����ػ���
#ifdef _PET_VALIDITY
    CHAR_PETVALIDITY    = CHAR_VIPTIME,
#endif
} CHAR_PET;


typedef enum
{
    CHAR_NAME,
    CHAR_OWNTITLE,
    CHAR_USERPETNAME = CHAR_OWNTITLE,
    CHAR_NPCARGUMENT,
    CHAR_CDKEY = CHAR_NPCARGUMENT,
    CHAR_OWNERCDKEY,
    CHAR_OWNERCHARANAME,
    CHAR_FMNAME,
#ifdef _UNIQUE_P_I
    CHAR_UNIQUECODE,
#endif

#ifdef _TEACHER_SYSTEM
    CHAR_TEACHER_ID,        // ��ʦ�ʺ�
    CHAR_TEACHER_NAME,  // ��ʦ����
#endif
#ifdef _ITEM_SETLOVER
    CHAR_LOVE,         // ����ж�
    CHAR_LOVERID,      // �����ʺ�
    CHAR_LOVERNAME,    // ��������
#endif

#ifdef _GM_ITEM
    CHAR_GMFUNCTION,   // ���GM����
#endif
#ifdef _NEW_NAME     //�Զ���ƺ�
    CHAR_NEWNAME,
#endif

#ifdef _LOTTERY_SYSTEM
    CHAR_LOTTERY_VALUE,
#endif
#ifdef _POWER_UP
    CHAR_POWER_UP,
#endif
    CHAR_DAMAGE_BUFF,
#ifdef  _PET_LV1_4V
    CHAR_PET_4V,
    CHAR_PETTRN_4V,
#endif
#ifdef  _PAOHUAN
    CHAR_PAOHUAN,
#endif
#ifdef _CHAR_TITLE_NEW_
    CHAR_TITLENEW_DATA,
#endif
#ifdef  _NEW_TEACHER_SYSTEM
    CHAR_NEW_TEACHER_DATA,
#endif
#ifdef  _PLAYER_UID
    CHAR_PLAYERUID,
#endif
#ifdef _NEW_EVENT_ID_UPDATE
    CHAR_EVENTID,
#endif // _NEW_EVENT_ID_UPDATE
#ifdef _PARTY_HIGH
    CHAR_PARTYOTHERNAME,
#endif // _PARTY_HIGH
#ifdef _ACTIVE_GAME
    CHAR_ACTIVE_TIME,
#endif
#ifdef _CAPTURE_DATA
    CHAR_CAPTURE_DATA,
#endif // _CAPTURE_DATA
#ifdef _479SA
    CHAR_CARD_TYPE,
    CHAR_PET_OLD_4V,
    CHAR_PET_OLD_4V_FIRST,
    CHAR_PET_OLD_ZF,
    CHAR_ACTIVE_MAIN,
    CHAR_ACTIVE_ACT1,
    CHAR_ACTIVE_ACT2,
    CHAR_ACTIVE_ACT3,
    CHAR_ACTIVE_ACT4,
    CHAR_ACTIVE_ACT5,
    CHAR_ACTIVE_ACT6,
    CHAR_ACTIVE_ACT7,
    CHAR_ACTIVE_ACT8,
    CHAR_ACTIVE_ACT9,
    CHAR_ACTIVE_ACT10,
    CHAR_ACTIVE_ACT11,
    CHAR_ACTIVE_ACT12,
    CHAR_ACTIVE_ACT13,
    CHAR_ACTIVE_ACT14,
    CHAR_ACTIVE_ACT15,
    CHAR_ACTIVE_ACT16,
#endif // _479SA
    CHAR_SHUALOU_DATA,
    CHAR_DATACHARNUM,
} CHAR_DATACHAR;

typedef enum
{
    CHAR_ISATTACK,
    CHAR_ISATTACKED,
    CHAR_ISOVER,
    CHAR_ISOVERED,
    CHAR_HAVEHEIGHT,
    CHAR_ISVISIBLE,
    CHAR_ISTRANSPARENT,
    CHAR_ISFLYING,
    CHAR_ISDIE,
    CHAR_ISBIG,
    CHAR_ISSHOWBATTLEMSG,
    CHAR_ISPARTY,
    CHAR_ISWARP,
    CHAR_ISDUEL,
    CHAR_ISPARTYCHAT,
    CHAR_ISTRADECARD,
    CHAR_ISTRADE,
#ifdef _CHANNEL_MODIFY
    CHAR_ISTELL,                //����Ƶ������
    CHAR_ISFM,                  //����Ƶ������
    CHAR_ISOCC,                 //ְҵƵ������
    CHAR_ISSAVE,                //�Ի����濪��
    CHAR_ISCHAT,                //������
#ifdef _THE_WORLD_SEND
    CHAR_ISWORLD,               //����Ƶ������
#endif
#ifdef _NEW_FS_TYPE
    CHAR_ISEFFECT,               //��Ч����
    CHAR_ISPLAYER,               //�������￪��
    CHAR_ISTEAMFULL,               //������Ա����
    CHAR_ISSHOWPET,               //�鿴���￪��
    CHAR_ISAUTOPARTY,               //�Զ�����ӿ���
    CHAR_ISALLWORLD,               //����Ƶ������
    CHAR_ISDRAWEFFECT,               //��Ч����
#endif // _NEW_FS_TYPE

#endif
    CHAR_FLGNUM,
} CHAR_DATAFLG;

#define CHAR_FS_PARTY           (1 << 0)
#define CHAR_FS_BATTLE      (1 << 1)
#define CHAR_FS_DUEL        (1 << 2)
#define CHAR_FS_PARTYCHAT   (1 << 3)    //����Ƶ������
#define CHAR_FS_TRADECARD   (1 << 4)
#define CHAR_FS_TRADE           (1 << 5)
#ifdef _CHANNEL_MODIFY
#define CHAR_FS_TELL            (1 << 6)    //����Ƶ������
#define CHAR_FS_FM              (1 << 7)    //����Ƶ������
#define CHAR_FS_OCC             (1 << 8)    //ְҵƵ������
#define CHAR_FS_SAVE            (1 << 9)    //�Ի����濪��
#ifdef _THE_WORLD_SEND
#define CHAR_FS_WORLD           (1 << 10)   //����Ƶ������
#endif
#define CHAR_FS_MOD     (1 << 12)
#define CHAR_FS_DRAW_EFFECT     (1<<13)
#define CHAR_FS_NAME_BTN        (1<<14)
#ifdef _NEW_FS_TYPE
#define CHAR_FS_EFFECT     (1 << 15)
#define CHAR_FS_PLAYER     (1<<16)
#define CHAR_FS_TEAMFULL        (1<<17)
#define CHAR_FS_SHOWPET        (1<<18)
#define CHAR_FS_AUTOPARTY        (1<<19)
#define CHAR_FS_ALLWORLD        (1<<11)
#endif

#endif
typedef enum
{
    CHAR_WORKBATTLEMODE,
    CHAR_WORKBATTLEINDEX,
    CHAR_WORKBATTLESIDE,
    CHAR_WORKBATTLECOM1,
    CHAR_WORKBATTLECOM2,
    CHAR_WORKBATTLECOM3,
#ifdef _PSKILL_MDFYATTACK
    CHAR_WORKBATTLECOM4,
#endif
#ifdef _PETSKILL_ISULTIMATE
    CHAR_WORKBATTLECOM5,
    CHAR_WORKBATTLECOM6,
#endif
    CHAR_WORKBATTLEFLG,
    CHAR_WORKBATTLEWATCH,

    CHAR_WORKFIXEARTHAT,
    CHAR_WORKFIXWATERAT,
    CHAR_WORKFIXFIREAT,
    CHAR_WORKFIXWINDAT,
#ifdef _PROFESSION_ADDSKILL
    CHAR_WORKFIXEARTHAT_BOUNDARY, //�ؽ��  ��16λԪ��ż�ǿ�ٷֱ� ��16λԪ��Żغ���
    CHAR_WORKFIXWATERAT_BOUNDARY, //ˮ���
    CHAR_WORKFIXFIREAT_BOUNDARY,  //����
    CHAR_WORKFIXWINDAT_BOUNDARY,  //����
#endif
    CHAR_WORKMAXHP,
    CHAR_WORKMAXMP,

    CHAR_WORKATTACKPOWER,
    CHAR_WORKDEFENCEPOWER,
    CHAR_WORKQUICK,

    CHAR_WORKFIXVITAL,
    CHAR_WORKFIXSTR,
    CHAR_WORKFIXTOUGH,
    CHAR_WORKFIXDEX,

#ifdef _ITEMSET5_TXT
    CHAR_WORKFIXARRANGE,
    CHAR_WORKARRANGEPOWER,
    CHAR_WORKFIXSEQUENCE,
    CHAR_WORKSEQUENCEPOWER,
    CHAR_WORKATTACHPILE,
    CHAR_WORKHITRIGHT,  //��������
#endif
#ifdef _ITEMSET6_TXT
    CHAR_WORKNEGLECTGUARD,
#endif
    CHAR_WORKMODATTACK,
    CHAR_WORKMODDEFENCE,
    CHAR_WORKMODQUICK,

    CHAR_WORKMODCAPTURE,
    CHAR_WORKMODCHARM,

    CHAR_WORKFIXCHARM,
    CHAR_WORKFIXLUCK,
    CHAR_WORKFIXAI,
    CHAR_WORKFIXAVOID,

    CHAR_WORKDAMAGEABSROB,
    CHAR_WORKDAMAGEREFLEC,
    CHAR_WORKDAMAGEVANISH,
    CHAR_WORKPOISON,
    CHAR_WORKPARALYSIS,
    CHAR_WORKSLEEP,
    CHAR_WORKSTONE,
    CHAR_WORKDRUNK,
    CHAR_WORKCONFUSION,
    CHAR_WORKWEAKEN,             //����  vincent add  _MAGIC_WEAKEN
    CHAR_WORKDEEPPOISON,         //�綾  vincent add _MAGIC_DEEPPOISON
    CHAR_WORKBARRIER,            //ħ��  vincent add _MAGIC_BARRIER
    CHAR_WORKNOCAST,             //��Ĭ  vincent add _MAGIC_NOCAST
    CHAR_WORKMODPOISON,
    CHAR_WORKMODPARALYSIS,
    CHAR_WORKMODSLEEP,
    CHAR_WORKMODSTONE,
    CHAR_WORKMODDRUNK,
    CHAR_WORKMODCONFUSION,
    CHAR_WORKMODWEAKEN,
    CHAR_WORKMODDEEPPOISON,
    CHAR_WORKMODBARRIER,        //ħ��
    CHAR_WORKMODNOCAST,         //��Ĭ
    CHAR_WORKMODCRITICAL,       //һ����ɱ
    CHAR_WORKULTIMATE,
    CHAR_WORKPARTYMODE,
    CHAR_WORKTRADEMODE,         /* ����ģʽ
                                 * 0: CHAR_TRADE_FREE   :�ɽ���
                                 * 1: CHAR_TRADE_SENDING:����������
                                 * 2: CHAR_TRADE_TRADING:������
                                 * 3: CHAR_TRADE_LOCK   :����������
                                */
    CHAR_WORKPARTYINDEX1,
    CHAR_WORKPARTYINDEX2,
    CHAR_WORKPARTYINDEX3,
    CHAR_WORKPARTYINDEX4,
    CHAR_WORKPARTYINDEX5,
#ifdef _AIRCRAFT_PLUS_
    CHAR_WORKPARTYINDEX6,
    CHAR_WORKPARTYINDEX7,
    CHAR_WORKPARTYINDEX8,
#endif
    CHAR_WORKOBJINDEX,
    CHAR_WORKWALKSTARTSEC,
    CHAR_WORKWALKSTARTMSEC,
    CHAR_WORKLOOPSTARTSEC,
    CHAR_WORKLOOPSTARTMSEC,
    CHAR_WORKLASTATTACKCHARAINDEX,
    CHAR_WORKEVENTTYPE,
    CHAR_WORKGETEXP,
    CHAR_WORKMODCAPTUREDEFAULT,
    CHAR_WORKACTION,
    CHAR_WORKFD,
    CHAR_WORKFLG,
    CHAR_WORKGMLEVEL,
    CHAR_NPCWORKINT1,
    CHAR_NPCWORKINT2,
    CHAR_NPCWORKINT3,
    CHAR_NPCWORKINT4,
    CHAR_NPCWORKINT5,
    CHAR_NPCWORKINT6,
    CHAR_NPCWORKINT7,
    CHAR_NPCWORKINT8,
    CHAR_NPCWORKINT9,
    CHAR_NPCWORKINT10,
    CHAR_NPCWORKINT11,
#ifdef _PETRACE
    CHAR_NPCWORKINT12,
#endif
    CHAR_NPCWORKINT13,
    CHAR_NPCWORKINT14,
    CHAR_WORKWARPCHECK,
    CHAR_TENSEICHECKED,
    CHAR_WORKFMINDEXI,      // �������� index
    CHAR_WORKFMCHANNEL,     // ����Ƶ��
    CHAR_WORKFMCHANNELQUICK,
    CHAR_WORKFMFLOOR,       // ����ݵ�ͼ��
    CHAR_WORKFMDP,          // ����ģ�ֵ
    CHAR_WORKFMPKFLAG,      // �Ƿ����������У� 1:Yes
    CHAR_WORKFMSETUPFLAG,   // �����Ƿ�����ʽ����
    CHAR_WORKFMMANINDEX,    // FMWARPMAN Index
    CHAR_WORKFMCHARINDEX,   // �����Ա���� index
    CHAR_WORKBATTLEFLAG,    // -1(ս������)��1(��ս��)

    CHAR_WORK_PET0_STAT,
    CHAR_WORK_PET1_STAT,
    CHAR_WORK_PET2_STAT,
    CHAR_WORK_PET3_STAT,
    CHAR_WORK_PET4_STAT,

    CHAR_WORKLOGINTIME,     // ��ҵ���ʱ��
    CHAR_WORKTALKCOUNT,
    CHAR_WORKTALKTIME,
    CHAR_WORKPETFOLLOW,     // �������
    CHAR_WORKPETFALL,       // ս������
    CHAR_WORKLEADERCHANGE,  // �峤��λindex
#ifdef _EQUIT_DEFMAGIC
    CHAR_EQUITDEFMAGIC_E,
    CHAR_EQUITDEFMAGIC_WA,
    CHAR_EQUITDEFMAGIC_F,
    CHAR_EQUITDEFMAGIC_WI,
    CHAR_EQUITQUIMAGIC,
#endif
#ifdef _EQUIT_RESIST
    CHAR_WORKEQUITFIRE,   //װ������
    CHAR_WORKEQUITTHUNDER,//װ������
    CHAR_WORKEQUITICE,//װ������
    CHAR_WORKEQUITWEAKEN,//װ��������
    CHAR_WORKEQUITBARRIER,//װ����ħ��
    CHAR_WORKEQUITNOCAST,//װ������Ĭ
    CHAR_WORKEQUITFALLRIDE,//װ��������
#endif
#ifdef _PETMAIL_DEFNUMS
    CHAR_PETMAILNUMS,
    CHAR_PETMAILSENDTIME,
#endif
#ifdef _ITEM_METAMO
    CHAR_WORKITEMMETAMO,
#endif
#ifdef _ADD_DEAMGEDEFC
    CHAR_WORKOTHERDMAGE,
    CHAR_WORKOTHERDEFC,
#endif

#ifdef _LOSE_FINCH_
    CHAR_WORKSPETRELIFE,    //���︴������
#endif

#ifdef _CHIKULA_STONE
    CHAR_WORKCHIKULAHP,
    CHAR_WORKCHIKULAMP,
#endif
#ifdef _PETSKILL_SETDUCK
    CHAR_MYSKILLDUCKPOWER,
    CHAR_MYSKILLDUCK,
    CHAR_MYSKILLSTRPOWER,
    CHAR_MYSKILLSTR,
    CHAR_MYSKILLTGHPOWER,
    CHAR_MYSKILLTGH,
    CHAR_MYSKILLDEXPOWER,
    CHAR_MYSKILLDEX,
    CHAR_MAGICPETMP,
#endif
    CHAR_SKILLSTRPOWER,
    CHAR_SKILLTGHPOWER,
    CHAR_SKILLDEXPOWER,
#ifdef _NPC_MAKEPAIR
    CHAR_MYPAIRINDEX,
    CHAR_MYPAIRFLG,
    CHAR_MYPAIRCODE,
#endif
#ifdef _OTHER_MAGICSTAUTS
    CHAR_DEFMAGICSTATUS,
#ifdef _MAGIC_SUPERWALL
    CHAR_MAGICSUPERWALL,
#endif
    CHAR_OTHERSTATUSNUMS,
#endif
#ifdef _NPCCHANGE_PLAYERIMG
    CHAR_WORKNPCMETAMO,
#endif
    CHAR_PickAllPet,
#ifdef _DROPSTAKENEW
    CHAR_WORKSTAKEFLAG,
    CHAR_WORKSTAKETYPE1,
    CHAR_WORKSTAKETYPE2,
    CHAR_WORKSTAKETYPE3,
    CHAR_WORKSTAKETYPE4,
    CHAR_WORKSTAKETYPE5,
#endif

    CHAR_WORKTOXICATION,

#ifdef _SUIT_ITEM
    CHAR_WORKSUITITEM,
    CHAR_WORKROUNDHP,
    CHAR_WORKROUNDMP,
    CHAR_WORKSUITVIT,
    CHAR_WORKSUITSTR,
    CHAR_WORKSUITTGH,
    CHAR_WORKSUITDEX,
    CHAR_WORKSUITMODSTR,
#ifdef _SUIT_ADDENDUM
    CHAR_WORKRESIST, //�쳣������
    CHAR_WORKCOUNTER,//������
    CHAR_WORKMPOWER, //��ǿ��ʦ��ħ��( ��Ӱ��ķ���:��ɽȪ ������ ������ ������ ������ ������ ������ ����ĩ�� ����ǹ ��Ѫ���� ��Ѫ�� һ���Ѫ ������ )
#endif
#ifdef _SUIT_TWFWENDUM
    CHAR_WORK_EA,//��
    CHAR_WORK_WR,//ˮ
    CHAR_WORK_FI,//��
    CHAR_WORK_WI,//��
#endif
#ifdef _SUIT_ADDPART3
    CHAR_WORKDUCKPOWER,//��װ�ر�
    CHAR_WORKRENOCAST, //��Ĭ������
    CHAR_WORKSUITSTR_P,//������ ��λΪ%
    CHAR_WORKSUITTGH_P,//������ ��λΪ%
    CHAR_WORKSUITDEX_P,//������ ��λΪ%
#endif
#ifdef _SUIT_ADDPART4
    CHAR_SUITPOISON,   //װ������
    CHAR_WORKMPOWER2,  //��CHAR_WORKMPOWER��ͬ
    CHAR_WORKUNMPOWER, //�ֿ���ʦ��ħ��( ��Ӱ��ķ���:��ɽȪ ������ ������ ������ ������ ������ ������ ����ĩ�� ����ǹ ��Ѫ���� ��Ѫ�� һ���Ѫ ������ )
#endif
#endif//_SUIT_ITEM

#ifdef _IMPRECATE_ITEM
    CHAR_WORKHURTMP,        //�˺� MP
    CHAR_WORKWISHESHP,      //ף�� hp
    CHAR_WORKWISHESMP,      //ף�� MP
    CHAR_WORKIMPRECATENUM1,
    CHAR_WORKIMPRECATENUM2,
    CHAR_WORKIMPRECATENUM3,
#endif
#ifdef _STATUS_WATERWORD
    CHAR_WORKMAPFLOORTYPE,
    CHAR_WORKSTATUSWATER,
#endif
#ifdef _BATTLENPC_WARP_PLAYER
    CHAR_WORKTURN,
#endif
#ifdef _MIND_ICON
    CHAR_MIND_NUM,
#endif
#ifdef _ITEM_CRACKER
    CHAR_WORKITEM_CRACKER,
#endif
#ifdef _ITEM_ADDEXP //vincent ��������
    CHAR_WORKITEM_ADDEXP,
    CHAR_WORKITEM_ADDEXPTIME,
#endif
#ifdef _ALLDOMAN // (���ɿ�) Syu ADD ���а�NPC
    CHAR_WORKHEROFLOOR,
#endif
#ifdef _PLAYER_EFFECT
    CHAR_PLAYER_EFFECT,
#endif
#ifdef FAMILY_MANOR_
    CHAR_PLAYER_EFFECT_MANOR,
#endif
#ifdef _PROFESSION_SKILL            // WON ADD ����ְҵ����
    CHAR_ENCOUNT_FIX,       // ������
    CHAR_ENCOUNT_NUM,
    CHAR_WORK_F_PROFICIENCY,// ��������
    CHAR_WORK_I_PROFICIENCY,// ��������
    CHAR_WORK_T_PROFICIENCY,// ��������
    CHAR_WORK_F_RESIST,     // ��
    CHAR_WORK_I_RESIST,     // ����
    CHAR_WORK_T_RESIST,     // �翹
    CHAR_MYSKILLHIT,        // ������
    CHAR_MYSKILLHIT_NUM,
    CHAR_WORKDIZZY,         // ��ѣ
    CHAR_WORKMODDIZZY,
    CHAR_WORKENTWINE,       // ��������
    CHAR_WORKMODENTWINE,
    CHAR_WORKTRAP,          // ����
    CHAR_WORKMODTRAP,
    CHAR_WORKDRAGNET,       // ���޵���
    CHAR_WORKMODDRAGNET,
#ifdef _PROFESSION_ADDSKILL
    //***********ע��:�����λ�ò��ɸ���*************
    CHAR_WORKICECRACK,      // ������
    CHAR_WORKICECRACK2,     // ��������Ŵ�2
    CHAR_WORKICECRACK3,     // ��������Ŵ�3
    CHAR_WORKICECRACK4,     // ��������Ŵ�4
    CHAR_WORKICECRACK5,     // ��������Ŵ�5
    CHAR_WORKICECRACK6,     // ��������Ŵ�6
    CHAR_WORKICECRACK7,     // ��������Ŵ�7
    CHAR_WORKICECRACK8,     // ��������Ŵ�8
    CHAR_WORKICECRACK9,     // ��������Ŵ�9
    CHAR_WORKICECRACK10,    // ��������Ŵ�10
    CHAR_WORKMODICECRACK,
    CHAR_WORKMODICECRACK2,
    CHAR_WORKMODICECRACK3,
    CHAR_WORKMODICECRACK4,
    CHAR_WORKMODICECRACK5,
    CHAR_WORKMODICECRACK6,
    CHAR_WORKMODICECRACK7,
    CHAR_WORKMODICECRACK8,
    CHAR_WORKMODICECRACK9,
    CHAR_WORKMODICECRACK10,
    //*****************************************
#else
    CHAR_WORKICECRACK,      // ������
    CHAR_WORKMODICECRACK,
#endif //_PROFESSION_ADDSKILL

    CHAR_WORKOBLIVION,      // ����
    CHAR_WORKMODOBLIVION,
    CHAR_WORKICEARROW,      // ����
    CHAR_WORKMODICEARROW,
    CHAR_WORKBLOODWORMS,    // ��Ѫ��
    CHAR_WORKMODBLOODWORMS,
    CHAR_WORKBLOODWORMSID,
    CHAR_WORKSIGN,          // һ���Ѫ
    CHAR_WORKMODSIGN,
    CHAR_WORKSIGNID,
    CHAR_WORK_P_DUCK,       // �ر�
    CHAR_WORKMOD_P_DUCK,
    CHAR_WORK_WEAPON,       // ����ר��
    CHAR_WORKMOD_WEAPON,
    CHAR_WORK_FOCUS,        // רעս��
    CHAR_WORKMOD_FOCUS,
    CHAR_WORKINSTIGATE,     // ����
    CHAR_WORKMODINSTIGATE,
    CHAR_WORK_F_ENCLOSE,    // ����
    CHAR_WORK_I_ENCLOSE,    // ������
    CHAR_WORK_T_ENCLOSE,    // �׸���
    CHAR_WORKMOD_F_ENCLOSE,
    CHAR_WORKMOD_I_ENCLOSE,
    CHAR_WORKMOD_T_ENCLOSE,
    CHAR_WORK_F_ENCLOSE_2,  // ��
    CHAR_WORK_I_ENCLOSE_2,  // ����
    CHAR_WORK_T_ENCLOSE_2,  // �׸�
    CHAR_WORKMOD_F_ENCLOSE_2,
    CHAR_WORKMOD_I_ENCLOSE_2,
    CHAR_WORKMOD_T_ENCLOSE_2,
    CHAR_WORKRESIST_F,      // ��������
    CHAR_WORKRESIST_I,      // ����������
    CHAR_WORKRESIST_T,      // �����׿���
#ifdef _PROFESSION_ADDSKILL
    CHAR_WORKERSIST_F_I_T,  // ��������׿���
    CHAR_WORKANNEX,         //����
    CHAR_WORKWATER,         //ˮ����
    CHAR_WORKFEAR,          //�־�
    CHAR_DOOMTIME,          //����ĩ�ռ���(�����ǹ����)
    CHAR_WORK_com1,
    CHAR_WORK_toNo,
    CHAR_WORK_mode,
    CHAR_WORK_skill_level,
    CHAR_WORK_array,
#endif
    CHAR_WORKMODRESIST_F,   // �������԰ٷֱ���ֵ
    CHAR_WORKMODRESIST_I,   // ���������԰ٷֱ���ֵ
    CHAR_WORKMODRESIST_T,   // �����׿��԰ٷֱ���ֵ
#endif

#ifdef _MAGICSTAUTS_RESIST
    CHAR_MAGICFIRE, //�𿹾���
    CHAR_MAGICTHUNDER,//�翹����
    CHAR_MAGICICE,//��������
#endif

#ifdef _PET_SKILL_SARS          // WON ADD ��ɷ����
    CHAR_WORKSARS,
    CHAR_WORKMODSARS,
#endif

#ifdef _PETSKILL_ACUPUNCTURE
    CHAR_WORKACUPUNCTURE,
#endif
#ifdef _PETSKILL_RETRACE
    CHAR_WORKRETRACE,
#endif

#ifdef _CHATROOMPROTOCOL            // (���ɿ�) Syu ADD ������Ƶ��
    CHAR_WORKCHATROOMTYPE,
    CHAR_WORKCHATROOMNUM,
#endif

#ifdef _MAGIC_RESIST_EQUIT          // WON ADD ְҵ����װ��
    CHAR_WORK_F_SUIT,
    CHAR_WORK_I_SUIT,
    CHAR_WORK_T_SUIT,
#endif

#ifdef _PETSKILL_BECOMEFOX
    CHAR_WORKFOXROUND,  //�����������غ���
#endif

    CHAR_WORKSTANDBYPET, // Robin add ������
#ifdef _STREET_VENDOR
    CHAR_WORKSTREETVENDOR, // ��̯״̬ -1:û�ڰ�̯,Ҳû�ڰ�̯����;1:��̯��;2:�����ڽ�����;3:��̯����
    CHAR_WORKSTREETVENDOR_WHO, // ˭���ҽ���
#endif

#ifdef _ANGEL_SUMMON
    CHAR_WORKMISSION,
    CHAR_WORKANGELMODE,
#endif
#ifdef _MAP_TIME
    CHAR_WORK_MAP_TIME, // �����ͼ����
#endif
#ifdef _PETSKILL_LER
    CHAR_WORK_RELIFE,       // �׶�����
#endif
#ifdef _TEACHER_SYSTEM
    CHAR_WORK_GET_TEACHER_FAME, // ��ʦ�쵼����
#endif

    CHAR_WORKSKYWALKER, // GM������??

    CHAR_WORKLASTMERGETIME, // ����ϳ�ʱ��

#ifdef _FONT_SIZE
    CHAR_WORKFONTSIZE, //
#endif

#ifdef _TIME_TICKET
    CHAR_WORKTICKETTIME,
    CHAR_WORKTICKETTIMESTART,
#endif

    CHAR_WORKDEBUGMODE, // ����ģʽ��

    CHAR_WORKITEMINDEX, // ��Ʒ���

    CHAR_WORKTOITEMINDEX, // ��Ʒ���

#ifdef _NEW_STREET_VENDOR
    CHAR_WORKSTREETVENDORTYPE, // ��̯����
#endif

#ifdef _TALK_CHECK
    CHAR_WORK_TALKCHECK,  // ˵����֤
#endif
#ifdef _STRENGTH_PETSKILL
    CHAR_WORK_STRENGTH,
#endif
#ifdef _LOSTLOST_PETSKILL
    CHAR_WORK_LOSTLOST,
#endif
#ifdef _BATTLE_PK_TYPE
    CHAR_WORK_BATTLEPKTYPE,
#endif
#ifdef _THE_WORLD_SEND
    CHAR_WORKWORLDSENDTIME,
#endif
    CHAR_WORK_LISTPAGE,

#ifdef _AUTO_PK
    CHAR_WORK_AUTOPK,                   // �Զ���PK
    CHAR_WORK_AUTOPK_DEAD,              // ������
    CHAR_WORK_AUTOPK_DEFINDEX,      //�Է�INDEX
#endif

#ifdef _BATTLE_PK
    CHAR_WORK_BATTLEPK,
#endif
#ifdef _ONLINE_TIME
    CHAR_WORK_LOGIN_TIME,
#endif

#ifdef _OFFLINE_SYSTEM
    CHAR_WORK_OFFLINE,
#endif
#ifdef _SPECIAL_COUNTER
    CHAR_WORK_COUNTER,
#endif
#ifdef _SPECIAL_TIMER
    CHAR_WORK_TIMER,
#endif
#ifdef _RECORD_IP
    CHAR_WORK_RECORD_IP,
#endif
    CHAR_WORK_SHOWBATTLETIME,

#ifdef _BATTLE_BOUT_TIME
    CHAR_BATTLE_BOUT_TIME,
#endif


#ifdef _FIX_SAVE_CHAR
    CHAR_WORK_SAVE_CHAR,
#endif
#ifdef _BATTLE_OVER_STAR_TIME
    CHAR_WORKDBATTLETIME,
#endif
    CHAR_WORKDBATTLEESCAPE,

#ifdef _NEWCLISETSERVID
    CHAR_WORK_SERVID,
#endif

    CHAR_WORK_X,
    CHAR_WORK_Y,
    CHAR_WORK_W,
    CHAR_WORK_DIR,
#ifdef _CANCEL_STREET_VENDOR
    CHAR_WORK_STREET_VENDOR_TIME,
#endif
    CHAR_WORKUSEINDEX,
    CHAR_WORKTOPPINDEX,
#ifdef _ITEM_EXP_LV
    CHAR_WORKITEMEXP,
#endif
#ifdef _WAN_FIX
    CHAR_WORKBATTLEPROTYPE,
#endif
    CHAR_WATCHBATTLETYPE,
#ifdef _OFFLINE_BATTLE_RETEUN
    CHAR_OFFLINEBATTLENUM,
    CHAR_OFFLINEBATTLETIME,
#endif
    CHAR_FAMELEVEL,
#ifdef  _FIX_BATTLE_DIE
    CHAR_WORKBATTLEDIETYPE,
#endif
#ifdef  _FMBATTLE_POINT
    CHAR_WORKFMBATTLETYPE,
#endif
#ifdef _NPC_EVENT_NOTICE
    CHAR_WORK_NOTICE,
#endif
#ifdef _AIRCRAFT_PLUS_
    CHAR_WORK_AIRCRAFT_MODE,
#endif
#ifdef _LUA_RESIST
    CHAR_WORK_RESIST_POISON,
    CHAR_WORK_RESIST_PARALYSIS,
    CHAR_WORK_RESIST_SLEEP,
    CHAR_WORK_RESIST_STONE,
    CHAR_WORK_RESIST_DRUNK,
    CHAR_WORK_RESIST_CONFUSION,
    CHAR_WORK_RESIST_WEAKEN,
    CHAR_WORK_RESIST_DEEPPOISON,
    CHAR_WORK_RESIST_BARRIER,
    CHAR_WORK_RESIST_NOCAST,
#endif
#ifdef _ADDPARTY_
    CHAR_WORK_ISNEWPARTY,
#endif
#ifdef  _NEW_TEACHER_SYSTEM
    CHAR_WORK_NEWTEACHER_INDEX,
#endif
#ifdef  _PETSTANDY_ABDUCT
    CHAR_WORK_PETABDUCTTYPE,
#endif
    CHAR_WORK_DWTEMP,
#ifdef _IPHONEADDJIKOU_
    CHAR_WORK_IPHONE_INFO,
#endif
#ifdef _IPHONE_SHOP_FLAG_
    CHAR_WORK_IPHONE_FLAG,
#endif
#ifdef _PETSKILL_RIDE
    CHAR_WORK_RIDE,//���|,��������
#endif
#ifdef _SPEED_CHECK_
    CHAR_WORK_CHECK_SPEED_SEC,
    CHAR_WORK_CHECK_SPEED_USEC,
    CHAR_WORK_CHECK_SPEED_NUM,
#endif
    CHAR_WORK_MOONACT_NUM,
#ifdef _LC_CHECKNAME_
    CHAR_WORK_CHECK_NAME,
#endif
    CHAR_WORKBATTLEFINISHTIME,
    CHAR_WORKBATTLEEFFECTS,
    CHAR_WORKSAFEMODE,
#ifdef _YAYA_VOICE
    CHAR_WORKYAYAVOICE,
#endif
    CHAR_WORKDAMAGE,//�����˺�
    CHAR_WORKECHOTIME,//ECHO�������ʱ��
    CHAR_WORKECHONUM,//ECHO��������������
    CHAR_WORKBATTLEWAITCNT, //ս����ʱ����
    CHAR_NPCWORKINT15,
    CHAR_NPCWORKINT16,
    CHAR_NPCWORKINT17,
    CHAR_NPCWORKINT18,
    CHAR_NPCWORKINT19,
    CHAR_NPCWORKINT20,
    CHAR_WORKAUTOPARTY,//�Զ����ģʽ
#ifdef _PLAYER_QUESTION
    CHAR_WORKPLAYERQUESTION, //��Ҵ���
#endif // _PLAYER_QUESTION
    CHAR_WORKSKININDEX,
    CHAR_WORKSKINTIME,
    CHAR_WORKBATTLECOM7,
#ifdef _VARY_WOLF_JOINBATTLE
    CHAR_WORKPETDATA,
#endif
    CHAR_WORKRIDETIME,
    CHAR_WORKKICKTIME,
#ifdef _SMART_ENEMY_LUA
    CHAR_WORK_CRAZYFLG,
    CHAR_WORK_CRAZYCNT,
    CHAR_WORK_CRAZYROUND,
#endif
#ifdef _TRUE_DAMAGE_MAGIC
    CHAR_WORK_DAMGEFLG,
#endif
#ifdef _PETSKILL_TEMPTATION
    CHAR_WORKTEMPTATION,
#endif
    CHAR_WORKDATAINTNUM,

    CHAR_WORKENCOUNTPROBABILITY_MIN = CHAR_NPCWORKINT1,
    CHAR_WORKENCOUNTPROBABILITY_MAX = CHAR_NPCWORKINT2,
    CHAR_WORK_TOHELOS_CUTRATE = CHAR_NPCWORKINT3,
    CHAR_WORK_TOHELOS_COUNT = CHAR_NPCWORKINT4,
    CHAR_WORKSHOPRELEVANT = CHAR_NPCWORKINT5,
    CHAR_WORKSHOPRELEVANTSEC = CHAR_NPCWORKINT6,
    CHAR_WORKSHOPRELEVANTTRD = CHAR_NPCWORKINT7,
    CHAR_WORKTRADER = CHAR_NPCWORKINT8,
    CHAR_WORKEFFECT = CHAR_NPCWORKINT9,
    CHAR_WORKRENAMEITEMNUM = CHAR_NPCWORKINT10,
    CHAR_WORKRENAMEITEMINDEX = CHAR_NPCWORKINT5,
    CHAR_WORKPLAYERINDEX = CHAR_NPCWORKINT1,
    CHAR_WORKTACTICS = CHAR_NPCWORKINT2,
    CHAR_WORKPETFOLLOWMODE = CHAR_NPCWORKINT3,
    CHAR_WORKPETFOLLOWCOUNT = CHAR_NPCWORKINT4,
    CHAR_WORK_PETFLG = CHAR_NPCWORKINT1,
    CHAR_WORKGENERATEINDEX = CHAR_NPCWORKINT1,
    CHAR_WORKDOORCLOSETIME  = CHAR_NPCWORKINT1,
    CHAR_WORKDOORSWITCHCOUNT = CHAR_NPCWORKINT2,
    CHAR_WORKDOOROPENG = CHAR_NPCWORKINT3,
    CHAR_WORKDOORCLOSEG = CHAR_NPCWORKINT4,
    CHAR_WORKDOORSOONFLG = CHAR_NPCWORKINT5,
    CHAR_WORKDOORPASSFLG = CHAR_NPCWORKINT6,
    CHAR_WORKDOORKEYITEMID = CHAR_NPCWORKINT7,
    CHAR_WORKDOORSWITCHCURRENT = CHAR_NPCWORKINT8,
    CHAR_WORKDOORORIGINALGRAPHIC = CHAR_NPCWORKINT9,
    CHAR_WORKDOOREXPIRETIME = CHAR_NPCWORKINT10,
    CHAR_WORKOLDMANID = CHAR_NPCWORKINT1,
    CHAR_WORKSHOPCLIENTINDEX = CHAR_NPCWORKINT1,
    CHAR_WORKDENGONMAXID = CHAR_NPCWORKINT1,

} CHAR_WORKDATAINT;


enum
{
    WORKFLG_DEBUGMODE = (1 << 0),
    WORKFLG_MICMODE = (2 << 0),
#ifdef _SUPER_FLOOR_MIC
    WORKFLG_SUPERMICMODE = (3 << 0)
#endif
};

typedef enum
{
    CHAR_INITFUNC,
    CHAR_FIRSTFUNCTION = CHAR_INITFUNC,
    CHAR_WALKPREFUNC,
    CHAR_WALKPOSTFUNC,
    CHAR_PREOVERFUNC,
    CHAR_POSTOVERFUNC,
    CHAR_WATCHFUNC,
    CHAR_LOOPFUNC,
    CHAR_DYINGFUNC,
    CHAR_TALKEDFUNC,
    CHAR_PREATTACKEDFUNC,
    CHAR_POSTATTACKEDFUNC,
    CHAR_OFFFUNC,
    CHAR_LOOKEDFUNC,
    CHAR_ITEMPUTFUNC,
    CHAR_SPECIALTALKEDFUNC,
    CHAR_WINDOWTALKEDFUNC,
#ifdef _USER_CHARLOOPS
    CHAR_LOOPFUNCTEMP1,
    CHAR_LOOPFUNCTEMP2,
    CHAR_BATTLEPROPERTY,
#endif
#ifdef _ALLBLUES_LUA
    CHAR_OVERLAPEDFUNC,
    CHAR_BATTLEOVERDFUNC,
#ifdef _ALLBLUES_LUA_1_4
    CHAR_LOGINOUTFUNC,
#endif
#ifdef _ALLBLUES_LUA_1_9
    CHAR_BATTLESETFUNC,
#endif
#endif
    CHAR_LASTFUNCTION,
    CHAR_FUNCTABLENUM = CHAR_LASTFUNCTION,
} CHAR_FUNCTABLE;

typedef enum
{
    CHAR_WORKWALKARRAY,
    CHAR_NPCWORKCHAR1,
    CHAR_NPCWORKCHAR2,
    CHAR_NPCWORKCHAR3,
    CHAR_NPCWORKCHAR4,
    CHAR_NPCWORKCHAR5,
    CHAR_NPCWORKCHAR6,
#ifdef _ANGEL_SUMMON
    CHAR_WORKHEROINFO,
#endif
#ifdef _STREET_VENDOR
    CHAR_STREETVENDOR_NAME,
#endif

#ifdef _NEWCLISETMAC
    CHAR_WORK_MAC,
    CHAR_WORK_MAC2,
    CHAR_WORK_MAC3,
#endif
#ifdef _ITEM_MMEXP
    CHAR_WORK_MMEXPITEM,
#endif

    CHAR_WORKDATACHARNUM,
    CHAR_WORKBATTLE_TACTICSOPTION = CHAR_NPCWORKCHAR1,
#ifdef _BATTLENPC_WARP_PLAYER
    CHAR_WORKBATTLE_ACT_CONDITION = CHAR_NPCWORKCHAR2,
#endif
    CHAR_WORKDOORPASSWD         = CHAR_NPCWORKCHAR1,
    CHAR_WORKDOORNAME           = CHAR_NPCWORKCHAR2,
    CHAR_WORKDOORGOLDLIMIT      = CHAR_NPCWORKCHAR3,
    CHAR_WORKDOORWEEK           = CHAR_NPCWORKCHAR4,
    CHAR_WORKDOORHOUR           = CHAR_NPCWORKCHAR5,
    CHAR_WORKDOORMINUTE         = CHAR_NPCWORKCHAR6,
    CHAR_WORKDOORTITLE          = CHAR_NPCWORKCHAR6,
    CHAR_WORKDOORMANDOORNAME    = CHAR_NPCWORKCHAR1,
    CHAR_TIME1,
    CHAR_TIME2          = CHAR_NPCWORKCHAR6,

} CHAR_WORKDATACHAR;


typedef enum
{
    CHAR_IMAGETYPE_GIRL,
    CHAR_IMAGETYPE_BOY,
    CHAR_IMAGETYPE_CHILDBOY,
    CHAR_IMAGETYPE_CHILDGIRL,
    CHAR_IMAGETYPE_MAN,
    CHAR_IMAGETYPE_WOMAN,
    CHAR_IMAGETYPE_OLDMAN,
    CHAR_IMAGETYPE_DOG,
    CHAR_IMAGETYPENUM
} CHAR_ImageType;


#define         CHAR_P_STRING_HP                ( 1 << 1  )     // 0x00000002
#define         CHAR_P_STRING_MAXHP             ( 1 << 2  )     // 0x00000004
#define         CHAR_P_STRING_MP                ( 1 << 3  )     // 0x00000008
#define         CHAR_P_STRING_MAXMP             ( 1 << 4  )     // 0x00000010
#define         CHAR_P_STRING_VITAL             ( 1 << 5  )
#define         CHAR_P_STRING_STR               ( 1 << 6  )
#define         CHAR_P_STRING_TOUGH             ( 1 << 7  )
#define         CHAR_P_STRING_DEX               ( 1 << 8  )
#define         CHAR_P_STRING_EXP               ( 1 << 9  )
#define         CHAR_P_STRING_NEXTEXP           ( 1 << 10 )
#define         CHAR_P_STRING_LV                ( 1 << 11 )
#define         CHAR_P_STRING_ATK               ( 1 << 12 )
#define         CHAR_P_STRING_DEF               ( 1 << 13 )
#define         CHAR_P_STRING_QUICK             ( 1 << 14 )
#define         CHAR_P_STRING_CHARM             ( 1 << 15 )
#define         CHAR_P_STRING_LUCK              ( 1 << 16 )
#define         CHAR_P_STRING_EARTH             ( 1 << 17 )
#define         CHAR_P_STRING_WATER             ( 1 << 18 )
#define         CHAR_P_STRING_FIRE              ( 1 << 19 )
#define         CHAR_P_STRING_WIND              ( 1 << 20 )
#define         CHAR_P_STRING_GOLD              ( 1 << 21 )
#ifdef _NEW_ADDEXPTIME_SHOW
#define         CHAR_P_STRING_ADDEXPTIME        ( 1 << 22 )
#else
#define         CHAR_P_STRING_TITLE             ( 1 << 22 )
#endif // _NEW_ADDEXPTIME_SHOW
#define         CHAR_P_STRING_DUELPOINT         ( 1 << 23 )
#define         CHAR_P_STRING_TRANSMIGRATION    ( 1 << 24 )
#define         CHAR_P_STRING_NAME              ( 1 << 25 )
#define         CHAR_P_STRING_OWNTITLE          ( 1 << 26 )
#define         CHAR_P_STRING_RIDEPET           ( 1 << 27 )     // 0x08000000
#define         CHAR_P_STRING_LEARNRIDE         ( 1 << 28 )     // 0x10000000
#define         CHAR_P_STRING_BASEBASEIMAGENUMBER   ( 1 << 29 )     // 0x20000000
//#define           CHAR_P_STRING_PKDP              ( 1 << 30 )
//#ifdef _CHAR_PROFESSION
//#define           CHAR_P_STRING_PROFESSION                ( 1 << 30 )
//#endif
#define         CHAR_P_STRING_SKYWALKER         ( 1 << 30 )     // 0x40000000
#define         CHAR_P_STRING_DEBUGMODE         ( 1 << 31 )     // 0x80000000



#define         CHAR_N_STRING_OBJINDEX          ( 1 << 1  )
#define         CHAR_N_STRING_LV                ( 1 << 2  )
#define         CHAR_N_STRING_MAXHP             ( 1 << 3  )
#define         CHAR_N_STRING_HP                ( 1 << 4  )
#define         CHAR_N_STRING_MP                ( 1 << 5  )
#define         CHAR_N_STRING_NAME              ( 1 << 6  )

#define         CHAR_K_STRING_BASEIMAGENUMBER   ( 1 << 1  )
#define         CHAR_K_STRING_HP                ( 1 << 2  )
#define         CHAR_K_STRING_MAXHP             ( 1 << 3  )
#define         CHAR_K_STRING_MP                ( 1 << 4  )
#define         CHAR_K_STRING_MAXMP             ( 1 << 5  )
#define         CHAR_K_STRING_EXP               ( 1 << 6  )
#define         CHAR_K_STRING_NEXTEXP           ( 1 << 7  )
#define         CHAR_K_STRING_LV                ( 1 << 8  )
#define         CHAR_K_STRING_ATK               ( 1 << 9  )
#define         CHAR_K_STRING_DEF               ( 1 << 10 )
#define         CHAR_K_STRING_QUICK             ( 1 << 11 )
#define         CHAR_K_STRING_AI                ( 1 << 12 )
#define         CHAR_K_STRING_EARTH             ( 1 << 13 )
#define         CHAR_K_STRING_WATER             ( 1 << 14 )
#define         CHAR_K_STRING_FIRE              ( 1 << 15 )
#define         CHAR_K_STRING_WIND              ( 1 << 16 )
#define         CHAR_K_STRING_SLOT              ( 1 << 17 )
#define         CHAR_K_STRING_CHANGENAMEFLG     ( 1 << 18 )
#define         CHAR_K_STRING_NAME              ( 1 << 19 )
#define         CHAR_K_STRING_USERPETNAME       ( 1 << 20 )
#ifdef _PETCOM_
#define         CHAR_K_STRING_YHP               ( 1 << 21 )
#define         CHAR_K_STRING_YATK              ( 1 << 22 )
#define         CHAR_K_STRING_YDEF              ( 1 << 23 )
#define         CHAR_K_STRING_YQUICK            ( 1 << 24 )
#define         CHAR_K_STRING_YLV               ( 1 << 25 )
#endif
#ifdef _PET_LV1_4V
#define         CHAR_K_STRING_YHP               ( 1 << 21 )
#define         CHAR_K_STRING_YATK              ( 1 << 22 )
#define         CHAR_K_STRING_YDEF              ( 1 << 23 )
#define         CHAR_K_STRING_YQUICK            ( 1 << 24 )
#define         CHAR_K_STRING_YLV               ( 1 << 25 )
#endif

typedef struct
{
    int     indexOfPet[CHAR_MAXPETHAVE];
    int       indexOfPetskill[CHAR_MAXPETSKILLHAVE];
} CHAR_UNIONTABLE;

typedef struct tagChar
{
    BOOL        use;

    int         data[CHAR_DATAINTNUM];
    STRING128   string[CHAR_DATACHARNUM];
    char        flg[(CHAR_FLGNUM % (sizeof(char) * 8))
                                                 ? (CHAR_FLGNUM / (sizeof(char) * 8)) + 1
                                                 : (CHAR_FLGNUM / (sizeof(char) * 8))];
    int         indexOfExistItems[CHAR_MAXITEMHAVE];
    int         indexOfExistPoolItems[CHAR_MAXPOOLITEMHAVE];

#ifdef _CHAR_POOLITEM
    int         *indexOfExistDepotItems;
#endif
#ifdef _CHAR_POOLPET
    int         *indexOfExistDepotPets;
#endif

    CHAR_HaveSkill      haveSkill[CHAR_SKILLMAXHAVE];
    int         indexOfHaveTitle[CHAR_TITLEMAXHAVE];
    ADDRESSBOOK_entry   addressBook[ADDRESSBOOK_MAX];
    CHAR_UNIONTABLE     unionTable;
    int         indexOfPoolPet[CHAR_MAXPOOLPETHAVE];
    STRING32    charfunctable[CHAR_FUNCTABLENUM];
    int         workint[CHAR_WORKDATAINTNUM];
    STRING64    workchar[CHAR_WORKDATACHARNUM];
    int         CharMakeSequenceNumber;
    void*       functable[CHAR_FUNCTABLENUM];
#ifdef _STREET_VENDOR
    StreetVendor_t StreetVendor[MAX_SELL_ITEM];
#endif
#ifdef _ALLBLUES_LUA
    lua_State *lua[CHAR_FUNCTABLENUM];
    char *luafunctable[CHAR_FUNCTABLENUM];
#endif
} Char;


#define     CHAR_CHECKINDEX( index) _CHAR_CHECKINDEX( __FILE__, __LINE__, index)
INLINE BOOL _CHAR_CHECKINDEX(char *file, int line, int index);
#define     CHAR_CHECKITEMINDEX( charaindex, iindex) _CHAR_CHECKITEMINDEX( __FILE__, __LINE__, charaindex, iindex)
INLINE BOOL _CHAR_CHECKITEMINDEX(char *file, int line, int charaindex, int iindex);
#define     CHAR_CHECKPETINDEX( petindex) _CHAR_CHECKPETINDEX( __FILE__, __LINE__, petindex )
INLINE BOOL _CHAR_CHECKPETINDEX(char *file, int line, int petindex);
#define     CHAR_CHECKPOOLPETINDEX( petindex) _CHAR_CHECKPOOLPETINDEX( __FILE__, __LINE__, petindex )
INLINE BOOL _CHAR_CHECKPOOLPETINDEX(char *file, int line, int petindex);
#define     CHAR_CHECKPETSKILLINDEX( havepetskillindex) _CHAR_CHECKPETSKILLINDEX( __FILE__, __LINE__, havepetskillindex)
INLINE BOOL _CHAR_CHECKPETSKILLINDEX(char *file, int line, int havepetskillindex);

//INLINE int CHAR_getInt( int index , CHAR_DATAINT element);

#define CHAR_getInt( index , element) _CHAR_getInt( __FILE__, __LINE__, index , element)
INLINE int _CHAR_getInt(char *file, int line, int index, CHAR_DATAINT element);
#define CHAR_setInt( index , element, data) _CHAR_setInt( __FILE__, __LINE__, index , element, data)
INLINE int _CHAR_setInt(char *file, int line, int index, CHAR_DATAINT element, int data);
#ifdef _FIX_SETWORKINT
#define CHAR_getWorkInt( index , element) _CHAR_getWorkInt( __FILE__, __LINE__, index , element)
INLINE int _CHAR_getWorkInt(char *file, int line, int index, CHAR_WORKDATAINT element);
#define CHAR_setWorkInt( index, element, data) _CHAR_setWorkInt( __FILE__, __LINE__, index , element, data)
INLINE int _CHAR_setWorkInt(char *file, int line, int index, CHAR_WORKDATAINT element, int data);
#else
INLINE int CHAR_setWorkInt(int index, CHAR_WORKDATAINT element, int data);
#endif
#define     CHAR_getChar( index, elem) _CHAR_getChar( __FILE__, __LINE__, index, elem)
INLINE char* _CHAR_getChar(char *file, int line, int index, CHAR_DATACHAR element);
#define     CHAR_setChar( index, elem, n) _CHAR_setChar( __FILE__, __LINE__, index, elem, n)
INLINE BOOL _CHAR_setChar(char *file, int line, int index, CHAR_DATACHAR element, const char* new);
#define     CHAR_getFlg( index, elem) _CHAR_getFlg( __FILE__, __LINE__, index, elem)
INLINE BOOL _CHAR_getFlg(char *file, int line, int index, CHAR_DATAFLG element);
#define     CHAR_setFlg( index, elem, n) _CHAR_setFlg( __FILE__, __LINE__, index, elem, n)
INLINE BOOL _CHAR_setFlg(char *file, int line, int index, CHAR_DATACHAR element, int newdata);
#define     CHAR_getWorkChar( index, elem) _CHAR_getWorkChar( __FILE__, __LINE__, index, elem)
INLINE char * _CHAR_getWorkChar(char *file, int line, int index, CHAR_WORKDATACHAR element);
#define     CHAR_setWorkChar( index, elem, n) _CHAR_setWorkChar( __FILE__, __LINE__, index, elem, n)
INLINE BOOL _CHAR_setWorkChar(char *file, int line, int index, CHAR_WORKDATACHAR element, const char * new);

#define     CHAR_getItemIndex( index, iindex) _CHAR_getItemIndex( __FILE__, __LINE__, index, iindex)
INLINE int _CHAR_getItemIndex(char *file, int line, int charaindex, int ti);
#define     CHAR_setItemIndex( index, iindex,id) _CHAR_setItemIndex( __FILE__, __LINE__, index, iindex, id)
INLINE int _CHAR_setItemIndex(char *file, int line, int index, int iindex, int id);
#define     CHAR_getPoolItemIndex( index, iindex) _CHAR_getPoolItemIndex( __FILE__, __LINE__, index, iindex)
INLINE int _CHAR_getPoolItemIndex(char *file, int line, int index, int iindex);
#define     CHAR_setPoolItemIndex( index, iindex,id) _CHAR_setPoolItemIndex( __FILE__, __LINE__, index, iindex, id)
INLINE int _CHAR_setPoolItemIndex(char *file, int line, int index, int iindex, int id);
INLINE BOOL CHAR_setAddressbookEntry(int index, int aindex, ADDRESSBOOK_entry *a);
INLINE ADDRESSBOOK_entry* CHAR_getAddressbookEntry(int index, int aindex);
INLINE Char* CHAR_getCharPointer(int index);
INLINE int CHAR_getCharNum(void);
INLINE int CHAR_getPlayerMaxNum(void);
INLINE int CHAR_getPetMaxNum(void);
INLINE int CHAR_getOthersMaxNum(void);
INLINE BOOL CHAR_getCharUse(int index);
INLINE CHAR_HaveSkill* CHAR_getCharHaveSkill(int index, int sindex);
INLINE int  CHAR_getCharHaveTitle(int index, int tindex);
INLINE int CHAR_setCharHaveTitle(int charaindex, int tindex, int new);
INLINE int CHAR_getCharPet(int charaindex, int petindex);
INLINE int CHAR_setCharPet(int charaindex, int petindex, int new);
INLINE int CHAR_getCharPoolPet(int charaindex, int petindex);
INLINE int CHAR_setCharPoolPet(int charaindex, int petindex, int new);
int CHAR_getCharPetElement(int charaindex);
int CHAR_getCharPoolPetElement(int charaindex);
int CHAR_getCharPoolItemIndexElement(int charaindex);
int CHAR_getEmptyCharPoolItemIndexNum(int charaindex);
#ifdef _CHAR_POOLPET
int CHAR_getCharDepotPetElement(int charaindex);
#endif
#define     CHAR_getIntPSkill( index, skillti, ti) _CHAR_getIntPSkill( __FILE__, __LINE__, index, skillti, ti)
INLINE int _CHAR_getIntPSkill(char *file, int line, int index, int skillti, SKILL_DATAINT ti);

#define     CHAR_setIntPSkill( index, skillti, ti, data) _CHAR_setIntPSkill( __FILE__, __LINE__, index, skillti, ti, data)
INLINE void _CHAR_setIntPSkill(char *file, int line, int index, int skillti, SKILL_DATAINT ti, int data);


extern Char *CHAR_chara;


INLINE int CHAR_getCharMakeSequenceNumber(int charaindex);
void CHAR_constructFunctable(int charaindex);
void* CHAR_getFunctionPointer(int charaindex, int functype);
#ifdef _ALLBLUES_LUA

INLINE BOOL CHAR_setLUAFunction(int charaindex, int functype, lua_State *L, const char *luafunctable);
INLINE BOOL CHAR_delLUAFunction(int charaindex, int functype);
INLINE lua_State *CHAR_getLUAFunction(int charaindex, int functype);
#endif
BOOL CHAR_initCharArray(int pnum,  int petnum, int onum);
BOOL CHAR_endCharArray(void);
int CHAR_initCharOneArray(Char* ch);
void _CHAR_endCharOneArray(int index, char*file, int line);
#define     CHAR_endCharOneArray( index) _CHAR_endCharOneArray( index, __FILE__, __LINE__)
void CHAR_removeHaveItem(Char* ch);
void CHAR_endCharData(Char* ch);
char* CHAR_makeStringFromCharData(Char* one);
char* CHAR_makeStringFromCharIndex(int index);
BOOL CHAR_makeCharFromStringToArg(char* data, Char* one);
int CHAR_makePetFromStringToArg(char *src, Char *ch, int ti);
char *CHAR_makePetStringFromPetIndex(int petindex);
#define CHAR_getPetSkillElement( petindex) _CHAR_getPetSkillElement( __FILE__, __LINE__, petindex)
int _CHAR_getPetSkillElement(char *file, int line, int petindex);
#define CHAR_setPetSkill( petindex, havepetskillindex, n) _CHAR_setPetSkill( __FILE__, __LINE__, petindex, havepetskillindex, n)
INLINE int _CHAR_setPetSkill(char *file, int line, int petindex, int havepetskillindex, int new);
#define     CHAR_getPetSkill( petindex, havepetskillindex) _CHAR_getPetSkill( __FILE__, __LINE__, petindex, havepetskillindex)
INLINE int _CHAR_getPetSkill(char *file, int line, int petindex, int havepetskillindex);
#define SETFLG(a,b,c,d,e,f,g,h) (((a)<<0)+((b)<<1)+((c)<<2)+((d)<<3)+((e)<<4)+((f)<<5)+((g)<<6)+((h)<<7))

INLINE int CHAR_AddMaxExp(int charaindex, int addexp);
INLINE int CHAR_setMaxExpFromLevel(int charaindex, int level);
INLINE int CHAR_setMaxExp(int charaindex, unsigned long int Setexp);
#ifdef _NEWOPEN_MAXEXP
INLINE int CHAR_ChangeExp(int charaindex);
INLINE int CHAR_HandleExp(int charaindex);

#endif

// ride Pet table
typedef struct tagtagRidePetTable
{
    int rideNo;
    int charNo;
    int petNo;
    int petId;

} tagRidePetTable;


// CoolFish: Family Member Kind 2001/8/28
typedef enum
{
    FMMEMBER_NONE = -1,
    FMMEMBER_MEMBER = 1,
    FMMEMBER_APPLY,
    FMMEMBER_LEADER,
    FMMEMBER_ELDER,
} CHAR_FM_MEMBERKIND;

// shan 2002/01/10
typedef enum
{
    PET_STAT_NONE = 0,
    PET_STAT_SELECT,
    PET_STAT_MAIL = 4,
} CHAR_PET_STAT;


#ifdef _PERSONAL_FAME   // Arminius: �����������
void CHAR_earnFame(int index, int fame);
#endif

// CoolFish: 2001/10/11
#ifdef _UNIQUE_P_I
void CHAR_setPetUniCode(int petindex);
void ITEM_setItemUniCode(int itemindex);
void Check_P_I_UniCode(int charindex);
#endif

// Arminius 12.15 �ж��Ƿ�����ʿ
int IsMale(int charindex);

// Arminius 12.15 �ж��Ƿ���Ůʿ
int IsFemale(int charindex);


#ifdef _NEW_RIDEPETS
#define RIDE_PET0   1
#define RIDE_PET1   ( 1 << 1 )
#define RIDE_PET2   ( 1 << 2 )
#define RIDE_PET3   ( 1 << 3 )
#define RIDE_PET4   ( 1 << 4 )
#define RIDE_PET5   ( 1 << 5 )
#define RIDE_PET6   ( 1 << 6 )
#define RIDE_PET7   ( 1 << 7 )
#define RIDE_PET8   ( 1 << 8 )
#define RIDE_PET9   ( 1 << 9 )
#define RIDE_PET10  ( 1 << 11 )
#define RIDE_PET11  ( 1 << 10 )
#define RIDE_PET12  ( 1 << 12 )
#define RIDE_PET13  ( 1 << 13 )
#define RIDE_PET14  ( 1 << 14 )
#ifdef _RIDE_CF
#define RIDE_PET15 ( 1 << 15 )
#define RIDE_PET16 ( 1 << 16 )
#define RIDE_PET17 ( 1 << 17 )
#define RIDE_PET19 ( 1 << 19 )
#define RIDE_PET18 ( 1 << 18 )
#define RIDE_PET20 ( 1 << 20 )
#define RIDE_PET21 ( 1 << 21 )
#define RIDE_PET22 ( 1 << 22 )
#define RIDE_PET23 ( 1 << 23 )
#define RIDE_PET24 ( 1 << 24 )
#define RIDE_PET25 ( 1 << 25 )
#define RIDE_PET26 ( 1 << 26 )
#define RIDE_PET27 ( 1 << 27 )
#define RIDE_PET28 ( 1 << 28 )
#define RIDE_PET29 ( 1 << 29 )
#define RIDE_PET30 ( 1 << 30 )
#define RIDE_PET31 ( 1 << 31 )
#endif
#define RIDE_TPYE1 1
#define RIDE_TPYE2 ( 1 << 1 )

#define RIDE_PET_ALL    ( 0xFFFFFFFF )
#define RIDE_TPYE_ALL   ( 0xFFFFFFFF )

#ifdef _ADD_RIDE_CF
#define MAXNOINDEX 32*3
#else
#define MAXNOINDEX 32*2
#endif
#else
#define RIDE_PET_ALL    ( RIDE_PET0|RIDE_PET1|RIDE_PET2|RIDE_PET3|RIDE_PET4|RIDE_PET5|RIDE_PET6|RIDE_PET7|RIDE_PET8|RIDE_PET9|RIDE_PET10|RIDE_PET11 )
#define MAXNOINDEX 15
#endif


#ifdef _PET_BEATITUDE
#define BEATITUDE_VITAL ( 1 << 0 )
#define BEATITUDE_STR   ( 1 << 1 )
#define BEATITUDE_TOUGH ( 1 << 2 )
#define BEATITUDE_DEX   ( 1 << 3 )
#endif

#ifdef _FM_LEADER_RIDE
typedef struct
{
    int fmfloor;
    int ride[3];
} tagFmLeaderRide;
#endif

#ifdef _FM_LEADER_RIDE
int CHAR_FmLeaderRide(int meindex, int pet);
#endif

typedef struct
{
    int petNo;
#ifdef _RIDE_CF
    int petNo1;
#ifdef _ADD_RIDE_CF
    int petNo2;
#endif
#endif
    unsigned int learnCode;
} tagRideCodeMode;

typedef struct
{
    int RideNo[MAXNOINDEX];
    int flg;
} tagRideNoList;

typedef struct
{
    int charNo;
    int Noindex;
    int sex;
} tagRidePetList;

int RIDEPET_getNOindex(int baseNo);
#ifdef _RIDE_CF
#ifdef _ADD_RIDE_CF
int RIDEPET_getPETindex(int PetNo, unsigned int learnCode, int unsigned learnCode1, int unsigned learnCode2);
#else
int RIDEPET_getPETindex(int PetNo, unsigned int learnCode, int unsigned learnCode1);
#endif
#else
int RIDEPET_getPETindex(int PetNo, unsigned int learnCode);
#endif
int RIDEPET_getRIDEno(int index, int ti);

#ifdef _FUSION_TWO
int CHAR_OldFusion_init();
#endif

#ifdef _PET_EVOLUTION
int EVOLUTION_getPetTable(int charaindex, int petindex1, int petindex2);
int EVOLUTION_getPropertyTable(int charaindex, int petindex1, int petindex2);
int EVOLUTION_getFusionTable(int charaindex, int px, int py);
int EVOLUTION_getPetFusionCode(int petid);
#endif

#ifdef _EMENY_CHANCEMAN
int CHAR_getSexInt(int baseNo);
#endif
#ifdef _TYPE_TOXICATION
BOOL CHAR_CanCureFlg(int charaindex, char *arg);
#endif

BOOL CHAR_getCharOnArrayPercentage(int mode, int *max, int *min, int *cnt);


#define CHAR_DelItemMess( index, ti, flg) _CHAR_DelItem( __FILE__, __LINE__, index, ti, 1, flg)
#define CHAR_DelItem( index, ti) _CHAR_DelItem( __FILE__, __LINE__, index, ti, 1, 1)
#define CHAR_DelPileItemMess( index, ti, num, flg) _CHAR_DelItem( __FILE__, __LINE__, index, ti, num, flg)

#define CHAR_AddPileItem( index, itemindex) _CHAR_AddPileItem( __FILE__, __LINE__, index, itemindex)
INLINE int _CHAR_AddPileItem(char *file, int line, int charaindex, int itemindex);

INLINE int _CHAR_DelItem(char *file, int line, int charaindex, int ti, int num, int flg);

#define CHAR_AddGold( index, ti) _CHAR_AddGold( __FILE__, __LINE__, index, ti)
INLINE int _CHAR_AddGold(char *file, int line, int charaindex, int gold);
#define CHAR_DelGold( index, ti) _CHAR_DelGold( __FILE__, __LINE__, index, ti)
INLINE int _CHAR_DelGold(char *file, int line, int charaindex, int gold);
int CHAR_getMaxHaveGold(int charaindex);


#ifdef _CHAR_PROFESSION         // WON ADD ����ְҵ

int CHAR_getCharSkill(int index, int sindex);
int CHAR_setCharSkill(int index, int sindex, int new);
int CHAR_getCharSkillLevel(int index, int sindex);
void CHAR_setCharSkillLevel(int index, int sindex, int new);
#define CHAR_CHECK_PROFESSION_SKILLINDEX( skillindex) _CHAR_CHECK_PROFESSION_SKILLINDEX( __FILE__, __LINE__, skillindex)
INLINE BOOL _CHAR_CHECK_PROFESSION_SKILLINDEX(char *file, int line, int skillindex);
#define PROFESSION_SKILL_getSkill( charaindex, skillindex) _PROFESSION_SKILL_getSkill( __FILE__, __LINE__, charaindex, skillindex)
INLINE int _PROFESSION_SKILL_getSkill(char *file, int line, int charaindex, int skillindex);
#define PROFESSION_SKILL_setSkill( charaindex, skillindex, n) _PROFESSION_SKILL_setSkill( __FILE__, __LINE__, charaindex, skillindex, n)
INLINE int _PROFESSION_SKILL_setSkill(char *file, int line, int charaindex, int skillindex, int new);
#define PROFESSION_SKILL_getSkillElement(charaindex) _PROFESSION_SKILL_getSkillElement( __FILE__, __LINE__, charaindex)
int _PROFESSION_SKILL_getSkillElement(char *file, int line, int charaindex);

#endif

#ifdef _NPC_SEPARATEDATA
void CHAR_showTempInt(int index);
BOOL CHAR_InitCharTempNum(Char* ch);
void CHAR_EndCharTempNum(void);
void CHAR_getDefaultCharTempNum(Char* ch);
#endif

#ifdef _CHAR_POOLITEM

#define CHAR_getDepotItemIndex( index, iindex) _CHAR_getDepotItemIndex( __FILE__, __LINE__, index, iindex)
INLINE int _CHAR_getDepotItemIndex(char *file, int line, int index, int iindex);
#define CHAR_setDepotItemIndex( index, iindex,id) _CHAR_setDepotItemIndex( __FILE__, __LINE__, index, iindex, id)
INLINE int _CHAR_setDepotItemIndex(char *file, int line, int index, int iindex, int id);

void CHAR_removeHaveDepotItem(Char* ch);
void CHAR_removeDepotItem(int charaindex);

char *CHAR_makeDepotItemFromCharIndex(int charaindex);
BOOL CHAR_makeDepotItemStringToChar(int charaindex, char* data);

BOOL CHAR_SaveDepotItem(int charaindex);
BOOL CHAR_GetDepotItem(int meindex, int charaindex);
void CHAR_ShowMyDepotItems(int charaindex);

int CHAR_findEmptyDepotItem(int charaindex);
int CHAR_getfindEmptyDepotItem(int charaindex);
BOOL CHAR_CheckDepotItem(int charaindex);
#endif

#ifdef _CHAR_POOLPET

#define CHAR_getDepotPetIndex( index, iindex) _CHAR_getDepotPetIndex( __FILE__, __LINE__, index, iindex)
INLINE int _CHAR_getDepotPetIndex(char *file, int line, int index, int iindex);
#define CHAR_setDepotPetIndex( index, iindex,id) _CHAR_setDepotPetIndex( __FILE__, __LINE__, index, iindex, id)
INLINE int _CHAR_setDepotPetIndex(char *file, int line, int index, int iindex, int id);

void CHAR_removeHaveDepotPet(Char* ch);
void CHAR_removeDepotPet(int charaindex);

char *CHAR_makeDepotPetFromCharIndex(int charaindex);
BOOL CHAR_makeDepotPetStringToChar(int charaindex, char* data);

BOOL CHAR_SaveDepotPet(int charaindex);
BOOL CHAR_GetDepotPet(int meindex, int charaindex);
void CHAR_ShowMyDepotPets(int charaindex);

int CHAR_findEmptyDepotPet(int charaindex);
int CHAR_getfindEmptyDepotPet(int charaindex);
BOOL CHAR_CheckDepotPet(int charaindex);
#endif

#ifdef _RIDE_CF
int CHAR_Ride_CF_init();
#endif

void LodBadPetString(char *data, char *err, int ti);

#ifdef _STREET_VENDOR
BOOL CHAR_setStreetVendor(int charindex, int index, int set, int num);
INLINE void CHAR_clearStreetVendor(int charindex, int index);
int CHAR_getStreetVendor(int charindex, int index, int set);
#endif

#ifdef _FM_LEADER_RIDE
int CHAR_FmLeaderRide_init();
#endif

#ifdef _ANGEL_SUMMON

#define ANGELITEM 2884 //20701  //ʹ�ߵ����� ���߱��
#define HEROITEM 2885 //20702   //���ߵ����� ���߱��

typedef enum
{
    ANGEL_NONE = 0,
    ANGEL_ANGEL,
    ANGEL_HERO,
} ANGEL_TYPE;

typedef enum
{
    MISSION_NONE = 0,
    MISSION_WAIT_ANSWER,
    MISSION_DOING,
    MISSION_HERO_COMPLETE,
    MISSION_TIMEOVER,
} ANGEL_MISSIONFLAG;
#endif

#endif

