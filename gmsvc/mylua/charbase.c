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
    {{"����"},                        CHAR_WORKOBJINDEX}
#ifdef _FONT_SIZE
    , {{"�ֺ�"},                   CHAR_WORKFONTSIZE}
#endif
    , { { "AI" }, CHAR_WORKFIXAI }
    , {{"ս��"},                   CHAR_WORKBATTLEMODE}
    , {{"���"},                   CHAR_WORKPARTYMODE}
    , {{"ս������"},             CHAR_WORKBATTLEINDEX}
    , {{"NPC��ʱ1"},           CHAR_NPCWORKINT1}
    , {{"NPC��ʱ2"},             CHAR_NPCWORKINT2}
    , {{"NPC��ʱ3"},             CHAR_NPCWORKINT3}
    , {{"NPC��ʱ4"},             CHAR_NPCWORKINT4}
    , {{"NPC��ʱ5"},             CHAR_NPCWORKINT5}
    , {{"NPC��ʱ6"},             CHAR_NPCWORKINT6}
    , {{"NPC��ʱ7"},             CHAR_NPCWORKINT7}
    , {{"NPC��ʱ8"},             CHAR_NPCWORKINT8}
    , {{"NPC��ʱ9"},             CHAR_NPCWORKINT9}
    , {{"NPC��ʱ10"},            CHAR_NPCWORKINT10}
    , {{"NPC��ʱ11"},            CHAR_NPCWORKINT11}
    , {{"NPC��ʱ12"},          CHAR_NPCWORKINT12}
    , {{"NPC��ʱ13"},          CHAR_NPCWORKINT13}
    , { { "NPC��ʱ14" }, CHAR_NPCWORKINT14 }
    , { { "NPC��ʱ15" }, CHAR_NPCWORKINT15 }
    , { { "NPC��ʱ16" }, CHAR_NPCWORKINT16 }
    , { { "NPC��ʱ17" }, CHAR_NPCWORKINT17 }
    , { { "NPC��ʱ18" }, CHAR_NPCWORKINT18 }
    , { { "NPC��ʱ19" }, CHAR_NPCWORKINT19 }
    , { { "NPC��ʱ20" }, CHAR_NPCWORKINT20 }
    , {{"����ģʽ"},             CHAR_WORKPETFOLLOWMODE}
    , {{"��Ա1"},                    CHAR_WORKPARTYINDEX1}
    , {{"��Ա2"},                    CHAR_WORKPARTYINDEX2}
    , {{"��Ա3"},                    CHAR_WORKPARTYINDEX3}
    , {{"��Ա4"},                  CHAR_WORKPARTYINDEX4}
    , {{"��Ա5"},                  CHAR_WORKPARTYINDEX5}
#ifdef _AIRCRAFT_PLUS_
    , {{"��Ա6"},                  CHAR_WORKPARTYINDEX6}
    , {{"��Ա7"},                  CHAR_WORKPARTYINDEX7}
    , {{"��Ա8"},                  CHAR_WORKPARTYINDEX8}
#endif
    , {{"���HP"},                 CHAR_WORKMAXHP}
    , {{"���MP"},                 CHAR_WORKMAXMP}
    , {{"������"},                    CHAR_WORKATTACKPOWER}
    , {{"������"},                    CHAR_WORKDEFENCEPOWER}
    , {{"������"},                    CHAR_WORKQUICK}
    , {{"��������"},             CHAR_WORKFIXVITAL}
    , {{"��������"},             CHAR_WORKFIXSTR}
    , {{"��������"},             CHAR_WORKFIXTOUGH}
    , {{"�����ٶ�"},             CHAR_WORKFIXDEX}
    , {{"�����ͼ"},             CHAR_WORKFMFLOOR}
    , {{"����"},                       CHAR_WORKATTACKPOWER}
    , {{"����"},                       CHAR_WORKDEFENCEPOWER}
    , {{"����"},                       CHAR_WORKQUICK}
    , {{"����"},                       CHAR_WORKDBATTLEESCAPE}
    , {{"����ʱ��"},             CHAR_WORKITEM_ADDEXPTIME}
    , {{"����ӳ�"},             CHAR_WORKITEM_ADDEXP}
#ifdef _OFFLINE_SYSTEM
    , {{"����"},                   CHAR_WORK_OFFLINE}
#endif
    , {{"��þ���"},             CHAR_WORKGETEXP}

#ifdef _AUTO_PK
    , {{"�Զ�PK��"},              CHAR_WORK_AUTOPK}
    , {{"�Զ�PK����"},           CHAR_WORK_AUTOPK_DEAD}
    , {{"�Զ�PK����"},           CHAR_WORK_AUTOPK_DEFINDEX}
#endif
#ifdef _SPECIAL_COUNTER
    , {{"������"},                CHAR_WORK_COUNTER}
#endif
#ifdef _SPECIAL_TIMER
    , {{"��ʱ��"},                CHAR_WORK_TIMER}
#endif
#ifdef _STREET_VENDOR
    , {{"��̯"},                   CHAR_WORKSTREETVENDOR}
#endif
#ifdef _RECORD_IP
    , {{"IP"},                       CHAR_WORK_RECORD_IP}
#endif
#ifdef _ONLINE_TIME
    , {{"��½ʱ��"},             CHAR_WORK_LOGIN_TIME}
#endif
#ifdef _NEWCLISETSERVID
    , {{"������ID"},          CHAR_WORK_SERVID}
#endif
    , {{"PKʱ��"},             CHAR_WORKDBATTLETIME}
    , {{"ʹ�õ�������"},                       CHAR_WORKUSEINDEX}
    , {{"��ʹ�õ�������"},                    CHAR_WORKTOPPINDEX}
    , {{"ս��ģʽ"},                         CHAR_WORKBATTLEFLAG}
    , {{"��ս��ʶ"},                         CHAR_WORKFMPKFLAG}
    , {{"�ҳ�"},                       CHAR_WORKFIXAI}
#ifdef _NPC_EVENT_NOTICE
    , {{"NOTICE"},               CHAR_WORK_NOTICE}
#endif
    , {{"������"},            CHAR_WORKMODPOISON}
    , {{"�鿹��"},            CHAR_WORKMODPARALYSIS}
    , {{"˯����"},            CHAR_WORKMODSLEEP}
    , {{"ʯ����"},            CHAR_WORKMODSTONE}
    , {{"�ƿ���"},            CHAR_WORKMODDRUNK}
    , {{"�쿹��"},            CHAR_WORKMODCONFUSION}
    , {{"�鿹��"},            CHAR_WORKMODWEAKEN}
    , {{"�翹��"},            CHAR_WORKMODDEEPPOISON}
    , {{"�Ͽ���"},            CHAR_WORKMODBARRIER}
    , {{"Ĭ����"},            CHAR_WORKMODNOCAST}
#ifdef _LUA_RESIST
    , {{"��������"},         CHAR_WORK_RESIST_POISON}
    , {{"�鿹����"},         CHAR_WORK_RESIST_PARALYSIS}
    , {{"˯������"},         CHAR_WORK_RESIST_SLEEP}
    , {{"ʯ������"},         CHAR_WORK_RESIST_STONE}
    , {{"�ƿ�����"},         CHAR_WORK_RESIST_DRUNK}
    , {{"�쿹����"},         CHAR_WORK_RESIST_CONFUSION}
    , {{"�鿹����"},         CHAR_WORK_RESIST_WEAKEN}
    , {{"�翹����"},         CHAR_WORK_RESIST_DEEPPOISON}
    , {{"�Ͽ�����"},         CHAR_WORK_RESIST_BARRIER}
    , {{"Ĭ������"},         CHAR_WORK_RESIST_NOCAST}
#endif
#ifdef  _FMBATTLE_POINT
    , {{"��ս���ֱ�ʶ"},   CHAR_WORKFMBATTLETYPE}
#endif
#ifdef  _NEW_TEACHER_SYSTEM
    , {{"��ʦͽ��ʱ����"},    CHAR_WORK_NEWTEACHER_INDEX}
#endif
    , {{"��ս��־"},                     CHAR_WORKFMPKFLAG}
    , { { "��սս��" }, CHAR_WORKBATTLEFLAG }
    , { { "���ͱ�ʶ" }, CHAR_WORKWARPCHECK }
    , {{"������ʱ����"},   CHAR_WORKFMINDEXI}
    , {{"��λ��ʱ"}, CHAR_WORK_DWTEMP}
    , {{"������������"},   CHAR_WORKPLAYERINDEX}
#ifdef _IPHONEADDJIKOU_
    , {{"�ֻ����ϱ�־"},                   CHAR_WORK_IPHONE_INFO}
#endif
#ifdef _IPHONE_SHOP_FLAG_
    , {{"ƻ���̳Ǳ�־"},                   CHAR_WORK_IPHONE_FLAG}
#endif
#ifdef _SPEED_CHECK_
    , { { "���ټƴ�" }, CHAR_WORK_CHECK_SPEED_NUM }
#endif
#ifdef FAMILY_MANOR_
    , { { "ׯ԰��Ч" }, CHAR_PLAYER_EFFECT_MANOR }
#endif
#ifdef _PLAYER_EFFECT
    , { { "����Ч��" }, CHAR_PLAYER_EFFECT }
#endif
    , { { "�̵����" }, CHAR_WORKSHOPRELEVANT }
    , { { "ս����Ч" }, CHAR_WORKBATTLEEFFECTS }
    , { { "��ȫģʽ" }, CHAR_WORKSAFEMODE }
#ifdef _YAYA_VOICE
    , { { "����ģʽ" }, CHAR_WORKYAYAVOICE }
#endif
    , { { "�����˺�" }, CHAR_WORKDAMAGE }
    , { { "���Ӷѵ���" }, CHAR_WORKATTACHPILE }
    , { { "����ʱ��" }, CHAR_WORKECHOTIME }
    , { { "��������" }, CHAR_WORKECHONUM }
    , { { "��ʱ�غ���" }, CHAR_WORKBATTLEWAITCNT }
    , { { "����״̬" }, CHAR_WORKTRADEMODE }
    , { { "�Զ����" }, CHAR_WORKAUTOPARTY }
#ifdef _PLAYER_QUESTION
    , { { "��Ҵ���" }, CHAR_WORKPLAYERQUESTION }
#endif
    , { { "Ƥ������" }, CHAR_WORKSKININDEX }
    , { { "Ƥ��ʱ��" }, CHAR_WORKSKINTIME }
    , { { "ѱ��ģʽ" }, CHAR_PickAllPet }
    , { { "�����" }, CHAR_WORK_RELIFE }
    , { { "ս����" }, CHAR_WORKBATTLESIDE }
    , { { "ս��״̬��ʶ" }, CHAR_WORKBATTLEFLG }
    , { { "��֤ʱ��" }, CHAR_WORKRIDETIME }
    , { { "EVENT����" }, CHAR_WORKEVENTTYPE }
    , { { "����ʱ��" }, CHAR_WORKKICKTIME }
#ifdef _SMART_ENEMY_LUA
    , { {"�񱩱��"},           CHAR_WORK_CRAZYFLG }
    , { {"�񱩼���"},           CHAR_WORK_CRAZYCNT }
    , { {"�񱩻غ�"},           CHAR_WORK_CRAZYROUND }
#endif
#ifdef _TRUE_DAMAGE_MAGIC
    , { {"��ʵ�˺�"},           CHAR_WORK_DAMGEFLG }
#endif
    , { {"ս���ӿ�1"},          CHAR_WORKBATTLECOM1 }
    , { {"ս���ӿ�2"},          CHAR_WORKBATTLECOM2 }
    , { {"ս���ӿ�3"},          CHAR_WORKBATTLECOM3 }
    , { {"ս���ӿ�4"},          CHAR_WORKBATTLECOM4 }
#ifdef _PROFESSION_SKILL            // WON ADD ����ְҵ����
    , { {"����"},          CHAR_WORK_F_RESIST }
    , { {"������"},          CHAR_WORK_I_RESIST }
    , { {"�翹��"},          CHAR_WORK_T_RESIST }
#endif
    , { {"�峤��λ����"},          CHAR_WORKLEADERCHANGE }
#ifdef _VARY_WOLF_JOINBATTLE
    , { {"������ʱ����"},          CHAR_WORKPETDATA }
#endif
};

static CharBase CharBaseWorkChar[] =
{
    {{"NPC��ʱ1"},                CHAR_NPCWORKCHAR1}
    , {{"NPC��ʱ2"},             CHAR_NPCWORKCHAR2}
    , {{"NPC��ʱ3"},             CHAR_NPCWORKCHAR3}
    , {{"NPC��ʱ4"},             CHAR_NPCWORKCHAR4}
    , {{"NPC��ʱ5"},             CHAR_NPCWORKCHAR5}
    , {{"NPC��ʱ6"},             CHAR_NPCWORKCHAR6}
#ifdef _STREET_VENDOR
    , {{"��̯"},                       CHAR_STREETVENDOR_NAME}
#endif
#ifdef _NEWCLISETMAC
    , {{"MAC"},                      CHAR_WORK_MAC}
    , {{"MAC2"},                         CHAR_WORK_MAC2}
    , {{"MAC3"},                         CHAR_WORK_MAC3}
#endif
#ifdef _ITEM_MMEXP
    , { { "MM����" }, CHAR_WORK_MMEXPITEM }
#endif
};

static CharBase CharBaseValue[] =
{
    {{"��"},                   0}
    , {{"����"},           1}
    , {{"��"},              2}
    , {{"����"},           3}
    , {{"��"},              4}
    , {{"����"},           5}
    , {{"��"},              6}
    , {{"����"},           7}
    , {{"������"},        CHAR_TYPENONE}
    , {{"���"},           CHAR_TYPEPLAYER}
    , {{"����"},           CHAR_TYPEENEMY}
    , {{"����"},           CHAR_TYPEPET}
#ifdef _ALLBLUES_LUA
    , {{"LUA"},          CHAR_TYPELUANPC}
#endif
#ifdef _PLAYER_NPC
    , {{"����"},           CHAR_TYPEPLAYERNPC}
    , {{"���"},           CHAR_TYPEPLAYERPETNPC}
#endif
};

static CharBase CharBaseWorkValue[] =
{
    {{"��"},                                   CHAR_PARTY_NONE}
    , {{"�ӳ�"},                           CHAR_PARTY_LEADER}
    , {{"��Ա"},                           CHAR_PARTY_CLIENT}
    , {{"��ս��"},                        BATTLE_CHARMODE_NONE}
    , {{"ս����ʼ��"},              BATTLE_CHARMODE_INIT}
    , {{"�ȴ�ս��"},                     BATTLE_CHARMODE_C_WAIT}
    , {{"ȷ��ս��"},                     BATTLE_CHARMODE_C_OK}
    , {{"����ս��"},                     BATTLE_CHARMODE_RESCUE}
    , {{"����ս��"},                     BATTLE_CHARMODE_FINAL}
    , {{"��ս��ʼ��"},              BATTLE_CHARMODE_WATCHINIT}
    , {{"û�м���"},                     FMMEMBER_NONE}
    , {{"��������"},                   FMMEMBER_APPLY}
    , {{"�峤"},                             FMMEMBER_LEADER}
    , {{"��Ա"},                             FMMEMBER_MEMBER}
    , {{"����"},                             FMMEMBER_ELDER}
};

static CharBase CharBaseInt[] =
{
    {{"ͼ���"},             CHAR_BASEIMAGENUMBER}
    , {{"ԭͼ���"},     CHAR_BASEBASEIMAGENUMBER}
    , {{"ͷ���"},    CHAR_FACEIMAGENUMBER}
    , {{"�浵"},               CHAR_SAVEINDEXNUMBER}
    , {{"��ͼ��"},            CHAR_FLOOR}
    , {{"����X"},          CHAR_X}
    , {{"����Y"},          CHAR_Y}
    , {{"����"},           CHAR_DIR}
    , {{"�ȼ�"},           CHAR_LV}
    , {{"ʯ��"},           CHAR_GOLD}
    , {{"HP"},               CHAR_HP}
    , {{"MP"},               CHAR_MP}
    , {{"���MP"},         CHAR_MAXMP}
    , {{"����"},           CHAR_VITAL}
    , {{"����"},           CHAR_STR}
    , {{"����"},               CHAR_TOUGH}
    , {{"�ٶ�"},               CHAR_DEX}
    , {{"��"},                  CHAR_EARTHAT}
    , {{"ˮ"},                  CHAR_WATERAT}
    , {{"��"},              CHAR_FIREAT}
    , {{"��"},              CHAR_WINDAT}
    , {{"ս��"},           CHAR_DEFAULTPET}
    , {{"����"},           CHAR_CHARM}
    , {{"����"},           CHAR_LUCK}
    , {{"��������"},     CHAR_DEADCOUNT}
    , {{"��·����"},     CHAR_WALKCOUNT}
    , {{"˵������"},     CHAR_TALKCOUNT}
    , {{"�𻵴���"},     CHAR_DAMAGECOUNT}
    , {{"�������"},     CHAR_GETPETCOUNT}
    , {{"ɱ�����"},     CHAR_KILLPETCOUNT}
    , {{"�������"},     CHAR_DEADPETCOUNT}
    , {{"���ʼ���"},     CHAR_SENDMAILCOUNT}
    , {{"�ϳɴ���"},     CHAR_MERGEITEMCOUNT}
    , {{"PK����"},         CHAR_DUELBATTLECOUNT}
    , {{"PKӮ��"},         CHAR_DUELWINCOUNT}
    , {{"PK����"},         CHAR_DUELLOSECOUNT}
    , {{"PK��ʤ"},         CHAR_DUELSTWINCOUNT}
    , {{"PK����"},         CHAR_DUELSTLOSECOUNT}
    , {{"PK�����ʤ"},   CHAR_DUELMAXSTWINCOUNT}
    , {{"����"},           CHAR_WHICHTYPE}
    , { { "ץ���ȼ�" }, CHAR_PETGETLV }
    , {{"ѭ���¼�ʱ��"},   CHAR_LOOPINTERVAL}
#ifdef _NEWOPEN_MAXEXP
    , {{"�ܾ���"},        CHAR_OLDEXP}
#endif
    , {{"��ǰ����"},     CHAR_EXP}
    , {{"���ܵ�"},        CHAR_SKILLUPPOINT}
    , {{"������"},        CHAR_LEVELUPPOINT}
    , {{"����ʱ��"},     CHAR_LASTTIMESETLUCK}
    , {{"DP"},               CHAR_DUELPOINT}
    , {{"����"},           CHAR_EXP}
    , {{"������"},        CHAR_LASTTALKELDER}
    , {{"ת��"},           CHAR_TRANSMIGRATION}
    , {{"����"},           CHAR_SILENT}
    , {{"��������"},     CHAR_FMINDEX}
    , {{"�����λ"},     CHAR_FMLEADERFLAG}
    , {{"��������"},     CHAR_FMSPRITE}
#ifdef _NEW_RIDEPETS
    , {{"֤�����"},     CHAR_LOWRIDEPETS}
#endif
#ifdef _RIDE_CF
    , {{"֤�����1"},    CHAR_LOWRIDEPETS1}
#ifdef _ADD_RIDE_CF
    , {{"֤�����2"},    CHAR_HIGHRIDEPET2}
#endif
#endif
    , {{"��������"},     CHAR_BANKGOLD}
    , {{"���"},           CHAR_RIDEPET}
    , {{"����"},           CHAR_CRITIAL}
    , {{"����"},           CHAR_COUNTER}
    , {{"����ʱ��"},     CHAR_LASTLEAVETIME}
#ifdef _GAMBLE_BANK
//  ,{{"�ĳ�����"},     CHAR_RIDEPET}
#endif
#ifdef _DROPSTAKENEW
    , {{"�ĳ�����"},     CHAR_GAMBLENUM}
#endif
#ifdef _PERSONAL_FAME   // Arminius 8.30: �����������
    , {{"����"},           CHAR_FAME}
#endif
#ifdef _ATTACK_EFFECT
    , { { "������Ч" }, CHAR_ATTACK_EFFECT }
#endif
#ifdef _ATTACK_MAGIC
    , {{"��ħ��������"},           CHAR_EARTH_EXP}
    , {{"ˮħ��������"},           CHAR_WATER_EXP}
    , {{"��ħ��������"},           CHAR_FIRE_EXP}
    , {{"��ħ��������"},           CHAR_WIND_EXP}
    , {{"��ħ������"},              CHAR_EARTH_RESIST}
    , {{"ˮħ������"},              CHAR_WATER_RESIST}
    , {{"��ħ������"},              CHAR_FIRE_RESIST}
    , {{"��ħ������"},              CHAR_WIND_RESIST}
    , {{"��ħ�������Ⱦ���"},     CHAR_EARTH_ATTMAGIC_EXP}
    , {{"ˮħ�������Ⱦ���"},     CHAR_WATER_ATTMAGIC_EXP}
    , {{"��ħ�������Ⱦ���"},     CHAR_FIRE_ATTMAGIC_EXP}
    , {{"��ħ�������Ⱦ���"},     CHAR_WIND_ATTMAGIC_EXP}
    , {{"��ħ�����Ծ���ֵ"},     CHAR_EARTH_DEFMAGIC_EXP}
    , {{"ˮħ�����Ծ���ֵ"},     CHAR_WATER_DEFMAGIC_EXP}
    , {{"��ħ�����Ծ���ֵ"},     CHAR_FIRE_DEFMAGIC_EXP}
    , {{"��ħ�����Ծ���ֵ"},     CHAR_WIND_DEFMAGIC_EXP}
#endif
#ifdef _CHAR_PROFESSION         // WON ADD ����ְҵ
    , {{"ְҵ���"},    PROFESSION_CLASS}
    , {{"ְҵ�ȼ�"},     PROFESSION_LEVEL}
    , { {"��ְ����"},     PROFESSION_CNT }
    , {{"���ܵ���"},     PROFESSION_SKILL_POINT}
    , {{"���Ӷѵ�"},     ATTACHPILE}
    , {{"��������"},     PROFESSION_FIRE_P}
    , {{"��������"},     PROFESSION_ICE_P}
    , {{"��������"},     PROFESSION_THUNDER_P}
    , {{"����"},    PROFESSION_FIRE_R}
    , {{"������"},    PROFESSION_ICE_R}
    , {{"�׿���"},    PROFESSION_THUNDER_R}
#endif
#ifdef _NEW_MANOR_LAW
    , {{"����"},       CHAR_MOMENTUM}
#endif
#ifdef _VIP_SERVER
    , {{"����"},       CHAR_AMPOINT}
#endif
#ifdef _VIP_RIDE
    , {{"��Ա"},           CHAR_VIPRIDE}
    , {{"��Ա��Ч��"}, CHAR_VIPTIME}
#endif
#ifdef _ITEM_PET_LOCKED
    , {{"��ȫ��"},        CHAR_LOCKED}
#endif
#ifdef _ONLINE_SHOP
    , {{"����"},           CHAR_BJ}
#endif
    , {{"ģʽAI"},     CHAR_MODAI}
    , {{"�ɱ�AI"},     CHAR_VARIABLEAI}
    , {{"�輼λ"},    CHAR_SLOT}
    , {{"��ID"},        CHAR_PETID}
    , {{"�ػ���"},    CHAR_PETFAMILY}
    , {{"����ֵ"},    CHAR_ALLOCPOINT}
    , {{"�ɳ�����"}, CHAR_PETRANK}
    , {{"���Ƶȼ�"}, CHAR_LIMITLEVEL}
#ifdef _PET_BEATITUDE
    , {{"����ֵ"},    CHAR_BEATITUDE}
#endif
#ifdef _CAMEO_MONEY
    , {{"����"},       CHAR_CAMEO}
#endif
#ifdef _MISSION_TRAIN
    , {{"������"},    CHAR_MISSIONTRAIN_NUM}
    , {{"������ʱ��"},  CHAR_MISSIONTRAIN_TIME}
#endif
#ifdef _PET_EVOLVE
    , {{"����"},       CHAR_EVOLVE}
#endif
#ifdef _ONLINE_TIME
    , {{"����ʱ��"},         CHAR_ONLINE_TIME}
#endif

#ifdef _FLOOR_DIFI_FIELD
    , {{"¥��ʱ��"},         CHAR_FLOORDIFI_TIME}
#endif
#ifdef _ACTIVE_GAME
    , {{"����"},       CHAR_ACTIVE}
#endif
#ifdef _SUPER
    , {{"��Ʒ"},       CHAR_SUPER}
#endif
#ifdef _ALLDOMAN
    , {{"Ӣ�۳ƺ�"},         CHAR_HEROFLOOR}
#endif
#ifdef _MISSION_TIME
    , {{"�����ʱ"},         CHAR_MISSION_TIME}
#endif
#ifdef _PET_FUSION
    , {{"�ںϳ�"},        CHAR_FUSIONBEIT}
    , {{"�ںϵ�"},        CHAR_FUSIONRAISE}
#endif
#ifdef _REGISTRATION_TIME
    , {{"ǩ��FLG"},        CHAR_REGISTRATION_FLG}
    , {{"ǩ��ʱ��"},         CHAR_REGISTRATION_TIME}
    , {{"ǩ������"},         CHAR_REGISTRATION_COUNT}
    , {{"ǩ������ʱ��"}, CHAR_REGISTRATION_ONLINE_TIME}
#endif
#ifdef _PETCOM_
    , { { "������" }, CHAR_YHP }
    , { { "������" }, CHAR_YATK }
    , { { "������" }, CHAR_YDEF }
    , { { "���ٶ�" }, CHAR_YQUICK }
    , { { "���ȼ�" }, CHAR_YLV }
#endif
#ifdef _SPECIAL_COUNTER
    , {{"������"},        CHAR_SPCOUNTER}
#endif
#ifdef _SPECIAL_TIMER
    , {{"��ʱ��"},        CHAR_SPTIMER}
#endif
#ifdef _OFFLINE_TIME
    , {{"����ʱ��"},     CHAR_OFFLINE_TIME}
#endif
#ifdef _FAME_REG_TIME
    , {{"����ǩ��ʱ��"},   CHAR_FAME_REG_TIME}
#endif
#ifdef _PET_VALIDITY
    , {{"������Ч��"},      CHAR_PETVALIDITY}
#endif
    , {{"ˢ¥����"},     CHAR_SHUALOU_POINT}
#ifdef  _NEW_MAP_NO_FAME
    , {{"������ģʽ"},      CHAR_NOFAME}
#endif
    , {{"�������ڴ���"},       CHAR_DATEWORLDCNT}
    , {{"����ʱ��"},     CHAR_WORLDTIME}
    , {{"�������ģʽ"},       CHAR_RIDENAMEFLG}
    , {{"�����"},     CHAR_HDNUM}
    , {{"�˺�����"},     CHAR_SHNUM}
    , {{"�����"},     CHAR_HDDATE}
#ifdef  _ENEMY_MIN_ADD
    , {{"���м���ʱ��"},       CHAR_ENEMY_MIN_TIME}
    , {{"���м��ʱ���"},       CHAR_ENEMY_MIN_BASE}
#endif
#ifdef  _FMBATTLE_POINT
    , {{"��ս����"},     CHAR_FMBATTLE_POINT}
#endif
#ifdef _CHAR_TITLE_STR_
    , { { "�ƺ�״̬1" }, CHAR_TITLE1 }
    , { { "�ƺ�״̬2" }, CHAR_TITLE2 }
    , { { "�ƺ�״̬3" }, CHAR_TITLE3 }
    , {{"���ֳƺ�"},     CHAR_TITLE_}
    , { { "�ƺ�����" }, CHAR_TITLE_TYPE }
#endif
#ifdef _CHAR_MANOR_
    , {{"����⻷"},     CHAR_TRUMP_EFFECT}
#endif
#ifdef _CHAR_TITLE_NEW_
    , {{"�����ֳƺ�"},      CHAR_TITLE_NEW}
#endif
#ifdef  _NEW_TEACHER_SYSTEM
    , {{"��ʦͽ����"},  CHAR_NEW_TEACHER_POINT}
#endif
    , {{"��������"}, CHAR_BUCHANG_DATE}
    , {{"��λ����"}, CHAR_DWPOINT}
    , {{"��λģʽ"}, CHAR_DWTYPE}
    , {{"��λʱ��"}, CHAR_DWTIME}
#ifdef _ADDLOGINTIME_
    , {{"��½��ʱ��"},      CHAR_LOGIN_TIME}
    , {{"��½������"},      CHAR_LOGIN_DATA}
#endif
#ifdef _PAIPAISONG_
    , {{"�������"},         CHAR_PPSNUM}
    , {{"�������ֵ"},      CHAR_PPSLUCKY}
#endif
#ifdef _NEWPAIHANG_
    , { { "132�ƺ�" }, CHAR_132TITLE_FLG }
#endif
#ifdef _PETCOM_
    , { { "Ԥ��Ѫ��" }, CHAR_YHP }
    , { { "Ԥ�⹥��" }, CHAR_YATK }
    , { { "Ԥ�����" }, CHAR_YDEF }
    , { { "Ԥ������" }, CHAR_YQUICK }
    , { { "Ԥ��ȼ�" }, CHAR_YLV }
#endif
    , { { "������ʾ��ɫ" }, CHAR_POPUPNAMECOLOR }
    , { { "����ʱ��" }, CHAR_ADDEXPTIME }
    , { { "����ӳ�" }, CHAR_ADDEXPPOWER }
#ifdef _BUY_VIGORDATA
    , { { "�����������" }, CHAR_VIGORDATA }
#endif
    , { { "���ȼ�" }, CHAR_LEARNRIDE }
#ifdef _NEWPLAYER_FLG
    , { { "��������" }, CHAR_NEWPLAYERFLG }
#endif
    , { { "ǩ��˫��ʱ��" }, CHAR_DAYITEMEXPTIME }
#ifdef _PARTY_HIGH
    , { { "�����ǿģʽ" }, CHAR_PARTYTYPE }
#endif
#ifdef _OFFLINE_EXP
    , { { "���߾���" }, CHAR_OFFLINE_EXP }
#endif // _OFFLINE_EXP

#ifdef _U8_QUESTION
    , { { "U8�ʾ����" }, CHAR_U8QUESTION }
#endif // _U8_QUESTION
    , { { "����������1" }, CHAR_ENDEVENT }
    , { { "����������2" }, CHAR_ENDEVENT2 }
    , { { "����������3" }, CHAR_ENDEVENT3 }
    , { { "����������4" }, CHAR_ENDEVENT4 }
    , { { "����������5" }, CHAR_ENDEVENT5 }
    , { { "����������6" }, CHAR_ENDEVENT6 }
    , { { "����������7" }, CHAR_ENDEVENT7 }
    , { { "����������8" }, CHAR_ENDEVENT8 }
    , { { "����������9" }, CHAR_ENDEVENT9 }
    , { { "����������10" }, CHAR_ENDEVENT10 }
    , { { "����������11" }, CHAR_ENDEVENT11 }
    , { { "����������12" }, CHAR_ENDEVENT12 }
    , { { "����������13" }, CHAR_ENDEVENT13 }
    , { { "����������14" }, CHAR_ENDEVENT14 }
    , { { "����������15" }, CHAR_ENDEVENT15 }
    , { { "����������16" }, CHAR_ENDEVENT16 }
    , { { "����������17" }, CHAR_ENDEVENT17 }
    , { { "����������18" }, CHAR_ENDEVENT18 }
    , { { "����������19" }, CHAR_ENDEVENT19 }
    , { { "����������20" }, CHAR_ENDEVENT20 }
    , { { "����������21" }, CHAR_ENDEVENT21 }
    , { { "����������22" }, CHAR_ENDEVENT22 }
    , { { "����������23" }, CHAR_ENDEVENT23 }
    , { { "����������24" }, CHAR_ENDEVENT24 }
    , { { "����������25" }, CHAR_ENDEVENT25 }
    , { { "����������26" }, CHAR_ENDEVENT26 }
    , { { "����������27" }, CHAR_ENDEVENT27 }
    , { { "����������28" }, CHAR_ENDEVENT28 }
    , { { "����������29" }, CHAR_ENDEVENT29 }
    , { { "����������30" }, CHAR_ENDEVENT30 }
    , { { "����������31" }, CHAR_ENDEVENT31 }
    , { { "����������32" }, CHAR_ENDEVENT32 }

    , { { "�����������1" }, CHAR_NOWEVENT }
    , { { "�����������2" }, CHAR_NOWEVENT2 }
    , { { "�����������3" }, CHAR_NOWEVENT3 }
    , { { "�����������4" }, CHAR_NOWEVENT4 }
    , { { "�����������5" }, CHAR_NOWEVENT5 }
    , { { "�����������6" }, CHAR_NOWEVENT6 }
    , { { "�����������7" }, CHAR_NOWEVENT7 }
    , { { "�����������8" }, CHAR_NOWEVENT8 }
    , { { "�����������9" }, CHAR_NOWEVENT9 }
    , { { "�����������10" }, CHAR_NOWEVENT10 }
    , { { "�����������11" }, CHAR_NOWEVENT11 }
    , { { "�����������12" }, CHAR_NOWEVENT12 }
    , { { "�����������13" }, CHAR_NOWEVENT13 }
    , { { "�����������14" }, CHAR_NOWEVENT14 }
    , { { "�����������15" }, CHAR_NOWEVENT15 }
    , { { "�����������16" }, CHAR_NOWEVENT16 }
    , { { "�����������17" }, CHAR_NOWEVENT17 }
    , { { "�����������18" }, CHAR_NOWEVENT18 }
    , { { "�����������19" }, CHAR_NOWEVENT19 }
    , { { "�����������20" }, CHAR_NOWEVENT20 }
    , { { "�����������21" }, CHAR_NOWEVENT21 }
    , { { "�����������22" }, CHAR_NOWEVENT22 }
    , { { "�����������23" }, CHAR_NOWEVENT23 }
    , { { "�����������24" }, CHAR_NOWEVENT24 }
    , { { "�����������25" }, CHAR_NOWEVENT25 }
    , { { "�����������26" }, CHAR_NOWEVENT26 }
    , { { "�����������27" }, CHAR_NOWEVENT27 }
    , { { "�����������28" }, CHAR_NOWEVENT28 }
    , { { "�����������29" }, CHAR_NOWEVENT29 }
    , { { "�����������30" }, CHAR_NOWEVENT30 }
    , { { "�����������31" }, CHAR_NOWEVENT31 }
    , { { "�����������32" }, CHAR_NOWEVENT32 }
    , { { "���︽������" }, CHAR_PETDATA }
    , { { "���︽������2" }, CHAR_PETDATA2 }
    , { { "���︽������3" }, CHAR_PETDATA3 }
    , { { "���︽������4" }, CHAR_PETDATA4 }
    , { { "���︽������5" }, CHAR_PETDATA5 }
#ifdef _PET_IMAGE
    , { { "����ͼ��1" }, CHAR_PETIMAGE1 }
    , { { "����ͼ��2" }, CHAR_PETIMAGE2 }
    , { { "����ͼ��3" }, CHAR_PETIMAGE3 }
    , { { "����ͼ��4" }, CHAR_PETIMAGE4 }
    , { { "����ͼ��5" }, CHAR_PETIMAGE5 }
    , { { "����ͼ��6" }, CHAR_PETIMAGE6 }
    , { { "����ͼ��7" }, CHAR_PETIMAGE7 }
    , { { "����ͼ��8" }, CHAR_PETIMAGE8 }
    , { { "����ͼ��9" }, CHAR_PETIMAGE9 }
    , { { "����ͼ��10" }, CHAR_PETIMAGE10 }
#endif
    , { { "�����" }, CHAR_SAVEPOINT }
#ifdef _479SA
    , { {"�¿�����"},       CHAR_CARD_TIME }
    , { {"��ʷ����"},       CHAR_PET_OLD_NUM }
#endif // _479SA
#ifdef _BATTLE_EFFECTS
    , { { "ս����Ч" }, CHAR_BATTLEEFFECTS }
    , { { "ս����Ч2" }, CHAR_BATTLEEFFECTS2 }
#endif // _BATTLE_EFFECTS
#ifdef _NEWPLAYER_FUNC
    , {{"����״̬"},     CHAR_NEWFLG}
    , {{"������ID"},      CHAR_MAINID}
    , {{"������ID"},      CHAR_SUBID}
#endif
#ifdef __NEW_CONSIGNMENT
    , {{"�ۼ�"},               CHAR_PRICE}
    , {{"����"},               CHAR_CONSIGN}
#endif
};

static CharBase CharBaseChar[] =
{
    {{"����"},                CHAR_NAME}
    , {{"�ǳ�"},           CHAR_OWNTITLE}
    , {{"����"},               CHAR_USERPETNAME}
    , {{"�˺�"},           CHAR_CDKEY}
    , {{"����"},           CHAR_FMNAME}
    , {{"Ψһ���"},     CHAR_UNIQUECODE}
#ifdef _TEACHER_SYSTEM
    , {{"��ʦ�ʺ�"},     CHAR_TEACHER_ID}
    , {{"��ʦ����"},     CHAR_TEACHER_NAME}
#endif
#ifdef _ITEM_SETLOVER
    , {{"�����˺�"},     CHAR_LOVERID}
    , {{"��������"},     CHAR_LOVERNAME}
#endif
#ifdef _NEW_NAME
    , {{"�ƺ�"},               CHAR_NEWNAME}
#endif
#ifdef _LOTTERY_SYSTEM
    , {{"��Ʊ"},               CHAR_LOTTERY_VALUE}
#endif
    , {{"�����˺�"},     CHAR_OWNERCDKEY}
    , {{"��������"},     CHAR_OWNERCHARANAME}
#ifdef _POWER_UP
    , {{"��������"},     CHAR_POWER_UP}
#endif
    , {{"�˺�BUFF"},       CHAR_DAMAGE_BUFF}
#ifdef _PET_LV1_4V
    , {{"������Χ"},     CHAR_PET_4V}
    , {{"����ת����Χ"},     CHAR_PETTRN_4V}
#endif
#ifdef _PAOHUAN
    , {{"�ܻ�����"},     CHAR_PAOHUAN}
#endif
#ifdef _CHAR_TITLE_NEW_
    , {{"�³ƺ�����"},  CHAR_TITLENEW_DATA}
#endif
#ifdef  _NEW_TEACHER_SYSTEM
    , {{"��ʦͽ����"},  CHAR_NEW_TEACHER_DATA}
#endif
#ifdef  _PLAYER_UID
    , {{"UID"},  CHAR_PLAYERUID}
#endif
#ifdef _NEW_EVENT_ID_UPDATE
    , { { "����С��" }, CHAR_EVENTID }
#endif // _NEW_EVENT_ID_UPDATE
#ifdef _PARTY_HIGH
    , { { "�����ǿ����" }, CHAR_PARTYOTHERNAME }
#endif
#ifdef _ACTIVE_GAME
    , { { "����ʱ��" }, CHAR_ACTIVE_TIME }
#endif
#ifdef _CAPTURE_DATA
    , { { "ץ������" }, CHAR_CAPTURE_DATA }
#endif // _CAPTURE_DATA
#ifdef _479SA
    , { { "�¿�״̬" }, CHAR_CARD_TYPE }
    , { { "��ʷ��Χ" }, CHAR_PET_OLD_4V }
    , { { "��ʷ��Χ��" }, CHAR_PET_OLD_4V_FIRST }
    , { { "��ʷף��" }, CHAR_PET_OLD_ZF }
    , { {"��Ծ��"},      CHAR_ACTIVE_MAIN }
    , { {"�1"},        CHAR_ACTIVE_ACT1 }
    , { {"�2"},        CHAR_ACTIVE_ACT2 }
    , { {"�3"},        CHAR_ACTIVE_ACT3 }
    , { {"�4"},        CHAR_ACTIVE_ACT4 }
    , { {"�5"},        CHAR_ACTIVE_ACT5 }
    , { {"�6"},        CHAR_ACTIVE_ACT6 }
    , { {"�7"},        CHAR_ACTIVE_ACT7 }
    , { {"�8"},        CHAR_ACTIVE_ACT8 }
    , { {"�9"},        CHAR_ACTIVE_ACT9 }
    , { {"�10"},       CHAR_ACTIVE_ACT10 }
    , { {"�11"},       CHAR_ACTIVE_ACT11 }
    , { {"�12"},       CHAR_ACTIVE_ACT12 }
    , { {"�13"},       CHAR_ACTIVE_ACT13 }
    , { {"�14"},       CHAR_ACTIVE_ACT14 }
    , { {"�15"},       CHAR_ACTIVE_ACT15 }
    , { {"�16"},       CHAR_ACTIVE_ACT16 }
    , { {"�¿�����"},       CHAR_CARD_TIME }
    , { {"��ʷ����"},       CHAR_PET_OLD_NUM }
#endif // _479SA
    , { {"ˢ¥����"},       CHAR_SHUALOU_DATA },
};


static CharBase CharBaseEvent[] =
{
    {{"��ʼ���¼�"},       CHAR_INITFUNC}
    //,{{"�����¼�"},       CHAR_WALKPREFUNC}
    //,{{"���ߺ��¼�"},        CHAR_WALKPOSTFUNC}
    //,{{"δ֪�¼�"},       CHAR_PREOVERFUNC}
    //,{{"δ֪�¼�"},       CHAR_POSTOVERFUNC}
    //,{{"�ۿ��¼�"},       CHAR_WATCHFUNC}
    , {{"ѭ���¼�"},         CHAR_LOOPFUNC}
    //,{{"�����¼�"},       CHAR_DYINGFUNC}
    , {{"�Ի��¼�"},         CHAR_TALKEDFUNC}
    //,{{"�����¼�"},           CHAR_PREATTACKEDFUNC}
    //,{{"�������¼�"},    CHAR_POSTATTACKEDFUNC}
    //,{{"�뿪�¼�"},       CHAR_OFFFUNC}
    //,{{"���¼�"},          CHAR_LOOKEDFUNC}
    //,{{"�����¼�"},           CHAR_ITEMPUTFUNC}
    //,{{"����Ի��¼�"}, CHAR_SPECIALTALKEDFUNC}
    , {{"�����¼�"},         CHAR_WINDOWTALKEDFUNC}
#ifdef _USER_CHARLOOPS
    //,{{"ѭ���¼�1"},      CHAR_LOOPFUNCTEMP1}
    //,{{"ѭ���¼�2"},      CHAR_LOOPFUNCTEMP2}
    //,{{"δ֪�¼�"},       CHAR_BATTLEPROPERTY}
#endif
    , {{"�ص��¼�"},         CHAR_OVERLAPEDFUNC}
    , {{"ս���¼�"},         CHAR_BATTLEOVERDFUNC}
#ifdef _ALLBLUES_LUA_1_4
    , {{"�ǳ��¼�"},         CHAR_LOGINOUTFUNC}
#endif
#ifdef _ALLBLUES_LUA_1_9
    , {{"ս�������¼�"}, CHAR_BATTLESETFUNC}
#endif
};

//static CharBase CharBaseColor[] = {
//  {{"��ɫ"},            CHAR_COLORWHITE}
//  ,{{"��ɫ"},       CHAR_COLORCYAN}
//  ,{{"��ɫ"},           CHAR_COLORPURPLE}
//  ,{{"��ɫ"},       CHAR_COLORBLUE}
//  ,{{"��ɫ"},       CHAR_COLORYELLOW}
//  ,{{"��ɫ"},       CHAR_COLORGREEN}
//  ,{{"��ɫ"},       CHAR_COLORRED}
//  ,{{"�Ұ�ɫ"},    CHAR_COLORGRAY}
//  ,{{"����ɫ"},    CHAR_COLORBLUE2}
//  ,{{"����ɫ"},    CHAR_COLORGREEN2}
//
//
//#ifdef _NEW_FONT // <<<<<<-------
//  ,{{"��ɫ"},     CHAR_COLORVIOLET}
//  ,{{"��ɫ"},     CHAR_COLORORANGE}
//  ,{{"���Sɫ"},    CHAR_COLORYELLOW2}
//  ,{{"��ɫ"},     CHAR_COLORGOLDEN}
//  ,{{"����ɫ"},    CHAR_COLORORANGE2}
//#endif
//  ,{{"��ɫ"},   CHAR_COLORBLACK}
//  ,{{"���ɫ"},   CHAR_COLORNUM}
//
//};

static CharBase CharBaseColor[] =
{
    {{"��ɫ"},            CHAR_COLORWHITE}
    , {{"��ɫ"},       CHAR_COLORCYAN}
    , {{"��ɫ"},           CHAR_COLORPURPLE}
    , {{"��ɫ"},       CHAR_COLORBLUE}
    , {{"��ɫ"},       CHAR_COLORYELLOW}
    , {{"��ɫ"},       CHAR_COLORGREEN}
    , {{"��ɫ"},       CHAR_COLORRED}
    , {{"�Ұ�ɫ"},   CHAR_COLORGRAY}
    , {{"����ɫ"},   CHAR_COLORBLUE2}
    , {{"����ɫ"},   CHAR_COLORGREEN2}

#ifdef _NEW_FONT // <<<<<<------- 
    , {{"��ɫ"},     CHAR_COLORVIOLET}
    , {{"��ɫ"},     CHAR_COLORORANGE}
    , {{"���Sɫ"},    CHAR_COLORYELLOW2}
    , {{"��ɫ"},     CHAR_COLORGOLDEN}
    , {{"����ɫ"},    CHAR_COLORORANGE2}
#endif
    , {{"��ɫ"},   CHAR_COLORBLACK}
    , {{"���ɫ"},   CHAR_COLORNUM}
};

static CharBase CharBaseUpdata[] =
{
    {{"HP"},                        CHAR_P_STRING_HP}
    , {{"MAXHP"},                CHAR_P_STRING_MAXHP}
    , {{"MP"},                       CHAR_P_STRING_MP}
    , {{"MAXMP"},                CHAR_P_STRING_MAXMP}
    , {{"����"},                   CHAR_P_STRING_VITAL}
    , {{"����"},                   CHAR_P_STRING_STR}
    , {{"����"},                   CHAR_P_STRING_TOUGH}
    , {{"�ٶ�"},                   CHAR_P_STRING_DEX}
    , {{"����"},                   CHAR_P_STRING_EXP}
    , {{"��һ������"},      CHAR_P_STRING_NEXTEXP}
    , {{"�ȼ�"},                   CHAR_P_STRING_LV}
    , {{"������"},                CHAR_P_STRING_ATK}
    , {{"������"},                CHAR_P_STRING_DEF}
    , {{"������"},                CHAR_P_STRING_QUICK}
    , {{"����"},                   CHAR_P_STRING_CHARM}
    , {{"����"},                   CHAR_P_STRING_LUCK}
    , {{"��"},                      CHAR_P_STRING_EARTH}
    , {{"ˮ"},                      CHAR_P_STRING_WATER}
    , {{"��"},                      CHAR_P_STRING_FIRE}
    , {{"��"},                      CHAR_P_STRING_WIND}
    , {{"ʯ��"},                   CHAR_P_STRING_GOLD}
    //,{{"����"},                 CHAR_P_STRING_TITLE}
    , {{"DP"},                       CHAR_P_STRING_DUELPOINT}
    , {{"ת��"},                   CHAR_P_STRING_TRANSMIGRATION}
    , {{"����"},                   CHAR_P_STRING_NAME}
    , {{"�ǳ�"},                   CHAR_P_STRING_OWNTITLE}
    , {{"���"},                   CHAR_P_STRING_RIDEPET}
    , {{"ѧ��"},                   CHAR_P_STRING_LEARNRIDE}
    , {{"ͼ��"},                   CHAR_P_STRING_BASEBASEIMAGENUMBER}
    , {{"������"},                CHAR_P_STRING_SKYWALKER}
    , {{"����ģʽ"},         CHAR_P_STRING_DEBUGMODE}
};

static CharBase CharBaseFlg[] =
{
    {{"�ɼ�"},                        CHAR_ISVISIBLE}
    , {{"͸��"},               CHAR_ISTRANSPARENT}
    , {{"����"},             CHAR_ISFLYING}
    , {{"����"},             CHAR_ISDIE}
    , {{"���"},             CHAR_ISPARTY}
    , {{"����"},             CHAR_ISDUEL}
    , {{"��Ƭ"},             CHAR_ISTRADECARD}
    , {{"����"},             CHAR_ISTRADE}
#ifdef _CHANNEL_MODIFY
    , {{"����Ƶ��"},       CHAR_ISTELL}
    , {{"����Ƶ��"},       CHAR_ISFM}
    , {{"ְҵƵ��"},       CHAR_ISOCC}
    , {{"�Ի�����"},       CHAR_ISSAVE}
    , {{"������"},          CHAR_ISCHAT}
#ifdef _CHATROOMPROTOCOL
#ifdef _THE_WORLD_SEND
    , {{"����Ƶ��"},       CHAR_ISWORLD}
#endif
#endif
#ifdef _NEW_FS_TYPE
    , { { "��Ч" }, CHAR_ISEFFECT }
    , { { "����" }, CHAR_ISPLAYER }
    , { { "������Ա" }, CHAR_ISTEAMFULL }
    , { { "�鿴����" }, CHAR_ISSHOWPET }
    , { { "�Զ����" }, CHAR_ISAUTOPARTY }
    , { { "����Ƶ��" }, CHAR_ISALLWORLD }
#endif
#endif

};

static CharBase CharBaseAction[] =
{
    {{"վ��"},                    CHAR_ACTSTAND}
    , {{"�߶�"},                   CHAR_ACTWALK}
    , {{"����"},                   CHAR_ACTATTACK}
    , {{"Ͷ��"},                   CHAR_ACTTHROW}
    , {{"����"},                   CHAR_ACTDAMAGE}
    , {{"����"},                   CHAR_ACTDEAD}
    , {{"ħ��"},                   CHAR_ACTMAGIC}
    , {{"����"},                   CHAR_ACTITEM}
    , {{"Ч��"},                   CHAR_ACTEFFECT}
    , {{"��"},                      CHAR_ACTDOWN}
    , {{"��"},                      CHAR_ACTSIT}
    , {{"����"},                   CHAR_ACTHAND}
    , {{"����"},                   CHAR_ACTPLEASURE}
    , {{"��ŭ"},                   CHAR_ACTANGRY}
    , {{"����"},                   CHAR_ACTSAD}
    , {{"����"},                   CHAR_ACTGUARD}
    , {{"��·"},                   CHAR_ACTACTIONWALK}
    , {{"��ͷ"},                   CHAR_ACTNOD}
    , {{"վ��"},                   CHAR_ACTACTIONSTAND}
    , {{"ս��"},                   CHAR_ACTBATTLE}
    , {{"����"},                   CHAR_ACTLEADER}
    , {{"��ս"},                   CHAR_ACTBATTLEWATCH}
    , {{"δ֪"},                   CHAR_ACTPOPUPNAME}
    , {{"��ת"},                   CHAR_ACTTURN}
    , {{"����"},                   CHAR_ACTWARP}
    , {{"����"},                   CHAR_ACTTRADE}
#ifdef _ANGEL_SUMMON
    , {{"��ʹ"},                   CHAR_ACTANGEL}
#endif

#ifdef _MIND_ICON
    , {{"��˼"},                   CHAR_MIND}
#endif
#ifdef _STREET_VENDOR
    , {{"��̯̲"},         CHAR_STREETVENDOR_OPEN}
    , {{"�ر�̯̲"},         CHAR_STREETVENDOR_CLOSE}
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
#ifdef _add_item_log_name  // WON ADD ��item��log������item����
                itemindex,
#else
                atoi(message),
#endif
                "AddItem(��������LUA)",
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
        "����LUA��char.AddPet",
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
        "����LUA��char.AddPet",
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
        "����LUA��char.AddPetCf",
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
        "����LUA��char.AddPetCf",
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
    //print("������%d ������%d ������%d  �ٶȣ�%d\n", CHAR_getInt(index, CHAR_VITAL), CHAR_getInt(index, CHAR_STR), CHAR_getInt(index, CHAR_TOUGH), CHAR_getInt(index, CHAR_DEX));
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
        CHAR_talkToCli(charaindex, -1, "��Ա�޷�ʹ�á�", CHAR_COLORYELLOW);
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

            LogPet(CHAR_getChar(charaindex, CHAR_NAME),    // ƽ�ҷ�
                   CHAR_getChar(charaindex, CHAR_CDKEY),
                   CHAR_getChar(petindex, CHAR_NAME),
                   CHAR_getInt(petindex, CHAR_LV),
                   "EvnetDell(LUA����ɾ��)",
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
                   sizeof(ch->charfunctable[CHAR_LOOPFUNCTEMP1]), "CHAR_BattleStayLoop"); //ս��
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

#ifdef _PROFESSION_SKILL            // WON ADD ����ְҵ����
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
    p = localtime(&timep); /*ȡ�õ���ʱ��*/

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
#ifdef _PROFESSION_SKILL            // WON ADD ����ְҵ����
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
