#include "version.h"
#include <stdio.h>
#include <string.h>
#include "common.h"
#include "buf.h"
#include "configfile.h"
#include "char_base.h"
#include "char_data.h"
#include "pet_skill.h"
#include "battle.h"
#include "battle_event.h"
#include "battle_magic.h"
#include "item.h"
#include "item_gen.h"
#include "char_talk.h"
#include "autil.h"
#include "mylua/function.h"

/*========================================================================
 * 矢永玄及  卞勾中化及末□旦 宠物的各式技能
 *========================================================================*/

static Petskill *PETSKILL_petskill;
static int      PETSKILL_petskillnum;

typedef struct tagPetskill_PetskillFunctionTable
{
    char            *functionname;      /*  涩烂白央奶伙卞踏仁楮醒及  蟆 */
    PETSKILL_CALLFUNC   func;               /*    端卞裟太请今木月楮醒 */
    int             hash;               /*  hash */
    int             status;             /*  仇及戊穴件玉毛矢永玄互隙烂仄凶执 */
} PETSKILL_PetskillFunctionTable;

/*   毛  支仄凶日仇仇卞瓒  允月仪 */
static PETSKILL_PetskillFunctionTable PETSKILL_functbl[] =
{
    { "PETSKILL_None",              PETSKILL_None,      0 },
    { "PETSKILL_NormalAttack",      PETSKILL_NormalAttack,  0 },
    { "PETSKILL_NormalGuard",       PETSKILL_NormalGuard,   0 },
    { "PETSKILL_ContinuationAttack", PETSKILL_ContinuationAttack,    0 },
    { "PETSKILL_ChargeAttack",      PETSKILL_ChargeAttack,          0 },
    { "PETSKILL_Guardian",          PETSKILL_Guardian,      0 },
    { "PETSKILL_PowerBalance",      PETSKILL_PowerBalance,      0 },
    { "PETSKILL_Mighty",            PETSKILL_Mighty,        0 },
    { "PETSKILL_StatusChange",      PETSKILL_StatusChange,  0 },
    { "PETSKILL_EarthRound",        PETSKILL_EarthRound,    0 },
    { "PETSKILL_GuardBreak",        PETSKILL_GuardBreak,    0 },
#ifdef _SKILL_GUARDBREAK2//破除防御2 vincent add 2002/05/20
    { "PETSKILL_GuardBreak2",       PETSKILL_GuardBreak2,   0 },
#endif
    { "PETSKILL_Abduct",            PETSKILL_Abduct,        0 },
    { "PETSKILL_Steal",             PETSKILL_Steal,         0 },
    { "PETSKILL_Merge",             PETSKILL_Merge,         0 },
#ifdef _ALCHEMIST
    { "PETSKILL_Alchemist",         PETSKILL_Merge,         0 },
#endif
    { "PETSKILL_NoGuard",           PETSKILL_NoGuard,       0 },
#ifdef _ITEM_INSLAY
    { "PETSKILL_Inslay",            PETSKILL_Inslay,        0 },
#endif
#ifdef _PETSKILL_FIXITEM
    { "PETSKILL_Fixitem",           PETSKILL_Fixitem,       0 },
#endif

#ifdef _MAGIC_SUPERWALL
    { "PETSKILL_MagicStatusChange", PETSKILL_MagicStatusChange, 0},
#endif

// Terry add 2001/11/05
#ifdef _ATTACK_MAGIC
    { "PETSKILL_AttackMagic",   PETSKILL_AttackMagic, 0},
#endif

#ifdef _PSKILL_FALLGROUND
    { "PETSKILL_FallGround",    PETSKILL_FallGround, 0},
#endif
#ifdef _PETSKILL_EXPLODE
    { "PETSKILL_Explode",   PETSKILL_Explode, 0},
#endif
#ifdef _BATTLESTEAL_FIX
    { "PETSKILL_StealMoney",    PETSKILL_StealMoney,    0 },
#endif
#ifdef _PRO_BATTLEENEMYSKILL
    { "ENEMYSKILL_ReLife",  ENEMYSKILL_ReLife, 0},
    { "ENEMYSKILL_ReHP",    ENEMYSKILL_ReHP, 0},
    { "ENEMYSKILL_EnemyHelp",   ENEMYSKILL_EnemyHelp, 0},
#endif
#ifdef _SKILL_DAMAGETOHP
    { "PETSKILL_DamageToHp",    PETSKILL_DamageToHp, 0},
#endif

#ifdef _PETSKILL_TIMID
    { "PETSKILL_BattleTimid",   PETSKILL_BattleTimid, 0},
#endif
#ifdef _PETSKILL_2TIMID
    { "PETSKILL_2BattleTimid",  PETSKILL_2BattleTimid, 0},
#endif
#ifdef _PETSKILL_ANTINTER
    { "PETSKILL_AntInter",  PETSKILL_AntInter, 0},
#endif

#ifdef _PETSKILL_PROPERTY
    { "PETSKILL_BattleProperty",    PETSKILL_BattleProperty, 0},
#endif

#ifdef _PETSKILL_TEAR
    { "PETSKILL_BattleTearDamage",  PETSKILL_BattleTearDamage, 0},
#endif

#ifdef _BATTLE_LIGHTTAKE
    { "PETSKILL_Lighttakeed",   PETSKILL_Lighttakeed, 0},
#endif

#ifdef _BATTLE_ATTCRAZED
    { "PETSKILL_AttackCrazed",  PETSKILL_AttackCrazed, 0},
#endif

#ifdef _SHOOTCHESTNUT   // Syu ADD 宠技：丢栗子
    { "PETSKILL_AttackShoot",   PETSKILL_AttackShoot, 0},
#endif

#ifdef _Skill_MPDAMAGE
    { "PETSKILL_MpDamage",  PETSKILL_MpDamage, 0},
#endif

#ifdef _SKILL_TOOTH
    { "PETSKILL_ToothCrushe",   PETSKILL_ToothCrushe, 0},
#endif

#ifdef _PETSKILL_SETDUCK
    { "PETSKILL_SetDuck",   PETSKILL_SetDuck, 0},
#endif
#ifdef _MAGICPET_SKILL
    { "PETSKILL_SetMagicPet",   PETSKILL_SetMagicPet, 0},
#endif
#ifdef _PSKILL_MODIFY
    { "PETSKILL_Modifyattack",  PETSKILL_Modifyattack, 0},
#endif

#ifdef _PSKILL_MDFYATTACK
    { "PETSKILL_Mdfyattack",    PETSKILL_Mdfyattack, 0},
#endif
#ifdef _VARY_WOLF
    { "PETSKILL_Vary", PETSKILL_Vary, 0},
#endif
#ifdef _SKILL_WILDVIOLENT_ATT
    { "PETSKILL_WildViolentAttack", PETSKILL_WildViolentAttack, 0},//vincent add 2002/05/16
#endif

#ifdef _SKILL_SPEEDY_ATT
    { "PETSKILL_SpeedyAttack",  PETSKILL_SpeedyAttack, 0},//vincent add 2002/05/20
#endif
#ifdef _SKILL_SACRIFICE
    { "PETSKILL_Sacrifice", PETSKILL_Sacrifice, 0},//vincent add 2002/05/30
#endif
#ifdef _SKILL_REFRESH
    { "PETSKILL_Refresh",   PETSKILL_Refresh, 0},//vincent add 2002/08/08
#endif
#ifdef _SKILL_WEAKEN  //vincent宠技:虚弱
    { "PETSKILL_Weaken",    PETSKILL_Weaken, 0},//vincent add 2002/07/11
#endif
#ifdef _SKILL_DEEPPOISON  //vincent宠技:剧毒 
    { "PETSKILL_Deeppoison",    PETSKILL_Deeppoison, 0},//vincent add 2002/07/16
#endif
#ifdef _SKILL_BARRIER  //vincent宠技:魔障 
    { "PETSKILL_Barrier",   PETSKILL_Barrier, 0},//vincent add 2002/07/16
#endif
#ifdef _SKILL_NOCAST  //vincent宠技:沉默 
    { "PETSKILL_Nocast",    PETSKILL_Nocast, 0},//vincent add 2002/07/16
#endif
#ifdef _SKILL_ROAR //vincent宠技:大吼
    { "PETSKILL_Roar",  PETSKILL_Roar, 0},//vincent add 2002/07/11
#endif

#ifdef _PET_SKILL_SARS              // WON ADD 毒煞蔓延
    { "PETSKILL_Sars",  PETSKILL_Sars, 0},
#endif

#ifdef _SONIC_ATTACK                // WON ADD 音波攻击
    { "PETSKILL_Sonic", PETSKILL_Sonic, 0},
#endif
#ifdef _PETSKILL_REGRET
    { "PETSKILL_Regret",    PETSKILL_Regret, 0},
#endif

#ifdef _PETSKILL_GYRATE
    { "PETSKILL_Gyrate", PETSKILL_Gyrate, 0},
#endif

#ifdef _PETSKILL_ACUPUNCTURE //针刺外皮
    { "PETSKILL_Acupuncture", PETSKILL_Acupuncture, 0},
#endif

#ifdef _PETSKILL_RETRACE
    { "PETSKILL_Retrace", PETSKILL_Retrace, 0},
#endif

#ifdef _PETSKILL_HECTOR
    { "PETSKILL_Hector", PETSKILL_Hector, 0},
#endif

#ifdef _PETSKILL_FIREKILL
    { "PETSKILL_Firekill", PETSKILL_Firekill, 0},
#endif

#ifdef _PETSKILL_DAMAGETOHP
    { "PETSKILL_DamageToHp2",   PETSKILL_DamageToHp2, 0},
#endif

#ifdef _PETSKILL_BECOMEFOX
    { "PETSKILL_BecomeFox", PETSKILL_BecomeFox, 0},
#endif

#ifdef _PETSKILL_BECOMEPIG
    { "PETSKILL_BecomePig", PETSKILL_BecomePig, 0},
#endif

#ifdef _PETSKILL_SHOWMERCY
    { "PETSKILL_ShowMercy", PETSKILL_ShowMercy, 0},
#endif

#ifdef _PETSKILL_COMBINED
    { "PETSKILL_Combined",  PETSKILL_Combined, 0},
    { "PETSKILL_Combined2", PETSKILL_Combined2, 0},
#endif
#ifdef _PETSKILL_LER
    { "PETSKILL_BatFly",    PETSKILL_BatFly, 0},
    { "PETSKILL_DivideAttack",  PETSKILL_DivideAttack, 0},
#endif
#ifdef _PETSKILL_BATTLE_MODEL
    { "PETSKILL_BattleModel",   PETSKILL_BattleModel, 0},
#endif

#ifdef _MASSAGE_PETSKILL
    { "PETSKILL_Massage",   PETSKILL_Massage, 0},
#endif

#ifdef _STRENGTH_PETSKILL
    { "PETSKILL_Strength",  PETSKILL_Strength, 0},
#endif

#ifdef _RESURRECTION_PETSKILL
    { "PETSKILL_Resurrection",  PETSKILL_Resurrection, 0},
#endif
#ifdef _LOSTLOST_PETSKILL
    { "PETSKILL_Lostlost",  PETSKILL_Lostlost, 0},
#endif
#ifdef _GRAPPLING_PETSKILL
    { "PETSKILL_Grappling", PETSKILL_Grappling, 0},
#endif

#ifdef _PETOUT_PETSKILL
    { "PETSKILL_Petout",    PETSKILL_Petout, 0},
#endif
#ifdef _INVERSION_PETSKILL
    { "PETSKILL_Inversion", PETSKILL_Inversion, 0},
#endif
#ifdef _PETSKILL_RIDE
    { "PETSKILL_Ride",  PETSKILL_Ride, 0},
#endif
#ifdef _PETSKILL_ISULTIMATE
    { "PETSKILL_Isultimate", PETSKILL_Isultimate, 0},
#endif
#ifdef _PETSKILL_TEMPTATION
    { "PETSKILL_Temptation",    PETSKILL_temptation, 0 },
#endif

};

/*----------------------------------------------------------------------*/


/* 湘  民尼永弁］失弁本旦楮溢 */
/*----------------------------------------------------------------------*/
BOOL PETSKILL_CHECKINDEX(int index)
{
    if (PETSKILL_petskillnum <= index || index < 0)
    {
        return FALSE;
    }

    return TRUE;
}
/*----------------------------------------------------------------------*/
static INLINE BOOL PETSKILL_CHECKINTDATAINDEX(int index)
{
    if (PETSKILL_DATAINTNUM <= index || index < 0)
    {
        return FALSE;
    }

    return TRUE;
}
/*----------------------------------------------------------------------*/
static INLINE BOOL PETSKILL_CHECKCHARDATAINDEX(int index)
{
    if (PETSKILL_DATACHARNUM <= index || index < 0)
    {
        return FALSE;
    }

    return TRUE;
}
/*----------------------------------------------------------------------*/
INLINE int PETSKILL_getInt(int index, PETSKILL_DATAINT element)
{
    return PETSKILL_petskill[index].data[element];
}
/*----------------------------------------------------------------------*/
INLINE int PETSKILL_setInt(int index, PETSKILL_DATAINT element, int data)
{
    int buf;
    buf = PETSKILL_petskill[index].data[element];
    PETSKILL_petskill[index].data[element] = data;
    return buf;
}
/*----------------------------------------------------------------------*/
INLINE char* PETSKILL_getChar(int index, PETSKILL_DATACHAR element)
{
    if (!PETSKILL_CHECKINDEX(index))
    {
        return "\0";
    }

    if (!PETSKILL_CHECKCHARDATAINDEX(element))
    {
        return "\0";
    }

    return PETSKILL_petskill[index].string[element].string;
}

/*----------------------------------------------------------------------*/
INLINE BOOL PETSKILL_setChar(int index, PETSKILL_DATACHAR element, char* new)
{
    if (!PETSKILL_CHECKINDEX(index))
    {
        return FALSE;
    }

    if (!PETSKILL_CHECKCHARDATAINDEX(element))
    {
        return FALSE;
    }

    strcpysafe(PETSKILL_petskill[index].string[element].string,
               sizeof(PETSKILL_petskill[index].string[element].string),
               new);
    return TRUE;
}
/*----------------------------------------------------------------------
 * 矢永玄  及醒毛襞月［
 *---------------------------------------------------------------------*/
int PETSKILL_getPetskillNum(void)
{
    return PETSKILL_petskillnum;
}

#ifdef _CFREE_petskill
#define PETSKILL_STARTINTNUM    7 //PETSKILL_DATACHARNUM+1
#else
#define PETSKILL_STARTINTNUM        5
#endif

/*----------------------------------------------------------------------
 * 矢永玄  及涩烂白央奶伙毛  戈//初始宠技
 *---------------------------------------------------------------------*/
BOOL PETSKILL_initPetskill(char *filename)
{
    FILE*   f;
    char    line[256];
    int     linenum = 0;
    int     petskill_readlen = 0;
    int     i, j;
    int     max_skillid = 0;
#ifdef _CRYPTO_DATA
    char realopfile[256];
    BOOL crypto = FALSE;
    sprintf(realopfile, "%s.allblues", filename);
    f = fopen(realopfile, "r");

    if (f != NULL)
    {
        crypto = TRUE;
    }
    else
#endif
    {
        f = fopen(filename, "r");
    }

    if (f == NULL)
    {
        print("file open error\n");
        return FALSE;
    }

    PETSKILL_petskillnum = 0;

    /*  引内  躲卅垫互窒垫丐月井升丹井譬屯月    *///读曲宠技总数

    while (fgets(line, sizeof(line), f))
    {
#ifdef _CRYPTO_DATA

        if (crypto == TRUE)
        {
            DecryptKey(line);
        }

#endif
        char    token[256];
        linenum ++;

        if (line[0] == '#')
        {
            continue;    /* comment */
        }

        if (line[0] == '\n')
        {
            continue;    /* none    */
        }

        chomp(line);
        PETSKILL_petskillnum++;
#ifdef _PETSKILL_OPTIMUM // Robin 取出最大宠技ID

        if (getStringFromIndexWithDelim(line, ",", PETSKILL_STARTINTNUM, token, sizeof(token)) == FALSE)
        {
            continue;
        }

        //print("\n 宠技ID:%d %s ", atoi( token), line);
        max_skillid = max(atoi(token), max_skillid);
#endif
    }

    if (fseek(f, 0, SEEK_SET) == -1)
    {
        fprint("寻找失败\n");
        fclose(f);
        return FALSE;
    }

#ifdef _PETSKILL_OPTIMUM // 宠技Table数为 最大技能ID +1
    PETSKILL_petskillnum = max_skillid + 1;
//  print("\n 最大宠技ID = %d\n", max_skillid);
#endif
    PETSKILL_petskill = allocateMemory(sizeof(struct tagPetskill)
                                       * PETSKILL_petskillnum);

    if (PETSKILL_petskill == NULL)
    {
        fprint("无法分配内存 %d\n",
               sizeof(struct tagPetskill)*PETSKILL_petskillnum);
        fclose(f);
        return FALSE;
    }

    /* 赓渝祭 */
    for (i = 0; i < PETSKILL_petskillnum; i ++)
    {
        for (j = 0; j < PETSKILL_DATAINTNUM; j ++)
        {
            PETSKILL_setInt(i, j, -1);
        }

        for (j = 0; j < PETSKILL_DATACHARNUM; j ++)
        {
            PETSKILL_setChar(i, j, "");
        }
    }

    /*  引凶  心  允    */
    linenum = 0;

    while (fgets(line, sizeof(line), f))
    {
#ifdef _CRYPTO_DATA

        if (crypto == TRUE)
        {
            DecryptKey(line);
        }

#endif
        linenum ++;

        if (line[0] == '#')
        {
            continue;    /* comment */
        }

        if (line[0] == '\n')
        {
            continue;    /* none    */
        }

        chomp(line);
        /*  垫毛帮溥允月    */
        /*  引内 tab 毛 " " 卞  五晶尹月    */
        replaceString(line, '\t', ' ');
        /* 燮  及旦矢□旦毛潸月［*/
        {
            char    buf[256];

            for (i = 0; i < strlen(line); i ++)
            {
                if (line[i] != ' ')
                {
                    break;
                }

                strcpy(buf, &line[i]);
            }

            if (i != 0)
            {
                strcpy(line, buf);
            }
        }
        {
            char    token[256];
            int     ret;
#ifdef _PETSKILL_OPTIMUM // 读取本行宠技的ID, 直接以宠技ID当Table index
            ret = getStringFromIndexWithDelim(line, ",", PETSKILL_STARTINTNUM, token, sizeof(token));

            if (ret == FALSE)
            {
                fprint("文件语法错误:%s 第%d行\n", filename, linenum);
                break;
            }

            petskill_readlen = atoi(token);
#endif

            for (i = 0; i < PETSKILL_DATACHARNUM; i ++)
            {
                /*    侬  迕玄□弁件毛苇月    */
                ret = getStringFromIndexWithDelim(line, ",", i + 1, token, sizeof(token));

                if (ret == FALSE)
                {
                    fprint("文件语法错误:%s 第%d行\n", filename, linenum);
                    break;
                }

                PETSKILL_setChar(petskill_readlen, i, token);
            }

            /* 4勾  动嫦反醒袄犯□正 */
            for (i = PETSKILL_STARTINTNUM; i < PETSKILL_DATAINTNUM + PETSKILL_STARTINTNUM; i ++)
            {
                ret = getStringFromIndexWithDelim(line, ",", i, token,
                                                  sizeof(token));

                if (ret == FALSE)
                {
                    fprint("文件语法错误:%s 第%d行\n", filename, linenum);
                    break;
                }

                if (strlen(token) != 0)
                {
                    PETSKILL_setInt(petskill_readlen, i - PETSKILL_STARTINTNUM, atoi(token));
                }
            }

            if (i < PETSKILL_DATAINTNUM + PETSKILL_STARTINTNUM)
            {
                continue;
            }

#ifdef _CFREE_petskill

            if (line[0] == 'E')
            {
                PETSKILL_setInt(petskill_readlen, PETSKILL_ILLEGAL, 1);
            }

#else

            // Robin 2001/02/26 +1 set_illegal_Skill
            if (line[0] == 'T')
            {
                PETSKILL_setInt(petskill_readlen, PETSKILL_ILLEGAL, 1);
            }

#endif
            //print("Id:%d illegal:%d\n",PETSKILL_getInt(petskill_readlen,PETSKILL_ID),PETSKILL_getInt(petskill_readlen,PETSKILL_ILLEGAL));
            petskill_readlen ++;
        }
    }

    fclose(f);
    PETSKILL_petskillnum = petskill_readlen;
    print("有效宠物技能数是 %d...", PETSKILL_petskillnum);

    /* hash 及瓒   */
    for (i = 0; i < arraysizeof(PETSKILL_functbl); i ++)
    {
        PETSKILL_functbl[i].hash = hashpjw(PETSKILL_functbl[i].functionname);
    }

    /*
    #ifdef _CFREE_petskill
        for( i=0; i <PETSKILL_petskillnum ; i++ ){
            for( j = PETSKILL_NAME; j < PETSKILL_DATACHARNUM; j ++ ){
              print( "%s ", PETSKILL_getChar( i, j));
            }
            print( "\n");
            for( j = PETSKILL_ID; j < PETSKILL_DATAINTNUM; j ++ ){
                print( "%d ", PETSKILL_getInt( i, j));
            }
            print( "\n-------------------------------------------------\n");

        }
    #endif
    */
    return TRUE;
}
/*------------------------------------------------------------------------
 * Petskill及涩烂白央奶伙  心  仄
 *-----------------------------------------------------------------------*/
BOOL PETSKILL_reinitPetskill(void)
{
    freeMemory(PETSKILL_petskill);
    return (PETSKILL_initPetskill(getPetskillfile()));
}
/*------------------------------------------------------------------------
 * PETSKILL_ID井日骄侬毛襞月楮醒
 * 忒曰袄
 * 岳  : 骄侬
 * 撩  : -1
 *-----------------------------------------------------------------------*/
int PETSKILL_getPetskillArray(int petskillid)
{
#ifdef _PETSKILL_OPTIMUM // 查询宠技ID改为阵列式

    if (petskillid >= 0 && petskillid < PETSKILL_petskillnum)
    {
        return PETSKILL_petskill[petskillid].data[PETSKILL_ID];
    }

#else
    int     i;

    for (i = 0; i < PETSKILL_petskillnum; i ++)
    {
        if (PETSKILL_petskill[i].data[PETSKILL_ID] == petskillid)
        {
            return i;
        }
    }

#endif
    return -1;
}
/*------------------------------------------------------------
 * 矢永玄  及楮醒  井日禾奶件正□毛忒允
 * 娄醒
 *  name        char*       热诸及  蟆
 * 忒曰袄
 *  楮醒尺及禾奶件正［卅中桦宁卞反NULL
 ------------------------------------------------------------*/
PETSKILL_CALLFUNC PETSKILL_getPetskillFuncPointer(char* name)
{
    int i;
    int hash = hashpjw(name);

    for (i = 0 ; i < arraysizeof(PETSKILL_functbl) ; i++)
    {
        if (PETSKILL_functbl[i].hash == hash)
        {
            if (strcmp(PETSKILL_functbl[i].functionname, name) == 0)
            {
                return PETSKILL_functbl[i].func;
            }
        }
    }

    return NULL;
}

int PETSKILL_Use(
    int charaindex,
    int havepetskill,
    int toindex,
    char *data
    //BOOL isCLI    // Robin 2001/02/26 if owner is player
)
{
    int     array, petskillid;
    int     ret;
    PETSKILL_CALLFUNC func;
    petskillid = CHAR_getPetSkill(charaindex, havepetskill);
#ifdef _FIXWOLF  // Syu ADD 修正狼人变身Bug

    if (petskillid == 600)
    {
        if (CHAR_getInt(charaindex, CHAR_BASEIMAGENUMBER) == 101428
#ifdef _EXPANSION_VARY_WOLF
                || CHAR_getInt(charaindex, CHAR_BASEIMAGENUMBER) == 104109
#endif
           )
        {
            petskillid = -1;
        }
    }

#endif
#ifdef  _RIDE_ATT_PET_SKILL

    if (CHAR_getInt(charaindex, CHAR_WHICHTYPE) == CHAR_TYPEPET)
    {
        int ownerindex = CHAR_getWorkInt(charaindex, CHAR_WORKPLAYERINDEX);
        int ridepetid = CHAR_getInt(ownerindex, CHAR_RIDEPET);
        int attpetid = CHAR_getInt(ownerindex, CHAR_DEFAULTPET);

        if (ridepetid != -1 && attpetid != -1)
        {
            if (ridepetid == attpetid && CHAR_getCharPet(ownerindex, ridepetid) == charaindex)
            {
                if (petskillid > 2 && petskillid != 61)
                {
                    petskillid = 0;
                }
            }
        }
    }

#endif
    array = PETSKILL_getPetskillArray(petskillid);

    if (array == -1)
    {
        return FALSE;
    }

#ifndef _OPEN_E_PETSKILL

    if (PETSKILL_getInt(array, PETSKILL_ILLEGAL))
    {
        if ((CHAR_getInt(charaindex, CHAR_WHICHTYPE) == CHAR_TYPEPET))
        {
            return FALSE;
        }
    }

#endif
#ifdef _PETSKILL_CHECKTYPE
#define _SKILLTYPE_NONE 0x01
#define _SKILLTYPE_BATTLE 0x02

    if ((CHAR_getInt(charaindex, CHAR_WHICHTYPE) == CHAR_TYPEPET) &&
            (PETSKILL_getInt(array, PETSKILL_USETYPE) & _SKILLTYPE_BATTLE) > 0)
    {
        int masterindex = CHAR_getWorkInt(charaindex, CHAR_WORKPLAYERINDEX);

        if (!CHAR_CHECKINDEX(masterindex))
        {
            return FALSE;
        }

        if (CHAR_getWorkInt(masterindex, CHAR_WORKBATTLEMODE) == BATTLE_CHARMODE_NONE)
        {
            return FALSE;
        }

#ifdef  _EARTHROUND_BUG
        int battleindex = CHAR_getWorkInt(masterindex, CHAR_WORKBATTLEINDEX);

        if (battleindex > -1)
        {
            int defindex = BATTLE_No2Index(battleindex, toindex);

            if (CHAR_getWorkInt(defindex, CHAR_WORKBATTLECOM1) == BATTLE_COM_S_EARTHROUND0)
            {
                return FALSE;
            }
        }

#endif
    }

#endif
    func = PETSKILL_getPetskillFuncPointer(PETSKILL_getChar(array, PETSKILL_FUNCNAME));

    if (func)
    {
#ifdef _ONE_PET_SKILL
        char *buf = PETSKILL_getChar(array, PETSKILL_NAME);

        if (strlen(buf) > 0 && buf[0] == '$')
        {
            CHAR_setPetSkill(charaindex, havepetskill, -1);
        }

#endif
        ret = func(charaindex,  toindex,  array, data);
    }
    else
    {
        ret = FALSE;
    }

    return ret;
}


int _PETSKILL_GetArray(char *file, int line, int charaindex, int havepetskill)
{
    int     petskillid, array;
    petskillid = CHAR_getPetSkill(charaindex, havepetskill);

    if (petskillid == -1)
    {
        return -1;
    }

    array = PETSKILL_getPetskillArray(petskillid);
    return array;
}


//----------------------------------------------------------------------
int PETSKILL_ContinuationAttack(
    int charaindex,
    int toindex,
    int array,
    char *data

)
{
    char *pszOption, *pszP;
    int N = 1, Per = 0;
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM1, BATTLE_COM_S_RENZOKU);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM2, toindex);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLEMODE, BATTLE_CHARMODE_C_OK);
    pszOption = PETSKILL_getChar(array, PETSKILL_OPTION);

    if (sscanf(pszOption, "%d", &N) == 1)
    {
        if (N < 1 || N > 10)
        {
            N = 1;
        }
    }
    else
    {
        N = 1;
    }

    if ((pszP = strstr(pszOption, "攻%")) != NULL)
    {
        sscanf(pszP + 3, "%d", &Per);
    }

    CHAR_SETWORKINT_LOW(charaindex, CHAR_WORKBATTLECOM3, N);
    CHAR_SETWORKINT_HIGH(charaindex, CHAR_WORKBATTLECOM3, Per);
    return TRUE;
}

int PETSKILL_ChargeAttack(
    int charaindex,
    int toindex,
    int array,
    char *data
)
{
    char *pszOption, *pszP;
    int N = 1, Per = 0;
#ifdef _MOD_PETSKILL_CHARGEATTACK
    int hit = 0;
#endif
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM1, BATTLE_COM_S_CHARGE);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM2, toindex);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLEMODE, BATTLE_CHARMODE_C_OK);
    pszOption = PETSKILL_getChar(array, PETSKILL_OPTION);

    if (sscanf(pszOption, "%d", &N) == 1)
    {
        if (N < 1 || N > 10)
        {
            N = 1;
        }
    }
    else
    {
        N = 1;
    }

    if ((pszP = strstr(pszOption, "攻%")) != NULL)
    {
        sscanf(pszP + 3, "%d", &Per);
    }

#ifdef _MOD_PETSKILL_CHARGEATTACK

    if ((pszP = strstr(pszOption, "精")) != NULL)
    {
        hit = 1;
        CHAR_SETWORKINT_LOW(charaindex, CHAR_WORKBATTLECOM5, 777);
    }

#endif
    CHAR_SETWORKINT_LOW(charaindex, CHAR_WORKBATTLECOM3, N);
    CHAR_SETWORKINT_HIGH(charaindex, CHAR_WORKBATTLECOM3, Per);
    return TRUE;
}

int PETSKILL_Guardian(
    int charaindex,
    int toNo,
    int array,
    char *data
)
{
    char *pszOption, *pszP;
    int
    battleindex, side, pos, ownerpos,
                 strdef, flg;
    float fPer = 0.01;
    pszOption = PETSKILL_getChar(array, PETSKILL_OPTION);

    if ((pszP = strstr(pszOption, "攻%")) != NULL)
    {
        sscanf(pszP + 3, "%f", &fPer);
        fPer = (fPer / 100);
        strdef = CHAR_getWorkInt(charaindex, CHAR_WORKFIXSTR);
        strdef = (int)(strdef * fPer);
        CHAR_setWorkInt(charaindex, CHAR_WORKATTACKPOWER, (CHAR_getWorkInt(charaindex, CHAR_WORKFIXSTR) + strdef));
    }

    if ((pszP = strstr(pszOption, "防%")) != NULL)
    {
        sscanf(pszP + 3, "%f", &fPer);
        fPer = (fPer / 100);
        strdef = CHAR_getWorkInt(charaindex, CHAR_WORKFIXTOUGH);
        strdef = (int)(strdef * fPer);
        CHAR_setWorkInt(charaindex, CHAR_WORKDEFENCEPOWER, (CHAR_getWorkInt(charaindex, CHAR_WORKFIXTOUGH) + strdef));
    }

    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM1, BATTLE_COM_S_GUARDIAN_ATTACK);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM2, toNo);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLEMODE, BATTLE_CHARMODE_C_OK);
    flg = CHAR_getWorkInt(charaindex, CHAR_WORKBATTLEFLG);
    flg |= CHAR_BATTLEFLG_GUARDIAN;
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLEFLG, flg);
    battleindex = CHAR_getWorkInt(charaindex, CHAR_WORKBATTLEINDEX);
    pos = BATTLE_Index2No(battleindex, charaindex);

    if ((pszP = strstr(pszOption, "COM:")) != NULL
            &&  strstr(pszP + 4, "防御") != NULL
       )
    {
        CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM1, BATTLE_COM_GUARD);
        side = (toNo >= SIDE_OFFSET) ? (1) : (0);
        ownerpos = toNo - side * SIDE_OFFSET;

        if (0 <= ownerpos && ownerpos < SIDE_OFFSET)
        {
            BattleArray[battleindex].Side[side].Entry[ownerpos].guardian = pos;
        }
    }
    else
    {
        side = CHAR_getWorkInt(charaindex, CHAR_WORKBATTLESIDE);
        ownerpos = pos - 5;
        ownerpos -= side * SIDE_OFFSET;

        if (ownerpos < 0 || ownerpos > 19)
        {
        }
        else
        {
            BattleArray[battleindex].Side[side].Entry[ownerpos].guardian = pos;
        }
    }

    return TRUE;
}

int PETSKILL_Mighty(
    int charaindex,
    int toindex,
    int array,
    char *data
)
{
    char *pszOption, *pszP;
    int iBai = 0, iDuck = 0;
    float fBai = 2.00;
    float fPer = 0.01;
    int strdef;
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM1, BATTLE_COM_S_MIGHTY);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM2, toindex);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLEMODE, BATTLE_CHARMODE_C_OK);
    pszOption = PETSKILL_getChar(array, PETSKILL_OPTION);

    if ((pszP = strstr(pszOption, "倍")) != NULL)
    {
        sscanf(pszP + 2, "%f", &fBai);
        iBai = (int)(fBai * 100);
    }

    CHAR_SETWORKINT_LOW(charaindex, CHAR_WORKBATTLECOM3, iBai);
    pszOption = PETSKILL_getChar(array, PETSKILL_OPTION);

    if ((pszP = strstr(pszOption, "避")) != NULL)
    {
        sscanf(pszP + 2, "%d", &iDuck);
    }

    if ((pszP = strstr(pszOption, "攻%")) != NULL)
    {
        sscanf(pszP + 3, "%f", &fPer);
        fPer = (fPer / 100);
        strdef = CHAR_getWorkInt(charaindex, CHAR_WORKFIXSTR);
        strdef = (int)(strdef * fPer);
        CHAR_setWorkInt(charaindex, CHAR_WORKATTACKPOWER, (CHAR_getWorkInt(charaindex, CHAR_WORKFIXSTR) + strdef));
    }

    if ((pszP = strstr(pszOption, "防%")) != NULL)
    {
        sscanf(pszP + 3, "%f", &fPer);
        fPer = (fPer / 100);
        strdef = CHAR_getWorkInt(charaindex, CHAR_WORKFIXTOUGH);
        strdef = (int)(strdef * fPer);
        CHAR_setWorkInt(charaindex, CHAR_WORKDEFENCEPOWER, (CHAR_getWorkInt(charaindex, CHAR_WORKFIXTOUGH) + strdef));
    }

    if ((pszP = strstr(pszOption, "敏%")) != NULL)
    {
        sscanf(pszP + 3, "%f", &fPer);
        fPer = (fPer / 100);
        strdef = CHAR_getWorkInt(charaindex, CHAR_WORKFIXDEX);
        strdef = (int)(strdef * fPer);
        CHAR_setWorkInt(charaindex, CHAR_WORKQUICK, (CHAR_getWorkInt(charaindex, CHAR_WORKFIXDEX) + strdef));
    }

    CHAR_SETWORKINT_HIGH(charaindex, CHAR_WORKBATTLECOM3, iDuck);
    return TRUE;
}



int PETSKILL_PowerBalance(
    int charaindex,
    int toindex,
    int array,
    char *data
)
{
    char *pszOption = "\0", *pszP;
    float fPer = 0.01;
    int strdef;
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM1, BATTLE_COM_S_POWERBALANCE);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM2, toindex);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLEMODE, BATTLE_CHARMODE_C_OK);
    pszOption = PETSKILL_getChar(array, PETSKILL_OPTION);

    if (pszOption == "\0")
    {
        print("\n pszOption == NULL ");
        return FALSE;
    }

#ifdef _POWER_UP
    char *buff = CHAR_getChar(charaindex, CHAR_POWER_UP);
    char token[256];
    int type = 0;

    if (getStringFromIndexWithDelim(buff, "|", 1, token, sizeof(token)))
    {
        type = atoi(token);
    }

    int value = 0;

    if (getStringFromIndexWithDelim(buff, "|", 2, token, sizeof(token)))
    {
        value += atoi(token);
    }

    if (getStringFromIndexWithDelim(buff, "|", 3, token, sizeof(token)))
    {
        value += atoi(token);
    }

    if (getStringFromIndexWithDelim(buff, "|", 4, token, sizeof(token)))
    {
        value += atoi(token);
    }

#endif

    if ((pszP = strstr(pszOption, "攻%")) != NULL)
    {
        sscanf(pszP + 3, "%f", &fPer);
        fPer = (fPer / 100);
#ifdef _POWER_UP

        if (type == 2)
        {
            strdef = CHAR_getWorkInt(charaindex, CHAR_WORKFIXSTR) + value;
            strdef = (int)(strdef * fPer);
            CHAR_setWorkInt(charaindex, CHAR_WORKATTACKPOWER, (CHAR_getWorkInt(charaindex, CHAR_WORKFIXSTR) + strdef + value));
        }
        else
#endif
        {
            strdef = CHAR_getWorkInt(charaindex, CHAR_WORKFIXSTR);
            strdef = (int)(strdef * fPer);
            CHAR_setWorkInt(charaindex, CHAR_WORKATTACKPOWER, (CHAR_getWorkInt(charaindex, CHAR_WORKFIXSTR) + strdef));
        }
    }

    if ((pszP = strstr(pszOption, "防%")) != NULL)
    {
        sscanf(pszP + 3, "%f", &fPer);
        fPer = (fPer / 100);
#ifdef _POWER_UP

        if (type == 3)
        {
            strdef = CHAR_getWorkInt(charaindex, CHAR_WORKFIXTOUGH) + value;
            strdef = (int)(strdef * fPer);
            CHAR_setWorkInt(charaindex, CHAR_WORKDEFENCEPOWER, (CHAR_getWorkInt(charaindex, CHAR_WORKFIXTOUGH) + strdef + value));
        }
        else
#endif
        {
            strdef = CHAR_getWorkInt(charaindex, CHAR_WORKFIXTOUGH);
            strdef = (int)(strdef * fPer);
            CHAR_setWorkInt(charaindex, CHAR_WORKDEFENCEPOWER, (CHAR_getWorkInt(charaindex, CHAR_WORKFIXTOUGH) + strdef));
        }
    }

#ifdef _EXPANSION_POWERBALANCE

    if ((pszP = strstr(pszOption, "敏%")) != NULL)
    {
        sscanf(pszP + 3, "%f", &fPer);
        fPer = (fPer / 100);
#ifdef _POWER_UP

        if (type == 4)
        {
            strdef = CHAR_getWorkInt(charaindex, CHAR_WORKFIXDEX) + value;
            strdef = (int)(strdef * fPer);
            CHAR_setWorkInt(charaindex, CHAR_WORKQUICK, (CHAR_getWorkInt(charaindex, CHAR_WORKFIXDEX) + strdef + value));
        }
        else
#endif
        {
            strdef = CHAR_getWorkInt(charaindex, CHAR_WORKFIXDEX);
            strdef = (int)(strdef * fPer);
            CHAR_setWorkInt(charaindex, CHAR_WORKQUICK, (CHAR_getWorkInt(charaindex, CHAR_WORKFIXDEX) + strdef));
        }
    }

#endif
    return TRUE;
}

//----------------------------------------------------------------------
int PETSKILL_StatusChange(
    int charaindex,
    int toindex,
    int array,
    char *data

)
{
    char *pszOption, *pszP;
    float fPer = 0.01;
    int strdef;
    int status = -1, i = 0, turn = 3;
    char szTurn[] = "turn";
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM1, BATTLE_COM_S_STATUSCHANGE);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM2, toindex);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLEMODE, BATTLE_CHARMODE_C_OK);
    pszOption = PETSKILL_getChar(array, PETSKILL_OPTION);
    pszP = pszOption;

    for (; status == -1 && pszP[0] != 0; pszP++)
    {
        for (i = 1; i < min(BATTLE_ST_END, 32); i ++)
        {
            if (strncmp(pszP, aszStatus[i], 2) == 0)
            {
                status = i;
                pszP += 2;
                break;
            }
        }
    }

    if ((pszP = strstr(pszP, szTurn)) != NULL)
    {
        pszP += sizeof(szTurn);
        sscanf(pszP, "%d", &turn);
    }

    CHAR_SETWORKINT_LOW(charaindex, CHAR_WORKBATTLECOM3, i);
    CHAR_SETWORKINT_HIGH(charaindex, CHAR_WORKBATTLECOM3, turn);

    if ((pszP = strstr(pszOption, "攻%")) != NULL)
    {
        sscanf(pszP + 3, "%f", &fPer);
        fPer = (fPer / 100);
        strdef = CHAR_getWorkInt(charaindex, CHAR_WORKFIXSTR);
        strdef = (int)(strdef * fPer);
        CHAR_setWorkInt(charaindex, CHAR_WORKATTACKPOWER,
                        (CHAR_getWorkInt(charaindex, CHAR_WORKFIXSTR) + strdef));
    }

    if ((pszP = strstr(pszOption, "防%")) != NULL)
    {
        sscanf(pszP + 3, "%f", &fPer);
        fPer = (fPer / 100);
        strdef = CHAR_getWorkInt(charaindex, CHAR_WORKFIXTOUGH);
        strdef = (int)(strdef * fPer);
        CHAR_setWorkInt(charaindex, CHAR_WORKDEFENCEPOWER,
                        (CHAR_getWorkInt(charaindex, CHAR_WORKFIXTOUGH) + strdef));
    }

    if ((pszP = strstr(pszOption, "敏%")) != NULL)
    {
        sscanf(pszP + 3, "%f", &fPer);
        fPer = (fPer / 100);
        strdef = CHAR_getWorkInt(charaindex, CHAR_WORKFIXDEX);
        strdef = (int)(strdef * fPer);
        CHAR_setWorkInt(charaindex, CHAR_WORKQUICK,
                        (CHAR_getWorkInt(charaindex, CHAR_WORKFIXDEX) + strdef));
    }

#ifdef _PETSKILL_ISULTIMATE

    if ((pszP = strstr(pszOption, "命%")) != NULL)
    {
        sscanf(pszP + 3, "%f", &fPer);
        CHAR_SETWORKINT_HIGH(charaindex, CHAR_WORKBATTLECOM6, -fPer);
    }
    else
    {
        CHAR_SETWORKINT_HIGH(charaindex, CHAR_WORKBATTLECOM6, 0);
    }

    if ((pszP = strstr(pszOption, "怒1")) != NULL)
    {
        sscanf(pszP + 3, "%d", &turn);
        CHAR_SETWORKINT_LOW(charaindex, CHAR_WORKBATTLECOM5, turn);
    }
    else
    {
        CHAR_SETWORKINT_LOW(charaindex, CHAR_WORKBATTLECOM5, 0);
    }

    if ((pszP = strstr(pszOption, "怒2")) != NULL)
    {
        sscanf(pszP + 3, "%d", &turn);
        CHAR_SETWORKINT_HIGH(charaindex, CHAR_WORKBATTLECOM5, turn);
    }
    else
    {
        CHAR_SETWORKINT_HIGH(charaindex, CHAR_WORKBATTLECOM5, 0);
    }

    if ((pszP = strstr(pszOption, "次")) != NULL)
    {
        sscanf(pszP + 2, "%f", &fPer);
        CHAR_SETWORKINT_LOW(charaindex, CHAR_WORKBATTLECOM6, fPer);
    }
    else
    {
        CHAR_SETWORKINT_LOW(charaindex, CHAR_WORKBATTLECOM6, 0);
    }

#endif
    return TRUE;
}

#ifdef _PETSKILL_ISULTIMATE
int PETSKILL_Isultimate(int charaindex, int toNo, int array, char *data)
{
    char *pszOption, *pszP;
    int strdef;
    float fPer = 0.01;
    pszOption = PETSKILL_getChar(array, PETSKILL_OPTION);

    if ((pszP = strstr(pszOption, "攻%")) != NULL)
    {
        sscanf(pszP + 3, "%f", &fPer);
        fPer = (fPer / 100);
        strdef = CHAR_getWorkInt(charaindex, CHAR_WORKFIXSTR);
        strdef = (int)(strdef * fPer);
        CHAR_setWorkInt(charaindex, CHAR_WORKATTACKPOWER, (CHAR_getWorkInt(charaindex, CHAR_WORKFIXSTR) + strdef));
    }

    if ((pszP = strstr(pszOption, "防%")) != NULL)
    {
        sscanf(pszP + 3, "%f", &fPer);
        fPer = (fPer / 100);
        strdef = CHAR_getWorkInt(charaindex, CHAR_WORKFIXTOUGH);
        strdef = (int)(strdef * fPer);
        CHAR_setWorkInt(charaindex, CHAR_WORKDEFENCEPOWER, (CHAR_getWorkInt(charaindex, CHAR_WORKFIXTOUGH) + strdef));
    }

    if ((pszP = strstr(pszOption, "敏%")) != NULL)
    {
        sscanf(pszP + 3, "%f", &fPer);
        CHAR_setWorkInt(charaindex, CHAR_SKILLDEXPOWER, fPer);
        fPer = fPer / 100;
        strdef = CHAR_getWorkInt(charaindex, CHAR_WORKFIXDEX);
        strdef = (int)(strdef * fPer);
        CHAR_setWorkInt(charaindex, CHAR_WORKQUICK, CHAR_getWorkInt(charaindex, CHAR_WORKFIXDEX) + strdef);
    }

    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM1, BATTLE_COM_S_ISULTIMATE);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM2, toNo);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLEMODE, BATTLE_CHARMODE_C_OK);
    CHAR_SETWORKINT_LOW(charaindex, CHAR_WORKBATTLECOM3, array);
    //CHAR_SETWORKINT_LOW( charaindex, CHAR_WORKBATTLECOM3, kill[rand()%count] );
    CHAR_SETWORKINT_HIGH(charaindex, CHAR_WORKBATTLECOM3, 0);
    return TRUE;
}
#endif

#ifdef _VARY_WOLF

int PETSKILL_Vary(int cindex, int tindex, int id, char* data)
{
    char  *pszOption = "\0", *pszP;
    float fPer = 0.01;
    int a_dep;
    int d_dep;
#ifndef _EXPANSION_VARY_WOLF
    int i;
    int petid[4] = {981, 982, 983, 984};

    for (i = 0; i < 4; i++)
    {
        if (CHAR_getInt(cindex, CHAR_PETID) == petid[i])
        {
            break;
        }
    }

    if (i >= 4)
    {
        return FALSE;
    }

#endif
    CHAR_setWorkInt(cindex, CHAR_WORKBATTLECOM1, BATTLE_COM_S_VARY);
    CHAR_setWorkInt(cindex, CHAR_WORKBATTLECOM2, tindex);
    CHAR_setWorkInt(cindex, CHAR_WORKBATTLEMODE, BATTLE_CHARMODE_C_OK);
    pszOption = PETSKILL_getChar(id, PETSKILL_OPTION);

    if (pszOption == "\0")
    {
        print("\n pszOption==NULL");
        return FALSE;
    }

    if ((pszP = strstr(pszOption, "攻%")) != NULL)
    {
        sscanf(pszP + 3, "%f", &fPer);
        CHAR_setWorkInt(cindex, CHAR_SKILLSTRPOWER, fPer);
        fPer = fPer / 100;
        a_dep = CHAR_getWorkInt(cindex, CHAR_WORKFIXSTR);
        a_dep = (int)(a_dep * fPer);
        CHAR_setWorkInt(cindex, CHAR_WORKATTACKPOWER, CHAR_getWorkInt(cindex, CHAR_WORKFIXSTR) + a_dep);
    }

    if ((pszP = strstr(pszOption, "防%")) != NULL)
    {
        sscanf(pszP + 3, "%f", &fPer);
        CHAR_setWorkInt(cindex, CHAR_SKILLTGHPOWER, fPer);
        fPer = fPer / 100;
        a_dep = CHAR_getWorkInt(cindex, CHAR_WORKFIXTOUGH);
        a_dep = (int)(a_dep * fPer);
        CHAR_setWorkInt(cindex, CHAR_WORKDEFENCEPOWER, CHAR_getWorkInt(cindex, CHAR_WORKFIXTOUGH) + a_dep);
    }

    if ((pszP = strstr(pszOption, "敏%")) != NULL)
    {
        sscanf(pszP + 3, "%f", &fPer);
        CHAR_setWorkInt(cindex, CHAR_SKILLDEXPOWER, fPer);
        fPer = fPer / 100;
        d_dep = CHAR_getWorkInt(cindex, CHAR_WORKFIXDEX);
        d_dep = (int)(d_dep * fPer);
        CHAR_setWorkInt(cindex, CHAR_WORKQUICK, CHAR_getWorkInt(cindex, CHAR_WORKFIXDEX) + d_dep);
    }

#ifdef _EXPANSION_VARY_WOLF

    if ((pszP = strstr(pszOption, "图%")) != NULL)
    {
        int image = 0;
        sscanf(pszP + 3, "%d", &image);
        CHAR_setInt(cindex, CHAR_BASEIMAGENUMBER, image);
    }
    else
    {
        CHAR_setInt(cindex, CHAR_BASEIMAGENUMBER, 101428);
    }

    //CHAR_SETWORKINT_LOW( cindex, CHAR_WORKBATTLECOM3, CHAR_getInt( cindex, CHAR_BASEIMAGENUMBER));
    CHAR_setWorkInt(cindex, CHAR_WORKBATTLECOM3, 0);

    if ((pszP = strstr(pszOption, "动%")) != NULL)
    {
        int image = 0;
        sscanf(pszP + 3, "%d", &image);
        CHAR_setWorkInt(cindex, CHAR_WORKBATTLECOM3, image);
    }

#else
    CHAR_setInt(cindex, CHAR_BASEIMAGENUMBER, 101428);
#endif
    CHAR_setWorkInt(cindex, CHAR_WORKTURN, 0);
    return TRUE;
}
#endif

#ifdef _SKILL_WILDVIOLENT_ATT

int PETSKILL_WildViolentAttack(
    int charaindex,
    int toindex,
    int array,
    char *data

)
{
    char *pszOption = "\0", *pszP;
    float fPer = 0.01;
    int strdef = 0;
    int iDuck = 0;
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM1, BATTLE_COM_S_WILDVIOLENTATTACK);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM2, toindex);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLEMODE, BATTLE_CHARMODE_C_OK);
    pszOption = PETSKILL_getChar(array, PETSKILL_OPTION);

    if (pszOption == "\0")
    {
        print("\n pszOption == NULL ");
        return FALSE;
    }

    //读取攻值
    if ((pszP = strstr(pszOption, "攻%")) != NULL)
    {
        sscanf(pszP + 3, "%f", &fPer);
        fPer = (fPer / 100);
        //设定修正值
        strdef = CHAR_getWorkInt(charaindex, CHAR_WORKFIXSTR);
        strdef = (int)(strdef * fPer);
        CHAR_setWorkInt(charaindex, CHAR_WORKATTACKPOWER, (CHAR_getWorkInt(charaindex, CHAR_WORKFIXSTR) + strdef));
    }

    //读取防值
    if ((pszP = strstr(pszOption, "防%")) != NULL)
    {
        sscanf(pszP + 3, "%f", &fPer);
        fPer = (fPer / 100);
        //设定修正值
        strdef = CHAR_getWorkInt(charaindex, CHAR_WORKFIXTOUGH);
        strdef = (int)(strdef * fPer);
        CHAR_setWorkInt(charaindex, CHAR_WORKDEFENCEPOWER, (CHAR_getWorkInt(charaindex, CHAR_WORKFIXTOUGH) + strdef));
    }

    //读取避值
    if ((pszP = strstr(pszOption, "避")) != NULL)
    {
        sscanf(pszP + 2, "%d", &iDuck);
    }

    //       膜恳毛忡绣
    CHAR_SETWORKINT_HIGH(charaindex, CHAR_WORKBATTLECOM3, iDuck);
    return TRUE;
}
#endif

#ifdef _SKILL_SACRIFICE
int PETSKILL_Sacrifice(
    int charaindex,
    int toindex,
    int array,
    char *data

)
{
    char buf[256];

    if (CHAR_getInt(charaindex, CHAR_HP) > CHAR_getWorkInt(charaindex, CHAR_WORKMAXHP) * 0.2)
    {
        CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM1, BATTLE_COM_S_SACRIFICE);
        CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM2, toindex);
        CHAR_setWorkInt(charaindex, CHAR_WORKBATTLEMODE, BATTLE_CHARMODE_C_OK);
    }
    else
    {
        sprintf(buf, "%s自身耐久力不足 救援失败!!", CHAR_getChar(charaindex, CHAR_NAME));
        CHAR_talkToCli(CHAR_getWorkInt(charaindex, CHAR_WORKPLAYERINDEX), -1, buf, CHAR_COLORYELLOW);
        return FALSE;
    }

    return TRUE;
}
#endif
#ifdef _SKILL_REFRESH
int PETSKILL_Refresh(
    int charaindex,
    int toindex,
    int array,
    char *data

)
{
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM1, BATTLE_COM_S_REFRESH);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM2, toindex);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLEMODE, BATTLE_CHARMODE_C_OK);
    CHAR_SETWORKINT_LOW(charaindex, CHAR_WORKBATTLECOM3, array);
    return TRUE;
}
#endif
#ifdef _SKILL_WEAKEN  //vincent宠技:虚弱
int PETSKILL_Weaken(
    int charaindex,
    int toindex,
    int array,
    char *data

)
{
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM1, BATTLE_COM_S_WEAKEN);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM2, toindex);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLEMODE, BATTLE_CHARMODE_C_OK);
    CHAR_SETWORKINT_LOW(charaindex, CHAR_WORKBATTLECOM3, array);
    return TRUE;
}
#endif
#ifdef _SKILL_DEEPPOISON  //vincent宠技:剧毒 
int PETSKILL_Deeppoison(
    int charaindex,
    int toindex,
    int array,
    char *data

)
{
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM1, BATTLE_COM_S_DEEPPOISON);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM2, toindex);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLEMODE, BATTLE_CHARMODE_C_OK);
    CHAR_SETWORKINT_LOW(charaindex, CHAR_WORKBATTLECOM3, array);
    return TRUE;
}
#endif

#ifdef _SKILL_BARRIER  //vincent宠技:魔障
int PETSKILL_Barrier(
    int charaindex,
    int toindex,
    int array,
    char *data

)
{
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM1, BATTLE_COM_S_BARRIER);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM2, toindex);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLEMODE, BATTLE_CHARMODE_C_OK);
    CHAR_SETWORKINT_LOW(charaindex, CHAR_WORKBATTLECOM3, array);
    return TRUE;
}
#endif

#ifdef _SKILL_NOCAST  //vincent宠技:沉默
int PETSKILL_Nocast(
    int charaindex,
    int toindex,
    int array,
    char *data

)
{
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM1, BATTLE_COM_S_NOCAST);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM2, toindex);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLEMODE, BATTLE_CHARMODE_C_OK);
    CHAR_SETWORKINT_LOW(charaindex, CHAR_WORKBATTLECOM3, array);
    return TRUE;
}
#endif

#ifdef _SKILL_ROAR //vincent宠技:大吼
int PETSKILL_Roar(
    int charaindex,
    int toindex,
    int array,
    char *data

)
{
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM1, BATTLE_COM_S_ROAR);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM2, toindex);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLEMODE, BATTLE_CHARMODE_C_OK);
    CHAR_SETWORKINT_LOW(charaindex, CHAR_WORKBATTLECOM3, array);
    return TRUE;
}
#endif

#ifdef _SKILL_GUARDBREAK2//破除防御2 vincent add 2002/05/20
int PETSKILL_GuardBreak2(
    int charaindex,
    int toNo,
    int array,
    char *data

)
{
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM1, BATTLE_COM_S_GBREAK2);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM2, toNo);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLEMODE, BATTLE_CHARMODE_C_OK);
    return TRUE;
}
#endif

//*******************************************************
int PETSKILL_NormalAttack(
    int charaindex,
    int toindex,
    int array,
    char *data

)
{
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM1, BATTLE_COM_ATTACK);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM2, toindex);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLEMODE, BATTLE_CHARMODE_C_OK);
    return TRUE;
}

#ifdef _INVERSION_PETSKILL
int PETSKILL_Inversion(
    int charaindex,
    int toindex,
    int array,
    char *data

)
{
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM1, BATTLE_COM_S_INVERSION);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM2, toindex);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLEMODE, BATTLE_CHARMODE_C_OK);
    return TRUE;
}
#endif



#ifdef _MASSAGE_PETSKILL
int PETSKILL_Massage(
    int charaindex,
    int toindex,
    int array,
    char *data

)
{
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM1, BATTLE_COM_S_MASSAGE);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM2, toindex);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLEMODE, BATTLE_CHARMODE_C_OK);
    return TRUE;
}
#endif

#ifdef _PETOUT_PETSKILL
int PETSKILL_Petout(
    int charaindex,
    int toindex,
    int array,
    char *data

)
{
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM1, BATTLE_COM_S_PETOUT);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM2, toindex);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLEMODE, BATTLE_CHARMODE_C_OK);
    CHAR_SETWORKINT_LOW(charaindex, CHAR_WORKBATTLECOM3, array); //记录属性
    return TRUE;
}
#endif

#ifdef _STRENGTH_PETSKILL
int PETSKILL_Strength(int charaindex, int toNo, int array, char *data)
{
    int nums = 0;

    if (CHAR_CHECKINDEX(charaindex) == FALSE)
    {
        return FALSE;
    }

    nums = CHAR_getWorkInt(charaindex, CHAR_WORK_STRENGTH);
    char *skillarg = PETSKILL_getChar(array, PETSKILL_OPTION);
    char buf1[256];

    if (getStringFromIndexWithDelim(skillarg, "|", 1, buf1, sizeof(buf1)) == FALSE)
    {
        return FALSE;
    }

    if (nums >= max(1, atoi(buf1)))
    {
        int toindex = CHAR_getWorkInt(charaindex, CHAR_WORKPLAYERINDEX);
        char buf[256];
        sprintf(buf, "此技能单场限用 %d 次。", max(1, atoi(buf1)));
        CHAR_talkToCli(toindex, -1, buf, CHAR_COLORYELLOW);
        return FALSE;
    }

    CHAR_setWorkInt(charaindex, CHAR_WORK_STRENGTH, ++nums);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM1, BATTLE_COM_S_STRENGTH);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM2, toNo);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLEMODE, BATTLE_CHARMODE_C_OK);
    CHAR_SETWORKINT_LOW(charaindex, CHAR_WORKBATTLECOM3, array); //记录属性
    return TRUE;
}
#endif

int PETSKILL_NormalGuard(
    int charaindex,
    int toindex,
    int array,
    char *data

)
{
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM1, BATTLE_COM_GUARD);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM2, toindex);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLEMODE, BATTLE_CHARMODE_C_OK);
    return TRUE;
}


int PETSKILL_None(
    int charaindex,
    int toindex,
    int array,
    char *data
)
{
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM1, BATTLE_COM_NONE);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM2, toindex);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLEMODE, BATTLE_CHARMODE_C_OK);
    return TRUE;
}


int PETSKILL_EarthRound(
    int charaindex,
    int toNo,
    int array,
    char *data
)
{
    char *pszOption, *pszP;
    float fPer = 0.01;
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM1, BATTLE_COM_S_EARTHROUND1);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM2, toNo);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLEMODE, BATTLE_CHARMODE_C_OK);
    pszOption = PETSKILL_getChar(array, PETSKILL_OPTION);

    if ((pszP = strstr(pszOption, "攻%")) != NULL)
    {
        sscanf(pszP + 3, "%f", &fPer);
        CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM3, (int)fPer);
    }

    return TRUE;
}


int PETSKILL_GuardBreak(
    int charaindex,
    int toNo,
    int array,
    char *data

)
{
    char *pszOption, *pszP;
    float fPer = 0.01;
    int strdef;
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM1, BATTLE_COM_S_GBREAK);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM2, toNo);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLEMODE, BATTLE_CHARMODE_C_OK);
    pszOption = PETSKILL_getChar(array, PETSKILL_OPTION);

    if ((pszP = strstr(pszOption, "攻%")) != NULL)
    {
        sscanf(pszP + 3, "%f", &fPer);
        fPer = (fPer / 100);
        strdef = CHAR_getWorkInt(charaindex, CHAR_WORKFIXSTR);
        strdef = (int)(strdef * fPer);
        CHAR_setWorkInt(charaindex, CHAR_WORKATTACKPOWER, (CHAR_getWorkInt(charaindex, CHAR_WORKFIXSTR) + strdef));
    }

    if ((pszP = strstr(pszOption, "防%")) != NULL)
    {
        sscanf(pszP + 3, "%f", &fPer);
        fPer = (fPer / 100);
        strdef = CHAR_getWorkInt(charaindex, CHAR_WORKFIXTOUGH);
        strdef = (int)(strdef * fPer);
        CHAR_setWorkInt(charaindex, CHAR_WORKDEFENCEPOWER, (CHAR_getWorkInt(charaindex, CHAR_WORKFIXTOUGH) + strdef));
    }

    if ((pszP = strstr(pszOption, "敏%")) != NULL)
    {
        sscanf(pszP + 3, "%f", &fPer);
        fPer = (fPer / 100);
        strdef = CHAR_getWorkInt(charaindex, CHAR_WORKFIXDEX);
        strdef = (int)(strdef * fPer);
        CHAR_setWorkInt(charaindex, CHAR_WORKQUICK, (CHAR_getWorkInt(charaindex, CHAR_WORKFIXDEX) + strdef));
    }

    return TRUE;
}
#ifdef _SKILL_SPEEDY_ATT//疾速攻击 vincent add 2002/05/20
int PETSKILL_SpeedyAttack(
    int charaindex,
    int toNo,
    int array,
    char *data

)
{
    char *pszOption = "\0", *pszP;
    float fPer = 0.01;
    int strdef = 0;
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM1, BATTLE_COM_S_SPEEDYATTACK);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM2, toNo);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLEMODE, BATTLE_CHARMODE_C_OK);
    pszOption = PETSKILL_getChar(array, PETSKILL_OPTION);

    if (pszOption == "\0")
    {
        print("\n pszOption == NULL ");
        return FALSE;
    }

    if ((pszP = strstr(pszOption, "攻%")) != NULL)
    {
        sscanf(pszP + 3, "%f", &fPer);
        fPer = (fPer / 100);
        strdef = CHAR_getWorkInt(charaindex, CHAR_WORKFIXSTR);
        strdef = (int)(strdef * fPer);
        CHAR_setWorkInt(charaindex, CHAR_WORKATTACKPOWER, (CHAR_getWorkInt(charaindex, CHAR_WORKFIXSTR) + strdef));
    }

    if ((pszP = strstr(pszOption, "防%")) != NULL)
    {
        sscanf(pszP + 3, "%f", &fPer);
        fPer = (fPer / 100);
        strdef = CHAR_getWorkInt(charaindex, CHAR_WORKFIXTOUGH);
        strdef = (int)(strdef * fPer);
        CHAR_setWorkInt(charaindex, CHAR_WORKDEFENCEPOWER, (CHAR_getWorkInt(charaindex, CHAR_WORKFIXTOUGH) + strdef));
    }

    if ((pszP = strstr(pszOption, "敏%")) != NULL)
    {
        sscanf(pszP + 3, "%f", &fPer);
        fPer = (fPer / 100);
        strdef = CHAR_getWorkInt(charaindex, CHAR_WORKFIXDEX);
        strdef = (int)(strdef * fPer);
        CHAR_setWorkInt(charaindex, CHAR_WORKQUICK, (CHAR_getWorkInt(charaindex, CHAR_WORKFIXDEX) + strdef));
    }

    return TRUE;
}
#endif

int PETSKILL_Abduct(
    int charaindex,
    int toNo,
    int array,
    char *data
)
{
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM1, BATTLE_COM_S_ABDUCT);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM2, toNo);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLEMODE, BATTLE_CHARMODE_C_OK);
    CHAR_SETWORKINT_LOW(charaindex, CHAR_WORKBATTLECOM3, array);
#ifdef _ABDUCT_NO_PETOUT
    {
        int badindex = CHAR_getWorkInt(charaindex, CHAR_WORKPLAYERINDEX);

        if (CHAR_getWorkInt(badindex, CHAR_WORKBATTLECOM1) == BATTLE_COM_PETOUT)
        {
            CHAR_setWorkInt(badindex, CHAR_WORKBATTLECOM1, BATTLE_COM_NONE);
            CHAR_talkToCli(badindex, -1, "使用旅程伙伴无法同时换宠出来！", CHAR_COLORYELLOW);
        }
    }
#endif
    return TRUE;
}

int PETSKILL_Steal(
    int charaindex,
    int toNo,
    int array,
    char *data
)
{
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM1, BATTLE_COM_S_STEAL);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM2, toNo);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLEMODE, BATTLE_CHARMODE_C_OK);
    return TRUE;
}

#ifdef _BATTLESTEAL_FIX
int PETSKILL_StealMoney(int charaindex, int toNo, int array, char *data)
{
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM1, BATTLE_COM_S_STEALMONEY);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM2, toNo);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLEMODE, BATTLE_CHARMODE_C_OK);
    return TRUE;
}
#endif

//*******************************************************
// 檗  -- 失奶  丞宁岳
//
int PETSKILL_Merge(
    int charaindex,
    int toNo,
    int array,
    char *data
)
{
    int ret, ownerindex;
    // Robin 0521 debug
    ownerindex = CHAR_getWorkInt(charaindex, CHAR_WORKPLAYERINDEX);

    if (CHAR_getWorkInt(ownerindex, CHAR_WORKBATTLEMODE) != BATTLE_CHARMODE_NONE)
    {
#ifdef  _NO_WAIGUA
        int     j;
        char    category[3];

        for (j = 0; j < CHAR_MAXPETHAVE; j ++)
        {
            snprintf(category, sizeof(category), "K%d", j);
            CHAR_sendStatusString(ownerindex, category);
            snprintf(category, sizeof(category), "W%d", j);
            CHAR_sendStatusString(ownerindex, category);
        }

#endif
        return FALSE;
    }

    ret = FreeMergeItem(ownerindex, data);

    if (ret == FALSE)
        ret = ITEM_mergeItem_merge(CHAR_getWorkInt(charaindex, CHAR_WORKPLAYERINDEX),
                                   CHAR_getInt(charaindex, CHAR_PETID), data, charaindex, 0);

#ifdef  _NO_WAIGUA
    int     j;
    char    category[3];

    for (j = 0; j < CHAR_MAXPETHAVE; j ++)
    {
        snprintf(category, sizeof(category), "K%d", j);
        CHAR_sendStatusString(ownerindex, category);
        snprintf(category, sizeof(category), "W%d", j);
        CHAR_sendStatusString(ownerindex, category);
    }

#endif

    if (ret)
    {
        FreeMergeItemOk(ownerindex);
    }
    else
    {
        FreeMergeItemFalse(ownerindex);
    }

    return ret;
}

#ifdef _ALCHEMIST
int PETSKILL_Alchemist(
    int charaindex,
    int toNo,
    int array,
    char *data
)
{
    int ret, ownerindex;
    // Robin 0521 debug
    ownerindex = CHAR_getWorkInt(charaindex, CHAR_WORKPLAYERINDEX);

    if (CHAR_getWorkInt(ownerindex, CHAR_WORKBATTLEMODE) != BATTLE_CHARMODE_NONE)
    {
        print(" Merge_Alchemist_ErrB:fd=%d ", ownerindex);
        return FALSE;
    }

    ret = ITEM_mergeItem_merge(CHAR_getWorkInt(charaindex, CHAR_WORKPLAYERINDEX),
                               CHAR_getInt(charaindex, CHAR_PETID), data, charaindex, 1);
    return ret;
}
#endif

int PETSKILL_NoGuard(
    int charaindex,
    int toNo,
    int array,
    char *data

)
{
    char *pszOption, *pszP;
    int Duck = 0, Counter = 0, Critical = 0;
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM1, BATTLE_COM_S_NOGUARD);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM2, toNo);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLEMODE, BATTLE_CHARMODE_C_OK);
    pszOption = PETSKILL_getChar(array, PETSKILL_OPTION);

    if ((pszP = strstr(pszOption, "避%")) != NULL)
    {
        sscanf(pszP + 3, "%d", &Duck);
        CHAR_SETWORKINT_HIGH(charaindex, CHAR_WORKBATTLECOM3, Duck);
    }

    if ((pszP = strstr(pszOption, "击%")) != NULL)
    {
        sscanf(pszP + 3, "%d", &Counter);
    }

    if ((pszP = strstr(pszOption, "心%")) != NULL)
    {
        sscanf(pszP + 3, "%d", &Critical);
    }

    CHAR_SETWORKINT_LOW(charaindex, CHAR_WORKBATTLECOM3,
                        (Counter << 8) + Critical);
    return TRUE;
}

#ifdef _ATTACK_MAGIC

int PETSKILL_AttackMagic(
    int charaindex,
    int toindex,
    int array,
    char *data
)
{
    char *pszP;
    char szMagic[] = "magic";
    int magic = 313;    // magic id预设为火魔法等级一,item index预设为19659
//  int item = 19659;
    // 取得宠物技能命令
    pszP = PETSKILL_getChar(array, PETSKILL_OPTION);

    // 取得施放的魔法的magic id
    if ((pszP = strstr(pszP, szMagic)) != NULL)
    {
        pszP += sizeof(szMagic);
        sscanf(pszP, "%d", &magic);
    }

    /*
        // 取得有此魔法属性的item index
        if((pszP = strstr(pszP,szItem)) != NULL)
        {
            pszP += sizeof(szItem);
            sscanf(pszP,"%d",&item);
        }
    */
    // 设定宠物技能为魔法
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM1, BATTLE_COM_S_ATTACK_MAGIC);
    // 攻击目标设定
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM2, toindex);
    // 设定所使用的是那一种魔法
    CHAR_SETWORKINT_LOW(charaindex, CHAR_WORKBATTLECOM3, magic);
    // 设定所使用的魔法的道具
//  CHAR_SETWORKINT_HIGH(charaindex,CHAR_WORKBATTLECOM3,item);
    // 命令下达完毕
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLEMODE, BATTLE_CHARMODE_C_OK);
    return TRUE;
}

#endif

#ifdef _PSKILL_FALLGROUND   //落马术
int PETSKILL_FallGround(int charaindex, int toNo, int array, char *data)
{
    char *pszOption, *pszP;
    float fPer = 0.01;
    int strdef;
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM1, BATTLE_COM_S_FALLRIDE);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM2, toNo);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLEMODE, BATTLE_CHARMODE_C_OK);
    pszOption = PETSKILL_getChar(array, PETSKILL_OPTION);

    if ((pszP = strstr(pszOption, "攻%")) != NULL)
    {
        sscanf(pszP + 3, "%f", &fPer);
        fPer = (fPer / 100);
        strdef = CHAR_getWorkInt(charaindex, CHAR_WORKFIXSTR);
        strdef = (int)(strdef * fPer);
        CHAR_setWorkInt(charaindex, CHAR_WORKATTACKPOWER, (CHAR_getWorkInt(charaindex, CHAR_WORKFIXSTR) + strdef));
    }

    return TRUE;
}   //BATTLE_COM_S_DAMAGETOHP
#endif
#ifdef _PETSKILL_EXPLODE
int PETSKILL_Explode(int charaindex, int toNo, int array, char *data)
{
    char *pszOption, *pszP;
    float fPer = 0.01;
    int strdef;
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM2, toNo);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLEMODE, BATTLE_CHARMODE_C_OK);

    if (BattleArray[CHAR_getWorkInt(charaindex, CHAR_WORKBATTLEINDEX)].type != BATTLE_TYPE_P_vs_P)
    {
        CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM1, BATTLE_COM_ATTACK);
        return TRUE;
    }
    else
    {
        CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM1, BATTLE_COM_S_EXPLODE);
    }

    pszOption = PETSKILL_getChar(array, PETSKILL_OPTION);

    if ((pszP = strstr(pszOption, "命%")) != NULL)
    {
        sscanf(pszP + 3, "%f", &fPer);
        fPer = (fPer / 100);
        strdef = CHAR_getWorkInt(charaindex, CHAR_WORKFIXSTR);
        strdef = (int)(strdef * fPer);
        CHAR_setWorkInt(charaindex, CHAR_WORKFIXDEX, (CHAR_getWorkInt(charaindex, CHAR_WORKFIXDEX) + strdef));
    }

    if ((pszP = strstr(pszOption, "防%")) != NULL)
    {
        sscanf(pszP + 3, "%f", &fPer);
        fPer = (fPer / 100);
        strdef = CHAR_getWorkInt(charaindex, CHAR_WORKFIXSTR);
        strdef = (int)(strdef * fPer);
        CHAR_setWorkInt(charaindex, CHAR_WORKATTACKPOWER, (CHAR_getWorkInt(charaindex, CHAR_WORKFIXSTR) + strdef));
    }

    return TRUE;
}   //BATTLE_COM_S_DAMAGETOHP
#endif

#ifdef _PRO_BATTLEENEMYSKILL
int ENEMYSKILL_ReLife(int enemyindex, int toNo, int array, char *data)
{
    CHAR_setWorkInt(enemyindex, CHAR_WORKBATTLECOM1, BATTLE_COM_S_ENEMYRELIFE);
    CHAR_setWorkInt(enemyindex, CHAR_WORKBATTLECOM2, toNo);
    CHAR_setWorkInt(enemyindex, CHAR_WORKBATTLEMODE, BATTLE_CHARMODE_C_OK);
    return TRUE;
}

int ENEMYSKILL_ReHP(int enemyindex, int toNo, int array, char *data)
{
    CHAR_setWorkInt(enemyindex, CHAR_WORKBATTLECOM1, BATTLE_COM_S_ENEMYREHP);
    CHAR_setWorkInt(enemyindex, CHAR_WORKBATTLECOM2, toNo);
    CHAR_setWorkInt(enemyindex, CHAR_WORKBATTLEMODE, BATTLE_CHARMODE_C_OK);
    return TRUE;
}

int ENEMYSKILL_EnemyHelp(int enemyindex, int toNo, int array, char *data)
{
    CHAR_setWorkInt(enemyindex, CHAR_WORKBATTLECOM1, BATTLE_COM_S_ENEMYHELP);
    CHAR_setWorkInt(enemyindex, CHAR_WORKBATTLECOM2, toNo);
    CHAR_setWorkInt(enemyindex, CHAR_WORKBATTLEMODE, BATTLE_CHARMODE_C_OK);
    return TRUE;
}

#endif

#ifdef _SKILL_DAMAGETOHP    //嗜血技 { "PETSKILL_DamageToHp",    PETSKILL_DamageToHp, 0},
int PETSKILL_DamageToHp(int charaindex, int toNo, int array, char *data)
{
    char *pszOption;
    char buf1[256];
    int strdef;
    float def = 0.00;
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM1, BATTLE_COM_S_DAMAGETOHP);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM2, toNo);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLEMODE, BATTLE_CHARMODE_C_OK);
    CHAR_SETWORKINT_LOW(charaindex, CHAR_WORKBATTLECOM3, array); //记录技能
    pszOption = PETSKILL_getChar(array, PETSKILL_OPTION);

    if (pszOption == "\0")
    {
        return FALSE;
    }

    if (getStringFromIndexWithDelim(pszOption, "|", 1, buf1, sizeof(buf1)) == FALSE)
    {
        return FALSE;
    }

    strdef = CHAR_getWorkInt(charaindex, CHAR_WORKFIXSTR);
    def = (float)atoi(buf1) / 100;
    strdef = (int)(strdef * def);
    CHAR_setWorkInt(charaindex, CHAR_WORKATTACKPOWER, CHAR_getWorkInt(charaindex, CHAR_WORKFIXSTR) - strdef);
    return TRUE;
}
#endif

#ifdef _Skill_MPDAMAGE
int PETSKILL_MpDamage(int charaindex, int toNo, int array, char *data)
{
    char *pszOption;
    char buf1[256];
    int strdef;
    float def = 0.01;
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM1, BATTLE_COM_S_MPDAMAGE);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM2, toNo);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLEMODE, BATTLE_CHARMODE_C_OK);
    CHAR_SETWORKINT_LOW(charaindex, CHAR_WORKBATTLECOM3, array); ////记录技能
    pszOption = PETSKILL_getChar(array, PETSKILL_OPTION);

    if (pszOption == "\0")
    {
        return FALSE;
    }

    if (getStringFromIndexWithDelim(pszOption, "|", 1, buf1, sizeof(buf1)) == FALSE)
    {
        return FALSE;
    }

    def = (float)atoi(buf1) / 100;
    strdef = CHAR_getWorkInt(charaindex, CHAR_WORKFIXSTR);
    strdef = (int)(strdef * def);
    CHAR_setWorkInt(charaindex, CHAR_WORKATTACKPOWER, CHAR_getWorkInt(charaindex, CHAR_WORKFIXSTR) - strdef);
    return TRUE;
}
#endif


#ifdef _SKILL_TOOTH
int PETSKILL_ToothCrushe(int charaindex, int toNo, int array, char *data)
{
    //char *pszOption;
    //char buf1[256];
    //int strdef;
    //float def=0.00;
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM1, BATTLE_COM_S_TOOTHCRUSHE);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM2, toNo);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLEMODE, BATTLE_CHARMODE_C_OK);
    CHAR_SETWORKINT_LOW(charaindex, CHAR_WORKBATTLECOM3, array);     //记录技能
    /*
        pszOption = PETSKILL_getChar( array, PETSKILL_OPTION );
        if( pszOption == "\0" ) return FALSE;
        if( getStringFromIndexWithDelim( pszOption, "|", 1, buf1, sizeof( buf1)) == FALSE )
            return FALSE;

        def = (float)(atoi( buf1)/100);
        strdef = CHAR_getWorkInt(charaindex,CHAR_WORKFIXSTR);
        strdef = strdef - (int)( strdef*def);
        CHAR_setWorkInt( charaindex, CHAR_WORKATTACKPOWER, strdef);
    */
    return TRUE;
}
#endif
#ifdef _PSKILL_MODIFY
int PETSKILL_Modifyattack(int charaindex, int toNo, int array, char *data)
{
    //char *pszOption;
    //char buf1[256];
    //int strdef;
    //float def=0.00;
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM1, BATTLE_COM_S_MODIFYATT);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM2, toNo);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLEMODE, BATTLE_CHARMODE_C_OK);
    CHAR_SETWORKINT_LOW(charaindex, CHAR_WORKBATTLECOM3, array);     //记录技能
    /*
        pszOption = PETSKILL_getChar( array, PETSKILL_OPTION );
        if( pszOption == "\0" ) return FALSE;
        if( getStringFromIndexWithDelim( pszOption, "|", 1, buf1, sizeof( buf1)) == FALSE )
            return FALSE;

        def = (float)(atoi( buf1)/100);
        strdef = CHAR_getWorkInt(charaindex,CHAR_WORKFIXSTR);
        strdef = strdef - (int)( strdef*def);
        CHAR_setWorkInt( charaindex, CHAR_WORKATTACKPOWER, strdef);
    */
    return TRUE;
}
#endif

#ifdef _PSKILL_MDFYATTACK
int PETSKILL_Mdfyattack(int charaindex, int toNo, int array, char *data)
{
    char *pszOption;
    char buf1[256];
    char MNodify[][36] = {"EA", "WA", "FI", "WI"};
    int nums, i;
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM1, BATTLE_COM_S_MDFYATTACK);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM2, toNo);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLEMODE, BATTLE_CHARMODE_C_OK);
    CHAR_SETWORKINT_LOW(charaindex, CHAR_WORKBATTLECOM3, array); //记录属性
    pszOption = PETSKILL_getChar(array, PETSKILL_OPTION);

    if (pszOption == "\0")
    {
        return FALSE;
    }

    if (getStringFromIndexWithDelim(pszOption, "|", 1, buf1, sizeof(buf1)) == FALSE)
    {
        return FALSE;
    }

    for (i = 0; i < 4; i++)
    {
        if (strcmp(buf1, MNodify[i]))
        {
            continue;
        }

        break;
    }

    if (i == 4)
    {
        return FALSE;
    }

    CHAR_SETWORKINT_LOW(charaindex, CHAR_WORKBATTLECOM4, i); //记录属性

    if (getStringFromIndexWithDelim(pszOption, "|", 2, buf1, sizeof(buf1)) == FALSE)
    {
        return FALSE;
    }

    nums = atoi(buf1);
    CHAR_SETWORKINT_HIGH(charaindex, CHAR_WORKBATTLECOM4, nums); //记录数值
    return TRUE;
}
#endif

#ifdef _ITEM_INSLAY
int PETSKILL_Inslay(int index, int toNo, int array, char *data)
{
    int charaindex, petid;
    int inslayindex = -1, inslayno = -1;
    int i, j, itemno;
#define MAXINSLAY 15
    int     itemindexs[MAXINSLAY] = {-1, -1, -1, -1};
    int     itemnos[MAXINSLAY] = {-1, -1, -1, -1};

    if (!CHAR_CHECKINDEX(index))
    {
        return FALSE;
    }

    if (CHAR_getInt(index, CHAR_WHICHTYPE) != CHAR_TYPEPET)
    {
        return FALSE;
    }

    charaindex = CHAR_getWorkInt(index, CHAR_WORKPLAYERINDEX);

    if (!CHAR_CHECKINDEX(charaindex))
    {
        return FALSE;
    }

    if (CHAR_getWorkInt(charaindex, CHAR_WORKBATTLEMODE) != BATTLE_CHARMODE_NONE)
    {
        return FALSE;
    }

    petid = CHAR_getInt(charaindex, CHAR_PETID);

    for (i = 0; i < 15; i ++)
    {
        char buf[256];
        char *buf1;
        int itemindex;

        if (getStringFromIndexWithDelim(data, "|", i + 1, buf,  sizeof(buf)) == FALSE)
        {
            break;
        }

        itemno = atoi(buf);

        if (!CHAR_CHECKITEMINDEX(charaindex, itemno))
        {
            continue;
        }

        itemindex = CHAR_getItemIndex(charaindex, itemno);

        if (!ITEM_CHECKINDEX(itemindex))
        {
            continue;
        }

#ifdef _INSLAY_NOT_PILENUMS

        if (ITEM_getInt(itemindex, ITEM_CANBEPILE) == 1 &&
                ITEM_getInt(itemindex, ITEM_USEPILENUMS) > 1)
        {
            CHAR_talkToCli(charaindex, -1, "叠加物品无法进行精工。", CHAR_COLORYELLOW);
            return FALSE;
        }

#endif

        if (i >= MAXINSLAY)
        {
            CHAR_talkToCli(charaindex, -1, "精工每次只能选择四个物品。", CHAR_COLORYELLOW);
            return FALSE;
        }

        for (j = 0; j < MAXINSLAY; j++)
        {
            if (itemindexs[j] == itemindex)
            {
                print("精工物品index相同 ERROR !!\n");
                return FALSE;
            }
        }

        if ((buf1 = ITEM_getChar(itemindex, ITEM_TYPECODE)) == "\0")
        {
            print(" ITEM_TYPECODE == NULL 错误 !!\n");
            return FALSE;
        }

        if (!strcmp(buf1, "\0") || !strcmp(buf1, "NULL"))
        {
            CHAR_talkToCli(charaindex, -1, "这样物品不适合精工。", CHAR_COLORYELLOW);
            return FALSE;
        }

        if (strstr(buf1, "INSLAY") != 0)
        {
            if (!ITEM_CHECKINDEX(inslayindex))
            {
                inslayindex = itemindex;
                inslayno = itemno;
            }
            else
            {
                CHAR_talkToCli(charaindex, -1, "精工每次只能选择一个武器或防具。", CHAR_COLORYELLOW);
                return FALSE;
            }
        }

        itemindexs[i] = itemindex;
        itemnos[i] = itemno;
    }

    if (!ITEM_CHECKINDEX(inslayindex))
    {
        CHAR_talkToCli(charaindex, -1, "精工必须选择一个武器或防具。", CHAR_COLORYELLOW);
        return FALSE;
    }

#ifdef _EXPANSION_ITEM_INSLAY
    int maxinslay = max(3, min(atoi(PETSKILL_getChar(array, PETSKILL_OPTION)), 25));
#endif

    for (i = 0; i < MAXINSLAY; i++)
    {
        if (!ITEM_CHECKINDEX(itemindexs[i]))
        {
            break;
        }

        if (inslayindex == itemindexs[i])
        {
            continue;
        }

#ifdef _EXPANSION_ITEM_INSLAY

        if (PETSKILL_ITEM_inslay(charaindex, inslayindex, itemindexs[i], maxinslay) == FALSE)
        {
            return FALSE;
        }

#else

        if (PETSKILL_ITEM_inslay(charaindex, inslayindex, itemindexs[i]) == FALSE)
        {
            return FALSE;
        }

#endif
        CHAR_DelItem(charaindex, itemnos[i]);
        /*
                CHAR_setItemIndex( charaindex, itemnos[i] ,-1);
                CHAR_sendItemDataOne( charaindex, itemnos[i] );
                ITEM_endExistItemsOne( itemindexs[i]);
        */
    }

    CHAR_sendItemDataOne(charaindex, inslayno);
    CHAR_complianceParameter(charaindex);
    CHAR_send_P_StatusString(charaindex, CHAR_P_STRING_ATK | CHAR_P_STRING_DEF
                             | CHAR_P_STRING_QUICK | CHAR_P_STRING_CHARM);
    return TRUE;
}

#endif

#ifdef _MAGIC_SUPERWALL
int PETSKILL_MagicStatusChange(int charaindex, int toindex, int array, char *data)
{
    if (CHAR_CHECKINDEX(charaindex) == FALSE)
    {
        return FALSE;
    }

    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM1, BATTLE_COM_S_SUPERWALL);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM2, toindex);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLEMODE, BATTLE_CHARMODE_C_OK);
    CHAR_SETWORKINT_LOW(charaindex, CHAR_WORKBATTLECOM3, array); //记录属性
    return TRUE;
}

#endif

#ifdef _PETSKILL_SETDUCK
int PETSKILL_SetDuck(int charaindex, int toNo, int array, char *data)
{
    if (CHAR_CHECKINDEX(charaindex) == FALSE)
    {
        return FALSE;
    }

    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM1, BATTLE_COM_S_SETDUCK);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM2, toNo);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLEMODE, BATTLE_CHARMODE_C_OK);
    CHAR_SETWORKINT_LOW(charaindex, CHAR_WORKBATTLECOM3, array); //记录属性
    CHAR_setWorkInt(charaindex, CHAR_MAGICPETMP, 0);
    return TRUE;
}
#endif

#ifdef _MAGICPET_SKILL
int PETSKILL_SetMagicPet(int charaindex, int toNo, int array, char *data)
{
    int nums = 0;

    if (CHAR_CHECKINDEX(charaindex) == FALSE)
    {
        return FALSE;
    }

    nums = CHAR_getWorkInt(charaindex, CHAR_MAGICPETMP);

    if (nums >= 3)
    {
        int toindex = CHAR_getWorkInt(charaindex, CHAR_WORKPLAYERINDEX);
        CHAR_talkToCli(toindex, -1, "此技能单场限用三次。", CHAR_COLORYELLOW);
        return FALSE;
    }

    char* pszOption = PETSKILL_getChar(array, PETSKILL_OPTION);

    if (pszOption == "\0")
    {
        return FALSE;
    }

    char buf1[32];

    if (getStringFromIndexWithDelim(pszOption, "|", 3, buf1, sizeof(buf1)) == FALSE)
    {
        return FALSE;
    }

    if (strstr(buf1, "HP") != 0)
    {
        int battleindex = CHAR_getWorkInt(charaindex, CHAR_WORKBATTLEINDEX);

        if (BATTLE_getType(battleindex) == BATTLE_TYPE_P_vs_P)
        {
            return FALSE;
        }
    }

    CHAR_setWorkInt(charaindex, CHAR_MAGICPETMP, nums);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM1, BATTLE_COM_S_SETMAGICPET);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM2, toNo);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLEMODE, BATTLE_CHARMODE_C_OK);
    CHAR_SETWORKINT_LOW(charaindex, CHAR_WORKBATTLECOM3, array); //记录属性
    return TRUE;
}
#endif

#ifdef _PETSKILL_FIXITEM
int PETSKILL_Fixitem(int index, int toNo, int array, char *data)
{
    int charaindex, petid;
    int fixindex = -1, fixno = -1;
    int i, j, itemno;
#define MAXFIXITEM 2
    int     itemindexs[MAXFIXITEM] = {-1, -1};
    int     itemnos[MAXFIXITEM] = {-1, -1};

    if (!CHAR_CHECKINDEX(index))
    {
        return FALSE;
    }

    if (CHAR_getInt(index, CHAR_WHICHTYPE) != CHAR_TYPEPET)
    {
        return FALSE;
    }

    charaindex = CHAR_getWorkInt(index, CHAR_WORKPLAYERINDEX);

    if (!CHAR_CHECKINDEX(charaindex))
    {
        return FALSE;
    }

    if (CHAR_getWorkInt(charaindex, CHAR_WORKBATTLEMODE) != BATTLE_CHARMODE_NONE)
    {
        return FALSE;
    }

    petid = CHAR_getInt(charaindex, CHAR_PETID);

    for (i = 0; i < 15; i ++)
    {
        char buf[256];
        int itemtype = -1;
        int itemindex;

        if (getStringFromIndexWithDelim(data, "|", i + 1, buf,  sizeof(buf)) == FALSE)
        {
            break;
        }

        itemno = atoi(buf);

        if (!CHAR_CHECKITEMINDEX(charaindex, itemno))
        {
            continue;
        }

        itemindex = CHAR_getItemIndex(charaindex, itemno);

        if (!ITEM_CHECKINDEX(itemindex))
        {
            continue;
        }

        if (i >= MAXFIXITEM)
        {
            CHAR_talkToCli(charaindex, -1, "修复每次只能选择两个物品。", CHAR_COLORYELLOW);
            return FALSE;
        }

        for (j = 0; j < MAXFIXITEM; j++)
        {
            if (itemindexs[j] == itemindex)
            {
                print("修复物品index相同 ERROR !!\n");
                return FALSE;
            }
        }

        itemtype = ITEM_getInt(itemindex, ITEM_TYPE);

        if (itemtype == ITEM_DISH)
        {
            CHAR_talkToCli(charaindex, -1, "料理不能做修复。", CHAR_COLORYELLOW);
            return FALSE;
        }

        if ((itemtype >= ITEM_FIST && itemtype <= ITEM_AMULET) ||
                itemtype == ITEM_BOOMERANG || itemtype == ITEM_BOUNDTHROW || itemtype == ITEM_BREAKTHROW
#ifdef _ITEM_EQUITSPACE
                || itemtype == ITEM_WBELT
                || itemtype == ITEM_WSHIELD
                || itemtype == ITEM_WSHOES
#endif
#ifdef _EQUIT_NEWGLOVE
                || itemtype == ITEM_WGLOVE
#endif
#ifdef _PET_ITEM
#ifndef _TAKE_ITEMDAMAGE_FOR_PET
                || (itemtype >= ITEM_PET_HEAD && itemtype <= ITEM_PET_FEET)
#endif
#endif
           )
        {
            if (!ITEM_CHECKINDEX(fixindex))
            {
                fixindex = itemindex;
                fixno = itemno;
            }
            else
            {
                CHAR_talkToCli(charaindex, -1, "修复每次只能选择一个武器或防具。", CHAR_COLORYELLOW);
                return FALSE;
            }
        }

        itemindexs[i] = itemindex;
        itemnos[i] = itemno;
    }

    if (!ITEM_CHECKINDEX(fixindex))
    {
        CHAR_talkToCli(charaindex, -1, "修复必须选择一个武器或防具。", CHAR_COLORYELLOW);
        return FALSE;
    }

    if (PETSKILL_ITEM_FixItem(charaindex, fixindex, itemindexs) == FALSE)
    {
        return FALSE;
    }
    else
    {
        for (i = 0; i < MAXFIXITEM; i++)
        {
            if (!ITEM_CHECKINDEX(itemindexs[i]))
            {
                break;
            }

            if (fixindex == itemindexs[i])
            {
                continue;
            }

            CHAR_setItemIndex(charaindex, itemnos[i], -1);
            CHAR_sendItemDataOne(charaindex, itemnos[i]);
            ITEM_endExistItemsOne(itemindexs[i]);
        }
    }

    CHAR_sendItemDataOne(charaindex, fixno);
    CHAR_complianceParameter(charaindex);
    CHAR_send_P_StatusString(charaindex, CHAR_P_STRING_ATK | CHAR_P_STRING_DEF
                             | CHAR_P_STRING_QUICK | CHAR_P_STRING_CHARM);
    CHAR_talkToCli(charaindex, -1, "修复完成。", CHAR_COLORYELLOW);
    return TRUE;
}
#endif

#ifdef _PETSKILL_TIMID
int PETSKILL_BattleTimid(int charaindex, int toNo, int array, char *data)
{
    if (!CHAR_CHECKINDEX(charaindex))
    {
        return FALSE;
    }

    if (CHAR_getInt(charaindex, CHAR_WHICHTYPE) == CHAR_TYPEPLAYER)
    {
        return FALSE;
    }

    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM1, BATTLE_COM_S_TIMID);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM2, toNo);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLEMODE, BATTLE_CHARMODE_C_OK);
    CHAR_setWorkInt(charaindex, CHAR_WORKATTACKPOWER, (CHAR_getWorkInt(charaindex, CHAR_WORKFIXSTR) * 0.7));
    CHAR_setWorkInt(charaindex, CHAR_WORKDEFENCEPOWER, (CHAR_getWorkInt(charaindex, CHAR_WORKFIXTOUGH) * 0.4));
    CHAR_setWorkInt(charaindex, CHAR_WORKQUICK, (CHAR_getWorkInt(charaindex, CHAR_WORKFIXDEX) * 0.8));
    CHAR_SETWORKINT_LOW(charaindex, CHAR_WORKBATTLECOM3, array); //记录属性
    return TRUE;
}
#endif
#ifdef _PETSKILL_2TIMID
int PETSKILL_2BattleTimid(int charaindex, int toNo, int array, char *data)
{
    char *pszOption = "\0";
    char *pszP;
    float fPer = 0;

    if (!CHAR_CHECKINDEX(charaindex))
    {
        return FALSE;
    }

    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM1, BATTLE_COM_S_2TIMID);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM2, toNo);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLEMODE, BATTLE_CHARMODE_C_OK);
    pszOption = PETSKILL_getChar(array, PETSKILL_OPTION);

    if (pszOption == "\0")
    {
        return FALSE;
    }

    if ((pszP = strstr(pszOption, "-攻%")) != NULL)
    {
        sscanf(pszP + 4, "%f", &fPer);
        fPer = (fPer / 100);
        CHAR_setWorkInt(charaindex, CHAR_WORKATTACKPOWER, (CHAR_getWorkInt(charaindex, CHAR_WORKFIXSTR)*fPer));
    }

    if ((pszP = strstr(pszOption, "+攻%")) != NULL)
    {
        sscanf(pszP + 4, "%f", &fPer);
        fPer = (fPer / 100);
        CHAR_setWorkInt(charaindex, CHAR_WORKATTACKPOWER, CHAR_getWorkInt(charaindex, CHAR_WORKFIXSTR) + (CHAR_getWorkInt(charaindex, CHAR_WORKFIXSTR)*fPer));
    }

    if ((pszP = strstr(pszOption, "-防%")) != NULL)
    {
        sscanf(pszP + 4, "%f", &fPer);
        fPer = (fPer / 100);
        CHAR_setWorkInt(charaindex, CHAR_WORKDEFENCEPOWER, (CHAR_getWorkInt(charaindex, CHAR_WORKFIXTOUGH)*fPer));
    }

    if ((pszP = strstr(pszOption, "+防%")) != NULL)
    {
        sscanf(pszP + 4, "%f", &fPer);
        fPer = (fPer / 100);
        CHAR_setWorkInt(charaindex, CHAR_WORKDEFENCEPOWER, CHAR_getWorkInt(charaindex, CHAR_WORKFIXTOUGH) + (CHAR_getWorkInt(charaindex, CHAR_WORKFIXTOUGH)*fPer));
    }

    if ((pszP = strstr(pszOption, "-敏%")) != NULL)
    {
        sscanf(pszP + 4, "%f", &fPer);
        fPer = (fPer / 100);
        CHAR_setWorkInt(charaindex, CHAR_WORKQUICK, (CHAR_getWorkInt(charaindex, CHAR_WORKFIXDEX)*fPer));
    }

    if ((pszP = strstr(pszOption, "+敏%")) != NULL)
    {
        sscanf(pszP + 4, "%f", &fPer);
        fPer = (fPer / 100);
        CHAR_setWorkInt(charaindex, CHAR_WORKQUICK, CHAR_getWorkInt(charaindex, CHAR_WORKFIXDEX) + (CHAR_getWorkInt(charaindex, CHAR_WORKFIXDEX)*fPer));
    }

    CHAR_SETWORKINT_LOW(charaindex, CHAR_WORKBATTLECOM3, array); //记录属性
    return TRUE;
}
#endif
#ifdef _PETSKILL_ANTINTER
int PETSKILL_AntInter(int charaindex, int toNo, int array, char* data)
{
    if (!CHAR_CHECKINDEX(charaindex))
    {
        return FALSE;
    }

    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM1, BATTLE_COM_S_ANTINTER);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM2, toNo);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLEMODE, BATTLE_CHARMODE_C_OK);
    CHAR_SETWORKINT_LOW(charaindex, CHAR_WORKBATTLECOM3, array); //记录属性
    return TRUE;
}
#endif

#ifdef _RESURRECTION_PETSKILL
int PETSKILL_Resurrection(int charaindex, int toNo, int array, char* data)
{
    if (!CHAR_CHECKINDEX(charaindex))
    {
        return FALSE;
    }

    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM1, BATTLE_COM_S_RESURRECTION);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM2, toNo);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLEMODE, BATTLE_CHARMODE_C_OK);
    CHAR_SETWORKINT_LOW(charaindex, CHAR_WORKBATTLECOM3, array); //记录属性
    return TRUE;
}
#endif

#ifdef _GRAPPLING_PETSKILL
int PETSKILL_Grappling(int charaindex, int toNo, int array, char* data)
{
    if (!CHAR_CHECKINDEX(charaindex))
    {
        return FALSE;
    }

    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM1, BATTLE_COM_S_GRAPPLING);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM2, toNo);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLEMODE, BATTLE_CHARMODE_C_OK);
    CHAR_SETWORKINT_LOW(charaindex, CHAR_WORKBATTLECOM3, array); //记录属性
    return TRUE;
}
#endif

#ifdef _LOSTLOST_PETSKILL
int PETSKILL_Lostlost(int charaindex, int toNo, int array, char* data)
{
    int nums = 0;

    if (CHAR_CHECKINDEX(charaindex) == FALSE)
    {
        return FALSE;
    }

    nums = CHAR_getWorkInt(charaindex, CHAR_WORK_LOSTLOST);
    char *skillarg = PETSKILL_getChar(array, PETSKILL_OPTION);

    if (nums >= max(1, atoi(skillarg)))
    {
        int toindex = CHAR_getWorkInt(charaindex, CHAR_WORKPLAYERINDEX);
        char buf[256];
        sprintf(buf, "此技能单场限用 %d 次。", max(1, atoi(skillarg)));
        CHAR_talkToCli(toindex, -1, buf, CHAR_COLORYELLOW);
        return FALSE;
    }

    CHAR_setWorkInt(charaindex, CHAR_WORK_LOSTLOST, ++nums);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM1, BATTLE_COM_S_LOSTLOST);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM2, toNo);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLEMODE, BATTLE_CHARMODE_C_OK);
    CHAR_SETWORKINT_LOW(charaindex, CHAR_WORKBATTLECOM3, array); //记录属性
    return TRUE;
}
#endif

#ifdef _PETSKILL_PROPERTY
int PETSKILL_BattleProperty(int charaindex, int toNo, int array, char *data)
{
    if (!CHAR_CHECKINDEX(charaindex))
    {
        return FALSE;
    }

    if (CHAR_getInt(charaindex, CHAR_WHICHTYPE) == CHAR_TYPEPLAYER)
    {
        return FALSE;
    }

    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM1, BATTLE_COM_S_PROPERTYSKILL);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM2, toNo);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLEMODE, BATTLE_CHARMODE_C_OK);
//  CHAR_setWorkInt( charaindex, CHAR_WORKATTACKPOWER, ( CHAR_getWorkInt( charaindex, CHAR_WORKFIXSTR)*0.7) );
//  CHAR_setWorkInt( charaindex, CHAR_WORKDEFENCEPOWER, ( CHAR_getWorkInt( charaindex, CHAR_WORKFIXTOUGH)*0.5) );
//  CHAR_setWorkInt( charaindex, CHAR_WORKQUICK, ( CHAR_getWorkInt( charaindex, CHAR_WORKFIXDEX)*0.95) );
    CHAR_SETWORKINT_LOW(charaindex, CHAR_WORKBATTLECOM3, array); //记录属性
    return TRUE;
}
#endif

#ifdef _PETSKILL_TEAR
int PETSKILL_BattleTearDamage(int charaindex, int toNo, int array, char *data)
{
    if (!CHAR_CHECKINDEX(charaindex))
    {
        return FALSE;
    }

    if (CHAR_getInt(charaindex, CHAR_WHICHTYPE) == CHAR_TYPEPLAYER)
    {
        return FALSE;
    }

    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM1, BATTLE_COM_S_PETSKILLTEAR);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM2, toNo);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLEMODE, BATTLE_CHARMODE_C_OK);
    CHAR_setWorkInt(charaindex, CHAR_WORKATTACKPOWER, (CHAR_getWorkInt(charaindex, CHAR_WORKFIXSTR) * 0.9));
    CHAR_setWorkInt(charaindex, CHAR_WORKDEFENCEPOWER, (CHAR_getWorkInt(charaindex, CHAR_WORKFIXTOUGH) * 0.3));
    CHAR_SETWORKINT_LOW(charaindex, CHAR_WORKBATTLECOM3, array); //记录属性
    return TRUE;
}
#endif


#ifdef _BATTLE_LIGHTTAKE
int PETSKILL_Lighttakeed(int charaindex, int toNo, int array, char *data)
{
    if (!CHAR_CHECKINDEX(charaindex))
    {
        return FALSE;
    }

    if (CHAR_getInt(charaindex, CHAR_WHICHTYPE) == CHAR_TYPEPLAYER)
    {
        return FALSE;
    }

    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM1, BATTLE_COM_S_LIGHTTAKE);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM2, toNo);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLEMODE, BATTLE_CHARMODE_C_OK);
    CHAR_setWorkInt(charaindex, CHAR_WORKATTACKPOWER, (CHAR_getWorkInt(charaindex, CHAR_WORKFIXSTR) * 0.7));
    CHAR_setWorkInt(charaindex, CHAR_WORKDEFENCEPOWER, (CHAR_getWorkInt(charaindex, CHAR_WORKFIXTOUGH) * 0.5));
//  CHAR_setWorkInt( charaindex, CHAR_WORKQUICK, (CHAR_getWorkInt( charaindex, CHAR_WORKFIXDEX)*0.95) );
    CHAR_SETWORKINT_LOW(charaindex, CHAR_WORKBATTLECOM3, array); //记录属性
    return TRUE;
}
#endif

#ifdef _BATTLE_ATTCRAZED
int PETSKILL_AttackCrazed(int charaindex, int toNo, int array, char *data)
{
    char *pszOption = "\0";

    if (!CHAR_CHECKINDEX(charaindex))
    {
        return FALSE;
    }

    if (CHAR_getInt(charaindex, CHAR_WHICHTYPE) == CHAR_TYPEPLAYER)
    {
        return FALSE;
    }

    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM1, BATTLE_COM_S_ATTCRAZED);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM2, toNo);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLEMODE, BATTLE_CHARMODE_C_OK);
    CHAR_setWorkInt(charaindex, CHAR_WORKATTACKPOWER, (CHAR_getWorkInt(charaindex, CHAR_WORKFIXSTR) * 0.8));
    CHAR_setWorkInt(charaindex, CHAR_WORKDEFENCEPOWER, (CHAR_getWorkInt(charaindex, CHAR_WORKFIXTOUGH) * 0.7));
    pszOption = PETSKILL_getChar(array, PETSKILL_OPTION);

    if (pszOption == "\0")
    {
        return FALSE;
    }

    CHAR_SETWORKINT_LOW(charaindex, CHAR_WORKBATTLECOM3, array); //记录属性
    CHAR_SETWORKINT_HIGH(charaindex, CHAR_WORKBATTLECOM3, atoi(pszOption));
    return TRUE;
}
#endif

#ifdef _SHOOTCHESTNUT   // Syu ADD 宠技：丢栗子
int PETSKILL_AttackShoot(int charaindex, int toNo, int array, char *data)
{
    //int n = RAND ( 3 , 5 ) ;
    int n ;
    int loyal = CHAR_getWorkInt(charaindex, CHAR_WORKFIXAI);
    char bb1[16], bb2[16] ;
    char *buf = "\0" ;

    if (!CHAR_CHECKINDEX(charaindex))
    {
        return FALSE;
    }

    if (CHAR_getInt(charaindex, CHAR_WHICHTYPE) == CHAR_TYPEPLAYER)
    {
        return FALSE;
    }

    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM1, BATTLE_COM_S_ATTSHOOT);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM2, toNo);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLEMODE, BATTLE_CHARMODE_C_OK);
    buf = PETSKILL_getChar(array, PETSKILL_OPTION) ;

    if (buf == "\0")
    {
        return FALSE;
    }

    getStringFromIndexWithDelim(buf, "|", 1, bb1, sizeof(bb1));
    getStringFromIndexWithDelim(buf, "|", 2, bb2, sizeof(bb2));
    n = RAND(atoi(bb1), atoi(bb2)) ;

    if (loyal >= 100)
    {
        if (RAND(1, 300) > 299)
        {
            n = 8 ;
        }
        else if ((CHAR_getInt(charaindex, CHAR_HP)) < 20 && (RAND(1, 50) > 49))
        {
            n = 8 ;
        }
    }

    /*
        if ( n != 8 )
            CHAR_setWorkInt( charaindex, CHAR_WORKATTACKPOWER, (CHAR_getWorkInt( charaindex, CHAR_WORKFIXSTR) * 1.2 / n ) );
        else
            CHAR_setWorkInt( charaindex, CHAR_WORKATTACKPOWER, (CHAR_getWorkInt( charaindex, CHAR_WORKFIXSTR) * 1.2 / 4 ) );
        CHAR_setWorkInt( charaindex, CHAR_WORKDEFENCEPOWER, (CHAR_getWorkInt( charaindex, CHAR_WORKFIXTOUGH) * 1) );
    */
    CHAR_SETWORKINT_LOW(charaindex, CHAR_WORKBATTLECOM3, array); //记录属性
    CHAR_SETWORKINT_HIGH(charaindex, CHAR_WORKBATTLECOM3, n);
    return TRUE;
}
#endif

#ifdef _PET_SKILL_SARS              // WON ADD 毒煞蔓延
int PETSKILL_Sars(int charaindex, int toNo, int array, char *data)
{
    char *pszOption, *pszP;
    int status = -1, i = 0, turn = 3;
    char szTurn[] = "turn";
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM1, BATTLE_COM_S_SARS);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM2, toNo);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLEMODE, BATTLE_CHARMODE_C_OK);
    pszOption = PETSKILL_getChar(array, PETSKILL_OPTION);
    pszP = pszOption;

    for (; status == -1 && pszP[0] != 0; pszP++)
    {
        for (i = 1; i < min(BATTLE_ST_END, 32); i ++)
        {
            if (strncmp(pszP, aszStatus[i], 2) == 0)
            {
                status = i;
                pszP += 2;
                break;
            }
        }
    }

    if ((pszP = strstr(pszP, szTurn)) != NULL)
    {
        pszP += sizeof(szTurn);
        sscanf(pszP, "%d", &turn);
    }

    CHAR_SETWORKINT_LOW(charaindex, CHAR_WORKBATTLECOM3, status);
    CHAR_SETWORKINT_HIGH(charaindex, CHAR_WORKBATTLECOM3, turn);
    return TRUE;
}
#endif

#ifdef _SONIC_ATTACK                // WON ADD 音波攻击
int PETSKILL_Sonic(int charaindex, int toNo, int array, char *data)
{
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM1, BATTLE_COM_S_SONIC);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM2, toNo);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLEMODE, BATTLE_CHARMODE_C_OK);
    CHAR_SETWORKINT_LOW(charaindex, CHAR_WORKBATTLECOM3, array);     //记录属性
    return TRUE;
}
#endif
#ifdef _PETSKILL_REGRET
int PETSKILL_Regret(int charaindex, int toNo, int array, char *data)
{
    char *pszOption, *pszP;
    int strdef;
    float fPer = 0.01;
    pszOption = PETSKILL_getChar(array, PETSKILL_OPTION);

    if ((pszP = strstr(pszOption, "攻%")) != NULL)
    {
        sscanf(pszP + 3, "%f", &fPer);
        fPer = (fPer / 100);
        strdef = CHAR_getWorkInt(charaindex, CHAR_WORKFIXSTR);
        strdef = (int)(strdef * fPer);
        CHAR_setWorkInt(charaindex, CHAR_WORKATTACKPOWER, (CHAR_getWorkInt(charaindex, CHAR_WORKFIXSTR) + strdef));
    }

    if ((pszP = strstr(pszOption, "防%")) != NULL)
    {
        sscanf(pszP + 3, "%f", &fPer);
        fPer = (fPer / 100);
        strdef = CHAR_getWorkInt(charaindex, CHAR_WORKFIXTOUGH);
        strdef = (int)(strdef * fPer);
        CHAR_setWorkInt(charaindex, CHAR_WORKDEFENCEPOWER, (CHAR_getWorkInt(charaindex, CHAR_WORKFIXTOUGH) + strdef));
    }

    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM1, BATTLE_COM_S_REGRET);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM2, toNo);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLEMODE, BATTLE_CHARMODE_C_OK);
    CHAR_SETWORKINT_LOW(charaindex, CHAR_WORKBATTLECOM3, array);     //记录属性
    return TRUE;
}
#endif

#ifdef _PETSKILL_GYRATE
int PETSKILL_Gyrate(int charaindex, int toNo, int array, char *data)
{
    char *pszOption = "\0", *pszP;
    float fPer = 0.01;
    int strdef = 0;
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM1, BATTLE_COM_S_GYRATE);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM2, toNo);   //攻击对象  23,24,25,26 为单排
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLEMODE, BATTLE_CHARMODE_C_OK);
    pszOption = PETSKILL_getChar(array, PETSKILL_OPTION);

    if (pszOption == "\0")
    {
        print("\n pszOption == NULL ");
        return FALSE;
    }

    //读取攻值
    if ((pszP = strstr(pszOption, "攻%")) != NULL)
    {
        sscanf(pszP + 3, "%f", &fPer);
        fPer = (fPer / 100);
        //设定修正值
        strdef = CHAR_getWorkInt(charaindex, CHAR_WORKFIXSTR);
        strdef = (int)(strdef * fPer);
        CHAR_setWorkInt(charaindex, CHAR_WORKATTACKPOWER, (CHAR_getWorkInt(charaindex, CHAR_WORKFIXSTR) + strdef));
    }

    CHAR_SETWORKINT_LOW(charaindex, CHAR_WORKBATTLECOM3, array);     //记录属性
    return TRUE;
}
#endif

#ifdef _PETSKILL_ACUPUNCTURE
int PETSKILL_Acupuncture(int charaindex, int toNo, int array, char *data)
{
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM1, BATTLE_COM_S_ACUPUNCTURE);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM2, toNo);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLEMODE, BATTLE_CHARMODE_C_OK);
    CHAR_SETWORKINT_LOW(charaindex, CHAR_WORKBATTLECOM3, array);     //记录属性
    return TRUE;
}
#endif

#ifdef _PETSKILL_RETRACE
int PETSKILL_Retrace(int charaindex, int toNo, int array, char *data)
{
    //char *pszOption="\0", *pszP;
    //float fPer = 0.01;
    //int strdef=0;
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM1, BATTLE_COM_S_RETRACE);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM2, toNo);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLEMODE, BATTLE_CHARMODE_C_OK);
    /*  pszOption = PETSKILL_getChar( array, PETSKILL_OPTION );

        if( pszOption == "\0" ) {
            print("\n pszOption == NULL ");
            return FALSE;
        }

        //读取攻值
        if( (pszP = strstr( pszOption, "攻%" ) ) != NULL ){
            sscanf( pszP+3, "%f", &fPer );
            fPer=(fPer/100);
            //设定修正值
            strdef=CHAR_getWorkInt(charaindex,CHAR_WORKFIXSTR);
            strdef=(int)(strdef * fPer);
            CHAR_setWorkInt(charaindex,CHAR_WORKATTACKPOWER,(CHAR_getWorkInt(charaindex,CHAR_WORKFIXSTR)+strdef));
        }
    */
    CHAR_SETWORKINT_LOW(charaindex, CHAR_WORKBATTLECOM3, array);     //记录属性
    return TRUE;
}
#endif

#ifdef _PETSKILL_HECTOR
int PETSKILL_Hector(int charaindex, int toNo, int array, char *data)
{
    char *pszOption = "\0", *pszP;
    float fPer = 0.01;
    int strdef;
    //char szTurn[] = "turn";
    int d_dep;
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM1, BATTLE_COM_S_HECTOR);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM2, toNo);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLEMODE, BATTLE_CHARMODE_C_OK);
    pszOption = PETSKILL_getChar(array, PETSKILL_OPTION);

    if (pszOption == "\0")
    {
        print("\n pszOption == NULL ");
        return FALSE;
    }

    //读取攻值
    if ((pszP = strstr(pszOption, "攻%")) != NULL)
    {
        sscanf(pszP + 3, "%f", &fPer);
        fPer = (fPer / 100);
        //设定修正值
        strdef = CHAR_getWorkInt(charaindex, CHAR_WORKFIXSTR);
        strdef = (int)(strdef * fPer);
        CHAR_setWorkInt(charaindex, CHAR_WORKATTACKPOWER, (CHAR_getWorkInt(charaindex, CHAR_WORKFIXSTR) + strdef));
    }

    if ((pszP = strstr(pszOption, "敏%")) != NULL)
    {
        sscanf(pszP + 3, "%f", &fPer);
        CHAR_setWorkInt(charaindex, CHAR_SKILLDEXPOWER, fPer);
        fPer = fPer / 100;
        d_dep = CHAR_getWorkInt(charaindex, CHAR_WORKFIXDEX);
        d_dep = (int)(d_dep * fPer);
        CHAR_setWorkInt(charaindex, CHAR_WORKQUICK, CHAR_getWorkInt(charaindex, CHAR_WORKFIXDEX) + d_dep);
    }

    CHAR_SETWORKINT_LOW(charaindex, CHAR_WORKBATTLECOM3, array);     //记录属性
    return TRUE;
}
#endif

#ifdef _PETSKILL_FIREKILL
int PETSKILL_Firekill(int charaindex, int toNo, int array, char *data)
{
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM1, BATTLE_COM_S_FIREKILL);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM2, toNo);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLEMODE, BATTLE_CHARMODE_C_OK);
    CHAR_SETWORKINT_LOW(charaindex, CHAR_WORKBATTLECOM3, array);     //记录属性
    return TRUE;
}
#endif

#ifdef _PETSKILL_DAMAGETOHP
int PETSKILL_DamageToHp2(int charaindex, int toNo, int array, char *data)
{
    if (CHAR_getInt(charaindex, CHAR_HP) * 2 >= CHAR_getWorkInt(charaindex, CHAR_WORKMAXHP))
    {
        CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM1, BATTLE_COM_GUARD);
        CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM2, toNo);
        CHAR_setWorkInt(charaindex, CHAR_WORKBATTLEMODE, BATTLE_CHARMODE_C_OK);
    }
    else
    {
        CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM1, BATTLE_COM_S_DAMAGETOHP2);
        CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM2, toNo);
        CHAR_setWorkInt(charaindex, CHAR_WORKBATTLEMODE, BATTLE_CHARMODE_C_OK);
        CHAR_SETWORKINT_LOW(charaindex, CHAR_WORKBATTLECOM3, array);    //记录技能
        /*  pszOption = PETSKILL_getChar( array, PETSKILL_OPTION );

            if( pszOption == "\0" ) return FALSE;

            strdef = CHAR_getWorkInt(charaindex,CHAR_WORKFIXSTR);
            def = (atoi( pszOption)/100);
            strdef = strdef - (int)(strdef*def);
            CHAR_setWorkInt( charaindex, CHAR_WORKATTACKPOWER, strdef);
            */
    }

    return TRUE;
}
#endif

#ifdef _PETSKILL_BECOMEFOX
int PETSKILL_BecomeFox(int charaindex, int toNo, int array, char* data)
{
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM1, BATTLE_COM_S_BECOMEFOX);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM2, toNo);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLEMODE, BATTLE_CHARMODE_C_OK);
    CHAR_SETWORKINT_LOW(charaindex, CHAR_WORKBATTLECOM3, array); //记录技能
    return TRUE;
}
#endif

#ifdef _PETSKILL_BECOMEPIG
int PETSKILL_BecomePig(int charaindex, int toNo, int array, char* data)
{
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM1, BATTLE_COM_S_BECOMEPIG);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM2, toNo);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLEMODE, BATTLE_CHARMODE_C_OK);
    CHAR_SETWORKINT_LOW(charaindex, CHAR_WORKBATTLECOM3, array);     //记录属性
    return TRUE;
}
#endif

#ifdef _PETSKILL_LER
int PETSKILL_BatFly(int charaindex, int toNo, int array, char* data)
{
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM1, BATTLE_COM_S_BAT_FLY);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM2, toNo);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLEMODE, BATTLE_CHARMODE_C_OK);
    CHAR_SETWORKINT_LOW(charaindex, CHAR_WORKBATTLECOM3, array);
    return TRUE;
}

int PETSKILL_DivideAttack(int charaindex, int toNo, int array, char* data)
{
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM1, BATTLE_COM_S_DIVIDE_ATTACK);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM2, toNo);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLEMODE, BATTLE_CHARMODE_C_OK);
    CHAR_SETWORKINT_LOW(charaindex, CHAR_WORKBATTLECOM3, array);
    return TRUE;
}
#endif

#ifdef _PETSKILL_BATTLE_MODEL
int PETSKILL_BattleModel(int charaindex, int toindex, int array, char* data)
{
    float fPer;
    int i, iType, iObjectNum, iValue;
    int iAddPowerType[3] = {CHAR_WORKATTACKPOWER, CHAR_WORKDEFENCEPOWER, CHAR_WORKQUICK};
    char *pszOption = "\0", szData[32], szData2[32];
    char szWord[3][3] = {"攻", "防", "敏"};
    pszOption = PETSKILL_getChar(array, PETSKILL_OPTION);

    if (pszOption == "\0")
    {
        printf("PETSKILL_BattleModel: 读取 PETSKILL_OPTION 错误!!(数组:%d,文件:%s,第%d行)\n", array, __FILE__, __LINE__);
        return FALSE;
    }

    // 取得攻击类型
    if (getStringFromIndexWithDelim(pszOption, "|", 1, szData, sizeof(szData)) == FALSE)
    {
        printf("PETSKILL_BattleModel: no type data!!(文件:%s,第%d行)\n", __FILE__, __LINE__);
        return FALSE;
    }

    iType = atoi(szData);

    // 取得攻击物件数量
    if (getStringFromIndexWithDelim(pszOption, "|", 2, szData, sizeof(szData)) == FALSE)
    {
        printf("PETSKILL_BattleModel: no object number data!!(文件:%s,第%d行)\n", __FILE__, __LINE__);
        return FALSE;
    }

    iObjectNum = atoi(szData);

    if (iObjectNum <= 0)
    {
        iObjectNum = RAND(1, 10);
    }
    else if (iObjectNum > 10)
    {
        iObjectNum = 10;
    }

    // 取得能力增减
    if (getStringFromIndexWithDelim(pszOption, "|", 6, szData, sizeof(szData)) != FALSE)
    {
        for (i = 0; i < 3; i++)
        {
            if (getStringFromIndexWithDelim(szData, " ", i + 1, szData2, sizeof(szData2)) != FALSE)
            {
                // 检查设定
                if (strstr(szData2, szWord[i]) != NULL)
                {
                    iValue = CHAR_getWorkInt(charaindex, CHAR_WORKATTACKPOWER);

                    // 有带 "%" 的设定 ex: "攻%"
                    if (strstr(szData2, "%") != NULL)
                    {
                        sscanf(szData2 + 3, "%f", &fPer);
                        fPer = (fPer / 100);
                        iValue += (int)(iValue * fPer);
                    }
                    // 没带 "%" 的设定 ex: "攻"
                    else
                    {
                        sscanf(szData2 + 2, "%f", &fPer);
                        iValue = (int)fPer;
                    }

                    CHAR_setWorkInt(charaindex, iAddPowerType[i], iValue);
                }
            }
            else
            {
                break;
            }
        }
    }

    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLEMODE, BATTLE_CHARMODE_C_OK);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM1, BATTLE_COM_S_BATTLE_MODEL);
    //CHAR_SETWORKINT_LOW(charaindex, CHAR_WORKBATTLECOM2, iType);            // 记录攻击类型
    //CHAR_SETWORKINT_HIGH(charaindex, CHAR_WORKBATTLECOM2, iObjectNum);  // 记录攻击物件数量
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM3, array);
    return TRUE;
}
#endif

#ifdef _PETSKILL_SHOWMERCY
int PETSKILL_ShowMercy(int charaindex, int toNo, int array, char* data)
{
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM1, BATTLE_COM_S_SHOWMERCY);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM2, toNo);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLEMODE, BATTLE_CHARMODE_C_OK);
    return TRUE;
}
#endif

#ifdef _PETSKILL_COMBINED
int PETSKILL_Combined(int charaindex, int toNo, int array, char* data)
{
    char *pszOption = "\0";
    char combined[32];
    int kill[10], count = 0, i;
    char killstr[32], countstr[32];
    pszOption = PETSKILL_getChar(array, PETSKILL_OPTION);

    if (pszOption == "\0")
    {
        return FALSE;
    }

    strcpy(combined, " ");
    getStringFromIndexWithDelim(pszOption, "|", 1, combined, sizeof(combined));

    if (strcmp(combined, "综合法") == 0)
    {
        if (getStringFromIndexWithDelim(pszOption, "|", 2, countstr, sizeof(countstr)) == FALSE)
        {
            return FALSE;
        }

        count = atoi(countstr);

        if (count > 10)
        {
            count = 10;
        }

        for (i = 0; i < count; i++)
        {
            if (getStringFromIndexWithDelim(pszOption, "|", 3 + i, killstr, sizeof(killstr)))
            {
                kill[i] = atoi(killstr);
            }
        }
    }

    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM2, toNo);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM1, BATTLE_COM_JYUJYUTU);
    CHAR_SETWORKINT_LOW(charaindex, CHAR_WORKBATTLECOM3, kill[rand() % count]);
    CHAR_SETWORKINT_HIGH(charaindex, CHAR_WORKBATTLECOM3, 0);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLEMODE, BATTLE_CHARMODE_C_OK);
    return TRUE;
}

int PETSKILL_Combined2(int charaindex, int toNo, int array, char* data)
{
    char *pszOption = "\0";
    char combined[32];
    int kill[10], count = 0, i;
    char killstr[32], countstr[32];
    pszOption = PETSKILL_getChar(array, PETSKILL_OPTION);

    if (pszOption == "\0")
    {
        return FALSE;
    }

    strcpy(combined, " ");
    getStringFromIndexWithDelim(pszOption, "|", 1, combined, sizeof(combined));

    if (strcmp(combined, "综合法") == 0)
    {
        if (getStringFromIndexWithDelim(pszOption, "|", 2, countstr, sizeof(countstr)) == FALSE)
        {
            return FALSE;
        }

        count = atoi(countstr);

        if (count > 10)
        {
            count = 10;
        }

        for (i = 0; i < count; i++)
        {
            if (getStringFromIndexWithDelim(pszOption, "|", 3 + i, killstr, sizeof(killstr)))
            {
                kill[i] = atoi(killstr);
            }
        }
    }

    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM2, toNo);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM1, BATTLE_COM_JYUJYUTU2);
    CHAR_SETWORKINT_LOW(charaindex, CHAR_WORKBATTLECOM3, kill[rand() % count]);
    CHAR_SETWORKINT_HIGH(charaindex, CHAR_WORKBATTLECOM3, 0);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLEMODE, BATTLE_CHARMODE_C_OK);
    return TRUE;
}
#endif

#ifdef _PETSKILL_RIDE
int PETSKILL_Ride(int charaindex, int toNo, int array, char *data)
{
    if (!CHAR_CHECKINDEX(charaindex))
    {
        return FALSE;
    }

    if (CHAR_getInt(charaindex, CHAR_WHICHTYPE) == CHAR_TYPEPLAYER)
    {
        return FALSE;
    }

    if (toNo > 19)
    {
        return FALSE;
    }

    int ownindex = CHAR_getWorkInt(charaindex, CHAR_WORKPLAYERINDEX);

    if (!CHAR_CHECKINDEX(ownindex))
    {
        return FALSE;
    }

    int ridedefidx = BATTLE_No2Index(CHAR_getWorkInt(ownindex, CHAR_WORKBATTLEINDEX), toNo);

    if (ridedefidx != ownindex)
    {
        return FALSE;
    }

    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM1, BATTLE_COM_S_RIDE);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM2, toNo);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLEMODE, BATTLE_CHARMODE_C_OK);
    CHAR_SETWORKINT_LOW(charaindex, CHAR_WORKBATTLECOM3, array); //记录属性
    return TRUE;
}
#endif

#ifdef _PETSKILL_TEMPTATION    //水漾诱惑 
int PETSKILL_temptation(int charaindex, int toNo, int array, char* data)
{
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM1, BATTLE_COM_S_TEMPTATION);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLECOM2, toNo);
    CHAR_setWorkInt(charaindex, CHAR_WORKBATTLEMODE, BATTLE_CHARMODE_C_OK);
    CHAR_SETWORKINT_LOW(charaindex, CHAR_WORKBATTLECOM3, array);
    return TRUE;
}
#endif

