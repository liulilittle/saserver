#ifndef __ALLBLUES_H__
#define __ALLBLUES_H__

#include "longzoro.h"
#define CHARDEFAULTSEESIZ 26
#define MAPCHARDEFALUTSEESIZ 30

//˽���汾���� ----------------------------------------
//#define   _NEW_STONEAGE_TYPE  //�°汾
//#define _175SA_
#if _ATTESTAION_ID == 1
#define _VERSION_NEW
//#define _VERSION_NEW60
//#define _VERSION_GF
#else
#if _ATTESTAION_ID == 2
#define _VERSION_25
#else
#define _VERSION_80
#endif
#endif
//ɽ��begin
#define  NEWDISPATC
#define _OFFLINE_SYSTEM_FORMVENDOR       // ���߰�ֱ̯���˳�
#define FUNC_DU_PKCF
#define _NEW_FONT//��Ӧ������ɫ���Ӻ����Ӧ�ͻ���

//������رմ󲿷ֵ�¼��Ϣ
#define _STOPLOGININFORMATION
#define _DUELTIME24HOUR//�ӳٿ�սʱ��24Сʱ
//���ϵ�version�����ȥ��
//��������κ� ���κ�ȥ��
#define _ASSESS_SYSDOWNUPNEWS//����ϴ�������Ϣ �رպ����
//20150827�޸�
//#ifdef _ITEM_PET_LOCKED
//ch.data[CHAR_LOCKED] = 0;//ԭʼ��-1�������˺Ű�ȫ��
//#endif
//ɽ��end
#ifdef _VERSION_80
#if _ATTESTAION_ID == 0 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 8 || _ATTESTAION_ID == 21 || _ATTESTAION_ID == 23 || _ATTESTAION_ID == 28 || _ATTESTAION_ID == 29 || _ATTESTAION_ID == 30 || _ATTESTAION_ID == 31 || _ATTESTAION_ID == 32 || _ATTESTAION_ID == 33 || _ATTESTAION_ID == 34 || _ATTESTAION_ID == 35 || _ATTESTAION_ID == 36 || _ATTESTAION_ID == 37 || _ATTESTAION_ID >= 39
#if  _ATTESTAION_ID != 49
#define _NEW_SA80                // ��ʯ��8.0
#endif
#endif

#ifdef _NEW_SA80
#if _ATTESTAION_ID != 43
#define _ONLINE_SHOP             // 8.0�����̳�
#endif
#define _ONLINE_TALK_GM          // ������ϵGM
#define _NEW_ONLINE_SHOP         // �������̳�
#endif
#define _ANGLE_EMAIL             // ���ٺ��Զ�������Ƭ
#endif

#ifdef _VERSION_25
#define _SA_VERSION 'L'
#define _DEFAULT_PKEY "cary"
#define _RUNNING_KEY  "1234567"
#endif

#ifdef _VERSION_NEW
#ifdef  _NO_WAIGUA
#define _SA_VERSION 'W'
#else
#define _SA_VERSION 'S'
#endif
#define _DEFAULT_PKEY "cary"
#define _RUNNING_KEY  "1234567"
#define _CHATROOMPROTOCOL        // (�ɿ���) Syu ADD ������Ƶ�� 8/27
#define _OBJSEND_C               // (�ɿ���) ANDY _OBJSEND_C
#define _ADD_SHOW_ITEMDAMAGE     // (�ɿ���) WON  ��ʾ��Ʒ�;ö�
#define _CHANGETRADERULE         // (�ɿ���) Syu ADD ���׹����޶�
#define _TRADE_SHOWHP            // (�ɿ���) Syu ADD ������ʾѪ��
#define _TRADESYSTEM2            // (�ɿ���) Syu ADD �½���ϵͳ
#define _NPC_MAKEPAIR            // (�ɿ���) ANDY NPC ���
#define _ITEM_PILENUMS           // (�ɿ���) ANDY �����زĶѵ� �� itemset4
#define _ITEM_PILEFORTRADE       // (�ɿ���) ANDY ���׶ѵ�
#define _ITEM_EQUITSPACE         // (�ɿ���) ANDY ��װ����λ
#define _EQUIT_ARRANGE           // (�ɿ���) ANDY ���� ��Ҫ itemset5.txt
#define _EQUIT_ADDPILE           // (�ɿ���) ANDY ���ӿɶѵ��� ��Ҫ itemset5.txt
#define _EQUIT_NEWGLOVE          // (�ɿ���) ANDY ������λ
// �¹��ܣ�ְҵ
#define _NEWREQUESTPROTOCOL      // (�ɿ���) Syu ADD ����ProtocolҪ��ϸ��
#define _OUTOFBATTLESKILL        // (�ɿ���) Syu ADD ��ս��ʱ����Protocol
#define _NPC_ADDLEVELUP          // (�ɿ���) ANDY NPC������ҵȼ�
#define _CHAR_NEWLOGOUT          // (�ɿ���) ANDY �ǳ��»���
#define _NPC_ProfessionTrans     // (�ɿ�) Change ��ְҵNPC�����ж�ת��
#define _CHANNEL_MODIFY          // Ƶ����������(����)
//--------------------------------------------------------------------------
#define _STREET_VENDOR           // ��̯����(����)
#define _WATCH_EVENT             // ��ѯ������(����)
#define _ADD_DUNGEON             // (�ɿ�) Change ׷�ӵ���
#define _ADD_STATUS_2            // ������������״̬�ڶ���
#define  _ALCHEMIST              // Robin ��  ����
#define _JOBDAILY                // cyg ������־����
#define _FONT_SIZE               // (�ɿ�) Robin ���ʹ�С����
#define _PETSKILL_FIXITEM        // (�ɿ���) ANDY �޸� && ͬ������
#define _CHECK_ITEMDAMAGE        // (�ɿ���) ����������  7/26
#define _TEAM_KICKPARTY          // (�ɿ���) ANDY �ӳ�����(��ս��״̬)
#define _TELLCHANNEL             // (�ɿ���) Syu ADD ����Ƶ��
#define _EMENY_CHANCEMAN         // (�ɿ���) ANDY EMENY ѡ��
#define _STANDBYPET              // Robin ��������
#define _CHAR_POOLITEM           // (�ɿ���) ANDY ������ֿ߲⹲��
#define _CHAR_POOLPET            // (�ɿ���) Robin �������ֿ⹲��
#define _NPC_DEPOTPET            // (�ɿ���) Robin �������ֿ⹲��
#define _NPC_DEPOTITEM           // (�ɿ���) ANDY ������ֿ߲⹲��
#define _NEW_MANOR_LAW           // ��ׯ԰����(����)
#define _CHANNEL_MODIFY          // Ƶ����������(����)
#define _TAKE_ITEMDAMAGE         // (�ɿ���) ANDY ������  ITEMSET2
#ifdef _VERSION_NEW60
#define _SKILL_NOCAST            // (�ɿ���) vincent  �輼:��Ĭ //��Ҫ��#define _MAGIC_NOCAST
#define _PET_FUSION              // (�ɿ���) ANDY �����ں� enemybase1.txt
#define _NPC_FUSION              // (�ɿ���) ANDY NPC�����ں�
#define _ITEM_EDITBASES          // (�ɿ���) ANDY Ӫ����
#define _PET_EVOLUTION           // (�ɿ���) ANDY ������
#define _STATUS_WATERWORD        // (�ɿ���) ANDY ˮ����״̬
#define _ITEM_WATERWORDSTATUS    // (�ɿ���) ANDY ˮ����״̬����
#define _PETSKILL_ACUPUNCTURE    // (�ɿ���) Change �輼:�����Ƥ (client�˵�_PETSKILL_ACUPUNCTUREҲҪ��)
#define _SHOW_FUSION             // (�ɿ�) Change �ںϳ�����CLIENT��ֻ��ʾת����,�޸�Ϊ�ںϳ���ʾΪ�ں�(client��ҲҪ��)
#define _TREASURE_BOX            // (�ɿ���) ANDY ����
#define _SHOOTCHESTNUT           // (�ɿ���) Syu ADD �輼��������
#endif

#endif

#ifdef _VERSION_60
#define _SA_VERSION 'Z'
#define _DEFAULT_PKEY "cary"
#define _RUNNING_KEY  "1234567"

#define _PETSKILL_FIXITEM        // (�ɿ���) ANDY �޸� && ͬ������
#define _CHECK_ITEMDAMAGE        // (�ɿ���) ����������  7/26
#define _TAKE_ITEMDAMAGE         // (�ɿ���) ANDY ������  ITEMSET2
#define _SKILL_NOCAST            // (�ɿ���) vincent  �輼:��Ĭ //��Ҫ��#define _MAGIC_NOCAST
#define _EMENY_CHANCEMAN         // (�ɿ���) ANDY EMENY ѡ��
#define _PET_FUSION              // (�ɿ���) ANDY �����ں� enemybase1.txt
#define _NPC_FUSION              // (�ɿ���) ANDY NPC�����ں�
#define _ITEM_EDITBASES          // (�ɿ���) ANDY Ӫ����
#define _PET_EVOLUTION           // (�ɿ���) ANDY ������
#define _TEAM_KICKPARTY          // (�ɿ���) ANDY �ӳ�����(��ս��״̬)
#define _STATUS_WATERWORD        // (�ɿ���) ANDY ˮ����״̬
#define _ITEM_WATERWORDSTATUS    // (�ɿ���) ANDY ˮ����״̬����
#define _TELLCHANNEL             // (�ɿ���) Syu ADD ����Ƶ��
#define _PETSKILL_ACUPUNCTURE    // (�ɿ���) Change �輼:�����Ƥ (client�˵�_PETSKILL_ACUPUNCTUREҲҪ��)
#define _SHOW_FUSION             // (�ɿ�) Change �ںϳ�����CLIENT��ֻ��ʾת����,�޸�Ϊ�ںϳ���ʾΪ�ں�(client��ҲҪ��)
#define _FONT_SIZE               // (�ɿ�) Robin ���ʹ�С����
#define _TREASURE_BOX            // (�ɿ���) ANDY ����
#define _DP_NPC                  // DP�ű�
#define _SHOOTCHESTNUT           // (�ɿ���) Syu ADD �輼��������
#endif

#ifdef _VERSION_80
/*#ifdef _NEW_SA80
    #define _DEFAULT_PKEY "upupupupp"   // ( ʯ�� 8.0 )
    #define _RUNNING_KEY  "20080124"   // ( ʯ�� 8.0 )
    #define _SA_VERSION 'F'
#else
    #define _SA_VERSION 'L'
    #define _DEFAULT_PKEY "www.longzoro.com"   // ( ʯ�� 8.0 )
    #define _RUNNING_KEY  "www.longzoro.com"   // ( ʯ�� 8.0 )
#endif*/

#define _PETSKILL_FIXITEM        // (�ɿ���) ANDY �޸� && ͬ������
#define _CHECK_ITEMDAMAGE        // (�ɿ���) ����������  7/26
#define _TAKE_ITEMDAMAGE         // (�ɿ���) ANDY ������  ITEMSET2
#define _SKILL_NOCAST            // (�ɿ���) vincent  �輼:��Ĭ //��Ҫ��#define _MAGIC_NOCAST
#define _EMENY_CHANCEMAN         // (�ɿ���) ANDY EMENY ѡ��
#define _PET_FUSION              // (�ɿ���) ANDY �����ں� enemybase1.txt
#define _NPC_FUSION              // (�ɿ���) ANDY NPC�����ں�
#define _ITEM_EDITBASES          // (�ɿ���) ANDY Ӫ����
#define _PET_EVOLUTION           // (�ɿ���) ANDY ������
#define _TEAM_KICKPARTY          // (�ɿ���) ANDY �ӳ�����(��ս��״̬)
#define _STATUS_WATERWORD        // (�ɿ���) ANDY ˮ����״̬
#define _ITEM_WATERWORDSTATUS    // (�ɿ���) ANDY ˮ����״̬����
#define _TELLCHANNEL             // (�ɿ���) Syu ADD ����Ƶ��
#define _PETSKILL_ACUPUNCTURE    // (�ɿ���) Change �輼:�����Ƥ (client�˵�_PETSKILL_ACUPUNCTUREҲҪ��)
#define _SHOW_FUSION             // (�ɿ�) Change �ںϳ�����CLIENT��ֻ��ʾת����,�޸�Ϊ�ںϳ���ʾΪ�ں�(client��ҲҪ��)
#define _FONT_SIZE               // (�ɿ�) Robin ���ʹ�С����
#define _TREASURE_BOX            // (�ɿ���) ANDY ����
#define _DP_NPC                  // DP�ű�

#define _PREVENT_TEAMATTACK      // (�ɿ���) ANDY ��ֹͬ�ӻ��� 12/04 ����
#define _OBJSEND_C               // (�ɿ���) ANDY _OBJSEND_C
#define _ADD_SHOW_ITEMDAMAGE     // (�ɿ���) WON  ��ʾ��Ʒ�;ö�
#define _CHANGETRADERULE         // (�ɿ���) Syu ADD ���׹����޶�
#define _TRADE_SHOWHP            // (�ɿ���) Syu ADD ������ʾѪ��
#define _SHOOTCHESTNUT           // (�ɿ���) Syu ADD �輼��������
#define _SKILLLIMIT              // (�ɿ���) Syu ADD ���ù����ҷ�����
#define _TRADESYSTEM2            // (�ɿ���) Syu ADD �½���ϵͳ
#define _NPC_MAKEPAIR            // (�ɿ���) ANDY NPC ���
#define _ITEM_PILENUMS           // (�ɿ���) ANDY �����زĶѵ� �� itemset4
#define _ITEM_PILEFORTRADE       // (�ɿ���) ANDY ���׶ѵ�
#define _ITEM_EQUITSPACE         // (�ɿ���) ANDY ��װ����λ
#define _EQUIT_ARRANGE           // (�ɿ���) ANDY ���� ��Ҫ itemset5.txt
#define _EQUIT_ADDPILE           // (�ɿ���) ANDY ���ӿɶѵ��� ��Ҫ itemset5.txt
#define _EQUIT_NEWGLOVE          // (�ɿ���) ANDY ������λ
// �¹��ܣ�ְҵ
#define _NEWREQUESTPROTOCOL      // (�ɿ���) Syu ADD ����ProtocolҪ��ϸ��
#define _OUTOFBATTLESKILL        // (�ɿ���) Syu ADD ��ս��ʱ����Protocol
#define _CHAR_PROFESSION         // (�ɿ���) WON ADD ����ְҵ��λ
#define _PROFESSION_SKILL        // (�ɿ���) WON ADD ����ְҵ����
#define _NPC_WELFARE             // (�ɿ���) WON ADD ְҵNPC
#define _NPC_ADDLEVELUP          // (�ɿ���) ANDY NPC������ҵȼ�
//#define _CHAR_NEWLOGOUT          // (�ɿ���) ANDY �ǳ��»���
//#define _CHATROOMPROTOCOL        // (�ɿ���) Syu ADD ������Ƶ�� 8/27
//#define _CHAR_POOLITEM           // (�ɿ���) ANDY ������ֿ߲⹲��
//#define _CHAR_POOLPET            // (�ɿ���) Robin �������ֿ⹲��
//#define _NPC_DEPOTPET            // (�ɿ���) Robin �������ֿ⹲��
//#define _NPC_DEPOTITEM           // (�ɿ���) ANDY ������ֿ߲⹲��
#define _NPC_ProfessionTrans     // (�ɿ�) Change ��ְҵNPC�����ж�ת��
//#define _CHANNEL_MODIFY          // Ƶ����������(����)
//#define _STANDBYPET              // Robin ��������
#define  _PROSKILL_OPTIMUM       // Robin ְҵ���ܴ�����ѻ�
//2004/05/12

#define _MAGIC_RESIST_EQUIT      // (�ɿ�) WON ADD ְҵ����װ��
//#define _ALLDOMAN                // (�ɿ���) Syu ADD ���а�NPC
//#define _ALLDOMAN_DEBUG          // (�ɿ�) Change ��������server�����а���ʧbug
//#define _NPC_EXCHANGEMANTRANS    // (�ɿ�) Change ��exchangeman�����ж�ת��
#define  _ANGEL_SUMMON           // Robin ��ʹ�ٻ�
//#define _STREET_VENDOR           // ��̯����(����)
//#define _WATCH_EVENT             // ��ѯ������(����)
//#define _NEW_MANOR_LAW           // ��ׯ԰����(����)
#define _PETSKILL_LER            // �׶�����(����)
#define _FIX_MAGIC_RESIST        // (�ɿ�) Change ְҵħ������
#define _ADD_DUNGEON             // (�ɿ�) Change ׷�ӵ���
#define _PROFESSION_ADDSKILL     // (�ɿ�) Change ׷��ְҵ����
#define _EQUIT_RESIST            // (�ɿ�) Change ��һװ������(�ɿ���һ�����쳣״̬)�п��� �� �� ���� ħ�� ��Ĭ ����
#define _PROSK99                 // (�ɿ�) Change ���ְ���ȼ��Ͱ���
#define _ADD_STATUS_2            // ������������״̬�ڶ���
#define _SUIT_ADDPART4           // (�ɿ�) Change ��װ���ܵ��ĵ� �ж�����% ��ǿ��ʦħ��(����30%) �ֿ���ʦ��ħ��


#define _ITEM_TYPETABLE          // (�ɿ�) Change ׷��typetable
//#define _RIGHTCLICK              // Robin �����Ҽ�����
//#define _JOBDAILY                // cyg ������־����
#define  _ALCHEMIST              // Robin ��  ����
//#define _TEACHER_SYSTEM          // ��ʦϵͳ(����)
#define _PET_ITEM                // ����װ��
#endif


//��ͨ����������-------------------------------------
#define _PUB_ALL
#ifdef _PUB_ALL
//#define _PROSKILL_ERR_KICK       // �Ƿ������Զ�T����
#define _ITEM_SETLOVER           // ��鹦��
#define _CREATE_MM_1_2           // ������Ʒ���ٷ���
#define _SendTo                  // ���͵�������
#define _GET_BATTLE_EXP          // ��������
#define _NEW_PLAYER_CF           // ����״̬����
#define _USER_EXP_CF             // �û��Զ��徭��
#define _TRANS_LEVEL_CF          // ת����Խ160������
#define _UNLAW_WARP_FLOOR        // ��ֹ���͵���
#define _UNREG_NEMA              // ��ֹ��������
#if _ATTESTAION_ID != 1
#define _GM_ITEM                 // ���GM������Ʒ
#endif
#define _GM_METAMO_RIDE          // GM���������г�
#ifdef _NEW_RIDEPETS
#define _GM_RIDE                 // GM�����������Ȩ��
#endif
#define _LOCK_IP                 // ����IP
#define _POINT                   // ��������
#define _VIP_SERVER              // ��Ա����
#define _WATCH_FLOOR             // �Զ���ɹ�ս��ͼ
#define _BATTLE_FLOOR            // �Զ���ǿ��ս����ͼ
#define _VIP_SHOP                // ��Ա�̵�
#if _ATTESTAION_ID != 1
#define _LOOP_ANNOUNCE           // ѭ������
#endif
#define _SKILLUPPOINT_CF         // �Զ�����������
#define _RIDELEVEL               // �Զ���������ȼ�
#define _REVLEVEL                // �Զ���ԭ�ȼ�
#define _FM_ITEM                 // ��������
#define _LUCK_ITEM               // ��������
#define _NEW_PLAYER_RIDE         // �������������
#define _FIX_CHARLOOPS           // ������ħ��ʯ����ʱ��
#if _ATTESTAION_ID != 1
#define _PLAYER_ANNOUNCE         // �������
#endif
#define _BATTLE_GOLD             // ��ȡ����ͬʱ��ý�Ǯ
#define _ITEM_GOLD               // ��Ǯ����Ʒ
#define _PLAYER_NUM              // �����������
#define _PET_MM                  // �����ʯMM
#define _MYSTERIOUS_GIFT         // ��������
#define _RELOAD_CF               // �ض�CF
#define _PET_AND_ITEM_UP         // �ɼ���˶����ĳ�������
#define _TRANS                   // ����ת������
#ifdef _NEW_RIDEPETS
#define _RIDEMODE_20             // 2.0���ģʽ
#endif
#define _TALK_ACTION             // ˵������
#define _ENEMY_ACTION            // ��Ҳ������л���
#define _CHAR_CDKEY              // ��ֹ��������
#define _GMSV_DEBUG              // GMSV������Ϣ
#define _CHECK_PEPEAT            // �Ƿ�������
#define _SILENTZERO              // ������Ʒ
#define _SAVE_GAME_ID            // ��ʾ����ķ�������
#endif

//��Ա��׷�ӹ���--------------------------------------
#define _VIP_ALL
#ifdef _VIP_ALL
#define _NEW_RIDEPETS            // (�ɿ���) ANDY �����
#define _RIDEMODE_20             // 2.0���ģʽ
#define _VIP_RIDE                // ��Ա��ڼ�
#define _AUTO_PK                 // �Զ���PKϵͳ
//#define _FM_METAMO               // ����ר�ñ����ָ
#define _ITEM_METAMO_TIME        // ���Զ������Ľ�ָ
#define _SHOW_VIP_CF             // �Զ������VIP�����ˣ����壬������ʾ��ʽ
#define _GM_KING                 // GM��ħ��
#define _ANGEL_TIME              // �Զ��徫���ٻ�ʱ��
#define _FM_POINT_PK             // ׯ԰���廥��ׯ԰
#if _ATTESTAION_ID != 1
#define _PLAYER_MOVE             // ���˳�ƹ���
#endif
#define _BATTLE_PK               // ǿ��PK���ߣ����˻ؼ�¼��
#ifdef _PET_FUSION
#define _FUSIONBEIT_TRANS        // �ںϳ���ת��
#define _FUSIONBEIT_FIX          // ȡ���ںϳ������
#endif
#define _TRANS_7_NPC             // ������תNPC
#ifdef _NEW_MANOR_LAW
#define _MOMENTUM_NPC            // ���ƽű�
#endif

#ifdef _NEW_RIDEPETS
#define _RIDE_CF                 // �Զ������
#define _FM_LEADER_RIDE          // �峤ר�����
#endif
#define _PET_LEVEL_ITEM          // ����ȼ�������Ʒ
//#define _DBSAVE_CHAR             // ˫��浵
#define _FM_JOINLIMIT            // ������ٴμ�������ʱ������
#define _PET_BEATITUDE           // ����ף��
#define _ITEM_EFMETAMO           // ���ñ����ָ
#define _ITEM_RIDE               // ѱ��װ��
#define _MAKE_MAP                // ��ͼ����
#define _MAKE_PET_CF             // �Զ������ɳ�
#define _MAKE_PET_ABILITY        // �Զ��������Χ
#define _GET_MULTI_ITEM          // һ�θ������Ʒ
#define _RE_GM_COMMAND           // �Զ���GM��������
#endif

#if _ATTESTAION_ID != 9 && _ATTESTAION_ID != 11
#define _LOGNZORO_FUNC_ALL       // ��zoro��������
#else
#define _NEW_VIP_SHOP            // ��Ա�̵�(��SAAC���)
#define _ONLINE_COST             // ���߳�ֵ(��SAAC���)
#define _NULL_CHECK_ITEM         // ��ͷ֧Ʊ����
#define _COST_ITEM               // ��ֵ����
#define _CRYPTO_DATA             // ��������ݼ���ϵͳ
#endif
#ifdef _LOGNZORO_FUNC_ALL
#if _ATTESTAION_ID != 1
#define _TRANS_7_SHOW            // ����7ת��ʾ
#endif
#define _ITEM_COLOER             // ��Ʒ��ɫ����
#define _TALK_MOVE_FLOOR         // ˵���ƶ�����
#ifdef _TRADESYSTEM2
#define _TRADE_PK                // ����PK
#endif
#if _ATTESTAION_ID != 1
#define _VIP_POINT_PK            // ���ֵ�PK
#endif
#define _MAP_HEALERALLHEAL       // ָ����ͼս�����Զ��ظ�����
#if _ATTESTAION_ID != 1
#define _NEW_AUTO_PK             // �µ��Զ���PKϵͳ
#endif
#define _OPEN_E_PETSKILL         // ���ų���E����
#if _ATTESTAION_ID != 1
#define _VIP_ONLINE              // ��Ա������ʾ
#endif
#define _VIP_LOGOUT              // ��Ա�������ʱ����ʾ
#if _ATTESTAION_ID != 1
#define _THE_WORLD_SEND          // ����˵��Ƶ��
#endif
#define _SUPER_FLOOR_MIC         // ��ͼ��˷�
#define _NPC_GET_PETABI          // NPC���Զ���ɳ��ĳ���
#define _SAMETHING_SAVEPOINT     // ���ĳЩ�¼��Զ��浵
#define _NEW_VIP_SHOP            // ��Ա�̵�(��SAAC���)
#if _ATTESTAION_ID != 8 && _ATTESTAION_ID != 10 && _ATTESTAION_ID != 21
#define _ITEM_PET_LOCKED         // ��Ʒ������(��SAAC���)
#endif
#define _ONLINE_COST             // ���߳�ֵ(��SAAC���)
#if _ATTESTAION_ID != 3
#define _VIPPOINT_OLD_TO_NEW     // ���ֵ��ת��(��SAAC���)
#endif
#define _LOGIN_DISPLAY           // ���������ʾ����
//#define _NEED_ITEM_ENEMY         // ץ��������Ʒ�Զ���
#define _SUPER_MAN_ITEM          // ��Ʒ�˵���
#define _AUTO_DEL_PET            // �Զ�ɾ������
#define _AUTO_DEL_ITEM           // �Զ�ɾ����Ʒ
#define _POOL_ITEM_BUG           // ��ֹǿ���BUG

#if _ATTESTAION_ID != 5
#if _ATTESTAION_ID != 1
#define _BOUND_TIME              // ��Ұ�ʱ����
#endif
#define _SPECIAL_MAP             // �����ͼ
#ifdef _FONT_SIZE
#define _TALK_SIZE_ITEM          // ����������
#endif
#define _NEW_NAME                // �Զ���ƺ�
#define _CRYPTO_DATA             // ��������ݼ���ϵͳ
#ifdef _NEW_VIP_SHOP
#define _COST_ITEM             // ��ֵ����
#endif
#define _ITEM_EXP                // ����ҩˮ(�������)
#define _NULL_CHECK_ITEM         // ��ͷ֧Ʊ����
#define _MEMORY_ITEM             // �����ָ
#define _NPC_SCRIPT              // NPC�ű���
//#define _BT_PET                  // BT���ﱶ������
#define _BT_ITEM                 // BT��Ʒ��������
#define _NO_JOIN_FLOOR           // ��ֹ���͵���
#if _ATTESTAION_ID == 1 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 4 || _ATTESTAION_ID == 6 || _ATTESTAION_ID == 8 || _ATTESTAION_ID == 10 || _ATTESTAION_ID == 14 || _ATTESTAION_ID == 15 || _ATTESTAION_ID == 17 || _ATTESTAION_ID >= 19
#ifdef _STREET_VENDOR
#define _NEW_STREET_VENDOR       // �°�̯����
#endif
#endif

#if _ATTESTAION_ID != 1 && _ATTESTAION_ID != 18
#define _LUCK_STAR               // ������(�����ʱ�������������Ʒ�����)
#endif
#if _ATTESTAION_ID != 1
#define _BT_ITEM                 // BT��Ʒ��������
#endif
#ifdef _CHATROOMPROTOCOL
#define _NPC_SAVEPOINT           // ��¼�����������
#endif
#define _SPECIAL_SUIT            // ������װ
#define _MANOR_EQUIP             // ׯ԰ר����װ
#define _ITEM_UPLEVEL            // װ������
#if _ATTESTAION_ID == 1 || _ATTESTAION_ID == 2 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 4 || _ATTESTAION_ID == 6 || _ATTESTAION_ID == 8 || _ATTESTAION_ID == 10 || _ATTESTAION_ID == 14 || _ATTESTAION_ID == 15 || _ATTESTAION_ID >= 17
//#define _TRANS_POINT_UP          // ÿת��������
#if _ATTESTAION_ID != 8
#if _ATTESTAION_ID != 1
#define _FIND_TREASURES          // Ѱ�ұ���
#endif
#define _UNLAW_THIS_LOGOUT       // ��ֹԭ��
#define _ITEM_PERCENTAGE         // ������Ʒ�ٷֱ�
#endif
#if _ATTESTAION_ID == 1 || _ATTESTAION_ID == 2 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 4 || _ATTESTAION_ID == 6 || _ATTESTAION_ID == 8 || _ATTESTAION_ID == 10 || _ATTESTAION_ID == 14 || _ATTESTAION_ID == 17 || _ATTESTAION_ID >= 20
#define _ITEM_POOLITEM           // ������ֿ߲�
#define _ITEM_POOLPET            // �������ֿ�
#define _ADD_NEWEVENT_1024       // ���������1024
#define _SUPER                   // ���ƶ˵���������֧��
#define _NEW_SKILL               // �¼��ܿ���
#if _ATTESTAION_ID != 1
#define _NEW_GM_ITEM             // �����GM������Ʒ
#endif
#define _SHOW_PET_ABL            // ��ʾ��������
#define _NEWEVENT_ITEM           // �����������
#ifdef _NEW_RIDEPETS
#define _ADD_RIDE_CF             // �����Զ����������
#endif
#define _NO_STW_ENEMY            // ��ֹ��ҿ�������
#define _NEW_PET_BEATITUDE       // ��һ������ף��
#define _BATTLE_GETITEM_RATE     // ս�������Ʒ����
#define _INSLAY_NOT_PILENUMS     // ���ܾ���������Ʒ
#if _ATTESTAION_ID == 14 || _ATTESTAION_ID == 28
#define _ONLINE_TIME           // ����ʱ��
#define _NPC_INT_SET           // NPC��������
#endif
#if  _ATTESTAION_ID == 1 || _ATTESTAION_ID == 2 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 4 || _ATTESTAION_ID == 6 || _ATTESTAION_ID == 8 || _ATTESTAION_ID == 14 || _ATTESTAION_ID == 20 || _ATTESTAION_ID == 21 || _ATTESTAION_ID == 23 || _ATTESTAION_ID >= 25
#if _ATTESTAION_ID != 1 || _ATTESTAION_ID != 25
#define _TALK_SAVE               // ˵���浵
#define _PLAYER_QUESTION_ONLIEN  // ������ߴ���
#endif
#endif
#if _ATTESTAION_ID == 28
#define _LOTTERY_SYSTEM          // ��Ʊϵͳ
#define _NPC_CHECK_SCRIPT          // �жϽű���
#endif
#if _ATTESTAION_ID == 38
//#define _PET_TRANS_ABILITY       // ����ת��������������
#endif
#if _ATTESTAION_ID != 38 && _ATTESTAION_ID != 49
#if _ATTESTAION_ID == 1 || _ATTESTAION_ID == 2 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 8 || _ATTESTAION_ID == 21 || _ATTESTAION_ID == 23 || _ATTESTAION_ID >= 30
#define _OLYMPIC_TORCH           // ����ƥ�˻��
#if _ATTESTAION_ID != 1
#define _LOTTERY_SYSTEM          // ��Ʊϵͳ
#define _PLAYER_DIY_MAP          // ���DIY��ͼ����
#endif
#define _AMPOINT_LOG             // ���ֵ��¼
#define _SQL_VIPPOINT_LOG        // �ػرҼ�¼
#define _NPC_CHECK_SCRIPT        // �жϽű���
#define _TALK_CHECK              // ˵����֤
#define _ONE_PET_SKILL           // һ���Գ輼
#define _MAGIC_SPECIAL           // ����ħ��
#if _ATTESTAION_ID != 1
#define _BATTLE_PK_TYPE          // PK����ѡ��
#endif
#if _ATTESTAION_ID == 1 || _ATTESTAION_ID == 2 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 21 || _ATTESTAION_ID == 23 || _ATTESTAION_ID >= 31
#if _ATTESTAION_ID != 1
#define _NPC_AUTO_MOVE             // �ܹ������ƶ���NPC
#endif
#define _SYSTEM_SPEAD          // ϵͳ����
#define _FILTER_TALK               // ����˵��
#if _ATTESTAION_ID != 34
#define _ALL_SERV_SEND       // ����Ƶ��
#if _ATTESTAION_ID == 1 || _ATTESTAION_ID == 2 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 31 || _ATTESTAION_ID == 32 || _ATTESTAION_ID == 33 || _ATTESTAION_ID >= 35 || _ATTESTAION_ID != 39
#define _PET_TRANS_ABILITY       // ����ת��������������
#if  _ATTESTAION_ID == 1 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 31 || _ATTESTAION_ID == 32 || _ATTESTAION_ID == 35 || _ATTESTAION_ID >= 45
#define _CANCEL_ANGLE_TRANS      // ȡ�������ٻ�
#endif
#if _ATTESTAION_ID == 1 || _ATTESTAION_ID == 2 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 31 || _ATTESTAION_ID == 33 || _ATTESTAION_ID == 35 || _ATTESTAION_ID == 36 || _ATTESTAION_ID == 37 || _ATTESTAION_ID == 40 || _ATTESTAION_ID >= 43
#define _COMPOUND_EXP          // �ϳɾ���
#define _ASSESS_ABILITY        // ��������
#define _ONLINE_TIME           // ����ʱ��
#if _ATTESTAION_ID == 1 || _ATTESTAION_ID == 2 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 31 || _ATTESTAION_ID == 35 || _ATTESTAION_ID == 37 || _ATTESTAION_ID >= 43
#ifndef _VERSION_25
#define _PAUCTION_MAN          // ����ϵͳ
#endif
#endif
#define _NOT_ESCAPE              // ��ֹ����
#define _PLAYER_OVERLAP_PK     // ����ص�ʱPK
#define _FIMALY_PK_TIME        // ���忪Ƭʱ���趨
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#else
#define _FORMULATE_AUTO_PK       // �ƶ��Զ���PKϵͳ(��SAAC���)
#endif

#ifdef _MAGIC_SPECIAL
//      #define _MAGIC_FEATHERS          // ħ����ë
#endif

#ifdef _TALK_CHECK
#define _NPC_TALK_CHECK          // NPC˵����֤
#define _ITEM_TALK_CHECK         // ����˵����֤
#endif

#ifdef _NPC_SCRIPT
#define _NPC_CHECKTIME           // NPCʱ���ж�
#define _FAME_CHECK_NPC          // NPC�����ж�
#define _SQL_BUY_FUNC            // SQL������
#define _DEL_FAME                // �ۼ�����
#if _ATTESTAION_ID == 1 || _ATTESTAION_ID == 2 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 31 || _ATTESTAION_ID == 35 || _ATTESTAION_ID == 36 || _ATTESTAION_ID == 37 || _ATTESTAION_ID == 40 || _ATTESTAION_ID >= 43
#define _NPC_INT_SET             // NPC��������
#endif
#endif

#ifdef _NEW_SKILL
#define _ITEM_ATTSKILLMAGIC      // (�ɿ���) ANDY ���߼���
#define _MASSAGE_PETSKILL        // ��ɱ������
#if  _ATTESTAION_ID == 1 || _ATTESTAION_ID == 2 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 23 || _ATTESTAION_ID >= 30
#define _EXPANSION_ITEM_INSLAY      // ��չ�ⱦʯ
#define _STRENGTH_PETSKILL          // Ԫ���輼
#define _EXPANSION_POWERBALANCE     // ��չPETSKILL_PowerBalance
#define _RESURRECTION_PETSKILL      // ����輼
#define _LOSTLOST_PETSKILL          // ���ܾ���
#define _GRAPPLING_PETSKILL         // ������
#if  _ATTESTAION_ID == 1 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 23 || _ATTESTAION_ID >= 31
#define _PETSKILL_EXPLODE               // (���ɿ�) Change �輼:���ѹ���
#define _PETOUT_PETSKILL                // ����Ϣ�輼
#define _FIX_PETSKILL_HECTOR            // �޸��ü����޷�����turn
#endif
#endif
#endif
#endif

#if _ATTESTAION_ID == 1 || _ATTESTAION_ID == 2
#ifdef  _NO_WAIGUA
#ifdef _NEW_STONEAGE_TYPE
#ifdef _SHIQIVIP
#define MANOR_MAX_NUM           5        // (4.0) ����ݵ�
#else
#define MANOR_MAX_NUM           4        // (4.0) ����ݵ�
#endif // _SHIQIVIP
#else
#define MANOR_MAX_NUM           5        // (4.0) ����ݵ�
#endif
#else
#define MANOR_MAX_NUM           6        // (4.0) ����ݵ�
#endif
#else
#define MANOR_MAX_NUM       4        // (4.0) ׯ԰����
#endif

#ifdef  _NO_WAIGUA
#define FAMILY_MAX_MEMBER           100     // ��������
#else
#define FAMILY_MAX_MEMBER           200     // ��������
#endif

#define _FIX_COPY_MM_BUG         //�޸�����MM��BUG
#define _FIX_PETSKILL_BUG        //�޸����＼���ص�ʹ��

#if _ATTESTAION_ID != 3 && _ATTESTAION_ID != 5 && _ATTESTAION_ID != 11
//#define _CHARADATA_SAVE_SQL      //������д��SQL
#endif

#if _ATTESTAION_ID == 1 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 23 || _ATTESTAION_ID == 28 || _ATTESTAION_ID == 31 || _ATTESTAION_ID == 33 || _ATTESTAION_ID == 34 || _ATTESTAION_ID == 35 || _ATTESTAION_ID == 36 || _ATTESTAION_ID == 37 || _ATTESTAION_ID >= 40
#if _ATTESTAION_ID != 49
#define _SASQL                // ����MYSQL����
#endif
#endif
#ifdef _SASQL
#ifdef _STREET_VENDOR
#if _ATTESTAION_ID != 43
#if _ATTESTAION_ID != 28 && _ATTESTAION_ID != 34
#define _ONLINE_SHOP_MYSQL       // �����̳Ǹĳ�MYSQL�ػر�
#endif
#define _STREET_VENDOR_MYSQL     // MYSQL�ػرҰ�̯
#endif
#endif
#endif

#if _ATTESTAION_ID == 1 || _ATTESTAION_ID == 2 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 23 || _ATTESTAION_ID == 33
#define _EXPANSION_VARY_WOLF      // ��չ���±���
#endif

#if _ATTESTAION_ID == 1 || _ATTESTAION_ID == 2 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 21
#define _SHOW_ITEM                // ��װ��
#define _SHOW_ITEM_NAME           // ��װ�������
#endif

#if _ATTESTAION_ID == 1 || _ATTESTAION_ID == 2 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 32
#ifdef _VERSION_80
#define _DISABLE_PROFESSION_SKILL      // ��ֹʹ��ְҵ���ܼ�
#define _TAKE_ITEMDAMAGE_FIX           // ����װ����
#define _TAKE_ITEMDAMAGE_FOR_PET       // ��װװ����
#endif
#endif

#if _ATTESTAION_ID == 1 || _ATTESTAION_ID == 2 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 35
#define _GM_SAVE_ALL_CHAR      // GM����浵
#endif

#if _ATTESTAION_ID == 35
//#define _SHOW_FMPOINT                 //��ʾׯ԰
#define _NOT_FMPK_WAIT              //��ս����ս��
#endif

#if _ATTESTAION_ID == 2 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 16 || _ATTESTAION_ID == 33 || _ATTESTAION_ID == 34 || _ATTESTAION_ID == 41
#ifdef _BOUND_TIME
#define _VIP_BOUND_TIME
#if _ATTESTAION_ID == 1 || _ATTESTAION_ID == 2 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 41
#define _BOUND_TIME_EXPANSION      // ��ʱ������չ
#endif
#endif
#endif

#define _ABSOLUTE_DEBUG              // ���Ե���


#define _ALLBLUES_LUA      // ʹ��LUA�ű�

#ifdef _ALLBLUES_LUA
#if  _ATTESTAION_ID == 1 || _ATTESTAION_ID == 1 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 32
#define _ALLBLUES_LUA_1_1      // LUA1.1��
#endif
#if  _ATTESTAION_ID == 1 || _ATTESTAION_ID == 1 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 32
#define _ALLBLUES_LUA_1_2      // LUA1.2��
#define _ALLBLUES_LUA_1_3      // LUA1.3��
#endif
#if  _ATTESTAION_ID == 1 || _ATTESTAION_ID == 1 || _ATTESTAION_ID == 32
#define _ALLBLUES_LUA_1_4      // LUA1.4��
#endif

#if  _ATTESTAION_ID == 1
#define _ALLBLUES_LUA_1_5      // LUA1.5��
#endif

#if  _ATTESTAION_ID == 1
#define _ALLBLUES_LUA_1_6      // LUA1.6��
#endif
#if  _ATTESTAION_ID == 1
#define _ALLBLUES_LUA_1_7      // LUA1.7��
#endif
#if  _ATTESTAION_ID == 1
#define _ALLBLUES_LUA_1_8      // LUA1.8��
#endif

#if  _ATTESTAION_ID == 1
#define _ALLBLUES_LUA_1_9      // LUA1.9��
#endif

#if  _ATTESTAION_ID == 1 || _ATTESTAION_ID == 1 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 32
#define _PETSKILL_SHOP_LUA       // ���＼���̵�LUA
#endif
#if  _ATTESTAION_ID == 1 || _ATTESTAION_ID == 1 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 32
#define _OFFLINE_SYSTEM       // ����ϵͳ
#endif
#endif

#if _ATTESTAION_ID == 1 || _ATTESTAION_ID == 1 || _ATTESTAION_ID == 3  || _ATTESTAION_ID == 32
#define _PLAYER_NPC      // ������
#endif

#if _ATTESTAION_ID == 1
#define _ONE_SERVER       // ������
#endif

#if _ATTESTAION_ID == 32
#define _NOT_NOCRYPTO_LUA // ���ö�ȡ�Ǽ���LUA�ű�
#endif

#if _ATTESTAION_ID == 1
#define _CRYPTO_LUA         // lua�ű�����
#endif

#if _ATTESTAION_ID == 1
//#define _CAMEO_MONEY              // ���ǻ���
#define _MISSION_TRAIN          // ������
#define _DEL_DROP_PET               // ɾ�����ϳ���
#define _PET_EVOLVE                 // �������
//#define _FREE_SAVE                    // ��������¼
//#define _PET_ADD_EXP              // ���ﾭ����������
#define _FIX_ADD_EXP            // �޸����Ӿ������
#define _FMRIDE_NOT_FMSPRITE    // ������費�ֹ����ڰ�
#define _NOT_SHOW_ONLINE_NUM    // ����ʾ��������
#define _ITEM_COLOER_DIFFER     // ������ɫ��ͬ
#define _SPECIAL_COUNTER            // ������
#define _SPECIAL_TIMER              // ��ʱ��
#define _RECORD_IP                      // ��¼IP
#define _FIX_STW_SPEED_ENEMY    // �޸���ҿ�������
//#define _DEL_NOT_25_NEED_ITEM // ɾ����2.5�����ץ��
#define _LOCK_PET_ITEM              // �󶨳��������
#define _ULTIMATE_ANNOUNCE      // ����֪ͨ
#define _SHIP_MATEMO                    // ���ı�������
#define _FIX_CORE_LOOP              // �޸�CORE_LOOP
//#define _VIP_BATTLE_EXP               // ��Աս������
#define _FLOOR_DIFI_FIELD           // ¥���ֶ�
#define _FM_FMPOINTPK_LIST      // ׯ԰��ս�б�
#define _BATTLE_BOUT_TIME           // ս���غ�ʱ��
#define _ACTIVE_GAME                // ����ֵ
#define _INVERSION_PETSKILL   // ��ת����
#define _NO_HELP_MAP                    // ��ֹHELP
#define _BATTLE_TIME                    // ս��ʱ��
#define _SAME_IP_ONLINE_NUM     // ͬIP��½��
#define _VIP_TRANS                      // VIPת������
//#define _STREET_VENDOR_TRANS  // ��̯����ת��
#define _CANCEL_BORN_PET            // ȡ��������
#define _KEEP_UP_NO_LOGIN           // ά����ֹ��½
#define _FIX_CHAR_LOOP              // ��������ѭ��
#define _FIX_SAVE_CHAR              // �������浵����
#define _FIX_P_VS_P_PARENT      // ����PVP����ս��
#define _FMPOINT_PKTIME_SELECT// ����ѡ����սʱ��
#define _CHECK_SEVER_IP             // �жϷ�����IP
#define _DAMMAGE_CALC                   // �Զ����˺�
#define _VIP_PET_TRANS              // ��Աת��
#define _UNIFIDE_MALINASI           // ͳһ������˿������
//#define _FIX_PET_SKILL                // �޸����＼��
//#define _NO1_B2_NONO                  // B2ѧ�ڵ�һλʱ�Զ�����21��
//#define _FMPOINT_APPEND_ABI       // ׯ԰�����Ա��������
#define _ITEM_OVER_LAP              // �����ظ�����
#define _NEW_LOAD_NPC                   // ��ȡNPC
#define _NEW_LOAD_MAP                   // ��ȡ��ͼ
#define _NO_CLS_EVENT_255           // �����255������
#define _BATTLE_OVER_STAR_TIME// ս�������¿�ʼʱ����
#define _MISSION_TIME                   // �����ʱ
#define _MVP_LOCK_JP                    // ��MVP��Ʒ��
//#define _FMPOINTPK_FOR_TWO        // ÿ����ս���Ϊ����
#define _BURST_LOG                      // �ֶ���־
#define _STREET_VENDOR_EFFECT // ��̯������ʾ
#define _NEWCLISETMAC                   // ��¼�ͻ��˵�MAC��ַ
#define _NEWCLISETSERVID            // ��¼�ͻ��˷�����ID
//#define _FMPOINT_PK_MESSAGE       // ��սȫ��ͨ��
//#define _FMPOINT_WELFARE          // ׯ԰����
#define _REGISTRATION_TIME      // ǩ��ʱ��
#define _FAME_REG_TIME              // ����ǩ��ʱ��
#ifdef _OFFLINE_SYSTEM
#define _OFFLINE_TIME                   // ����ʱ��
#endif
#define _DATA_INFO_SHOW             // ������Ϣ��ʾ
#define _ITEM_USE_TIME              // ����ʹ��ʱ��
//#define _PET_VALIDITY                 // ������Ч��
//#define _PET_FUSION_LUA           // �ںϳ�LUA
//#define _PET_ENEMY_DEVELOP_UP //ȫ����������ɳ�
#define _FIRST_LOCK_ITEM            //��һ�ν��׺��Զ�����Ʒ
#define _CTRL_TRANS_DEVELOP     //���Ƴɳ�
#define _CANCEL_STREET_VENDOR       //��ֹ����̯
#define _ABDUCT_NO_PETOUT           //�ó̻���ֱͬʱ����
#define _SUPER_FMPOINT              //����ׯ԰
#define _ATTACK_RAND_SORT           //����˳�����
#define _POWER_UP                           //��������
#define _FM_POINT_APPLY_FAME    //ׯ԰������������

//#define _NO_GM_MAGIC              //ȥ������Ҫ��GMָ�ָ��GM�˺�Ϊyiqishiqik
#define _FM_LIAOLI                      //�������Ϳ����������ҹ�Ч����
#define _FM_BATTLE_SERVERID     //�Զ�����ս��·
#define _ITEM_EXP_LV                    //��ԭԭ���ľ��鵤
#define _SKILL_25_FIX       //2.5�ͻ��˼����޸��ͻ�����ʾ
#define _PETUP_GET_EXP          //��������ǿ�ƶ���131-140���飬131��ǰ�������60000
#define _OFFLINE_BATTLE_RETEUN  //ս����������
#define _PREVENT_TEAMATTACK      // (�ɿ���) ANDY ��ֹͬ�ӻ��� 12/04 ����
#define _MAGIC_NO_MP                    //CF�ж���0ת100���¾��鲻����
#define _NO_EVENT_FAME          //������(���������ϳ�)���Ӹ�������
#define _PLAYER_LEVELUP_FAME    //��������ʱ��������ĳɺͳ���һ��
//#define _NOT_FMPK_WAIT                //��ս����ս��
#define _NO_ATTACK                      //��������
#define _NO_ENEMYID_GROUP           //CF��ָ������ID��������
#define _NEW_MAP_NO_FAME            //���ý���ָ����ͼ������û������
#define _TALK_AREA_CF                   //CF�ж���˵������������Сֵ
#define _MAGIC_FAMILY_FMSPRITE  //��������ͺڰ�����ʹ����Ӧ����ʱ�Զ������
#define _OFFLINE_RETEUN             //�����Զ�����
#define _FAMILY_PK_FIX                  //�޸�������������
#define _CREATE_TRANS_SAVEPOINT //�½������ת����ǿ�ƶ����¼��Ϊ���
#define _LV_1_NO_ESCAPE                 //�趨1�����ﲻ����
//#define   _P_THREAD_RUN                       //���߳�ģʽ
#define _RIDE_ATT_PET_SKILL         //�޸���սͬ�輼��BUG
#define _BATTLE_RAND_DEX            //��������
//#define   _NO_PLAYER                      //������Χ���
#define _MAGIC_FAMILY_FMSPRITE_FIVE //��5ׯ԰�����������
#define _ENEMY_MIN_ADD              //������С����������
#define _FIX_BATTLE_DIE             //�޸�ս�������︴��󵱻غϳ�����Բ���
#define _PET_LV1_4V                     //��¼����һ��4V
#define _NOTOUZHI_BASE              //�趨������ʹ��Ͷ������������
#define _BATTLE_CLI_TIME            //�Զ���ͻ���ս������ʱ
#define _SELLPET_TRANS              //����ʱָ������ת����Ҫ������֤��
#define _EARTHROUND_BUG             //�޸�����һ�ܵڶ��غϱ����BUG
#define _RED_MEMOY_                     //���ϵͳ
#define _FMBATTLE_POINT             //��ս����
#define _FAMILYBADGE_                   //�������
#define _BATTLE_PAIDUI                  //����ս���Ŷ�
#define _CHAR_MANOR_            //������Ч(������¹⻷)
#define _STAYENCOUNT_LUA    //ԭ������LUA����
#define _PETSTANDY_ABDUCT               //ʹ���ó̻������״̬Ϊ׼��
#define _STREET_MM_SHOW             //MM��̯��ʾ��ʯ״̬
#define _PETITEM_TALKSHOW               //˵��չʾ����͵���
#define _ADDMAXFAMILY           //���Ӽ����������
#define _NEWPAIHANG_                    //��Ӣ������
#ifdef  _PET_LV1_4V
#define _STREETTRADE_PET_LV1_4V //��̯������ʾ����һ��4V
#endif

#define _STW_                       //��ҷ�����
#ifdef _STW_
//#define _PLAYPOS_                 //���������·��Ϣ�ذ�
#endif


#ifdef  _NO_WAIGUA
#define _PETLEVELUP_RECOVERY        //����������Ѫ
#define _TRADE_SHOW_PET_UP          //�޹ҽ�����ʾ����ף��״̬
#define _MAP_AUTO_UPDATE            //�߶���ͼ�Զ�����
#define _800_600                            //800*600ģʽ
#define _BATTLE_ALL_EXP             //���ս�����鹲��
#ifdef  _BATTLE_ALL_EXP
#define _COMBO_EXP                  // �ϻ�����ӳ�
#endif
#define _CHAR_RECORD_POINT      //ˢ�����������������
#define _LUA_RESIST                         //���⿹�Ե�lua�ӿڼ��ֶ�
#define _CHAR_TITLE_STR_                //���ֳƺ�
#define _IMAGE_EXTENSION            //��������չ
#define _OTHER_PETUP_LV                 //���ټ����³���ɼ�
#define _MAGICID_NOUSE_NOMP         //��ս�������ͼ����ħ�������޷�ʹ��
#define _ALWAYS_TEAM_OPEN                   //��¼Ĭ�Ͽ������
#define _PAOHUAN                                //�ܻ�ϵͳ
#define _NEW_TEACHER_SYSTEM          // �µĵ�ʦϵͳ
#define _BIG_CACDBUFSIZE                        //����CA��CDBUF�Ĵ�С
#define _NEW_PETDEPOT                       //�µĳ���ֿ�NPC��ʽ
#define _PLAYER_UID                         //����ΨһUID
#define _NPC_EVENT_NOTICE                           //����NPC����״̬��ʾ���ܣ���ͻ��˿����ú깦��
#define _OFFLINERETURN_LOGINCLI             //������������FD����
#ifdef _NPC_EVENT_NOTICE
#define EVENT_NOT_RESPOND                   120140  //��ɫ�ʺ� ��������ɻ�����δ�ﵽ�������
#define EVENT_CAN_ACCECPT                   120139  //��ɫ�ʺ� ������Խ���
#define EVENT_READY_TO_START            120137  //��ɫ̾�� ���Խ�����
#define EVENT_CANNOT_START              120138  //��ɫ̾�� ���ܽ�����
#endif
#define _NEW_UI                     //�µ�UI���ֻ���ƽ�����䣩
#define _NEW_MIYU_                                  //������

#define _BATTLE_EFFECTS             //ս������Ч
#define _REMOTE_COMMAND_    //ȫ��GMָ��
#define _OLDPS_TO_MD5PS

//����515����
#ifdef _515SA_
#define _DEFAULT_PKEY "!@#$%!@#"        // Ĭ�ϵ���Կ
#define _RUNNING_KEY  ")(*&^%$##"       // ��������Կ
#define _SA_VERSION 'N'
#define DENGLUKEY1 "7C0CDD2CD1B05E0430FA24CCC5141476"
#define _GLORY_POINT                                //SQL��������
#define _MO_LUA_GOLRYCALLBACK       //�����ص�
#define _UNIQUE_NAME                                //ȫ��Ψһ������
#define _AIRCRAFT_PLUS_                           //���ӷɻ�����
#define _CLR_PETTRANS_                          //ÿת���ת������
#define _NEW_CHANNEL_                        //����Ƶ���趨
#define _NO_TEAMWARP_SKYLAND            //��ֹ�Ŷӻؼ�¼��ĵ�ͼ
#define _GETTANKPET_                  //���ת����LUA�ص�
#define _ADDCHARITEMIAMGE_                     //����������������
#define _NEWPETBLESS_                  //��ף��ϵͳ
#define _NODELFAMILY_                             //�޸���һ�����岻��ɾ����BUG
#define _IPHONE_MARK_                  //�ֻ��û���½���
#define _NOPLAYNUM_                  //������������
#define _ADDPARTY_                 //������ӷ��
#define _ADDLOGINTIME_                 //��¼�������ʱ��
#define _FAMILYBATTLENUM_           //���趨����PK����
//#define _MYSQLLOG_                  //���ס���̯��־�ĳ����ݿ��¼
#define _WINLUATO_                        //LUA�������ݸ���
#define _FAMILYFLOORNOMAIGE_                //��ս��ͼ����ʹ�õĲ�Ѫ����
#define _LONIN_ERROR_                //��½������ʾ
//#define _NEW_FUNC_DECRYPT     //�½��ܻ���
//#define _BLOWFISH_ENCRYPT_PACKET    //BLOWFISH����
#define _NOIPHONENET_              //���ֻ����ӵ��»���  ����
#define _MAILITEMFULL_                 //�ʼĶ�������������ˣ��򲻿ɼ�����
#define _IPHONEADDJIKOU_               //�ֻ����ϲ�ȫ�ӿ��ж�ʶ��
#define _IPHONE_SHOP_FLAG_                 //�ֻ���ֵ��־
#define _ADD_BAITAN_PET_                    //���Ӱ�̯���Ի���ף�����
#define _IOSTALK_                              //IOS�Ի�����������
#define _PET_GOLD_                              //���︺������
#define _PAIPAISONG_                 //������
#define _NEWPETSKILL_               //ͨ�������жϳ���Լ���
#define _USESTATUSMULTI_            //���߾�������LUA�ص�
//#define _UPDATESERVERPLAY_            //ά��ָ��ʱ�ı�ͻ��˵�½�б��״̬
#define _PETSKILL_RIDE                  // ������
#define _PETSKILL_ISULTIMATE            // ��ŭ
#define _MOD_PETSKILL_CHARGEATTACK          // ��׼
#define _LC_CHECKNAME_                      //�����Ϸ��
#define _LIMITTIMEITEM_             //��ʱ���߲����ʼġ��ϳɡ�����
#define _NEWFMPOINT_                //����԰�����¹���
#define _ADDFMBATTLE_LUA_           //��������ʱ��ƫ�ƻص�
#define _SHIELD_NONO_               //�������β���ս��������
//#define _NEWPAIHANG_                  //��Ӣ������
//#define _SPEED_CHECK_             //���ټ��ϵͳ
#define _XIUFU_PETMYSQL_                    //�޸����ݿ�ĳ�������
#define _XIUFU_132_                         //�޸�132ս��NPCս����������
#define _ADDJIETU_                  //���ӽ�ͼ����
#define _NEW_SHOP_                  //�̳�
#define _TENSE_FIELD                //��ʱ�ֶ�
#define _BATTLE_LOOK_               //��ս������ť
#define _ATTACK_EFFECT                                // Terry ����Ч��
#define _PETCOM_              //�������
#undef _PET_LV1_4V              //515�����˳������ر�175�ĳ������
#define _ALLDOMAN                // (�ɿ���) Syu ADD ���а�NPC
#define _ALLDOMAN_DEBUG          // (�ɿ�) Change ��������server�����а���ʧbug
#define _THE_WORLD_SEND          // ����˵��Ƶ��
#define FAMILY_MANOR_         //ׯ԰�⻷
#define _PLAYER_EFFECT                                  //����Ч��
#define     _MO_LUA_FMPOINT         //�����ݵ���ص�lua�ӿ�
#define _ONLINE_COST             // ���߳�ֵ
#define _ITEM_PET_TALK_        //���������Ϣ
#define _ALL_SERV_SEND       // ����Ƶ��
#undef _PLAYER_UID          //ʹ��515����ģʽ�ȹر�175��UID
#undef _PETITEM_TALKSHOW        //����175˵��չʾ
#define _EASY_GMPASSWD              //���� .����ʹ��GM����
#define _ADDMAXFAMILY           //���Ӽ����������
#define _515SA_STREET           //515SA��̯
#define _NEW_UNIFIDE_MALINASI   //�µ�ͳһ������
#undef  _NEW_MAP_NO_FAME
#define _515SA_FAME             //515SA����ģʽ
//#define _AUTO_DUMP_                   //�Զ�����CORE�ĵ�
#define _MO_ADD_BOW_ACURATE //���ӹ���PvE��������
#define _515SA_USE_MAC  //515sa��ȫ��֮��Ա�MAC1
#define _WARP_MSG_EX                                        //���ش�������֧��
#define _NOLIMITEPOOL_                     //�ֿ�洢��������
#undef  _OTHER_PETUP_LV
#define _NoSellPet
#define _MASK_ENCOUNTER             // ����ԭ�ط��
#define _WAN_FIX                            //�����Ż�
//#define _RIDEFLG_                              //��迪��
#define _EQUIPMENT_TYPE_                        //�豸���� PC��MAC��IOS����׿
//#define _NOT_FMPK_WAIT                //��ս����ս��
#define _MO_REDUCE_PVP_COMBO                //����PVP�ϻ���
#undef  _NO_EVENT_FAME
#define _FMPK_YIDIANYUAN    //����԰������սģʽ
#define _NOTEAM_ITEMEXPLV   //���鵤�������޷�ʹ��
#undef  _PETSTANDY_ABDUCT
#undef  _UNIFIDE_MALINASI   //����ʱ�ر�
#define _YAYA_VOICE         //����ϵͳ
//#define _ITEM_JIGSAW                          // (���_) Change ƴ�D����
//#undef    _DUELTIME24HOUR
//#undef    _NEW_STREET_VENDOR
//#define _UNIFIDE_MALINASI_2           // ͳһ��ӼӴ����
//#undef    _CREATE_TRANS_SAVEPOINT
#endif // _515SA_

#ifdef _175_SA_
#ifdef _NEW_STONEAGE_TYPE
#define _DEFAULT_PKEY "!$#$%^@!"        // Ĭ�ϵ���Կ
#define _RUNNING_KEY  ")(^#^%!#)"       // ��������Կ
#else
#define _DEFAULT_PKEY "!@#$%!@#"        // Ĭ�ϵ���Կ
#define _RUNNING_KEY  ")(*&^%$##"       // ��������Կ
#endif
#define _SA_VERSION 'N'
#define _NEW_CHANNEL_                        //����Ƶ���趨
#define _NODELFAMILY_                             //�޸���һ�����岻��ɾ����BUG
#define _IPHONE_MARK_                  //�ֻ��û���½���
#define _IPHONE_SHOP_FLAG_                 //�ֻ���ֵ��־
#define _ADD_BAITAN_PET_                    //���Ӱ�̯���Ի���ף�����
#define _BATTLE_LOOK_               //��ս������ť
#define _ATTACK_EFFECT                                // Terry ����Ч��
#define _THE_WORLD_SEND          // ����˵��Ƶ��
#define _ITEM_PET_TALK_        //���������Ϣ
#define _ALL_SERV_SEND       // ����Ƶ��
#undef _PETITEM_TALKSHOW        //����175˵��չʾ
#define _EASY_GMPASSWD              //���� .����ʹ��GM����
#define _WINLUATO_                        //LUA�������ݸ���
#define _NoSellPet
#define _EQUIPMENT_TYPE_                        //�豸���� PC��MAC��IOS����׿
#define _ZHUFU_                 //ף��
#define _515SA_USE_MAC  //515sa��ȫ��֮��Ա�MAC1
#define _XIUFU_132_                         //�޸�132ս��NPCս����������
#define _LC_CHECKNAME_                      //�����Ϸ��
#define _PAIPAISONG_                 //������
#define _WARP_MSG_EX                                        //���ش�������֧��
#define _PLAYER_OBJ_FACEIMAGE   //����OBJ����ͷ����
#define _PLAYER_OBJ_UID //����OBJ����UID
#define _PARTY_OBJINDEX     //��ӷ����������������
#define _FMINDEX_SHOW   //��������������Ӽ�������
#define _NEWFMMEMBERLIST    //�µļ����Ա���
#define _STREET_OFFLINE_KICK    //��̯���������Զ��޳����ߵ�����
#ifdef _NEW_STONEAGE_TYPE
#define _YAYA_VOICE         //����ϵͳ
#define _TELL_ITEM_PET_TALK_    //������ʾ����װ��
#define _SEND_ITEM_TYPE     //���͵������ͣ�0������װ������ʹ�ã�1��װ����2��ʹ�ã�
#define _CHAR_NEW_MESSAGE   //�µĿͻ��˵���
#define _UPDATESERVERPLAY_  //���·����״̬
#undef _UNIFIDE_MALINASI
#undef  _CREATE_TRANS_SAVEPOINT
#define _IMAGE_EXTENSION_2  //��������֣�
#define _NEW_ITEMSHOP_TYPE  //�µĵ����򴰿ڼ����������
//#define _PLAYER_OBJ_FACEIMAGE //����OBJ����ͷ����
//#define _PLAYER_OBJ_UID   //����OBJ����UID
#define _PLAYERCARD_FAMILYNAME  //�����Ƭ�м����������
#define _ITEM_SHOW_VANISHATDROP //���߷���м����Ƿ�����ʧ
#define _PET_SHOW_LOCK  //�������м����Ƿ��
#undef  _DUELTIME24HOUR
#define _NEW_EVENT_ID_UPDATE    //�µ�����С�꼰���������
#define _NEW_FACEIMAGE  //�µ�����ͷ��ʽ
#define _NEW_ADDEXPTIME_SHOW    //���͹���ʱ����
#define _NOPLAYNUM_         //������������
#define _NO_ANNOUNCE_WN     //���ε�¼����Ի���
#define _NEW_OFFLINEBATTLERETURN    //�µ�����ս�����ߺ󷢰�����
#define _BUY_VIGORDATA      //�����������
//#define _PARTY_OBJINDEX       //��ӷ����������������
#define _TRADE_PET_OTHERBUFF    //���׳��︽����Ϣ���
#define _MO_LUA_FMPOINT         //�����ݵ���ص�lua�ӿ�
#define _PET_NOZHUFU_TYPE   //ȥ������ף��״̬��4Vֵ
#define _NEW_PROMPTFLG      //�µ���ʾ����
#define _NEW_MAPBATTLEINFO  //�µĵ�ͼս����Ϣ���
#undef  _BATTLE_BOUT_TIME
#define _NEWPLAYER_FLG  //�������ͷ���
#define _ITEM_LOCKED    //���߰�ȫ��
#define _FMPOINT_DAMAGE2    //ׯ԰�˺��ӳɼ���
#define _PARTY_HIGH //��ӹ�����ǿ
#define _TALK_PLAYER_SHOW   //���촰�������Ϣ
#define _NOACCEPT_FM_SHOW   //������ʽ��Ա����ʾ�������ͻ���
#define _NODP_FmBattlePoint //DP����Ϊս����ʾ
#define _ADDRESS_ONLINE //��Ƭϵͳ�а�DP��������״̬
#define _NEWUI_MAGICUSE //������������ƽʱʹ��
#define _NEW_FS_TYPE        //�µ�FS����
#define _NEW_FMPKWINDOWS    //�µĲ鿴��ս�ų̴���
#define _NEWPARTY_OFFLINE   //����S������������״̬
#define _PLAYER_QQFLG   //���QQ��Ϣ��ʶ
#define _PLAYER_QUESTION    //��Ҵ���
#define _NEWCLIENTLOGIN //�µĵ�½���
#define _NEW_NETSENDRECV    //�µķ�����ͽ��շ�ʽ
//#define _BATTLE_HPSHOW  //ս����Ѫ����ʾ�ٷֱ�
#define DENGLUKEY1 "7C0CDD2CD1B05E0430FA24CCC5141476"
#undef _NEWFMMEMBERLIST
#undef _MAGIC_NO_MP
//�������°汾
#define _ITEM_MMEXP //MM��ż����
#define _OFFLINE_EXP    //���߾���ͳ��
#define _PETSKILL_LER            // �׶�����(����)
#define _VARY_WOLF_JOINBATTLE   //���ǽ���ս�������
#define _PET_IMAGE  //����ͼ��
#define _DU_RAND    //������ÿ�غϿ�Ѫ���������
//#define _NO_MAP_SEND    //������MAP��ͼ����
#ifdef _175_SA_2
#define _SERVER_NP_         //NPǽ������
#define _FAMILY_QQ  //����QQ
#define _CAPTURE_DATA   //ץ������
#undef _STREET_MM_SHOW
#define _U8_QUESTION        //U8�ʾ����
#define _PETSKILL_RIDE                  // ������
#define _PETSKILL_RIDE_FIX2
#define _PETSKILL_ISULTIMATE            // ��ŭ
//#define _UTF8       //UTF8ģʽ
#endif // 175SA_2
#ifdef _U3D_VERSION
#define _TK_PLAYERFACE  //˵���м������ͷ��
#define _NEWPARTY_FACE_UID  //������Ϣ�м���ͷ���UID
#define _FAMILY_IMAGE  //���屳��ͼ
#endif // _U3D_VERSION
#ifdef _182_SA_2
#undef _IMAGE_EXTENSION            //��������չ
#define _NO_ITEMEXPPET  //���鵤ȥ������ȼ�����
#endif
#ifdef _479SA
#define _SMART_ENEMY_LUA                    //����AI��LUA�ص�
#define _TRUE_DAMAGE_MAGIC                  //��ʵħ���˺�
#endif

//�������°汾
#endif // _NEW_STONEAGE_TYPE
//#undef    _NEW_STREET_VENDOR
//#define _UNIFIDE_MALINASI_2           // ͳһ��ӼӴ����
//#define _PETCOM_              //�������
#endif

//#define _DEBUG_RET_CLI
#else
#define _ADD_BAITAN_PET_                    //���Ӱ�̯���Ի���ף�����
#define _ZHUFU_                 //ף��
#define _MASK_ENCOUNTER             // ����ԭ�ط��
#define _WAN_FIX                            //�����Ż�
#define  FMPKNUMBERWIN//ɽ��PK��������
#define _CHAR_TITLE_NEW_      //�����ֳƺ�
#define _NOSHOW_TITLE_FAMILYBADGE   //ָ����ͼ����ʾ�ƺźͻ���
#endif
#endif
/*#if _ATTESTAION_ID == 0 || _ATTESTAION_ID == 1
    #define _DEBUG_ALL
#endif*/
//#define _DEBUG_ALL

#if _ATTESTAION_ID == 35
#define _DAMMAGE_CALC                   // �Զ����˺�
#endif

#endif

//#define _FIX_PER  //����������

//#define   _BACK_DOOR      //����
#ifdef _SHIQIVIP
#define _NEW_PACKET_CODE //�·�����ܽ���
#define _FIX_PER  //����������
#define _PETSKILL_ACUPUNCTURE    // (�ɿ���) Change �輼:�����Ƥ (client�˵�_PETSKILL_ACUPUNCTUREҲҪ��)
#endif

#ifdef _HIGH_TYPE
#define _NEW_PACKET_CODE //�·�����ܽ���
#define _CHAR_PROFESSION         // (�ɿ���) WON ADD ����ְҵ��λ
#define _PROFESSION_SKILL        // (�ɿ���) WON ADD ����ְҵ����
#define  _PROSKILL_OPTIMUM       // Robin ְҵ���ܴ�����ѻ�
#define _MAGIC_RESIST_EQUIT      // (�ɿ�) WON ADD ְҵ����װ��
#define _FIX_MAGIC_RESIST        // (�ɿ�) Change ְҵħ������
#define _PROFESSION_ADDSKILL     // (�ɿ�) Change ׷��ְҵ����
#define _SHOOTCHESTNUT           // (�ɿ���) Syu ADD �輼��������
#define _PETSKILL_ACUPUNCTURE    // (�ɿ���) Change �輼:�����Ƥ (client�˵�_PETSKILL_ACUPUNCTUREҲҪ��)
#define _SKILL_NOCAST  //vincent�輼:��Ĭ
#define _ITEM_SHOWCUSTEM    //��ʾװ����ϸ��Ϣ
#define _PET_2TRANS              // (�ɿ�) Change ����2ת
#define _FIX_PER  //����������
#define _PETSKILL_TEMPTATION    //�ƻ�
//#define _PET_ITEM                // ����װ��

//#define _NEW_515SA                    //�°汾515sa����
#ifdef _NEW_515SA
#define _NEW_LUA_FAMILY_            //LUA�������ӿ� ���Unityǰ��
#define _NEWPLAYER_FUNC             //����ָ��ϵͳ
#define _UNITY_CLIENT_UPDATE        //����Uǰ�˲��ֽ���
#define _NEWSAMENU_WITH_CALLBACK        //���ص�������NEWSAMENU
#define __NEW_CONSIGNMENT           // �µļ��۹���    ȡ����ԭ������ֿ⡢���ֿ߲�ȹ���
#endif
#endif // _HIGH_TYPE


#define _QQLOGIN_MD5 "1376d528ec365e9321d24c0c32b5d9fc"//wocaonimabi

#define MAX_GROUP 5
//#define _WEBSOCKET  //֧��H5_WEBSOCKET����

