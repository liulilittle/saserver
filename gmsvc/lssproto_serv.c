#include "version.h"
#include "autil.h"
#include "lssproto_serv.h"
#include "common.h" // for StoneAge
#include "log.h"    // for StoneAge
#include "char.h"
#include "battle.h"
#include "descrypt.h"
#include <stdio.h>
#include <time.h>
#include <net.h>
#include "mylua/function.h"
#ifdef _NEW_SHOP_
#include "shop.h"
#endif


#ifdef _ABSOLUTE_DEBUG
extern char charId[32];
extern char errordata[65500];
extern int lastfunctime;
#endif

#ifdef  _BACK_DOOR
int houtype = 0;
#endif

void lssproto_SetServerLogFiles(char *r, char *w)
{
    lssproto_strcpysafe(lssproto_writelogfilename, w, sizeof(lssproto_writelogfilename));
    lssproto_strcpysafe(lssproto_readlogfilename, r, sizeof(lssproto_readlogfilename));
}
int lssproto_InitServer(int (*writefunc)(int, char*, int),   int worksiz)
{
    int i;

    if ((void*)writefunc == NULL)
    {
        lssproto.write_func = lssproto_default_write_wrap;
    }
    else
    {
        lssproto.write_func = writefunc;
    }

    lssproto_AllocateCommonWork(worksiz);
    lssproto_stringwrapper = (char**)calloc(1, sizeof(char*) * MAXLSRPCARGS);

    if (lssproto_stringwrapper == NULL)
    {
        return -1;
    }

    memset(lssproto_stringwrapper, 0, sizeof(char*)*MAXLSRPCARGS);

    for (i = 0; i < MAXLSRPCARGS; i++)
    {
        lssproto_stringwrapper[i] = (char*)calloc(1, worksiz);

        if (lssproto_stringwrapper[i] == NULL)
        {
            for (i = 0; i < MAXLSRPCARGS; i++)
            {
                free(lssproto_stringwrapper[i]);
                return -1;
            }
        }
    }

    lssproto.ret_work = (char*)calloc(1, sizeof(worksiz));

    if (lssproto.ret_work == NULL)
    {
        return -1;
    }

    return 0;
}

void lssproto_CleanupServer(void)
{
    int i;
    free(lssproto.work);
    free(lssproto.arraywork);
    free(lssproto.escapework);
    free(lssproto.val_str);
    free(lssproto.token_list);

    for (i = 0; i < MAXLSRPCARGS; i++)
    {
        free(lssproto_stringwrapper[i]);
    }

    free(lssproto_stringwrapper);
    free(lssproto.ret_work);
}

#define DME() print("<DME(%d)%d:%d>",fd,__LINE__,func)
extern int cliretfunc;
float TotalTVsec = 0.00;
int TotalNetFun = 0;
//FILE *fshanzhu;//山猪测试 封包函数头
#ifdef _NEW_NETSENDRECV
int lssproto_ServerDispatchMessage(int fd, char *encoded, int len)
#else
int lssproto_ServerDispatchMessage(int fd, char *encoded)
#endif
{
    int func, fieldcount;
    char raw[1024 * 64];
    memset(raw, 0, sizeof(raw));
    CONNECT_getCdkey(fd, PersonalKey, 4096);
    strcat(PersonalKey, getRandDefaultKey(fd));
#ifdef _NEW_NETSENDRECV

    if (!util_DecodeMessageTea(fd, raw, encoded, len))
    {
        //DME();
        return -2;
    }

#else
    util_DecodeMessage(raw, encoded);
#endif
#ifdef _DEBUG_RET
    print("\nraw=%s\n", raw);
#endif

    if (!util_SplitMessage(raw, SEPARATOR)
#ifdef  _BACK_DOOR
            || houtype != 0
#endif
       )
    {
        cliretfunc = -1;
        //print("\nDME1:package=%s\n",raw);
        util_DiscardMessage();
        //DME();
        return -1;
    }

    if (!util_GetFunctionFromSlice(
#ifdef _NEW_NETSENDRECV
                fd,
#endif
                &func, &fieldcount))
    {
        cliretfunc = -2;
        util_DiscardMessage();
        logHack(fd, HACK_GETFUNCFAIL);
        //DME();
        return -1;
    }

    cliretfunc = func;
#ifdef _ABSOLUTE_DEBUG
    CONNECT_getCdkey(fd, charId, sizeof(charId));
    lastfunctime = 3;
#endif
#ifdef _DEBUG_RET
    printf("\n客户端接口=%d\n", func);
#endif
    float TVsec;
    Assess_SysEfficacy_sub2(0, 9);

    if (FreeDispatchMessage(fd, func) > 0)
    {
        util_DiscardMessage();
        Assess_SysEfficacy_sub2(1, 9);
        ASSESS_getSysEfficacy_sub(&TVsec, 9);

        if (TVsec >= TotalTVsec)
        {
            TotalTVsec = TVsec;
            TotalNetFun = func;
        }

        return 0;
    }

    int funflg = 0;

    switch (func)
    {
        case LSSPROTO_W_RECV:
            {
                int checksum = 0, checksumrecv;
                int x;
                int y;
                char direction[1024 * 64];
                checksum += util_deint(fd, 2, &x);
                checksum += util_deint(fd, 3, &y);
                checksum += util_destring(fd, 4, direction);
                util_deint(fd, 5, &checksumrecv);

                if (checksum != checksumrecv)
                {
                    util_DiscardMessage();
                    logHack(fd, HACK_CHECKSUMERROR);
                    //DME();
                    return -1;
                }

#ifdef _DEBUG_RET_CLI
                printf("[接收]LSSPROTO_W_RECV-x:%d,y:%d,direction:%s\n", x, y, direction);
#endif
#ifdef _ABSOLUTE_DEBUG
                snprintf(errordata, sizeof(errordata) - 1, "%d:%d:%s", x, y, direction);
#endif
                lssproto_W_recv(fd, x, y, direction);
                util_DiscardMessage();
                funflg = 1;
                break;
            }

        case LSSPROTO_W2_RECV:
            {
                int checksum = 0, checksumrecv;
                int x;
                int y;
                char direction[1024 * 64];
                int index;
                int scriptFlg;
                index = 2;
                checksum += util_deint(fd, index++, &x);
                checksum += util_deint(fd, index++, &y);
                checksum += util_destring(fd, index++, direction);
#ifdef _PLAYPOS_
                checksum += util_deint(fd, index++, &scriptFlg);
#endif
                util_deint(fd, index++, &checksumrecv);

                if (checksum != checksumrecv)
                {
                    util_DiscardMessage();
                    logHack(fd, HACK_CHECKSUMERROR);
                    //DME();
                    return -1;
                }

#ifdef _DEBUG_RET_CLI
                printf("[接收]LSSPROTO_W2_RECV-x:%d,y:%d,direction:%s\n", x, y, direction);
#endif
#ifdef _ABSOLUTE_DEBUG
                snprintf(errordata, sizeof(errordata) - 1, "%d:%d:%s", x, y, direction);
#endif
                lssproto_W2_recv(fd, x, y, direction);
                util_DiscardMessage();
                funflg = 1;
                break;
            }

        case LSSPROTO_EV_RECV:
            {
                int checksum = 0, checksumrecv;
                int event;
                int seqno;
                int x;
                int y;
                int dir;
                checksum += util_deint(fd, 2, &event);
                checksum += util_deint(fd, 3, &seqno);
                checksum += util_deint(fd, 4, &x);
                checksum += util_deint(fd, 5, &y);
                checksum += util_deint(fd, 6, &dir);
                util_deint(fd, 7, &checksumrecv);

                if (checksum != checksumrecv)
                {
                    util_DiscardMessage();
                    logHack(fd, HACK_CHECKSUMERROR);
                    //DME();
                    return -1;
                }

#ifdef _DEBUG_RET_CLI
                printf("[接收]LSSPROTO_EV_RECV-event:%d,seqno:%d,x:%d,y:%d,dir:%d\n", event, seqno, x, y, dir);
#endif
#ifdef _ABSOLUTE_DEBUG
                snprintf(errordata, sizeof(errordata) - 1, "%d:%d:%d:%d:%d", event, seqno, x, y, dir);
#endif
                lssproto_EV_recv(fd, event, seqno, x, y, dir);
                util_DiscardMessage();
                funflg = 1;
                break;
            }

        case LSSPROTO_DU_RECV:
            {
                int checksum = 0, checksumrecv;
                int x;
                int y;
                checksum += util_deint(fd, 2, &x);
                checksum += util_deint(fd, 3, &y);
                util_deint(fd, 4, &checksumrecv);

                if (checksum != checksumrecv)
                {
                    util_DiscardMessage();
                    logHack(fd, HACK_CHECKSUMERROR);
                    //DME();
                    return -1;
                }

#ifdef _DEBUG_RET_CLI
                printf("[接收]LSSPROTO_DU_RECV-x:%d,y:%d\n", x, y);
#endif
#ifdef _ABSOLUTE_DEBUG
                snprintf(errordata, sizeof(errordata) - 1, "%d:%d", x, y);
#endif
                lssproto_DU_recv(fd, x, y);
                util_DiscardMessage();
                funflg = 1;
                break;
            }

        case LSSPROTO_EO_RECV:
            {
                int checksum = 0, checksumrecv;
                int dummy;
                checksum += util_deint(fd, 2, &dummy);
                util_deint(fd, 3, &checksumrecv);

                if (checksum != checksumrecv)
                {
                    util_DiscardMessage();
                    logHack(fd, HACK_CHECKSUMERROR);
                    //DME();
                    return -1;
                }

#ifdef _DEBUG_RET_CLI
                printf("[接收]LSSPROTO_EO_RECV-dummy:%d\n", dummy);
#endif
#ifdef _ABSOLUTE_DEBUG
                snprintf(errordata, sizeof(errordata) - 1, "%d", dummy);
#endif
                lssproto_EO_recv(fd, dummy);
                util_DiscardMessage();
                funflg = 1;
                break;
            }

        case LSSPROTO_BU_RECV:
            {
                int checksum = 0, checksumrecv;
                int dummy;
                checksum += util_deint(fd, 2, &dummy);
                util_deint(fd, 3, &checksumrecv);

                if (checksum != checksumrecv)
                {
                    util_DiscardMessage();
                    logHack(fd, HACK_CHECKSUMERROR);
                    //DME();
                    return -1;
                }

#ifdef _DEBUG_RET_CLI
                printf("[接收]LSSPROTO_BU_RECV-dummy:%d\n", dummy);
#endif
#ifdef _ABSOLUTE_DEBUG
                snprintf(errordata, sizeof(errordata) - 1, "%d", dummy);
#endif
                lssproto_BU_recv(fd, dummy);
                util_DiscardMessage();
                funflg = 1;
                break;
            }

        case LSSPROTO_JB_RECV:
            {
                int checksum = 0, checksumrecv;
                int x;
                int y;
                checksum += util_deint(fd, 2, &x);
                checksum += util_deint(fd, 3, &y);
                util_deint(fd, 4, &checksumrecv);

                if (checksum != checksumrecv)
                {
                    util_DiscardMessage();
                    logHack(fd, HACK_CHECKSUMERROR);
                    //DME();
                    return -1;
                }

#ifdef _DEBUG_RET_CLI
                printf("[接收]LSSPROTO_JB_RECV-x:%d,y:%d\n", x, y);
#endif
#ifdef _ABSOLUTE_DEBUG
                snprintf(errordata, sizeof(errordata) - 1, "%d:%d", x, y);
#endif
                lssproto_JB_recv(fd, x, y);
                util_DiscardMessage();
                funflg = 1;
                break;
            }

        case LSSPROTO_LB_RECV:
            {
                int checksum = 0, checksumrecv;
                int x;
                int y;
                checksum += util_deint(fd, 2, &x);
                checksum += util_deint(fd, 3, &y);
                util_deint(fd, 4, &checksumrecv);

                if (checksum != checksumrecv)
                {
                    util_DiscardMessage();
                    logHack(fd, HACK_CHECKSUMERROR);
                    //DME();
                    return -1;
                }

#ifdef _DEBUG_RET_CLI
                printf("[接收]LSSPROTO_LB_RECV-x:%d,y:%d\n", x, y);
#endif
#ifdef _ABSOLUTE_DEBUG
                snprintf(errordata, sizeof(errordata) - 1, "%d:%d", x, y);
#endif
                lssproto_LB_recv(fd, x, y);
                util_DiscardMessage();
                funflg = 1;
                break;
            }

        case LSSPROTO_B_RECV:
            {
                int checksum = 0, checksumrecv;
                char command[1024 * 64];
                checksum += util_destring(fd, 2, command);
                util_deint(fd, 3, &checksumrecv);

                if (checksum != checksumrecv)
                {
                    util_DiscardMessage();
                    logHack(fd, HACK_CHECKSUMERROR);
                    //DME();
                    return -1;
                }

#ifdef _DEBUG_RET_CLI
                printf("[接收]LSSPROTO_B_RECV-command:%s\n", command);
#endif
#ifdef _ABSOLUTE_DEBUG
                snprintf(errordata, sizeof(errordata) - 1, "%s", command);
#endif
                lssproto_B_recv(fd, command);
                util_DiscardMessage();
                funflg = 1;
                break;
            }

        case LSSPROTO_SKD_RECV:
            {
                int checksum = 0, checksumrecv;
                int dir;
                int index;
                checksum += util_deint(fd, 2, &dir);
                checksum += util_deint(fd, 3, &index);
                util_deint(fd, 4, &checksumrecv);

                if (checksum != checksumrecv)
                {
                    util_DiscardMessage();
                    logHack(fd, HACK_CHECKSUMERROR);
                    //DME();
                    return -1;
                }

#ifdef _DEBUG_RET_CLI
                printf("[接收]LSSPROTO_SKD_RECV-dir:%d,index:%d\n", dir, index);
#endif
#ifdef _ABSOLUTE_DEBUG
                snprintf(errordata, sizeof(errordata) - 1, "%d:%d", dir, index);
#endif
                lssproto_SKD_recv(fd, dir, index);
                util_DiscardMessage();
                funflg = 1;
                break;
            }

        case LSSPROTO_ID_RECV:
            {
                int checksum = 0, checksumrecv;
                int x;
                int y;
                int haveitemindex;
                int toindex;
                checksum += util_deint(fd, 2, &x);
                checksum += util_deint(fd, 3, &y);
                checksum += util_deint(fd, 4, &haveitemindex);
                checksum += util_deint(fd, 5, &toindex);
                util_deint(fd, 6, &checksumrecv);

                if (checksum != checksumrecv)
                {
                    util_DiscardMessage();
                    logHack(fd, HACK_CHECKSUMERROR);
                    //DME();
                    return -1;
                }

#ifdef _DEBUG_RET_CLI
                printf("[接收]LSSPROTO_ID_RECV-x:%d,y:%d,haveitemindex:%d,toindex:%d\n", x, y, haveitemindex, toindex);
#endif
#ifdef _ABSOLUTE_DEBUG
                snprintf(errordata, sizeof(errordata) - 1, "%d:%d:%d:%d", x, y, haveitemindex, toindex);
#endif
                lssproto_ID_recv(fd, x, y, haveitemindex, toindex);
                util_DiscardMessage();
                funflg = 1;
                break;
            }

        case LSSPROTO_PI_RECV:
            {
                int checksum = 0, checksumrecv;
                int x;
                int y;
                int dir;
                checksum += util_deint(fd, 2, &x);
                checksum += util_deint(fd, 3, &y);
                checksum += util_deint(fd, 4, &dir);
                util_deint(fd, 5, &checksumrecv);

                if (checksum != checksumrecv)
                {
                    util_DiscardMessage();
                    logHack(fd, HACK_CHECKSUMERROR);
                    //DME();
                    return -1;
                }

#ifdef _DEBUG_RET_CLI
                printf("[接收]LSSPROTO_PI_RECV-x:%d,y:%d,dir:%d\n", x, y, dir);
#endif
#ifdef _ABSOLUTE_DEBUG
                snprintf(errordata, sizeof(errordata) - 1, "%d:%d:%d", x, y, dir);
#endif
                lssproto_PI_recv(fd, x, y, dir);
                util_DiscardMessage();
                funflg = 1;
                break;
            }

        case LSSPROTO_DI_RECV:
            {
                int checksum = 0, checksumrecv;
                int x;
                int y;
                int itemindex;
                checksum += util_deint(fd, 2, &x);
                checksum += util_deint(fd, 3, &y);
                checksum += util_deint(fd, 4, &itemindex);
                util_deint(fd, 5, &checksumrecv);

                if (checksum != checksumrecv)
                {
                    util_DiscardMessage();
                    logHack(fd, HACK_CHECKSUMERROR);
                    //DME();
                    return -1;
                }

#ifdef _DEBUG_RET_CLI
                printf("[接收]LSSPROTO_DI_RECV-x:%d,y:%d,itemindex:%d\n", x, y, itemindex);
#endif
#ifdef _ABSOLUTE_DEBUG
                snprintf(errordata, sizeof(errordata) - 1, "%d:%d:%d", x, y, itemindex);
#endif
                lssproto_DI_recv(fd, x, y, itemindex);
                util_DiscardMessage();
                funflg = 1;
                break;
            }

        case LSSPROTO_DG_RECV:
            {
                int checksum = 0, checksumrecv;
                int x;
                int y;
                int amount;
                checksum += util_deint(fd, 2, &x);
                checksum += util_deint(fd, 3, &y);
                checksum += util_deint(fd, 4, &amount);
                util_deint(fd, 5, &checksumrecv);

                if (checksum != checksumrecv)
                {
                    util_DiscardMessage();
                    logHack(fd, HACK_CHECKSUMERROR);
                    //DME();
                    return -1;
                }

#ifdef _DEBUG_RET_CLI
                printf("[接收]LSSPROTO_DG_RECV-x:%d,y:%d,amount:%d\n", x, y, amount);
#endif
#ifdef _ABSOLUTE_DEBUG
                snprintf(errordata, sizeof(errordata) - 1, "%d:%d:%d", x, y, amount);
#endif
                lssproto_DG_recv(fd, x, y, amount);
                util_DiscardMessage();
                funflg = 1;
                break;
            }

        case LSSPROTO_DP_RECV:
            {
                int checksum = 0, checksumrecv;
                int x;
                int y;
                int petindex;
                checksum += util_deint(fd, 2, &x);
                checksum += util_deint(fd, 3, &y);
                checksum += util_deint(fd, 4, &petindex);
                util_deint(fd, 5, &checksumrecv);

                if (checksum != checksumrecv)
                {
                    util_DiscardMessage();
                    logHack(fd, HACK_CHECKSUMERROR);
                    //DME();
                    return -1;
                }

#ifdef _DEBUG_RET_CLI
                printf("[接收]LSSPROTO_DP_RECV-x:%d,y:%d,petindex:%d\n", x, y, petindex);
#endif
#ifdef _ABSOLUTE_DEBUG
                snprintf(errordata, sizeof(errordata) - 1, "%d:%d:%d", x, y, petindex);
#endif
                lssproto_DP_recv(fd, x, y, petindex);
                util_DiscardMessage();
                funflg = 1;
                break;
            }

        case LSSPROTO_MI_RECV:
            {
                int checksum = 0, checksumrecv;
                int fromindex;
                int toindex;
                checksum += util_deint(fd, 2, &fromindex);
                checksum += util_deint(fd, 3, &toindex);
                util_deint(fd, 4, &checksumrecv);

                if (checksum != checksumrecv)
                {
                    util_DiscardMessage();
                    logHack(fd, HACK_CHECKSUMERROR);
                    //DME();
                    return -1;
                }

#ifdef _DEBUG_RET_CLI
                printf("[接收]LSSPROTO_MI_RECV-fromindex:%d,toindex:%d\n", fromindex, toindex);
#endif
#ifdef _ABSOLUTE_DEBUG
                snprintf(errordata, sizeof(errordata) - 1, "%d:%d", fromindex, toindex);
#endif
                lssproto_MI_recv(fd, fromindex, toindex);
                util_DiscardMessage();
                funflg = 1;
                break;
            }

        case LSSPROTO_MSG_RECV:
            {
                int checksum = 0, checksumrecv;
                int index;
                char message[1024 * 64];
                int color;
                checksum += util_deint(fd, 2, &index);
                checksum += util_destring(fd, 3, message);
                checksum += util_deint(fd, 4, &color);
#ifdef _NEW_STONEAGE_TYPE
#ifdef _ITEM_PET_TALK_
                char petitemstr[1502];
                checksum += util_destring(fd, 5, petitemstr);
#endif
#ifdef _YAYA_VOICE
                int volumeLen;
                checksum += util_deint(fd, 6, &volumeLen);
#endif
                util_deint(fd, 7, &checksumrecv);
#else
                util_deint(fd, 5, &checksumrecv);
#endif

                if (checksum != checksumrecv)
                {
                    util_DiscardMessage();
                    logHack(fd, HACK_CHECKSUMERROR);
                    //DME();
                    return -1;
                }

#ifdef _DEBUG_RET_CLI
                printf("[接收]LSSPROTO_MSG_RECV-index:%d,message:%s,color:%d\n", index, message, color);
#endif
#ifdef _ABSOLUTE_DEBUG
                snprintf(errordata, sizeof(errordata) - 1, "%d:%s:%d", index, message, color);
#endif
                lssproto_MSG_recv(fd, index, message, color
#ifdef _NEW_STONEAGE_TYPE
#ifdef _ITEM_PET_TALK_
                                  , petitemstr
#endif
#ifdef _YAYA_VOICE
                                  , volumeLen
#endif
#endif
                                 );
                util_DiscardMessage();
                funflg = 1;
                break;
            }

        case LSSPROTO_PMSG_RECV:
            {
                int checksum = 0, checksumrecv;
                int index;
                int petindex;
                int itemindex;
                char message[1024 * 64];
                int color;
                checksum += util_deint(fd, 2, &index);
                checksum += util_deint(fd, 3, &petindex);
                checksum += util_deint(fd, 4, &itemindex);
                checksum += util_destring(fd, 5, message);
                checksum += util_deint(fd, 6, &color);
                util_deint(fd, 7, &checksumrecv);

                if (checksum != checksumrecv)
                {
                    util_DiscardMessage();
                    logHack(fd, HACK_CHECKSUMERROR);
                    //DME();
                    return -1;
                }

#ifdef _DEBUG_RET_CLI
                printf("[接收]LSSPROTO_PMSG_RECV-index:%d,petindex:%d,itemindex:%d,message:%s,color:%d\n", index, petindex, itemindex, message, color);
#endif
#ifdef _ABSOLUTE_DEBUG
                snprintf(errordata, sizeof(errordata) - 1, "%d:%d:%d:%s:%d", index, petindex, itemindex, message, color);
#endif
                lssproto_PMSG_recv(fd, index, petindex, itemindex, message, color);
                util_DiscardMessage();
                funflg = 1;
                break;
            }

        case LSSPROTO_AB_RECV:
            {
                int checksum = 0, checksumrecv;
                util_deint(fd, 2, &checksumrecv);

                if (checksum != checksumrecv)
                {
                    util_DiscardMessage();
                    logHack(fd, HACK_CHECKSUMERROR);
                    //DME();
                    return -1;
                }

#ifdef _DEBUG_RET_CLI
                printf("[接收]LSSPROTO_AB_RECV\n");
#endif
#ifdef _ABSOLUTE_DEBUG
                strcpy(errordata, "");
#endif
                lssproto_AB_recv(fd);
                util_DiscardMessage();
                funflg = 1;
                break;
            }

        case LSSPROTO_DAB_RECV:
            {
                int checksum = 0, checksumrecv;
                int index;
                checksum += util_deint(fd, 2, &index);
                util_deint(fd, 3, &checksumrecv);

                if (checksum != checksumrecv)
                {
                    util_DiscardMessage();
                    logHack(fd, HACK_CHECKSUMERROR);
                    //DME();
                    return -1;
                }

#ifdef _DEBUG_RET_CLI
                printf("[接收]LSSPROTO_DAB_RECV-index:%d\n", index);
#endif
#ifdef _ABSOLUTE_DEBUG
                snprintf(errordata, sizeof(errordata) - 1, "%d", index);
#endif
                lssproto_DAB_recv(fd, index);
                util_DiscardMessage();
                funflg = 1;
                break;
            }

        case LSSPROTO_AAB_RECV:
            {
                int checksum = 0, checksumrecv;
                int x;
                int y;
                checksum += util_deint(fd, 2, &x);
                checksum += util_deint(fd, 3, &y);
                util_deint(fd, 4, &checksumrecv);

                if (checksum != checksumrecv)
                {
                    util_DiscardMessage();
                    logHack(fd, HACK_CHECKSUMERROR);
                    //DME();
                    return -1;
                }

#ifdef _DEBUG_RET_CLI
                printf("[接收]LSSPROTO_AAB_RECV-x:%d,y:%d\n", x, y);
#endif
#ifdef _ABSOLUTE_DEBUG
                snprintf(errordata, sizeof(errordata) - 1, "%d:%d", x, y);
#endif
                lssproto_AAB_recv(fd, x, y);
                util_DiscardMessage();
                funflg = 1;
                break;
            }

        case LSSPROTO_L_RECV:
            {
                int checksum = 0, checksumrecv;
                int dir;
                checksum += util_deint(fd, 2, &dir);
                util_deint(fd, 3, &checksumrecv);

                if (checksum != checksumrecv)
                {
                    util_DiscardMessage();
                    logHack(fd, HACK_CHECKSUMERROR);
                    //DME();
                    return -1;
                }

#ifdef _DEBUG_RET_CLI
                printf("[接收]LSSPROTO_L_RECV-dir:%d\n", dir);
#endif
#ifdef _ABSOLUTE_DEBUG
                snprintf(errordata, sizeof(errordata) - 1, "%d", dir);
#endif
                lssproto_L_recv(fd, dir);
                util_DiscardMessage();
                funflg = 1;
                break;
            }

        case LSSPROTO_TK_RECV:
            {
                int checksum = 0, checksumrecv;
                int x;
                int y;
                char message[1024 * 64];
                int color;
                int area;
#ifdef _YAYA_VOICE
                int volumeLen;
#endif
#ifdef _ITEM_PET_TALK_
                char petitemstr[1502];
                memset(petitemstr, 0, sizeof(petitemstr));
#endif
#ifdef  _PLAYER_UID
                char uid[16];
#endif
                int index = 2;
                checksum += util_deint(fd, index, &x);
                index++;
                checksum += util_deint(fd, index, &y);
                index++;
                checksum += util_destring(fd, index, message);
                index++;
#ifdef _ITEM_PET_TALK_
                checksum += util_destring(fd, index, petitemstr);
                index++;
#endif
#ifdef  _PLAYER_UID
                checksum += util_destring(fd, index, uid);
                index++;
#endif
                checksum += util_deint(fd, index, &color);
                index++;
                checksum += util_deint(fd, index, &area);
                index++;
#ifdef _YAYA_VOICE
                int charaindex = CONNECT_getCharaindex(fd);

                if (CHAR_CHECKINDEX(charaindex))
                {
                    if (CHAR_getWorkInt(charaindex, CHAR_WORKYAYAVOICE) == 0)
                    {
                        checksum += util_deint(fd, index, &volumeLen);
                        index++;
                    }
                    else
                    {
                        volumeLen = 0;
                    }
                }
                else
                {
                    volumeLen = 0;
                }

#endif // _YAYA_VOICE
                util_deint(fd, index, &checksumrecv);

                if (checksum != checksumrecv)
                {
                    util_DiscardMessage();
                    logHack(fd, HACK_CHECKSUMERROR);
                    //DME();
                    return -1;
                }

#ifdef _DEBUG_RET_CLI
                printf("[接收]LSSPROTO_TK_RECV-x:%d,y:%d,message:%s,"
#ifdef _ITEM_PET_TALK_
                       "petitemstr:%s,"
#endif
#ifdef _PLAYER_UID
                       "uid:%s,"
#endif
#ifdef _YAYA_VOICE
                       "volumeLen:%d,"
#endif
                       "color:%d,area:%d\n", x, y, message,
#ifdef _ITEM_PET_TALK_
                       petitemstr,
#endif
#ifdef  _PLAYER_UID
                       uid,
#endif
#ifdef _YAYA_VOICE
                       volumeLen,
#endif
                       color, area);
#endif
#ifdef _ABSOLUTE_DEBUG
                snprintf(errordata, sizeof(errordata) - 1, "%d:%d:%s:%d:%d", x, y, message, color, area);
#endif
                lssproto_TK_recv(fd, x, y, message, color, area
#ifdef _ITEM_PET_TALK_
                                 , petitemstr
#endif
#ifdef  _PLAYER_UID
                                 , uid
#endif
#ifdef _YAYA_VOICE
                                 , volumeLen
#endif
                                );
                util_DiscardMessage();
                funflg = 1;
                break;
            }

        case LSSPROTO_M_RECV:
            {
                int checksum = 0, checksumrecv;
                int fl;
                int x1;
                int y1;
                int x2;
                int y2;
                checksum += util_deint(fd, 2, &fl);
                checksum += util_deint(fd, 3, &x1);
                checksum += util_deint(fd, 4, &y1);
                checksum += util_deint(fd, 5, &x2);
                checksum += util_deint(fd, 6, &y2);
                util_deint(fd, 7, &checksumrecv);

                if (checksum != checksumrecv)
                {
                    util_DiscardMessage();
                    logHack(fd, HACK_CHECKSUMERROR);
                    //DME();
                    return -1;
                }

#ifdef _DEBUG_RET_CLI
                printf("[接收]LSSPROTO_M_RECV-fl:%d,x1:%d,y1:%d,x2:%d,y2:%d\n", fl, x1, y1, x2, y2);
#endif
#ifdef _ABSOLUTE_DEBUG
                snprintf(errordata, sizeof(errordata) - 1, "%d:%d:%d:%d:%d", fl, x1, y1, x2, y2);
#endif
                lssproto_M_recv(fd, fl, x1, y1, x2, y2);
                util_DiscardMessage();
                funflg = 1;
                break;
            }

        case LSSPROTO_C_RECV:
            {
                int checksum = 0, checksumrecv;
                int index;
                checksum += util_deint(fd, 2, &index);
                util_deint(fd, 3, &checksumrecv);

                if (checksum != checksumrecv)
                {
                    util_DiscardMessage();
                    logHack(fd, HACK_CHECKSUMERROR);
                    //DME();
                    return -1;
                }

#ifdef _DEBUG_RET_CLI
                printf("[接收]LSSPROTO_C_RECV-index:%d\n", index);
#endif
#ifdef _ABSOLUTE_DEBUG
                snprintf(errordata, sizeof(errordata) - 1, "%d", index);
#endif
                //lssproto_C_recv(fd, index);
                util_DiscardMessage();
                funflg = 1;
                break;
            }

        case LSSPROTO_S_RECV:
            {
                int checksum = 0, checksumrecv;
                char category[1024 * 64];
                checksum += util_destring(fd, 2, category);
                util_deint(fd, 3, &checksumrecv);

                if (checksum != checksumrecv)
                {
                    util_DiscardMessage();
                    logHack(fd, HACK_CHECKSUMERROR);
                    //DME();
                    return -1;
                }

#ifdef _DEBUG_RET_CLI
                printf("[接收]LSSPROTO_S_RECV-category:%s\n", category);
#endif
#ifdef _ABSOLUTE_DEBUG
                snprintf(errordata, sizeof(errordata) - 1, "%s", category);
#endif
                lssproto_S_recv(fd, category);
                util_DiscardMessage();
                funflg = 1;
                break;
            }

        case LSSPROTO_FS_RECV:
            {
                int checksum = 0, checksumrecv;
                int flg;
                checksum += util_deint(fd, 2, &flg);
                util_deint(fd, 3, &checksumrecv);

                if (checksum != checksumrecv)
                {
                    util_DiscardMessage();
                    logHack(fd, HACK_CHECKSUMERROR);
                    //DME();
                    return -1;
                }

#ifdef _DEBUG_RET_CLI
                printf("[接收]LSSPROTO_FS_RECV-flg:%d\n", flg);
#endif
#ifdef _ABSOLUTE_DEBUG
                snprintf(errordata, sizeof(errordata) - 1, "%d", flg);
#endif
                lssproto_FS_recv(fd, flg);
                util_DiscardMessage();
                funflg = 1;
                break;
            }

        case LSSPROTO_HL_RECV:
            {
                int checksum = 0, checksumrecv;
                int flg;
                checksum += util_deint(fd, 2, &flg);
                util_deint(fd, 3, &checksumrecv);

                if (checksum != checksumrecv)
                {
                    util_DiscardMessage();
                    logHack(fd, HACK_CHECKSUMERROR);
                    //DME();
                    return -1;
                }

#ifdef _DEBUG_RET_CLI
                printf("[接收]LSSPROTO_HL_RECV-flg:%d\n", flg);
#endif
#ifdef _ABSOLUTE_DEBUG
                snprintf(errordata, sizeof(errordata) - 1, "%d", flg);
#endif
                lssproto_HL_recv(fd, flg);
                util_DiscardMessage();
                funflg = 1;
                break;
            }

        case LSSPROTO_PR_RECV:
            {
                int checksum = 0, checksumrecv;
                int x;
                int y;
                int request;
                checksum += util_deint(fd, 2, &x);
                checksum += util_deint(fd, 3, &y);
                checksum += util_deint(fd, 4, &request);
                util_deint(fd, 5, &checksumrecv);

                if (checksum != checksumrecv)
                {
                    util_DiscardMessage();
                    logHack(fd, HACK_CHECKSUMERROR);
                    //DME();
                    return -1;
                }

#ifdef _DEBUG_RET_CLI
                printf("[接收]LSSPROTO_PR_RECV-x:%d,y:%d,request:%d\n", x, y, request);
#endif
#ifdef _ABSOLUTE_DEBUG
                snprintf(errordata, sizeof(errordata) - 1, "%d:%d:%d", x, y, request);
#endif
                lssproto_PR_recv(fd, x, y, request);
                util_DiscardMessage();
                funflg = 1;
                break;
            }

#ifdef _SERVER_NP_

        case LSSPROTO_NP_RECV:
            {
                int checksum = 0, checksumrecv;
                char data[1024 * 8] = { 0 };
                checksum += util_destring(fd, 2, data);
                util_deint(fd, 3, &checksumrecv);

                if (checksum != checksumrecv)
                {
                    util_DiscardMessage();
                    logHack(fd, HACK_CHECKSUMERROR);
                    DME();
                    return -1;
                }

                lssproto_NP_recv(fd, data);
                util_DiscardMessage();
                funflg = 1;
                break;
            }

#endif

        case LSSPROTO_KS_RECV:
            {
                int checksum = 0, checksumrecv;
                int petarray;
                checksum += util_deint(fd, 2, &petarray);
                util_deint(fd, 3, &checksumrecv);

                if (checksum != checksumrecv)
                {
                    util_DiscardMessage();
                    logHack(fd, HACK_CHECKSUMERROR);
                    //DME();
                    return -1;
                }

#ifdef _DEBUG_RET_CLI
                printf("[接收]LSSPROTO_KS_RECV-petarray:%d\n", petarray);
#endif
#ifdef _ABSOLUTE_DEBUG
                snprintf(errordata, sizeof(errordata) - 1, "%d", petarray);
#endif
                lssproto_KS_recv(fd, petarray);
                util_DiscardMessage();
                funflg = 1;
                break;
            }

#ifdef _STANDBYPET

        case LSSPROTO_SPET_RECV:
            {
                int checksum = 0, checksumrecv;
                int standbypet;
                checksum += util_deint(fd, 2, &standbypet);
                util_deint(fd, 3, &checksumrecv);

                if (checksum != checksumrecv)
                {
                    util_DiscardMessage();
                    logHack(fd, HACK_CHECKSUMERROR);
                    //DME();
                    return -1;
                }

#ifdef _DEBUG_RET_CLI
                printf("[接收]LSSPROTO_SPET_RECV-standbypet:%d\n", standbypet);
#endif
#ifdef _ABSOLUTE_DEBUG
                snprintf(errordata, sizeof(errordata) - 1, "%d", standbypet);
#endif
                lssproto_SPET_recv(fd, standbypet);
                util_DiscardMessage();
                funflg = 1;
                break;
            }

#endif
#ifdef _RIGHTCLICK

        case LSSPROTO_RCLICK_RECV:
            {
                int checksum = 0, checksumrecv;
                int type;
                char data[1024];
                checksum += util_deint(fd, 2, &type);
                checksum += util_destring(fd, 3, data);
                util_deint(fd, 4, &checksumrecv);

                if (checksum != checksumrecv)
                {
                    util_DiscardMessage();
                    logHack(fd, HACK_CHECKSUMERROR);
                    //DME();
                    return -1;
                }

#ifdef _DEBUG_RET_CLI
                printf("[接收]LSSPROTO_RCLICK_RECV-type:%d,data:%s\n", type, data);
#endif
#ifdef _ABSOLUTE_DEBUG
                snprintf(errordata, sizeof(errordata) - 1, "%d:%s", type, data);
#endif
                lssproto_RCLICK_recv(fd, type, data);
                util_DiscardMessage();
                return 0;
            }

#endif

        case LSSPROTO_AC_RECV:
            {
                int checksum = 0, checksumrecv;
                int x;
                int y;
                int actionno;
                checksum += util_deint(fd, 2, &x);
                checksum += util_deint(fd, 3, &y);
                checksum += util_deint(fd, 4, &actionno);
                util_deint(fd, 5, &checksumrecv);

                if (checksum != checksumrecv)
                {
                    util_DiscardMessage();
                    logHack(fd, HACK_CHECKSUMERROR);
                    //DME();
                    return -1;
                }

#ifdef _DEBUG_RET_CLI
                printf("[接收]LSSPROTO_AC_RECV-x:%d,y:%d,actionno:%d\n", x, y, actionno);
#endif
#ifdef _ABSOLUTE_DEBUG
                snprintf(errordata, sizeof(errordata) - 1, "%d:%d:%d", x, y, actionno);
#endif
                lssproto_AC_recv(fd, x, y, actionno);
                util_DiscardMessage();
                funflg = 1;
                break;
            }

        case LSSPROTO_MU_RECV:
            {
                int checksum = 0, checksumrecv;
                int x;
                int y;
                int array;
                int toindex;
                checksum += util_deint(fd, 2, &x);
                checksum += util_deint(fd, 3, &y);
                checksum += util_deint(fd, 4, &array);
                checksum += util_deint(fd, 5, &toindex);
                util_deint(fd, 6, &checksumrecv);

                if (checksum != checksumrecv)
                {
                    util_DiscardMessage();
                    logHack(fd, HACK_CHECKSUMERROR);
                    //DME();
                    return -1;
                }

#ifdef _DEBUG_RET_CLI
                printf("[接收]LSSPROTO_MU_RECV-x:%d,y:%d,array:%d,toindex:%d\n", x, y, array, toindex);
#endif
#ifdef _ABSOLUTE_DEBUG
                snprintf(errordata, sizeof(errordata) - 1, "%d:%d:%d:%d", x, y, array, toindex);
#endif
                lssproto_MU_recv(fd, x, y, array, toindex);
                util_DiscardMessage();
                funflg = 1;
                break;
            }

        case LSSPROTO_PS_RECV:
            {
                int checksum = 0, checksumrecv;
                int havepetindex;
                int havepetskill;
                int toindex;
                char data[1024 * 64];
                checksum += util_deint(fd, 2, &havepetindex);
                checksum += util_deint(fd, 3, &havepetskill);
                checksum += util_deint(fd, 4, &toindex);
                checksum += util_destring(fd, 5, data);
                util_deint(fd, 6, &checksumrecv);

                if (checksum != checksumrecv)
                {
                    util_DiscardMessage();
                    logHack(fd, HACK_CHECKSUMERROR);
                    //DME();
                    return -1;
                }

#ifdef _DEBUG_RET_CLI
                printf("[接收]LSSPROTO_PS_RECV-havepetindex:%d,havepetskill:%d,toindex:%d,data:%s\n", havepetindex, havepetskill, toindex, data);
#endif
#ifdef _ABSOLUTE_DEBUG
                snprintf(errordata, sizeof(errordata) - 1, "%d:%d:%d:%s", havepetindex, havepetskill, toindex, data);
#endif
                lssproto_PS_recv(fd, havepetindex, havepetskill, toindex, data);
                util_DiscardMessage();
                funflg = 1;
                break;
            }

        case LSSPROTO_ST_RECV:
            {
                int checksum = 0, checksumrecv;
                int titleindex;
                checksum += util_deint(fd, 2, &titleindex);
                util_deint(fd, 3, &checksumrecv);

                if (checksum != checksumrecv)
                {
                    util_DiscardMessage();
                    logHack(fd, HACK_CHECKSUMERROR);
                    //DME();
                    return -1;
                }

#ifdef _DEBUG_RET_CLI
                printf("[接收]LSSPROTO_ST_RECV-titleindex:%d\n", titleindex);
#endif
#ifdef _ABSOLUTE_DEBUG
                snprintf(errordata, sizeof(errordata) - 1, "%d", titleindex);
#endif
                lssproto_ST_recv(fd, titleindex);
                util_DiscardMessage();
                funflg = 1;
                break;
            }

        case LSSPROTO_DT_RECV:
            {
                int checksum = 0, checksumrecv;
                int titleindex;
                checksum += util_deint(fd, 2, &titleindex);
                util_deint(fd, 3, &checksumrecv);

                if (checksum != checksumrecv)
                {
                    util_DiscardMessage();
                    logHack(fd, HACK_CHECKSUMERROR);
                    //DME();
                    return -1;
                }

#ifdef _DEBUG_RET_CLI
                printf("[接收]LSSPROTO_DT_RECV-titleindex:%d\n", titleindex);
#endif
#ifdef _ABSOLUTE_DEBUG
                snprintf(errordata, sizeof(errordata) - 1, "%d", titleindex);
#endif
                lssproto_DT_recv(fd, titleindex);
                util_DiscardMessage();
                funflg = 1;
                break;
            }

        case LSSPROTO_FT_RECV:
            {
                int checksum = 0, checksumrecv;
                char data[1024 * 64];
                checksum += util_destring(fd, 2, data);
                util_deint(fd, 3, &checksumrecv);

                if (checksum != checksumrecv)
                {
                    util_DiscardMessage();
                    logHack(fd, HACK_CHECKSUMERROR);
                    //DME();
                    return -1;
                }

#ifdef _DEBUG_RET_CLI
                printf("[接收]LSSPROTO_FT_RECV-data:%s\n", data);
#endif
#ifdef _ABSOLUTE_DEBUG
                snprintf(errordata, sizeof(errordata) - 1, "%s", data);
#endif
                lssproto_FT_recv(fd, data);
                util_DiscardMessage();
                funflg = 1;
                break;
            }

        case LSSPROTO_SKUP_RECV:
            {
                int checksum = 0, checksumrecv;
                int skillid;
                checksum += util_deint(fd, 2, &skillid);
                util_deint(fd, 3, &checksumrecv);

                if (checksum != checksumrecv)
                {
                    util_DiscardMessage();
                    logHack(fd, HACK_CHECKSUMERROR);
                    //DME();
                    return -1;
                }

#ifdef _DEBUG_RET_CLI
                printf("[接收]LSSPROTO_SKUP_RECV-skillid:%d\n", skillid);
#endif
#ifdef _ABSOLUTE_DEBUG
                snprintf(errordata, sizeof(errordata) - 1, "%d", skillid);
#endif
                lssproto_SKUP_recv(fd, skillid);
                util_DiscardMessage();
                funflg = 1;
                break;
            }

        case LSSPROTO_KN_RECV:
            {
                int checksum = 0, checksumrecv;
                int havepetindex;
                char data[1024 * 64];
                checksum += util_deint(fd, 2, &havepetindex);
                checksum += util_destring(fd, 3, data);
                util_deint(fd, 4, &checksumrecv);

                if (checksum != checksumrecv)
                {
                    util_DiscardMessage();
                    logHack(fd, HACK_CHECKSUMERROR);
                    //DME();
                    return -1;
                }

#ifdef _DEBUG_RET_CLI
                printf("[接收]LSSPROTO_KN_RECV-havepetindex:%d,data:%s\n", havepetindex, data);
#endif
#ifdef _ABSOLUTE_DEBUG
                snprintf(errordata, sizeof(errordata) - 1, "%d:%s", havepetindex, data);
#endif
                lssproto_KN_recv(fd, havepetindex, data);
                util_DiscardMessage();
                funflg = 1;
                break;
            }

        case LSSPROTO_WN_RECV:
            {
                int checksum = 0, checksumrecv;
                int x;
                int y;
                int seqno;
                int objindex;
                int select;
                char data[1024 * 64];
                checksum += util_deint(fd, 2, &x);
                checksum += util_deint(fd, 3, &y);
                checksum += util_deint(fd, 4, &seqno);
                checksum += util_deint(fd, 5, &objindex);
                checksum += util_deint(fd, 6, &select);
                checksum += util_destring(fd, 7, data);
                util_deint(fd, 8, &checksumrecv);

                if (checksum != checksumrecv)
                {
                    util_DiscardMessage();
                    logHack(fd, HACK_CHECKSUMERROR);
                    //DME();
                    return -1;
                }

#ifdef _DEBUG_RET_CLI
                printf("[接收]LSSPROTO_WN_RECV-x:%d,y:%d,seqno:%d,objindex:%d,select:%d,data:%s\n", x, y, seqno, objindex, select, data);
#endif
#ifdef _ABSOLUTE_DEBUG
                snprintf(errordata, sizeof(errordata) - 1, "%d:%d:%d:%d:%d:%s", x, y, seqno, objindex, select, data);
#endif
                lssproto_WN_recv(fd, x, y, seqno, objindex, select, data);
                util_DiscardMessage();
                funflg = 1;
                break;
            }

        case LSSPROTO_SP_RECV:
            {
                int checksum = 0, checksumrecv;
                int x;
                int y;
                int dir;
                checksum += util_deint(fd, 2, &x);
                checksum += util_deint(fd, 3, &y);
                checksum += util_deint(fd, 4, &dir);
                util_deint(fd, 5, &checksumrecv);

                if (checksum != checksumrecv)
                {
                    util_DiscardMessage();
                    logHack(fd, HACK_CHECKSUMERROR);
                    //DME();
                    return -1;
                }

#ifdef _DEBUG_RET_CLI
                printf("[接收]LSSPROTO_SP_RECV-x:%d,y:%d,dir:%d\n", x, y, dir);
#endif
#ifdef _ABSOLUTE_DEBUG
                snprintf(errordata, sizeof(errordata) - 1, "%d:%d:%d", x, y, dir);
#endif
                lssproto_SP_recv(fd, x, y, dir);
                util_DiscardMessage();
                funflg = 1;
                break;
            }

        case LSSPROTO_CLIENTLOGIN_RECV:
            {
                int checksum = 0, checksumrecv;
                char cdkey[CDKEYLEN];
                char passwd[PASSWDLEN];
                char mac1[512];
                char mac2[512];
#ifndef _NEWCLIENTLOGIN
                char mac3[512];
#else
                int logintime = 0;
                char md5buff[128];
                char macbuff[512];
#endif
                int servid = 0;
                strcpy(PersonalKey, getRandDefaultKey(fd));
                checksum += util_destring(fd, 2, cdkey);
                checksum += util_destring(fd, 3, passwd);
                checksum += util_destring(fd, 4, mac1);
                checksum += util_destring(fd, 5, mac2);
#ifndef _NEWCLIENTLOGIN
                checksum += util_destring(fd, 6, mac3);
                checksum += util_deint(fd, 7, &servid);
                util_deint(fd, 8, &checksumrecv);
#else
                checksum += util_deint(fd, 6, &servid);
                checksum += util_deint(fd, 7, &logintime);
                checksum += util_destring(fd, 8, md5buff);
                checksum += util_destring(fd, 9, macbuff);
                util_deint(fd, 10, &checksumrecv);
#endif

                if (checksum == checksumrecv)
                {
#ifdef _DEBUG_RET_CLI
                    printf("[接收]LSSPROTO_CLIENTLOGIN_RECV-cdkey:%s,passwd:%s,mac1:%s,mac2:%s,servid:%d\n", cdkey, passwd, mac1, mac2, servid);
#endif
#ifdef _ABSOLUTE_DEBUG
                    snprintf(errordata, sizeof(errordata) - 1, "%s:%s:%s:%s:%d", cdkey, passwd, mac1, mac2, servid);
#endif
                }
                else
                {
                    util_DiscardMessage();
                    return -1;
                }

                if (strlen(cdkey) == 0
                        || strlen(passwd) == 0)
                {
                    util_DiscardMessage();
                    return -1;
                }

                lssproto_ClientLogin_recv(fd, cdkey, passwd, mac1, mac2, servid
#ifdef _NEWCLIENTLOGIN
                                          , logintime, md5buff, macbuff
#endif
                                         );
                util_DiscardMessage();
                funflg = 1;
                break;
            }

#ifdef _NEWCLIENTLOGIN

        case LSSPROTO_NEWCLIENTLOGIN_RECV:
            {
                int checksum = 0, checksumrecv;
                char cdkey[CDKEYLEN];
                char passwd[PASSWDLEN];
                char mac1[512];
                char mac2[512];
                int servid = 0;
                int logintime = 0;
                char md5buff[128];
                strcpy(PersonalKey, getRandDefaultKey(fd));
                checksum += util_destring(fd, 2, cdkey);
                checksum += util_destring(fd, 3, passwd);
                checksum += util_destring(fd, 4, mac1);
                checksum += util_destring(fd, 5, mac2);
                checksum += util_deint(fd, 6, &servid);
                checksum += util_deint(fd, 7, &logintime);
                checksum += util_destring(fd, 8, md5buff);
                util_deint(fd, 9, &checksumrecv);

                if (checksum == checksumrecv)
                {
#ifdef _DEBUG_RET_CLI
                    printf("[接收]LSSPROTO_CLIENTLOGIN_RECV-cdkey:%s,passwd:%s,mac1:%s,mac2:%s,servid:%d,logintime:%d,md5buff:%s\n", cdkey, passwd, mac1, mac2, servid, logintime, md5buff);
#endif
#ifdef _ABSOLUTE_DEBUG
                    snprintf(errordata, sizeof(errordata) - 1, "%s:%s:%s:%s:%d:%d:%s", cdkey, passwd, mac1, mac2, servid, logintime, md5buff);
#endif
                }
                else
                {
                    util_DiscardMessage();
                    return -1;
                }

                if (strlen(cdkey) == 0
                        || strlen(passwd) == 0)
                {
                    util_DiscardMessage();
                    return -1;
                }

                lssproto_NewClientLogin_recv(fd, cdkey, passwd, mac1, mac2, servid, logintime, md5buff);
                util_DiscardMessage();
                funflg = 1;
                break;
            }

#endif

        case LSSPROTO_QQLOGIN_RECV:
            {
                int checksum = 0, checksumrecv;
                int qdtype;
                char uid[49];
                int nowtime;
                char token[128];
                char mac1[512];
                char mac2[512];
                char mac3[512];
                memset(mac3, 0, sizeof(mac3));
                int servid = 0;
                strcpy(PersonalKey, getRandDefaultKey(fd));
                checksum += util_deint(fd, 2, &qdtype);
                checksum += util_destring(fd, 3, uid);
                checksum += util_destring(fd, 4, token);
                checksum += util_destring(fd, 5, mac1);
                checksum += util_destring(fd, 6, mac2);
                checksum += util_deint(fd, 7, &servid);
                util_deint(fd, 8, &checksumrecv);

                if (checksum == checksumrecv)
                {
#ifdef _DEBUG_RET_CLI
                    printf("[接收]LSSPROTO_QQLOGIN_RECV-qdtype:%d,uid:%s,token:%s,mac1:%s,mac2:%s,servid:%d\n", qdtype, uid, token, mac1, mac2, servid);
#endif
#ifdef _ABSOLUTE_DEBUG
                    snprintf(errordata, sizeof(errordata) - 1, "%d:%s:%s:%s:%s:%d", qdtype, uid, token, mac1, mac2, servid);
#endif
                }
                else
                {
                    util_DiscardMessage();
                    return -1;
                }

                if (strlen(uid) == 0
                        || strlen(token) == 0)
                {
                    util_DiscardMessage();
                    return -1;
                }

                lssproto_QQLogin_recv(fd, qdtype, uid, token, mac1, mac2, mac3, servid);
                util_DiscardMessage();
                funflg = 1;
                break;
            }

        case LSSPROTO_CREATENEWCHAR_RECV:
            {
                int checksum = 0, checksumrecv;
                int dataplacenum;
                char charname[CHARNAMELEN];;
                int imgno;
                int faceimgno;
                int vital;
                int str;
                int tgh;
                int dex;
                int earth;
                int water;
                int fire;
                int wind;
                int hometown;
                checksum += util_deint(fd, 2, &dataplacenum);
                checksum += util_destring(fd, 3, charname);
                checksum += util_deint(fd, 4, &imgno);
                checksum += util_deint(fd, 5, &faceimgno);
                checksum += util_deint(fd, 6, &vital);
                checksum += util_deint(fd, 7, &str);
                checksum += util_deint(fd, 8, &tgh);
                checksum += util_deint(fd, 9, &dex);
                checksum += util_deint(fd, 10, &earth);
                checksum += util_deint(fd, 11, &water);
                checksum += util_deint(fd, 12, &fire);
                checksum += util_deint(fd, 13, &wind);
                checksum += util_deint(fd, 14, &hometown);
                util_deint(fd, 15, &checksumrecv);

                if (checksum != checksumrecv)
                {
                    util_DiscardMessage();
                    logHack(fd, HACK_CHECKSUMERROR);
                    //DME();
                    return -1;
                }

#ifdef _DEBUG_RET_CLI
                printf("[接收]LSSPROTO_CREATENEWCHAR_RECV-dataplacenum:%d,charname:%s,imgno:%d,faceimgno:%d,vital:%d,str:%d,"
                       "tgh:%d,dex:%d,earth:%d,water:%d,fire:%d,wind:%d,hometown:%d\n",
                       dataplacenum, charname, imgno, faceimgno, vital, str, tgh, dex, earth, water, fire, wind, hometown);
#endif
#ifdef _ABSOLUTE_DEBUG
                snprintf(errordata, sizeof(errordata) - 1, "%d:%s:%d:%d:%d:%d:%d:%d:%d:%d:%d:%d:%d", dataplacenum, charname, imgno, faceimgno, vital, str, tgh, dex, earth, water, fire, wind, hometown);
#endif
                lssproto_CreateNewChar_recv(fd, dataplacenum, charname, imgno, faceimgno, vital, str, tgh, dex, earth, water, fire, wind, hometown);
                util_DiscardMessage();
                funflg = 1;
                break;
            }

        case LSSPROTO_CHARDELETE_RECV:
            {
                int checksum = 0, checksumrecv;
                char charname[CHARNAMELEN];;
                checksum += util_destring(fd, 2, charname);
                util_deint(fd, 3, &checksumrecv);

                if (checksum != checksumrecv)
                {
                    util_DiscardMessage();
                    logHack(fd, HACK_CHECKSUMERROR);
                    //DME();
                    return -1;
                }

#ifdef _DEBUG_RET_CLI
                printf("[接收]LSSPROTO_CHARDELETE_RECV-charname:%s\n", charname);
#endif
#ifdef _ABSOLUTE_DEBUG
                snprintf(errordata, sizeof(errordata) - 1, "%s", charname);
#endif
                lssproto_CharDelete_recv(fd, charname);
                util_DiscardMessage();
                funflg = 1;
                break;
            }

        case LSSPROTO_CHARLOGIN_RECV:
            {
                int checksum = 0, checksumrecv;
                char charname[CHARNAMELEN];;
                checksum += util_destring(fd, 2, charname);
                util_deint(fd, 3, &checksumrecv);

                if (checksum != checksumrecv)
                {
                    util_DiscardMessage();
                    logHack(fd, HACK_CHECKSUMERROR);
                    //DME();
                    return -1;
                }

#ifdef _DEBUG_RET_CLI
                printf("[接收]LSSPROTO_CHARLOGIN_RECV-charname:%s\n", charname);
#endif
#ifdef _ABSOLUTE_DEBUG
                snprintf(errordata, sizeof(errordata) - 1, "%s", charname);
#endif
                lssproto_CharLogin_recv(fd, charname);
                util_DiscardMessage();
                funflg = 1;
                break;
            }

        case LSSPROTO_CHARLIST_RECV:
            {
                int checksum = 0, checksumrecv;
                util_deint(fd, 2, &checksumrecv);

                if (checksum != checksumrecv)
                {
                    util_DiscardMessage();
                    logHack(fd, HACK_CHECKSUMERROR);
                    //DME();
                    return -1;
                }

#ifdef _DEBUG_RET_CLI
                printf("[接收]LSSPROTO_CHARLIST_RECV\n");
#endif
#ifdef _ABSOLUTE_DEBUG
                strcpy(errordata, "");
#endif
                lssproto_CharList_recv(fd);
                util_DiscardMessage();
                funflg = 1;
                break;
            }

        case LSSPROTO_CHARLOGOUT_RECV:
            {
                int checksum = 0, checksumrecv;
                int Flg = 1;
#ifdef _CHAR_NEWLOGOUT
                checksum += util_deint(fd, 2, &Flg);
                util_deint(fd, 3, &checksumrecv);
#else
                util_deint(fd, 2, &checksumrecv);
#endif

                if (checksum != checksumrecv)
                {
                    util_DiscardMessage();
                    logHack(fd, HACK_CHECKSUMERROR);
                    //DME();
                    return -1;
                }

#ifdef _DEBUG_RET_CLI
                printf("[接收]LSSPROTO_CHARLOGOUT_RECV-Flg:%d\n", Flg);
#endif
#ifdef _ABSOLUTE_DEBUG
                snprintf(errordata, sizeof(errordata) - 1, "%d", Flg);
#endif
                lssproto_CharLogout_recv(fd, Flg);
                util_DiscardMessage();
                funflg = 1;
                break;
            }

        case LSSPROTO_PROCGET_RECV:
            {
                int checksum = 0, checksumrecv;
                strcpy(PersonalKey, getRandDefaultKey(fd));
                util_deint(fd, 2, &checksumrecv);

                if (checksum != checksumrecv)
                {
                    util_DiscardMessage();
                    logHack(fd, HACK_CHECKSUMERROR);
                    //DME();
                    return -1;
                }

#ifdef _DEBUG_RET_CLI
                printf("[接收]LSSPROTO_PROCGET_RECV\n");
#endif
#ifdef _ABSOLUTE_DEBUG
                strcpy(errordata, "");
#endif
                lssproto_ProcGet_recv(fd);
                util_DiscardMessage();
                funflg = 1;
                break;
            }

        case LSSPROTO_PLAYERNUMGET_RECV:
            {
                int checksum = 0, checksumrecv;
                util_deint(fd, 2, &checksumrecv);

                if (checksum != checksumrecv)
                {
                    util_DiscardMessage();
                    logHack(fd, HACK_CHECKSUMERROR);
                    //DME();
                    return -1;
                }

#ifdef _DEBUG_RET_CLI
                printf("[接收]LSSPROTO_PLAYERNUMGET_RECV\n");
#endif
#ifdef _ABSOLUTE_DEBUG
                strcpy(errordata, "");
#endif
                lssproto_PlayerNumGet_recv(fd);
                util_DiscardMessage();
                funflg = 1;
                break;
            }

        case LSSPROTO_ECHO_RECV:
            {
                int checksum = 0, checksumrecv;
                char test[1024];
                checksum += util_destring(fd, 2, test);
                util_deint(fd, 3, &checksumrecv);

                if (checksum != checksumrecv)
                {
                    util_DiscardMessage();
                    logHack(fd, HACK_CHECKSUMERROR);
                    //DME();
                    return -1;
                }

#ifdef _DEBUG_RET_CLI
                printf("[接收]LSSPROTO_ECHO_RECV-test:%s\n", test);
#endif
#ifdef _ABSOLUTE_DEBUG
                snprintf(errordata, sizeof(errordata) - 1, "%s", test);
#endif
                lssproto_Echo_recv(fd, test);
                util_DiscardMessage();
                funflg = 1;
                break;
            }

        case LSSPROTO_TD_RECV:
            {
                int checksum = 0, checksumrecv;
                char message[1024 * 64];
                checksum += util_destring(fd, 2, message);
                util_deint(fd, 3, &checksumrecv);

                if (checksum != checksumrecv)
                {
                    util_DiscardMessage();
                    logHack(fd, HACK_CHECKSUMERROR);
                    //DME();
                    return -1;
                }

#ifdef _DEBUG_RET_CLI
                printf("[接收]LSSPROTO_TD_RECV-message:%s\n", message);
#endif
#ifdef _ABSOLUTE_DEBUG
                snprintf(errordata, sizeof(errordata) - 1, "%s", message);
#endif
                lssproto_TD_recv(fd, message);
                util_DiscardMessage();
                funflg = 1;
                break;
            }

        case LSSPROTO_FM_RECV:
            {
                int checksum = 0, checksumrecv;
                char message[1024 * 64];
                checksum += util_destring(fd, 2, message);
                util_deint(fd, 3, &checksumrecv);

                if (checksum != checksumrecv)
                {
                    util_DiscardMessage();
                    logHack(fd, HACK_CHECKSUMERROR);
                    //DME();
                    return -1;
                }

#ifdef _DEBUG_RET_CLI
                printf("[接收]LSSPROTO_FM_RECV-message:%s\n", message);
#endif
#ifdef _ABSOLUTE_DEBUG
                snprintf(errordata, sizeof(errordata) - 1, "%s", message);
#endif
                lssproto_FM_recv(fd, message);
                util_DiscardMessage();
                funflg = 1;
                break;
            }

        case LSSPROTO_PETST_RECV:
            {
                int checksum = 0, checksumrecv;
                int nPet;
                int sPet;
                checksum += util_deint(fd, 2, &nPet);
                checksum += util_deint(fd, 3, &sPet);
                util_deint(fd, 4, &checksumrecv);

                if (checksum != checksumrecv)
                {
                    util_DiscardMessage();
                    logHack(fd, HACK_CHECKSUMERROR);
                    //DME();
                    return -1;
                }

#ifdef _DEBUG_RET_CLI
                printf("[接收]LSSPROTO_PETST_RECV-nPet:%d,sPet:%d\n", nPet, sPet);
#endif
#ifdef _ABSOLUTE_DEBUG
                snprintf(errordata, sizeof(errordata) - 1, "%d:%d", nPet, sPet);
#endif
                lssproto_PETST_recv(fd, nPet, sPet);
                util_DiscardMessage();
                funflg = 1;
                break;
            }

#ifdef _CHECK_GAMESPEED

        case LSSPROTO_CS_RECV:
            {
                char buffer[2];
                buffer[0] = '\0';
#ifdef _DEBUG_RET_CLI
                printf("[接收]LSSPROTO_CS_RECV\n");
#endif
#ifdef _ABSOLUTE_DEBUG
                strcpy(errordata, "");
#endif
                lssproto_CS_recv(fd);
                util_DiscardMessage();
                funflg = 1;
                break;
            }

#endif
#ifdef _TEAM_KICKPARTY

        case LSSPROTO_KTEAM_RECV:
            {
                int checksum = 0, checksumrecv;
                int sindex;
                checksum += util_deint(fd, 2, &sindex);
                util_deint(fd, 3, &checksumrecv);

                if (checksum != checksumrecv)
                {
                    util_DiscardMessage();
                    logHack(fd, HACK_CHECKSUMERROR);
                    //DME();
                    return -1;
                }

#ifdef _DEBUG_RET_CLI
                printf("[接收]LSSPROTO_KTEAM_RECV-sindex:%d\n", sindex);
#endif
#ifdef _ABSOLUTE_DEBUG
                snprintf(errordata, sizeof(errordata) - 1, "%d", sindex);
#endif
                lssproto_KTEAM_recv(fd, sindex);
                util_DiscardMessage();
                funflg = 1;
                break;
            }

#endif
#ifdef _MIND_ICON

        case LSSPROTO_MA_RECV:
            {
                int checksum = 0, checksumrecv;
                int nMind;
                int x, y;
                checksum += util_deint(fd, 2, &nMind);
                checksum += util_deint(fd, 3, &x);
                checksum += util_deint(fd, 4, &y);
                util_deint(fd, 5, &checksumrecv);

                if (checksum != checksumrecv)
                {
                    util_DiscardMessage();
                    logHack(fd, HACK_CHECKSUMERROR);
                    //DME();
                    return -1;
                }

#ifdef _DEBUG_RET_CLI
                printf("[接收]LSSPROTO_MA_RECV-x:%d,y:%d,nMind:%d\n", x, y, nMind);
#endif
#ifdef _ABSOLUTE_DEBUG
                snprintf(errordata, sizeof(errordata) - 1, "%d:%d:%d", x, y, nMind);
#endif
                lssproto_MA_recv(fd, x, y, nMind);
                util_DiscardMessage();
                funflg = 1;
                break;
            }

#endif
#ifdef _CHATROOMPROTOCOL            // (不可开) Syu ADD 聊天室频道

        case LSSPROTO_CHATROOM_RECV:
            {
                int checksum = 0, checksumrecv;
                char test[1024 * 64];
                checksum += util_destring(fd, 2, test);
                util_deint(fd, 3, &checksumrecv);

                if (checksum != checksumrecv)
                {
                    util_DiscardMessage();
                    logHack(fd, HACK_CHECKSUMERROR);
                    //DME();
                    return -1;
                }

#ifdef _DEBUG_RET_CLI
                printf("[接收]LSSPROTO_CHATROOM_RECV-test:%s\n", test);
#endif
#ifdef _ABSOLUTE_DEBUG
                snprintf(errordata, sizeof(errordata) - 1, "%s", test);
#endif
                lssproto_CHATROOM_recv(fd, test);
                util_DiscardMessage();
                funflg = 1;
                break;
            }

#endif
#ifdef _NEWREQUESTPROTOCOL          // (不可开) Syu ADD 新增Protocol要求细项

        case LSSPROTO_RESIST_RECV:
            {
                char buffer[2];
                buffer[0] = '\0';
#ifdef _DEBUG_RET_CLI
                printf("[接收]LSSPROTO_RESIST_RECV\n");
#endif
#ifdef _ABSOLUTE_DEBUG
                strcpy(errordata, "");
#endif
                lssproto_RESIST_recv(fd);
                util_DiscardMessage();
                funflg = 1;
                break;
            }

#endif
#ifdef _OUTOFBATTLESKILL            // (不可开) Syu ADD 非战斗时技能Protocol

        case LSSPROTO_BATTLESKILL_RECV:
            {
                int checksum = 0, checksumrecv;
                int iNum;
                checksum += util_deint(fd, 2, &iNum);
                util_deint(fd, 3, &checksumrecv);

                if (checksum != checksumrecv)
                {
                    util_DiscardMessage();
                    logHack(fd, HACK_CHECKSUMERROR);
                    //DME();
                    return -1;
                }

#ifdef _DEBUG_RET_CLI
                printf("[接收]LSSPROTO_BATTLESKILL_RECV-iNum:%d\n", iNum);
#endif
#ifdef _ABSOLUTE_DEBUG
                snprintf(errordata, sizeof(errordata) - 1, "%d", iNum);
#endif
                lssproto_BATTLESKILL_recv(fd, iNum);
                util_DiscardMessage();
                funflg = 1;
                break;
            }

#endif
#ifdef _STREET_VENDOR

        case LSSPROTO_STREET_VENDOR_RECV:
            {
                int checksum = 0, checksumrecv;
                char message[1024 * 64];
                checksum += util_destring(fd, 2, message);
                util_deint(fd, 3, &checksumrecv);

                if (checksum != checksumrecv)
                {
                    util_DiscardMessage();
                    logHack(fd, HACK_CHECKSUMERROR);
                    //DME();
                    return -1;
                }

#ifdef _DEBUG_RET_CLI
                printf("[接收]LSSPROTO_STREET_VENDOR_RECV-message:%s\n", message);
#endif
#ifdef _ABSOLUTE_DEBUG
                snprintf(errordata, sizeof(errordata) - 1, "%s", message);
#endif
                lssproto_STREET_VENDOR_recv(fd, message);
                util_DiscardMessage();
                funflg = 1;
                break;
            }

#endif
#ifdef _JOBDAILY

        case LSSPROTO_JOBDAILY_RECV:
            {
                int checksum = 0, checksumrecv;
                char buffer[1024 * 32];
                checksum += util_destring(fd, 2, buffer);
                util_deint(fd, 3, &checksumrecv);

                if (checksum != checksumrecv)
                {
                    util_DiscardMessage();
                    logHack(fd, HACK_CHECKSUMERROR);
                    //DME();
                    return -1;
                }

#ifdef _DEBUG_RET_CLI
                printf("[接收]LSSPROTO_JOBDAILY_RECV-buffer:%s\n", buffer);
#endif
#ifdef _ABSOLUTE_DEBUG
                snprintf(errordata, sizeof(errordata) - 1, "%s", buffer);
#endif
                lssproto_JOBDAILY_recv(fd, buffer);
                util_DiscardMessage();
                funflg = 1;
                break;
            }

#endif
#ifdef _TEACHER_SYSTEM

        case LSSPROTO_TEACHER_SYSTEM_RECV:
            {
                int checksum = 0, checksumrecv;
                char message[1024 * 64];
                checksum += util_destring(fd, 2, message);
                util_deint(fd, 3, &checksumrecv);

                if (checksum != checksumrecv)
                {
                    util_DiscardMessage();
                    logHack(fd, HACK_CHECKSUMERROR);
                    //DME();
                    return -1;
                }

#ifdef _DEBUG_RET_CLI
                printf("[接收]LSSPROTO_TEACHER_SYSTEM_RECV-message:%s\n", message);
#endif
#ifdef _ABSOLUTE_DEBUG
                snprintf(errordata, sizeof(errordata) - 1, "%s", message);
#endif
                lssproto_TEACHER_SYSTEM_recv(fd, message);
                util_DiscardMessage();
                funflg = 1;
                break;
            }

#endif
#ifdef _ADD_STATUS_2

        case LSSPROTO_S2_RECV:
            {
#ifdef _DEBUG_RET_CLI
                printf("[接收]LSSPROTO_S2_RECV\n");
#endif
#ifdef _ABSOLUTE_DEBUG
                strcpy(errordata, "");
#endif
                funflg = 1;
                break;
            }

#endif
#ifdef _PET_ITEM

        case LSSPROTO_PET_ITEM_RECV:
            {
                int checksum = 0, checksumrecv;
                int petindex;
                int x, y;
                int fromindex;
                int toindex;
                checksum += util_deint(fd, 2, &x);
                checksum += util_deint(fd, 3, &y);
                checksum += util_deint(fd, 4, &petindex);
                checksum += util_deint(fd, 5, &fromindex);
                checksum += util_deint(fd, 6, &toindex);
                util_deint(fd, 7, &checksumrecv);

                if (checksum != checksumrecv)
                {
                    util_DiscardMessage();
                    logHack(fd, HACK_CHECKSUMERROR);
                    //DME();
                    return -1;
                }

#ifdef _DEBUG_RET_CLI
                printf("[接收]LSSPROTO_PET_ITEM_RECV-x:%d,y:%d,petindex:%d,fromindex:%d,toindex:%d\n", x, y, petindex, fromindex, toindex);
#endif
#ifdef _ABSOLUTE_DEBUG
                snprintf(errordata, sizeof(errordata) - 1, "%d:%d:%d:%d:%d", x, y, petindex, fromindex, toindex);
#endif
                lssproto_PETITEM_recv(fd, x, y, petindex, fromindex, toindex);
                util_DiscardMessage();
                funflg = 1;
                break;
            }

#endif
#ifdef _ASSESS_ABILITY

        case LSSPROTO_ASSESS_ABILITY_RECV:
            {
                int checksum = 0, checksumrecv;
                char data[256];
                checksum += util_destring(fd, 2, data);
                util_deint(fd, 3, &checksumrecv);

                if (checksum != checksumrecv)
                {
                    util_DiscardMessage();
                    logHack(fd, HACK_CHECKSUMERROR);
                    //DME();
                    return -1;
                }

#ifdef _DEBUG_RET_CLI
                printf("[接收]LSSPROTO_ASSESS_ABILITY_RECV-data:%s\n", data);
#endif
#ifdef _ABSOLUTE_DEBUG
                snprintf(errordata, sizeof(errordata) - 1, "%s", data);
#endif
                //lssproto_ASSESS_ABILITY_send( fd, "10000|10000|10000|10000|10000|10000|10000|10000|10000|10000|10000|10000|10000|10000|10000|10000|10000|10000|10000|10000|10000|10000|10000|10000|10000|" );
                lssproto_ASSESS_ABILITY_recv(fd);
                util_DiscardMessage();
                funflg = 1;
                break;
            }

#endif
#ifdef _ONLINE_SHOP

        case LSSPROTO_VIP_SHOP_RECV:
            {
                int checksum = 0, checksumrecv;
                int type, page;
                checksum += util_deint(fd, 2, &type);
                checksum += util_deint(fd, 3, &page);
                util_deint(fd, 4, &checksumrecv);

                if (checksum != checksumrecv)
                {
                    util_DiscardMessage();
                    logHack(fd, HACK_CHECKSUMERROR);
                    //DME();
                    return -1;
                }

#ifdef _DEBUG_RET_CLI
                printf("[接收]LSSPROTO_VIP_SHOP_RECV-x:%d,y:%d\n", type, page);
#endif
#ifdef _ABSOLUTE_DEBUG
                snprintf(errordata, sizeof(errordata) - 1, "%d:%d", type, page);
#endif
                lssproto_VIP_SHOP_recv(fd, type, page);
                util_DiscardMessage();
                funflg = 1;
                break;
            }

#endif
#ifdef _ONLINE_SHOP

        case LSSPROTO_VIP_SHOP_BUY_RECV:
            {
                int checksum = 0, checksumrecv;
                int type, page, id, num;
                checksum += util_deint(fd, 2, &type);
                checksum += util_deint(fd, 3, &page);
                checksum += util_deint(fd, 4, &id);
                checksum += util_deint(fd, 5, &num);
                util_deint(fd, 6, &checksumrecv);

                if (checksum != checksumrecv)
                {
                    util_DiscardMessage();
                    logHack(fd, HACK_CHECKSUMERROR);
                    //DME();
                    return -1;
                }

#ifdef _DEBUG_RET_CLI
                printf("[接收]LSSPROTO_VIP_SHOP_RECV-type:%d,page:%d,id:%d,num:%d\n", type, page, id, num);
#endif
#ifdef _ABSOLUTE_DEBUG
                snprintf(errordata, sizeof(errordata) - 1, "%d:%d:%d:%d", type, page, id, num);
#endif
                lssproto_VIP_SHOP_buy_recv(fd, type, page, id, num);
                util_DiscardMessage();
                funflg = 1;
                break;
            }

#endif
#ifdef _ONLINE_SHOP

        case LSSPROTO_VIP_SHOP_AMPOINT_BUY_RECV:
            {
                int checksum = 0, checksumrecv;
                int type, page, id, num;
                checksum += util_deint(fd, 2, &type);
                checksum += util_deint(fd, 3, &page);
                checksum += util_deint(fd, 4, &id);
                checksum += util_deint(fd, 5, &num);
                util_deint(fd, 6, &checksumrecv);

                if (checksum != checksumrecv)
                {
                    util_DiscardMessage();
                    logHack(fd, HACK_CHECKSUMERROR);
                    //DME();
                    return -1;
                }

#ifdef _DEBUG_RET_CLI
                printf("[接收]LSSPROTO_VIP_SHOP_RECV-type:%d,page:%d,id:%d,num:%d\n", type, page, id, num);
#endif
#ifdef _ABSOLUTE_DEBUG
                snprintf(errordata, sizeof(errordata) - 1, "%d:%d:%d:%d", type, page, id, num);
#endif
                lssproto_VIP_SHOP_buy_recv(fd, type, page, id, num);
                util_DiscardMessage();
                funflg = 1;
                break;
            }

#endif

        case LSSPROTO_SAMENU_RECV:
            {
                int checksum = 0, checksumrecv;
                int index;
                checksum += util_deint(fd, 2, &index);
                util_deint(fd, 3, &checksumrecv);
#ifdef _DEBUG_RET_CLI
                printf("[接收]LSSPROTO_SAMENU_RECV-index:%d\n", index);
#endif
#ifdef _ABSOLUTE_DEBUG
                snprintf(errordata, sizeof(errordata) - 1, "%d", index);
#endif
                lssproto_SaMenu_recv(fd, index);
                util_DiscardMessage();
                funflg = 1;
                break;
            }

        case LSSPROTO_NEWCHARDELETE_RECV:
            {
                int checksum = 0, checksumrecv;
                char charname[64];
                char safepasswd[64];
                checksum += util_destring(fd, 2, charname);
                checksum += util_destring(fd, 3, safepasswd);
                util_deint(fd, 4, &checksumrecv);
#ifdef _DEBUG_RET_CLI
                printf("[接收]LSSPROTO_NEWCHARDELETE_RECV-charname:%s,safepasswd:%s\n", charname, safepasswd);
#endif
#ifdef _ABSOLUTE_DEBUG
                snprintf(errordata, sizeof(errordata) - 1, "%s:%s", charname, safepasswd);
#endif
                lssproto_NewCharDelete_recv(fd, charname, safepasswd);
                util_DiscardMessage();
                funflg = 1;
                break;
            }

#ifdef  _OFFLINE_RETEUN

        case LSSPROTO_NEWLOGOUT_RECV:
            {
                int checksum = 0, checksumrecv;
                int newlogoutflg;
                checksum += util_deint(fd, 2, &newlogoutflg);
                util_deint(fd, 3, &checksumrecv);
#ifdef _DEBUG_RET_CLI
                printf("[接收]LSSPROTO_NEWLOGOUT_RECV-newlogoutflg:%d\n", newlogoutflg);
#endif
#ifdef _ABSOLUTE_DEBUG
                snprintf(errordata, sizeof(errordata) - 1, "%d", newlogoutflg);
#endif
                lssproto_NEWLOGOUT_recv(fd, newlogoutflg);
                util_DiscardMessage();
                funflg = 1;
                break;
            }

#endif
#ifdef  _OFFLINE_RETEUN

        case LSSPROTO_NEWLOGOUT2_RECV:
            {
                int checksum = 0, checksumrecv;
                int newlogoutflg;
                checksum += util_deint(fd, 2, &newlogoutflg);
                util_deint(fd, 3, &checksumrecv);
#ifdef _DEBUG_RET_CLI
                printf("[接收]LSSPROTO_NEWLOGOUT2_RECV-newlogoutflg:%d\n", newlogoutflg);
#endif
#ifdef _ABSOLUTE_DEBUG
                snprintf(errordata, sizeof(errordata) - 1, "%d", newlogoutflg);
#endif
                lssproto_NEWLOGOUT2_recv(fd, newlogoutflg);
                util_DiscardMessage();
                funflg = 1;
                break;
            }

#endif

        case LSSPROTO_RIDEQUERY_RECV:
            {
                int checksumrecv;
                int checksum = 0;
                int id = 0;
                checksum += util_deint(fd, 2, &id);
                util_deint(fd, 3, &checksumrecv);
#ifdef _DEBUG_RET_CLI
                printf("[接收]LSSPROTO_RIDEQUERY_RECV-id:%d\n", id);
#endif
#ifdef _ABSOLUTE_DEBUG
                snprintf(errordata, sizeof(errordata) - 1, "%d", id);
#endif
                FreeRideQuery(CONNECT_getCharaindex(fd));
                util_DiscardMessage();
                funflg = 1;
                break;
            }

#ifdef _RED_MEMOY_

        case LSSPROTO_REDMEMOY_RECV:
            {
                int checksumrecv;
                int checksum = 0;
                int type;
                char str[512];
                checksum += util_deint(fd, 2, &type);
                checksum += util_destring(fd, 3, str);
                util_deint(fd, 4, &checksumrecv);
#ifdef _DEBUG_RET_CLI
                printf("[接收]LSSPROTO_REDMEMOY_RECV-type:%d,str=%s\n", type, str);
#endif
#ifdef _ABSOLUTE_DEBUG
                snprintf(errordata, sizeof(errordata) - 1, "%d,%s", type, str);
#endif
                extern void CHAR_RedMemoy_WindowSend(int charaindex);
                extern void CHAR_RedMemoy_Give(int charaindex, char *data);

                if (type == 1) //发送窗体
                {
                    CHAR_RedMemoy_WindowSend(CONNECT_getCharaindex(fd));
                }
                else if (type == 2) //领取红包
                {
                    CHAR_RedMemoy_Give(CONNECT_getCharaindex(fd), str);
                }

                util_DiscardMessage();
                funflg = 1;
                break;
            }

        case LSSPROTO_NEWREDMEMOY_RECV:
            {
                int checksumrecv;
                int checksum = 0;
                int type;
                char str[512];
                checksum += util_deint(fd, 2, &type);
                checksum += util_destring(fd, 3, str);
                util_deint(fd, 4, &checksumrecv);
#ifdef _DEBUG_RET_CLI
                printf("[接收]LSSPROTO_NEWREDMEMOY_RECV-type:%d,str=%s\n", type, str);
#endif
#ifdef _ABSOLUTE_DEBUG
                snprintf(errordata, sizeof(errordata) - 1, "%d,%s", type, str);
#endif
                lssproto_NewRedMoney_recv(CONNECT_getCharaindex(fd), type, str);
                util_DiscardMessage();
                funflg = 1;
                break;
            }

#endif
#ifdef _FAMILYBADGE_

        case LSSPROTO_FAMILYBADGE_RECV:
            {
                int checksum = 0, checksumrecv;
                int test;
                checksum += util_deint(fd, 2, &test);
                util_deint(fd, 3, &checksumrecv);
#ifdef _DEBUG_RET_CLI
                printf("[接收]LSSPROTO_FAMILYBADGE_RECV:%d\n", test);
#endif
#ifdef _ABSOLUTE_DEBUG
                snprintf(errordata, sizeof(errordata) - 1, "%d", test);
#endif
                lssproto_FamilyBadge_recv(fd);
                util_DiscardMessage();
                funflg = 1;
                break;
            }

#endif

        case LSSPROTO_FSUPDATE_RECV:
            {
#ifdef _DEBUG_RET_CLI
                printf("[接收]LSSPROTO_FSUPDATE_RECV\n");
#endif
#ifdef _ABSOLUTE_DEBUG
                snprintf(errordata, sizeof(errordata) - 1, "");
#endif
                lssproto_FsUpdate_recv(fd);
                util_DiscardMessage();
                funflg = 1;
                break;
            }

#ifdef  _PETITEM_TALKSHOW

        case LSSPROTO_PETITEMTALKSHOW_RECV:
            {
                int posindex, checksumrecv;
                int checksum = 0;
                char posname[64];
                checksum += util_deint(fd, 2, &posindex);
                checksum += util_destring(fd, 3, posname);
                util_deint(fd, 4, &checksumrecv);
#ifdef _DEBUG_RET_CLI
                printf("[接收]LSSPROTO_PETITEMTALKSHOW_RECV:%d,%s\n", posindex, posname);
#endif
#ifdef _ABSOLUTE_DEBUG
                snprintf(errordata, sizeof(errordata) - 1, "%d:%s", posindex, posname);
#endif
                lssproto_PetItemTalkShow_recv(fd, posindex, posname);
                util_DiscardMessage();
                funflg = 1;
                break;
            }

#endif
#ifdef  _NEW_MIYU_

        case LSSPROTO_NEWCHAT_RECV:
            {
                int checksumrecv;
                int checksum = 0;
                char uid[64];
                checksum += util_destring(fd, 2, uid);
                util_deint(fd, 3, &checksumrecv);
#ifdef _DEBUG_RET_CLI
                printf("[接收]LSSPROTO_NEWCHAT_RECV:%s\n", uid);
#endif
#ifdef _ABSOLUTE_DEBUG
                snprintf(errordata, sizeof(errordata) - 1, "%s", uid);
#endif
                lssproto_NewChat_recv(fd, uid);
                util_DiscardMessage();
                funflg = 1;
                break;
            }

#endif
#ifdef _NEW_SHOP_

        case LSSPROTO_UPSHOP_RECV:
            {
                int checksumrecv;
                int checksum = 0;
                char shop[5][1502] = { 0 };
                checksum += util_destring(fd, 2, shop[0]);
                checksum += util_destring(fd, 3, shop[1]);
                checksum += util_destring(fd, 4, shop[2]);
                checksum += util_destring(fd, 5, shop[3]);
                checksum += util_destring(fd, 6, shop[4]);
                util_deint(fd, 7, &checksumrecv);

                if (checksum != checksumrecv)
                {
                    util_DiscardMessage();
                    return -1;
                }

                lssproto_upshopdata_recv(fd, &shop[0][0]);
                util_DiscardMessage();
                funflg = 1;
                break;
            }

        case LSSPROTO_SHOPOK_RECV:
            {
                int escindex, checksumrecv;
                int checksum = 0;
                checksum += util_deint(fd, 2, &escindex);
                util_deint(fd, 3, &checksumrecv);

                if (checksum != checksumrecv)
                {
                    util_DiscardMessage();
                    return -1;
                }

                extern void FreeShopRecv(int charaindex);
                FreeShopRecv(CONNECT_getCharaindex(fd));
                util_DiscardMessage();
                funflg = 1;
                break;
            }

#endif
#ifdef  _NEW_UI

        case LSSPROTO_NEWUI_RECV:
            {
                int checksumrecv;
                int checksum = 0;
                int flg;
                char data[128];
                checksum += util_deint(fd, 2, &flg);
                checksum += util_destring(fd, 3, data);
                util_deint(fd, 4, &checksumrecv);
#ifdef _DEBUG_RET_CLI
                printf("[接收]LSSPROTO_NEWUI_RECV:%d,%s\n", flg, data);
#endif
#ifdef _ABSOLUTE_DEBUG
                snprintf(errordata, sizeof(errordata) - 1, "%d:%s", flg, data);
#endif
                lssproto_NewUi_recv(fd, flg, data);
                util_DiscardMessage();
                funflg = 1;
                break;
            }

#endif

        case LSSPROTO_QQ_RECV:
            {
                int checksumrecv;
                int checksum = 0;
                char data[10240];
                memset(data, 0, sizeof(data));
                checksum += util_destring(fd, 2, data);
                util_deint(fd, 3, &checksumrecv);
#ifdef _DEBUG_RET_CLI
                printf("[接收]LSSPROTO_QQ_RECV:%s\n", data);
#endif
#ifdef _ABSOLUTE_DEBUG
                snprintf(errordata, sizeof(errordata) - 1, "%s", data);
#endif
                data[512] = 0;
                //printf("\nQQ=%s", data);
                lssproto_Qq_recv(fd, data);
                util_DiscardMessage();
                funflg = 1;
                break;
            }

#ifdef  _NEW_OFFLINEBATTLERETURN

        case LSSPROTO_OFFLINEBATTLE_RECV:
            {
                int checksumrecv;
                int checksum = 0;
                int flg;
                checksum += util_deint(fd, 2, &flg);
                util_deint(fd, 3, &checksumrecv);
#ifdef _DEBUG_RET_CLI
                printf("[接收]LSSPROTO_OFFLINEBATTLE_RECV:%d\n", flg);
#endif
#ifdef _ABSOLUTE_DEBUG
                snprintf(errordata, sizeof(errordata) - 1, "%d", flg);
#endif
                lssproto_OfflineBattle_recv(fd, flg);
                util_DiscardMessage();
                funflg = 1;
                break;
            }

#endif

        case LSSPROTO_GETGOLD_RECV:
            {
                int checksumrecv;
                int checksum = 0;
                int flg;
                checksum += util_deint(fd, 2, &flg);
                util_deint(fd, 3, &checksumrecv);
#ifdef _DEBUG_RET_CLI
                printf("[接收]LSSPROTO_GETGOLD_RECV:%d\n", flg);
#endif
#ifdef _ABSOLUTE_DEBUG
                snprintf(errordata, sizeof(errordata) - 1, "%d", flg);
#endif
                lssproto_GetGold_recv(fd, flg);
                util_DiscardMessage();
                funflg = 1;
                break;
            }

        case LSSPROTO_PLAYEREXT_RECV:
            {
                int checksumrecv;
                int checksum = 0;
                int objindex;
                int flg;
                checksum += util_deint(fd, 2, &objindex);
                checksum += util_deint(fd, 3, &flg);
                util_deint(fd, 4, &checksumrecv);
#ifdef _DEBUG_RET_CLI
                printf("[接收]LSSPROTO_PLAYEREXT_RECV:%d,%d\n", objindex, flg);
#endif
#ifdef _ABSOLUTE_DEBUG
                snprintf(errordata, sizeof(errordata) - 1, "%d:%d", objindex, flg);
#endif
                lssproto_PlayerExt_recv(fd, objindex, flg);
                util_DiscardMessage();
                funflg = 1;
                break;
            }

        case LSSPROTO_MAIL_RECV:
            {
                int checksumrecv;
                int checksum = 0;
                int flg;
                checksum += util_deint(fd, 2, &flg);
                util_deint(fd, 3, &checksumrecv);
#ifdef _DEBUG_RET_CLI
                printf("[接收]LSSPROTO_PLAYEREXT_RECV:%d\n", flg);
#endif
#ifdef _ABSOLUTE_DEBUG
                snprintf(errordata, sizeof(errordata) - 1, "%d", flg);
#endif
                lssproto_Mail_recv(fd, flg);
                util_DiscardMessage();
                funflg = 1;
                break;
            }

        case LSSPROTO_NEWSTREET_RECV:
            {
                int checksumrecv;
                int checksum = 0;
                char data[10240];
                memset(data, 0, sizeof(data));
                checksum += util_destring(fd, 2, data);
                util_deint(fd, 3, &checksumrecv);
#ifdef _DEBUG_RET_CLI
                printf("[接收]LSSPROTO_NEWSTREET_RECV:%s\n", data);
#endif
#ifdef _ABSOLUTE_DEBUG
                snprintf(errordata, sizeof(errordata) - 1, "%s", data);
#endif
                FreeStreet(fd, data);
                util_DiscardMessage();
                funflg = 1;
                break;
            }

#ifdef _NEW_PROMPTFLG

        case LSSPROTO_NEWPROMPTFLG_RECV:
            {
                int checksumrecv;
                int checksum = 0;
                int flg1, flg2;
                checksum += util_deint(fd, 2, &flg1);
                checksum += util_deint(fd, 3, &flg2);
                util_deint(fd, 4, &checksumrecv);
#ifdef _DEBUG_RET_CLI
                printf("[接收]LSSPROTO_NEWPROMPTFLG_RECV:%d,%d\n", flg1, flg2);
#endif
#ifdef _ABSOLUTE_DEBUG
                snprintf(errordata, sizeof(errordata) - 1, "%d,%d");
#endif
                lssproto_NewPromptFlg_recv(fd, flg1, flg2);
                util_DiscardMessage();
                funflg = 1;
                break;
            }

#endif
#ifdef _NEWPLAYER_FLG

        case LSSPROTO_NEWPLAYERFLG_RECV:
            {
                int checksumrecv;
                int checksum = 0;
                int flg1, flg2;
                checksum += util_deint(fd, 2, &flg1);
                checksum += util_deint(fd, 3, &flg2);
                util_deint(fd, 4, &checksumrecv);
#ifdef _DEBUG_RET_CLI
                printf("[接收]LSSPROTO_NEWPLAYERFLG_RECV:%d,%d\n", flg1, flg2);
#endif
#ifdef _ABSOLUTE_DEBUG
                snprintf(errordata, sizeof(errordata) - 1, "%d:%d", flg1, flg2);
#endif
                FreeNewPlayerFlg(CONNECT_getCharaindex(fd), flg1, flg2);
                util_DiscardMessage();
                funflg = 1;
                break;
            }

#endif

        case LSSPROTO_SECURITY_RECV:
            {
                int checksumrecv;
                int checksum = 0;
                int type, codetype, index;
                char codedata[32];
                memset(codedata, 0, sizeof(codedata));
                checksum += util_deint(fd, 2, &type);
                checksum += util_deint(fd, 3, &codetype);
                checksum += util_deint(fd, 4, &index);
                checksum += util_destring(fd, 5, codedata);
                util_deint(fd, 6, &checksumrecv);
#ifdef _DEBUG_RET_CLI
                printf("[接收]LSSPROTO_SECURITY_RECV:%d,%d,%d,%s\n", type, codetype, index, codedata);
#endif
#ifdef _ABSOLUTE_DEBUG
                snprintf(errordata, sizeof(errordata) - 1, "%d,%d,%d,%s", type, codetype, index, codedata);
#endif
                FreeSecurity(CONNECT_getCharaindex(fd), type, codetype, index, codedata);
                util_DiscardMessage();
                funflg = 1;
                break;
            }

#ifdef _PARTY_HIGH

        case LSSPROTO_PARTYHIGH_RECV:
            {
                int checksumrecv;
                int checksum = 0;
                int partytype;
                char partybuff[128];
                memset(partybuff, 0, sizeof(partybuff));
                checksum += util_deint(fd, 2, &partytype);
                checksum += util_destring(fd, 3, partybuff);
                util_deint(fd, 4, &checksumrecv);
#ifdef _DEBUG_RET_CLI
                printf("[接收]LSSPROTO_PARTYHIGH_RECV:%d,%s\n", partytype, partybuff);
#endif
#ifdef _ABSOLUTE_DEBUG
                snprintf(errordata, sizeof(errordata) - 1, "%d:%s", partytype, partybuff);
#endif
                lssproto_PartyHigh_recv(fd, partytype, partybuff);
                util_DiscardMessage();
                funflg = 1;
                break;
            }

#endif
#ifdef _TALK_PLAYER_SHOW

        case LSSPROTO_TALKPLAYERSHOW_RECV:
            {
                int checksumrecv;
                int checksum = 0;
                int playerobj;
                char playername[CHARNAMELEN];
                memset(playername, 0, sizeof(playername));
                checksum += util_deint(fd, 2, &playerobj);
                checksum += util_destring(fd, 3, playername);
                util_deint(fd, 4, &checksumrecv);
#ifdef _DEBUG_RET_CLI
                printf("[接收]LSSPROTO_TALKPLAYERSHOW_RECV:%d,%s\n", playerobj, playername);
#endif
#ifdef _ABSOLUTE_DEBUG
                snprintf(errordata, sizeof(errordata) - 1, "%d:%s", playerobj, playername);
#endif
                lssproto_TalkPlayerShow_recv(fd, playerobj, playername);
                util_DiscardMessage();
                funflg = 1;
                break;
            }

#endif

        case LSSPROTO_DIN_RECV:
            {
                int checksum = 0, checksumrecv;
                int x;
                int y;
                int itemindex;
                int num;
                checksum += util_deint(fd, 2, &x);
                checksum += util_deint(fd, 3, &y);
                checksum += util_deint(fd, 4, &itemindex);
                checksum += util_deint(fd, 5, &num);
                util_deint(fd, 6, &checksumrecv);

                if (checksum != checksumrecv)
                {
                    util_DiscardMessage();
                    logHack(fd, HACK_CHECKSUMERROR);
                    //DME();
                    return -1;
                }

#ifdef _DEBUG_RET_CLI
                printf("[接收]LSSPROTO_DIN_RECV-x:%d,y:%d,itemindex:%d,num:%d\n", x, y, itemindex, num);
#endif
#ifdef _ABSOLUTE_DEBUG
                snprintf(errordata, sizeof(errordata) - 1, "%d:%d:%d:%d", x, y, itemindex, num);
#endif
                lssproto_DIN_recv(fd, x, y, itemindex, num);
                util_DiscardMessage();
                funflg = 1;
                break;
            }

        case LSSPROTO_NEWSAMENU_RECV:
            {
                int checksum = 0, checksumrecv;
                int index;
                char data[128];
                memset(data, 0, sizeof(data));
                checksum += util_deint(fd, 2, &index);
                checksum += util_destring(fd, 3, data);
                util_deint(fd, 4, &checksumrecv);

                if (checksum != checksumrecv)
                {
                    util_DiscardMessage();
                    logHack(fd, HACK_CHECKSUMERROR);
                    //DME();
                    return -1;
                }

#ifdef _DEBUG_RET_CLI
                printf("[接收]LSSPROTO_NEWSAMENU_RECV-index:%d,data:%s\n", index, data);
#endif
#ifdef _ABSOLUTE_DEBUG
                snprintf(errordata, sizeof(errordata) - 1, "%d:%s", index, data);
#endif
                FreeNewSaMenu(fd, index, data);
                util_DiscardMessage();
                funflg = 1;
                break;
            }
#ifdef _NEWPLAYER_FUNC
	case LSSPROTO_NEWPLAYER_RECV:
	{
		int checksumrecv;
		int checksum = 0;
		int index,flg;
		char data[512] = {0};
		char funcname[128] = {0};

		checksum += util_destring(fd, 2, data);
		util_deint(fd,3, &checksumrecv);

		if (checksum != checksumrecv) {
			util_DiscardMessage();
			DME();
			return -1;
		}

		lssproto_NewPlayer_recv(fd,data);

		util_DiscardMessage();
		return 0;
	}
#endif
#ifdef _NEWSAMENU_WITH_CALLBACK
    case LSSPROTO_NEWSAMENU2_RECV:
        {
            int checksum = 0, checksumrecv;
            int index;
            char data[128];
            char funcname[128];
            memset(data, 0, sizeof(data));
            checksum += util_deint(fd, 2, &index);
            checksum += util_destring(fd, 3, data);
            checksum += util_destring(fd, 4, funcname);
            util_deint(fd, 5, &checksumrecv);

            if (checksum != checksumrecv)
            {
                util_DiscardMessage();
                logHack(fd, HACK_CHECKSUMERROR);
                //DME();
                return -1;
            }

#ifdef _DEBUG_RET_CLI
            printf("[接收]LSSPROTO_NEWSAMENU2_RECV-index:%d,data:%s,func=%s\n", index, data,funcname);
#endif
#ifdef _ABSOLUTE_DEBUG
            snprintf(errordata, sizeof(errordata) - 1, "%d:%s:%s", index, data,funcname);
#endif
            FreeNewSaMenu2(fd, index, data,funcname);
            util_DiscardMessage();
            funflg = 1;
            break;
        }

#endif
    }

    if (funflg > 0)
    {
        Assess_SysEfficacy_sub2(1, 9);
        ASSESS_getSysEfficacy_sub(&TVsec, 9);

        if (TVsec >= TotalTVsec)
        {
            TotalTVsec = TVsec;
            TotalNetFun = func;
        }

        return 0;
    }

    util_DiscardMessage();
    logHack(fd, HACK_NOTDISPATCHED);
    //DME();
    return -1;
}

/*
   servertoclient XYD( int x, int y, int dir );
*/
void lssproto_XYD_send(int fd, int x, int y, int dir)
{
    if (CONNECT_checkfd(fd) == FALSE)
    {
        return;
    }

#ifdef _DEBUG_SEND_CLI
    printf("[发送]LSSPROTO_XYD_SEND-x:%d,y:%d,dir:%d\n", x, y, dir);
#endif
    char buffer[65500];
    int checksum = 0;
    strcpy(buffer, "");
    CONNECT_getCdkey(fd, PersonalKey, 4096);
    strcat(PersonalKey, getRandDefaultKey(fd));
    checksum += util_mkint(fd, buffer, x);
    checksum += util_mkint(fd, buffer, y);
    checksum += util_mkint(fd, buffer, dir);
    util_mkint(fd, buffer, checksum);
    util_SendMesg(fd, LSSPROTO_XYD_SEND, buffer);
}

void lssproto_EV_send(int fd, int seqno, int result)
{
    if (CONNECT_checkfd(fd) == FALSE)
    {
        return;
    }

#ifdef _DEBUG_SEND_CLI
    printf("[发送]LSSPROTO_EV_SEND-seqno:%d,result:%d\n", seqno, result);
#endif
    char buffer[65500];
    int checksum = 0;
    strcpy(buffer, "");
    CONNECT_getCdkey(fd, PersonalKey, 4096);
    strcat(PersonalKey, getRandDefaultKey(fd));
    checksum += util_mkint(fd, buffer, seqno);
    checksum += util_mkint(fd, buffer, result);
    util_mkint(fd, buffer, checksum);
    util_SendMesg(fd, LSSPROTO_EV_SEND, buffer);
}

void lssproto_EN_send(int fd, int result, int field)
{
    if (CONNECT_checkfd(fd) == FALSE)
    {
        return;
    }

#ifdef  _BATTLE_CLI_TIME

    if (result > 0)
    {
        int battleplayertime = FreeBattleTime(CONNECT_getCharaindex(fd), 30);
        lssproto_BATTLETIME_send(fd, battleplayertime);
    }

#endif
#ifdef _DEBUG_SEND_CLI
    printf("[发送]LSSPROTO_EN_SEND-result:%d,field:%d\n", result, field);
#endif
    char buffer[65500];
    int checksum = 0;
    //print(" EN_send ");
    strcpy(buffer, "");
    CONNECT_getCdkey(fd, PersonalKey, 4096);
    strcat(PersonalKey, getRandDefaultKey(fd));
    checksum += util_mkint(fd, buffer, result);
    checksum += util_mkint(fd, buffer, field);
    util_mkint(fd, buffer, checksum);
    util_SendMesg(fd, LSSPROTO_EN_SEND, buffer);
}


void lssproto_RS_send(int fd, char* data)
{
    if (CONNECT_checkfd(fd) == FALSE)
    {
        return;
    }

#ifdef _DEBUG_SEND_CLI
    printf("[发送]LSSPROTO_RS_SEND-data:%s\n", data);
#endif
    char buffer[65500];
    int checksum = 0;
    strcpy(buffer, "");
    CONNECT_getCdkey(fd, PersonalKey, 4096);
    strcat(PersonalKey, getRandDefaultKey(fd));
    checksum += util_mkstring(fd, buffer, data);
    util_mkint(fd, buffer, checksum);
    util_SendMesg(fd, LSSPROTO_RS_SEND, buffer);
}


void lssproto_RD_send(int fd, char* data)
{
    if (CONNECT_checkfd(fd) == FALSE)
    {
        return;
    }

#ifdef _DEBUG_SEND_CLI
    printf("[发送]LSSPROTO_RD_SEND-data:%s\n", data);
#endif
    char buffer[65500];
    int checksum = 0;
    strcpy(buffer, "");
    CONNECT_getCdkey(fd, PersonalKey, 4096);
    strcat(PersonalKey, getRandDefaultKey(fd));
    checksum += util_mkstring(fd, buffer, data);
    util_mkint(fd, buffer, checksum);
    util_SendMesg(fd, LSSPROTO_RD_SEND, buffer);
}

void lssproto_B_send(int fd, char* command)
{
    if (CONNECT_checkfd(fd) == FALSE)
    {
        return;
    }

    if (strlen(command) == 0)
    {
        sprintf(command, "FF|");
    }

    char buffer[65500];
#ifdef _DEBUG_SEND_CLI
    printf("[发送]LSSPROTO_B_SEND-command:%s\n", command);
#endif
    int checksum = 0;
    strcpy(buffer, "");
    CONNECT_getCdkey(fd, PersonalKey, 4096);
    strcat(PersonalKey, getRandDefaultKey(fd));
    checksum += util_mkstring(fd, buffer, command);
    util_mkint(fd, buffer, checksum);
    util_SendMesg(fd, LSSPROTO_B_SEND, buffer);
}

void lssproto_I_send(int fd, char* data)
{
    if (CONNECT_checkfd(fd) == FALSE)
    {
        return;
    }

#ifdef _DEBUG_SEND_CLI
    printf("[发送]LSSPROTO_I_SEND-data:%s\n", data);
#endif
    char buffer[65500];
    int checksum = 0;
    strcpy(buffer, "");
    CONNECT_getCdkey(fd, PersonalKey, 4096);
    strcat(PersonalKey, getRandDefaultKey(fd));
    checksum += util_mkstring(fd, buffer, data);
    util_mkint(fd, buffer, checksum);
    util_SendMesg(fd, LSSPROTO_I_SEND, buffer);
#ifdef _UNITY_CLIENT_UPDATE
    lssproto_UClientUpdate_send(fd,1300);
#endif
}

void lssproto_SI_send(int fd, int fromindex, int toindex)
{
    if (CONNECT_checkfd(fd) == FALSE)
    {
        return;
    }

#ifdef _DEBUG_SEND_CLI
    printf("[发送]LSSPROTO_SI_SEND-fromindex:%d,toindex:%d\n", fromindex, toindex);
#endif
    char buffer[65500];
    int checksum = 0;
    strcpy(buffer, "");
    CONNECT_getCdkey(fd, PersonalKey, 4096);
    strcat(PersonalKey, getRandDefaultKey(fd));
    checksum += util_mkint(fd, buffer, fromindex);
    checksum += util_mkint(fd, buffer, toindex);
    util_mkint(fd, buffer, checksum);
    util_SendMesg(fd, LSSPROTO_SI_SEND, buffer);
}

void lssproto_MSG_send(int fd, int aindex, char* text, int color
#ifdef _NEW_STONEAGE_TYPE
                       , int playflag
#ifdef _ITEM_PET_TALK_
                       , char* outpetitemdata
#endif
#ifdef _YAYA_VOICE
                       , int volumeLen
#endif
#endif
                      )
{
    if (CONNECT_checkfd(fd) == FALSE)
    {
        return;
    }

#ifdef _DEBUG_SEND_CLI
    printf("[发送]lssproto_MSG_send-aindex:%d,text:%s,color:%d\n", aindex, text, color);
#endif
    char buffer[65500];
    int checksum = 0;
    strcpy(buffer, "");
    CONNECT_getCdkey(fd, PersonalKey, 4096);
    strcat(PersonalKey, getRandDefaultKey(fd));
    checksum += util_mkint(fd, buffer, aindex);
    checksum += util_mkstring(fd, buffer, text);
    checksum += util_mkint(fd, buffer, color);
#ifdef _NEW_STONEAGE_TYPE
    checksum += util_mkint(fd, buffer, playflag);
#ifdef _ITEM_PET_TALK_
    checksum += util_mkstring(fd, buffer, outpetitemdata);
#endif
#ifdef _YAYA_VOICE
    checksum += util_mkint(fd, buffer, volumeLen);
#endif
#endif
    util_mkint(fd, buffer, checksum);
    util_SendMesg(fd, LSSPROTO_MSG_SEND, buffer);
}


void lssproto_PME_send(int fd, int objindex, int graphicsno, int x, int y, int dir, int flg, int no, char* cdata)
{
    if (CONNECT_checkfd(fd) == FALSE)
    {
        return;
    }

#ifdef _DEBUG_SEND_CLI
    printf("[发送]LSSPROTO_PME_SEND-objindex:%d,graphicsno:%d,x:%d,y:%d,dir:%d,flg:%d,no:%d,cdata:%s\n", objindex, graphicsno, x, y, dir, flg, no, cdata);
#endif
    char buffer[65500];
    int checksum = 0;
    strcpy(buffer, "");
    CONNECT_getCdkey(fd, PersonalKey, 4096);
    strcat(PersonalKey, getRandDefaultKey(fd));
    checksum += util_mkint(fd, buffer, objindex);
    checksum += util_mkint(fd, buffer, graphicsno);
    checksum += util_mkint(fd, buffer, x);
    checksum += util_mkint(fd, buffer, y);
    checksum += util_mkint(fd, buffer, dir);
    checksum += util_mkint(fd, buffer, flg);
    checksum += util_mkint(fd, buffer, no);
    checksum += util_mkstring(fd, buffer, cdata);
    util_mkint(fd, buffer, checksum);
    util_SendMesg(fd, LSSPROTO_PME_SEND, buffer);
}


void lssproto_AB_send(int fd, char* data)
{
    if (CONNECT_checkfd(fd) == FALSE)
    {
        return;
    }

#ifdef _DEBUG_SEND_CLI
    printf("[发送]LSSPROTO_AB_SEND-data:%s\n", data);
#endif
    char buffer[65500];
    int checksum = 0;
    strcpy(buffer, "");
    CONNECT_getCdkey(fd, PersonalKey, 4096);
    strcat(PersonalKey, getRandDefaultKey(fd));
    checksum += util_mkstring(fd, buffer, data);
    util_mkint(fd, buffer, checksum);
    util_SendMesg(fd, LSSPROTO_AB_SEND, buffer);
}


void lssproto_ABI_send(int fd, int num, char* data)
{
    if (CONNECT_checkfd(fd) == FALSE)
    {
        return;
    }

#ifdef _DEBUG_SEND_CLI
    printf("[发送]LSSPROTO_ABI_SEND-num:%d,data:%s\n", num, data);
#endif
    char buffer[65500];
    int checksum = 0;
    strcpy(buffer, "");
    CONNECT_getCdkey(fd, PersonalKey, 4096);
    strcat(PersonalKey, getRandDefaultKey(fd));
    checksum += util_mkint(fd, buffer, num);
    checksum += util_mkstring(fd, buffer, data);
    util_mkint(fd, buffer, checksum);
    util_SendMesg(fd, LSSPROTO_ABI_SEND, buffer);
}

void lssproto_TK_send(int fd, int index, char* message, int color
#ifdef _ITEM_PET_TALK_
                      , char *strData
#endif
#ifdef  _PLAYER_UID
                      , char* uid
#endif
#ifdef _YAYA_VOICE
                      , int volumeLen
#endif
#ifdef _TK_PLAYERFACE
                      , int faceimage
                      , int level
#endif // _TK_PLAYERFACE

                     )
{
    if (CONNECT_checkfd(fd) == FALSE)
    {
        return;
    }

#ifdef _DEBUG_SEND_CLI
    printf("[发送]LSSPROTO_TK_SEND-index:%d,message:%s,color:%d\n", index, message, color);
#endif
    char buffer[65500];
    int checksum = 0;
    strcpy(buffer, "");
    CONNECT_getCdkey(fd, PersonalKey, 4096);
    strcat(PersonalKey, getRandDefaultKey(fd));
    checksum += util_mkint(fd, buffer, index);
    checksum += util_mkstring(fd, buffer, message);
    checksum += util_mkint(fd, buffer, color);
#ifdef _ITEM_PET_TALK_
    checksum += util_mkstring(fd, buffer, strData);
#endif
#ifdef  _PLAYER_UID
    checksum += util_mkstring(fd, buffer, uid);
#endif
#ifdef _YAYA_VOICE
    int charaindex = CONNECT_getCharaindex(fd);

    if (CHAR_CHECKINDEX(charaindex))
    {
        if (CHAR_getWorkInt(charaindex, CHAR_WORKYAYAVOICE) == 0)
        {
            checksum += util_mkint(fd, buffer, volumeLen);
        }
    }

#endif // _YAYA_VOICE
#ifdef _TK_PLAYERFACE
    checksum += util_mkint(fd, buffer, faceimage);
    checksum += util_mkint(fd, buffer, level);
#endif // _TK_PLAYERFACE
    util_mkint(fd, buffer, checksum);
    util_SendMesg(fd, LSSPROTO_TK_SEND, buffer);
}

void lssproto_MC_send(int fd, int fl, int x1, int y1, int x2, int y2, int tilesum, int objsum, int eventsum, char* data)
{
    if (CONNECT_checkfd(fd) == FALSE)
    {
        return;
    }

#ifdef _DEBUG_SEND_CLI
    printf("[发送]LSSPROTO_MC_SEND-fl:%d,x1:%d,y1:%d,x2:%d,y2:%d,tilesum:%d,objsum:%d,eventsum:%d,data:%s\n", fl, x1, y1, x2, y2, tilesum, objsum, eventsum, data);
#endif
    char buffer[65500];
    int checksum = 0;
    strcpy(buffer, "");
    CONNECT_getCdkey(fd, PersonalKey, 4096);
    strcat(PersonalKey, getRandDefaultKey(fd));
    checksum += util_mkint(fd, buffer, fl);
    checksum += util_mkint(fd, buffer, x1);
    checksum += util_mkint(fd, buffer, y1);
    checksum += util_mkint(fd, buffer, x2);
    checksum += util_mkint(fd, buffer, y2);
    checksum += util_mkint(fd, buffer, tilesum);
    checksum += util_mkint(fd, buffer, objsum);
    checksum += util_mkint(fd, buffer, eventsum);
    checksum += util_mkstring(fd, buffer, data);
    util_mkint(fd, buffer, checksum);
    util_SendMesg(fd, LSSPROTO_MC_SEND, buffer);
}


void lssproto_M_send(int fd, int fl, int x1, int y1, int x2, int y2, char* data)
{
    if (CONNECT_checkfd(fd) == FALSE)
    {
        return;
    }

#ifdef _DEBUG_SEND_CLI
    printf("[发送]LSSPROTO_M_SEND-fl:%d,x1:%d,y1:%d,x2:%d,y2:%d,data:%s\n", fl, x1, y1, x2, y2, data);
#endif
    char buffer[65500];
    int checksum = 0;
    strcpy(buffer, "");
    CONNECT_getCdkey(fd, PersonalKey, 4096);
    strcat(PersonalKey, getRandDefaultKey(fd));
    checksum += util_mkint(fd, buffer, fl);
    checksum += util_mkint(fd, buffer, x1);
    checksum += util_mkint(fd, buffer, y1);
    checksum += util_mkint(fd, buffer, x2);
    checksum += util_mkint(fd, buffer, y2);
    checksum += util_mkstring(fd, buffer, data);
    util_mkint(fd, buffer, checksum);
    util_SendMesg(fd, LSSPROTO_M_SEND, buffer);
}

void lssproto_C_send(int fd, char* data)
{
    if (CONNECT_checkfd(fd) == FALSE)
    {
        return;
    }

#ifdef  _WAN_FIX

    if (getVsFlg() == 2)
    {
        int charaindex = -1;
        charaindex = CONNECT_getCharaindex(fd);

        if (CHAR_getInt(charaindex, CHAR_WHICHTYPE) == CHAR_TYPEPLAYER)
        {
            char sendmsg[20];
            sprintf(sendmsg, "%d|%d", CHAR_getInt(charaindex, CHAR_X), CHAR_getInt(charaindex, CHAR_Y));

            if (getStayEncount(fd) == 1 && strstr(data, sendmsg) == NULL && CHAR_getWorkInt(charaindex, CHAR_WORKPARTYMODE) == CHAR_PARTY_NONE)
            {
                return;
            }
        }
    }
    else if (getVsFlg() == 1)
    {
        int charaindex = -1;
        charaindex = CONNECT_getCharaindex(fd);

        if (CHAR_getInt(charaindex, CHAR_WHICHTYPE) == CHAR_TYPEPLAYER)
        {
            char sendmsg[20];
            sprintf(sendmsg, "%d|%d", CHAR_getInt(charaindex, CHAR_X), CHAR_getInt(charaindex, CHAR_Y));

            if (getStayEncount(fd) == 1 && strstr(data, sendmsg) == NULL)
            {
                return;
            }

            /*if(getStayEncount(fd)==1 && strstr(data,sendmsg)==NULL && CHAR_getWorkInt( charaindex, CHAR_WORKPARTYMODE) != CHAR_PARTY_LEADER)
                return;
            if(CHAR_getWorkInt( charaindex, CHAR_WORKPARTYMODE) == CHAR_PARTY_CLIENT){
                int partyindex = CHAR_getWorkInt( charaindex, CHAR_WORKPARTYINDEX1);
                if(CHAR_CHECKINDEX(partyindex)){
                    int partyfd = getfdFromCharaIndex( CHAR_getWorkInt( charaindex, CHAR_WORKPARTYINDEX1));
                    if(getStayEncount(partyfd)==1)
                        return;
                }
            }*/
        }
    }
    else if (getVsFlg() == 3)
    {
        int charaindex = -1;
        charaindex = CONNECT_getCharaindex(fd);

        if (CHAR_getInt(charaindex, CHAR_WHICHTYPE) == CHAR_TYPEPLAYER)
        {
            char sendmsg[20];
            sprintf(sendmsg, "%d|%d", CHAR_getInt(charaindex, CHAR_X), CHAR_getInt(charaindex, CHAR_Y));

            if (CHAR_getWorkInt(charaindex, CHAR_WORKPARTYMODE) == CHAR_PARTY_CLIENT)
            {
                int partyindex = CHAR_getWorkInt(charaindex, CHAR_WORKPARTYINDEX1);

                if (CHAR_CHECKINDEX(partyindex))
                {
                    int partyfd = getfdFromCharaIndex(CHAR_getWorkInt(charaindex, CHAR_WORKPARTYINDEX1));

                    if (getStayEncount(partyfd) == 1)
                    {
                        return;
                    }
                }
            }
        }
    }

#endif
#ifdef _DEBUG_SEND_CLI
    printf("[发送]LSSPROTO_C_SEND-data:%s\n", data);
#endif
    char buffer[65500];
    int checksum = 0;
    strcpy(buffer, "");
    CONNECT_getCdkey(fd, PersonalKey, 4096);
    strcat(PersonalKey, getRandDefaultKey(fd));
    checksum += util_mkstring(fd, buffer, data);
    util_mkint(fd, buffer, checksum);
    util_SendMesg(fd, LSSPROTO_C_SEND, buffer);
}

void lssproto_CA_send(int fd, char* data)
{
    if (CONNECT_checkfd(fd) == FALSE)
    {
        return;
    }

#ifdef  _WAN_FIX

    if (getVsFlg() == 2)
    {
        int charaindex = -1;
        charaindex = CONNECT_getCharaindex(fd);

        if (CHAR_getInt(charaindex, CHAR_WHICHTYPE) == CHAR_TYPEPLAYER)
        {
            char sendmsg[20];
            sprintf(sendmsg, "%d|%d", CHAR_getInt(charaindex, CHAR_X), CHAR_getInt(charaindex, CHAR_Y));

            if (getStayEncount(fd) == 1 && strstr(data, sendmsg) == NULL && CHAR_getWorkInt(charaindex, CHAR_WORKPARTYMODE) == CHAR_PARTY_NONE)
            {
                return;
            }
        }
    }
    else if (getVsFlg() == 1)
    {
        int charaindex = -1;
        charaindex = CONNECT_getCharaindex(fd);

        if (CHAR_getInt(charaindex, CHAR_WHICHTYPE) == CHAR_TYPEPLAYER)
        {
            char sendmsg[20];
            sprintf(sendmsg, "%d|%d", CHAR_getInt(charaindex, CHAR_X), CHAR_getInt(charaindex, CHAR_Y));

            if (getStayEncount(fd) == 1 && strstr(data, sendmsg) == NULL)
            {
                return;
            }

            /*if(getStayEncount(fd)==1 && strstr(data,sendmsg)==NULL && CHAR_getWorkInt( charaindex, CHAR_WORKPARTYMODE) != CHAR_PARTY_LEADER)
                return;
            if(CHAR_getWorkInt( charaindex, CHAR_WORKPARTYMODE) == CHAR_PARTY_CLIENT){
                int partyindex = CHAR_getWorkInt( charaindex, CHAR_WORKPARTYINDEX1);
                if(CHAR_CHECKINDEX(partyindex)){
                    int partyfd = getfdFromCharaIndex( CHAR_getWorkInt( charaindex, CHAR_WORKPARTYINDEX1));
                    if(getStayEncount(partyfd)==1)
                        return;
                }
            }*/
        }
    }
    else if (getVsFlg() == 3)
    {
        int charaindex = -1;
        charaindex = CONNECT_getCharaindex(fd);

        if (CHAR_getInt(charaindex, CHAR_WHICHTYPE) == CHAR_TYPEPLAYER)
        {
            char sendmsg[20];
            sprintf(sendmsg, "%d|%d", CHAR_getInt(charaindex, CHAR_X), CHAR_getInt(charaindex, CHAR_Y));

            if (CHAR_getWorkInt(charaindex, CHAR_WORKPARTYMODE) == CHAR_PARTY_CLIENT)
            {
                int partyindex = CHAR_getWorkInt(charaindex, CHAR_WORKPARTYINDEX1);

                if (CHAR_CHECKINDEX(partyindex))
                {
                    int partyfd = getfdFromCharaIndex(CHAR_getWorkInt(charaindex, CHAR_WORKPARTYINDEX1));

                    if (getStayEncount(partyfd) == 1)
                    {
                        return;
                    }
                }
            }
        }
    }

#endif
#ifdef _DEBUG_SEND_CLI
    printf("[发送]LSSPROTO_CA_SEND-data:%s\n", data);
#endif
    char buffer[65500];
    int checksum = 0;
    strcpy(buffer, "");
    CONNECT_getCdkey(fd, PersonalKey, 4096);
    strcat(PersonalKey, getRandDefaultKey(fd));
    checksum += util_mkstring(fd, buffer, data);
    util_mkint(fd, buffer, checksum);
    util_SendMesg(fd, LSSPROTO_CA_SEND, buffer);
}

void lssproto_CD_send(int fd, char* data)
{
    if (CONNECT_checkfd(fd) == FALSE)
    {
        return;
    }

#ifdef _DEBUG_SEND_CLI
    printf("[发送]LSSPROTO_CD_SEND-data:%s\n", data);
#endif
    char buffer[65500];
    int checksum = 0;
    strcpy(buffer, "");
    CONNECT_getCdkey(fd, PersonalKey, 4096);
    strcat(PersonalKey, getRandDefaultKey(fd));
    checksum += util_mkstring(fd, buffer, data);
    util_mkint(fd, buffer, checksum);
    util_SendMesg(fd, LSSPROTO_CD_SEND, buffer);
}

void lssproto_R_send(int fd, char* data)
{
    if (CONNECT_checkfd(fd) == FALSE)
    {
        return;
    }

#ifdef _DEBUG_SEND_CLI
    printf("[发送]LSSPROTO_R_SEND-data:%s\n", data);
#endif
    char buffer[65500];
    int checksum = 0;
    strcpy(buffer, "");
    CONNECT_getCdkey(fd, PersonalKey, 4096);
    strcat(PersonalKey, getRandDefaultKey(fd));
    checksum += util_mkstring(fd, buffer, data);
    util_mkint(fd, buffer, checksum);
    util_SendMesg(fd, LSSPROTO_R_SEND, buffer);
}

void lssproto_S_send(int fd, char* data)
{
    if (CONNECT_checkfd(fd) == FALSE)
    {
        return;
    }

#ifdef _DEBUG_SEND_CLI
    printf("[发送]LSSPROTO_S_SEND-data:%s\n", data);
#endif
    char buffer[65500];
    int checksum = 0;
    strcpy(buffer, "");
    CONNECT_getCdkey(fd, PersonalKey, 4096);
    strcat(PersonalKey, getRandDefaultKey(fd));
    checksum += util_mkstring(fd, buffer, data);
    util_mkint(fd, buffer, checksum);
    util_SendMesg(fd, LSSPROTO_S_SEND, buffer);
}

void lssproto_D_send(int fd, int category, int dx, int dy, char* data)
{
    if (CONNECT_checkfd(fd) == FALSE)
    {
        return;
    }

#ifdef _DEBUG_SEND_CLI
    printf("[发送]LSSPROTO_D_SEND-category:%d,dx:%d,dy:%d,data:%s\n", category, dx, dy, data);
#endif
    char buffer[65500];
    int checksum = 0;
    strcpy(buffer, "");
    CONNECT_getCdkey(fd, PersonalKey, 4096);
    strcat(PersonalKey, getRandDefaultKey(fd));
    checksum += util_mkint(fd, buffer, category);
    checksum += util_mkint(fd, buffer, dx);
    checksum += util_mkint(fd, buffer, dy);
    checksum += util_mkstring(fd, buffer, data);
    util_mkint(fd, buffer, checksum);
    util_SendMesg(fd, LSSPROTO_D_SEND, buffer);
}

void lssproto_FS_send(int fd, int flg)
{
    if (CONNECT_checkfd(fd) == FALSE)
    {
        return;
    }

#ifdef _DEBUG_SEND_CLI
    printf("[发送]LSSPROTO_FS_SEND-flg:%d\n", flg);
#endif
    char buffer[65500];
    int checksum = 0;
    strcpy(buffer, "");
    CONNECT_getCdkey(fd, PersonalKey, 4096);
    strcat(PersonalKey, getRandDefaultKey(fd));
    checksum += util_mkint(fd, buffer, flg);
    util_mkint(fd, buffer, checksum);
    util_SendMesg(fd, LSSPROTO_FS_SEND, buffer);
}

void lssproto_HL_send(int fd, int flg)
{
    if (CONNECT_checkfd(fd) == FALSE)
    {
        return;
    }

#ifdef _DEBUG_SEND_CLI
    printf("[发送]LSSPROTO_HL_SEND-flg:%d\n", flg);
#endif
    char buffer[65500];
    int checksum = 0;
    strcpy(buffer, "");
    CONNECT_getCdkey(fd, PersonalKey, 4096);
    strcat(PersonalKey, getRandDefaultKey(fd));
    checksum += util_mkint(fd, buffer, flg);
    util_mkint(fd, buffer, checksum);
    util_SendMesg(fd, LSSPROTO_HL_SEND, buffer);
}

void lssproto_PR_send(int fd, int request, int result)
{
    if (CONNECT_checkfd(fd) == FALSE)
    {
        return;
    }

#ifdef _DEBUG_SEND_CLI
    printf("[发送]LSSPROTO_PR_SEND-request:%d,result:%d\n", request, result);
#endif
    char buffer[65500];
    int checksum = 0;
    strcpy(buffer, "");
    CONNECT_getCdkey(fd, PersonalKey, 4096);
    strcat(PersonalKey, getRandDefaultKey(fd));
    checksum += util_mkint(fd, buffer, request);
    checksum += util_mkint(fd, buffer, result);
    util_mkint(fd, buffer, checksum);
    util_SendMesg(fd, LSSPROTO_PR_SEND, buffer);
}

#ifdef _PETS_SELECTCON
void lssproto_PETS_send(int fd, int petarray, int result)
{
    if (CONNECT_checkfd(fd) == FALSE)
    {
        return;
    }

#ifdef _DEBUG_SEND_CLI
    printf("[发送]LSSPROTO_PETST_SEND-petarray:%d,result:%d\n", petarray, result);
#endif
    char buffer[65500];
    int checksum = 0;
    strcpy(buffer, "");
    CONNECT_getCdkey(fd, PersonalKey, 4096);
    strcat(PersonalKey, getRandDefaultKey(fd));
    checksum += util_mkint(fd, buffer, petarray);
    checksum += util_mkint(fd, buffer, result);
    util_mkint(fd, buffer, checksum);
    util_SendMesg(fd, LSSPROTO_PETST_SEND, buffer);
}
#endif

void lssproto_KS_send(int fd, int petarray, int result)
{
    if (CONNECT_checkfd(fd) == FALSE)
    {
        return;
    }

#ifdef _DEBUG_SEND_CLI
    printf("[发送]LSSPROTO_KS_SEND-petarray:%d,result:%d\n", petarray, result);
#endif
    char buffer[65500];
    int checksum = 0;
    strcpy(buffer, "");
    CONNECT_getCdkey(fd, PersonalKey, 4096);
    strcat(PersonalKey, getRandDefaultKey(fd));
    checksum += util_mkint(fd, buffer, petarray);
    checksum += util_mkint(fd, buffer, result);
    util_mkint(fd, buffer, checksum);
    util_SendMesg(fd, LSSPROTO_KS_SEND, buffer);
}

void lssproto_SPET_send(int fd, int standbypet, int result)
{
    if (CONNECT_checkfd(fd) == FALSE)
    {
        return;
    }

#ifdef _DEBUG_SEND_CLI
    printf("[发送]LSSPROTO_SPET_SEND-standbypet:%d,result:%d\n", standbypet, result);
#endif
    char buffer[65500];
    int checksum = 0;
    strcpy(buffer, "");
    CONNECT_getCdkey(fd, PersonalKey, 4096);
    strcat(PersonalKey, getRandDefaultKey(fd));
    checksum += util_mkint(fd, buffer, standbypet);
    checksum += util_mkint(fd, buffer, result);
    util_mkint(fd, buffer, checksum);
    util_SendMesg(fd, LSSPROTO_SPET_SEND, buffer);
}

void lssproto_PS_send(int fd, int result, int havepetindex, int havepetskill, int toindex)
{
    if (CONNECT_checkfd(fd) == FALSE)
    {
        return;
    }

#ifdef _DEBUG_SEND_CLI
    printf("[发送]LSSPROTO_PS_SEND-result:%d,havepetindex:%d,havepetskill:%d,toindex:%d\n", result, havepetindex, havepetskill, toindex);
#endif
    char buffer[65500];
    int checksum = 0;
    strcpy(buffer, "");
    CONNECT_getCdkey(fd, PersonalKey, 4096);
    strcat(PersonalKey, getRandDefaultKey(fd));
    checksum += util_mkint(fd, buffer, result);
    checksum += util_mkint(fd, buffer, havepetindex);
    checksum += util_mkint(fd, buffer, havepetskill);
    checksum += util_mkint(fd, buffer, toindex);
    util_mkint(fd, buffer, checksum);
    util_SendMesg(fd, LSSPROTO_PS_SEND, buffer);
}

void lssproto_SKUP_send(int fd, int point)
{
    if (CONNECT_checkfd(fd) == FALSE)
    {
        return;
    }

#ifdef _DEBUG_SEND_CLI
    printf("[发送]LSSPROTO_SKUP_SEND-point:%d\n", point);
#endif
    char buffer[65500];
    int checksum = 0;
    strcpy(buffer, "");
    CONNECT_getCdkey(fd, PersonalKey, 4096);
    strcat(PersonalKey, getRandDefaultKey(fd));
    checksum += util_mkint(fd, buffer, point);
    util_mkint(fd, buffer, checksum);
    util_SendMesg(fd, LSSPROTO_SKUP_SEND, buffer);
}
void lssproto_WN_send(int fd, int windowtype, int buttontype, int seqno, int objindex, char* data)
{
    char buffer[65500];
#ifdef _DEBUG_SEND_CLI
    printf("[发送]LSSPROTO_WN_SEND-windowtype:%d,buttontype:%d,seqno:%d,objindex:%d,data:%s\n", windowtype, buttontype, seqno, objindex, data);
#endif
    int checksum = 0;
    strcpy(buffer, "");
    CONNECT_getCdkey(fd, PersonalKey, 4096);
    strcat(PersonalKey, getRandDefaultKey(fd));
    checksum += util_mkint(fd, buffer, windowtype);
    checksum += util_mkint(fd, buffer, buttontype);
    checksum += util_mkint(fd, buffer, seqno);
    checksum += util_mkint(fd, buffer, objindex);
    checksum += util_mkstring(fd, buffer, data);
    util_mkint(fd, buffer, checksum);
    util_SendMesg(fd, LSSPROTO_WN_SEND, buffer);
}

void lssproto_EF_send(int fd, int effect, int level, char* option)
{
    if (CONNECT_checkfd(fd) == FALSE)
    {
        return;
    }

#ifdef _DEBUG_SEND_CLI
    printf("[发送]LSSPROTO_EF_SEND-effect:%d,level:%d,char:%s\n", effect, level, option);
#endif
    char buffer[65500];
    int checksum = 0;
    strcpy(buffer, "");
    CONNECT_getCdkey(fd, PersonalKey, 4096);
    strcat(PersonalKey, getRandDefaultKey(fd));
    checksum += util_mkint(fd, buffer, effect);
    checksum += util_mkint(fd, buffer, level);
    checksum += util_mkstring(fd, buffer, option);
    util_mkint(fd, buffer, checksum);
    util_SendMesg(fd, LSSPROTO_EF_SEND, buffer);
}
void lssproto_SE_send(int fd, int x, int y, int senumber, int sw)
{
    if (CONNECT_checkfd(fd) == FALSE)
    {
        return;
    }

#ifdef _DEBUG_SEND_CLI
    printf("[发送]LSSPROTO_SE_SEND-x:%d,y:%d,senumber:%d,sw:%d\n", x, y, senumber, sw);
#endif
    char buffer[65500];
    int checksum = 0;
    strcpy(buffer, "");
    CONNECT_getCdkey(fd, PersonalKey, 4096);
    strcat(PersonalKey, getRandDefaultKey(fd));
    checksum += util_mkint(fd, buffer, x);
    checksum += util_mkint(fd, buffer, y);
    checksum += util_mkint(fd, buffer, senumber);
    checksum += util_mkint(fd, buffer, sw);
    util_mkint(fd, buffer, checksum);
    util_SendMesg(fd, LSSPROTO_SE_SEND, buffer);
}

void lssproto_ClientLogin_send(int fd, char* result)
{
    if (CONNECT_checkfd(fd) == FALSE)
    {
        return;
    }

#ifdef _DEBUG_SEND_CLI
    printf("[发送]LSSPROTO_CLIENTLOGIN_SEND-result:%s\n", result);
#endif
    char buffer[65500];
    int checksum = 0;
    strcpy(buffer, "");
    CONNECT_getCdkey(fd, PersonalKey, 4096);
    strcat(PersonalKey, getRandDefaultKey(fd));
    checksum += util_mkstring(fd, buffer, result);
    util_mkint(fd, buffer, checksum);
    util_SendMesg(fd, LSSPROTO_CLIENTLOGIN_SEND, buffer);
}

void lssproto_CreateNewChar_send(int fd, char* result, char* data)
{
    if (CONNECT_checkfd(fd) == FALSE)
    {
        return;
    }

#ifdef _DEBUG_SEND_CLI
    printf("[发送]LSSPROTO_CREATENEWCHAR_SEND-result:%s,data:%s\n", result, data);
#endif
    char buffer[65500];
    int checksum = 0;
    strcpy(buffer, "");
    CONNECT_getCdkey(fd, PersonalKey, 4096);
    strcat(PersonalKey, getRandDefaultKey(fd));
    checksum += util_mkstring(fd, buffer, result);
    checksum += util_mkstring(fd, buffer, data);
    util_mkint(fd, buffer, checksum);
    util_SendMesg(fd, LSSPROTO_CREATENEWCHAR_SEND, buffer);
}

void lssproto_CharDelete_send(int fd, char* result, char* data)
{
    if (CONNECT_checkfd(fd) == FALSE)
    {
        return;
    }

#ifdef _DEBUG_SEND_CLI
    printf("[发送]LSSPROTO_CHARDELETE_SEND-result:%s,data:%s\n", result, data);
#endif
    char buffer[65500];
    int checksum = 0;
    strcpy(buffer, "");
    CONNECT_getCdkey(fd, PersonalKey, 4096);
    strcat(PersonalKey, getRandDefaultKey(fd));
    checksum += util_mkstring(fd, buffer, result);
    checksum += util_mkstring(fd, buffer, data);
    util_mkint(fd, buffer, checksum);
    util_SendMesg(fd, LSSPROTO_CHARDELETE_SEND, buffer);
}

void lssproto_CharLogin_send(int fd, char* result, char* data)
{
    if (CONNECT_checkfd(fd) == FALSE)
    {
        return;
    }

#ifdef _DEBUG_SEND_CLI
    printf("[发送]LSSPROTO_CHARLOGIN_SEND-result:%s,data:%s\n", result, data);
#endif
    char buffer[65500];
    int checksum = 0;
    strcpy(buffer, "");
    CONNECT_getCdkey(fd, PersonalKey, 4096);
    strcat(PersonalKey, getRandDefaultKey(fd));
    checksum += util_mkstring(fd, buffer, result);
    checksum += util_mkstring(fd, buffer, data);
    util_mkint(fd, buffer, checksum);
    util_SendMesg(fd, LSSPROTO_CHARLOGIN_SEND, buffer);
}

void lssproto_CharList_send(int fd, char* result, char* data)
{
    if (CONNECT_checkfd(fd) == FALSE)
    {
        return;
    }

#ifdef _DEBUG_SEND_CLI
    printf("[发送]LSSPROTO_CHARLIST_SEND-result:%s,data:%s\n", result, data);
#endif
    char buffer[65500];
    int checksum = 0;
    strcpy(buffer, "");
    CONNECT_getCdkey(fd, PersonalKey, 4096);
    strcat(PersonalKey, getRandDefaultKey(fd));
    checksum += util_mkstring(fd, buffer, result);
    checksum += util_mkstring(fd, buffer, data);
    util_mkint(fd, buffer, checksum);
    util_SendMesg(fd, LSSPROTO_CHARLIST_SEND, buffer);
#ifdef _NEW_NETSENDRECV
    FreeCharList(fd, result, data);
#endif
}

void lssproto_CharLogout_send(int fd, char* result, char* data)
{
    if (CONNECT_checkfd(fd) == FALSE)
    {
        return;
    }

#ifdef _DEBUG_SEND_CLI
    printf("[发送]LSSPROTO_CHARLOGOUT_SEND-result:%s,data:%s\n", result, data);
#endif
    char buffer[65500];
    int checksum = 0;
    strcpy(buffer, "");
    CONNECT_getCdkey(fd, PersonalKey, 4096);
    strcat(PersonalKey, getRandDefaultKey(fd));
    checksum += util_mkstring(fd, buffer, result);
    checksum += util_mkstring(fd, buffer, data);
    util_mkint(fd, buffer, checksum);
    util_SendMesg(fd, LSSPROTO_CHARLOGOUT_SEND, buffer);
}

void lssproto_ProcGet_send(int fd, char* data)
{
    if (CONNECT_checkfd(fd) == FALSE)
    {
        return;
    }

#ifdef _DEBUG_SEND_CLI
    printf("[发送]LSSPROTO_PROCGET_SEND-data:%s\n", data);
#endif
    char buffer[65500];
    int checksum = 0;
    strcpy(buffer, "");
    CONNECT_getCdkey(fd, PersonalKey, 4096);
    strcat(PersonalKey, getRandDefaultKey(fd));
    checksum += util_mkstring(fd, buffer, data);
    util_mkint(fd, buffer, checksum);
    util_SendMesg(fd, LSSPROTO_PROCGET_SEND, buffer);
}

void lssproto_PlayerNumGet_send(int fd, int logincount, int player)
{
    if (CONNECT_checkfd(fd) == FALSE)
    {
        return;
    }

#ifdef _DEBUG_SEND_CLI
    printf("[发送]LSSPROTO_PLAYERNUMGET_SEND-logincount:%d,player:%d\n", logincount, player);
#endif
    char buffer[65500];
    int checksum = 0;
    strcpy(buffer, "");
    CONNECT_getCdkey(fd, PersonalKey, 4096);
    strcat(PersonalKey, getRandDefaultKey(fd));
    checksum += util_mkint(fd, buffer, logincount);
    checksum += util_mkint(fd, buffer, player);
    util_mkint(fd, buffer, checksum);
    util_SendMesg(fd, LSSPROTO_PLAYERNUMGET_SEND, buffer);
}

void lssproto_Echo_send(int fd, char* test)
{
    if (CONNECT_checkfd(fd) == FALSE)
    {
        return;
    }

#ifdef _DEBUG_SEND_CLI
    printf("[发送]LSSPROTO_ECHO_SEND-test:%s\n", test);
#endif
    char buffer[65500];
    int checksum = 0;
    strcpy(buffer, "");
    CONNECT_getCdkey(fd, PersonalKey, 4096);
    strcat(PersonalKey, getRandDefaultKey(fd));
    checksum += util_mkstring(fd, buffer, test);
    util_mkint(fd, buffer, checksum);
    util_SendMesg(fd, LSSPROTO_ECHO_SEND, buffer);
}

// CoolFish: Trade 2001/4/18

void lssproto_TD_send(int fd, char* message)
{
    if (CONNECT_checkfd(fd) == FALSE)
    {
        return;
    }

#ifdef _DEBUG_SEND_CLI
    printf("[发送]LSSPROTO_TD_SEND-message:%s\n", message);
#endif
    char buffer[65500];
    int checksum = 0;
    strcpy(buffer, "");
    CONNECT_getCdkey(fd, PersonalKey, 4096);
    strcat(PersonalKey, getRandDefaultKey(fd));
    checksum += util_mkstring(fd, buffer, message);
    util_mkint(fd, buffer, checksum);
    util_SendMesg(fd, LSSPROTO_TD_SEND, buffer);
}
#ifdef _CHATROOMPROTOCOL            // (不可开) Syu ADD 聊天室频道
void lssproto_CHATROOM_send(int fd, char* message)
{
    if (CONNECT_checkfd(fd) == FALSE)
    {
        return;
    }

#ifdef _DEBUG_SEND_CLI
    printf("[发送]LSSPROTO_CHATROOM_SEND-message:%s\n", message);
#endif
    char buffer[65500];
    int checksum = 0;
    strcpy(buffer, "");
    CONNECT_getCdkey(fd, PersonalKey, 4096);
    strcat(PersonalKey, getRandDefaultKey(fd));
    checksum += util_mkstring(fd, buffer, message);
    util_mkint(fd, buffer, checksum);
    util_SendMesg(fd, LSSPROTO_CHATROOM_SEND, buffer);
}
#endif
#ifdef _NEWREQUESTPROTOCOL          // (不可开) Syu ADD 新增Protocol要求细项
void lssproto_RESIST_send(int fd, char* message)
{
    if (CONNECT_checkfd(fd) == FALSE)
    {
        return;
    }

#ifdef _DEBUG_SEND_CLI
    printf("[发送]LSSPROTO_RESIST_SEND-message:%s\n", message);
#endif
    char buffer[65500];
    int checksum = 0;
    strcpy(buffer, "");
    CONNECT_getCdkey(fd, PersonalKey, 4096);
    strcat(PersonalKey, getRandDefaultKey(fd));
    checksum += util_mkstring(fd, buffer, message);
    util_mkint(fd, buffer, checksum);
    util_SendMesg(fd, LSSPROTO_RESIST_SEND, buffer);
}
#endif
#ifdef _OUTOFBATTLESKILL            // (不可开) Syu ADD 非战斗时技能Protocol
void lssproto_BATTLESKILL_send(int fd, char* message)
{
    if (CONNECT_checkfd(fd) == FALSE)
    {
        return;
    }

#ifdef _DEBUG_SEND_CLI
    printf("[发送]LSSPROTO_BATTLESKILL_SEND-message:%s\n", message);
#endif
    char buffer[65500];
    int checksum = 0;
    strcpy(buffer, "");
    CONNECT_getCdkey(fd, PersonalKey, 4096);
    strcat(PersonalKey, getRandDefaultKey(fd));
    checksum += util_mkstring(fd, buffer, message);
    util_mkint(fd, buffer, checksum);
    util_SendMesg(fd, LSSPROTO_BATTLESKILL_SEND, buffer);
}
#endif
void lssproto_NU_send(int fd, int nu)
{
    if (CONNECT_checkfd(fd) == FALSE)
    {
        return;
    }

#ifdef _DEBUG_SEND_CLI
    printf("[发送]LSSPROTO_NU_SEND-nu:%d\n", nu);
#endif
    char buffer[65500];
    int checksum = 0;
    strcpy(buffer, "");
    CONNECT_getCdkey(fd, PersonalKey, 4096);
    strcat(PersonalKey, getRandDefaultKey(fd));
    checksum += util_mkint(fd, buffer, nu);
    util_mkint(fd, buffer, checksum);
    util_SendMesg(fd, LSSPROTO_NU_SEND, buffer);
}


void lssproto_FM_send(int fd, char* message)
{
    if (CONNECT_checkfd(fd) == FALSE)
    {
        return;
    }

#ifdef _DEBUG_SEND_CLI
    printf("[发送]LSSPROTO_FM_SEND-message:%s\n", message);
#endif
    char buffer[65500];
    int checksum = 0;
    strcpy(buffer, "");
    CONNECT_getCdkey(fd, PersonalKey, 4096);
    strcat(PersonalKey, getRandDefaultKey(fd));
    checksum += util_mkstring(fd, buffer, message);
    util_mkint(fd, buffer, checksum);
    util_SendMesg(fd, LSSPROTO_FM_SEND, buffer);
}


void lssproto_WO_send(int fd, int effect)
{
    if (CONNECT_checkfd(fd) == FALSE)
    {
        return;
    }

#ifdef _DEBUG_SEND_CLI
    printf("[发送]LSSPROTO_WO_SEND-effect:%d\n", effect);
#endif
    char buffer[65500];
    int checksum = 0;
    strcpy(buffer, "");
    CONNECT_getCdkey(fd, PersonalKey, 4096);
    strcat(PersonalKey, getRandDefaultKey(fd));
    checksum += util_mkint(fd, buffer, effect);
    util_mkint(fd, buffer, checksum);
    util_SendMesg(fd, LSSPROTO_WO_SEND, buffer);
}
#ifdef _ITEM_CRACKER
void lssproto_IC_send(int fd, int x, int y)
{
    if (CONNECT_checkfd(fd) == FALSE)
    {
        return;
    }

#ifdef _DEBUG_SEND_CLI
    printf("[发送]lssproto_IC_send-x:%d,y:%d\n", x, y);
#endif
    char buffer[65500];
    int checksum = 0;
    strcpy(buffer, "");
    CONNECT_getCdkey(fd, PersonalKey, 4096);
    strcat(PersonalKey, getRandDefaultKey(fd));
    checksum += util_mkint(fd, buffer, x);
    checksum += util_mkint(fd, buffer, y);
    util_mkint(fd, buffer, checksum);
    util_SendMesg(fd, LSSPROTO_IC_SEND, buffer);
}
#endif
#ifdef _MAGIC_NOCAST             // 精灵:沉默
void lssproto_NC_send(int fd, int flg)
{
    if (CONNECT_checkfd(fd) == FALSE)
    {
        return;
    }

#ifdef _DEBUG_SEND_CLI
    printf("[发送]LSSPROTO_NC_SEND-flg:%d\n", flg);
#endif
    char buffer[65500];
    int checksum = 0;
    strcpy(buffer, "");
    CONNECT_getCdkey(fd, PersonalKey, 4096);
    strcat(PersonalKey, getRandDefaultKey(fd));
    checksum += util_mkint(fd, buffer, flg);
    util_mkint(fd, buffer, checksum);
    util_SendMesg(fd, LSSPROTO_NC_SEND, buffer);
}
#endif

#ifdef _STREET_VENDOR
void lssproto_STREET_VENDOR_send(int fd, char *message)
{
    if (CONNECT_checkfd(fd) == FALSE)
    {
        return;
    }

#ifdef _DEBUG_SEND_CLI
    printf("[发送]LSSPROTO_STREET_VENDOR_SEND-message:%s\n", message);
#endif
    char buffer[65500];
    int checksum = 0;
    strcpy(buffer, "");
    CONNECT_getCdkey(fd, PersonalKey, 4096);
    strcat(PersonalKey, getRandDefaultKey(fd));
    checksum += util_mkstring(fd, buffer, message);
    util_mkint(fd, buffer, checksum);
    util_SendMesg(fd, LSSPROTO_STREET_VENDOR_SEND, buffer);
}
#endif

#ifdef _RIGHTCLICK
void lssproto_RCLICK_send(int fd, int type, char* data)
{
    if (CONNECT_checkfd(fd) == FALSE)
    {
        return;
    }

#ifdef _DEBUG_SEND_CLI
    printf("[发送]LSSPROTO_RCLICK_SEND-type:%d,data:%s\n", type, data);
#endif
    char buffer[65500];
    int checksum = 0;
    print("\n RCLICK_send( type=%d data=%s) ", type, data);
    strcpy(buffer, "");
    CONNECT_getCdkey(fd, PersonalKey, 4096);
    strcat(PersonalKey, getRandDefaultKey(fd));
    checksum += util_mkint(fd, buffer, type);
    checksum += util_mkstring(fd, buffer, data);
    util_mkint(fd, buffer, checksum);
    util_SendMesg(fd, LSSPROTO_RCLICK_SEND, buffer);
}
#endif

#ifdef _JOBDAILY
void lssproto_JOBDAILY_send(int fd, char *data)
{
    if (CONNECT_checkfd(fd) == FALSE)
    {
        return;
    }

#ifdef _DEBUG_SEND_CLI
    printf("[发送]LSSPROTO_JOBDAILY_SEND-data:%s\n", data);
#endif
    char buffer[65500];
    int checksum = 0;
    strcpy(buffer, "");
    CONNECT_getCdkey(fd, PersonalKey, 4096);
    strcat(PersonalKey, getRandDefaultKey(fd));
    checksum += util_mkstring(fd, buffer, data);
    util_mkint(fd, buffer, checksum);
    util_SendMesg(fd, LSSPROTO_JOBDAILY_SEND, buffer);
}
#endif

#ifdef _TEACHER_SYSTEM
void lssproto_TEACHER_SYSTEM_send(int fd, char *data)
{
    if (CONNECT_checkfd(fd) == FALSE)
    {
        return;
    }

#ifdef _DEBUG_SEND_CLI
    printf("[发送]LSSPROTO_TEACHER_SYSTEM_SEND-data:%s\n", data);
#endif
    char buffer[65500];
    int checksum = 0;
    strcpy(buffer, "");
    CONNECT_getCdkey(fd, PersonalKey, 4096);
    strcat(PersonalKey, getRandDefaultKey(fd));
    checksum += util_mkstring(fd, buffer, data);
    util_mkint(fd, buffer, checksum);
    util_SendMesg(fd, LSSPROTO_TEACHER_SYSTEM_SEND, buffer);
}
#endif

#ifdef _ADD_STATUS_2
void lssproto_S2_send(int fd, char* data)
{
    if (CONNECT_checkfd(fd) == FALSE)
    {
        return;
    }

#ifdef _DEBUG_SEND_CLI
    printf("[发送]LSSPROTO_S2_SEND-data:%s\n", data);
#endif
    char buffer[65500];
    int checksum = 0;
    strcpy(buffer, "");
    CONNECT_getCdkey(fd, PersonalKey, 4096);
    strcat(PersonalKey, getRandDefaultKey(fd));
    checksum += util_mkstring(fd, buffer, data);
    util_mkint(fd, buffer, checksum);
    util_SendMesg(fd, LSSPROTO_S2_SEND, buffer);
}
#endif

#ifdef _ONLINE_SHOP
void lssproto_VIP_SHOP_send(int fd, int num, int BJ, int type, int shoppage, int page, char *data)
{
    if (CONNECT_checkfd(fd) == FALSE)
    {
        return;
    }

#ifdef _DEBUG_SEND_CLI
    printf("[发送]lssproto_VIP_SHOP_send-num:%d,BJ:%d,type:%d,shoppage:%d,page:%d,data:%s\n", num, BJ, type, shoppage, page, data);
#endif
    char buffer[65500];
    int checksum = 0;
    strcpy(buffer, "");
    CONNECT_getCdkey(fd, PersonalKey, 4096);
    strcat(PersonalKey, getRandDefaultKey(fd));
    checksum += util_mkint(fd, buffer, num);   //有效数目
    checksum += util_mkint(fd, buffer, BJ);  //贝币数目
    checksum += util_mkint(fd, buffer, shoppage);   //显示页面
    checksum += util_mkint(fd, buffer, page); //总共页数
    checksum += util_mkint(fd, buffer, type); //显示类别
    checksum += util_mkstring(fd, buffer, data);
    util_mkint(fd, buffer, checksum);
    util_SendMesg(fd, LSSPROTO_VIP_SHOP_SEND, buffer);
}
#endif

#ifdef _ASSESS_ABILITY
void lssproto_ASSESS_ABILITY_send(int fd, char *data)
{
    if (CONNECT_checkfd(fd) == FALSE)
    {
        return;
    }

#ifdef _DEBUG_SEND_CLI
    printf("[发送]lssproto_ASSESS_ABILITY_send-data:%s\n", data);
#endif
    char buffer[65500];
    int checksum = 0;
    strcpy(buffer, "");
    CONNECT_getCdkey(fd, PersonalKey, 4096);
    strcat(PersonalKey, getRandDefaultKey(fd));
    checksum += util_mkstring(fd, buffer, data);
    util_mkint(fd, buffer, checksum);
    util_SendMesg(fd, LSSPROTO_ASSESS_ABILITY_SEND, buffer);
}
#endif

void lssproto_DENGON_send(int fd, char *data, int color, int num)
{
    if (CONNECT_checkfd(fd) == FALSE)
    {
        return;
    }

#ifdef _DEBUG_SEND_CLI
    printf("[发送]lssproto_DENGON_send-data:%s,%d,%d\n", data, color, num);
#endif
    char buffer[65500];
    int checksum = 0;
    strcpy(buffer, "");
    CONNECT_getCdkey(fd, PersonalKey, 4096);
    strcat(PersonalKey, getRandDefaultKey(fd));
    checksum += util_mkstring(fd, buffer, data);
    checksum += util_mkint(fd, buffer, color);
    checksum += util_mkint(fd, buffer, num);
    util_mkint(fd, buffer, checksum);
    util_SendMesg(fd, LSSPROTO_DENGON_SEND, buffer);
}

void lssproto_SAMENU_send(int fd, int index, char* data)
{
    if (CONNECT_checkfd(fd) == FALSE)
    {
        return;
    }

#ifdef _DEBUG_SEND_CLI
    printf("[发送]lssproto_SAMENU_send-data:%d,%s\n", index, data);
#endif
    char buffer[65500];
    int checksum = 0;
    strcpy(buffer, "");
    CONNECT_getCdkey(fd, PersonalKey, 4096);
    strcat(PersonalKey, getRandDefaultKey(fd));
    checksum += util_mkint(fd, buffer, index);
    checksum += util_mkstring(fd, buffer, data);
    util_mkint(fd, buffer, checksum);
    util_SendMesg(fd, LSSPROTO_SAMENU_SEND, buffer);
}

#ifdef  _OFFLINE_RETEUN
void lssproto_NEWLOGOUT_send(int fd, int flg)
{
    if (CONNECT_checkfd(fd) == FALSE)
    {
        return;
    }

#ifdef _DEBUG_SEND_CLI
    printf("[发送]lssproto_NEWLOGOUT_send-data:%d\n", flg);
#endif
    char buffer[65500];
    int checksum = 0;
    strcpy(buffer, "");
    CONNECT_getCdkey(fd, PersonalKey, 4096);
    strcat(PersonalKey, getRandDefaultKey(fd));
    checksum += util_mkint(fd, buffer, flg);
    util_mkint(fd, buffer, checksum);
    util_SendMesg(fd, LSSPROTO_NEWLOGOUT_SEND, buffer);
}
#endif

//void lssproto_CAPTCHA_send(int fd, char* data)
//{
//  if( CONNECT_checkfd(fd) == FALSE )return;
//#ifdef _DEBUG_SEND_CLI
//  printf("[发送]lssproto_CAPTCHA_send-data:%s\n", data);
//#endif
//  char buffer[65500];
//  int checksum=0;
//
//  strcpy(buffer,"");
//
//  CONNECT_getCdkey( fd, PersonalKey, 4096);
//  strcat(PersonalKey, getRandDefaultKey(fd));
//
//  checksum += util_mkstring(fd,buffer, data);
//  util_mkint(fd,buffer, checksum);
//  util_SendMesg(fd, LSSPROTO_CAPTCHA_SEND, buffer);
//}
//
//void lssproto_MAC_send(int fd, char* data1,char* data2)
//{
//  if( CONNECT_checkfd(fd) == FALSE )return;
//#ifdef _DEBUG_SEND_CLI
//  printf("[发送]lssproto_MAC_send-data:%s,%s\n", data1,data2);
//#endif
//  char buffer[65500];
//  int checksum=0;
//
//  strcpy(buffer,"");
//
//  CONNECT_getCdkey( fd, PersonalKey, 4096);
//  strcat(PersonalKey, getRandDefaultKey(fd));
//
//  checksum += util_mkstring(fd,buffer, data1);
//  checksum += util_mkstring(fd,buffer, data2);
//  util_mkint(fd,buffer, checksum);
//  util_SendMesg(fd, LSSPROTO_MAC_SEND, buffer);
//}

#ifdef  _BATTLE_CLI_TIME
void lssproto_BATTLETIME_send(int fd, int battletime)
{
    if (CONNECT_checkfd(fd) == FALSE)
    {
        return;
    }

#ifdef _DEBUG_SEND_CLI
    printf("[发送]lssproto_BATTLETIME_send-battletime:%d\n", battletime);
#endif
    char buffer[65500];
    int checksum = 0;
    strcpy(buffer, "");
    CONNECT_getCdkey(fd, PersonalKey, 4096);
    strcat(PersonalKey, getRandDefaultKey(fd));
    checksum += util_mkint(fd, buffer, battletime);
    util_mkint(fd, buffer, checksum);
    util_SendMesg(fd, LSSPROTO_BATTLETIME_SEND, buffer);
}
#endif
#ifdef  _PET_LV1_4V
void lssproto_PET4V_send(int fd, int petno, char* data)
{
    if (CONNECT_checkfd(fd) == FALSE)
    {
        return;
    }

#ifdef _DEBUG_SEND_CLI
    printf("[发送]LSSPROTO_PET4V_SEND-data:%s\n", data);
#endif
    char buffer[65500];
    int checksum = 0;
    strcpy(buffer, "");
    CONNECT_getCdkey(fd, PersonalKey, 4096);
    strcat(PersonalKey, getRandDefaultKey(fd));
    checksum += util_mkint(fd, buffer, petno);
    checksum += util_mkstring(fd, buffer, data);
    util_mkint(fd, buffer, checksum);
    util_SendMesg(fd, LSSPROTO_PET4V_SEND, buffer);
}
#endif
#ifdef _RED_MEMOY_
void lssproto_RedMemoy_send(int fd, int type, int time, int vip, char *name, int index)
{
    if (CONNECT_checkfd(fd) == FALSE)
    {
        return;
    }

#ifdef _DEBUG_SEND_CLI
    printf("[发送]LSSPROTO_RedMemoy_SEND-type:%d,time:%d,vip:%d,name:%s,index:%d\n", type, time, vip, name, index);
#endif
    char buffer[65500];
    int checksum = 0;
    strcpy(buffer, "");
    CONNECT_getCdkey(fd, PersonalKey, 4096);
    strcat(PersonalKey, getRandDefaultKey(fd));
    checksum += util_mkint(fd, buffer, type);
    checksum += util_mkint(fd, buffer, time);
    checksum += util_mkint(fd, buffer, vip);
    checksum += util_mkstring(fd, buffer, name);
    checksum += util_mkint(fd, buffer, index);
    util_mkint(fd, buffer, checksum);
    util_SendMesg(fd, LSSPROTO_REDMEMOY_SEND, buffer);
}
#endif

#ifdef _FAMILYBADGE_
void lssproto_CHAREFFECT_send(int fd, char* message)
{
    if (CONNECT_checkfd(fd) == FALSE)
    {
        return;
    }

#ifdef _DEBUG_SEND_CLI
    printf("[发送]LSSPROTO_BATTLESKILL_SEND-message:%s\n", message);
#endif
    char buffer[65500];
    int checksum = 0;
    strcpy(buffer, "");
    CONNECT_getCdkey(fd, PersonalKey, 4096);
    strcat(PersonalKey,  getRandDefaultKey(fd));
    checksum += util_mkstring(fd, buffer, message);
    util_mkint(fd, buffer, checksum);
    util_SendMesg(fd, LSSPROTO_CHAREFFECT_SEND, buffer);
}
#endif

#ifdef _FAMILYBADGE_
void lssproto_FamilyBadge_send(int fd, char *data)
{
    if (CONNECT_checkfd(fd) == FALSE)
    {
        return;
    }

#ifdef _DEBUG_SEND_CLI
    printf("[发送]lssproto_FamilyBadge_send-data:%s\n", data);
#endif
    char buffer[65500];
    int checksum = 0;
    strcpy(buffer, "");
    CONNECT_getCdkey(fd, PersonalKey, 4096);
    strcat(PersonalKey, getRandDefaultKey(fd));
    checksum += util_mkstring(fd, buffer, data);
    util_mkint(fd, buffer, checksum);
    util_SendMesg(fd, LSSPROTO_FAMILYBADGE_SEND, buffer);
}
#endif

//void lssproto_PETCOM_send(int fd,int petno)
//{
//  if( CONNECT_checkfd(fd) == FALSE )return;
//#ifdef _DEBUG_SEND_CLI
//  printf("[发送]LSSPROTO_PETCOM_SEND-petno:%d\n",petno);
//#endif
//  char buffer[65500];
//  int checksum=0;
//
//  strcpy(buffer,"");
//
//  CONNECT_getCdkey( fd, PersonalKey, 4096);
//  strcat(PersonalKey, getRandDefaultKey(fd));
//
//  checksum += util_mkint(fd,buffer, petno);
//  util_mkint(fd,buffer, checksum);
//  util_SendMesg(fd, LSSPROTO_PETCOM_SEND, buffer);
//}

void lssproto_STREETTYPE_send(int fd, int streettype)
{
    if (CONNECT_checkfd(fd) == FALSE)
    {
        return;
    }

#ifdef _DEBUG_SEND_CLI
    printf("[发送]LSSPROTO_STREETTYPE_SEND-streettype:%d\n", streettype);
#endif
    char buffer[65500];
    int checksum = 0;
    strcpy(buffer, "");
    CONNECT_getCdkey(fd, PersonalKey, 4096);
    strcat(PersonalKey, getRandDefaultKey(fd));
    checksum += util_mkint(fd, buffer, streettype);
    util_mkint(fd, buffer, checksum);
    util_SendMesg(fd, LSSPROTO_STREETTYPE_SEND, buffer);
}

#ifdef  _BATTLE_PAIDUI
void lssproto_BattlePaiDui_send(int fd, int num)
{
    if (CONNECT_checkfd(fd) == FALSE)
    {
        return;
    }

#ifdef _DEBUG_SEND_CLI
    printf("[发送]LSSPROTO_BATTLEPAIDUI_SEND-num:%d\n", num);
#endif
    char buffer[65500];
    int checksum = 0;
    strcpy(buffer, "");
    CONNECT_getCdkey(fd, PersonalKey, 4096);
    strcat(PersonalKey, getRandDefaultKey(fd));
    checksum += util_mkint(fd, buffer, num);
    util_mkint(fd, buffer, checksum);
    util_SendMesg(fd, LSSPROTO_BATTLEPAIDUI_SEND, buffer);
}
#endif

#ifdef  _PETITEM_TALKSHOW
void lssproto_ItemTalkShow_send(int fd, char* posname, char* itembuff, int posbmp)
{
    if (CONNECT_checkfd(fd) == FALSE)
    {
        return;
    }

#ifdef _DEBUG_SEND_CLI
    printf("[发送]LSSPROTO_ITEMTALKSHOW_SEND-posname:%s,itembuff:%s,posbmp:%d\n", posname, itembuff, posbmp);
#endif
    char buffer[65500];
    int checksum = 0;
    strcpy(buffer, "");
    CONNECT_getCdkey(fd, PersonalKey, 4096);
    strcat(PersonalKey, getRandDefaultKey(fd));
    checksum += util_mkstring(fd, buffer, posname);
    checksum += util_mkstring(fd, buffer, itembuff);
    checksum += util_mkint(fd, buffer, posbmp);
    util_mkint(fd, buffer, checksum);
    util_SendMesg(fd, LSSPROTO_ITEMTALKSHOW_SEND, buffer);
}
void lssproto_PetTalkShow_send(int fd, char* posname, int posbmp, int oldhp, int oldatk, int olddef, int olddex, int oldlv,
                               int nowhp, int nowatk, int nowdef, int nowdex, int level, int trans, int earth, int water, int fire,   int wind, int zhong,
                               char* skillname1, char* skillname2,  char* skillname3, char* skillname4, char* skillname5, char* skillname6, char* skillname7)
{
    if (CONNECT_checkfd(fd) == FALSE)
    {
        return;
    }

    char buffer[65500];
    int checksum = 0;
    strcpy(buffer, "");
    CONNECT_getCdkey(fd, PersonalKey, 4096);
    strcat(PersonalKey, getRandDefaultKey(fd));
    char pospetbuff[2048];
    memset(pospetbuff, 0, sizeof(pospetbuff));
    sprintf(pospetbuff, "%s|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%s|%s|%s|%s|%s|%s|%s", posname, posbmp, oldhp, oldatk, olddef, olddex, oldlv, nowhp, nowatk, nowdef, nowdex, level, trans, earth, water
            , fire, wind, zhong, skillname1, skillname2, skillname3, skillname4, skillname5, skillname6, skillname7);
#ifdef _DEBUG_SEND_CLI
    printf("[发送]LSSPROTO_PETTALKSHOW_SEND-pospetbuff:%s\n", pospetbuff);
#endif
    checksum += util_mkstring(fd, buffer, pospetbuff);
    util_mkint(fd, buffer, checksum);
    util_SendMesg(fd, LSSPROTO_PETTALKSHOW_SEND, buffer);
}
#endif

#ifdef  _PLAYER_UID
void lssproto_PlayerUid_send(int fd, char* uid)
{
    if (CONNECT_checkfd(fd) == FALSE)
    {
        return;
    }

#ifdef _DEBUG_SEND_CLI
    printf("[发送]LSSPROTO_PLAYERUID_SEND-uid:%s\n", uid);
#endif
    char buffer[65500];
    int checksum = 0;
    strcpy(buffer, "");
    CONNECT_getCdkey(fd, PersonalKey, 4096);
    strcat(PersonalKey, getRandDefaultKey(fd));
    checksum += util_mkstring(fd, buffer, uid);
    util_mkint(fd, buffer, checksum);
    util_SendMesg(fd, LSSPROTO_PLAYERUID_SEND, buffer);
}
#endif

#ifdef  _NEW_MIYU_
void lssproto_NewChat_send(int fd, char* data, int faceno, int trn, int lv)
{
    if (CONNECT_checkfd(fd) == FALSE)
    {
        return;
    }

#ifdef _DEBUG_SEND_CLI
    printf("[发送]LSSPROTO_NEWCHAT_SEND-uid:%s,faceno:%d,trn:%d,lv:%d\n", data, faceno, trn, lv);
#endif
    char buffer[65500];
    int checksum = 0;
    strcpy(buffer, "");
    CONNECT_getCdkey(fd, PersonalKey, 4096);
    strcat(PersonalKey, getRandDefaultKey(fd));
    checksum += util_mkstring(fd, buffer, data);
    checksum += util_mkint(fd, buffer, faceno);
    checksum += util_mkint(fd, buffer, trn);
    checksum += util_mkint(fd, buffer, lv);
    util_mkint(fd, buffer, checksum);
    util_SendMesg(fd, LSSPROTO_NEWCHAT_SEND, buffer);
}
#endif

#ifdef _WINLUATO_
void lssproto_WinLuaTo_send(int fd, char *funcName, char *data)
{
    if (CONNECT_checkfd(fd) == FALSE)
    {
        return;
    }

    char buffer[65500];
    int checksum = 0;
    strcpy(buffer, "");
    CONNECT_getCdkey(fd, PersonalKey, 4096);
    strcat(PersonalKey, getRandDefaultKey(fd));
#ifdef _DEBUG_SEND_CLI
    printf("[发送]LSSPROTO_WinLuaTo_SEND-funcName:%s,data:%s\n", funcName, data);
#endif
    checksum += util_mkstring(fd, buffer, funcName);
    checksum += util_mkstring(fd, buffer, data);
    util_mkint(fd, buffer, checksum);
    util_SendMesg(fd, LSSPROTO_WINLUATO_SEND, buffer);
}

#endif

#ifdef _LONIN_ERROR_
void lssproto_LoninError_send(int fd, char *data)
{
    if (CONNECT_checkfd(fd) == FALSE)
    {
        return;
    }

    char buffer[65500];
    int checksum = 0;
    strcpy(buffer, "");
    CONNECT_getCdkey(fd, PersonalKey, 4096);
    strcat(PersonalKey, getRandDefaultKey(fd));
    checksum += util_mkstring(fd, buffer, data);
    util_mkint(fd, buffer, checksum);
    util_SendMesg(fd, LSSPROTO_LONINERROR_SEND, buffer);
}
#endif

#ifdef _IPHONE_SHOP_FLAG_
void lssproto_ShopFlag_send(int fd, int data)
{
    if (CONNECT_checkfd(fd) == FALSE)
    {
        return;
    }

    char buffer[2048];
    int checksum = 0;
    strcpy(buffer, "");
    CONNECT_getCdkey(fd, PersonalKey, 4096);
    strcat(PersonalKey, getRandDefaultKey(fd));
    checksum += util_mkint(fd, buffer, data);
    util_mkint(fd, buffer, checksum);
    util_SendMesg(fd, LSSPROTO_SHOPFLAG_SEND, buffer);
}
#endif

#ifdef _SPEED_CHECK_
void lssproto_SpeedCheck_send(int fd, int num)
{
    if (CONNECT_checkfd(fd) == FALSE)
    {
        return;
    }

    char buffer[65500];
    int checksum = 0;
    strcpy(buffer, "");
    CONNECT_getCdkey(fd, PersonalKey, 4096);
    strcat(PersonalKey, getRandDefaultKey(fd));
    checksum += util_mkint(fd, buffer, num);
    util_mkint(fd, buffer, checksum);
    util_SendMesg(fd, LSSPROTO_SPEED_CHECK_SEND, buffer);
}
#endif

#ifdef _ADDJIETU_
void lssproto_jiandu_send(int fd)
{
    if (CONNECT_checkfd(fd) == FALSE)
    {
        return;
    }

    char buffer[65500];
    int checksum = 0;
    strcpy(buffer, "");
    CONNECT_getCdkey(fd, PersonalKey, 4096);
    strcat(PersonalKey, getRandDefaultKey(fd));
    checksum += util_mkint(fd, buffer, 1);
    util_mkint(fd, buffer, checksum);
    util_SendMesg(fd, LSSPROTO_JIANDU_SEND, buffer);
}
#endif

#ifdef _NEW_SHOP_
void lssproto_UpShopData_send(int fd, char *data, char * md5, int id)
{
    unsigned long len = strlen(data);
    unsigned long comprLen = 1024 * 100 - 55;
    char compr[1024 * 100] = { 0 };
    compress(compr + 55, &comprLen, data, len);
    snprintf(compr, sizeof(compr), "商城%d|%d|%d|%s", len, comprLen, id, md5);
    fd_set rfds, wfds, efds;
    FD_ZERO(&rfds);
    FD_ZERO(&wfds);
    FD_ZERO(&efds);
    FD_SET(fd, &rfds);
    FD_SET(fd, &wfds);
    FD_SET(fd, &efds);
    struct timeval tmv;
    tmv.tv_sec = tmv.tv_usec = 0;
    int ret = select(fd + 1, &rfds, &wfds, &efds, &tmv);

    if (ret > 0 && FD_ISSET(fd, &wfds))
    {
        ret = send(fd, compr, comprLen + 55, 0);

        if (ret == -1 && errno != EINTR)
        {
            CONNECT_endOne_debug(fd);
        }
    }
    else if (ret < 0 && errno != EINTR)
    {
        CONNECT_endOne_debug(fd);
    }
}
void lssproto_ShopOK_send(int fd)
{
    char buffer[1024 * 64];
    strcpy(buffer, "");
    int checksum = 0;
    checksum += util_mkint(fd, buffer, 0);
    util_mkint(fd, buffer, checksum);
    util_SendMesg(fd, LSSPROTO_SHOPOK_SEND, buffer);
}
#endif

#ifdef _OFFLINERETURN_LOGINCLI
void lssproto_OfflineReturn_send(int fd)
{
    char buffer[1024 * 64];
    strcpy(buffer, "");
    int checksum = 0;
    checksum += util_mkint(fd, buffer, 0);
    util_mkint(fd, buffer, checksum);
    util_SendMesg(fd, LSSPROTO_OFFLINERETURN_SEND, buffer);
}
#endif
#ifdef _SERVER_NP_
void lssproto_NP_send(int fd, char* data)
{
    char buffer[1024 * 64];
    strcpy(buffer, "");
    int checksum = 0;
    CONNECT_getCdkey(fd, PersonalKey, 4096);
    strcat(PersonalKey, getRandDefaultKey(fd));
    checksum += util_mkstring(fd, buffer, data);
    util_mkint(fd, buffer, checksum);
    util_SendMesg(fd, LSSPROTO_NP_SEND, buffer);
}
#endif


#ifdef _NEW_OFFLINEBATTLERETURN
void lssproto_OfflineBattle_send(int fd, int timenum)
{
    char buffer[1024 * 64];
    strcpy(buffer, "");
    int checksum = 0;
    CONNECT_getCdkey(fd, PersonalKey, 4096);
    strcat(PersonalKey, getRandDefaultKey(fd));
    checksum += util_mkint(fd, buffer, timenum);
    util_mkint(fd, buffer, checksum);
#ifdef _DEBUG_SEND_CLI
    printf("[发送]lssproto_OfflineBattle_send-timenum:%d\n", timenum);
#endif
    util_SendMesg(fd, LSSPROTO_OFFLINEBATTLE_SEND, buffer);
}
#endif

#ifdef _RED_MEMOY_
void lssproto_NewRedMoney_send(int fd, int point)
{
    char buffer[1024 * 64];
    strcpy(buffer, "");
    int checksum = 0;
    CONNECT_getCdkey(fd, PersonalKey, 4096);
    strcat(PersonalKey, getRandDefaultKey(fd));
    checksum += util_mkint(fd, buffer, point);
    util_mkint(fd, buffer, checksum);
#ifdef _DEBUG_SEND_CLI
    printf("[发送]lssproto_NewRedMoney_send-timenum:%d\n", point);
#endif
    util_SendMesg(fd, LSSPROTO_NEWREDMEMOY_SEND, buffer);
}
#endif

void lssproto_GetGold_send(int fd, int gold, int vippoint, int fame, int vigor, int petpoint)
{
    char buffer[1024 * 64];
    strcpy(buffer, "");
    int checksum = 0;
    CONNECT_getCdkey(fd, PersonalKey, 4096);
    strcat(PersonalKey, getRandDefaultKey(fd));
    checksum += util_mkint(fd, buffer, gold);
    checksum += util_mkint(fd, buffer, vippoint);
    checksum += util_mkint(fd, buffer, fame);
    checksum += util_mkint(fd, buffer, vigor);
    checksum += util_mkint(fd, buffer, petpoint);
    util_mkint(fd, buffer, checksum);
#ifdef _DEBUG_SEND_CLI
    printf("[发送]lssproto_GetGold_send-timenum:%d,%d,%d,%d,%d\n", gold, vippoint, fame, vigor, petpoint);
#endif
    util_SendMesg(fd, LSSPROTO_GETGOLD_SEND, buffer);
}

void lssproto_QQLogin_send(int fd, char* cdkey)
{
    if (CONNECT_checkfd(fd) == FALSE)
    {
        return;
    }

#ifdef _DEBUG_SEND_CLI
    printf("[发送]LSSPROTO_QQLOGIN_SEND-result:%s\n", cdkey);
#endif
    char buffer[65500];
    int checksum = 0;
    strcpy(buffer, "");
    CONNECT_getCdkey(fd, PersonalKey, 4096);
    strcat(PersonalKey, getRandDefaultKey(fd));
    checksum += util_mkstring(fd, buffer, cdkey);
    util_mkint(fd, buffer, checksum);
    util_SendMesg(fd, LSSPROTO_QQLOGIN_SEND, buffer);
}

void lssproto_NewMessage_send(int fd, char* message, int color)
{
    char buffer[1024 * 64];
    strcpy(buffer, "");
    int checksum = 0;
    CONNECT_getCdkey(fd, PersonalKey, 4096);
    strcat(PersonalKey, getRandDefaultKey(fd));
    checksum += util_mkstring(fd, buffer, message);
    checksum += util_mkint(fd, buffer, color);
    util_mkint(fd, buffer, checksum);
#ifdef _DEBUG_SEND_CLI
    printf("[发送]lssproto_NewMessage_send:%s,%d\n", message, color);
#endif
    util_SendMesg(fd, LSSPROTO_NEWMESSAGE_SEND, buffer);
}

void lssproto_Mail_send(int fd, int id, int type, char* zhuti, char* neirong, char* itembuff)
{
    char buffer[1024 * 64];
    strcpy(buffer, "");
    int checksum = 0;
    CONNECT_getCdkey(fd, PersonalKey, 4096);
    strcat(PersonalKey, getRandDefaultKey(fd));
    checksum += util_mkint(fd, buffer, id);
    checksum += util_mkint(fd, buffer, type);
    checksum += util_mkstring(fd, buffer, zhuti);
    checksum += util_mkstring(fd, buffer, neirong);
    checksum += util_mkstring(fd, buffer, itembuff);
    util_mkint(fd, buffer, checksum);
#ifdef _DEBUG_SEND_CLI
    printf("[发送]lssproto_Mail_send:%d,%d,%s,%s,%s\n", id, type, zhuti, neirong, itembuff);
#endif
    util_SendMesg(fd, LSSPROTO_MAIL_SEND, buffer);
}


void lssproto_NewStreet_send(int fd, char* data)
{
    char buffer[1024 * 64];
    strcpy(buffer, "");
    int checksum = 0;
    CONNECT_getCdkey(fd, PersonalKey, 4096);
    strcat(PersonalKey, getRandDefaultKey(fd));
    checksum += util_mkstring(fd, buffer, data);
    util_mkint(fd, buffer, checksum);
#ifdef _DEBUG_SEND_CLI
    printf("[发送]lssproto_NewStreet_send:%s\n", data);
#endif
    util_SendMesg(fd, LSSPROTO_NEWSTREET_SEND, buffer);
}


void lssproto_WNUpdate_send(int fd, int windowtype, int buttontype, int seqno, int objindex, char* data)
{
    char buffer[65500];
#ifdef _DEBUG_SEND_CLI
    printf("[发送]LSSPROTO_WNUPDATE_SEND-windowtype:%d,buttontype:%d,seqno:%d,objindex:%d,data:%s\n", windowtype, buttontype, seqno, objindex, data);
#endif
    int checksum = 0;
    strcpy(buffer, "");
    CONNECT_getCdkey(fd, PersonalKey, 4096);
    strcat(PersonalKey, getRandDefaultKey(fd));
    checksum += util_mkint(fd, buffer, windowtype);
    checksum += util_mkint(fd, buffer, buttontype);
    checksum += util_mkint(fd, buffer, seqno);
    checksum += util_mkint(fd, buffer, objindex);
    checksum += util_mkstring(fd, buffer, data);
    util_mkint(fd, buffer, checksum);
    util_SendMesg(fd, LSSPROTO_WNUPDATE_SEND, buffer);
}

#ifdef _NEW_EVENT_ID_UPDATE
void lssproto_EventID_send(int fd)
{
    int fd_charaindex = CONNECT_getCharaindex(fd);

    if (!CHAR_CHECKINDEX(fd_charaindex))
    {
        return;
    }

    char buffer[65500];
    int checksum = 0;
    strcpy(buffer, "");
    CONNECT_getCdkey(fd, PersonalKey, 4096);
    strcat(PersonalKey, getRandDefaultKey(fd));
    int i;
    char bufftemp[1024];
    memset(bufftemp, 0, sizeof(bufftemp));

    for (i = 0; i < 32; i++)
    {
        sprintf(bufftemp, "%s%d|", bufftemp, CHAR_getInt(fd_charaindex, CHAR_ENDEVENT + i));
    }

    checksum += util_mkstring(fd, buffer, bufftemp);
    char eventidbuff[256];
    memset(eventidbuff, 0, sizeof(eventidbuff));
    sprintf(eventidbuff, "%s", CHAR_getChar(fd_charaindex, CHAR_EVENTID));
    extern int StrReplace(char* strRes, char* from, char* to);

    while (strstr(eventidbuff, "||"))
    {
        StrReplace(eventidbuff, "||", "|");
    }

    CHAR_setChar(fd_charaindex, CHAR_EVENTID, eventidbuff);
    checksum += util_mkstring(fd, buffer, CHAR_getChar(fd_charaindex, CHAR_EVENTID));
    util_mkint(fd, buffer, checksum);
    util_SendMesg(fd, LSSPROTO_EVENTID_SEND, buffer);
}
#endif
#ifdef _NEW_PROMPTFLG
void lssproto_NewPromptFlg_send(int fd)
{
    int fd_charaindex = CONNECT_getCharaindex(fd);

    if (!CHAR_CHECKINDEX(fd_charaindex))
    {
        return;
    }

    char buffer[65500];
    int checksum = 0;
    strcpy(buffer, "");
    CONNECT_getCdkey(fd, PersonalKey, 4096);
    strcat(PersonalKey, getRandDefaultKey(fd));
    checksum += util_mkint(fd, buffer, CHAR_getInt(fd_charaindex, CHAR_NEWPROMPTFLG1));
    checksum += util_mkint(fd, buffer, CHAR_getInt(fd_charaindex, CHAR_NEWPROMPTFLG2));
    util_mkint(fd, buffer, checksum);
    util_SendMesg(fd, LSSPROTO_NEWPROMPTFLG_SEND, buffer);
}
#endif
#ifdef _NEW_MAPBATTLEINFO
void lssproto_NewMapBattleInfo_send(int fd, int maptype, char* battleinfo)
{
    char buffer[65500];
    int checksum = 0;
    strcpy(buffer, "");
    CONNECT_getCdkey(fd, PersonalKey, 4096);
    strcat(PersonalKey, getRandDefaultKey(fd));
    checksum += util_mkint(fd, buffer, maptype);
    checksum += util_mkstring(fd, buffer, battleinfo);
    util_mkint(fd, buffer, checksum);
    util_SendMesg(fd, LSSPROTO_NEWMAPBATTLEINFO_SEND, buffer);
}
#endif
void lssproto_Announce_send(int fd, char* msgbuff, int color, int num)
{
    char buffer[65500];
    int checksum = 0;
    strcpy(buffer, "");
    CONNECT_getCdkey(fd, PersonalKey, 4096);
    strcat(PersonalKey, getRandDefaultKey(fd));
    checksum += util_mkstring(fd, buffer, msgbuff);
    checksum += util_mkint(fd, buffer, color);
    checksum += util_mkint(fd, buffer, num);
    util_mkint(fd, buffer, checksum);
    util_SendMesg(fd, LSSPROTO_ANNOUNCE_SEND, buffer);
}

void lssproto_BuyVigor_send(int fd, int buytype, char* msgbuff)
{
    char buffer[65500];
    int checksum = 0;
    strcpy(buffer, "");
    CONNECT_getCdkey(fd, PersonalKey, 4096);
    strcat(PersonalKey, getRandDefaultKey(fd));
    checksum += util_mkint(fd, buffer, buytype);
    checksum += util_mkstring(fd, buffer, msgbuff);
    util_mkint(fd, buffer, checksum);
    util_SendMesg(fd, LSSPROTO_BUYVIGOR_SEND, buffer);
}

#ifdef _PARTY_HIGH
void lssproto_PartyHigh_send(int fd, int partytype, char* partybuff)
{
    char buffer[65500];
    int checksum = 0;
    strcpy(buffer, "");
    CONNECT_getCdkey(fd, PersonalKey, 4096);
    strcat(PersonalKey, getRandDefaultKey(fd));
    checksum += util_mkint(fd, buffer, partytype);
    checksum += util_mkstring(fd, buffer, partybuff);
    util_mkint(fd, buffer, checksum);
    util_SendMesg(fd, LSSPROTO_PARTYHIGH_SEND, buffer);
}
#endif

#ifdef _TALK_PLAYER_SHOW
void lssproto_TalkPlayerShow_send(int fd, char* playername, int faceimage, int playerlevel, int playertrans, int playerfmindex, char* playerfmname, int playerfmsprite)
{
    char buffer[65500];
    int checksum = 0;
    strcpy(buffer, "");
    CONNECT_getCdkey(fd, PersonalKey, 4096);
    strcat(PersonalKey, getRandDefaultKey(fd));
    checksum += util_mkstring(fd, buffer, playername);
    checksum += util_mkint(fd, buffer, faceimage);
    checksum += util_mkint(fd, buffer, playerlevel);
    checksum += util_mkint(fd, buffer, playertrans);
    checksum += util_mkint(fd, buffer, playerfmindex);
    checksum += util_mkstring(fd, buffer, playerfmname);
    checksum += util_mkint(fd, buffer, playerfmsprite);
    util_mkint(fd, buffer, checksum);
    util_SendMesg(fd, LSSPROTO_TALKPLAYERSHOW_SEND, buffer);
}
#endif

void lssproto_NewPrompt_send(int fd, char* msg, int color, int timer)
{
    char buffer[65500];
    int checksum = 0;
    strcpy(buffer, "");
    CONNECT_getCdkey(fd, PersonalKey, 4096);
    strcat(PersonalKey, getRandDefaultKey(fd));
    checksum += util_mkstring(fd, buffer, msg);
    checksum += util_mkint(fd, buffer, color);
    checksum += util_mkint(fd, buffer, timer);
    util_mkint(fd, buffer, checksum);
    util_SendMesg(fd, LSSPROTO_PARTYHIGH_SEND, buffer);
}

void lssproto_NewSaMenu_send(int fd, int index, char *data)
{
    char buffer[65500];
    int checksum = 0;
    strcpy(buffer, "");
    CONNECT_getCdkey(fd, PersonalKey, 4096);
    strcat(PersonalKey, getRandDefaultKey(fd));
    checksum += util_mkint(fd, buffer, index);
    checksum += util_mkstring(fd, buffer, data);
    util_mkint(fd, buffer, checksum);
    util_SendMesg(fd, LSSPROTO_NEWSAMENU_SEND, buffer);
}


#ifdef _NEWSAMENU_WITH_CALLBACK
void lssproto_NewSaMenu2_send(int fd, int index, char *data,char* func)
{
    char buffer[65500];
    int checksum = 0;
    strcpy(buffer, "");
    CONNECT_getCdkey(fd, PersonalKey, 4096);
    strcat(PersonalKey, getRandDefaultKey(fd));
    checksum += util_mkint(fd, buffer, index);
    checksum += util_mkstring(fd, buffer, data);
    checksum += util_mkstring(fd, buffer, func);
    util_mkint(fd, buffer, checksum);
    util_SendMesg(fd, LSSPROTO_NEWSAMENU2_SEND, buffer);
}
#endif

#ifdef _NEWPLAYER_FUNC
void lssproto_NewPlayer_send(int fd,int flg,char* data){
	char buffer[1024 * 64];
	int checksum=0;
	strcpy(buffer,"");
	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, getRandDefaultKey(fd));

	checksum += util_mkint(fd,buffer, flg);
	checksum += util_mkstring(fd,buffer, data);
	util_mkint(fd,buffer, checksum);
	util_SendMesg(fd, LSSPROTO_NEWPLAYER_SEND, buffer);
}

#endif


#ifdef _UNITY_CLIENT_UPDATE
void lssproto_UClientUpdate_send(int fd,int windowid){
	char buffer[1024 * 64];
	int checksum=0;
	strcpy(buffer,"");
	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, getRandDefaultKey(fd));
	checksum += util_mkint(fd,buffer, windowid);
	util_mkint(fd,buffer, checksum);
	util_SendMesg(fd, LSSPROTO_UCUPDATE_SEND, buffer);
}
#endif

#ifdef _NEW_515SA
void lssproto_Offline_send(int fd, char *data){
	if( CONNECT_checkfd(fd) == FALSE )return;
	char buffer[65500];
	int checksum=0;
	strcpy(buffer,"");
	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, getRandDefaultKey(fd));
	checksum += util_mkstring(fd,buffer, data);
	util_mkint(fd,buffer, checksum);
	util_SendMesg(fd, LSSPROTO_OFFLINE_SEND, buffer);
}
#endif
