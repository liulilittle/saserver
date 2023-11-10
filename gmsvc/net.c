#define __NET_C__
#include "version.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <time.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
//ttom+1
#include <sys/timeb.h>
#include <fcntl.h>
#include "net.h"
#include "buf.h"
#include "link.h"
#include "common.h"
#include "msignal.h"
#include "configfile.h"
#include "util.h"
#include "saacproto_cli.h"
#include "lssproto_serv.h"
#include "char.h"
#include "handletime.h"
#include "log.h"
#include "object.h"
#include "item_event.h"
#include "enemy.h"
// Arminius 7.31 cursed stone
#include "battle.h"
#include "version.h"
#include "pet_event.h"
#include "char_talk.h"
#include "petmail.h"
#ifdef _ALLBLUES_LUA
#include "mylua/function.h"
#endif
#ifdef _AUTO_PK
#include "npc_autopk.h"
#endif
#include "pet_event.h"
#ifdef _LUCK_STAR
#include "longzoro/luckstar.h"
#endif
#ifdef _PLAYER_DIY_MAP
#include "readmap.h"
extern Player_Diy_Map PlayerDiyMap[Player_Diy_Map_NUM];
#endif
#ifdef _SERVER_NP_
#include "ggsrv30.h"
#endif
#ifdef _WEBSOCKET
/*#include <openssl/sha.h>
#include <openssl/pem.h>
#include <openssl/bio.h>
#include <openssl/evp.h>*/
#endif
#ifdef _GMSV_DEBUG
extern char *DebugMainFunction;
#endif
#define MIN(x,y)     ( ( (x) < (y) ) ? (x) : (y) )
char rbmess[ 1024 * 1024 ];
#ifdef _WEBSOCKET
char rbmesstmp[1024 * 1024];
#endif
#define MAXSIZE         64000
#define MAXEPS            256
//#define EVENTS            100
#define LISTENQ         32
#define SERV_PORT     8000

#ifdef _NEW_SERVER_
BOOL bNewServer = TRUE;
#else
BOOL bNewServer = FALSE;
#endif
extern time_t initTime;
// Nuke +1 0901: For state monitor
int StateTable[ WHILESAVEWAIT + 1 ];

int CHAR_players();
#define CONO_CHECK_LOGIN 0x001
#define CONO_CHECK_ITEM 0x010
#define CONO_CHECK_PET 0x100
int cono_check = 0x111;

int AC_WBSIZE = (1024 * 1024);
//ttom+1 for the performatce
static unsigned int MAX_item_use = 0;
int i_shutdown_time = 0; //ttom
BOOL b_first_shutdown = FALSE; //ttom

int mfdfulll = 0;
int sendspeed = 0;
int recvspeed = 0;

extern char firstdesToken[100000][12];
extern char seconddesToken[100000][40];

extern int luaplayernum;

#ifdef  _P_THREAD_RUN
extern int pthreadflg;
#endif
int savetime = 0;
int safetime = 0;
char _sa_version_type = _SA_VERSION;

extern float TotalTVsec;
extern int TotalNetFun;
extern float SaacTVsec;
extern char SaacNetFun[255];
/*------------------------------------------------------------
 * 扔□田及橇谪
 ------------------------------------------------------------*/
typedef struct tag_serverState
{
    BOOL            acceptmore;     /*  1分匀凶日｝accept 仄凶丐午
                                        切斤仁匹｝close 允月 */
    unsigned int    fdid;           /*  fd 及骚曰袄 */
    unsigned int    closeallsocketnum;  /*   closeallsocket   及酸曰及
                                             醒*/
    int             shutdown;       /*  扔□田毛shutdown允月乒□玉
                                     *  0:骚橘 公木动陆:扑乓永玄扑乓永玄乒□玉
                                     * 乒□玉卞卅匀凶凛棉互  匀化月［
                                     */
    int             dsptime;        /* shutdown 乒□玉及伐□弁  醒*/
    int             limittime;      /* 仇木手 */
} ServerState;
typedef struct tagCONNECT
{
    BOOL use;

    char *rb;
    int rbuse;
    char *wb;
    int wbuse;
    int check_rb_oneline_b;
    int check_rb_time;
    pthread_mutex_t mutex;

    struct sockaddr_in sin; /* 涛粮燮及失玉伊旦 */
    ConnectType ctype;       /* 戊生弁扑亦件及潘挀 */

    char cdkey[ CDKEYLEN ];    /* CDKEY */
    char passwd[ PASSWDLEN ];  /* 由旦伐□玉 */
    LoginType state;        /* 蜇箕及夫弘奶件橇谪 */
    int nstatecount;
    char charname[ CHARNAMELEN ];  /* 夫弘奶件醱及平乓仿抩 */
    int charaindex;     /* char?昫尺及奶件犯永弁旦﹝
                               * 夫弘奶件詨卞袄互涩烂今木月﹝-1互犯白巧伙玄
                               * ?昫卞卅中凛﹝
                               */
#ifdef  _BIG_CACDBUFSIZE
    char CAbuf[ 8192 ];
#else
    char CAbuf[ 2048 ];         /*  CA() 毛做谅允月啃及田永白央 */
#endif
    int CAbufsiz;       /*  CAbuf 及扔奶术  */

    struct timeval lastCAsendtime;     /*瘉詨卞CA毛霜匀凶凛棉 */
#ifdef  _BIG_CACDBUFSIZE
    char CDbuf[ 8192 ];         /*  CD() 毛做谅允月啃及田永白央 */
#else
    char CDbuf[ 2048 ];         /*  CD() 毛做谅允月啃及田永白央 */
#endif
    int CDbufsiz;       /*  CDbuf 及扔奶术  */

    struct timeval lastCDsendtime;     /*瘉詨卞CD毛霜匀凶凛棉 */

    struct timeval lastCharSaveTime; /* 瘉詨卞平乓仿犯□正毛本□皮仄凶凛棉 */

    struct timeval lastprocesstime;    /* 瘉詨卞皿夫玄戊伙毛质咥仄凶凛棉*/

    struct timeval lastreadtime;       /* 瘉詨卞read仄凶凛棉﹝晓午反切互丹*/

    // Nuke start 08/27 : For acceleration avoidance
    // WALK_TOLERANCE: Permit n W messages in a second (3: is the most restricted)
#define WALK_TOLERANCE 4
#define WALK_SPOOL 5
#define WALK_RESTORE 100
    unsigned int Walktime;
    unsigned int lastWalktime;
    unsigned int Walkcount;
    int Walkspool;      // For walk burst after release key F10
    int Walkrestore;
    // B3_TOLERANCE: Time distance between recently 3 B message (8: is the latgest)
    // BEO_TOLERANCE: Time distance between the lastmost B and EO (5: is the largest)
#define B3_TOLERANCE 5
#define BEO_TOLERANCE 3
#define BEO_SPOOL 10
#define BEO_RESTORE 100
    unsigned int Btime;
    unsigned int lastBtime;
    unsigned int lastlastBtime;
    unsigned int EOtime;

#ifdef _BATTLE_TIMESPEED
    // unsigned int  DefBtime;
    int BDTime;
    int CBTime;
#endif

#ifdef _TYPE_TOXICATION
    int toxication;
#endif

#ifdef _ITEM_ADDEXP //vincent 经验提昇
    int EDTime;
#endif
    //    unsigned int      BEO;
    int BEOspool;
    int BEOrestore;

    int credit;
    int fcold;
    // Nuke 0406: New Flow Control
    int nu;
    int nu_decrease;

    // Nuke 1213: Flow Control 2
    int packetin;

    // Nuke 0624: Avoid Null Connection
    unsigned int cotime;
    // Nuke 0626: For no enemy
    int noenemy;
    // Arminius 7.2: Ra's amulet
    int eqnoenemy;
#ifdef _Item_MoonAct
    int eqrandenemy;
#endif

#ifdef _CHIKULA_STONE
    int chistone;
#endif
    // Arminius 7.31: cursed stone
    int stayencount;

    int battlecharaindex[ CONNECT_WINDOWBUFSIZE ];
    int duelcharaindex[ CONNECT_WINDOWBUFSIZE ];
    int tradecardcharaindex[ CONNECT_WINDOWBUFSIZE ];
    int joinpartycharaindex[ CONNECT_WINDOWBUFSIZE ];

    // CoolFish: Trade 2001/4/18
    int tradecharaindex[ CONNECT_WINDOWBUFSIZE ];
    int errornum;
    int fdid;

    int close_request; //the second have this

    int appendwb_overflow_flag;  /* 1荚匹手appendWb互撩?仄凶日1卞允月 */

    BOOL b_shut_up; //for avoid the user wash the screen
    BOOL b_pass;      //for avoid the unlimited area

    struct timeval Wtime;

    struct timeval WLtime;
    BOOL b_first_warp;
    int state_trans;

    // CoolFish: Trade 2001/4/18
    char TradeTmp[ 256 ];

#ifdef _ITEM_PILEFORTRADE
    int tradelist;
#endif
    // Shan Recvdata Time

    struct timeval lastrecvtime;      // 'FM' Stream Control time

    struct timeval lastrecvtime_d;    // DENGON Talk Control time

    // Arminius: 6.22 encounter
    int CEP; // Current Encounter Probability
    // Arminius 7.12 login announce
    int announced;

    // shan battle delay time 2001/12/26

    struct timeval battle_recvtime;

    BOOL confirm_key;    // shan  trade(DoubleCheck)

#ifdef _NEWCLISETSERVID
    int servid;
#endif

#ifdef _NEWCLISETMAC
    char mac[128];
    char mac2[128];
    char mac3[128];
#endif

    int connecttime;
    char defaultkey[40];
    char defaultdeskey[40];
#ifdef _IPHONE_MARK_
    int loginmark;
#endif
#ifdef _NEW_FUNC_DECRYPT
    int newerrnum;
#endif
#ifdef _ITEM_PET_TALK_
    char pettalk[256];
    char pettalkdata[1024];
#endif
#ifdef _EQUIPMENT_TYPE_
    int newworkindex;
    int newnetworkfalg;
#endif
#ifdef _SERVER_NP_
    PBYTE pbUser;
    unsigned int nptime;
    int npcnt;
#endif
#ifdef _NEW_NETSENDRECV
    int funcnum;
    char newdefaultkey[36];
#endif // DEBUG
#ifdef _WEBSOCKET
    int clitype;
#endif
}
CONNECT;

CONNECT *Connect;     /*コネクション瘦ち脱*/


/* 簇眶の黎片につけてわかるようにするだけのマクロ */
#define SINGLETHREAD
#define MUTLITHREAD
#define ANYTHREAD

ServerState servstate;

pthread_mutex_t MTIO_servstate_m;
#define SERVSTATE_LOCK() pthread_mutex_lock( &MTIO_servstate_m );
#define SERVSTATE_UNLOCK() pthread_mutex_unlock( &MTIO_servstate_m );
#define CONNECT_LOCK_ARG2(i,j) pthread_mutex_lock( &Connect[i].mutex );
#define CONNECT_UNLOCK_ARG2(i,j) pthread_mutex_unlock( &Connect[i].mutex );
#define CONNECT_LOCK(i) pthread_mutex_lock( &Connect[i].mutex );
#define CONNECT_UNLOCK(i) pthread_mutex_unlock( &Connect[i].mutex );
/*
#define SERVSTATE_LOCK()
#define SERVSTATE_UNLOCK()
#define CONNECT_LOCK_ARG2(i,j)
#define CONNECT_UNLOCK_ARG2(i,j)
#define CONNECT_LOCK(i)
#define CONNECT_UNLOCK(i)
*/

#ifdef _WEBSOCKET
#define GUID "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"
#define WEBSOCKET_DATA_ERROR    0
#define WEBSOCKET_DATA_RECVING  1
#define WEBSOCKET_DATA_FINISH   2
int ReceiveData(unsigned char* data, int cacheLen, int* packetLen, unsigned char** packfinishData, int* packetfinishLen)
{
    int payloadLength;
    unsigned char *start = data;
    unsigned char *end = start + cacheLen;
    BOOL isFinal = (start[0] & 0x80) != 0;
    int reservedBits = start[0] & 0x70;
    int frameType = start[0] & 0xf;
    BOOL isMasked = (start[1] & 0x80) != 0;
    int length = start[1] & 0x7f;

    if (start + 2 > end)
    {
        return WEBSOCKET_DATA_RECVING;
    }

    if (!isMasked || frameType != 2 || reservedBits != 0)
    {
        return WEBSOCKET_DATA_ERROR;
    }

    start += 2;

    if (length == 127)
    {
        if (start + 8 > end)
        {
            return WEBSOCKET_DATA_RECVING;
        }

        payloadLength = ((start[0] << 24) | (start[1] << 16) | (start[2] << 8) | start[3]);
        start += 8;
    }
    else if (length == 126)
    {
        if (start + 2 > end)
        {
            return WEBSOCKET_DATA_RECVING;
        }

        payloadLength = (start[0] << 8) | start[1];
        start += 2;
    }
    else
    {
        payloadLength = length;
    }

    if (start + 4 > end)
    {
        return WEBSOCKET_DATA_RECVING;
    }

    char *maskBytes = start;
    start += 4;

    if (start + payloadLength > end)
    {
        return WEBSOCKET_DATA_RECVING;    //Not complete
    }

    static unsigned char payloadData[RBSIZE];
    int i;

    for (i = 0; i < payloadLength; i++)
    {
        payloadData[i] = (unsigned char)(start[i] ^ maskBytes[i % 4]);
    }

    *packetLen = (start - data) + payloadLength;
    *packetfinishLen = payloadLength;
    *packfinishData = payloadData;
    return WEBSOCKET_DATA_FINISH;
}

int on_ws_recv_data(unsigned char*pkg_data, int pkg_len)
{
    int webret = 0;
    // 第一个字节是头，已经判断，跳过;
    unsigned char* mask = NULL;
    unsigned char* raw_data = NULL;
    unsigned int weblen = pkg_data[1];
    // 最高的一个bit始终为1,我们要把最高的这个bit,变为0;
    weblen = (weblen & 0x0000007f);

    if (weblen <= 125)
    {
        mask = pkg_data + 2; // 头字节，长度字节
    }
    else if (weblen == 126)   // 后面两个字节表示长度；
    {
        weblen = ((pkg_data[2]) << 8 | (pkg_data[3]));
        mask = pkg_data + 2 + 2;
    }
    else if (weblen == 127)   // 这种情况不用考虑,考虑前4个字节的大小，后面不管;
    {
        unsigned int hight = ((pkg_data[2] << 24) | (pkg_data[3] << 16) | (pkg_data[4] << 8) | (pkg_data[5]));
        unsigned int low = ((pkg_data[6] << 24) | (pkg_data[7] << 16) | (pkg_data[8] << 8) | (pkg_data[9]));

        if (hight != 0)   // 表示后四个字节有数据int存放不了，太大了，我们不要了。
        {
            return webret;
        }

        weblen = low;
        mask = pkg_data + 2 + 8;
    }

    // mask 固定4个字节，所以后面的数据部分
    raw_data = mask + 4;
    // 还原我们的发送过来的数据;
    // 从原始数据的第0个字节开始，然后，每个字节与对应的mask进行异或，得到真实的数据。
    // 由于mask只有4个字节，所以mask循环重复使用;(0, 1, 2, 3, 0, 1, 2, 3);
    static unsigned char recv_buf[1024 * 196];
    unsigned int i;

    for (i = 0; i < weblen; i++)
    {
        recv_buf[i] = raw_data[i] ^ mask[i % 4]; // mask只有4个字节的长度，所以，要循环使用，如果超出，取余就可以了。
    }

    recv_buf[weblen] = 0;
    int head = pkg_data[0];

    // if(pkg_data[0]==0x81 || pkg_data[0]==0x82){
    if (head == 129 || head == 130)
    {
        memcpy(pkg_data, recv_buf, weblen);
        webret = weblen;
    }

    //else
    //{
    //    printf("the head is not : 0x81 or 0x82\n");
    //}
    return webret;
}

int ws_send_data(int s, unsigned char* pkg_data, unsigned int pkg_len, int sendtype)
{
    static unsigned char send_buffer[WBSIZE];
    unsigned int send_len;
    unsigned char *tmpbuf;
    unsigned char *maskBytes;
    // 固定的头
    send_buffer[0] = 0x82;

    if (pkg_len <= 125)
    {
        send_buffer[1] = pkg_len; // 最高bit为0，
        send_len = 2;
    }
    else if (pkg_len <= 0xffff)
    {
        send_buffer[1] = 126;
        send_buffer[2] = ((pkg_len & 0x0000ff00) >> 8);
        send_buffer[3] = (pkg_len & 0x000000ff);
        send_len = 4;
    }
    else
    {
        send_buffer[1] = 127;
        send_buffer[2] = 0;
        send_buffer[3] = 0;
        send_buffer[4] = 0;
        send_buffer[5] = 0;
        send_buffer[6] = ((pkg_len & 0xff000000) >> 24);
        send_buffer[7] = ((pkg_len & 0x00ff0000) >> 16);
        send_buffer[8] = ((pkg_len & 0x0000ff00) >> 8);
        send_buffer[9] = (pkg_len & 0x000000ff);
        send_len = 10;
    }

    send_buffer[1] |= 0x80;  //add mask;
    int i;

    for (i = 0; i < 4; i++)
    {
        send_buffer[send_len + i] = rand() % 256;
    }

    maskBytes = send_buffer + send_len;
    send_len += 4;
    tmpbuf = send_buffer + send_len;

    for (i = 0; i < pkg_len; i++, tmpbuf++)
    {
        *tmpbuf = (unsigned char)(pkg_data[i] ^ maskBytes[i % 4]);
    }

    send_len += pkg_len;

    if (sendtype == 0)
    {
        send(s, send_buffer, send_len, 0);
    }
    else
    {
        write(s, send_buffer, send_len);
    }

    return pkg_len;
}

void makesenddata(unsigned char* pkg_data, unsigned int pkg_len, unsigned char** packfinishData, int* packetfinishLen)
{
    static unsigned char send_buffer[WBSIZE];
    unsigned int send_len;
    unsigned char *tmpbuf;
    unsigned char *maskBytes;
    // 固定的头
    send_buffer[0] = 0x82;

    if (pkg_len <= 125)
    {
        send_buffer[1] = pkg_len; // 最高bit为0，
        send_len = 2;
    }
    else if (pkg_len <= 0xffff)
    {
        send_buffer[1] = 126;
        send_buffer[2] = ((pkg_len & 0x0000ff00) >> 8);
        send_buffer[3] = (pkg_len & 0x000000ff);
        send_len = 4;
    }
    else
    {
        send_buffer[1] = 127;
        send_buffer[2] = 0;
        send_buffer[3] = 0;
        send_buffer[4] = 0;
        send_buffer[5] = 0;
        send_buffer[6] = ((pkg_len & 0xff000000) >> 24);
        send_buffer[7] = ((pkg_len & 0x00ff0000) >> 16);
        send_buffer[8] = ((pkg_len & 0x0000ff00) >> 8);
        send_buffer[9] = (pkg_len & 0x000000ff);
        send_len = 10;
    }

    send_buffer[1] |= 0x80;  //add mask;
    int i;

    for (i = 0; i < 4; i++)
    {
        send_buffer[send_len + i] = rand() % 256;
    }

    maskBytes = send_buffer + send_len;
    send_len += 4;
    tmpbuf = send_buffer + send_len;

    for (i = 0; i < pkg_len; i++, tmpbuf++)
    {
        *tmpbuf = (unsigned char)(pkg_data[i] ^ maskBytes[i % 4]);
    }

    send_len += pkg_len;
    *packfinishData = send_buffer;
    *packetfinishLen = send_len;
}

int _readline(char* allbuf, int level, char* linebuf)
{
    int len = strlen(allbuf);

    for (; level < len; ++level)
    {
        if (allbuf[level] == '\r' && allbuf[level + 1] == '\n')
        {
            return level + 2;
        }
        else
        {
            *(linebuf++) = allbuf[level];
        }
    }

    return -1;
}



/*int base64_encode(char *in_str, int in_len, char *out_str)
{
    BIO *b64, *bio;
    BUF_MEM *bptr = NULL;
    size_t size = 0;

    if (in_str == NULL || out_str == NULL)
    {
        return -1;
    }

    b64 = BIO_new(BIO_f_base64());
    bio = BIO_new(BIO_s_mem());
    bio = BIO_push(b64, bio);
    BIO_write(bio, in_str, in_len);
    BIO_flush(bio);
    BIO_get_mem_ptr(bio, &bptr);
    memcpy(out_str, bptr->data, bptr->length);
    out_str[bptr->length - 1] = '\0';
    size = bptr->length;
    BIO_free_all(bio);
    return size;
}*/
#endif // _WEBSOCKET

void SetTcpBuf(int fd, fd_set *fds)
{
    int yes = 1;
    int result = fcntl(fd, F_SETFL, O_NONBLOCK);
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char *)&yes, sizeof yes); // reuse fix
//  setsockopt(fd,SOL_SOCKET,SO_REUSEPORT,(char *)&yes,sizeof yes); //reuse fix
    setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, (char *)&yes, sizeof yes); // reuse fix

    if (fd == -1)
    {
        perror("accept");
    }
    else
    {
        FD_SET(fd, fds);
    }

    result = fcntl(fd, F_SETFL, O_NONBLOCK);
}

#ifdef _SAME_IP_ONLINE_NUM
int SameIpOnlineNum(unsigned long ip)
{
#ifdef _CHECK_SEVER_IP

    if (checkServerIp(ip) == TRUE)
    {
        return 0;
    }

#endif
    int MAX_USER = getFdnum();
    int i, num = 0;

    if (getSameIpOnlineNum() > 0)
    {
        for (i = 4; i < MAX_USER; i++)
        {
            int i_use;
            i_use = CONNECT_getUse(i);

            if (i_use)
            {
                if (ip == CONNECT_get_userip(i))
                {
                    num++;

                    if (num >= getSameIpOnlineNum())
                    {
                        return 1;
                    }
                }
            }
        }
    }

    return 0;
}
#endif

#ifdef  _NO_ATTACK
int checkipsafe(unsigned long ip)
{
#ifdef _CHECK_SEVER_IP

    if (checkServerIp(ip) == TRUE)
    {
        return 0;
    }

#endif
    int MAX_USER = getFdnum();
    int i, num = 0;

    for (i = 4; i < MAX_USER; i++)
    {
        int i_use;
        i_use = CONNECT_getUse(i);

        if (i_use)
        {
            if (CONNECT_getState(i) == NULLCONNECT && ip == CONNECT_get_userip(i))
            {
                num++;
            }

            if (num >= 2)
            {
                return 1;
            }
        }
    }

    return 0;
}
#endif

/*------------------------------------------------------------
 * servstate毛赓渝祭允月﹝
 * 娄醒﹜忒曰袄
 *  卅仄
 ------------------------------------------------------------*/
ANYTHREAD static void SERVSTATE_initserverState(void)
{
    SERVSTATE_LOCK();
    servstate.acceptmore = TRUE;
    servstate.fdid = 0;
    servstate.closeallsocketnum = -1;
    servstate.shutdown = 0;
    servstate.limittime = 0;
    servstate.dsptime = 0;
    SERVSTATE_UNLOCK();
}

ANYTHREAD int SERVSTATE_SetAcceptMore(int nvalue)
{
    BOOL buf;
    SERVSTATE_LOCK();
    buf = servstate.acceptmore;
    servstate.acceptmore = nvalue;
    SERVSTATE_UNLOCK();
    return buf;
}
ANYTHREAD static int SERVSTATE_incrementFdid(void)
{
    int ret;
    SERVSTATE_LOCK();
    ret = servstate.fdid++;
    SERVSTATE_UNLOCK();
    return ret;
}
ANYTHREAD static void SERVSTATE_setCloseallsocketnum(int a)
{
    SERVSTATE_LOCK();
    servstate.closeallsocketnum = a;
    SERVSTATE_UNLOCK();
}
ANYTHREAD static void SERVSTATE_incrementCloseallsocketnum(void)
{
    SERVSTATE_LOCK();
    servstate.closeallsocketnum ++;
    SERVSTATE_UNLOCK();
}
ANYTHREAD void SERVSTATE_decrementCloseallsocketnum(void)
{
    SERVSTATE_LOCK();
    servstate.closeallsocketnum --;
    SERVSTATE_UNLOCK();
}
ANYTHREAD int SERVSTATE_getCloseallsocketnum(void)
{
    int a;
    SERVSTATE_LOCK();
    a = servstate.closeallsocketnum;
    SERVSTATE_UNLOCK();
    return a;
}

ANYTHREAD static int SERVSTATE_getAcceptmore(void)
{
    int a;
    SERVSTATE_LOCK();
    a = servstate.acceptmore;
    SERVSTATE_UNLOCK();
    return a;
}
ANYTHREAD int SERVSTATE_getShutdown(void)
{
    int a;
    SERVSTATE_LOCK();
    a = servstate.shutdown;
    SERVSTATE_UNLOCK();
    return a;
}
ANYTHREAD void SERVSTATE_setShutdown(int a)
{
    SERVSTATE_LOCK();
    servstate.shutdown = a;
    SERVSTATE_UNLOCK();
}
ANYTHREAD int SERVSTATE_getLimittime(void)
{
    int a;
    SERVSTATE_LOCK();
    a = servstate.limittime;
    SERVSTATE_UNLOCK();
    return a;
}
ANYTHREAD void SERVSTATE_setLimittime(int a)
{
    SERVSTATE_LOCK();
    servstate.limittime = a;
    SERVSTATE_UNLOCK();
}
ANYTHREAD int SERVSTATE_getDsptime(void)
{
    int a;
    SERVSTATE_LOCK();
    a = servstate.dsptime;
    SERVSTATE_UNLOCK();
    return a;
}
ANYTHREAD void SERVSTATE_setDsptime(int a)
{
    SERVSTATE_LOCK();
    servstate.dsptime = a;
    SERVSTATE_UNLOCK();
}

static int appendWB(int fd, char *buf, int size)
{
    if (fd != acfd)
    {
        if (Connect[ fd ].wbuse + size >= WBSIZE)
        {
            print("appendWB:err buffer over[%d]:%s \n",
                  Connect[ fd ].wbuse + size, Connect[ fd ].cdkey);
            return -1;
        }
    }
    else
    {
        if (Connect[ fd ].wbuse + size > AC_WBSIZE)
        {
            /*
                  FILE * fp = NULL;

                  if ( ( fp = fopen( "appendWBerr.log", "a+" ) ) == NULL ) return -1;

                  fprintf( fp, "(SAAC) appendWB:err buffer over[%d+%d/%d]:\n", Connect[ fd ].wbuse, size, AC_WBSIZE );

                  fclose( fp );
            */
            //print( "appendWB:err buffer over[%d+%d]:(SAAC) \n", Connect[ fd ].wbuse, size );
            return -1;
        }
    }

#ifdef _WEBSOCKET

    if (fd != acfd)
    {
        if (Connect[fd].clitype == 2)
        {
            int packetLen, packetfinishLen = 0;
            char* packfinishData;
            makesenddata(buf, size, &packfinishData, &packetfinishLen);
            memcpy(Connect[fd].wb + Connect[fd].wbuse,
                   packfinishData, packetfinishLen);
            Connect[fd].wbuse += packetfinishLen;
            return packetfinishLen;
        }
    }

#endif
    memcpy(Connect[ fd ].wb + Connect[ fd ].wbuse,
           buf, size);
    Connect[ fd ].wbuse += size;
    return size;
}

static int appendRB(int fd, char *buf, int size)
{
    if (fd != acfd)
    {
        if (Connect[ fd ].rbuse + size > RBSIZE)
        {
            //print( "appendRB:OTHER(%d) err buffer over \n", fd );
            return -1;
        }
    }
    else
    {
        //if (strlen(buf) > size)
        //{
        //    print("appendRB AC buffer len err : %d/%d=\n", strlen(buf), size);
        //}
        if (Connect[ fd ].rbuse + size > AC_RBSIZE)
        {
            printf("%s\n", buf);
            print("appendRB AC err buffer over: len:%d - AC_RBSIZE:%d \n",
                  Connect[ fd ].rbuse + size, AC_RBSIZE);
            return -1;
        }
    }

    memcpy(Connect[ fd ].rb + Connect[ fd ].rbuse, buf, size);
    Connect[ fd ].rbuse += size;
    return size;
}

static int shiftWB(int fd, int len)
{
    if (Connect[ fd ].wbuse < len)
    {
        print("shiftWB: err\n");
        return -1;
    }

    memmove(Connect[ fd ].wb, Connect[ fd ].wb + len, Connect[ fd ].wbuse - len);
    Connect[ fd ].wbuse -= len;

    if (Connect[ fd ].wbuse < 0)
    {
        print("shiftWB:wbuse err\n");
        Connect[ fd ].wbuse = 0;
    }

    return len;
}

static int shiftRB(int fd, int len)
{
    if (Connect[ fd ].rbuse < len)
    {
        print("shiftRB: err\n");
        return -1;
    }

    memmove(Connect[ fd ].rb, Connect[ fd ].rb + len, Connect[ fd ].rbuse - len);
    Connect[ fd ].rbuse -= len;

    if (Connect[ fd ].rbuse < 0)
    {
        print("shiftRB:rbuse err\n");
        Connect[ fd ].rbuse = 0;
    }

    return len;
}

SINGLETHREAD int lsrpcClientWriteFunc(int fd, char* buf, int size)
{
    int r;

    if (Connect[ fd ].use == FALSE)
    {
        return FALSE;
    }

    if (Connect[ fd ].appendwb_overflow_flag)
    {
        print("lsrpcClientWriteFunc: buffer overflow fd:%d\n", fd);
        return -1;
    }

    r = appendWB(fd, buf, size);
    // Nuke *1 0907: Ignore acfd from WB error

    if ((r < 0) && (fd != acfd))
    {
        Connect[ fd ].appendwb_overflow_flag = 1;
        CONNECT_endOne_debug(fd);
        // Nuke + 1 0901: Why close
        //  print("closed in lsrpcClientWriteFunc");
    }

    return r;
}

static int logRBuseErr = 0;
SINGLETHREAD int GetOneLine_fix(int fd, char *buf, int max)
{
    int i;

    if (Connect[ fd ].rbuse == 0)
    {
        return FALSE;
    }

    if (Connect[ fd ].check_rb_oneline_b == 0 &&
            Connect[ fd ].check_rb_oneline_b == Connect[ fd ].rbuse)
    {
        return FALSE;
    }

    for (i = 0; i < Connect[ fd ].rbuse && i < (max - 1); i ++)
    {
#ifdef _NEW_NETSENDRECV
        BOOL flg = FALSE;
        int len = 0;

        if ((unsigned char)Connect[fd].rb[i] == 0x97)
        {
            len = *(int *)(Connect[fd].rb + i + 1);

            if (len > 0 && (i + 6 + len) <= Connect[fd].rbuse)
            {
                if (*(unsigned char *)(Connect[fd].rb + i + 5 + len) == 0xE5)
                {
                    flg = TRUE;
                }
                else
                {
                    memset(Connect[fd].rb, 0, sizeof(Connect[fd].rb));
                    Connect[fd].rbuse = 0;
                    logRBuseErr = 0;
                    return 0;
                }
            }
            else
            {
                break;
            }
        }

        if (flg == TRUE)
        {
            memcpy(buf, Connect[fd].rb + 5 + i, len);
            buf[len] = '\0';
            shiftRB(fd, i + 6 + len);
            logRBuseErr = 0;
            Connect[fd].check_rb_oneline_b = 0;
            Connect[fd].check_rb_time = 0;
            return len;
        }

#else

        if (Connect[ fd ].rb[ i ] == '\n')
        {
            memcpy(buf, Connect[ fd ].rb, i + 1);
            buf[ i + 1 ] = '\0';
            shiftRB(fd, i + 1);
            //--------
            /*
               //andy_log
               if( strstr( Connect[fd].rb , "ACCharLoad") != NULL &&
                Connect[fd].check_rb_oneline_b != 0 )//Connect[fd].rb
                LogAcMess( fd, "GetOne", Connect[fd].rb );
            */
            //--------
            logRBuseErr = 0;
            Connect[ fd ].check_rb_oneline_b = 0;
            Connect[ fd ].check_rb_time = 0;
            return TRUE;
        }

#endif
    }

    if (fd == acfd)
    {
        logRBuseErr++;
    }

    if (logRBuseErr >= 3)
    {
        printf("rebuse err %d:%d\n", logRBuseErr, Connect[fd].rbuse);
#ifdef _NEW_NETSENDRECV
        memset(Connect[fd].rb, 0, sizeof(Connect[fd].rb));
        Connect[fd].rbuse = 0;
#else
        Connect[ fd ].rb[Connect[ fd ].rbuse] = '\n';
#endif
        logRBuseErr = 0;
    }

    //--------
    //andy_log
    /*if (fd == acfd && strstr(Connect[ fd ].rb, "ACCharLoad") != NULL &&
            logRBuseErr >= 50)    //Connect[fd].rb
    {
        memcpy(buf, Connect[ fd ].rb, Connect[ fd ].rbuse + 1);
        buf[ Connect[ fd ].rbuse + 1 ] = 0;
        //LogAcMess(fd, "RBUFFER", rbuf);
        logRBuseErr = 0;
    }*/
    //--------
    Connect[ fd ].check_rb_oneline_b = Connect[ fd ].rbuse;
    return 0;
}
#ifdef _NEW_NETSENDRECV
SINGLETHREAD int getLineFromReadBuf(int fd, char *buf, int max)
{
    int i;

    if (Connect[fd].rbuse == 0)
    {
        return 0;
    }

    if (Connect[fd].check_rb_oneline_b == 0 &&
            Connect[fd].check_rb_oneline_b == Connect[fd].rbuse)
    {
        return 0;
    }

#ifdef _WEBSOCKET

    if (Connect[fd].clitype == 2)
    {
        int packetLen, packetfinishLen = 0;
        char* packfinishData;
        int recvtype = ReceiveData(Connect[fd].rb, Connect[fd].rbuse, &packetLen, &packfinishData, &packetfinishLen);

        if (recvtype == 0)
        {
            memset(Connect[fd].rb, 0, RBSIZE);
            Connect[fd].rbuse = 0;
            logRBuseErr = 0;
        }
        else if (recvtype == 2)
        {
            for (i = 0; i < packetfinishLen && i < (max - 1); i++)
            {
                int flg = 0;
                int len = 0;
#ifdef _175_SA_2

                if ((unsigned char)packfinishData[i] == 0xB0)
                {
                    len = *(short*)(packfinishData + i + 1);

                    if (len > 0 && (i + 4 + len) <= packetfinishLen && *(unsigned char *)(packfinishData + i + 3 + len) == 0xA7)
                    {
                        flg = 1;
                    }
                }

                if (flg == 1)
                {
                    memcpy(buf, packfinishData + 3 + i, len);
                    buf[len] = '\0';
                    shiftRB(fd, packetLen);
                    logRBuseErr = 0;
                    Connect[fd].check_rb_oneline_b = 0;
                    Connect[fd].check_rb_time = 0;
                    return len;
                }

#else

                if ((unsigned char)packfinishData[i] == 0x78)
                {
                    len = *(int*)(packfinishData + i + 1);
                    len ^= 15236412;

                    if (len > 0 && (i + 6 + len) <= packetfinishLen && *(unsigned char *)(packfinishData + i + 5 + len) == 0xD7)
                    {
                        flg = 1;
                    }
                }

                if (flg == 1)
                {
                    memcpy(buf, packfinishData + 5 + i, len);
                    buf[len] = '\0';
                    shiftRB(fd, packetLen);
                    logRBuseErr = 0;
                    Connect[fd].check_rb_oneline_b = 0;
                    Connect[fd].check_rb_time = 0;
                    return len;
                }

#endif
            }
        }
    }
    else
#endif // _WEBSOCKET
        for (i = 0; i < Connect[fd].rbuse && i < (max - 1); i++)
        {
            int flg = 0;
            int len = 0;
#ifdef _175_SA_2

            if ((unsigned char)Connect[fd].rb[i] == 0xB0)
            {
                len = *(short*)(Connect[fd].rb + i + 1);

                if (len > 0 && (i + 4 + len) <= Connect[fd].rbuse && *(unsigned char *)(Connect[fd].rb + i + 3 + len) == 0xA7)
                {
                    flg = 1;
                }
            }

            if (flg == 1)
            {
                memcpy(buf, Connect[fd].rb + 3 + i, len);
                buf[len] = '\0';
                shiftRB(fd, i + 4 + len);
                logRBuseErr = 0;
                Connect[fd].check_rb_oneline_b = 0;
                Connect[fd].check_rb_time = 0;
                return len;
            }

#else

            if ((unsigned char)Connect[fd].rb[i] == 0x78)
            {
                len = *(int*)(Connect[fd].rb + i + 1);
                len ^= 15236412;

                if (len > 0 && (i + 6 + len) <= Connect[fd].rbuse && *(unsigned char *)(Connect[fd].rb + i + 5 + len) == 0xD7)
                {
                    flg = 1;
                }
            }

            if (flg == 1)
            {
                memcpy(buf, Connect[fd].rb + 5 + i, len);
                buf[len] = '\0';
                shiftRB(fd, i + 6 + len);
                logRBuseErr = 0;
                Connect[fd].check_rb_oneline_b = 0;
                Connect[fd].check_rb_time = 0;
                return len;
            }

#endif
        }

    if (logRBuseErr >= 50)
    {
        Connect[fd].rb[Connect[fd].rbuse] = '\n';
        printf("rebuse err %d:%d\n", logRBuseErr, Connect[fd].rbuse);
        logRBuseErr = 0;
    }

    Connect[fd].check_rb_oneline_b = Connect[fd].rbuse;
    return 0;
}

#endif

char *getRandStr(int num)
{
    char str[num + 1];
    int i;
    struct timeval tv;
    struct timezone tz;
    gettimeofday(&tv, &tz);
    srand(tv.tv_usec);

    for (i = 0; i < num; i++)
    {
        int a = 0;
        a = (rand() % 2) ? (rand() % 26 + 65) : (rand() % 10 + 48);
        str[i] = a;
    }

    str[num] = '\0';
    return str;
}

ANYTHREAD BOOL initConnectOne(int sockfd, struct sockaddr_in* sin, int len)
{
    CONNECT_LOCK(sockfd);
#ifdef _EQUIPMENT_TYPE_
    Connect[sockfd].newworkindex = 1;
#endif
    Connect[ sockfd ].use = TRUE;
    Connect[ sockfd ].ctype = NOTDETECTED;
    Connect[ sockfd ].wbuse = Connect[ sockfd ].rbuse = 0;
    Connect[ sockfd ].check_rb_oneline_b = 0;
    Connect[ sockfd ].check_rb_time = 0;
    memset(Connect[ sockfd ].cdkey, 0, sizeof(Connect[ sockfd ].cdkey));
    memset(Connect[ sockfd ].passwd, 0, sizeof(Connect[ sockfd ].passwd));
    Connect[ sockfd ].state = NULLCONNECT;
    Connect[ sockfd ].nstatecount = 0;
    memset(Connect[ sockfd ].charname, 0, sizeof(Connect[ sockfd ].charname));
    Connect[ sockfd ].charaindex = -1;
    Connect[ sockfd ].CAbufsiz = 0;
    Connect[ sockfd ].CDbufsiz = 0;
    memset(Connect[ sockfd ].rb, 0, RBSIZE);
    memset(Connect[ sockfd ].wb, 0, WBSIZE);
    Connect[ sockfd ].rbuse = 0;
    Connect[ sockfd ].wbuse = 0;
    Connect[ sockfd ].check_rb_oneline_b = 0;
    Connect[ sockfd ].check_rb_time = 0;
    Connect[ sockfd ].close_request = 0;      /* 濠蝇邰菲白仿弘 */
    // Nuke 08/27 For acceleration avoidance
    Connect[ sockfd ].Walktime = 0;
    Connect[ sockfd ].lastWalktime = 0;
    Connect[ sockfd ].Walkcount = 0;
    Connect[ sockfd ].Walkspool = WALK_SPOOL;
    Connect[ sockfd ].Walkrestore = WALK_RESTORE;
    Connect[ sockfd ].Btime = 0;
    Connect[ sockfd ].lastBtime = 0;
    Connect[ sockfd ].lastlastBtime = 0;
    Connect[ sockfd ].EOtime = 0;
    Connect[ sockfd ].nu_decrease = 0;
#ifdef _BATTLE_TIMESPEED
    // Connect[sockfd].DefBtime = 0;
    Connect[ sockfd ].BDTime = 0;
    Connect[ sockfd ].CBTime = 0;
#endif
#ifdef _TYPE_TOXICATION
    Connect[ sockfd ].toxication = 0;
#endif
#ifdef _ITEM_ADDEXP //vincent 经验提升
    Connect[ sockfd ].EDTime = 0;
#endif
    //      Connect[sockfd].BEO = 0;
    Connect[ sockfd ].BEOspool = BEO_SPOOL;
    Connect[ sockfd ].BEOrestore = BEO_RESTORE;
    //ttom
    Connect[ sockfd ].b_shut_up = FALSE;
    Connect[ sockfd ].Wtime.tv_sec = 0; //
    Connect[ sockfd ].Wtime.tv_usec = 0; //
    Connect[ sockfd ].WLtime.tv_sec = 0; //
    Connect[ sockfd ].WLtime.tv_usec = 0; //
    Connect[ sockfd ].b_first_warp = FALSE;
    Connect[ sockfd ].state_trans = 0; //avoid the trans
    Connect[ sockfd ].credit = 3;
    Connect[ sockfd ].fcold = 0;
    // Nuke 0406: New Flow Control
    Connect[ sockfd ].nu = 30;
    // Nuke 1213: Flow Control 2
    Connect[ sockfd ].packetin = 30; // if 10x10 seconds no packet, drop the line
    // Nuke 0624: Avoid Useless Connection
    Connect[ sockfd ].cotime = 0;
    // Nuke 0626: For no enemy
    Connect[ sockfd ].noenemy = 0;
    // Arminius 7.2: Ra's amulet
    Connect[ sockfd ].eqnoenemy = 0;
#ifdef _Item_MoonAct
    Connect[ sockfd ].eqrandenemy = 0;
#endif
#ifdef _CHIKULA_STONE
    Connect[ sockfd ].chistone = 0;
#endif
    // Arminius 7.31: cursed stone
    Connect[ sockfd ].stayencount = 0;
    // CoolFish: Init Trade 2001/4/18
    memset(&Connect[ sockfd ].TradeTmp, 0, sizeof(Connect[ sockfd ].TradeTmp));
#ifdef _ITEM_PILEFORTRADE
    Connect[ sockfd ].tradelist = -1;
#endif
    // Arminius 6.22 Encounter
    Connect[ sockfd ].CEP = 0;
    // Arminius 7.12 login announce
    Connect[ sockfd ].announced = 0;
#ifdef _IPHONE_MARK_
    Connect[sockfd].loginmark = 0;
#endif
#ifdef _NEW_FUNC_DECRYPT
    Connect[sockfd].newerrnum = 0;
#endif
    Connect[ sockfd ].confirm_key = FALSE;   // shan trade(DoubleCheck)
#ifdef _ITEM_PET_TALK_
    memset(Connect[sockfd].pettalk, 0, sizeof(Connect[sockfd].pettalk));
    memset(Connect[sockfd].pettalkdata, 0, sizeof(Connect[sockfd].pettalkdata));
#endif

    if (sin != NULL)
    {
        memcpy(&Connect[ sockfd ].sin, sin, len);
    }

    memset(&Connect[ sockfd ].lastprocesstime, 0,
           sizeof(Connect[ sockfd ].lastprocesstime));
    memcpy(&Connect[ sockfd ].lastCAsendtime, &NowTime,
           sizeof(Connect[ sockfd ].lastCAsendtime));
    memcpy(&Connect[ sockfd ].lastCDsendtime, &NowTime,
           sizeof(Connect[ sockfd ].lastCDsendtime));
    memcpy(&Connect[ sockfd ].lastCharSaveTime, &NowTime,
           sizeof(Connect[ sockfd ].lastCharSaveTime));
    // Shan Add
    memcpy(&Connect[ sockfd ].lastrecvtime, &NowTime,
           sizeof(Connect[ sockfd ].lastrecvtime));
    memcpy(&Connect[ sockfd ].lastrecvtime_d, &NowTime,
           sizeof(Connect[ sockfd ].lastrecvtime_d));
    memcpy(&Connect[ sockfd ].battle_recvtime, &NowTime,
           sizeof(Connect[ sockfd ].battle_recvtime));
    memcpy(&Connect[ sockfd ].lastreadtime, &NowTime,
           sizeof(struct timeval));
    Connect[ sockfd ].lastreadtime.tv_sec -= DEBUG_ADJUSTTIME;
    Connect[ sockfd ].errornum = 0;
    Connect[ sockfd ].fdid = SERVSTATE_incrementFdid();
    CONNECT_UNLOCK(sockfd);
    Connect[ sockfd ].appendwb_overflow_flag = 0;
    Connect[sockfd].connecttime = time(NULL);
    memset(Connect[ sockfd ].mac, 0, sizeof(Connect[ sockfd ].mac));
    memset(Connect[ sockfd ].mac2, 0, sizeof(Connect[ sockfd ].mac2));
    memset(Connect[ sockfd ].mac3, 0, sizeof(Connect[ sockfd ].mac3));
    memset(Connect[ sockfd ].defaultkey, 0, sizeof(Connect[ sockfd ].defaultkey));
    memset(Connect[ sockfd ].defaultdeskey, 0, sizeof(Connect[ sockfd ].defaultdeskey));
    int deskeyrand = RAND(1, 100000);
    strcpy(Connect[ sockfd ].defaultkey, firstdesToken[deskeyrand - 1]);
    strcpy(Connect[ sockfd ].defaultdeskey, seconddesToken[deskeyrand - 1]);
#ifdef _SERVER_NP_
    Connect[sockfd].pbUser = CSAuth3_CreateInstance();
    CSAuth3_Init(Connect[sockfd].pbUser, TRUE);
    Connect[sockfd].nptime = time(NULL);
    Connect[sockfd].npcnt = 0;
#endif
#ifdef _NEW_NETSENDRECV
    Connect[sockfd].funcnum = 0;
    memset(Connect[sockfd].newdefaultkey, 0, sizeof(Connect[sockfd].newdefaultkey));
#endif
#ifdef _WEBSOCKET
    Connect[sockfd].clitype = 0;
#endif
    return TRUE;
}

ANYTHREAD BOOL _CONNECT_endOne(char *file, int fromline, int sockfd, int line)
{
    CONNECT_LOCK_ARG2(sockfd, line);

    if (Connect[sockfd].use == FALSE)
    {
        CONNECT_UNLOCK_ARG2(sockfd, line);
        //andy_log
        //print("连接已关闭[%d]!!\n", sockfd );
        return TRUE;
    }

    Connect[sockfd].use = FALSE;

    if (Connect[sockfd].ctype == CLI && CHAR_CHECKINDEX(Connect[sockfd].charaindex) == TRUE)
    {
        CONNECT_UNLOCK_ARG2(sockfd, line);

        if (CHAR_getWorkInt(Connect[sockfd].charaindex, CHAR_WORKTRADEMODE) != CHAR_TRADE_FREE)
        {
            TRADE_Close(sockfd, Connect[sockfd].charaindex, "w|-1");
        }

#ifdef _OFFLINE_SYSTEM
        //山猪增加摆摊真正退出
#ifdef  _OFFLINE_SYSTEM_FORMVENDOR

        if (CHAR_getWorkInt(Connect[sockfd].charaindex, CHAR_WORKSTREETVENDOR) == 1 &&
                CHAR_getWorkInt(Connect[sockfd].charaindex, CHAR_WORK_OFFLINE) == 0
           )
        {
            if (!CHAR_logout(Connect[sockfd].charaindex, TRUE))
            {
                print("退出账号=%s \n", Connect[sockfd].cdkey);
            }
        }

#endif

        if (CHAR_getWorkInt(Connect[sockfd].charaindex, CHAR_WORK_OFFLINE) != 0)
        {
            CHAR_setWorkInt(Connect[sockfd].charaindex, CHAR_WORKFD, -1);
        }

#ifdef _OFFLINE_BATTLE_RETEUN
        else if (CHAR_getWorkInt(Connect[sockfd].charaindex, CHAR_WORKBATTLEINDEX) > -1
                 && (BattleArray[CHAR_getWorkInt(Connect[sockfd].charaindex, CHAR_WORKBATTLEINDEX)].type == BATTLE_TYPE_P_vs_P
                     || (CHAR_getInt(Connect[sockfd].charaindex, CHAR_FLOOR) >= 40000 && CHAR_getInt(Connect[sockfd].charaindex, CHAR_FLOOR) <= 40034))
                 && CHAR_getFlg(Connect[sockfd].charaindex, CHAR_ISDIE) != 1)
        {
            //print("\nPK或者魔王掉线。");
            CHAR_setWorkInt(Connect[sockfd].charaindex, CHAR_WORK_OFFLINE, 1);
            CHAR_setWorkInt(Connect[sockfd].charaindex, CHAR_WORKFD, -1);
        }

#endif
        else
#endif
        {
#ifdef  _OFFLINE_RETEUN

            //print("\n普通掉线。");
            if (CHAR_getWorkInt(Connect[sockfd].charaindex, CHAR_WORKBATTLEINDEX) > -1 && BattleArray[CHAR_getWorkInt(Connect[sockfd].charaindex, CHAR_WORKBATTLEINDEX)].type == BATTLE_TYPE_WATCH || CHAR_getWorkInt(Connect[sockfd].charaindex, CHAR_WATCHBATTLETYPE) == 1)
            {
                CHAR_logout(Connect[sockfd].charaindex, TRUE);
                print("退出账号=%s \n", Connect[sockfd].cdkey);
            }
            else if (CHAR_getWorkInt(Connect[sockfd].charaindex, CHAR_WORKBATTLEINDEX) > -1 && CHAR_getFlg(Connect[sockfd].charaindex, CHAR_ISDIE) == TRUE)
            {
                CHAR_logout(Connect[sockfd].charaindex, TRUE);
                print("退出账号=%s \n", Connect[sockfd].cdkey);
            }
            else
            {
                if (getStayEncount(sockfd) == 1)
                {
                    CHAR_setWorkInt(Connect[sockfd].charaindex, CHAR_WORK_OFFLINE, 3);
                }
                else
                {
                    CHAR_setWorkInt(Connect[sockfd].charaindex, CHAR_WORK_OFFLINE, 2);
                }

                CHAR_setWorkInt(Connect[sockfd].charaindex, CHAR_WORKFD, -1);
                CHAR_setWorkInt(Connect[sockfd].charaindex, CHAR_WORK_LOGIN_TIME, time(NULL));
                CHAR_setInt(Connect[sockfd].charaindex, CHAR_LASTLEAVETIME, (int)time(NULL));
                CHAR_charSaveFromConnect(Connect[sockfd].charaindex, FALSE);
            }

#else

            if (!CHAR_logout(Connect[sockfd].charaindex, TRUE))
            {
                //print( "err %s:%d from %s:%d \n", __FILE__, __LINE__, file, fromline);
            }

            print("退出账号=%s \n", Connect[sockfd].cdkey);
#endif
        }

        CONNECT_LOCK_ARG2(sockfd, line);
    }
    else
    {
        if (strlen(Connect[sockfd].cdkey) > 0)
        {
            if (sasql_queryonline(Connect[sockfd].cdkey) == getServernumber())
            {
                int i;
                int zhaotype = 0;

                for (i = 0; i < CHAR_getPlayerMaxNum(); i++)
                {
                    if (CHAR_getCharUse(i) == FALSE)
                    {
                        continue;
                    }

                    if (strcmp(CHAR_getChar(i, CHAR_CDKEY), Connect[sockfd].cdkey) == 0)
                    {
                        if (CHAR_getWorkInt(i, CHAR_WORK_DWTEMP) == 1)
                        {
                            zhaotype = 1;
                        }

                        break;
                    }
                }

                if (zhaotype == 0)
                {
                    sasql_online(Connect[sockfd].cdkey, NULL, NULL, NULL, NULL, NULL, 0);
                }
            }
        }
    }

    Connect[sockfd].charaindex = -1;
    Connect[sockfd].wbuse = 0;
    Connect[sockfd].rbuse = 0;
    Connect[sockfd].CAbufsiz = 0;
    Connect[sockfd].CDbufsiz = 0;
    memset(Connect[ sockfd ].mac, 0, sizeof(Connect[ sockfd ].mac));
    memset(Connect[ sockfd ].mac2, 0, sizeof(Connect[ sockfd ].mac2));
    memset(Connect[ sockfd ].mac3, 0, sizeof(Connect[ sockfd ].mac3));
#ifdef _SERVER_NP_
    CSAuth3_Close(Connect[sockfd].pbUser);
    CSAuth3_Release(Connect[sockfd].pbUser);
#endif
    CONNECT_UNLOCK_ARG2(sockfd, line);
    //print( "编号=%d %s 来自文件:%s 行数:%d\n", sockfd,inet_ntoa(Connect[sockfd].sin.sin_addr ), file, fromline);
    close(sockfd);
    return TRUE;
}
SINGLETHREAD BOOL initConnect(int size)
{
    int i, j;
    ConnectLen = size;
    Connect = calloc(1, sizeof(CONNECT) * size);

    if (Connect == NULL)
    {
        return FALSE;
    }

    for (i = 0 ; i < size ; i ++)
    {
        memset(&Connect[ i ], 0, sizeof(CONNECT));
        Connect[ i ].charaindex = -1;
        Connect[ i ].rb = calloc(1, RBSIZE);

        if (Connect[ i ].rb == NULL)
        {
            fprint("calloc err\n");

            for (j = 0; j < i ; j ++)
            {
                free(Connect[ j ].rb);
                free(Connect[ j ].wb);
            }

            return FALSE;
        }

        memset(Connect[ i ].rb, 0, RBSIZE);
        Connect[ i ].wb = calloc(1, WBSIZE);

        if (Connect[ i ].wb == NULL)
        {
            fprint("calloc err\n");

            for (j = 0; j < i ; j ++)
            {
                free(Connect[ j ].rb);
                free(Connect[ j ].wb);
            }

            free(Connect[ j ].rb);
            return FALSE;
        }

        memset(Connect[ i ].wb, 0, WBSIZE);
    }

    print("预约 %d 接连...分配 %.2f MB 空间...", size, (sizeof(CONNECT) * size + RBSIZE * size + WBSIZE * size) / 1024.0 / 1024.0);
    SERVSTATE_initserverState();
    //ttom for the performance of gmsv
    MAX_item_use = getItemnum() * 0.98;
    return TRUE;
}
BOOL CONNECT_acfdInitRB(int fd)
{
    if (fd != acfd)
    {
        return FALSE;
    }

    Connect[ fd ].rb = realloc(Connect[ acfd ].rb, AC_RBSIZE);

    if (Connect[ acfd ].rb == NULL)
    {
        fprint("realloc err\n");
        return FALSE;
    }

    memset(Connect[ acfd ].rb, 0, AC_RBSIZE);
    return TRUE;
}
BOOL CONNECT_acfdInitWB(int fd)
{
    if (fd != acfd)
    {
        return FALSE;
    }

    Connect[ fd ].wb = realloc(Connect[ acfd ].wb, AC_WBSIZE);

    if (Connect[ acfd ].wb == NULL)
    {
        fprint("realloc err\n");
        return FALSE;
    }

    memset(Connect[ acfd ].wb, 0, AC_WBSIZE);
    return TRUE;
}

ANYTHREAD void endConnect(void)
{
    int i;

    for (i = 0 ; i < ConnectLen ; i ++)
    {
        int lco;
        lco = close(i);

        if (lco == 0)
        {
            CONNECT_endOne_debug(i);
        }

        free(Connect[ i ].rb);
        free(Connect[ i ].wb);
    }

    free(Connect);
}

ANYTHREAD BOOL CONNECT_appendCAbuf(int fd, char* data, int size)
{
#ifdef _MASK_ENCOUNTER
    int charaindex = CONNECT_getCharaindex(fd);

    if (CHAR_getWorkInt(charaindex, CHAR_WORKPARTYMODE) != CHAR_PARTY_CLIENT)
    {
        if (getStayEncount(fd))
        {
            return FALSE;
        }
    }
    else
    {
        int oyaindex = CHAR_getWorkInt(charaindex, CHAR_WORKPARTYINDEX1);

        if (getStayEncount(getfdFromCharaIndex(oyaindex)))
        {
            return FALSE;
        }
    }

#endif
    CONNECT_LOCK(fd);

    /*  呵稿のデリミタの ',' の尸驴く澄瘦しないかん祸に庙罢   */
    if ((Connect[ fd ].CAbufsiz + size) >= sizeof(Connect[ fd ].CAbuf))
    {
        CONNECT_UNLOCK(fd);
        return FALSE;
    }

    memcpy(Connect[ fd ].CAbuf + Connect[ fd ].CAbufsiz, data, size);
    Connect[ fd ].CAbuf[ Connect[ fd ].CAbufsiz + size ] = ',';
    Connect[ fd ].CAbufsiz += (size + 1);
    CONNECT_UNLOCK(fd);
    return TRUE;
}

ANYTHREAD static int CONNECT_getCAbuf(int fd, char *out, int outmax,
                                      int *outlen)
{
    CONNECT_LOCK(fd);

    if (Connect[ fd ].use == TRUE)
    {
        int cplen = MIN(outmax, Connect[ fd ].CAbufsiz);
        memcpy(out, Connect[ fd ].CAbuf, cplen);
        *outlen = cplen;
        CONNECT_UNLOCK(fd);
        return 0;
    }
    else
    {
        CONNECT_UNLOCK(fd);
        return -1;
    }
}
ANYTHREAD static int CONNECT_getCDbuf(int fd, char *out, int outmax,
                                      int *outlen)
{
    CONNECT_LOCK(fd);

    if (Connect[ fd ].use == TRUE)
    {
        int cplen = MIN(outmax, Connect[ fd ].CDbufsiz);
        memcpy(out, Connect[ fd ].CDbuf, cplen);
        *outlen = cplen;
        CONNECT_UNLOCK(fd);
        return 0;
    }
    else
    {
        CONNECT_UNLOCK(fd);
        return 0;
    }
}

ANYTHREAD static int CONNECT_setCAbufsiz(int fd, int len)
{
    CONNECT_LOCK(fd);

    if (Connect[ fd ].use == TRUE)
    {
        Connect[ fd ].CAbufsiz = len;
        CONNECT_UNLOCK(fd);
        return 0;
    }
    else
    {
        CONNECT_UNLOCK(fd);
        return -1;
    }
}
ANYTHREAD static int CONNECT_setCDbufsiz(int fd, int len)
{
    CONNECT_LOCK(fd);

    if (Connect[ fd ].use == TRUE)
    {
        Connect[ fd ].CDbufsiz = len;
        CONNECT_UNLOCK(fd);
        return 0;
    }
    else
    {
        CONNECT_UNLOCK(fd);
        return -1;
    }
}

ANYTHREAD static void CONNECT_setLastCAsendtime(int fd, struct timeval *t)
{
    CONNECT_LOCK(fd);
    Connect[ fd ].lastCAsendtime = *t;
    CONNECT_UNLOCK(fd);
}
ANYTHREAD static void CONNECT_getLastCAsendtime(int fd, struct timeval *t)
{
    CONNECT_LOCK(fd);
    *t = Connect[ fd ].lastCAsendtime;
    CONNECT_UNLOCK(fd);
}
ANYTHREAD static void CONNECT_setLastCDsendtime(int fd, struct timeval *t)
{
    CONNECT_LOCK(fd);
    Connect[ fd ].lastCDsendtime = *t;
    CONNECT_UNLOCK(fd);
}
ANYTHREAD static void CONNECT_getLastCDsendtime(int fd, struct timeval *t)
{
    CONNECT_LOCK(fd);
    *t = Connect[ fd ].lastCDsendtime;
    CONNECT_UNLOCK(fd);
}
ANYTHREAD int CONNECT_getUse_debug(int fd, int i)
{
    int a;

    if (0 > fd ||  fd >= ConnectLen)
    {
        return  0;
    }

    CONNECT_LOCK_ARG2(fd, i);
    a = Connect[ fd ].use;
    CONNECT_UNLOCK_ARG2(fd, i);
    return a;
}

ANYTHREAD int CONNECT_getUse(int fd)
{
    int a;

    if (0 > fd ||  fd >= ConnectLen)
    {
        return  0;
    }

    CONNECT_LOCK(fd);
    a = Connect[ fd ].use;
    CONNECT_UNLOCK(fd);
    return a;
}
void CONNECT_setUse(int fd, int a)
//ANYTHREAD static void CONNECT_setUse( int fd , int a)
{
    CONNECT_LOCK(fd);
    Connect[ fd ].use = a;
    CONNECT_UNLOCK(fd);
}

ANYTHREAD void CONNECT_checkStatecount(int a)
{
    int i;
    int count = 0;

    for (i = 0; i < ConnectLen; i++)
    {
        if (Connect[ i ].use == FALSE || Connect[ i ].state != a)
        {
            continue;
        }

        if (Connect[ i ].nstatecount <= 0)
        {
            Connect[ i ].nstatecount = (int) time(NULL) + 60;
        }
        else
        {
            if (Connect[ i ].nstatecount < (int) time(NULL))
            {
                CONNECT_endOne_debug(i);
                count++;
            }
        }
    }

    {
        memset(StateTable, 0, sizeof(StateTable));

        for (i = 0; i < ConnectLen; i++)
            if (Connect[ i ].use == TRUE)
            {
                StateTable[ Connect[ i ].state ] ++;
            }
    }
}

ANYTHREAD void CONNECT_setState(int fd, int a)
{
    CONNECT_LOCK(fd);
    Connect[ fd ].state = a;
    Connect[ fd ].nstatecount = 0;
    // Nuke start 0829: For debugging
    {
        char temp[128], buffer[256];
        int i;
        memset(StateTable, 0, sizeof(StateTable));

        for (i = 0; i < ConnectLen; i++)
        {
            if (Connect[ i ].use == TRUE)
            {
                if (Connect[ i ].state > WHILESAVEWAIT)
                {
                    continue;
                }

                StateTable[ Connect[ i ].state ] ++;
            }
        }

        buffer[ 0 ] = 0;

        for (i = 0; i <= WHILESAVEWAIT; i++)
        {
            sprintf(temp, "%-6d", StateTable[ i ]);
            strcat(buffer, temp);
        }

//      print( "\nFILE:%s,LINE:%d", file,fromline );
        //print( "\n{{%s}}", buffer );//原始
#ifdef _STOPLOGININFORMATION
#else
        print("\n{{%s}}", buffer);
#endif
    }
    // Nuke end
    CONNECT_UNLOCK(fd);
}

ANYTHREAD int CONNECT_getState(int fd)
{
    int a;
    CONNECT_LOCK(fd);
    a = Connect[ fd ].state;
    CONNECT_UNLOCK(fd);
    return a;
}

ANYTHREAD void CONNECT_setCharaindex(int fd, int a)
{
    if (fd < 0 ||  fd >= ConnectLen)
    {
        return;
    }

    CONNECT_LOCK(fd);
    Connect[ fd ].charaindex = a;
    CONNECT_UNLOCK(fd);
}
ANYTHREAD int CONNECT_getCharaindex(int fd)
{
    if (fd < 0 ||  fd >= ConnectLen)
    {
        return -1;
    }

    int a;
    CONNECT_LOCK(fd);
    a = Connect[ fd ].charaindex;
    CONNECT_UNLOCK(fd);
    return a;
}
ANYTHREAD void CONNECT_getCdkey(int fd, char *out, int outlen)
{
    if (fd < 0 ||  fd >= ConnectLen)
    {
        return;
    }

    CONNECT_LOCK(fd);
    strcpysafe(out, outlen, Connect[ fd ].cdkey);
    CONNECT_UNLOCK(fd);
}

ANYTHREAD void CONNECT_setCdkey(int fd, char *cd)
{
    if (fd < 0 ||  fd >= ConnectLen)
    {
        return;
    }

    CONNECT_LOCK(fd);
    snprintf(Connect[ fd ].cdkey, sizeof(Connect[ fd ].cdkey), "%s", cd);
    CONNECT_UNLOCK(fd);
}

#ifdef _NEWCLISETSERVID
ANYTHREAD void CONNECT_setServid(int fd, int a)
{
    if (fd < 0 ||  fd >= ConnectLen)
    {
        return;
    }

    CONNECT_LOCK(fd);
    Connect[ fd ].servid = a;
    CONNECT_UNLOCK(fd);
}
ANYTHREAD int CONNECT_getServid(int fd)
{
    if (fd < 0 ||  fd >= ConnectLen)
    {
        return -1;
    }

    int a;
    CONNECT_LOCK(fd);
    a = Connect[ fd ].servid;
    CONNECT_UNLOCK(fd);
    return a;
}
#endif

#ifdef _NEWCLISETMAC
ANYTHREAD void CONNECT_getMAC(int fd, char *out, int outlen)
{
    if (fd < 0 ||  fd >= ConnectLen)
    {
        return;
    }

    CONNECT_LOCK(fd);
    strcpysafe(out, outlen, Connect[ fd ].mac);
    CONNECT_UNLOCK(fd);
}

ANYTHREAD void CONNECT_getMAC2(int fd, char *out, int outlen)
{
    if (fd < 0 ||  fd >= ConnectLen)
    {
        return;
    }

    CONNECT_LOCK(fd);
    strcpysafe(out, outlen, Connect[ fd ].mac2);
    CONNECT_UNLOCK(fd);
}

ANYTHREAD void CONNECT_getMAC3(int fd, char *out, int outlen)
{
    if (fd < 0 ||  fd >= ConnectLen)
    {
        return;
    }

    CONNECT_LOCK(fd);
    strcpysafe(out, outlen, Connect[ fd ].mac3);
    CONNECT_UNLOCK(fd);
}

ANYTHREAD void CONNECT_setMAC(int fd, char *in)
{
    if (fd < 0 ||  fd >= ConnectLen)
    {
        return;
    }

    CONNECT_LOCK(fd);
    strcpysafe(Connect[ fd ].mac, sizeof(Connect[ fd ].mac), in);
    CONNECT_UNLOCK(fd);
}

ANYTHREAD void CONNECT_setMAC2(int fd, char *in)
{
    if (fd < 0 ||  fd >= ConnectLen)
    {
        return;
    }

    CONNECT_LOCK(fd);
    strcpysafe(Connect[ fd ].mac2, sizeof(Connect[ fd ].mac2), in);
    CONNECT_UNLOCK(fd);
}

ANYTHREAD void CONNECT_setMAC3(int fd, char *in)
{
    if (fd < 0 ||  fd >= ConnectLen)
    {
        return;
    }

    CONNECT_LOCK(fd);
    strcpysafe(Connect[ fd ].mac3, sizeof(Connect[ fd ].mac3), in);
    CONNECT_UNLOCK(fd);
}
#endif

ANYTHREAD void CONNECT_getPasswd(int fd, char *out, int outlen)
{
    if (fd < 0 ||  fd >= ConnectLen)
    {
        return;
    }

    CONNECT_LOCK(fd);
    strcpysafe(out, outlen, Connect[ fd ].passwd);
    CONNECT_UNLOCK(fd);
}
ANYTHREAD void CONNECT_setPasswd(int fd, char *in)
{
    if (fd < 0 ||  fd >= ConnectLen)
    {
        return;
    }

    CONNECT_LOCK(fd);
    strcpysafe(Connect[ fd ].passwd, sizeof(Connect[ fd ].passwd), in);
    CONNECT_UNLOCK(fd);
}
ANYTHREAD int CONNECT_getCtype(int fd)
{
    if (fd < 0 ||  fd >= ConnectLen)
    {
        return -1;
    }

    int a;
    CONNECT_LOCK(fd);
    a = Connect[ fd ].ctype;
    CONNECT_UNLOCK(fd);
    return a;
}
ANYTHREAD void CONNECT_setCtype(int fd, int a)
{
    if (fd < 0 ||  fd >= ConnectLen)
    {
        return;
    }

    CONNECT_LOCK(fd);
    Connect[ fd ].ctype = a;
    CONNECT_UNLOCK(fd);
}

ANYTHREAD void CONNECT_getCharname(int fd, char *out, int outlen)
{
    if (fd < 0 ||  fd >= ConnectLen)
    {
        return;
    }

    CONNECT_LOCK(fd);
    strcpysafe(out, outlen, Connect[ fd ].charname);
    CONNECT_UNLOCK(fd);
}
ANYTHREAD void CONNECT_setCharname(int fd, char *in)
{
    if (fd < 0 ||  fd >= ConnectLen)
    {
        return;
    }

    CONNECT_LOCK(fd);
    strcpysafe(Connect[ fd ].charname, sizeof(Connect[ fd ].charname),
               in);
    CONNECT_UNLOCK(fd);
}

ANYTHREAD int CONNECT_getFdid(int fd)
{
    if (fd < 0 ||  fd >= ConnectLen)
    {
        return -1;
    }

    int a;
    CONNECT_LOCK(fd);
    a = Connect[ fd ].fdid;
    CONNECT_UNLOCK(fd);
    return a;
}
ANYTHREAD void CONNECT_setDuelcharaindex(int fd, int i, int a)
{
    if (fd < 0 ||  fd >= ConnectLen)
    {
        return;
    }

    CONNECT_LOCK(fd);
    Connect[ fd ].duelcharaindex[ i ] = a;
    CONNECT_UNLOCK(fd);
}
ANYTHREAD int CONNECT_getDuelcharaindex(int fd, int i)
{
    if (fd < 0 ||  fd >= ConnectLen)
    {
        return -1;
    }

    int a;
    CONNECT_LOCK(fd);
    a = Connect[ fd ].duelcharaindex[ i ];
    CONNECT_UNLOCK(fd);
    return a;
}
ANYTHREAD void CONNECT_setBattlecharaindex(int fd, int i, int a)
{
    if (fd < 0 ||  fd >= ConnectLen)
    {
        return;
    }

    CONNECT_LOCK(fd);
    Connect[ fd ].battlecharaindex[ i ] = a;
    CONNECT_UNLOCK(fd);
}
ANYTHREAD int CONNECT_getBattlecharaindex(int fd, int i)
{
    if (fd < 0 ||  fd >= ConnectLen)
    {
        return -1;
    }

    int a;
    CONNECT_LOCK(fd);
    a = Connect[ fd ].battlecharaindex[ i ];
    CONNECT_UNLOCK(fd);
    return a;
}
ANYTHREAD void CONNECT_setJoinpartycharaindex(int fd, int i, int a)
{
    if (fd < 0 ||  fd >= ConnectLen)
    {
        return;
    }

    CONNECT_LOCK(fd);
    Connect[ fd ].joinpartycharaindex[ i ] = a;
    CONNECT_UNLOCK(fd);
}
ANYTHREAD int CONNECT_getJoinpartycharaindex(int fd, int i)
{
    if (fd < 0 ||  fd >= ConnectLen)
    {
        return -1;
    }

    int a;
    CONNECT_LOCK(fd);
    a = Connect[ fd ].joinpartycharaindex[ i ];
    CONNECT_UNLOCK(fd);
    return a;
}

// CoolFish: Trade 2001/4/18
ANYTHREAD void CONNECT_setTradecharaindex(int fd, int i, int a)
{
    if (fd < 0 ||  fd >= ConnectLen)
    {
        return;
    }

    CONNECT_LOCK(fd);
    Connect[ fd ].tradecharaindex[ i ] = a;
    CONNECT_UNLOCK(fd);
}

// Shan Begin
ANYTHREAD void CONNECT_setLastrecvtime(int fd, struct timeval *a)
{
    if (fd < 0 ||  fd >= ConnectLen)
    {
        return;
    }

    CONNECT_LOCK(fd);
    Connect[fd].lastrecvtime = *a;
    CONNECT_UNLOCK(fd);
}
ANYTHREAD void CONNECT_getLastrecvtime(int fd, struct timeval *a)
{
    if (fd < 0 ||  fd >= ConnectLen)
    {
        return;
    }

    CONNECT_LOCK(fd);
    *a = Connect[fd].lastrecvtime;
    CONNECT_UNLOCK(fd);
}

ANYTHREAD void CONNECT_setLastrecvtime_D(int fd, struct timeval *a)
{
    if (fd < 0 ||  fd >= ConnectLen)
    {
        return;
    }

    CONNECT_LOCK(fd);
    Connect[fd].lastrecvtime_d = *a;
    CONNECT_UNLOCK(fd);
}
ANYTHREAD void CONNECT_getLastrecvtime_D(int fd, struct timeval *a)
{
    CONNECT_LOCK(fd);
    *a = Connect[fd].lastrecvtime_d;
    CONNECT_UNLOCK(fd);
}

// 2001/12/26
ANYTHREAD void CONNECT_SetBattleRecvTime(int fd, struct timeval *a)
{
    if (fd < 0 ||  fd >= ConnectLen)
    {
        return;
    }

    CONNECT_LOCK(fd);
    Connect[fd].battle_recvtime = *a;
    CONNECT_UNLOCK(fd);
}
ANYTHREAD void CONNECT_GetBattleRecvTime(int fd, struct timeval *a)
{
    if (fd < 0 ||  fd >= ConnectLen)
    {
        return;
    }

    CONNECT_LOCK(fd);
    *a = Connect[fd].battle_recvtime;
    CONNECT_UNLOCK(fd);
}
// Shan End

#ifdef _ITEM_PILEFORTRADE
ANYTHREAD void CONNECT_setTradeList(int fd, int num)
{
    if (fd < 0 ||  fd >= ConnectLen)
    {
        return;
    }

    Connect[fd].tradelist = num;
}
ANYTHREAD int CONNECT_getTradeList(int fd)
{
    if (fd < 0 ||  fd >= ConnectLen)
    {
        return -1;
    }

    return Connect[fd].tradelist;
}
#endif

ANYTHREAD void CONNECT_setTradeTmp(int fd, char* a)
{
    if (fd < 0 ||  fd >= ConnectLen)
    {
        return;
    }

    CONNECT_LOCK(fd);
    strcpysafe(Connect[fd].TradeTmp, sizeof(Connect[fd].TradeTmp), a);
    CONNECT_UNLOCK(fd);
}
ANYTHREAD void CONNECT_getTradeTmp(int fd, char *trademsg, int trademsglen)
{
    if (fd < 0 ||  fd >= ConnectLen)
    {
        return;
    }

    CONNECT_LOCK(fd);
    strcpysafe(trademsg, trademsglen, Connect[fd].TradeTmp);
    CONNECT_UNLOCK(fd);
}

ANYTHREAD void CONNECT_setTradecardcharaindex(int fd, int i, int a)
{
    if (fd < 0 ||  fd >= ConnectLen)
    {
        return;
    }

    CONNECT_LOCK(fd);
    Connect[fd].joinpartycharaindex[i] = a;
    CONNECT_UNLOCK(fd);
}
ANYTHREAD int CONNECT_getTradecardcharaindex(int fd, int i)
{
    if (fd < 0 ||  fd >= ConnectLen)
    {
        return -1;
    }

    int a;
    CONNECT_LOCK(fd);
    a = Connect[fd].joinpartycharaindex[i];
    CONNECT_UNLOCK(fd);
    return a;
}

ANYTHREAD void CONNECT_setCloseRequest(int fd, int count)
{
    if (fd < 0 ||  fd >= ConnectLen)
    {
        return;
    }

//      print("\n关闭请求设置为 FILE:%s,LINE:%d ", file, line);
    CONNECT_LOCK(fd);
    Connect[fd].close_request = count;
    // Nuke
//    print("\n关闭请求设置为 %d ",fd);
    CONNECT_UNLOCK(fd);
}


/*------------------------------------------------------------
 * CAcheck などに蝗われる簇眶。悸狠に流る。
 * 苞眶
 *  fd      int     ファイルディスクリプタ
 * 手り猛
 *  なし
 ------------------------------------------------------------*/
ANYTHREAD void CAsend(int fd)
{
    char buf[sizeof(Connect[0].CAbuf)];
    int bufuse = 0;

    if (CONNECT_getCAbuf(fd, buf, sizeof(buf), &bufuse) < 0)
    {
        return;
    }

    if (bufuse == 0)
    {
        return;
    }

    //print("\nshan--->(CAsend)->%s fd->%d", buf, fd);
    /*呵稿のデリミタ ',' を'\0' とかえる*/
    buf[bufuse - 1] = '\0';
#ifdef  _BIG_CACDBUFSIZE

    if (strlen(buf) > 2048)
    {
        char buflong[4][2048];
        memset(buflong[0], 0, sizeof(buflong[0]));
        memset(buflong[1], 0, sizeof(buflong[0]));
        memset(buflong[2], 0, sizeof(buflong[0]));
        memset(buflong[3], 0, sizeof(buflong[0]));
        char buftest[2048];
        int i = 1;

        while (getStringFromIndexWithDelim(buf, ",", i, buftest, sizeof(buftest)))
        {
            if (strlen(buflong[0]) + strlen(buftest) < 2048)
            {
                if (strlen(buflong[0]) == 0)
                {
                    sprintf(buflong[0], "%s,", buftest);
                }
                else
                {
                    sprintf(buflong[0], "%s%s,", buflong[0], buftest);
                }
            }
            else if (strlen(buflong[1]) + strlen(buftest) < 2048)
            {
                if (strlen(buflong[1]) == 0)
                {
                    sprintf(buflong[1], "%s,", buftest);
                }
                else
                {
                    sprintf(buflong[1], "%s%s,", buflong[1], buftest);
                }
            }
            else if (strlen(buflong[2]) + strlen(buftest) < 2048)
            {
                if (strlen(buflong[2]) == 0)
                {
                    sprintf(buflong[2], "%s,", buftest);
                }
                else
                {
                    sprintf(buflong[2], "%s%s,", buflong[2], buftest);
                }
            }
            else if (strlen(buflong[3]) + strlen(buftest) < 2048)
            {
                if (strlen(buflong[3]) == 0)
                {
                    sprintf(buflong[3], "%s,", buftest);
                }
                else
                {
                    sprintf(buflong[3], "%s%s,", buflong[3], buftest);
                }
            }

            i++;
        }

        if (strlen(buflong[0]) > 0)
        {
            buflong[0][strlen(buflong[0])] = '\0';
            lssproto_CA_send(fd, buflong[0]);
        }

        if (strlen(buflong[1]) > 0)
        {
            buflong[1][strlen(buflong[1])] = '\0';
            lssproto_CA_send(fd, buflong[1]);
        }

        if (strlen(buflong[2]) > 0)
        {
            buflong[2][strlen(buflong[2])] = '\0';
            lssproto_CA_send(fd, buflong[2]);
        }

        if (strlen(buflong[3]) > 0)
        {
            buflong[3][strlen(buflong[3])] = '\0';
            lssproto_CA_send(fd, buflong[3]);
        }
    }
    else
#endif
        lssproto_CA_send(fd, buf);

    CONNECT_setCAbufsiz(fd, 0);
}


/*------------------------------------------------------------
 * CAを流る。
 * 苞眶
 * 手り猛
 *  なし
 ------------------------------------------------------------*/
ANYTHREAD void CAcheck(void)
{
    int     i;
#ifndef _WAN_FIX
#ifdef _SYSTEM_SPEAD
    unsigned int interval_us = getCAsendinterval_ms() * 200;
#else
    unsigned int interval_us = getCAsendinterval_ms() * 1000;
#endif
#endif

    /* Connect及蜊醒坌分仃支月井日褐中氏分卅［ */
    for (i = 0; i < ConnectLen; i ++)
    {
#ifndef _WAN_FIX
        struct timeval t;
#endif

        if (!CONNECT_getUse_debug(i, 1008))
        {
            continue;
        }

#ifndef _WAN_FIX
        CONNECT_getLastCAsendtime(i, &t);

        if (time_diff_us(NowTime, t) > interval_us)
        {
#endif
            CAsend(i);
#ifndef _WAN_FIX
            CONNECT_setLastCAsendtime(i, &NowTime);
        }

#endif
    }
}
ANYTHREAD void CAflush(int charaindex)
{
    int i;
    i = getfdFromCharaIndex(charaindex);

    if (i == -1)
    {
        return;
    }

    CAsend(i);
}


/*------------------------------------------------------------
 * CDbuf に纳裁する。
 * 苞眶
 *  fd      int     ファイルディスクリプタ
 *  data    char*   デ〖タ
 *  size    int     デ〖タのサイズ
 * 手り猛
 *  喇根    TRUE(1)
 *  己窃    FALSE(0)
 ------------------------------------------------------------*/
ANYTHREAD BOOL CONNECT_appendCDbuf(int fd, char* data, int size)
{
    CONNECT_LOCK(fd);

    if ((Connect[fd].CDbufsiz + size) >= sizeof(Connect[fd].CDbuf))
    {
        CONNECT_UNLOCK(fd);
        return FALSE;
    }

    memcpy(Connect[fd].CDbuf + Connect[fd].CDbufsiz, data, size);
    Connect[fd].CDbuf[Connect[fd].CDbufsiz + size] = ',';
    Connect[fd].CDbufsiz += (size + 1);
    CONNECT_UNLOCK(fd);
    return TRUE;
}


/*------------------------------------------------------------
 * CDcheck などに蝗われる簇眶。悸狠に流る。
 * 苞眶
 *  fd      int     ファイルディスクリプタ
 * 手り猛
 *  なし
 ------------------------------------------------------------*/
ANYTHREAD void CDsend(int fd)
{
    char buf[sizeof(Connect[0].CAbuf)];
    int bufuse = 0;

    if (CONNECT_getCDbuf(fd, buf, sizeof(buf), &bufuse) < 0)
    {
        return;
    }

    if (bufuse == 0)
    {
        return;
    }

    buf[bufuse - 1] = '\0';
    lssproto_CD_send(fd, buf);
    CONNECT_setCDbufsiz(fd, 0);
}


/*------------------------------------------------------------
 * CDを流る。
 * 苞眶
 * 手り猛
 *  なし
 ------------------------------------------------------------*/
ANYTHREAD void CDcheck(void)
{
    int     i;
#ifndef _WAN_FIX
    unsigned int interval_us = getCDsendinterval_ms() * 1000;
#endif

    for (i = 0; i < ConnectLen; i++)
    {
#ifndef _WAN_FIX
        struct timeval t;
#endif

        if (!CONNECT_getUse_debug(i, 1082))
        {
            continue;
        }

#ifndef _WAN_FIX
        CONNECT_getLastCDsendtime(i, &t);

        if (time_diff_us(NowTime, t) > interval_us)
        {
#endif
            CDsend(i);
#ifndef _WAN_FIX
            CONNECT_setLastCDsendtime(i, &NowTime);
        }

#endif
    }
}

ANYTHREAD void CDflush(int charaindex)
{
    int i;
    i = getfdFromCharaIndex(charaindex);

    if (i == -1)
    {
        return;
    }

    CDsend(i);
}

void chardatasavecheck(void)
{
    int i;
    int interval = getCharSavesendinterval();
    static struct timeval chardatasavecheck_store;
    DebugMainFunction = "chardatasavecheck3";

    if (NowTime.tv_sec > (chardatasavecheck_store.tv_sec + 10))
    {
        DebugMainFunction = "chardatasavecheck4";
        chardatasavecheck_store = NowTime;
        DebugMainFunction = "chardatasavecheck5";
        int maxnum = CHAR_getPlayerMaxNum();

        for (i = 0; i < maxnum; i++)
        {
            if (CHAR_CHECKINDEX(i) && NowTime.tv_sec - CHAR_getWorkInt(i, CHAR_WORK_SAVE_CHAR) > interval)
            {
                CHAR_setWorkInt(i, CHAR_WORK_SAVE_CHAR, (int)NowTime.tv_sec);
                CHAR_charSaveFromConnect(i, FALSE);
            }
        }

//      for( i = 0; i < ConnectLen; i ++) {
//          CONNECT_LOCK(i);
//          if( Connect[i].use == TRUE
//              && Connect[i].state == LOGIN
//              && NowTime.tv_sec - Connect[i].lastCharSaveTime.tv_sec
//      > interval ){
//          Connect[i].lastCharSaveTime = NowTime;
//          CONNECT_UNLOCK(i);
//          CHAR_charSaveFromConnect( Connect[i].charaindex, FALSE );
//          } else {
//              CONNECT_UNLOCK(i);
//          }
//      }
    }
    else
    {
        ;
    }
}

/*void chardatasavecheck( void )
{
    int i;
    DebugMainFunction="chardatasavecheck1";
    int interval = getCharSavesendinterval();
    DebugMainFunction="chardatasavecheck3";
    time_t now;
        struct tm *timenow;
        time(&now);
        timenow = localtime(&now);
    //if( NowTime.tv_sec > (chardatasavecheck_store.tv_sec +10)){
    if(timenow > savetime + 10){
            DebugMainFunction="chardatasavecheck4";
        //chardatasavecheck_store = NowTime;
        savetime = timenow;
        DebugMainFunction="chardatasavecheck5";

#ifdef _FIX_SAVE_CHAR
        for(i=0;i<CHAR_getPlayerMaxNum();i++){
                    if(CHAR_CHECKINDEX(i) == FALSE) continue;
                    int tv_sec = NowTime.tv_sec;
                    if(CHAR_getWorkInt( i, CHAR_WORK_SAVE_CHAR) == 0){
                        CHAR_setWorkInt( i, CHAR_WORK_SAVE_CHAR, tv_sec);
                    }else{
                        if(tv_sec - CHAR_getWorkInt( i, CHAR_WORK_SAVE_CHAR) > interval){
                            DebugMainFunction="chardatasavecheck6";
                            CHAR_charSaveFromConnect( i, FALSE );
                            DebugMainFunction="chardatasavecheck7";
                            CHAR_setWorkInt( i, CHAR_WORK_SAVE_CHAR, tv_sec);
                            DebugMainFunction="chardatasavecheck8";
                        }
                    }
                }
#else
        for( i = 0; i < ConnectLen; i ++) {
            CONNECT_LOCK(i);
            if( Connect[i].use == TRUE
                && Connect[i].state == LOGIN
                && NowTime.tv_sec - Connect[i].lastCharSaveTime.tv_sec
                > interval ){
                Connect[i].lastCharSaveTime = NowTime;
                CONNECT_UNLOCK(i);
                CHAR_charSaveFromConnect( CONNECT_getCharaindex( i ), FALSE );
            } else {
                CONNECT_UNLOCK(i);
            }
        }
#endif
    }
}*/

/*------------------------------------------------------------
 * fd 互 valid 卅手及井升丹井毛譬屯月
 * 娄醒
 *  fd          int         fd
 * 忒曰袄
 *  valid   TRUE(1)
 *  invalid FALSE(0)
 ------------------------------------------------------------*/
ANYTHREAD INLINE int CONNECT_checkfd(int fd)
{
    if (0 > fd ||  fd >= ConnectLen)
    {
        return FALSE;
    }

    CONNECT_LOCK(fd);

    if (Connect[fd].use == FALSE)
    {
        CONNECT_UNLOCK(fd);
        return FALSE;
    }
    else
    {
        CONNECT_UNLOCK(fd);
        return TRUE;
    }
}


/*------------------------------------------------------------
 * cdkey から fd を评る。
 * 苞眶
 *  cd      char*       cdkey
 * 手り猛
 *  ファイルディスクリプタ  よって、 -1 の箕はエラ〖
 ------------------------------------------------------------*/
ANYTHREAD int getfdFromCdkey(char* cd)
{
    int i;

    for (i = 0 ; i < ConnectLen ; i ++)
    {
        CONNECT_LOCK(i);

        if (Connect[i].use == TRUE &&
                strcmp(Connect[i].cdkey, cd) == 0)
        {
            CONNECT_UNLOCK(i);
            return i;
        }

        CONNECT_UNLOCK(i);
    }

    return -1;
}


/*------------------------------------------------------------
 * charaindex 井日 fd 毛  月［
 *   陆质  及民尼永弁毛聂仁仄凶［
 * 娄醒
 *  charaindex      int     平乓仿及奶件犯永弁旦
 * 忒曰袄
 *  白央奶伙犯奴旦弁伉皿正  方匀化｝ -1 及凛反巨仿□
 ------------------------------------------------------------*/
ANYTHREAD int getfdFromCharaIndex(int charaindex)
{
#if 1
    int ret;

    if (!CHAR_CHECKINDEX(charaindex))
    {
        return -1;
    }

    if (CHAR_getInt(charaindex, CHAR_WHICHTYPE) != CHAR_TYPEPLAYER)
    {
        return -1;
    }

    ret = CHAR_getWorkInt(charaindex, CHAR_WORKFD);

    if (ret < 0 || ret >= ConnectLen)
    {
        return -1;
    }

    return ret;
#else
    int i;

    for (i = 0 ; i < ConnectLen ; i ++)
    {
        CONNECT_LOCK(i);

        if (Connect[i].use == TRUE
                && Connect[i].charaindex == charaindex)
        {
            CONNECT_UNLOCK(i);
            return i;
        }

        CONNECT_UNLOCK(i);
    }

    return -1;
#endif
}
/*------------------------------------------------------------
 * charaindex 井日 cdkey 毛  月［
 * 娄醒
 *  charaindex  int     平乓仿及奶件犯永弁旦
 * 忒曰袄
 *  0卅日岳  ｝  卅日撩
 ------------------------------------------------------------*/
ANYTHREAD int getcdkeyFromCharaIndex(int charaindex, char *out, int outlen)
{
    int i;

    for (i = 0 ; i < ConnectLen ; i ++)
    {
        CONNECT_LOCK(i);

        if (Connect[i].use == TRUE
                && Connect[i].charaindex == charaindex)
        {
            snprintf(out, outlen, "%s", Connect[i].cdkey);
            CONNECT_UNLOCK(i);
            return 0;
        }

        CONNECT_UNLOCK(i);
    }

    return -1;
}


/*------------------------------------------------------------
 * 票じfdid の袍を玫す
 * 苞眶
 *  fdid    int     fdのid
 * 手り猛
 *  -1 及凛反巨仿□
 ------------------------------------------------------------*/
ANYTHREAD int getfdFromFdid(int fdid)
{
    int i;

    for (i = 0; i < ConnectLen ; i ++)
    {
        CONNECT_LOCK(i);

        if (Connect[i].use == TRUE
                && Connect[i].fdid == fdid)
        {
            CONNECT_UNLOCK(i);
            return i;
        }

        CONNECT_UNLOCK(i);
    }

    return -1;
}

/*------------------------------------------------------------
 * fdid からキャラのindex をもとめる。
 * 苞眶
 *  fdid    int     fdのid
 * 手り猛
 *  -1 の箕はログイン面のキャラはみつからなかった。0笆惧なら
 * ログイン面のキャラのキャラはいれつへの index
 ------------------------------------------------------------*/
ANYTHREAD int getCharindexFromFdid(int fdid)
{
    int i;

    for (i = 0; i < ConnectLen ; i ++)
    {
        CONNECT_LOCK(i);

        if (Connect[i].use == TRUE
                && Connect[i].fdid == fdid &&
                Connect[i].charaindex >= 0)
        {
            int a = Connect[i].charaindex;
            CONNECT_UNLOCK(i);
            return a;
        }

        CONNECT_UNLOCK(i);
    }

    return -1;
}
/*------------------------------------------------------------
 * キャラindex から fdid をもとめる。
 * 苞眶
 *  charind  int     ファイルディスクリプタ
 * かえりち fdid  砷だったらキャラindがおかしい
 ------------------------------------------------------------*/
ANYTHREAD int getFdidFromCharaIndex(int charind)
{
    int i;

    for (i = 0; i < ConnectLen ; i ++)
    {
        CONNECT_LOCK(i);

        if (Connect[i].use == TRUE
                && Connect[i].charaindex == charind)
        {
            int a = Connect[i].fdid;
            CONNECT_UNLOCK(i);
            return a;
        }

        CONNECT_UNLOCK(i);
    }

    return -1;
}


/*------------------------------------------------------------
 * fdに充り碰てられた儡鲁がクライアントだと、BOOLを手す
 * 嘿いエラ〖チェックはしない。
 * 苞眶
 *  fd  int     ファイルディスクリプタ
 ------------------------------------------------------------*/
ANYTHREAD BOOL CONNECT_isCLI(int fd)
{
    int a;
    CONNECT_LOCK(fd);
    a = (Connect[fd].ctype == CLI ? TRUE : FALSE);
    CONNECT_UNLOCK(fd);
    return a;
}


/*------------------------------------------------------------
 * fdに充り碰てられた儡鲁がアカウントサ〖バだと、BOOLを手す
 * 嘿いエラ〖チェックはしない。
 * 苞眶
 *  fd  int     ファイルディスクリプタ
 ------------------------------------------------------------*/
ANYTHREAD BOOL CONNECT_isAC(int fd)
{
    int a;
    CONNECT_LOCK(fd);
    a = (Connect[fd].ctype == AC ? TRUE : FALSE);
    CONNECT_UNLOCK(fd);
    return a;
}

/*------------------------------------------------------------
 * fdに充り碰てられた儡鲁がログイン觉轮であるかどうか
 * を手す
 * 苞眶
 *  fd  int     ファイルディスクリプタ
 ------------------------------------------------------------*/
ANYTHREAD BOOL CONNECT_isUnderLogin(int fd)
{
    int a;
    CONNECT_LOCK(fd);
    a = (Connect[fd].state == LOGIN ? TRUE : FALSE);
    CONNECT_UNLOCK(fd);
    return a;
}

/*------------------------------------------------------------
 * Login借妄面かどうか拇べる
 * 苞眶
 *  fd  int     ファイルディスクリプタ
 ------------------------------------------------------------*/
ANYTHREAD BOOL CONNECT_isWhileLogin(int fd)
{
    int a;
    CONNECT_LOCK(fd);
    a = (Connect[fd].state == WHILELOGIN ? TRUE : FALSE);
    CONNECT_UNLOCK(fd);
    return a;
}

/*------------------------------------------------------------
 * ログインしていない觉轮か
 * どうかを手す
 * 苞眶
 *  fd  int     ファイルディスクリプタ
 ------------------------------------------------------------*/
ANYTHREAD BOOL CONNECT_isNOTLOGIN(int fd)
{
    int a;
    CONNECT_LOCK(fd);
    a = (Connect[fd].state == NOTLOGIN ? TRUE : FALSE);
    CONNECT_UNLOCK(fd);
    return a;
}

/*------------------------------------------------------------
 * ログインしている觉轮か
 * どうかを手す
 * 苞眶
 *  fd  int     ファイルディスクリプタ
 ------------------------------------------------------------*/
ANYTHREAD BOOL CONNECT_isLOGIN(int fd)
{
    int a;
    CONNECT_LOCK(fd);
    a = (Connect[fd].state == LOGIN ? TRUE : FALSE);
    CONNECT_UNLOCK(fd);
    return a;
}



/*------------------------------------------------------------
 * 链镑の儡鲁を磊って、アカウントサ〖バにデ〖タを瘦赂しようとする。
 * 苞眶、手り猛
 *  なし
 ------------------------------------------------------------*/
void closeAllConnectionandSaveData(void)
{
    int     i;
    int     num;
    /*  これ笆惧 accept しないようにする    */
    SERVSTATE_setCloseallsocketnum(0);

    /*  链婶猴近する    */
    for (i = 0 ; i < ConnectLen ; i++)
    {
        if (CONNECT_getUse_debug(i, 1413) == TRUE)
        {
            BOOL    clilogin = FALSE;

            if (CONNECT_isAC(i))
            {
                continue;
            }

            if (CONNECT_isCLI(i) && CONNECT_isLOGIN(i))
            {
                clilogin = TRUE;
            }

            CONNECT_endOne_debug(i);
            // Nuke +1 0901: Why close
            //print("closed in closeAllConnectionandSaveData");

            if (clilogin)
            {
                CONNECT_setUse(i, TRUE);
                CONNECT_setState(i, WHILECLOSEALLSOCKETSSAVE);
                SERVSTATE_incrementCloseallsocketnum();
            }
        }
    }

    num = SERVSTATE_getCloseallsocketnum();

    if (num == 0)
    {
        SERVSTATE_SetAcceptMore(-1);
    }
    else
    {
        SERVSTATE_SetAcceptMore(0);
    }

    print("\n发送人物数据数目:%d\n", num);
    sigshutdown(0);
}

//andy_add 2003/02/12
void CONNECT_SysEvent_Loop(void)
{
    static time_t checkT = 0;
    static int chikulatime = 0;
    int NowTimes = time(NULL);

    if (checkT != NowTimes && (checkT + 10) <= NowTimes)
    {
        int i;
        checkT = time(NULL);
        chikulatime++;//每10秒

        if (chikulatime > 10000)
        {
            chikulatime = 0;
        }

#ifdef _LUCK_STAR

        if (getLuckStarTime() > 0)
        {
            if (chikulatime % getLuckStarTime() == 0)     // 每小时
            {
                LuckStar();
            }
        }

#endif
        int playernum = CHAR_getPlayerMaxNum();
        int charaindex;

        for (charaindex = 0; charaindex < playernum; charaindex++)
        {
            if (!CHAR_CHECKINDEX(charaindex))
            {
                continue;
            }

#ifdef _CANCEL_STREET_VENDOR
            char token[256];

            if (CHAR_getWorkInt(charaindex, CHAR_WORKSTREETVENDOR) == 3) //查看摆摊出售的状态
            {
                if (CHAR_getWorkInt(charaindex, CHAR_WORK_STREET_VENDOR_TIME) < time(NULL) - 90) //90秒
                {
                    int toindex = CHAR_getWorkInt(charaindex, CHAR_WORKSTREETVENDOR_WHO);
                    // 卖方设定为摆摊,清除买方状态
                    CHAR_setWorkInt(charaindex, CHAR_WORKSTREETVENDOR, -1);
                    CHAR_setWorkInt(toindex, CHAR_WORKSTREETVENDOR, 1);
                    // 清除交易图示
                    CHAR_sendTradeEffect(charaindex, 0);
                    CHAR_sendTradeEffect(toindex, 0);
                    CHAR_setWorkInt(toindex, CHAR_WORKSTREETVENDOR_WHO, -1);
                    CHAR_setWorkInt(charaindex, CHAR_WORKSTREETVENDOR_WHO, -1);
                    lssproto_STREET_VENDOR_send(charaindex, "C|");
                    CHAR_talkToCli(charaindex, -1, "查看摆摊时间过长,卖家自动取消你的查看", CHAR_COLORYELLOW);
                }
            }

#endif

            if (chikulatime % 6 == 0)     // 每60秒
            {
#ifdef _PETSKILL_BECOMEPIG

                if (CHAR_getInt(charaindex, CHAR_BECOMEPIG) > -1)       //处於乌力化状态
                {
                    if ((CHAR_getInt(charaindex, CHAR_BECOMEPIG) - 1) <= 0)         //乌力时间结束了
                    {
                        CHAR_setInt(charaindex, CHAR_BECOMEPIG, 0);

                        if (CHAR_getWorkInt(charaindex, CHAR_WORKBATTLEMODE) == BATTLE_CHARMODE_NONE)       //不是在战斗状态下
                        {
                            CHAR_setInt(charaindex, CHAR_BECOMEPIG, -1);   //结束乌力状态
                            CHAR_complianceParameter(charaindex);
                            CHAR_sendCToArroundCharacter(CHAR_getWorkInt(charaindex, CHAR_WORKOBJINDEX));
                            CHAR_send_P_StatusString(charaindex, CHAR_P_STRING_BASEBASEIMAGENUMBER);
                            CHAR_talkToCli(charaindex, -1, "乌力化失效了。", CHAR_COLORWHITE);
                        }
                    }
                    else
                    {
                        CHAR_setInt(charaindex, CHAR_BECOMEPIG, CHAR_getInt(charaindex, CHAR_BECOMEPIG) - 10);
                    }
                }

#endif
#ifdef _SPECIAL_MAP
                int floor = CHAR_getInt(charaindex, CHAR_FLOOR);
                int x;

                for (x = 0; x < 32; x++)
                {
                    if (floor == getSpecialMap(x))
                    {
                        break;
                    }
                }

                if (x < 32)
#endif
                {
#ifdef _BOUND_TIME

                    if (CHAR_getInt(charaindex, CHAR_BOUNDTIME) <= NowTimes)
                    {
                        int fl = 0, x = 0, y = 0;
                        CHAR_getElderPosition(CHAR_getInt(charaindex, CHAR_LASTTALKELDER), &fl, &x, &y);
                        CHAR_warpToSpecificPoint(charaindex, fl, x, y);
                        CHAR_talkToCli(charaindex, -1, "由于你的特权时间已到，所以把您送回记录点！", CHAR_COLORYELLOW);
                    }

#endif
                }
            }

            if (chikulatime % 30 == 0)     //每300秒
            {
#ifdef _ITEM_ADDEXP //vincent 经验提升
#ifdef _PET_ADD_EXP

                if (CHAR_getInt(charaindex, CHAR_WHICHTYPE) == CHAR_TYPEPLAYER)
                {
                    int exptime;

                    if (CHAR_getWorkInt(charaindex, CHAR_WORKITEM_ADDEXP) > 0)
                    {
                        exptime = CHAR_getWorkInt(charaindex, CHAR_WORKITEM_ADDEXPTIME) - 300;

                        if (exptime <= 0)
                        {
                            CHAR_setWorkInt(charaindex, CHAR_WORKITEM_ADDEXP, 0);
                            CHAR_setWorkInt(charaindex, CHAR_WORKITEM_ADDEXPTIME, 0);
                            CHAR_talkToCli(charaindex, -1, "提升学习经验的能力消失了!", CHAR_COLORYELLOW);
                        }
                        else
                        {
                            CHAR_setWorkInt(charaindex, CHAR_WORKITEM_ADDEXPTIME, exptime);
                            //print("\n 检查ADDEXPTIME %d ", exptime);

                            if ((exptime % (60 * 60)) < 300 && exptime >= (60 * 60))
                            {
                                char msg[ 1024 ];
                                sprintf(msg, "您的提升学习经验的能力剩大约 %d 小时。", (int)(exptime / (60 * 60)));
                                CHAR_talkToCli(charaindex, -1, msg, CHAR_COLORYELLOW);
                            }
                        }
                    }

                    int i;

                    for (i = 0; i < CHAR_MAXPETHAVE; i ++)
                    {
                        int petindex = CHAR_getCharPet(charaindex, i);

                        if (!CHAR_CHECKINDEX(petindex))
                        {
                            continue;
                        }

                        if (CHAR_getWorkInt(petindex, CHAR_WORKITEM_ADDEXP) > 0)
                        {
                            exptime = CHAR_getWorkInt(petindex, CHAR_WORKITEM_ADDEXPTIME) - 300;

                            if (exptime <= 0)
                            {
                                CHAR_setWorkInt(petindex, CHAR_WORKITEM_ADDEXP, 0);
                                CHAR_setWorkInt(petindex, CHAR_WORKITEM_ADDEXPTIME, 0);
                                CHAR_talkToCli(charaindex, -1, "提升学习经验的能力消失了!", CHAR_COLORYELLOW);
                            }
                            else
                            {
                                CHAR_setWorkInt(petindex, CHAR_WORKITEM_ADDEXPTIME, exptime);
                                //print("\n 检查ADDEXPTIME %d ", exptime);

                                if ((exptime % (60 * 60)) < 300 && exptime >= (60 * 60))
                                {
                                    char msg[ 1024 ];
                                    sprintf(msg, "宠物%s提升学习经验的能力剩大约 %d 小时。", CHAR_getChar(petindex, CHAR_NAME), (int)(exptime / (60 * 60)));
                                    CHAR_talkToCli(charaindex, -1, msg, CHAR_COLORYELLOW);
                                }
                            }
                        }
                    }
                }

#else

                if (CHAR_getWorkInt(charaindex, CHAR_WORKITEM_ADDEXP) > 0 &&
                        CHAR_getInt(charaindex, CHAR_WHICHTYPE) == CHAR_TYPEPLAYER)
                {
                    int exptime;
                    exptime = CHAR_getWorkInt(charaindex, CHAR_WORKITEM_ADDEXPTIME);
#ifndef _NEW_STONEAGE_TYPE

                    if ((time(NULL) - CHAR_getWorkInt(charaindex, CHAR_WORKBATTLEFINISHTIME)) / 60 < 10)
#endif // _NEW_STONEAGE_TYPE
                        exptime = exptime - 300;

                    int dayexptime = CHAR_getInt(charaindex, CHAR_DAYITEMEXPTIME);
                    dayexptime = dayexptime - 300;

                    if (dayexptime < 0)
                    {
                        dayexptime = 0;
                    }

                    CHAR_setInt(charaindex, CHAR_DAYITEMEXPTIME, dayexptime);

                    if (exptime <= 0)
                    {
                        CHAR_setWorkInt(charaindex, CHAR_WORKITEM_ADDEXP, 0);
                        CHAR_setWorkInt(charaindex, CHAR_WORKITEM_ADDEXPTIME, 0);
                        CHAR_talkToCli(charaindex, -1, "提升学习经验的能力消失了!", CHAR_COLORYELLOW);
                    }
                    else
                    {
                        CHAR_setWorkInt(charaindex, CHAR_WORKITEM_ADDEXPTIME, exptime);
                        //print("\n 检查ADDEXPTIME %d ", exptime);

                        if ((exptime % (60 * 60)) < 300 && exptime >= (60 * 60))
                        {
                            char msg[ 1024 ];
                            sprintf(msg, "提升学习经验的能力剩大约 %d 小时。", (int)(exptime / (60 * 60)));
                            //sprintf( msg, "提升学习经验的能力剩大约 %d 分。", (int)(exptime/(60)) );
                            CHAR_talkToCli(charaindex, -1, msg, CHAR_COLORYELLOW);
                        }
                    }

                    CHAR_setInt(charaindex, CHAR_ADDEXPPOWER,
                                CHAR_getWorkInt(charaindex, CHAR_WORKITEM_ADDEXP));
                    CHAR_setInt(charaindex, CHAR_ADDEXPTIME,
                                CHAR_getWorkInt(charaindex, CHAR_WORKITEM_ADDEXPTIME));
                }

#endif
#endif
#ifdef _ITEM_METAMO

                if (CHAR_getWorkInt(charaindex, CHAR_WORKITEMMETAMO) < NowTime.tv_sec
                        && CHAR_getWorkInt(charaindex, CHAR_WORKITEMMETAMO) != 0)
                {
                    CHAR_setWorkInt(charaindex, CHAR_WORKITEMMETAMO, 0);
                    CHAR_setWorkInt(charaindex, CHAR_WORKNPCMETAMO, 0);   //与npc对话後的变身也要变回来
                    CHAR_complianceParameter(charaindex);
                    CHAR_sendCToArroundCharacter(CHAR_getWorkInt(charaindex, CHAR_WORKOBJINDEX));
                    CHAR_send_P_StatusString(charaindex, CHAR_P_STRING_BASEBASEIMAGENUMBER);
                    CHAR_talkToCli(charaindex, -1, "变身失效了。", CHAR_COLORWHITE);
                }

#endif
#ifdef _ITEM_TIME_LIMIT
                ITEM_TimeLimit(charaindex);   // (可开放) shan time limit of item. code:shan
#endif
            } //%30

#ifdef _PETSKILL_BECOMEPIG

            if (CHAR_getWorkInt(charaindex, CHAR_WORKBATTLEMODE) == BATTLE_CHARMODE_NONE)       //不是在战斗状态下
            {
                if (CHAR_getInt(charaindex, CHAR_BECOMEPIG) > -1)       //处於乌力化状态
                {
                    char temp[ 256 ];
                    CHAR_setInt(charaindex, CHAR_BECOMEPIG, CHAR_getInt(charaindex, CHAR_BECOMEPIG) - 1);
                    sprintf(temp, "乌力时间:%d秒", CHAR_getInt(charaindex, CHAR_BECOMEPIG));
                    CHAR_talkToCli(charaindex, -1, temp, CHAR_COLORWHITE);
                }
            }

#endif
            //10秒
#ifdef _MAP_TIME

            if (CHAR_getWorkInt(charaindex, CHAR_WORK_MAP_TIME) > 0
                    && CHAR_getWorkInt(charaindex, CHAR_WORKBATTLEMODE) == BATTLE_CHARMODE_NONE)
            {
                CHAR_setWorkInt(charaindex, CHAR_WORK_MAP_TIME, CHAR_getWorkInt(charaindex, CHAR_WORK_MAP_TIME) - 10);

                if (CHAR_getWorkInt(charaindex, CHAR_WORK_MAP_TIME) <= 0)
                {
                    // 时间到了,传回入口
                    CHAR_talkToCli(charaindex, -1, "你因为受不了高热而热死！传回裂缝入口。", CHAR_COLORRED);
                    CHAR_warpToSpecificPoint(charaindex, 30008, 39, 38);
                    CHAR_setInt(charaindex, CHAR_HP, 1);
                    CHAR_AddCharm(charaindex, -3);
                    CHAR_send_P_StatusString(charaindex, CHAR_P_STRING_HP);
                    CHAR_send_P_StatusString(charaindex, CHAR_P_STRING_CHARM);
                }
                else
                {
                    char szMsg[ 64 ];
                    sprintf(szMsg, "在这高热的环境下你只能再待 %d 秒。", CHAR_getWorkInt(charaindex, CHAR_WORK_MAP_TIME));
                    CHAR_talkToCli(charaindex, -1, szMsg, CHAR_COLORRED);
                }
            }

#endif
        }

        for (i = 0; i < ConnectLen; i++)
        {
            if ((Connect[ i ].use) && (i != acfd))
                if (!CONNECT_getUse(i))
                {
                    continue;
                }

            if (!CHAR_CHECKINDEX(Connect[ i ].charaindex))
            {
                continue;
            }

            {
#ifdef _SERVER_NP_

                if (chikulatime % 18 == 0 && getServerNp() == 1)
                {
                    if (Connect[i].loginmark == 2)
                    {
                        UINT32 uPacketSize = 0;
                        UINT32 uReturn = CSAuth3_Get(Connect[i].pbUser, &uPacketSize);

                        if (uReturn >= 3000)
                        {
                            // 断开玩家连接
                            //print("\n获取NP状态失败");
                            lssproto_OfflineReturn_send(i);
                            CHAR_logout(Connect[i].charaindex, TRUE);
                            CONNECT_setCloseRequest(i, 1);
                            continue;
                            //CONNECT_endOne_debug(i);
                        }
                        else
                        {
                            PBYTE packet = CSAuth3_GetPacket(Connect[i].pbUser);
                            char packethex[4096];
                            memset(packethex, 0, sizeof(packethex));
                            stringtohexstr(packethex, packet, uPacketSize);
                            lssproto_NP_send(i, packethex);
                        }
                    }
                }

                if (chikulatime % 60 == 0)
                {
                    if (Connect[i].loginmark == 2)
                    {
                        if (time(NULL) - Connect[i].nptime > 600)
                        {
                            //print("\n获取NP包时间超时");
                            if (getServerNp() == 1)
                            {
                                lssproto_OfflineReturn_send(i);
                                CHAR_logout(Connect[i].charaindex, TRUE);
                                CONNECT_setCloseRequest(i, 1);
                            }
                            else
                            {
                                Connect[i].nptime = time(NULL);
                            }

                            //CONNECT_endOne_debug(i);
                            continue;
                        }
                    }
                }

#endif
#ifdef _NEW_AUTO_PK
                {
                    if (getAutoPkTime() > -1)
                    {
                        if (getBAward() == FALSE)
                        {
                            struct tm *p;
                            time_t timep;
                            time(&timep);
                            p = localtime(&timep); /*取得当地时间*/

                            if (getBAward() == FALSE)
                            {
                                if ((getAutoPkTime() == p->tm_hour) && (p->tm_min == 0))
                                {
                                    AutoPk_PKTimeSet(15);
                                    AutoPk_GetAwardStr();
                                }
                            }
                        }
                    }
                }
#endif
#ifndef _USER_CHARLOOPS

                //here 原地遇敌
                if (Connect[ i ].stayencount)
                {
                    if (Connect[ i ].BDTime < time(NULL))
                    {
                        if (CHAR_getWorkInt(Connect[ i ].charaindex, CHAR_WORKBATTLEMODE) == BATTLE_CHARMODE_NONE)
                        {
                            lssproto_EN_recv(i, CHAR_getInt(Connect[ i ].charaindex, CHAR_X),
                                             CHAR_getInt(Connect[ i ].charaindex, CHAR_Y));
                            Connect[ i ].BDTime = time(NULL);
                        }
                    }
                }

#endif
#ifdef _CHIKULA_STONE

                if (chikulatime % 3 == 0 && getChiStone(i) > 0)       //自动补血
                {
                    CHAR_AutoChikulaStone(Connect[ i ].charaindex, getChiStone(i));
                }

#endif

                if (chikulatime % 6 == 0)     //水世界状态
                {
#ifdef _STATUS_WATERWORD
                    CHAR_CheckWaterStatus(Connect[ i ].charaindex);
#endif
                    // Nuke 0626: No enemy

                    if (Connect[ i ].noenemy > 0)
                    {
                        Connect[ i ].noenemy--;

                        if (Connect[ i ].noenemy == 0)
                        {
                            CHAR_talkToCli(CONNECT_getCharaindex(i), -1, "守护消失了。", CHAR_COLORWHITE);
                        }
                    }
                }

                //每10秒
#ifdef _TYPE_TOXICATION //中毒

                if (Connect[ i ].toxication > 0)
                {
                    CHAR_ComToxicationHp(Connect[ i ].charaindex);
                }

#endif

                // Nuke 0624 Avoid Useless Connection
                if (Connect[ i ].state == NOTLOGIN)
                {
                    Connect[ i ].cotime++;

                    if (Connect[ i ].cotime > 30)
                    {
                        print("LATE");
                        CONNECT_endOne_debug(i);
                    }
                }
                else
                {
                    Connect[ i ].cotime = 0;
                }

                if ((Connect[ i ].nu <= 22))
                {
                    if (Connect[ i ].nu <= 0)
                    {
                        Connect[ i ].nu_decrease++;

                        if (Connect[ i ].nu_decrease >= 30)
                        {
                            Connect[ i ].nu_decrease = 30;
                        }

                        if (Connect[ i ].nu_decrease > 22)
                        {
                            logSpeed(i);
                        }
                    }
                    else
                    {
                        Connect[ i ].nu_decrease -= 1;

                        if (Connect[ i ].nu_decrease < 0)
                        {
                            Connect[ i ].nu_decrease = 0;
                        }
                    }

                    lssproto_NU_send(i, 0);
                    /*
                              r = 22 - Connect[ i ].nu_decrease;
                              r = ( r >= 15 ) ? r : 15;
                              lssproto_NU_send( i, r );
                              Connect[ i ].nu += r;
                    */
                }

                Connect[ i ].packetin--; // Remove a counter

                if (Connect[ i ].packetin <= 0)     // Time out, drop this line
                {
                    //print( "Drop line: sd=%d\n", i );
                    CONNECT_endOne_debug(i);
                }
            }
        }
    }
}

// Nuke 0126: Resource protection
int isThereThisIP(unsigned long ip)
{
    int i;
    unsigned long ipa;

    for (i = 0; i < ConnectLen ; i++)
        if (!Connect[ i ].use)
        {
            continue;
        }

    if (Connect[ i ].state == NOTLOGIN || Connect[ i ].state == WHILEDOWNLOADCHARLIST)
    {
        memcpy(&ipa, &Connect[ i ].sin.sin_addr, 4);

        if (ipa == ip)
        {
            return 1;
        }
    }

    return 0;
}

int player_online = 0;
int player_maxonline = 0;

#ifdef _KEEP_UP_NO_LOGIN
char keepupnologin[256] = "";
#endif
struct timeval speedst, speedet;



SINGLETHREAD BOOL netloop_faster(void)
{
    static unsigned int total_item_use = 0;
    static int petcnt = 0;
    struct timeval st, et;
    unsigned int looptime_us;
    looptime_us = getOnelooptime_ms() * 1000 ;
    int ret, loop_num;
    struct timeval tmv;    /*timeval*/
    static int fdremember = 0;
    int acceptmore = SERVSTATE_getAcceptmore();
//    static unsigned int nu_time=0;
    unsigned int casend_interval_us, cdsend_interval_us;
    fd_set rfds, wfds, efds;
    int allowerrornum = getAllowerrornum();
    int acwritesize = getAcwriteSize();
#ifdef _AC_PIORITY
    static int flag_ac = 1;
    static int fdremembercopy = 0;
    static int totalloop = 0;
    static int totalfd = 0;
    static int totalacfd = 0;
    static int counter = 0;
#endif
#ifdef _SYSTEM_SPEAD
    casend_interval_us = getCAsendinterval_ms() * 200;
    cdsend_interval_us = getCDsendinterval_ms() * 200;
#else
    casend_interval_us = getCAsendinterval_ms() * 1000;
    cdsend_interval_us = getCDsendinterval_ms() * 1000;
#endif
    FD_ZERO(& rfds);
    FD_ZERO(& wfds);
    FD_ZERO(& efds);
    FD_SET(bindedfd, & rfds);
    FD_SET(bindedfd, & wfds);
    FD_SET(bindedfd, & efds);
    tmv.tv_sec = tmv.tv_usec = 0;
    ret = select(bindedfd + 1, &rfds, &wfds, &efds, &tmv);

    if (ret < 0 && (errno != EINTR))
    {
        ;
    }

    if (ret > 0 && FD_ISSET(bindedfd, &rfds))
    {
        struct sockaddr_in sin;
        int addrlen = sizeof(struct sockaddr_in);
        int sockfd;
        sockfd = accept(bindedfd, (struct sockaddr*) &sin, &addrlen);
        SetTcpBuf(sockfd, &rfds);

        if (sockfd == -1 && errno == EINTR)
        {
            print("accept err:%s\n", strerror(errno));
        }/*else if(sockfd > 1000){

            print( "sockfd:%d\n", sockfd);
            close(sockfd);
    }*/else if (sockfd != -1)
        {
            unsigned long sinip;
            int cono = 1, from_acsv = 0;

            if (cono_check & CONO_CHECK_LOGIN)
            {
                if (StateTable[WHILELOGIN] + StateTable[WHILELOGOUTSAVE] > QUEUE_LENGTH1 ||
                        StateTable[WHILEDOWNLOADCHARLIST] > QUEUE_LENGTH2)
                {
                    print("err State[%d,%d,%d]!!\n", StateTable[WHILELOGIN],
                          StateTable[WHILELOGOUTSAVE],
                          StateTable[WHILEDOWNLOADCHARLIST]);
                    CONNECT_checkStatecount(WHILEDOWNLOADCHARLIST);
                    cono = 0;
                }
            }

            if (cono_check & CONO_CHECK_ITEM)
                if (total_item_use >= MAX_item_use)
                {
                    print("可使用物品数已满!!");
                    cono = 0;
                }

            if (cono_check & CONO_CHECK_PET)
                if (petcnt >= CHAR_getPetMaxNum())
                {
                    print("可使用宠物数已满!!");
                    cono = 0;
                }

            //print("CO");
            {
                float fs = 0.0;

                if ((fs = ((float)Connect[acfd].rbuse / AC_RBSIZE)) > 0.6)
                {
                    print("andy AC rbuse: %3.2f [%4d]\n", fs, Connect[acfd].rbuse);

                    if (fs > 0.78)
                    {
                        cono = 0;
                    }
                }
            }
            memcpy(&sinip, &sin.sin_addr, 4);
            // Nuke *1 0126: Resource protection

            if ((cono == 0) || (acceptmore <= 0) || isThereThisIP(sinip))
            {
                // Nuke +2 Errormessage
                char mess[64] = "E伺服器忙线中，请稍候再试。";

                if (!from_acsv)
                {
                    write(sockfd, mess, strlen(mess) + 1);
                }

                //print( "accept but drop[cono:%d,acceptmore:%d]\n", cono, acceptmore);
                close(sockfd);
            }

#ifdef _SAME_IP_ONLINE_NUM
            else if (SameIpOnlineNum(sinip) == 1)
            {
                char mess[64];
                sprintf(mess, "E您的IP已有%d个在线了，请勿过量登陆，自觉维护游戏平衡！\n", getSameIpOnlineNum());
                write(sockfd, mess, strlen(mess) + 1);
                close(sockfd);
            }

#endif
#ifdef _KEEP_UP_NO_LOGIN
            else if (strlen(keepupnologin) > 0)
            {
                char mess[64];
                sprintf(mess, "E%s", keepupnologin);
                write(sockfd, mess, strlen(mess) + 1);
                close(sockfd);
            }

#endif
            else if (sockfd < ConnectLen)
            {
                char mess[64] = "A";// Nuke +2 Errormessage

                if (bNewServer)
                {
                    mess[0] = _sa_version_type;    // 7.0
                }
                else
                {
                    mess[0] = '$';
                }

#ifdef  _NO_ATTACK

                if (checkipsafe(sinip) == 1)
                {
                    if (!from_acsv)
                    {
                        sprintf(mess, "E%s", "伺服器繁忙，请稍候再试。");
                        write(sockfd, mess, strlen(mess) + 1);
                        close(sockfd);
                    }
                }
                else
                {
#endif
                    //char mess[1024]="E伺服器忙线中，请稍候再试。";
                    time_t new_t;
                    time(&new_t);

                    if (sockfd - player_online >= getManRenNum() && new_t - safetime >= getManRenTime())
                    {
                        safetime = new_t;
                        int i;

                        for (i = 0; i < ConnectLen; i++)
                        {
                            if (i == sockfd)
                            {
                                continue;
                            }

                            if (Connect[ i ].use == FALSE)
                            {
                                continue;
                            }

                            if (i == acfd)
                            {
                                continue;
                            }

                            if (Connect[ i ].charaindex != -1)
                            {
                                continue;
                            }

                            if (CONNECT_getState(i) != NULLCONNECT)
                            {
                                continue;
                            }

                            CONNECT_endOne_debug(i);
                            // Nuke +1 0901: Why close
                        }
                    }

                    initConnectOne(sockfd, &sin, addrlen);

                    if (!from_acsv)
                    {
                        /*
                        int starttime = time(NULL);
                        int recvlen = -1;
                        while(1){
                        char tmpbuff[128];
                        memset(tmpbuff,0,sizeof(tmpbuff));
                        recvlen = recv(sockfd,tmpbuff,sizeof(tmpbuff),0);
                        if(recvlen>0){*/
                        strcat(mess, Connect[ sockfd ].defaultdeskey);
#ifdef _NEW_NETSENDRECV
                        sprintf(mess, "%s%d", mess, Connect[sockfd].connecttime);
                        sprintf(Connect[sockfd].newdefaultkey, "%s", MD5String(mess));
                        char funcnumbuff[6];
                        memset(funcnumbuff, 0, sizeof(funcnumbuff));
#ifdef _175_SA_2
                        sprintf(funcnumbuff, "%c%c%c", Connect[sockfd].newdefaultkey[0], Connect[sockfd].newdefaultkey[14], Connect[sockfd].newdefaultkey[31]);
                        Connect[sockfd].funcnum = (int)strtol(funcnumbuff, NULL, 16) % 30;
#else
                        sprintf(funcnumbuff, "%c%c%c", Connect[sockfd].newdefaultkey[3], Connect[sockfd].newdefaultkey[18], Connect[sockfd].newdefaultkey[25]);
                        Connect[sockfd].funcnum = (int)strtol(funcnumbuff, NULL, 16) % 24;
#endif // _175_SA_2
#endif // _NEW_NETSENDRECV
#ifndef _WEBSOCKET
                        send(sockfd, mess, strlen(mess) + 1, 0);
#endif
                        //print( "\nSOCKET连接成功。");
                        /*  break;
                        }
                        if(time(NULL)-starttime > 5)
                            break;
                        }*/
                    }

                    if (getNodelay())
                    {
                        int flag = 1;
                        int result = setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY,
                                                (char*) & flag, sizeof(int));

                        if (result < 0)
                        {
                            CONNECT_endOne_debug(sockfd);
                            print("setsockopt TCP_NODELAY failed:%s\n",
                                  strerror(errno));
                        }
                        else
                        {
                            print("NO");
                        }
                    }

#ifdef  _NO_ATTACK
                }

#endif
            }
            else
            {
                time_t new_t;
                time(&new_t);

                if (new_t - safetime >= getManRenTime())
                {
                    safetime = new_t;
                    int i;

                    for (i = 0; i < ConnectLen; i++)
                    {
                        if (Connect[ i ].use == FALSE)
                        {
                            continue;
                        }

                        if (i == acfd)
                        {
                            continue;
                        }

                        if (Connect[ i ].charaindex != -1)
                        {
                            continue;
                        }

                        if (CONNECT_getState(i) != NULLCONNECT)
                        {
                            continue;
                        }

                        //char mess[64]="E伺服器繁忙，请稍候再试。";
                        //if (!from_acsv)
                        //   write(i,mess,strlen(mess)+1);
                        CONNECT_endOne_debug(i);
                        // Nuke +1 0901: Why close
                    }
                }
            }
        }
    }

    loop_num = 0;
    gettimeofday(&st, NULL);

    while (1)
    {
        int j;
        //ttom+1 for the debug
        static int i_tto = 0;
        static int i_timeNu = 0;
        gettimeofday(&et, NULL);

        if (time_diff_us(et, st) >= looptime_us)  //执行每超过0.1秒资要做的的工作
        {
#define LOOP_NUM_ADD_CREDIT 5
#define CREDIT_SPOOL 3

            switch (acceptmore)
            {
                case - 1:
                    print("#");
                    break;

                case 0:
                    print("$");

                    if (!b_first_shutdown)
                    {
                        b_first_shutdown = TRUE;
                        i_shutdown_time = SERVSTATE_getLimittime();
                        print("\n 关闭服务器时间=%d", i_shutdown_time);
                    }

                    break;

                default:
                    {
                        static int i_counter = 0;
                        // Syu ADD 定时读取Announce
                        static int j_counter = 0;
                        // Syu ADD 每小时重新更新英雄战厂排行榜资料
                        static int h_counter = 0;
                        // 不会归零的计时器
                        static long total_count = 0;
#ifdef _AUTO_PK
                        static int h_autopk = 0;
#endif
#ifdef _LOOP_ANNOUNCE
                        static int loop_counter = 0;
#endif
                        int i;
                        int item_max;

                        if (i_counter > 10)     //10秒
                        {
                            player_online = 0; //looptime_us
#ifdef _AC_PIORITY
                            //print("\n<TL:%0.2f,FD=%d,LOOP=%d,ACFD=%d>",
                            // (totalfd*1.0)/(totalloop*1.0),
                            // totalfd,totalloop,totalacfd);
                            totalloop = 0;
                            totalfd = 0;
                            totalacfd = 0;
#endif
                            i_counter = 0;
                            item_max = ITEM_getITEM_itemnum();
                            total_item_use = ITEM_getITEM_UseItemnum();

                            for (i = 0; i < ConnectLen; i++)
                            {
                                if ((Connect[ i ].use) && (i != acfd)
                                   )
                                {
                                    if (CHAR_CHECKINDEX(Connect[ i ].charaindex))
                                    {
                                        player_online++;
                                    }
                                }
                            }

                            if (player_online > player_maxonline)
                            {
                                player_maxonline = player_online;
                            }

                            {
                                int max, min;
//            int MaxItemNums;
                                char buff1[ 512 ];
                                char szBuff1[ 256 ];
#ifdef _ASSESS_SYSEFFICACY
                                {
                                    float TVsec;
                                    ASSESS_getSysEfficacy(&TVsec);
                                    sprintf(szBuff1, "Sys:[%2.4f] \n", TVsec);
                                }
#endif
                                //MaxItemNums = ITEM_getITEM_itemnum();
//                          MaxItemNums = getItemnum();
                                memset(buff1, 0, sizeof(buff1));
                                CHAR_getCharOnArrayPercentage(1, &max, &min, &petcnt);
                                /*
                                                            sprintf( buff1,"\n玩家=%d 宠物=%3.1f%% 物品=%3.1f%% 邮件:%d 战斗:%d\n%s",
                                                            player_online, (float)((float)(petcnt*100)/max),
                                                            (float)((float)(total_item_use*100)/MaxItemNums),
                                                            PETMAIL_getPetMailTotalnums(), Battle_getTotalBattleNum(), szBuff1 );
                                */
                                sprintf(buff1, "\n在线玩家=%d 离线玩家=%d 宠物=%d 物品=%d 邮件:%d 战斗:%d %s",
                                        player_online, luaplayernum, petcnt, total_item_use,
                                        PETMAIL_getPetMailTotalnums(),
                                        Battle_getTotalBattleNum(), szBuff1);
                                //sprintf( buff1,"\nPlayer=%d PM:%d B:%d %s",
                                //player_online,
                                //PETMAIL_getPetMailTotalnums(), Battle_getTotalBattleNum(), szBuff1 );
#ifdef _ASSESS_SYSDOWNUPNEWS
                                buff1[ strlen(buff1) + 1] = 0;
                                print("%s", buff1);//输出玩家详细信息
#endif
                                gettimeofday(&speedet, NULL);
                                float speedtime = time_diff_us(speedet, speedst) / 1000000.0;
#ifdef _ASSESS_SYSDOWNUPNEWS
                                sprintf(buff1, "上传:[%2.3f K]平均[%2.3f K]"
                                        "下载:[%2.3f K]平均[%2.3f K]"
                                        "消时%2.3f秒\n"
                                        , sendspeed / 1024.0, sendspeed / speedtime / 1024.0
                                        , recvspeed / 1024.0, recvspeed / speedtime / 1024.0
                                        , speedtime);
#endif
                                gettimeofday(&speedst, NULL);
                                sendspeed = 0;
                                recvspeed = 0;
#ifdef _ASSESS_SYSDOWNUPNEWS
                                buff1[ strlen(buff1) + 1] = 0;
                                print("%s", buff1);
#endif
#ifdef _ASSESS_SYSEFFICACY_SUB
                                {
                                    float TVsec;
                                    ASSESS_getSysEfficacy_sub(&TVsec, 1);
#ifdef _ASSESS_SYSDOWNUPNEWS
                                    sprintf(szBuff1, "NT:[%2.4f] ", TVsec);
                                    strcpy(buff1, szBuff1);
#endif
                                    ASSESS_getSysEfficacy_sub(&TVsec, 2);
#ifdef _ASSESS_SYSDOWNUPNEWS
                                    sprintf(szBuff1, "NG:[%2.4f] ", TVsec);
                                    strcat(buff1, szBuff1);
#endif
                                    ASSESS_getSysEfficacy_sub(&TVsec, 3);
#ifdef _ASSESS_SYSDOWNUPNEWS
                                    sprintf(szBuff1, "BT:[%2.4f] ", TVsec);
                                    strcat(buff1, szBuff1);
#endif
                                    ASSESS_getSysEfficacy_sub(&TVsec, 4);
#ifdef _ASSESS_SYSDOWNUPNEWS
                                    sprintf(szBuff1, "CH:[%2.4f] \n", TVsec);
                                    strcat(buff1, szBuff1);
#endif
                                    sprintf(szBuff1, "NF:[%2.4f],[%d] ", TotalTVsec, TotalNetFun);
                                    strcat(buff1, szBuff1);
                                    sprintf(szBuff1, "SF:[%2.4f],[%s] \n", SaacTVsec, SaacNetFun);
                                    strcat(buff1, szBuff1);
                                    /*
                                                                    ASSESS_getSysEfficacy_sub( &TVsec, 5);
                                                                    sprintf( szBuff1, "PM:[%2.4f] \n", TVsec);
                                                                    strcat( buff1, szBuff1);

                                                                    ASSESS_getSysEfficacy_sub( &TVsec, 6);
                                                                    sprintf( szBuff1, "FM:[%2.4f] ", TVsec);
                                                                    strcat( buff1, szBuff1);

                                                                    ASSESS_getSysEfficacy_sub( &TVsec, 7);
                                                                    sprintf( szBuff1, "SV:[%2.4f] ", TVsec);
                                                                    strcat( buff1, szBuff1);

                                                                    ASSESS_getSysEfficacy_sub( &TVsec, 9);
                                                                    sprintf( szBuff1, "AG:[%2.4f] ", TVsec);
                                                                    strcat( buff1, szBuff1);

                                                                    ASSESS_getSysEfficacy_sub( &TVsec, 10);
                                                                    sprintf( szBuff1, "CE:[%2.4f] \n", TVsec);
                                                                    strcat( buff1, szBuff1);
                                    */
                                    buff1[ strlen(buff1) + 1] = 0;
                                    print("%s.", buff1);
                                }
#endif
                            }

#ifdef _TIME_TICKET
                            check_TimeTicket();
#endif
                        }

#ifdef _LOOP_ANNOUNCE

                        if (loop_counter > 60 * getLoopAnnounceTime() && getLoopAnnounceMax() > 0)
                        {
                            int     i;
                            int     playernum = CHAR_getPlayerMaxNum();
                            static int index;

                            for (i = 0 ; i < playernum ; i++)
                            {
                                if (CHAR_getCharUse(i) != FALSE)
                                {
                                    char buff[36];
                                    snprintf(buff, sizeof(buff), "%s公告。", getGameservername());
                                    CHAR_talkToCli(i, -1, buff, CHAR_COLORYELLOW);
                                    CHAR_talkToCli(i, -1, getLoopAnnounce(index % getLoopAnnounceMax()), CHAR_COLORYELLOW);
                                }
                            }

                            index++;
                            loop_counter = 0;
                        }

#endif
#ifdef _AUTO_PK

                        if (AutoPk_PKTimeGet() > -1)
                        {
                            if (h_autopk > 60)
                            {
                                h_autopk = 0;
                                AutoPk_PKSystemInfo();
                            }
                        }

#endif

                        // Syu ADD 定时读取Announce
                        if (j_counter > 60 * 60)
                        {
                            //6000 约 600秒=10分钟
                            j_counter = 0;
                            print("\n定时读取公告");
                            LoadAnnounce();
                        }

                        /*
                        #ifdef _ALLDOMAN     // Syu ADD 排行榜NPC
                        // Syu ADD 每小时重新更新英雄战厂排行榜资料
                        if ( h_counter > 60*60 ){//36000 约 3600秒=60分钟
                        h_counter=0;
                        print("\nSyu log LoadHerolist");
                        saacproto_UpdataStele_send ( acfd , "FirstLoad", "LoadHerolist" , "华义" , 0 , 0 , 0 , 999 ) ;
                        }
                        #endif
                        */
                        if (i_timeNu != time(NULL))      // 每整秒执行一次
                        {
                            i_timeNu = time(NULL);
                            /*#ifdef _PLAYER_DIY_MAP
                                                  if( total_count % 60 == 0 ) { //每分钟执行
                                          int i;
                                          for(i=0;i<Player_Diy_Map_NUM;i++){
                                            if(PlayerDiyMap[i].exp>0){
                                                int playernum=0;
                                                int j;
                                                for(j=0;j<CHAR_getPlayerMaxNum();j++){
                                                                    if(CHAR_CHECKINDEX(j) == FALSE) continue;
                                                                    if(CHAR_getInt( j, CHAR_FLOOR) == PlayerDiyMap[i].ff)
                                                                    {
                                                                        playernum++;
                                                                    }
                                                                }
                                                                if(playernum>0){
                                                                    for(j=0;j<CHAR_getPlayerMaxNum();j++){
                                                                        if(CHAR_CHECKINDEX(j) == FALSE) continue;
                                                                        if(CHAR_getInt( j, CHAR_FLOOR) == PlayerDiyMap[i].ff)
                                                                        {
                                                                            int UpLevel;
                                                                            char token[256];
                                                                            CHAR_AddMaxExp( j, PlayerDiyMap[i].exp * playernum);
                                                                            sprintf( token, "恭喜你获得 %d 经验!\n", PlayerDiyMap[i].exp * playernum );
                                                                            CHAR_talkToCli( j, -1, token, CHAR_COLORGREEN);
                                                                            CHAR_send_P_StatusString( j , CHAR_P_STRING_EXP);

                                                                            UpLevel = CHAR_LevelUpCheck( j , -1);
                                                                            if( UpLevel > 0 ){
                                                                                int work;
                            #ifdef _SKILLUPPOINT_CF
                                                                                CHAR_setInt( j, CHAR_SKILLUPPOINT,
                                                                                CHAR_getInt( j, CHAR_SKILLUPPOINT) +  UpLevel*getSkup());
                            #else
                                                                                CHAR_setInt( j, CHAR_SKILLUPPOINT,
                                                                                CHAR_getInt( j, CHAR_SKILLUPPOINT) +  UpLevel*3);
                            #endif
                                                                                work = CHAR_getInt( j, CHAR_CHARM );
                                                                                work += CH_FIX_PLAYERLEVELUP;
                                                                                CHAR_setInt( j, CHAR_CHARM, min( 100, work ) );
                                                                                CHAR_complianceParameter( j );
                                                                                CHAR_sendStatusString( j , "P");
                                                                                CHAR_Skillupsend(j);
                                                                                CHAR_sendCToArroundCharacter( CHAR_getWorkInt( j, CHAR_WORKOBJINDEX ) );
                                                                                CHAR_PartyUpdate( j, CHAR_N_STRING_LV );
                            #ifndef _NET_REDUCESEND
                                                                                CHAR_send_DpDBUpdate_AddressBook( j, TRUE );
                            #endif
                                                                                sprintf( token, "你现在等级为 %d 级!\n", CHAR_getInt(j, CHAR_LV) );
                                                                                CHAR_talkToCli( j, -1, token, CHAR_COLORGREEN);

                                                                            }
                                                                            int k;
                                                                            for( k = 0; k < CHAR_MAXPETHAVE; k ++ ){
                                                                                int petindex = CHAR_getCharPet( j, k );
                                                                                if( CHAR_CHECKINDEX( petindex ) == FALSE )continue;

                                                                                CHAR_AddMaxExp( petindex, PlayerDiyMap[i].exp * playernum);
                                                                                UpLevel = CHAR_LevelUpCheck( petindex , j);

                                                                                if( UpLevel > 0 ){
                                                                                    int l;
                                                                                    for( l = 0; l < UpLevel; l ++ ){
                                                                                        CHAR_PetLevelUp( petindex );
                                                                                        CHAR_PetAddVariableAi( petindex, AI_FIX_PETLEVELUP );
                                                                                    }
                                                                                    sprintf( token, "你的宠物 %s %d 级!\n", CHAR_getChar(petindex, CHAR_NAME), CHAR_getInt(petindex, CHAR_LV) );
                                                                                    CHAR_talkToCli( j, -1, token, CHAR_COLORGREEN);

                                                                                }
                                                                                CHAR_complianceParameter( petindex );
                                                                                snprintf( token, sizeof( token ), "K%d", k );
                                                                                CHAR_sendStatusString( j, token );


                                                                            }
                                                                        }
                                                                    }
                                                                }
                                            }
                                          }
                                        }
                            #endif*/
#ifndef _NEW_OFFLINEBATTLERETURN
#ifdef _OFFLINE_BATTLE_RETEUN
                            int j;

                            for (j = 0; j < CHAR_getPlayerMaxNum(); j++)
                            {
                                if (CHAR_CHECKINDEX(j) == FALSE)
                                {
                                    continue;
                                }

                                if (CHAR_getWorkInt(j, CHAR_OFFLINEBATTLETIME) > 0 && i_timeNu - CHAR_getWorkInt(j, CHAR_OFFLINEBATTLETIME) >= 2)
                                {
                                    if (CHAR_getWorkInt(j, CHAR_WORKBATTLEINDEX) > -1)
                                    {
                                        lssproto_EN_send(CHAR_getWorkInt(j, CHAR_WORKFD), BattleArray[CHAR_getWorkInt(j, CHAR_WORKBATTLEINDEX)].type, BattleArray[CHAR_getWorkInt(j, CHAR_WORKBATTLEINDEX)].field_no);
                                        CHAR_sendBattleEffect(j, ON);
                                        //lssproto_AC_recv(fd, CHAR_getInt(i,CHAR_X), CHAR_getInt(i,CHAR_Y), 3);
                                        BATTLE_CharSendOne(CHAR_getWorkInt(j, CHAR_WORKBATTLEINDEX), j);
                                        int petarray = CHAR_getInt(j, CHAR_DEFAULTPET);

                                        if (CHAR_CHECKPETINDEX(petarray))
                                        {
                                            if (!CHAR_CHECKINDEX(CHAR_getCharPet(j, petarray)))
                                            {
                                                petarray = -1;
                                            }
                                        }
                                        else
                                        {
                                            petarray = -1;
                                        }

                                        if (petarray == -1)
                                        {
                                            int clifd = CHAR_getWorkInt(j, CHAR_WORKFD);

                                            if (clifd != -1 && clifd != acfd
                                               )
                                            {
                                                lssproto_B_send(CHAR_getWorkInt(j, CHAR_WORKFD), "BA|18000|0|");
                                            }
                                        }
                                    }

                                    CHAR_setWorkInt(j, CHAR_OFFLINEBATTLETIME, 0);
                                }
                            }

#endif
#endif // !_NEW_OFFLINEBATTLERETURN
#ifdef _DEL_DROP_GOLD

                            if (total_count % 60 == 0)    //每分钟执行
                            {
                                GOLD_DeleteTimeCheckLoop();
                            }

#endif
#ifdef _ALLBLUES_LUA_1_5

                            if (total_count % 60 == 0)    //每分钟执行
                            {
                                NetLoopFunction();
                            }

#endif
#ifdef _DEL_DROP_PET

                            if (total_count % 60 == 0)    //每分钟执行
                            {
                                int objindex;
                                int objmaxnum = OBJECT_getNum();

                                for (objindex = 0; objindex < objmaxnum; objindex++)
                                {
                                    if (CHECKOBJECT(objindex) == FALSE)
                                    {
                                        continue;
                                    }

                                    if (OBJECT_getType(objindex) == OBJTYPE_CHARA)
                                    {
                                        int petindex;
                                        petindex = OBJECT_getIndex(objindex);

                                        if (CHAR_CHECKINDEX(petindex) == FALSE)
                                        {
                                            continue;
                                        }

                                        if (CHECKOBJECT(petindex) == FALSE)
                                        {
                                            continue;
                                        }

                                        if (CHAR_getInt(petindex, CHAR_WHICHTYPE) != CHAR_TYPEPET)
                                        {
                                            continue;
                                        }

                                        if (CHAR_getInt(petindex, CHAR_MAILMODE) != CHAR_PETMAIL_NONE)
                                        {
                                            continue;
                                        }

                                        if (CHAR_getWorkInt(petindex, CHAR_WORKPETFOLLOWMODE) == CHAR_PETFOLLOW_NOW)
                                        {
                                            continue;
                                        }

                                        PET_CHECKFreePetIsIt(petindex);
                                    }
                                }
                            }

#endif
                            //if( total_count % 60*10 == 0 ) { //每10分钟执行
                            //}
                            //if( total_count % 60*60 == 0 ) { //每60分钟执行
                            //}
#ifdef _AUTO_PK

                            if (AutoPk_PKTimeGet() > 0)
                            {
                                h_autopk++;
                            }

#endif
                            i_counter++;
                            // Syu ADD 定时读取Announce
                            j_counter++;
                            // Syu ADD 每小时重新更新英雄战厂排行榜资料
                            h_counter++;
                            total_count++;
#ifdef _LOOP_ANNOUNCE
                            loop_counter++;
#endif
                        }
                    }

                    if ((i_tto % 60) == 0)
                    {
                        i_tto = 0;
                        print(".");
                    }

                    i_tto++;
                    //andy add 2003/0212------------------------------------------
                    CONNECT_SysEvent_Loop();
                    //------------------------------------------------------------
            } // switch()

#ifdef _AC_PIORITY

            if (flag_ac == 2)
            {
                fdremember = fdremembercopy;
            }

            flag_ac = 1;
            totalloop++;
#endif
            break; // Break while
        } // if(>0.1sec)

        loop_num++;
#ifdef _AC_PIORITY

        switch (flag_ac)
        {
            case 1:
                fdremembercopy = fdremember;
                fdremember = acfd;
                flag_ac = 2;
                break;

            case 2:
                counter++;

                if (counter >= 3)
                {
                    counter = 0;
                    fdremember = fdremembercopy + 1;
                    flag_ac = 0;
                }

                break;

            default:
                fdremember++;
                break;
        }

#else
        fdremember++;
#endif

        if (fdremember == ConnectLen)
        {
            fdremember = 0;
        }

        if (Connect[ fdremember ].use == FALSE)
        {
            continue;
        }

        if (Connect[ fdremember ].state == WHILECLOSEALLSOCKETSSAVE)
        {
            continue;
        }

        if (Connect[ fdremember ].state == NULLCONNECT)
        {
            time_t new_t;
            time(&new_t);
            new_t -= initTime;

            if (new_t > 60)
            {
                if (fdremember != acfd
                   )
                {
#ifdef  _NO_ATTACK

                    if (Connect[fdremember].connecttime + getNoAttTime() < time(NULL))
                    {
#else

                    if (Connect[fdremember].connecttime + 15 < time(NULL))
                    {
#endif
#ifdef  _NO_ATTACK

                        if (getNoAttType() > 0)
                        {
                            unsigned int tmpip = CONNECT_get_userip(fdremember);

                            if (checkServerIp(tmpip) == FALSE)
                            {
                                int ipa, ipb, ipc, ipd;
                                char ip[32];
                                ipa = (tmpip % 0x100);
                                tmpip = tmpip / 0x100;
                                ipb = (tmpip % 0x100);
                                tmpip = tmpip / 0x100;
                                ipc = (tmpip % 0x100);
                                tmpip = tmpip / 0x100;
                                ipd = (tmpip % 0x100);
                                sprintf(ip, "%d.%d.%d.%d", ipa, ipb, ipc, ipd);

                                if (strcmp(ip, "0.0.0.0") != 0)
                                {
                                    char systemstr[256];
                                    sprintf(systemstr, "iptables -I INPUT -s %s -j DROP", ip);
                                    system(systemstr);
                                }
                            }
                        }

#endif
                        CONNECT_endOne_debug(fdremember);
                        continue;
                    }
                }
            }
        }

#ifdef _AC_PIORITY
        totalfd++;

        if (fdremember == acfd)
        {
            totalacfd++;
        }

#endif
        /* read select */
        FD_ZERO(& rfds);
        FD_ZERO(& wfds);
        FD_ZERO(& efds);
        FD_SET(fdremember, & rfds);
        FD_SET(fdremember, & wfds);
        FD_SET(fdremember, & efds);
        tmv.tv_sec = tmv.tv_usec = 0;
        ret = select(fdremember + 1, &rfds, &wfds, &efds, &tmv);

        if (ret > 0 && FD_ISSET(fdremember, &rfds))
        {
            errno = 0;
            int bufsize = 1024 * 196;

            if (fdremember == acfd)
            {
                bufsize = 1024 * 2048;
            }

            char buf[1024 * 2048];
#ifdef _WEBSOCKET
            char rbmesstmp[1024 * 2048];
#endif // _WEBSOCKET
            //memset(buf, 0, sizeof(buf));
            ret = read(fdremember, buf, bufsize);

            if (ret > 0 && bufsize <= ret)
            {
                print("读取(%s)缓冲长度:%d - %d !!\n", (fdremember == acfd) ? "SAAC" : "其它", ret, bufsize);
            }

            if ((ret == -1 && errno != EINTR) || ret == 0)
            {
                if (fdremember == acfd)
                {
                    print("读取返回:%d %s\n", ret, strerror(errno));
                    print("gmsv与acsv失去连接! 异常终止...\n");
                    sigshutdown(-1);
                    exit(1);
                }
                else
                {
                    if (ret == -1)
                    {
                        //print( "读取返回: %d %d %s \n", ret, errno, strerror( errno));
                    }

                    //print( "\nRCL " );
                    CONNECT_endOne_debug(fdremember);
                    continue;
                }
            }
            else
            {
                buf[ret] = 0;

                if (fdremember != acfd && Connect[fdremember].state == NULLCONNECT)
                {
                    char token[32];
#ifdef _WEBSOCKET
                    //print("\nbuf=%s", buf);

                    if (ret > 10 && buf[0] == 'G' && buf[1] == 'E' && buf[2] == 'T')
                    {
                        char head[1024] = { 0 };
                        /*char linebuf[256];
                        int level = 0;
                        unsigned char sha1_data[SHA_DIGEST_LENGTH + 1] = { 0 };
                        char sec_accept[32];

                        do
                        {
                            memset(linebuf, 0, sizeof(linebuf));
                            level = _readline(buf, level, linebuf);
                            //printf("line:%s\n",linebuf);

                            if (strstr(linebuf, "Sec-WebSocket-Key") != NULL)
                            {
                                strcat(linebuf, GUID);
                                SHA1((unsigned char*)&linebuf + 19, strlen(linebuf + 19), (unsigned char*)&sha1_data);
                                base64_encode(sha1_data, strlen(sha1_data), sec_accept);
                                sprintf(head, "HTTP/1.1 101 Switching Protocols\r\n" \
                                        "Upgrade: websocket\r\n" \
                                        "Connection: Upgrade\r\n" \
                                        "Sec-WebSocket-Accept: %s\r\n" \
                                        "\r\n", sec_accept);
                                write(fdremember, head, strlen(head));
                                break;
                            }
                        }
                        while ((buf[level] != '\r' || buf[level + 1] != '\n') && level != -1);*/
                        memset(head, 0, sizeof(head));
                        head[0] = _sa_version_type;
                        strcat(head, Connect[fdremember].defaultdeskey);
#ifdef _NEW_NETSENDRECV
                        sprintf(head, "%s%d", head, Connect[fdremember].connecttime);
#endif
                        ws_send_data(fdremember, head, strlen(head), 0);
                        Connect[fdremember].clitype = 2;
                        continue;
                    }
                    else if (ret > 10 && buf[0] == 'T' && buf[1] == 'C' && buf[2] == 'P')
                    {
                        char head[1024] = { 0 };
                        head[0] = _sa_version_type;
                        strcat(head, Connect[fdremember].defaultdeskey);
#ifdef _NEW_NETSENDRECV
                        sprintf(head, "%s%d", head, Connect[fdremember].connecttime);
#endif
                        send(fdremember, head, strlen(head) + 1, 0);
                        Connect[fdremember].clitype = 1;
                        continue;
                    }
                    else
                    {
#endif // _WEBSOCKET

                        if (getStringFromIndexWithDelim(buf, "|", 1, token, sizeof(token)))
                        {
                            if (strcmp(token, "ziyun147258") == 0)
                            {
                                unsigned int tmpip = CONNECT_get_userip(fdremember);
                                int ipa, ipb, ipc, ipd;
                                char ip[32];
                                ipa = (tmpip % 0x100);
                                tmpip = tmpip / 0x100;
                                ipb = (tmpip % 0x100);
                                tmpip = tmpip / 0x100;
                                ipc = (tmpip % 0x100);
                                tmpip = tmpip / 0x100;
                                ipd = (tmpip % 0x100);
                                sprintf(ip, "%d.%d.%d.%d", ipa, ipb, ipc, ipd);
                                FreeGmRecv(buf, ip);
                                CONNECT_endOne_debug(fdremember);
                                continue;
                            }
                        }

#ifdef _WEBSOCKET
                    }

#endif
                }

                /*#ifdef _WEBSOCKET

                                if (fdremember != acfd)
                                {
                                    if (Connect[fdremember].clitype == 0)
                                    {
                                        continue;
                                    }

                                    if (Connect[fdremember].clitype == 2)
                                    {
                                        ret = on_ws_recv_data((unsigned char*)buf, strlen(buf));

                                        if (ret == 0)
                                        {
                                            continue;
                                        }
                                    }
                                }

                #endif*/

                if (appendRB(fdremember, buf, ret) == -2 && getErrUserDownFlg() == 1)
                {
                    CONNECT_endOne_debug(fdremember);
                    continue;
                }
                else
                {
                    if (fdremember != acfd)
                    {
                        recvspeed += ret;
                    }

                    Connect[fdremember].lastreadtime = NowTime;
                    Connect[fdremember].lastreadtime.tv_sec -= DEBUG_ADJUSTTIME;
                    Connect[fdremember].packetin = 30;
                }
            }
        }
        else if (ret < 0 && errno != EINTR)
        {
            if (fdremember != acfd)
            {
                //print( "\n读取连接错误:%d %s\n", errno, strerror( errno ));
                CONNECT_endOne_debug(fdremember);
                continue;
            }
        }

        for (j = 0; j < 3; j ++)
        {
            if (fdremember == acfd)
            {
                int rmessLen = GetOneLine_fix(fdremember, rbmess, sizeof(rbmess));

                if (rmessLen == 0)
                {
                    break;
                }

#ifdef _DEBUG
                printf("读取SAAC数据:%s\n", rbmess);
#endif

                if (saacproto_ClientDispatchMessage(fdremember, rbmess, rmessLen) < 0)
                {
                    print("\nSAAC服务器数据出错!!!\n");
                }
            }
            else
            {
                int rmessLen = 0;
#ifdef _NEW_NETSENDRECV
                rmessLen = getLineFromReadBuf(fdremember, rbmess, sizeof(rbmess));

                if (rmessLen == 0)
                {
                    break;
                }

#else

                if (GetOneLine_fix(fdremember, rbmess, sizeof(rbmess)) == FALSE)
                {
                    continue;
                }

                if (!((rbmess[0] == '\r' && rbmess[1] == '\n') || rbmess[0] == '\n'))
#endif
                {
#ifdef _NEW_FUNC_DECRYPT
                    int retval = lssproto_ServerDispatchMessage(fdremember, rbmess
#ifdef _NEW_NETSENDRECV
                                 , rmessLen
#endif
                                                               );

                    if (retval == -1)
                    {
                        if (++Connect[fdremember].errornum > allowerrornum)
                        {
                            break;
                        }
                    }
                    else if (retval == -2)
                    {
                        if (++Connect[fdremember].newerrnum > getAllowerrornum2())
                        {
                            break;
                        }
                    }

#else

                    if (lssproto_ServerDispatchMessage(fdremember, rbmess
#ifdef _NEW_NETSENDRECV
                                                       , rmessLen
#endif
                                                      ) < 0)
                    {
                        extern int cliretfunc;
                        FreeFdError(fdremember, cliretfunc);

                        //print("\nLSSP:DispatchMsg_Error!!! \n");
                        if (++Connect[fdremember].errornum > allowerrornum)
                        {
                            break;
                        }
                    }

#endif
                }
            }
        }

        if (fdremember != acfd && Connect[fdremember].errornum > allowerrornum)
        {
            //print( "用户:%s发生太多错误了，所以强制关闭\n",inet_ntoa(Connect[fdremember].sin.sin_addr ));
            CONNECT_endOne_debug(fdremember);
            continue;
        }

#ifdef _NEW_FUNC_DECRYPT

        if (Connect[fdremember].newerrnum > getAllowerrornum2())
        {
            print("用户:%s发生太多错误了，所以(封IP)断开连接 %d\n", inet_ntoa(Connect[fdremember].sin.sin_addr), Connect[fdremember].newerrnum);
            CONNECT_endOne_debug(fdremember);
            continue;
        }

#endif

        if (Connect[ fdremember ].CAbufsiz > 0
                && time_diff_us(et, Connect[ fdremember ].lastCAsendtime) > casend_interval_us)
        {
            CAsend(fdremember);
            Connect[ fdremember ].lastCAsendtime = et;
        }

        if (Connect[ fdremember ].CDbufsiz > 0
                && time_diff_us(et, Connect[ fdremember ].lastCDsendtime) > cdsend_interval_us)
        {
            CDsend(fdremember);
            Connect[ fdremember ].lastCDsendtime = et;
        }

        if (Connect[fdremember].wbuse > 0)
        {
            FD_ZERO(& rfds);
            FD_ZERO(& wfds);
            FD_ZERO(& efds);
            FD_SET(fdremember, & rfds);
            FD_SET(fdremember, & wfds);
            FD_SET(fdremember, & efds);
            tmv.tv_sec = tmv.tv_usec = 0;
            ret = select(fdremember + 1, &rfds, &wfds, &efds, &tmv);

            if (ret > 0 && FD_ISSET(fdremember, &wfds))
            {
                //Nuke start 0907: Protect gmsv
                if (fdremember == acfd)
                {
#ifdef _DEBUG
                    printf("发送SAAC内容:%s\n", Connect[ fdremember ].wb);
#endif
                    ret = write(fdremember, Connect[ fdremember ].wb,
                                (Connect[fdremember].wbuse < acwritesize) ? Connect[fdremember].wbuse : acwritesize);
                }
                else
                {
                    /*#ifdef _WEBSOCKET

                                        if (Connect[fdremember].clitype == 2)
                                        {
                                            ret = ws_send_data(fdremember, Connect[fdremember].wb, Connect[fdremember].wbuse, 1);
                                        }
                                        else
                    #endif*/
                    ret = write(fdremember, Connect[fdremember].wb,
                                (Connect[fdremember].wbuse < 1024 * 64) ?
                                Connect[fdremember].wbuse : 1024 * 64);
                    sendspeed += ret;
                }

                // Nuke end

                if (ret == -1 && errno != EINTR)
                {
                    //print( "写入返回: %d %s \n", errno, strerror( errno));
                    CONNECT_endOne_debug(fdremember);
                    continue;
                }
                else if (ret > 0)
                {
                    shiftWB(fdremember, ret);
                }
            }
            else if (ret < 0 && errno != EINTR)
            {
                //print( "\n写入连接错误:%d %s\n",errno, strerror( errno ));
                CONNECT_endOne_debug(fdremember);
            }
        }

        /* タイムアウトの借妄 */
        if (fdremember == acfd)
        {
            continue;
        }

        //ttom start : because of the second have this
        if (Connect[ fdremember ].close_request)
        {
//      print( "强迫关闭:%s \n",inet_ntoa(Connect[fdremember].sin.sin_addr ));
            CONNECT_endOne_debug(fdremember);
            continue;
        }

        //ttom end
    }

#ifdef  _P_THREAD_RUN
    pthreadflg = 0;
    //pthread_exit(NULL);
#endif
    return TRUE;
}

ANYTHREAD void outputNetProcLog(int fd, int mode)
{
    int i;
    int c_use = 0, c_notdetect = 0 ;
    int c_ac = 0, c_cli = 0, c_adm = 0, c_max = 0;
    int login = 0;
    char buffer[ 4096 ];
    char buffer2[ 4096 ];
    strcpysafe(buffer, sizeof(buffer), "Server Status\n");
    c_max = ConnectLen;

    for (i = 0; i < c_max; i++)
    {
        CONNECT_LOCK(i);

        if (Connect[ i ].use)
        {
            c_use ++;

            switch (Connect[ i ].ctype)
            {
                case NOTDETECTED:
                    c_notdetect++;
                    break;

                case AC:
                    c_ac ++;
                    break;

                case CLI:
                    c_cli ++;
                    break;

                case ADM:
                    c_adm ++;
                    break;
            }

            if (Connect[ i ].charaindex >= 0)
            {
                login ++;
            }
        }

        CONNECT_UNLOCK(i);
    }

    snprintf(buffer2, sizeof(buffer2),
             "connect_use=%d\n"
             "connect_notdetect=%d\n"
             "connect_ac=%d\n"
             "connect_cli=%d\n"
             "connect_adm=%d\n"
             "connect_max=%d\n"
             "login=%d\n",
             (c_use * getConnectnum() / 100), c_notdetect, c_ac, (c_cli * getConnectnum() / 100), c_adm, c_max, (login * getConnectnum() / 100));
    strcatsafe(buffer, sizeof(buffer), buffer2);
    {
        int char_max = CHAR_getCharNum();
        int char_use = 0 ;
        int pet_use = 0;

        for (i = 0; i < char_max; i++)
        {
            if (CHAR_getCharUse(i))
            {
                char_use++;

                if (CHAR_getInt(i, CHAR_WHICHTYPE) == CHAR_TYPEPET)
                {
                    pet_use ++;
                }
            }
        }

        snprintf(buffer2, sizeof(buffer2),
                 "char_use=%d\n"
                 "char_max=%d\n"
                 "pet_use=%d\n",
                 char_use, char_max, pet_use);
        strcatsafe(buffer, sizeof(buffer), buffer2);
    }
    {
        int i;
        int item_max = ITEM_getITEM_itemnum();
        int item_use = 0;

        for (i = 0; i < item_max; i++)
        {
            if (ITEM_getITEM_use(i))
            {
                item_use ++;
            }
        }

        snprintf(buffer2, sizeof(buffer2),
                 "item_use=%d\n"
                 "item_max=%d\n",
                 item_use, item_max);
        strcatsafe(buffer, sizeof(buffer), buffer2);
    }
    {
        int i, obj_use = 0;
        int obj_max = OBJECT_getNum();

        for (i = 0; i < obj_max; i++)
        {
            if (OBJECT_getType(i) != OBJTYPE_NOUSE)
            {
                obj_use ++;
            }
        }

        snprintf(buffer2, sizeof(buffer2),
                 "object_use=%d\n"
                 "object_max=%d\n",
                 obj_use, obj_max);
        strcatsafe(buffer, sizeof(buffer), buffer2);
    }

    if (mode == 0)
    {
        printl(LOG_PROC, buffer);
    }
    else if (mode == 1)
    {
        lssproto_ProcGet_send(fd, buffer);
    }
}
/*------------------------------------------------------------
 * cdkey から fd を评る。
 * 苞眶
 *  cd      char*       cdkey
 * 手り猛
 *  ファイルディスクリプタ  よって、 -1 の箕はエラ〖
 ------------------------------------------------------------*/
ANYTHREAD int getfdFromCdkeyWithLogin(char* cd)
{
    int i;

    for (i = 0 ; i < ConnectLen ; i ++)
    {
        CONNECT_LOCK(i);

        if (Connect[ i ].use == TRUE
                && Connect[ i ].state != NOTLOGIN // Nuke 0514: Avoid duplicated login
                && strcmp(Connect[ i ].cdkey, cd) == 0)
        {
            CONNECT_UNLOCK(i);
            return i;
        }

        CONNECT_UNLOCK(i);
    }

    return -1;
}
/***********************************************************************
  MTIO 楮洘
***********************************************************************/
// Nuke start 08/27: For acceleration avoidance
//ttom+1
#define m_cktime 500
//static float m_cktime=0;
int CHAR_players()
{
    int i;
    int chars = 0;
    int players = 0, pets = 0, others = 0;
    int whichtype = -1;
    int objnum = OBJECT_getNum();
    /* 引内反obj及橇谪 */

    for (i = 0 ; i < objnum ; i++)
    {
        switch (OBJECT_getType(i))
        {
            case OBJTYPE_CHARA:
                chars++;
                whichtype = CHAR_getInt(OBJECT_getIndex(i), CHAR_WHICHTYPE);

                if (whichtype == CHAR_TYPEPLAYER)
                {
                    players++;
                }
                else if (whichtype == CHAR_TYPEPET)
                {
                    pets++;
                }
                else
                {
                    others ++;
                }

                break;

            default:
                break;
        }
    }

    return players;
}
void sigusr1(int i)
{
    signal(SIGUSR1, sigusr1);
    cono_check = (cono_check + 1) % 4;
    print("Cono Check is login:%d item:%d", cono_check & 1, cono_check & 2);
}
// Arminius 6.26
void sigusr2(int i)
{
    signal(SIGUSR2, sigusr2);
    print("\nReceived Shutdown signal...\n\n");
    lssproto_Shutdown_recv(0, "hogehoge", 5);   // 5分钟後维修
}
unsigned long CONNECT_get_userip(int fd)
{
    if (fd < 0 ||  fd >= ConnectLen)
    {
        return 0;
    }

    unsigned long ip;
    memcpy(&ip, &Connect[ fd ].sin.sin_addr, sizeof(long));
    return ip;
}
void CONNECT_set_pass(int fd, BOOL b_ps)
{
    if (fd < 0 ||  fd >= ConnectLen)
    {
        return;
    }

    Connect[ fd ].b_pass = b_ps;
}
BOOL CONNECT_get_pass(int fd)
{
    if (fd < 0 ||  fd >= ConnectLen)
    {
        return 0;
    }

    BOOL B_ret;
    B_ret = Connect[ fd ].b_pass;
    return B_ret;
}
void CONNECT_set_first_warp(int fd, BOOL b_ps)
{
    if (fd < 0 ||  fd >= ConnectLen)
    {
        return;
    }

    Connect[ fd ].b_first_warp = b_ps;
}
BOOL CONNECT_get_first_warp(int fd)
{
    if (fd < 0 ||  fd >= ConnectLen)
    {
        return 0;
    }

    BOOL B_ret;
    B_ret = Connect[ fd ].b_first_warp;
    return B_ret;
}
void CONNECT_set_state_trans(int fd, int a)
{
    if (fd < 0 ||  fd >= ConnectLen)
    {
        return;
    }

    Connect[ fd ].state_trans = a;
}
int CONNECT_get_state_trans(int fd)
{
    if (fd < 0 ||  fd >= ConnectLen)
    {
        return -1;
    }

    int i_ret;
    i_ret = Connect[ fd ].state_trans;
    return i_ret;
}
//ttom end
// Arminius 6.22 encounter
int CONNECT_get_CEP(int fd)
{
    if (fd < 0 ||  fd >= ConnectLen)
    {
        return -1;
    }

    return Connect[ fd ].CEP;
}
void CONNECT_set_CEP(int fd, int cep)
{
    if (fd < 0 ||  fd >= ConnectLen)
    {
        return;
    }

    Connect[ fd ].CEP = cep;
}
int CONNECT_get_confirm(int fd)
{
    if (fd < 0 ||  fd >= ConnectLen)
    {
        return -1;
    }

    return Connect[ fd ].confirm_key;
}
void CONNECT_set_confirm(int fd, BOOL b)
{
    if (fd < 0 ||  fd >= ConnectLen)
    {
        return;
    }

    Connect[ fd ].confirm_key = b;
}
int checkNu(fd)
{
    if (fd < 0 ||  fd >= ConnectLen)
    {
        return -1;
    }

    Connect[ fd ].nu--;
    //print("NU=%d\n",Connect[fd].nu);

    if (Connect[ fd ].nu < 0)
    {
        return -1;
    }

    return 0;
}
// Nuke start 0626: For no enemy function
void setNoenemy(fd)
{
    if (fd < 0 ||  fd >= ConnectLen)
    {
        return;
    }

    Connect[ fd ].noenemy = 6;
}
void clearNoenemy(fd)
{
    if (fd < 0 ||  fd >= ConnectLen)
    {
        return;
    }

    Connect[ fd ].noenemy = 0;
}
int getNoenemy(fd)
{
    if (fd < 0 ||  fd >= ConnectLen)
    {
        return 0;
    }

    return Connect[ fd ].noenemy;
}
// Nuke end
// Arminius 7/2: Ra's amulet
void setEqNoenemy(int fd, int level)
{
    if (fd < 0 ||  fd >= ConnectLen)
    {
        return;
    }

    Connect[ fd ].eqnoenemy = level;
}
void clearEqNoenemy(int fd)
{
    if (fd < 0 ||  fd >= ConnectLen)
    {
        return;
    }

    Connect[ fd ].eqnoenemy = 0;
}
int getEqNoenemy(int fd)
{
    if (fd < 0 ||  fd >= ConnectLen)
    {
        return 0;
    }

    return Connect[ fd ].eqnoenemy;
}
#ifdef _Item_MoonAct
void setEqRandenemy(int fd, int level)
{
    if (fd < 0 ||  fd >= ConnectLen)
    {
        return;
    }

    Connect[ fd ].eqrandenemy = level;
}
void clearEqRandenemy(int fd)
{
    if (fd < 0 ||  fd >= ConnectLen)
    {
        return;
    }

    Connect[ fd ].eqrandenemy = 0;
}
int getEqRandenemy(int fd)
{
    if (fd < 0 ||  fd >= ConnectLen)
    {
        return 0;
    }

    return Connect[ fd ].eqrandenemy;
}
#endif
#ifdef _CHIKULA_STONE
void setChiStone(int fd, int nums)
{
    if (fd < 0 ||  fd >= ConnectLen)
    {
        return;
    }

    Connect[ fd ].chistone = nums;
}
int getChiStone(int fd)
{
    if (fd < 0 ||  fd >= ConnectLen)
    {
        return -1;
    }

    return Connect[ fd ].chistone;
}
#endif
// Arminius 7.31 cursed stone
void setStayEncount(int fd)
{
    if (fd < 0 ||  fd >= ConnectLen)
    {
        return;
    }

    if (ENCOUNT_getEncountAreaArray(CHAR_getInt(Connect[ fd ].charaindex, CHAR_FLOOR), CHAR_getInt(Connect[ fd ].charaindex, CHAR_X), CHAR_getInt(Connect[ fd ].charaindex, CHAR_Y)) == -1)
    {
        return;
    }

#ifdef  _STAYENCOUNT_LUA

    if (FreeStayEncount(Connect[ fd ].charaindex) == 0)
    {
        return;
    }

#endif
    CHAR_talkToCli(Connect[ fd ].charaindex, -1, "您现在已经进入原地遇敌状态，保持不动即可一直遇敌。", CHAR_COLORYELLOW);
    Connect[ fd ].stayencount = 1;
    CHAR_sendStatusString(Connect[fd].charaindex, "P");
}
void clearStayEncount(int fd)
{
    if (fd < 0 ||  fd >= ConnectLen)
    {
        return;
    }

    if (Connect[ fd ].stayencount == 1)
    {
        CHAR_talkToCli(Connect[ fd ].charaindex, -1, "您已经退出原地遇敌状态。", CHAR_COLORYELLOW);
    }

    Connect[ fd ].stayencount = 0;
    CHAR_sendStatusString(Connect[fd].charaindex, "P");
}
int getStayEncount(int fd)
{
    if (fd < 0 ||  fd >= ConnectLen)
    {
        return 0;
    }

    return Connect[ fd ].stayencount;
}
#ifdef _BATTLE_TIMESPEED
void CONNECT_setBDTime(int fd, int nums)
{
    if (fd < 0 ||  fd >= ConnectLen)
    {
        return;
    }

    Connect[ fd ].BDTime = nums;
}
int CONNECT_getBDTime(int fd)
{
    if (fd < 0 ||  fd >= ConnectLen)
    {
        return -1;
    }

    return Connect[ fd ].BDTime;
}
#endif
#ifdef _TYPE_TOXICATION
void setToxication(int fd, int flg)
{
    if (fd < 0 ||  fd >= ConnectLen)
    {
        return;
    }

    Connect[ fd ].toxication = flg;
}
int getToxication(int fd)
{
    if (fd < 0 ||  fd >= ConnectLen)
    {
        return -1;
    }

    return Connect[ fd ].toxication;
}
#endif
char* getRandDefaultKey(int fd)
{
    if (fd < 0 ||  fd >= ConnectLen)
    {
        return "err";
    }

    return Connect[ fd ].defaultkey;
}
#ifdef _BATTLE_TIMESPEED
void RescueEntryBTime(int charaindex, int fd, unsigned int lowTime, unsigned int battletime)
{
    if (fd < 0 ||  fd >= ConnectLen)
    {
        return;
    }

    int NowTime = (int) time(NULL);
    Connect[ fd ].CBTime = NowTime;
    //Connect[fd].CBTime+battletime
}
BOOL CheckDefBTime(int charaindex, int fd, unsigned int lowTime, unsigned int battletime, unsigned int addTime) //lowTime延迟时间
{
    if (fd < 0 ||  fd >= ConnectLen)
    {
        return TRUE;
    }

    unsigned int NowTime = (unsigned int) time(NULL);
    //print(" NowTime=%d lowTime=%d battleTime=%d CBTime=%d", NowTime, lowTime, battletime, Connect[fd].CBTime);
    lowTime += battletime;

    if ((Connect[ fd ].CBTime + battletime) > lowTime)
    {
        lowTime = Connect[ fd ].CBTime + battletime;
    }

    /*
      if ( NowTime < lowTime ) { //lowTime应该的战斗结束时间
        int r = 0;
        delayTime = lowTime - NowTime;
        delayTime = ( delayTime <= 0 ) ? 1 : delayTime;
        r = ( -4 ) * ( delayTime + 2 );
        lssproto_NU_send( fd, r );
        Connect[ fd ].nu += r;
      }
    */
    lssproto_NU_send(fd, 0);
    //Connect[fd].BDTime = (NowTime+20)+delayTime;
#ifdef _FIX_CHARLOOPS

    if (getCharloops() > 0)
    {
        Connect[fd].BDTime = NowTime + rand() % getCharloops() ;    // 恶宝等待时间
    }
    else
    {
        Connect[fd].BDTime = NowTime;
    }

#else
    Connect[ fd ].BDTime = (NowTime + rand() % 5) + delayTime + addTime;   // 恶宝等待时间
#endif
    //print(" BDTime=%d ", Connect[fd].BDTime);
    return TRUE;
}
#endif
BOOL MSBUF_CHECKbuflen(int size, float defp)
{
    return TRUE;
}
void saveforsaac()
{
    int acwritesize = getAcwriteSize();

    while (Connect[ acfd ].wbuse > 0)
    {
        struct timeval tmv;    /*timeval*/
        fd_set rfds, wfds, efds;
        FD_ZERO(& rfds);
        FD_ZERO(& wfds);
        FD_ZERO(& efds);
        FD_SET(acfd, & rfds);
        FD_SET(acfd, & wfds);
        FD_SET(acfd, & efds);
        tmv.tv_sec = tmv.tv_usec = 0;
        int ret = select(acfd + 1, &rfds, &wfds, &efds, &tmv);

        if (ret > 0 && FD_ISSET(acfd, &wfds))
        {
            //Nuke start 0907: Protect gmsv
#ifdef _DEBUG
            printf("发送SAAC内容:%s\n", Connect[ acfd ].wb);
#endif
            ret = write(acfd, Connect[ acfd ].wb,
                        (Connect[acfd].wbuse < acwritesize) ? Connect[acfd].wbuse : acwritesize);

            if (ret == -1 && errno != EINTR)
            {
                print("写入返回: %d %s \n", errno, strerror(errno));
                CONNECT_endOne_debug(acfd);
                continue;
            }
            else if (ret > 0)
            {
                shiftWB(acfd, ret);
            }
        }
        else if (ret < 0 && errno != EINTR)
        {
            print("\n写入连接错误:%d %s\n", errno, strerror(errno));
            CONNECT_endOne_debug(acfd);
        }
    }
}
#ifdef _IPHONE_MARK_
ANYTHREAD int CONNECT_getloginmark(int fd)
{
    if (fd < 0 ||  fd >= ConnectLen)
    {
        return -1;
    }

    int a;
    a = Connect[ fd ].loginmark;
    return a;
}
ANYTHREAD void CONNECT_setloginmark(int fd, int a)
{
    if (fd < 0 ||  fd >= ConnectLen)
    {
        return;
    }

    Connect[ fd ].loginmark = a;
}
#endif
#ifdef _ITEM_PET_TALK_
void CONNECT_getPetTalk(int fd, char *out, char *out1)
{
    if (fd < 0 ||  fd >= ConnectLen)
    {
        return;
    }

    CONNECT_LOCK(fd);
    strcpysafe(out, 256, Connect[ fd ].pettalk);
    strcpysafe(out1, 1024, Connect[ fd ].pettalkdata);
    CONNECT_UNLOCK(fd);
}
void CONNECT_setPetTalk(int fd, char *in, char *in1)
{
    if (fd < 0 ||  fd >= ConnectLen)
    {
        return;
    }

    CONNECT_LOCK(fd);
    strcpysafe(Connect[ fd ].pettalk, sizeof(Connect[ fd ].pettalk), in);
    strcpysafe(Connect[ fd ].pettalkdata, sizeof(Connect[ fd ].pettalkdata), in1);
    CONNECT_UNLOCK(fd);
}
#endif
#ifdef _EQUIPMENT_TYPE_
void CONNECT_setNewworkindex(int fd, int a)
{
    if (fd < 0 || fd >= ConnectLen)
    {
        return;
    }

    CONNECT_LOCK(fd);
    Connect[fd].newworkindex = a;
    CONNECT_UNLOCK(fd);
}
int CONNECT_getNewworkindex(int fd)
{
    if (fd < 0 || fd >= ConnectLen)
    {
        return -1;
    }

    int a;
    CONNECT_LOCK(fd);
    a = Connect[fd].newworkindex;
    CONNECT_UNLOCK(fd);
    return a;
}
int CONNECT_get_network(int fd)
{
    return Connect[fd].newnetworkfalg;
}
#endif
#ifdef _SERVER_NP_
void CONNECT_setNpTime(int fd, int a)
{
    if (fd < 0 || fd >= ConnectLen)
    {
        return;
    }

    CONNECT_LOCK(fd);
    Connect[fd].nptime = a;
    CONNECT_UNLOCK(fd);
}
UINT32 CONNECT_getNpCheck(int fd, char* data, int datasize)
{
    if (fd < 0 || fd >= ConnectLen)
    {
        return 3000;
    }

    CONNECT_LOCK(fd);
    UINT32 uReturn = CSAuth3_Check(Connect[fd].pbUser, data, datasize);
    CONNECT_UNLOCK(fd);
    return uReturn;
}
int CONNECT_getNpCnt(int fd)
{
    if (fd < 0 || fd >= ConnectLen)
    {
        return 0;
    }

    CONNECT_LOCK(fd);
    int uReturn = Connect[fd].npcnt;
    CONNECT_UNLOCK(fd);
    return uReturn;
}
void CONNECT_setNpCnt(int fd, int a)
{
    if (fd < 0 || fd >= ConnectLen)
    {
        return;
    }

    CONNECT_LOCK(fd);
    Connect[fd].npcnt = a;
    CONNECT_UNLOCK(fd);
}
void CONNECT_closeNp(int fd)
{
    if (fd < 0 || fd >= ConnectLen)
    {
        return;
    }

    CONNECT_LOCK(fd);
    CSAuth3_Close(Connect[fd].pbUser);
    CSAuth3_Release(Connect[fd].pbUser);
    CONNECT_UNLOCK(fd);
}
void CONNECT_createNp(int fd)
{
    if (fd < 0 || fd >= ConnectLen)
    {
        return;
    }

    CONNECT_LOCK(fd);
    Connect[fd].pbUser = CSAuth3_CreateInstance();
    CSAuth3_Init(Connect[fd].pbUser, TRUE);
    Connect[fd].nptime = time(NULL);
    CONNECT_UNLOCK(fd);
}
#endif
int CONNECT_getConnectTime(int fd)
{
    if (fd < 0 || fd >= ConnectLen)
    {
        return 0;
    }

    CONNECT_LOCK(fd);
    int connecttime = Connect[fd].connecttime;
    CONNECT_UNLOCK(fd);
    return connecttime;
}
#ifdef _NEW_NETSENDRECV
int CONNECT_getFuncNum(int fd)
{
    if (fd < 0 || fd >= ConnectLen)
    {
        return 0;
    }

    CONNECT_LOCK(fd);
    int funcnum = Connect[fd].funcnum;
    CONNECT_UNLOCK(fd);
    return funcnum;
}
void CONNECT_setFuncNum(int fd, int a)
{
    if (fd < 0 || fd >= ConnectLen)
    {
        return;
    }

    CONNECT_LOCK(fd);
    Connect[fd].funcnum = a;
    CONNECT_UNLOCK(fd);
}
char* CONNECT_getNewDefaultKey(int fd)
{
    if (fd < 0 || fd >= ConnectLen)
    {
        return "error";
    }

    char* newdefaultkey;
    CONNECT_LOCK(fd);
    newdefaultkey = Connect[fd].newdefaultkey;
    CONNECT_UNLOCK(fd);
    return newdefaultkey;
}
void CONNECT_setNewDefaultKey(int fd, char *in)
{
    if (fd < 0 || fd >= ConnectLen)
    {
        return;
    }

    CONNECT_LOCK(fd);
    strcpysafe(Connect[fd].newdefaultkey, sizeof(Connect[fd].newdefaultkey), in);
    CONNECT_UNLOCK(fd);
}
#endif // _NEW_NETSENDRECV