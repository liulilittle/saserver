#ifndef __ALLBLUES_H__
#define __ALLBLUES_H__

#include "longzoro.h"
#define CHARDEFAULTSEESIZ 26
#define MAPCHARDEFALUTSEESIZ 30

//私服版本控制 ----------------------------------------
//#define   _NEW_STONEAGE_TYPE  //新版本
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
//山猪begin
#define  NEWDISPATC
#define _OFFLINE_SYSTEM_FORMVENDOR       // 离线摆摊直接退出
#define FUNC_DU_PKCF
#define _NEW_FONT//对应增加颜色橘子后面对应客户端

//开启后关闭大部分登录消息
#define _STOPLOGININFORMATION
#define _DUELTIME24HOUR//延迟开战时间24小时
//在老的version里面的去掉
//这个是屏蔽宏 屏蔽后去掉
#define _ASSESS_SYSDOWNUPNEWS//输出上传下载消息 关闭后不输出
//20150827修改
//#ifdef _ITEM_PET_LOCKED
//ch.data[CHAR_LOCKED] = 0;//原始是-1这里是账号安全锁
//#endif
//山猪end
#ifdef _VERSION_80
#if _ATTESTAION_ID == 0 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 8 || _ATTESTAION_ID == 21 || _ATTESTAION_ID == 23 || _ATTESTAION_ID == 28 || _ATTESTAION_ID == 29 || _ATTESTAION_ID == 30 || _ATTESTAION_ID == 31 || _ATTESTAION_ID == 32 || _ATTESTAION_ID == 33 || _ATTESTAION_ID == 34 || _ATTESTAION_ID == 35 || _ATTESTAION_ID == 36 || _ATTESTAION_ID == 37 || _ATTESTAION_ID >= 39
#if  _ATTESTAION_ID != 49
#define _NEW_SA80                // 新石器8.0
#endif
#endif

#ifdef _NEW_SA80
#if _ATTESTAION_ID != 43
#define _ONLINE_SHOP             // 8.0在线商城
#endif
#define _ONLINE_TALK_GM          // 在线联系GM
#define _NEW_ONLINE_SHOP         // 新在线商城
#endif
#define _ANGLE_EMAIL             // 精召后自动交换名片
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
#define _CHATROOMPROTOCOL        // (可开放) Syu ADD 聊天室频道 8/27
#define _OBJSEND_C               // (可开放) ANDY _OBJSEND_C
#define _ADD_SHOW_ITEMDAMAGE     // (可开放) WON  显示物品耐久度
#define _CHANGETRADERULE         // (可开放) Syu ADD 交易规则修订
#define _TRADE_SHOWHP            // (可开放) Syu ADD 交易显示血量
#define _TRADESYSTEM2            // (可开放) Syu ADD 新交易系统
#define _NPC_MAKEPAIR            // (可开放) ANDY NPC 配对
#define _ITEM_PILENUMS           // (可开放) ANDY 道具素材堆叠 需 itemset4
#define _ITEM_PILEFORTRADE       // (可开放) ANDY 交易堆叠
#define _ITEM_EQUITSPACE         // (可开放) ANDY 新装备栏位
#define _EQUIT_ARRANGE           // (可开放) ANDY 挡格 需要 itemset5.txt
#define _EQUIT_ADDPILE           // (可开放) ANDY 增加可堆叠数 需要 itemset5.txt
#define _EQUIT_NEWGLOVE          // (可开放) ANDY 手套栏位
// 新功能：职业
#define _NEWREQUESTPROTOCOL      // (可开放) Syu ADD 新增Protocol要求细项
#define _OUTOFBATTLESKILL        // (可开放) Syu ADD 非战斗时技能Protocol
#define _NPC_ADDLEVELUP          // (可开放) ANDY NPC增加玩家等级
#define _CHAR_NEWLOGOUT          // (可开放) ANDY 登出新机制
#define _NPC_ProfessionTrans     // (可开) Change 替职业NPC加入判断转生
#define _CHANNEL_MODIFY          // 频道功能整合(开放)
//--------------------------------------------------------------------------
#define _STREET_VENDOR           // 摆摊功能(开放)
#define _WATCH_EVENT             // 查询任务功能(开放)
#define _ADD_DUNGEON             // (可开) Change 追加地牢
#define _ADD_STATUS_2            // 新增传送人物状态第二类
#define  _ALCHEMIST              // Robin 精  功能
#define _JOBDAILY                // cyg 任务日志功能
#define _FONT_SIZE               // (可开) Robin 字型大小功能
#define _PETSKILL_FIXITEM        // (可开放) ANDY 修复 && 同道具损坏
#define _CHECK_ITEMDAMAGE        // (可开放) 道具损坏修正  7/26
#define _TEAM_KICKPARTY          // (可开放) ANDY 队长踢人(非战斗状态)
#define _TELLCHANNEL             // (可开放) Syu ADD 密语频道
#define _EMENY_CHANCEMAN         // (可开放) ANDY EMENY 选择
#define _STANDBYPET              // Robin 待机宠封包
#define _CHAR_POOLITEM           // (可开放) ANDY 人物道具仓库共用
#define _CHAR_POOLPET            // (可开放) Robin 人物宠物仓库共用
#define _NPC_DEPOTPET            // (可开放) Robin 人物宠物仓库共用
#define _NPC_DEPOTITEM           // (可开放) ANDY 人物道具仓库共用
#define _NEW_MANOR_LAW           // 新庄园规则(开放)
#define _CHANNEL_MODIFY          // 频道功能整合(开放)
#define _TAKE_ITEMDAMAGE         // (可开放) ANDY 道具损坏  ITEMSET2
#ifdef _VERSION_NEW60
#define _SKILL_NOCAST            // (可开放) vincent  宠技:沉默 //需要开#define _MAGIC_NOCAST
#define _PET_FUSION              // (可开放) ANDY 宠物融合 enemybase1.txt
#define _NPC_FUSION              // (可开放) ANDY NPC宠物融合
#define _ITEM_EDITBASES          // (可开放) ANDY 营养剂
#define _PET_EVOLUTION           // (可开放) ANDY 进化表
#define _STATUS_WATERWORD        // (可开放) ANDY 水世界状态
#define _ITEM_WATERWORDSTATUS    // (可开放) ANDY 水世界状态道具
#define _PETSKILL_ACUPUNCTURE    // (可开放) Change 宠技:针刺外皮 (client端的_PETSKILL_ACUPUNCTURE也要开)
#define _SHOW_FUSION             // (可开) Change 融合宠物在CLIENT端只显示转生数,修改为融合宠显示为融合(client端也要开)
#define _TREASURE_BOX            // (可开放) ANDY 宝箱
#define _SHOOTCHESTNUT           // (可开放) Syu ADD 宠技：丢栗子
#endif

#endif

#ifdef _VERSION_60
#define _SA_VERSION 'Z'
#define _DEFAULT_PKEY "cary"
#define _RUNNING_KEY  "1234567"

#define _PETSKILL_FIXITEM        // (可开放) ANDY 修复 && 同道具损坏
#define _CHECK_ITEMDAMAGE        // (可开放) 道具损坏修正  7/26
#define _TAKE_ITEMDAMAGE         // (可开放) ANDY 道具损坏  ITEMSET2
#define _SKILL_NOCAST            // (可开放) vincent  宠技:沉默 //需要开#define _MAGIC_NOCAST
#define _EMENY_CHANCEMAN         // (可开放) ANDY EMENY 选择
#define _PET_FUSION              // (可开放) ANDY 宠物融合 enemybase1.txt
#define _NPC_FUSION              // (可开放) ANDY NPC宠物融合
#define _ITEM_EDITBASES          // (可开放) ANDY 营养剂
#define _PET_EVOLUTION           // (可开放) ANDY 进化表
#define _TEAM_KICKPARTY          // (可开放) ANDY 队长踢人(非战斗状态)
#define _STATUS_WATERWORD        // (可开放) ANDY 水世界状态
#define _ITEM_WATERWORDSTATUS    // (可开放) ANDY 水世界状态道具
#define _TELLCHANNEL             // (可开放) Syu ADD 密语频道
#define _PETSKILL_ACUPUNCTURE    // (可开放) Change 宠技:针刺外皮 (client端的_PETSKILL_ACUPUNCTURE也要开)
#define _SHOW_FUSION             // (可开) Change 融合宠物在CLIENT端只显示转生数,修改为融合宠显示为融合(client端也要开)
#define _FONT_SIZE               // (可开) Robin 字型大小功能
#define _TREASURE_BOX            // (可开放) ANDY 宝箱
#define _DP_NPC                  // DP脚本
#define _SHOOTCHESTNUT           // (可开放) Syu ADD 宠技：丢栗子
#endif

#ifdef _VERSION_80
/*#ifdef _NEW_SA80
    #define _DEFAULT_PKEY "upupupupp"   // ( 石器 8.0 )
    #define _RUNNING_KEY  "20080124"   // ( 石器 8.0 )
    #define _SA_VERSION 'F'
#else
    #define _SA_VERSION 'L'
    #define _DEFAULT_PKEY "www.longzoro.com"   // ( 石器 8.0 )
    #define _RUNNING_KEY  "www.longzoro.com"   // ( 石器 8.0 )
#endif*/

#define _PETSKILL_FIXITEM        // (可开放) ANDY 修复 && 同道具损坏
#define _CHECK_ITEMDAMAGE        // (可开放) 道具损坏修正  7/26
#define _TAKE_ITEMDAMAGE         // (可开放) ANDY 道具损坏  ITEMSET2
#define _SKILL_NOCAST            // (可开放) vincent  宠技:沉默 //需要开#define _MAGIC_NOCAST
#define _EMENY_CHANCEMAN         // (可开放) ANDY EMENY 选择
#define _PET_FUSION              // (可开放) ANDY 宠物融合 enemybase1.txt
#define _NPC_FUSION              // (可开放) ANDY NPC宠物融合
#define _ITEM_EDITBASES          // (可开放) ANDY 营养剂
#define _PET_EVOLUTION           // (可开放) ANDY 进化表
#define _TEAM_KICKPARTY          // (可开放) ANDY 队长踢人(非战斗状态)
#define _STATUS_WATERWORD        // (可开放) ANDY 水世界状态
#define _ITEM_WATERWORDSTATUS    // (可开放) ANDY 水世界状态道具
#define _TELLCHANNEL             // (可开放) Syu ADD 密语频道
#define _PETSKILL_ACUPUNCTURE    // (可开放) Change 宠技:针刺外皮 (client端的_PETSKILL_ACUPUNCTURE也要开)
#define _SHOW_FUSION             // (可开) Change 融合宠物在CLIENT端只显示转生数,修改为融合宠显示为融合(client端也要开)
#define _FONT_SIZE               // (可开) Robin 字型大小功能
#define _TREASURE_BOX            // (可开放) ANDY 宝箱
#define _DP_NPC                  // DP脚本

#define _PREVENT_TEAMATTACK      // (可开放) ANDY 防止同队互打 12/04 更新
#define _OBJSEND_C               // (可开放) ANDY _OBJSEND_C
#define _ADD_SHOW_ITEMDAMAGE     // (可开放) WON  显示物品耐久度
#define _CHANGETRADERULE         // (可开放) Syu ADD 交易规则修订
#define _TRADE_SHOWHP            // (可开放) Syu ADD 交易显示血量
#define _SHOOTCHESTNUT           // (可开放) Syu ADD 宠技：丢栗子
#define _SKILLLIMIT              // (可开放) Syu ADD 不得攻击我方限制
#define _TRADESYSTEM2            // (可开放) Syu ADD 新交易系统
#define _NPC_MAKEPAIR            // (可开放) ANDY NPC 配对
#define _ITEM_PILENUMS           // (可开放) ANDY 道具素材堆叠 需 itemset4
#define _ITEM_PILEFORTRADE       // (可开放) ANDY 交易堆叠
#define _ITEM_EQUITSPACE         // (可开放) ANDY 新装备栏位
#define _EQUIT_ARRANGE           // (可开放) ANDY 挡格 需要 itemset5.txt
#define _EQUIT_ADDPILE           // (可开放) ANDY 增加可堆叠数 需要 itemset5.txt
#define _EQUIT_NEWGLOVE          // (可开放) ANDY 手套栏位
// 新功能：职业
#define _NEWREQUESTPROTOCOL      // (可开放) Syu ADD 新增Protocol要求细项
#define _OUTOFBATTLESKILL        // (可开放) Syu ADD 非战斗时技能Protocol
#define _CHAR_PROFESSION         // (可开放) WON ADD 人物职业栏位
#define _PROFESSION_SKILL        // (可开放) WON ADD 人物职业技能
#define _NPC_WELFARE             // (可开放) WON ADD 职业NPC
#define _NPC_ADDLEVELUP          // (可开放) ANDY NPC增加玩家等级
//#define _CHAR_NEWLOGOUT          // (可开放) ANDY 登出新机制
//#define _CHATROOMPROTOCOL        // (可开放) Syu ADD 聊天室频道 8/27
//#define _CHAR_POOLITEM           // (可开放) ANDY 人物道具仓库共用
//#define _CHAR_POOLPET            // (可开放) Robin 人物宠物仓库共用
//#define _NPC_DEPOTPET            // (可开放) Robin 人物宠物仓库共用
//#define _NPC_DEPOTITEM           // (可开放) ANDY 人物道具仓库共用
#define _NPC_ProfessionTrans     // (可开) Change 替职业NPC加入判断转生
//#define _CHANNEL_MODIFY          // 频道功能整合(开放)
//#define _STANDBYPET              // Robin 待机宠封包
#define  _PROSKILL_OPTIMUM       // Robin 职业技能处理最佳化
//2004/05/12

#define _MAGIC_RESIST_EQUIT      // (可开) WON ADD 职业抗性装备
//#define _ALLDOMAN                // (可开放) Syu ADD 排行榜NPC
//#define _ALLDOMAN_DEBUG          // (可开) Change 修正重启server後排行榜消失bug
//#define _NPC_EXCHANGEMANTRANS    // (可开) Change 替exchangeman加入判断转生
#define  _ANGEL_SUMMON           // Robin 天使召唤
//#define _STREET_VENDOR           // 摆摊功能(开放)
//#define _WATCH_EVENT             // 查询任务功能(开放)
//#define _NEW_MANOR_LAW           // 新庄园规则(开放)
#define _PETSKILL_LER            // 雷尔技能(开放)
#define _FIX_MAGIC_RESIST        // (可开) Change 职业魔法抗性
#define _ADD_DUNGEON             // (可开) Change 追加地牢
#define _PROFESSION_ADDSKILL     // (可开) Change 追加职业技能
#define _EQUIT_RESIST            // (可开) Change 单一装备抗性(可抗单一功能异常状态)有抗火 雷 冰 虚弱 魔障 沉默 落马
#define _PROSK99                 // (可开) Change 检查职技等级送白狼
#define _ADD_STATUS_2            // 新增传送人物状态第二类
#define _SUIT_ADDPART4           // (可开) Change 套装功能第四弹 中毒攻击% 加强法师魔法(机率30%) 抵抗法师的魔法


#define _ITEM_TYPETABLE          // (可开) Change 追加typetable
//#define _RIGHTCLICK              // Robin 滑鼠右键功能
//#define _JOBDAILY                // cyg 任务日志功能
#define  _ALCHEMIST              // Robin 精  功能
//#define _TEACHER_SYSTEM          // 导师系统(开放)
#define _PET_ITEM                // 宠物装备
#endif


//普通版新增功能-------------------------------------
#define _PUB_ALL
#ifdef _PUB_ALL
//#define _PROSKILL_ERR_KICK       // 非法技能自动T下线
#define _ITEM_SETLOVER           // 结婚功能
#define _CREATE_MM_1_2           // 制作极品玛蕾菲雅
#define _SendTo                  // 传送到玩家身边
#define _GET_BATTLE_EXP          // 经验设置
#define _NEW_PLAYER_CF           // 新手状态设置
#define _USER_EXP_CF             // 用户自定义经验
#define _TRANS_LEVEL_CF          // 转生超越160级设置
#define _UNLAW_WARP_FLOOR        // 禁止传送地区
#define _UNREG_NEMA              // 禁止人物名称
#if _ATTESTAION_ID != 1
#define _GM_ITEM                 // 玩家GM命令物品
#endif
#define _GM_METAMO_RIDE          // GM人物骑所有宠
#ifdef _NEW_RIDEPETS
#define _GM_RIDE                 // GM命令设置骑宠权限
#endif
#define _LOCK_IP                 // 锁定IP
#define _POINT                   // 点数上限
#define _VIP_SERVER              // 会员服务
#define _WATCH_FLOOR             // 自定义可观战地图
#define _BATTLE_FLOOR            // 自定义强制战斗地图
#define _VIP_SHOP                // 会员商店
#if _ATTESTAION_ID != 1
#define _LOOP_ANNOUNCE           // 循环公告
#endif
#define _SKILLUPPOINT_CF         // 自定义升级点数
#define _RIDELEVEL               // 自定义骑宠相差等级
#define _REVLEVEL                // 自动还原等级
#define _FM_ITEM                 // 声望道具
#define _LUCK_ITEM               // 运气道具
#define _NEW_PLAYER_RIDE         // 新手送配套骑宠
#define _FIX_CHARLOOPS           // 修正恶魔宝石遇敌时间
#if _ATTESTAION_ID != 1
#define _PLAYER_ANNOUNCE         // 玩家喇叭
#endif
#define _BATTLE_GOLD             // 获取经验同时获得金钱
#define _ITEM_GOLD               // 金钱大物品
#define _PLAYER_NUM              // 玩家在线人数
#define _PET_MM                  // 获得满石MM
#define _MYSTERIOUS_GIFT         // 神秘礼物
#define _RELOAD_CF               // 重读CF
#define _PET_AND_ITEM_UP         // 可捡别人丢出的宠物或道具
#define _TRANS                   // 人物转生命令
#ifdef _NEW_RIDEPETS
#define _RIDEMODE_20             // 2.0骑宠模式
#endif
#define _TALK_ACTION             // 说话动作
#define _ENEMY_ACTION            // 玩家步行遇敌机率
#define _CHAR_CDKEY              // 防止串号问题
#define _GMSV_DEBUG              // GMSV调试信息
#define _CHECK_PEPEAT            // 非法宠物检查
#define _SILENTZERO              // 黄莲物品
#define _SAVE_GAME_ID            // 显示保存的服务器名
#endif

//会员版追加功能--------------------------------------
#define _VIP_ALL
#ifdef _VIP_ALL
#define _NEW_RIDEPETS            // (可开放) ANDY 新骑宠
#define _RIDEMODE_20             // 2.0骑宠模式
#define _VIP_RIDE                // 会员骑黑鸡
#define _AUTO_PK                 // 自动化PK系统
//#define _FM_METAMO               // 家族专用变身戒指
#define _ITEM_METAMO_TIME        // 可自定义变身的戒指
#define _SHOW_VIP_CF             // 自定义调整VIP，爱人，家族，名称显示样式
#define _GM_KING                 // GM大魔王
#define _ANGEL_TIME              // 自定义精灵召唤时间
#define _FM_POINT_PK             // 庄园家族互抢庄园
#if _ATTESTAION_ID != 1
#define _PLAYER_MOVE             // 玩家顺移功能
#endif
#define _BATTLE_PK               // 强制PK道具，输了回记录点
#ifdef _PET_FUSION
#define _FUSIONBEIT_TRANS        // 融合宠物转生
#define _FUSIONBEIT_FIX          // 取消融合宠的限制
#endif
#define _TRANS_7_NPC             // 人物七转NPC
#ifdef _NEW_MANOR_LAW
#define _MOMENTUM_NPC            // 气势脚本
#endif

#ifdef _NEW_RIDEPETS
#define _RIDE_CF                 // 自定义骑宠
#define _FM_LEADER_RIDE          // 族长专用骑宠
#endif
#define _PET_LEVEL_ITEM          // 宠物等级提升物品
//#define _DBSAVE_CHAR             // 双向存档
#define _FM_JOINLIMIT            // 退族后再次加入家族的时间限制
#define _PET_BEATITUDE           // 宠物祝福
#define _ITEM_EFMETAMO           // 永久变身戒指
#define _ITEM_RIDE               // 驯骑装备
#define _MAKE_MAP                // 地图制作
#define _MAKE_PET_CF             // 自定义宠物成长
#define _MAKE_PET_ABILITY        // 自定义宠物四围
#define _GET_MULTI_ITEM          // 一次给多件物品
#define _RE_GM_COMMAND           // 自定义GM命令设置
#endif

#if _ATTESTAION_ID != 9 && _ATTESTAION_ID != 11
#define _LOGNZORO_FUNC_ALL       // 龙zoro新增功能
#else
#define _NEW_VIP_SHOP            // 会员商店(需SAAC配合)
#define _ONLINE_COST             // 在线充值(需SAAC配合)
#define _NULL_CHECK_ITEM         // 空头支票道具
#define _COST_ITEM               // 充值道具
#define _CRYPTO_DATA             // 服务端数据加密系统
#endif
#ifdef _LOGNZORO_FUNC_ALL
#if _ATTESTAION_ID != 1
#define _TRANS_7_SHOW            // 人物7转显示
#endif
#define _ITEM_COLOER             // 物品颜色调配
#define _TALK_MOVE_FLOOR         // 说话移动功能
#ifdef _TRADESYSTEM2
#define _TRADE_PK                // 交易PK
#endif
#if _ATTESTAION_ID != 1
#define _VIP_POINT_PK            // 积分点PK
#endif
#define _MAP_HEALERALLHEAL       // 指定地图战斗后自动回复体力
#if _ATTESTAION_ID != 1
#define _NEW_AUTO_PK             // 新的自动化PK系统
#endif
#define _OPEN_E_PETSKILL         // 开放宠物E技能
#if _ATTESTAION_ID != 1
#define _VIP_ONLINE              // 会员上线提示
#endif
#define _VIP_LOGOUT              // 会员最后离线时间提示
#if _ATTESTAION_ID != 1
#define _THE_WORLD_SEND          // 世界说话频道
#endif
#define _SUPER_FLOOR_MIC         // 地图麦克风
#define _NPC_GET_PETABI          // NPC给自定义成长的宠物
#define _SAMETHING_SAVEPOINT     // 玩家某些事件自动存档
#define _NEW_VIP_SHOP            // 会员商店(需SAAC配合)
#if _ATTESTAION_ID != 8 && _ATTESTAION_ID != 10 && _ATTESTAION_ID != 21
#define _ITEM_PET_LOCKED         // 物品宠物锁(需SAAC配合)
#endif
#define _ONLINE_COST             // 在线充值(需SAAC配合)
#if _ATTESTAION_ID != 3
#define _VIPPOINT_OLD_TO_NEW     // 积分点旧转新(需SAAC配合)
#endif
#define _LOGIN_DISPLAY           // 玩家上线提示开关
//#define _NEED_ITEM_ENEMY         // 抓宠所需物品自定义
#define _SUPER_MAN_ITEM          // 极品人道具
#define _AUTO_DEL_PET            // 自动删除宠物
#define _AUTO_DEL_ITEM           // 自动删除物品
#define _POOL_ITEM_BUG           // 禁止强存的BUG

#if _ATTESTAION_ID != 5
#if _ATTESTAION_ID != 1
#define _BOUND_TIME              // 玩家包时功能
#endif
#define _SPECIAL_MAP             // 特殊地图
#ifdef _FONT_SIZE
#define _TALK_SIZE_ITEM          // 大声公道具
#endif
#define _NEW_NAME                // 自定义称号
#define _CRYPTO_DATA             // 服务端数据加密系统
#ifdef _NEW_VIP_SHOP
#define _COST_ITEM             // 充值道具
#endif
#define _ITEM_EXP                // 神奇药水(经验道具)
#define _NULL_CHECK_ITEM         // 空头支票道具
#define _MEMORY_ITEM             // 记忆戒指
#define _NPC_SCRIPT              // NPC脚本包
//#define _BT_PET                  // BT宠物倍数设置
#define _BT_ITEM                 // BT物品倍数设置
#define _NO_JOIN_FLOOR           // 禁止传送地区
#if _ATTESTAION_ID == 1 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 4 || _ATTESTAION_ID == 6 || _ATTESTAION_ID == 8 || _ATTESTAION_ID == 10 || _ATTESTAION_ID == 14 || _ATTESTAION_ID == 15 || _ATTESTAION_ID == 17 || _ATTESTAION_ID >= 19
#ifdef _STREET_VENDOR
#define _NEW_STREET_VENDOR       // 新摆摊功能
#endif
#endif

#if _ATTESTAION_ID != 1 && _ATTESTAION_ID != 18
#define _LUCK_STAR               // 幸运星(随机随时给随机玩家随机物品或宠物)
#endif
#if _ATTESTAION_ID != 1
#define _BT_ITEM                 // BT物品倍数设置
#endif
#ifdef _CHATROOMPROTOCOL
#define _NPC_SAVEPOINT           // 纪录点存人物资料
#endif
#define _SPECIAL_SUIT            // 特殊套装
#define _MANOR_EQUIP             // 庄园专属套装
#define _ITEM_UPLEVEL            // 装备升级
#if _ATTESTAION_ID == 1 || _ATTESTAION_ID == 2 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 4 || _ATTESTAION_ID == 6 || _ATTESTAION_ID == 8 || _ATTESTAION_ID == 10 || _ATTESTAION_ID == 14 || _ATTESTAION_ID == 15 || _ATTESTAION_ID >= 17
//#define _TRANS_POINT_UP          // 每转点数上限
#if _ATTESTAION_ID != 8
#if _ATTESTAION_ID != 1
#define _FIND_TREASURES          // 寻找宝物
#endif
#define _UNLAW_THIS_LOGOUT       // 禁止原登
#define _ITEM_PERCENTAGE         // 设置物品百分比
#endif
#if _ATTESTAION_ID == 1 || _ATTESTAION_ID == 2 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 4 || _ATTESTAION_ID == 6 || _ATTESTAION_ID == 8 || _ATTESTAION_ID == 10 || _ATTESTAION_ID == 14 || _ATTESTAION_ID == 17 || _ATTESTAION_ID >= 20
#define _ITEM_POOLITEM           // 随身道具仓库
#define _ITEM_POOLPET            // 随身宠物仓库
#define _ADD_NEWEVENT_1024       // 增加旗标至1024
#define _SUPER                   // 紫云端的垃圾功能支持
#define _NEW_SKILL               // 新技能开发
#if _ATTESTAION_ID != 1
#define _NEW_GM_ITEM             // 新玩家GM命令物品
#endif
#define _SHOW_PET_ABL            // 显示宠物能力
#define _NEWEVENT_ITEM           // 新增任务道具
#ifdef _NEW_RIDEPETS
#define _ADD_RIDE_CF             // 增加自定义骑宠设置
#endif
#define _NO_STW_ENEMY            // 禁止外挂快速遇敌
#define _NEW_PET_BEATITUDE       // 新一代宠物祝福
#define _BATTLE_GETITEM_RATE     // 战斗获得物品机率
#define _INSLAY_NOT_PILENUMS     // 不能精工叠加物品
#if _ATTESTAION_ID == 14 || _ATTESTAION_ID == 28
#define _ONLINE_TIME           // 在线时间
#define _NPC_INT_SET           // NPC整型设置
#endif
#if  _ATTESTAION_ID == 1 || _ATTESTAION_ID == 2 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 4 || _ATTESTAION_ID == 6 || _ATTESTAION_ID == 8 || _ATTESTAION_ID == 14 || _ATTESTAION_ID == 20 || _ATTESTAION_ID == 21 || _ATTESTAION_ID == 23 || _ATTESTAION_ID >= 25
#if _ATTESTAION_ID != 1 || _ATTESTAION_ID != 25
#define _TALK_SAVE               // 说话存档
#define _PLAYER_QUESTION_ONLIEN  // 玩家在线答题
#endif
#endif
#if _ATTESTAION_ID == 28
#define _LOTTERY_SYSTEM          // 彩票系统
#define _NPC_CHECK_SCRIPT          // 判断脚本包
#endif
#if _ATTESTAION_ID == 38
//#define _PET_TRANS_ABILITY       // 宠物转生能力定义上限
#endif
#if _ATTESTAION_ID != 38 && _ATTESTAION_ID != 49
#if _ATTESTAION_ID == 1 || _ATTESTAION_ID == 2 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 8 || _ATTESTAION_ID == 21 || _ATTESTAION_ID == 23 || _ATTESTAION_ID >= 30
#define _OLYMPIC_TORCH           // 奥林匹克火举
#if _ATTESTAION_ID != 1
#define _LOTTERY_SYSTEM          // 彩票系统
#define _PLAYER_DIY_MAP          // 玩家DIY地图功能
#endif
#define _AMPOINT_LOG             // 积分点记录
#define _SQL_VIPPOINT_LOG        // 重回币记录
#define _NPC_CHECK_SCRIPT        // 判断脚本包
#define _TALK_CHECK              // 说话验证
#define _ONE_PET_SKILL           // 一次性宠技
#define _MAGIC_SPECIAL           // 特殊魔法
#if _ATTESTAION_ID != 1
#define _BATTLE_PK_TYPE          // PK类型选择
#endif
#if _ATTESTAION_ID == 1 || _ATTESTAION_ID == 2 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 21 || _ATTESTAION_ID == 23 || _ATTESTAION_ID >= 31
#if _ATTESTAION_ID != 1
#define _NPC_AUTO_MOVE             // 能够自已移动的NPC
#endif
#define _SYSTEM_SPEAD          // 系统加速
#define _FILTER_TALK               // 过滤说话
#if _ATTESTAION_ID != 34
#define _ALL_SERV_SEND       // 星球频道
#if _ATTESTAION_ID == 1 || _ATTESTAION_ID == 2 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 31 || _ATTESTAION_ID == 32 || _ATTESTAION_ID == 33 || _ATTESTAION_ID >= 35 || _ATTESTAION_ID != 39
#define _PET_TRANS_ABILITY       // 宠物转生能力定义上限
#if  _ATTESTAION_ID == 1 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 31 || _ATTESTAION_ID == 32 || _ATTESTAION_ID == 35 || _ATTESTAION_ID >= 45
#define _CANCEL_ANGLE_TRANS      // 取消精灵召唤
#endif
#if _ATTESTAION_ID == 1 || _ATTESTAION_ID == 2 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 31 || _ATTESTAION_ID == 33 || _ATTESTAION_ID == 35 || _ATTESTAION_ID == 36 || _ATTESTAION_ID == 37 || _ATTESTAION_ID == 40 || _ATTESTAION_ID >= 43
#define _COMPOUND_EXP          // 合成经验
#define _ASSESS_ABILITY        // 评定能力
#define _ONLINE_TIME           // 在线时间
#if _ATTESTAION_ID == 1 || _ATTESTAION_ID == 2 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 31 || _ATTESTAION_ID == 35 || _ATTESTAION_ID == 37 || _ATTESTAION_ID >= 43
#ifndef _VERSION_25
#define _PAUCTION_MAN          // 拍卖系统
#endif
#endif
#define _NOT_ESCAPE              // 禁止逃跑
#define _PLAYER_OVERLAP_PK     // 玩家重叠时PK
#define _FIMALY_PK_TIME        // 家族开片时间设定
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#else
#define _FORMULATE_AUTO_PK       // 制定自动化PK系统(需SAAC配合)
#endif

#ifdef _MAGIC_SPECIAL
//      #define _MAGIC_FEATHERS          // 魔法羽毛
#endif

#ifdef _TALK_CHECK
#define _NPC_TALK_CHECK          // NPC说话验证
#define _ITEM_TALK_CHECK         // 道具说话验证
#endif

#ifdef _NPC_SCRIPT
#define _NPC_CHECKTIME           // NPC时间判断
#define _FAME_CHECK_NPC          // NPC声望判断
#define _SQL_BUY_FUNC            // SQL购买功能
#define _DEL_FAME                // 扣减声望
#if _ATTESTAION_ID == 1 || _ATTESTAION_ID == 2 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 31 || _ATTESTAION_ID == 35 || _ATTESTAION_ID == 36 || _ATTESTAION_ID == 37 || _ATTESTAION_ID == 40 || _ATTESTAION_ID >= 43
#define _NPC_INT_SET             // NPC整型设置
#endif
#endif

#ifdef _NEW_SKILL
#define _ITEM_ATTSKILLMAGIC      // (可开放) ANDY 道具技能
#define _MASSAGE_PETSKILL        // 马杀鸡技能
#if  _ATTESTAION_ID == 1 || _ATTESTAION_ID == 2 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 23 || _ATTESTAION_ID >= 30
#define _EXPANSION_ITEM_INSLAY      // 扩展镶宝石
#define _STRENGTH_PETSKILL          // 元气宠技
#define _EXPANSION_POWERBALANCE     // 扩展PETSKILL_PowerBalance
#define _RESURRECTION_PETSKILL      // 复活宠技
#define _LOSTLOST_PETSKILL          // 两败俱伤
#define _GRAPPLING_PETSKILL         // 擒拿手
#if  _ATTESTAION_ID == 1 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 23 || _ATTESTAION_ID >= 31
#define _PETSKILL_EXPLODE               // (不可开) Change 宠技:爆裂攻击
#define _PETOUT_PETSKILL                // 换休息宠技
#define _FIX_PETSKILL_HECTOR            // 修复该技能无法设置turn
#endif
#endif
#endif
#endif

#if _ATTESTAION_ID == 1 || _ATTESTAION_ID == 2
#ifdef  _NO_WAIGUA
#ifdef _NEW_STONEAGE_TYPE
#ifdef _SHIQIVIP
#define MANOR_MAX_NUM           5        // (4.0) 家族据点
#else
#define MANOR_MAX_NUM           4        // (4.0) 家族据点
#endif // _SHIQIVIP
#else
#define MANOR_MAX_NUM           5        // (4.0) 家族据点
#endif
#else
#define MANOR_MAX_NUM           6        // (4.0) 家族据点
#endif
#else
#define MANOR_MAX_NUM       4        // (4.0) 庄园数量
#endif

#ifdef  _NO_WAIGUA
#define FAMILY_MAX_MEMBER           100     // 家族人数
#else
#define FAMILY_MAX_MEMBER           200     // 家族人数
#endif

#define _FIX_COPY_MM_BUG         //修复复制MM的BUG
#define _FIX_PETSKILL_BUG        //修复宠物技能重叠使用

#if _ATTESTAION_ID != 3 && _ATTESTAION_ID != 5 && _ATTESTAION_ID != 11
//#define _CHARADATA_SAVE_SQL      //将数据写入SQL
#endif

#if _ATTESTAION_ID == 1 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 23 || _ATTESTAION_ID == 28 || _ATTESTAION_ID == 31 || _ATTESTAION_ID == 33 || _ATTESTAION_ID == 34 || _ATTESTAION_ID == 35 || _ATTESTAION_ID == 36 || _ATTESTAION_ID == 37 || _ATTESTAION_ID >= 40
#if _ATTESTAION_ID != 49
#define _SASQL                // 加入MYSQL功能
#endif
#endif
#ifdef _SASQL
#ifdef _STREET_VENDOR
#if _ATTESTAION_ID != 43
#if _ATTESTAION_ID != 28 && _ATTESTAION_ID != 34
#define _ONLINE_SHOP_MYSQL       // 在线商城改成MYSQL重回币
#endif
#define _STREET_VENDOR_MYSQL     // MYSQL重回币摆摊
#endif
#endif
#endif

#if _ATTESTAION_ID == 1 || _ATTESTAION_ID == 2 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 23 || _ATTESTAION_ID == 33
#define _EXPANSION_VARY_WOLF      // 扩展暗月变身
#endif

#if _ATTESTAION_ID == 1 || _ATTESTAION_ID == 2 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 21
#define _SHOW_ITEM                // 秀装备
#define _SHOW_ITEM_NAME           // 秀装备玩家名
#endif

#if _ATTESTAION_ID == 1 || _ATTESTAION_ID == 2 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 32
#ifdef _VERSION_80
#define _DISABLE_PROFESSION_SKILL      // 禁止使用职业技能技
#define _TAKE_ITEMDAMAGE_FIX           // 下身装备损坏
#define _TAKE_ITEMDAMAGE_FOR_PET       // 宠装装备损坏
#endif
#endif

#if _ATTESTAION_ID == 1 || _ATTESTAION_ID == 2 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 35
#define _GM_SAVE_ALL_CHAR      // GM命令存档
#endif

#if _ATTESTAION_ID == 35
//#define _SHOW_FMPOINT                 //显示庄园
#define _NOT_FMPK_WAIT              //族战无休战期
#endif

#if _ATTESTAION_ID == 2 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 16 || _ATTESTAION_ID == 33 || _ATTESTAION_ID == 34 || _ATTESTAION_ID == 41
#ifdef _BOUND_TIME
#define _VIP_BOUND_TIME
#if _ATTESTAION_ID == 1 || _ATTESTAION_ID == 2 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 41
#define _BOUND_TIME_EXPANSION      // 包时功能扩展
#endif
#endif
#endif

#define _ABSOLUTE_DEBUG              // 绝对调试


#define _ALLBLUES_LUA      // 使用LUA脚本

#ifdef _ALLBLUES_LUA
#if  _ATTESTAION_ID == 1 || _ATTESTAION_ID == 1 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 32
#define _ALLBLUES_LUA_1_1      // LUA1.1版
#endif
#if  _ATTESTAION_ID == 1 || _ATTESTAION_ID == 1 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 32
#define _ALLBLUES_LUA_1_2      // LUA1.2版
#define _ALLBLUES_LUA_1_3      // LUA1.3版
#endif
#if  _ATTESTAION_ID == 1 || _ATTESTAION_ID == 1 || _ATTESTAION_ID == 32
#define _ALLBLUES_LUA_1_4      // LUA1.4版
#endif

#if  _ATTESTAION_ID == 1
#define _ALLBLUES_LUA_1_5      // LUA1.5版
#endif

#if  _ATTESTAION_ID == 1
#define _ALLBLUES_LUA_1_6      // LUA1.6版
#endif
#if  _ATTESTAION_ID == 1
#define _ALLBLUES_LUA_1_7      // LUA1.7版
#endif
#if  _ATTESTAION_ID == 1
#define _ALLBLUES_LUA_1_8      // LUA1.8版
#endif

#if  _ATTESTAION_ID == 1
#define _ALLBLUES_LUA_1_9      // LUA1.9版
#endif

#if  _ATTESTAION_ID == 1 || _ATTESTAION_ID == 1 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 32
#define _PETSKILL_SHOP_LUA       // 宠物技能商店LUA
#endif
#if  _ATTESTAION_ID == 1 || _ATTESTAION_ID == 1 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 32
#define _OFFLINE_SYSTEM       // 离线系统
#endif
#endif

#if _ATTESTAION_ID == 1 || _ATTESTAION_ID == 1 || _ATTESTAION_ID == 3  || _ATTESTAION_ID == 32
#define _PLAYER_NPC      // 虚假玩家
#endif

#if _ATTESTAION_ID == 1
#define _ONE_SERVER       // 单机绑定
#endif

#if _ATTESTAION_ID == 32
#define _NOT_NOCRYPTO_LUA // 不让读取非加密LUA脚本
#endif

#if _ATTESTAION_ID == 1
#define _CRYPTO_LUA         // lua脚本加密
#endif

#if _ATTESTAION_ID == 1
//#define _CAMEO_MONEY              // 贝壳货币
#define _MISSION_TRAIN          // 任务链
#define _DEL_DROP_PET               // 删除地上宠物
#define _PET_EVOLVE                 // 宠物进化
//#define _FREE_SAVE                    // 免条件记录
//#define _PET_ADD_EXP              // 宠物经验提升道具
#define _FIX_ADD_EXP            // 修复豆子经验叠加
#define _FMRIDE_NOT_FMSPRITE    // 家族骑宠不分光明黑暗
#define _NOT_SHOW_ONLINE_NUM    // 不显示在线人数
#define _ITEM_COLOER_DIFFER     // 道具颜色不同
#define _SPECIAL_COUNTER            // 计数器
#define _SPECIAL_TIMER              // 计时器
#define _RECORD_IP                      // 记录IP
#define _FIX_STW_SPEED_ENEMY    // 修改外挂快速遇敌
//#define _DEL_NOT_25_NEED_ITEM // 删除非2.5需道具抓宠
#define _LOCK_PET_ITEM              // 绑定宠物与道具
#define _ULTIMATE_ANNOUNCE      // 飞人通知
#define _SHIP_MATEMO                    // 船的变身形像
#define _FIX_CORE_LOOP              // 修改CORE_LOOP
//#define _VIP_BATTLE_EXP               // 会员战斗经验
#define _FLOOR_DIFI_FIELD           // 楼层字段
#define _FM_FMPOINTPK_LIST      // 庄园对战列表
#define _BATTLE_BOUT_TIME           // 战斗回合时间
#define _ACTIVE_GAME                // 活力值
#define _INVERSION_PETSKILL   // 反转技能
#define _NO_HELP_MAP                    // 禁止HELP
#define _BATTLE_TIME                    // 战斗时间
#define _SAME_IP_ONLINE_NUM     // 同IP登陆数
#define _VIP_TRANS                      // VIP转生服务
//#define _STREET_VENDOR_TRANS  // 摆摊限制转数
#define _CANCEL_BORN_PET            // 取消出生宠
#define _KEEP_UP_NO_LOGIN           // 维护禁止登陆
#define _FIX_CHAR_LOOP              // 修正数据循环
#define _FIX_SAVE_CHAR              // 修正保存档问题
#define _FIX_P_VS_P_PARENT      // 修正PVP加入战斗
#define _FMPOINT_PKTIME_SELECT// 自主选择族战时间
#define _CHECK_SEVER_IP             // 判断服务器IP
#define _DAMMAGE_CALC                   // 自定义伤害
#define _VIP_PET_TRANS              // 会员转宠
#define _UNIFIDE_MALINASI           // 统一玛丽娜丝鱼村出生
//#define _FIX_PET_SKILL                // 修复宠物技能
//#define _NO1_B2_NONO                  // B2学在第一位时自动锁定21忠
//#define _FMPOINT_APPEND_ABI       // 庄园家族成员附加能力
#define _ITEM_OVER_LAP              // 道具重复功能
#define _NEW_LOAD_NPC                   // 读取NPC
#define _NEW_LOAD_MAP                   // 读取地图
#define _NO_CLS_EVENT_255           // 不清楚255后的旗标
#define _BATTLE_OVER_STAR_TIME// 战斗后重新开始时间间隔
#define _MISSION_TIME                   // 任务计时
#define _MVP_LOCK_JP                    // 绑定MVP极品宠
//#define _FMPOINTPK_FOR_TWO        // 每天族战最多为两场
#define _BURST_LOG                      // 分段日志
#define _STREET_VENDOR_EFFECT // 摆摊属性显示
#define _NEWCLISETMAC                   // 记录客户端的MAC地址
#define _NEWCLISETSERVID            // 记录客户端服务器ID
//#define _FMPOINT_PK_MESSAGE       // 族战全服通告
//#define _FMPOINT_WELFARE          // 庄园福利
#define _REGISTRATION_TIME      // 签到时间
#define _FAME_REG_TIME              // 家族签到时间
#ifdef _OFFLINE_SYSTEM
#define _OFFLINE_TIME                   // 离线时间
#endif
#define _DATA_INFO_SHOW             // 资料信息显示
#define _ITEM_USE_TIME              // 道具使用时间
//#define _PET_VALIDITY                 // 宠物有效期
//#define _PET_FUSION_LUA           // 融合宠LUA
//#define _PET_ENEMY_DEVELOP_UP //全面提升宠物成长
#define _FIRST_LOCK_ITEM            //第一次交易后自动绑定物品
#define _CTRL_TRANS_DEVELOP     //控制成长
#define _CANCEL_STREET_VENDOR       //防止卡摆摊
#define _ABDUCT_NO_PETOUT           //旅程伙伴禁直同时换宠
#define _SUPER_FMPOINT              //超级庄园
#define _ATTACK_RAND_SORT           //攻击顺序随机
#define _POWER_UP                           //能力提升
#define _FM_POINT_APPLY_FAME    //庄园申请所需声望

//#define _NO_GM_MAGIC              //去除不需要的GM指令并指令GM账号为yiqishiqik
#define _FM_LIAOLI                      //无需家族就可以料理，并且功效翻倍
#define _FM_BATTLE_SERVERID     //自定义族战线路
#define _ITEM_EXP_LV                    //还原原来的经验丹
#define _SKILL_25_FIX       //2.5客户端技能修复客户端显示
#define _PETUP_GET_EXP          //宠物升级强制定义131-140经验，131以前经验除以60000
#define _OFFLINE_BATTLE_RETEUN  //战斗掉线重练
#define _PREVENT_TEAMATTACK      // (可开放) ANDY 防止同队互打 12/04 更新
#define _MAGIC_NO_MP                    //CF中定义0转100以下精灵不费气
#define _NO_EVENT_FAME          //做任务(包含料理，合成)不加个人声望
#define _PLAYER_LEVELUP_FAME    //人物升级时获得声望改成和宠物一样
//#define _NOT_FMPK_WAIT                //族战无休战期
#define _NO_ATTACK                      //防御攻击
#define _NO_ENEMYID_GROUP           //CF中指定怪物ID不能遇敌
#define _NEW_MAP_NO_FAME            //设置进入指定地图后升级没有声望
#define _TALK_AREA_CF                   //CF中定义说话音量最大和最小值
#define _MAGIC_FAMILY_FMSPRITE  //定义光明和黑暗精灵使用相应精灵时自定义废气
#define _OFFLINE_RETEUN             //掉线自动重连
#define _FAMILY_PK_FIX                  //修复家族下书问题
#define _CREATE_TRANS_SAVEPOINT //新建人物和转生后强制定义记录点为渔村
#define _LV_1_NO_ESCAPE                 //设定1级宠物不逃跑
//#define   _P_THREAD_RUN                       //多线程模式
#define _RIDE_ATT_PET_SKILL         //修复骑战同宠技能BUG
#define _BATTLE_RAND_DEX            //乱敏幅度
//#define   _NO_PLAYER                      //屏蔽周围玩家
#define _MAGIC_FAMILY_FMSPRITE_FIVE //第5庄园精灵废气定义
#define _ENEMY_MIN_ADD              //怪物最小遇敌率增加
#define _FIX_BATTLE_DIE             //修复战斗中人物复活后当回合宠物可以操作
#define _PET_LV1_4V                     //记录宠物一级4V
#define _NOTOUZHI_BASE              //设定不允许使用投掷动作的形象
#define _BATTLE_CLI_TIME            //自定义客户端战斗倒计时
#define _SELLPET_TRANS              //卖宠时指定宠物转数需要输入验证码
#define _EARTHROUND_BUG             //修复地球一周第二回合被打的BUG
#define _RED_MEMOY_                     //红包系统
#define _FMBATTLE_POINT             //族战积分
#define _FAMILYBADGE_                   //家族徽章
#define _BATTLE_PAIDUI                  //加入战斗排队
#define _CHAR_MANOR_            //法宝特效(人物脚下光环)
#define _STAYENCOUNT_LUA    //原地遇敌LUA限制
#define _PETSTANDY_ABDUCT               //使用旅程伙伴后宠物状态为准备
#define _STREET_MM_SHOW             //MM摆摊显示满石状态
#define _PETITEM_TALKSHOW               //说话展示宠物和道具
#define _ADDMAXFAMILY           //增加家族最大数量
#define _NEWPAIHANG_                    //新英雄排行
#ifdef  _PET_LV1_4V
#define _STREETTRADE_PET_LV1_4V //摆摊交易显示宠物一级4V
#endif

#define _STW_                       //外挂方面封包
#ifdef _STW_
//#define _PLAYPOS_                 //外挂人物走路消息回包
#endif


#ifdef  _NO_WAIGUA
#define _PETLEVELUP_RECOVERY        //宠物升级满血
#define _TRADE_SHOW_PET_UP          //无挂交易显示宠物祝福状态
#define _MAP_AUTO_UPDATE            //走动地图自动更新
#define _800_600                            //800*600模式
#define _BATTLE_ALL_EXP             //组队战斗经验共享
#ifdef  _BATTLE_ALL_EXP
#define _COMBO_EXP                  // 合击经验加成
#endif
#define _CHAR_RECORD_POINT      //刷新人物封包加入出生地
#define _LUA_RESIST                         //额外抗性的lua接口及字段
#define _CHAR_TITLE_STR_                //文字称号
#define _IMAGE_EXTENSION            //新人物扩展
#define _OTHER_PETUP_LV                 //多少级以下宠物可捡
#define _MAGICID_NOUSE_NOMP         //族战和乱舞地图特殊魔法面气无法使用
#define _ALWAYS_TEAM_OPEN                   //登录默认开启组队
#define _PAOHUAN                                //跑环系统
#define _NEW_TEACHER_SYSTEM          // 新的导师系统
#define _BIG_CACDBUFSIZE                        //扩大CA和CDBUF的大小
#define _NEW_PETDEPOT                       //新的宠物仓库NPC样式
#define _PLAYER_UID                         //人物唯一UID
#define _NPC_EVENT_NOTICE                           //新增NPC任务状态提示功能，需客户端开启该宏功能
#define _OFFLINERETURN_LOGINCLI             //掉线重连给老FD发包
#ifdef _NPC_EVENT_NOTICE
#define EVENT_NOT_RESPOND                   120140  //灰色问号 任务已完成或者尚未达到完成条件
#define EVENT_CAN_ACCECPT                   120139  //黄色问号 任务可以交接
#define EVENT_READY_TO_START            120137  //黄色叹号 可以接任务
#define EVENT_CANNOT_START              120138  //灰色叹号 不能接任务
#endif
#define _NEW_UI                     //新的UI（手机，平板适配）
#define _NEW_MIYU_                                  //新密语

#define _BATTLE_EFFECTS             //战斗中特效
#define _REMOTE_COMMAND_    //全服GM指令
#define _OLDPS_TO_MD5PS

//整合515功能
#ifdef _515SA_
#define _DEFAULT_PKEY "!@#$%!@#"        // 默认的密钥
#define _RUNNING_KEY  ")(*&^%$##"       // 运行中密钥
#define _SA_VERSION 'N'
#define DENGLUKEY1 "7C0CDD2CD1B05E0430FA24CCC5141476"
#define _GLORY_POINT                                //SQL名声积分
#define _MO_LUA_GOLRYCALLBACK       //名声回调
#define _UNIQUE_NAME                                //全局唯一性名字
#define _AIRCRAFT_PLUS_                           //增加飞机人数
#define _CLR_PETTRANS_                          //每转清除转宠任务
#define _NEW_CHANNEL_                        //新增频道设定
#define _NO_TEAMWARP_SKYLAND            //禁止团队回记录点的地图
#define _GETTANKPET_                  //获得转生宠LUA回调
#define _ADDCHARITEMIAMGE_                     //新增人物武器形像
#define _NEWPETBLESS_                  //新祝福系统
#define _NODELFAMILY_                             //修复第一个家族不能删除的BUG
#define _IPHONE_MARK_                  //手机用户登陆标记
#define _NOPLAYNUM_                  //屏蔽线上人数
#define _ADDPARTY_                 //增加组队封包
#define _ADDLOGINTIME_                 //记录玩家在线时间
#define _FAMILYBATTLENUM_           //新设定家族PK人数
//#define _MYSQLLOG_                  //交易、摆摊日志改成数据库记录
#define _WINLUATO_                        //LUA二级内容更新
#define _FAMILYFLOORNOMAIGE_                //族战地图不可使用的补血精灵
#define _LONIN_ERROR_                //登陆错误提示
//#define _NEW_FUNC_DECRYPT     //新解密机制
//#define _BLOWFISH_ENCRYPT_PACKET    //BLOWFISH加密
#define _NOIPHONENET_              //打开手机连接的新机制  问题
#define _MAILITEMFULL_                 //邮寄对象如果道具满了，则不可寄送了
#define _IPHONEADDJIKOU_               //手机资料补全接口判断识别
#define _IPHONE_SHOP_FLAG_                 //手机充值标志
#define _ADD_BAITAN_PET_                    //增加摆摊、对话的祝福封包
#define _IOSTALK_                              //IOS对话不检查面对面
#define _PET_GOLD_                              //宠物负数处理
#define _PAIPAISONG_                 //派派送
#define _NEWPETSKILL_               //通过形像判断宠物吃技能
#define _USESTATUSMULTI_            //道具精灵命中LUA回调
//#define _UPDATESERVERPLAY_            //维护指令时改变客户端登陆列表的状态
#define _PETSKILL_RIDE                  // 上马术
#define _PETSKILL_ISULTIMATE            // 愤怒
#define _MOD_PETSKILL_CHARGEATTACK          // 精准
#define _LC_CHECKNAME_                      //检查游戏名
#define _LIMITTIMEITEM_             //限时道具不可邮寄、合成、丢弃
#define _NEWFMPOINT_                //伊甸园家族新规则
#define _ADDFMBATTLE_LUA_           //增加下书时间偏移回调
#define _SHIELD_NONO_               //机暴屏蔽不守战法不攻击
//#define _NEWPAIHANG_                  //新英雄排行
//#define _SPEED_CHECK_             //加速检测系统
#define _XIUFU_PETMYSQL_                    //修复数据库的宠物问题
#define _XIUFU_132_                         //修复132战场NPC战斗卡的问题
#define _ADDJIETU_                  //增加截图功能
#define _NEW_SHOP_                  //商城
#define _TENSE_FIELD                //限时字段
#define _BATTLE_LOOK_               //观战单独按钮
#define _ATTACK_EFFECT                                // Terry 攻击效果
#define _PETCOM_              //宠物计算
#undef _PET_LV1_4V              //515开启了宠物计算关闭175的宠物计算
#define _ALLDOMAN                // (可开放) Syu ADD 排行榜NPC
#define _ALLDOMAN_DEBUG          // (可开) Change 修正重启server後排行榜消失bug
#define _THE_WORLD_SEND          // 世界说话频道
#define FAMILY_MANOR_         //庄园光环
#define _PLAYER_EFFECT                                  //人物效果
#define     _MO_LUA_FMPOINT         //新增据点相关的lua接口
#define _ONLINE_COST             // 在线充值
#define _ITEM_PET_TALK_        //宠物道具信息
#define _ALL_SERV_SEND       // 星球频道
#undef _PLAYER_UID          //使用515密语模式先关闭175的UID
#undef _PETITEM_TALKSHOW        //屏蔽175说话展示
#define _EASY_GMPASSWD              //输入 .即可使用GM命令
#define _ADDMAXFAMILY           //增加家族最大数量
#define _515SA_STREET           //515SA摆摊
#define _NEW_UNIFIDE_MALINASI   //新的统一出生地
#undef  _NEW_MAP_NO_FAME
#define _515SA_FAME             //515SA声望模式
//#define _AUTO_DUMP_                   //自动生成CORE文档
#define _MO_ADD_BOW_ACURATE //增加弓箭PvE的命中率
#define _515SA_USE_MAC  //515sa安全码之类对比MAC1
#define _WARP_MSG_EX                                        //多重传送密语支持
#define _NOLIMITEPOOL_                     //仓库存储道具设置
#undef  _OTHER_PETUP_LV
#define _NoSellPet
#define _MASK_ENCOUNTER             // 屏蔽原地封包
#define _WAN_FIX                            //带宽优化
//#define _RIDEFLG_                              //骑宠开关
#define _EQUIPMENT_TYPE_                        //设备分类 PC、MAC、IOS、安卓
//#define _NOT_FMPK_WAIT                //族战无休战期
#define _MO_REDUCE_PVP_COMBO                //降低PVP合击率
#undef  _NO_EVENT_FAME
#define _FMPK_YIDIANYUAN    //伊甸园特殊族战模式
#define _NOTEAM_ITEMEXPLV   //经验丹队伍中无法使用
#undef  _PETSTANDY_ABDUCT
#undef  _UNIFIDE_MALINASI   //测试时关闭
#define _YAYA_VOICE         //语音系统
//#define _ITEM_JIGSAW                          // (可開) Change 拼圖道具
//#undef    _DUELTIME24HOUR
//#undef    _NEW_STREET_VENDOR
//#define _UNIFIDE_MALINASI_2           // 统一玛加加村出生
//#undef    _CREATE_TRANS_SAVEPOINT
#endif // _515SA_

#ifdef _175_SA_
#ifdef _NEW_STONEAGE_TYPE
#define _DEFAULT_PKEY "!$#$%^@!"        // 默认的密钥
#define _RUNNING_KEY  ")(^#^%!#)"       // 运行中密钥
#else
#define _DEFAULT_PKEY "!@#$%!@#"        // 默认的密钥
#define _RUNNING_KEY  ")(*&^%$##"       // 运行中密钥
#endif
#define _SA_VERSION 'N'
#define _NEW_CHANNEL_                        //新增频道设定
#define _NODELFAMILY_                             //修复第一个家族不能删除的BUG
#define _IPHONE_MARK_                  //手机用户登陆标记
#define _IPHONE_SHOP_FLAG_                 //手机充值标志
#define _ADD_BAITAN_PET_                    //增加摆摊、对话的祝福封包
#define _BATTLE_LOOK_               //观战单独按钮
#define _ATTACK_EFFECT                                // Terry 攻击效果
#define _THE_WORLD_SEND          // 世界说话频道
#define _ITEM_PET_TALK_        //宠物道具信息
#define _ALL_SERV_SEND       // 星球频道
#undef _PETITEM_TALKSHOW        //屏蔽175说话展示
#define _EASY_GMPASSWD              //输入 .即可使用GM命令
#define _WINLUATO_                        //LUA二级内容更新
#define _NoSellPet
#define _EQUIPMENT_TYPE_                        //设备分类 PC、MAC、IOS、安卓
#define _ZHUFU_                 //祝福
#define _515SA_USE_MAC  //515sa安全码之类对比MAC1
#define _XIUFU_132_                         //修复132战场NPC战斗卡的问题
#define _LC_CHECKNAME_                      //检查游戏名
#define _PAIPAISONG_                 //派派送
#define _WARP_MSG_EX                                        //多重传送密语支持
#define _PLAYER_OBJ_FACEIMAGE   //人物OBJ发送头像封包
#define _PLAYER_OBJ_UID //人物OBJ发送UID
#define _PARTY_OBJINDEX     //组队发送数量和索引封包
#define _FMINDEX_SHOW   //家族资料面板增加家族索引
#define _NEWFMMEMBERLIST    //新的家族会员封包
#define _STREET_OFFLINE_KICK    //摆摊东西卖完自动剔除离线的下线
#ifdef _NEW_STONEAGE_TYPE
#define _YAYA_VOICE         //语音系统
#define _TELL_ITEM_PET_TALK_    //密语显示宠物装备
#define _SEND_ITEM_TYPE     //发送道具类型（0：不可装备不可使用，1：装备，2：使用）
#define _CHAR_NEW_MESSAGE   //新的客户端弹窗
#define _UPDATESERVERPLAY_  //更新服务端状态
#undef _UNIFIDE_MALINASI
#undef  _CREATE_TRANS_SAVEPOINT
#define _IMAGE_EXTENSION_2  //新人物（两种）
#define _NEW_ITEMSHOP_TYPE  //新的道具买窗口加入货币类型
//#define _PLAYER_OBJ_FACEIMAGE //人物OBJ发送头像封包
//#define _PLAYER_OBJ_UID   //人物OBJ发送UID
#define _PLAYERCARD_FAMILYNAME  //玩家名片中加入家族名称
#define _ITEM_SHOW_VANISHATDROP //道具封包中加入是否丢弃消失
#define _PET_SHOW_LOCK  //宠物封包中加入是否绑定
#undef  _DUELTIME24HOUR
#define _NEW_EVENT_ID_UPDATE    //新的任务小标及任务旗标封包
#define _NEW_FACEIMAGE  //新的人物头像方式
#define _NEW_ADDEXPTIME_SHOW    //发送果子时间封包
#define _NOPLAYNUM_         //屏蔽线上人数
#define _NO_ANNOUNCE_WN     //屏蔽登录公告对话框
#define _NEW_OFFLINEBATTLERETURN    //新的离线战斗上线后发包机制
#define _BUY_VIGORDATA      //购买活力数据
//#define _PARTY_OBJINDEX       //组队发送数量和索引封包
#define _TRADE_PET_OTHERBUFF    //交易宠物附加信息封包
#define _MO_LUA_FMPOINT         //新增据点相关的lua接口
#define _PET_NOZHUFU_TYPE   //去除宠物祝福状态的4V值
#define _NEW_PROMPTFLG      //新的提示开关
#define _NEW_MAPBATTLEINFO  //新的地图战斗信息封包
#undef  _BATTLE_BOUT_TIME
#define _NEWPLAYER_FLG  //新手旗标和发包
#define _ITEM_LOCKED    //道具安全锁
#define _FMPOINT_DAMAGE2    //庄园伤害加成减半
#define _PARTY_HIGH //组队功能增强
#define _TALK_PLAYER_SHOW   //聊天窗口玩家信息
#define _NOACCEPT_FM_SHOW   //不是正式成员不显示家族名和徽章
#define _NODP_FmBattlePoint //DP更换为战点显示
#define _ADDRESS_ONLINE //名片系统中把DP换成在线状态
#define _NEWUI_MAGICUSE //治愈和滋润精灵平时使用
#define _NEW_FS_TYPE        //新的FS开关
#define _NEW_FMPKWINDOWS    //新的查看族战排程窗口
#define _NEWPARTY_OFFLINE   //队伍S包中增加离线状态
#define _PLAYER_QQFLG   //玩家QQ信息标识
#define _PLAYER_QUESTION    //玩家答题
#define _NEWCLIENTLOGIN //新的登陆封包
#define _NEW_NETSENDRECV    //新的封包发送接收方式
//#define _BATTLE_HPSHOW  //战斗下血量显示百分比
#define DENGLUKEY1 "7C0CDD2CD1B05E0430FA24CCC5141476"
#undef _NEWFMMEMBERLIST
#undef _MAGIC_NO_MP
//以下是新版本
#define _ITEM_MMEXP //MM玩偶道具
#define _OFFLINE_EXP    //离线经验统计
#define _PETSKILL_LER            // 雷尔技能(开放)
#define _VARY_WOLF_JOINBATTLE   //人狼进入战斗后变身
#define _PET_IMAGE  //宠物图鉴
#define _DU_RAND    //毒技能每回合扣血加随机浮动
//#define _NO_MAP_SEND    //不发送MAP地图数据
#ifdef _175_SA_2
#define _SERVER_NP_         //NP墙服务器
#define _FAMILY_QQ  //家族QQ
#define _CAPTURE_DATA   //抓宠数据
#undef _STREET_MM_SHOW
#define _U8_QUESTION        //U8问卷调查
#define _PETSKILL_RIDE                  // 上马术
#define _PETSKILL_RIDE_FIX2
#define _PETSKILL_ISULTIMATE            // 愤怒
//#define _UTF8       //UTF8模式
#endif // 175SA_2
#ifdef _U3D_VERSION
#define _TK_PLAYERFACE  //说话中加入玩家头像
#define _NEWPARTY_FACE_UID  //队伍信息中加入头像和UID
#define _FAMILY_IMAGE  //家族背景图
#endif // _U3D_VERSION
#ifdef _182_SA_2
#undef _IMAGE_EXTENSION            //新人物扩展
#define _NO_ITEMEXPPET  //经验丹去除宠物等级限制
#endif
#ifdef _479SA
#define _SMART_ENEMY_LUA                    //智能AI的LUA回调
#define _TRUE_DAMAGE_MAGIC                  //真实魔法伤害
#endif

//以上是新版本
#endif // _NEW_STONEAGE_TYPE
//#undef    _NEW_STREET_VENDOR
//#define _UNIFIDE_MALINASI_2           // 统一玛加加村出生
//#define _PETCOM_              //宠物计算
#endif

//#define _DEBUG_RET_CLI
#else
#define _ADD_BAITAN_PET_                    //增加摆摊、对话的祝福封包
#define _ZHUFU_                 //祝福
#define _MASK_ENCOUNTER             // 屏蔽原地封包
#define _WAN_FIX                            //带宽优化
#define  FMPKNUMBERWIN//山猪PK人数窗口
#define _CHAR_TITLE_NEW_      //新文字称号
#define _NOSHOW_TITLE_FAMILYBADGE   //指定地图不显示称号和徽章
#endif
#endif
/*#if _ATTESTAION_ID == 0 || _ATTESTAION_ID == 1
    #define _DEBUG_ALL
#endif*/
//#define _DEBUG_ALL

#if _ATTESTAION_ID == 35
#define _DAMMAGE_CALC                   // 自定义伤害
#endif

#endif

//#define _FIX_PER  //修正命中率

//#define   _BACK_DOOR      //后门
#ifdef _SHIQIVIP
#define _NEW_PACKET_CODE //新封包加密解密
#define _FIX_PER  //修正命中率
#define _PETSKILL_ACUPUNCTURE    // (可开放) Change 宠技:针刺外皮 (client端的_PETSKILL_ACUPUNCTURE也要开)
#endif

#ifdef _HIGH_TYPE
#define _NEW_PACKET_CODE //新封包加密解密
#define _CHAR_PROFESSION         // (可开放) WON ADD 人物职业栏位
#define _PROFESSION_SKILL        // (可开放) WON ADD 人物职业技能
#define  _PROSKILL_OPTIMUM       // Robin 职业技能处理最佳化
#define _MAGIC_RESIST_EQUIT      // (可开) WON ADD 职业抗性装备
#define _FIX_MAGIC_RESIST        // (可开) Change 职业魔法抗性
#define _PROFESSION_ADDSKILL     // (可开) Change 追加职业技能
#define _SHOOTCHESTNUT           // (可开放) Syu ADD 宠技：丢栗子
#define _PETSKILL_ACUPUNCTURE    // (可开放) Change 宠技:针刺外皮 (client端的_PETSKILL_ACUPUNCTURE也要开)
#define _SKILL_NOCAST  //vincent宠技:沉默
#define _ITEM_SHOWCUSTEM    //显示装备详细信息
#define _PET_2TRANS              // (可开) Change 宠物2转
#define _FIX_PER  //修正命中率
#define _PETSKILL_TEMPTATION    //蛊惑
//#define _PET_ITEM                // 宠物装备

//#define _NEW_515SA                    //新版本515sa功能
#ifdef _NEW_515SA
#define _NEW_LUA_FAMILY_            //LUA处理家族接口 配合Unity前端
#define _NEWPLAYER_FUNC             //新手指引系统
#define _UNITY_CLIENT_UPDATE        //更新U前端部分界面
#define _NEWSAMENU_WITH_CALLBACK        //带回调函数的NEWSAMENU
#define __NEW_CONSIGNMENT           // 新的寄售功能    取消了原本宠物仓库、道具仓库等功能
#endif
#endif // _HIGH_TYPE


#define _QQLOGIN_MD5 "1376d528ec365e9321d24c0c32b5d9fc"//wocaonimabi

#define MAX_GROUP 5
//#define _WEBSOCKET  //支持H5_WEBSOCKET链接

