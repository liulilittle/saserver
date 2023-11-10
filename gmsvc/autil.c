// Arminius' protocol utilities ver 0.1
//
// Any questions and bugs, mailto: arminius@mail.hwaei.com.tw

// -------------------------------------------------------------------
// The following definitions is to define game-dependent codes.
// Before compiling, remove the "//".
#define __STONEAGE
#include "version.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "autil.h"
#include "char.h"
#ifdef __STONEAGE
#include "lssproto_util.h"
#include "common.h"
#endif
#ifdef _BLOWFISH_ENCRYPT_PACKET
#include "myfish_util.h"
#endif
#ifdef _NEW_PACKET_CODE
#include "base64.h"
#include "xxtea.h"
#include "aes.h"
#include "blowfish.h"
#endif

// Nuke 0701 fix
char *MesgSlice[SLICE_MAX];
int SliceCount;

char PersonalKey[4096];
#ifdef _NEW_PACKET_CODE
#define NEWPACKETNUM 1500
typedef struct tag_newPacket
{
    int type;
    char key[33];
} NewPacket;
NewPacket NewPacketData[NEWPACKETNUM];

void setNewPacketData(int id, int type, char* key)
{
    if (id < 0 || id >= NEWPACKETNUM)
    {
        return;
    }

    NewPacketData[id].type = type;
    strcpy(NewPacketData[id].key, key);
}
#endif // _NEW_PACKET_CODE
// -------------------------------------------------------------------
// Initialize utilities
//
BOOL util_Init(void)
{
    int i;

    for (i = 0; i < SLICE_MAX; i++)
    {
        MesgSlice[i] = (char *) calloc(1, SLICE_SIZE);

        if (MesgSlice[i] == NULL)
        {
            return FALSE;
        }
    }

    SliceCount = 0;
    strcpy(PersonalKey, _DEFAULT_PKEY);
    return TRUE;
}

// -------------------------------------------------------------------
// Split up a message into slices by spearator.  Store those slices
// into a global buffer "char **MesgSlice"
//
// arg: source=message string;  separator=message separator (1 byte)
// ret: (none)

// WON ADD
//void util_SplitMessage(char *source, char *separator)
BOOL util_SplitMessage(char *source, char *separator)
{
#ifdef _NEW_FUNC_DECRYPT
#ifdef _BLOWFISH_ENCRYPT_PACKET

    if (source[0] != '{')
    {
        return FALSE;
    }

#else

    if (source[0] != '*')
    {
        return FALSE;
    }

#endif
    source++;
#endif

    if (source && separator)      // NULL input is invalid.
    {
        char *ptr;
        char *head = source;

        // Nuke 1006 : Bug fix
        while ((ptr = (char *) strstr(head, separator)) && (SliceCount < SLICE_MAX) && (SliceCount >= 0))
        {
            ptr[0] = '\0';

            if (strlen(head) < SLICE_SIZE)    // discard slices too large
            {
                // Nuke 0701
//      if (*MesgSlice != *dumb) {
                //print("Warning! Mem may be broken\n");
                //}
                /*
                        if (MesgSlice[SliceCount]==0xffffffff) {
                                print("MesgSlice[%d] broken\n",SliceCount);
                                return FALSE;
                        } else {
                */
                strcpy(MesgSlice[SliceCount], head);
                SliceCount++;
                //}
            }

            head = ptr + 1;
        }

        strcpy(source, head);   // remove splited slices
    }

    return TRUE;
}

// -------------------------------------------------------------------
// Encode the message
//
// arg: dst=output  src=input
// ret: (none)
#ifdef _NEW_NETSENDRECV
extern int EncryptionTea(int* v, int n, int* k);
int util_newEncodeMessage(int fd, char *dst, char *src
#ifdef _NEW_PACKET_CODE
                          , int func
#endif



                         )
{
#ifdef _NEW_PACKET_CODE
    int i;
    char key1[33], key2[33], key3[33];
    int len;
    strcpy(key1, CONNECT_getNewDefaultKey(fd));
    strcpy(key2, NewPacketData[func].key);

    for (i = 1; i < 33; i++)
    {
        if ((i % 2) == 0)
        {
            key3[i - 1] = key1[i - 1];
        }
        else
        {
            key3[i - 1] = key2[i - 1];
        }
    }

    key3[32] = '\0';
    len = strlen(src);

    /////////////////////////
    switch (NewPacketData[func].type)
    {
        case 1:
            {
                strcpy(dst + 2, src);
                len = len % 4 ? len / 4 + 1 : len / 4;
                EncryptionTea((int *)(dst + 2), len, (int *)key3);
                len <<= 2;
                break;
            }

        case 2:

        //{
        //    key3[16] = '\0';
        //    xxtea_long retlen;
        //    len = len % 8 ? ((len >> 3) + 2) << 3 : ((len >> 3) + 1) << 3;
        //    retlen = len;
        //    xxtea_encrypt((unsigned char *)src, len, key3, 16, dst + 2, &retlen);
        //    len = retlen;
        //    break;
        //}
        case 3:
            {
                //AES
                len = len % 16 ? ((len >> 4) + 1) << 4 : len;
                aesEncrypt(src, len, dst + 2, key3);
                break;
            }

        case 4:
            {
                //BLOWFISH
                len = len % 8 ? ((len >> 3) + 1) << 3 : len;
                blowfishEncrypt(src, dst + 2, len, key3, 32);
                break;
            }

        default:
            break;
    }

    len += 2;
    *(short *)dst = func + 1590;
    return len;
#else
    strcpy(dst, src);
    int len = strlen(src);
    len = len % 4 ? len / 4 + 1 : len / 4;
    EncryptionTea((int *)dst, len, (int *)CONNECT_getNewDefaultKey(fd));
    return len * 4;
#endif
}
#endif

void util_EncodeMessage(char *dst, char *src)
{
//  strcpy(dst, src);
//  util_xorstring(dst, src);
    int rn = rand() % 99;
    int t1, t2;
    char t3[65500], tz[65500];
    util_swapint(&t1, &rn, "2413");
    t2 = t1 ^ 0xffffffff;
    util_256to64(tz, (char *) &t2, sizeof(int), DEFAULTTABLE);
    util_shlstring(t3, src, rn);
    strcat(tz, t3);
    util_xorstring(dst, tz);
}
void stringtohexstr(char* dst, char* src, int len)
{
    int i;

    for (i = 0; i < len; i++)
    {
        sprintf(dst + 2 * i, "%02X ", (unsigned char)src[i]);
    }

    dst[len * 2] = '\0';
}
void hexstrtostring(char* dst, char* src)
{
    int len;
    int b, tmp;
    len = strlen(src);
    int i;

    for (i = 0; i < len; i++)
    {
        if (src[i] >= 65)
        {
            b = (src[i] - 65 + 10);
        }
        else
        {
            b = src[i] - 48;
        }

        if (i % 2)
        {
            tmp = tmp + b;
            dst[i / 2] = tmp;
        }
        else
        {
            tmp = b << 4;
        }
    }

    dst[len / 2] = 0;
}
#ifdef _NEW_NETSENDRECV


unsigned int DELTA = 0x9e3779b9;
#define MX (z>>5^y<<2) + (y>>3^z<<4)^(sum^y) + (k[p&3^e]^z);

int EncryptionTea(int* v, int n, int* k)
{
    unsigned int z, y, sum, e;
    z = v[n - 1];
    y = v[0];
    sum = 0;
    int p, q;
    /* 加密过程 */
    q = 6 + 52 / n;

    while (q-- > 0)
    {
        sum += DELTA;
        e = (sum >> 2) & 3;

        for (p = 0; p < n - 1; p++)
        {
            y = v[p + 1], z = v[p] += MX;
        }

        y = v[0];
        z = v[n - 1] += MX;
    }

    return 0 ;
}

long DecodeTea(int* v, int n, int* k)
{
    unsigned int z, y, sum, e;
    z = v[n - 1];
    y = v[0];
    sum = 0;
    int p, q;
    q = 6 + 52 / n;
    sum = q * DELTA;

    while (sum != 0)
    {
        e = (sum >> 2) & 3;

        for (p = n - 1; p > 0; p--)
        {
            z = v[p - 1], y = v[p] -= MX;
        }

        z = v[n - 1];
        y = v[0] -= MX;
        sum -= DELTA;
    }

    return 0;
}
BOOL util_DecodeMessageTea(int fd, char *dst, char *sac, int len)
{
#ifdef _NEW_PACKET_CODE
    short func = *(short*)sac - 1029;
    char key1[33], key2[33], key3[33];

    if (func < 0 || func > 999)
    {
        return FALSE;
    }

    strcpy(key1, CONNECT_getNewDefaultKey(fd));
    strcpy(key2, NewPacketData[func].key);
    int i;
    sac += 2;
    len -= 2;
#ifdef _175_SA_2
	int crossNum[32] = { 1,0,0,1,0,1,1,1,0,0,1,1,0,1,1,0,1,1,1,0,1,0,1,1,0,1,0,0,0,1,1,1 };
#else
	int crossNum[32] = { 0,1,0,0,1,0,1,1,0,1,0,1,0,1,1,0,1,1,1,0,0,0,1,1,0,1,0,1,0,1,1,1 };
#endif // _175_SA_2
    for (i = 1; i < 33; i++)
    {
        //if ((i % 2) == 0)
		if (crossNum[i - 1] == 0)
        {
            key3[i - 1] = key1[i - 1];
        }
        else
        {
            key3[i - 1] = key2[i - 1];
        }
    }

    key3[32] = '\0';
    switch (NewPacketData[func].type)
    {
        case 1:
            if (len % 4)
            {
                return FALSE;
            }

            memcpy(dst, sac, len);
            DecodeTea((int*)dst, len >> 2, (int *)key3);
            break;

        case 2:

        //{
        //    xxtea_long retlen;
        //    if (len % 8)
        //    {
        //        return FALSE;
        //    }
        //    retlen = 1024 * 64;
        //    key3[16] = '\0';
        //    xxtea_decrypt(sac, (xxtea_long)len, (unsigned char *)key3, 16, dst, &retlen);
        //}
        //break;
        case 3:
            if (len % 16)
            {
                return FALSE;
            }

            aesDecrypt(sac, len, dst, key3);
            break;

        case 4:
            if (len % 8)
            {
                return FALSE;
            }

            blowfishDecrypt(sac, dst, len, key3, 32);
            break;

        default:
            break;
    }

    return TRUE;
#else

    if (len % 4 != 0)
    {
        return FALSE;
    }

    memcpy(dst, sac, len);
    DecodeTea((int*)dst, len / 4, (int *)CONNECT_getNewDefaultKey(fd));
    return TRUE;
#endif
}
#endif
// -------------------------------------------------------------------
// Decode the message
//
// arg: dst=output  src=input
// ret: (none)
void util_DecodeMessage(char *dst, char *src)
{
//  strcpy(dst, src);
//  util_xorstring(dst, src);
#define INTCODESIZE (sizeof(int)*8+5)/6
    int rn;
    int *t1, t2;
    char t3[4096], t4[4096];  // This buffer is enough for an integer.
    char tz[65500];
#ifndef _NEW_NETSENDRECV

    if (src[strlen(src) - 1] == '\n')
    {
        src[strlen(src) - 1] = '\0';
    }

#endif
    util_xorstring(tz, src);
    // get seed
    strncpy(t4, tz, INTCODESIZE);
    t4[INTCODESIZE] = '\0';
    util_64to256(t3, t4, DEFAULTTABLE);
    t1 = (int *) t3;
    t2 = *t1 ^ 0xffffffff;
    util_swapint(&rn, &t2, "3142");
    util_shrstring(dst, tz + INTCODESIZE, rn);
}

// -------------------------------------------------------------------
// Get a function information from MesgSlice.  A function is a complete
// and identifiable message received, beginned at DEFAULTFUNCBEGIN and
// ended at DEFAULTFUNCEND.  This routine will return the function ID
// (Action ID) and how many fields this function have.
//
// arg: func=return function ID    fieldcount=return fields of the function
// ret: 1=success  0=failed (function not complete)
int util_GetFunctionFromSlice(
#ifdef _NEW_NETSENDRECV
    int fd,
#endif
    int *func, int *fieldcount)
{
    char t1[16384];
    int i;
//  if (strcmp(MesgSlice[0], DEFAULTFUNCBEGIN)!=0) util_DiscardMessage();
    //strcpy(t1, MesgSlice[1]);
    strcpysafe(t1, sizeof(t1), MesgSlice[1]);
    // Robin adjust
#ifdef _VERSION_NEW
#ifdef _BLOWFISH_ENCRYPT_PACKET
    *func = atoi(t1) - 50;
#else
#ifdef _NEW_NETSENDRECV
    *func = atoi(t1) - CONNECT_getFuncNum(fd);
#else
    *func = atoi(t1) - 13;
#endif
#endif
#else
#ifdef _VERSION_80
    *func = atoi(t1) - 13;
#else
    *func = atoi(t1);
#endif
#endif

    for (i = 0; i < SLICE_MAX; i++)
        if (strcmp(MesgSlice[i], DEFAULTFUNCEND) == 0)
        {
            *fieldcount = i - 2; // - "&" - "#" - "func" 3 fields
            return 1;
        }

    return 0; // failed: message not complete
}

void util_DiscardMessage(void)
{
    SliceCount = 0;
}
extern int clisendfunc;
#ifdef _ABSOLUTE_DEBUG
extern int lastfunctime;
#endif
void _util_SendMesg(char *file, int line, int fd, int func, char *buffer)
{
//  char t1[16384], t2[16384];
    char t1[1024 * 64], t2[1024 * 64];
    clisendfunc = func;
#ifdef _ABSOLUTE_DEBUG
    lastfunctime = 4;
#endif

    // WON ADD
    if (fd < 0)
    {
        print("\n SendMesg fd err %s:%d!! ==> func(%d)\n", file, line, func);
        return;
    }

    if (strlen(buffer) > 1024 * 64 - 100)
    {
        return;
    }

#ifdef _VERSION_NEW
#ifdef _NEW_NETSENDRECV
    sprintf(t1, "&;%d%s;#;", func + CONNECT_getFuncNum(fd), buffer);
#else
    sprintf(t1, "&;%d%s;#;", func + 23, buffer);
#endif
#else
#ifdef _VERSION_80
    sprintf(t1, "&;%d%s;#;", func + 23, buffer);
#else
    sprintf(t1, "&;%d%s;#;", func, buffer);
#endif
#endif
#ifdef __STONEAGE
#ifdef _NEW_NETSENDRECV
    int len = util_newEncodeMessage(fd, t2, t1
#ifdef _NEW_PACKET_CODE
                                    , func
#endif
                                   );
    lssproto_Send(fd, t2, len);
#else
    util_EncodeMessage(t2, t1);
    lssproto_Send(fd, t2);
#endif
#endif
}

int util_256to64(char *dst, char *src, int len, char *table)
{
    unsigned int dw, dwcounter, i;

    if (!dst || !src || !table)
    {
        return 0;
    }

    dw = 0;
    dwcounter = 0;

    for (i = 0; i < len; i++)
    {
        dw = (((unsigned int)src[i] & 0xff) << ((i % 3) * 2)) | dw;
        dst[ dwcounter++ ] = table[ dw & 0x3f ];
        dw = (dw >> 6);

        if (i % 3 == 2)
        {
            dst[ dwcounter++ ] = table[ dw & 0x3f ];
            dw = 0;
        }
    }

    if (dw)
    {
        dst[ dwcounter++ ] = table[ dw ];
    }

    dst[ dwcounter ] = '\0';
    return dwcounter;
}

// -------------------------------------------------------------------
// Convert 6-bit strings into 8-bit strings, buffers that store these strings
// must have enough space.
//
// arg: dst=6-bit string;  src=8-bit string;  table=mapping table
// ret: 0=failed  >0=bytes converted
int util_64to256(char *dst, char *src, char *table)
{
    unsigned int dw, dwcounter, i;
    char *ptr = NULL;
    dw = 0;
    dwcounter = 0;

    if (!dst || !src || !table)
    {
        return 0;
    }

    for (i = 0; i < strlen(src); i++)
    {
        ptr = (char *) index(table, src[i]);

        if (!ptr)
        {
            return 0;
        }

        if (i % 4)
        {
            dw = ((unsigned int)(ptr - table) & 0x3f) << ((4 - (i % 4)) * 2) | dw;
            dst[ dwcounter++ ] = dw & 0xff;
            dw = dw >> 8;
        }
        else
        {
            dw = (unsigned int)(ptr - table) & 0x3f;
        }
    }

    if (dw)
    {
        dst[ dwcounter++ ] = dw & 0xff;
    }

    dst[ dwcounter ] = '\0';
    return dwcounter;
}

// -------------------------------------------------------------------
// This basically is a 256to64 encoder.  But it shifts the result by key.
//
// arg: dst=6-bit string;  src=8-bit string;  len=src strlen;
//      table=mapping table;  key=rotate key;
// ret: 0=failed  >0=bytes converted
int util_256to64_shr(char *dst, char *src, int len, char *table, char *key)
{
    unsigned int dw, dwcounter, i, j;

    if (!dst || !src || !table || !key)
    {
        return 0;
    }

    if (strlen(key) < 1)
    {
        return 0;    // key can't be empty.
    }

    dw = 0;
    dwcounter = 0;
    j = 0;

    for (i = 0; i < len; i++)
    {
        dw = (((unsigned int)src[i] & 0xff) << ((i % 3) * 2)) | dw;
        dst[ dwcounter++ ] = table[((dw & 0x3f) + key[j]) % 64 ];   // check!
        j++;

        if (!key[j])
        {
            j = 0;
        }

        dw = (dw >> 6);

        if (i % 3 == 2)
        {
            dst[ dwcounter++ ] = table[((dw & 0x3f) + key[j]) % 64 ]; // check!
            j++;

            if (!key[j])
            {
                j = 0;
            }

            dw = 0;
        }
    }

    if (dw)
    {
        dst[ dwcounter++ ] = table[(dw + key[j]) % 64 ];    // check!
    }

    dst[ dwcounter ] = '\0';
    return dwcounter;
}

// -------------------------------------------------------------------
// Decoding function of util_256to64_shr.
//
// arg: dst=8-bit string;  src=6-bit string;  table=mapping table;
//      key=rotate key;
// ret: 0=failed  >0=bytes converted
int util_shl_64to256(char *dst, char *src, char *table, char *key)
{
    unsigned int dw, dwcounter, i, j;
    char *ptr = NULL;

    if (!key || (strlen(key) < 1))
    {
        return 0;    // must have key
    }

    dw = 0;
    dwcounter = 0;
    j = 0;

    if (!dst || !src || !table)
    {
        return 0;
    }

    for (i = 0; i < strlen(src); i++)
    {
        ptr = (char *) index(table, src[i]);

        if (!ptr)
        {
            return 0;
        }

        if (i % 4)
        {
            // check!
            dw = ((((unsigned int)(ptr - table) & 0x3f) + 64 - key[j]) % 64)
                 << ((4 - (i % 4)) * 2) | dw;
            j++;

            if (!key[j])
            {
                j = 0;
            }

            dst[ dwcounter++ ] = dw & 0xff;
            dw = dw >> 8;
        }
        else
        {
            // check!
            dw = (((unsigned int)(ptr - table) & 0x3f) + 64 - key[j]) % 64;
            j++;

            if (!key[j])
            {
                j = 0;
            }
        }
    }

    if (dw)
    {
        dst[ dwcounter++ ] = dw & 0xff;
    }

    dst[ dwcounter ] = '\0';
    return dwcounter;
}

// -------------------------------------------------------------------
// This basically is a 256to64 encoder.  But it shifts the result by key.
//
// arg: dst=6-bit string;  src=8-bit string;  len=src strlen;
//      table=mapping table;  key=rotate key;
// ret: 0=failed  >0=bytes converted
int util_256to64_shl(char *dst, char *src, int len, char *table, char *key)
{
    unsigned int dw, dwcounter, i, j;

    if (!dst || !src || !table || !key)
    {
        return 0;
    }

    if (strlen(key) < 1)
    {
        return 0;    // key can't be empty.
    }

    dw = 0;
    dwcounter = 0;
    j = 0;

    for (i = 0; i < len; i++)
    {
        dw = (((unsigned int)src[i] & 0xff) << ((i % 3) * 2)) | dw;
        dst[ dwcounter++ ] = table[((dw & 0x3f) + 64 - key[j]) % 64 ];  // check!
        j++;

        if (!key[j])
        {
            j = 0;
        }

        dw = (dw >> 6);

        if (i % 3 == 2)
        {
            dst[ dwcounter++ ] = table[((dw & 0x3f) + 64 - key[j]) % 64 ];    // check!
            j++;

            if (!key[j])
            {
                j = 0;
            }

            dw = 0;
        }
    }

    if (dw)
    {
        dst[ dwcounter++ ] = table[(dw + 64 - key[j]) % 64 ];    // check!
    }

    dst[ dwcounter ] = '\0';
    return dwcounter;
}

// -------------------------------------------------------------------
// Decoding function of util_256to64_shl.
//
// arg: dst=8-bit string;  src=6-bit string;  table=mapping table;
//      key=rotate key;
// ret: 0=failed  >0=bytes converted
int util_shr_64to256(char *dst, char *src, char *table, char *key)
{
    unsigned int dw, dwcounter, i, j;
    char *ptr = NULL;

    if (!key || (strlen(key) < 1))
    {
        return 0;    // must have key
    }

    dw = 0;
    dwcounter = 0;
    j = 0;

    if (!dst || !src || !table)
    {
        return 0;
    }

    for (i = 0; i < strlen(src); i++)
    {
        ptr = (char *) index(table, src[i]);

        if (!ptr)
        {
            return 0;
        }

        if (i % 4)
        {
            // check!
            dw = ((((unsigned int)(ptr - table) & 0x3f) + key[j]) % 64)
                 << ((4 - (i % 4)) * 2) | dw;
            j++;

            if (!key[j])
            {
                j = 0;
            }

            dst[ dwcounter++ ] = dw & 0xff;
            dw = dw >> 8;
        }
        else
        {
            // check!
            dw = (((unsigned int)(ptr - table) & 0x3f) + key[j]) % 64;
            j++;

            if (!key[j])
            {
                j = 0;
            }
        }
    }

    if (dw)
    {
        dst[ dwcounter++ ] = dw & 0xff;
    }

    dst[ dwcounter ] = '\0';
    return dwcounter;
}

// -------------------------------------------------------------------
// Swap a integer (4 byte).
// The value "rule" indicates the swaping rule.  It's a 4 byte string
// such as "1324" or "2431".
//
void util_swapint(int *dst, int *src, char *rule)
{
    char *ptr, *qtr;
    int i;
    ptr = (char *) src;
    qtr = (char *) dst;

    for (i = 0; i < 4; i++)
    {
        qtr[rule[i] - '1'] = ptr[i];
    }
}

// -------------------------------------------------------------------
// Xor a string.  Be careful that your string contains '0xff'.  Your
// data may lose.
//
void util_xorstring(char *dst, char *src)
{
    int i;

    if (strlen(src) > 65500)
    {
        return;
    }

    for (i = 0; i < strlen(src); i++)
    {
        dst[i] = src[i] ^ 255;
    }

    dst[i] = '\0';
}

// -------------------------------------------------------------------
// Shift the string right.
//
void util_shrstring(char *dst, char *src, int offs)
{
    char *ptr;

    if (!dst || !src || (strlen(src) < 1))
    {
        return;
    }

    offs = strlen(src) - (offs % strlen(src));
    ptr = src + offs;
    strcpy(dst, ptr);
    strncat(dst, src, offs);
    dst[strlen(src)] = '\0';
}

// -------------------------------------------------------------------
// Shift the string left.
//
void util_shlstring(char *dst, char *src, int offs)
{
    char *ptr;

    if (!dst || !src || (strlen(src) < 1))
    {
        return;
    }

    offs = offs % strlen(src);
    ptr = src + offs;
    strcpy(dst, ptr);
    strncat(dst, src, offs);
    dst[strlen(src)] = '\0';
}

// -------------------------------------------------------------------
// Convert a message slice into integer.  Return a checksum.
//
// arg: sliceno=slice index in MesgSlice    value=result
// ret: checksum, this value must match the one generated by util_mkint
int util_deint(int fd, int sliceno, int *value)
{
#ifdef _NEW_PACKET_CODE
    char t3[4096];
    base64Decode(MesgSlice[sliceno],  t3);
    return *value = *(int *)t3;
#else
    int *t1, t2;
    char t3[4096];    // This buffer is enough for an integer.

    if (strlen(PersonalKey) == 0)
    {
        strcpy(PersonalKey, getRandDefaultKey(fd));
    }

    util_shl_64to256(t3, MesgSlice[sliceno], DEFAULTTABLE, PersonalKey);
    t1 = (int *) t3;
    t2 = *t1 ^ 0xffffffff;
    util_swapint(value, &t2, "2413");
    return *value;
#endif
}

int util_mkint(int fd, char *buffer, int value)
{
#ifdef _NEW_PACKET_CODE
    char t3[4096];
    base64Encode((unsigned char *)&value, t3, sizeof(int));
    strcat(buffer, ";");
    strcat(buffer, t3);
    return value;
#else
    int t1, t2;
    char t3[4096];

    if (strlen(PersonalKey) == 0)
    {
        strcpy(PersonalKey, getRandDefaultKey(fd));
    }

    util_swapint(&t1, &value, "3142");
    t2 = t1 ^ 0xffffffff;
    util_256to64_shr(t3, (char *) &t2, sizeof(int), DEFAULTTABLE, PersonalKey);
    strcat(buffer, ";");
    strcat(buffer, t3);
    return value;
#endif
}

// -------------------------------------------------------------------
// Convert a message slice into string.  Return a checksum.
//
// arg: sliceno=slice index in MesgSlice    value=result
// ret: checksum, this value must match the one generated by util_mkstring
int util_destring(int fd, int sliceno, char *value)
{
#ifdef _NEW_PACKET_CODE
    char t3[4096];
    int len = base64Decode(MesgSlice[sliceno], t3);
    strcpy(value, t3);
    return len;
#else

    if (strlen(PersonalKey) == 0)
    {
        strcpy(PersonalKey, getRandDefaultKey(fd));
    }

    util_shr_64to256(value, MesgSlice[sliceno], DEFAULTTABLE, PersonalKey);
    return strlen(value);
#endif
}

// -------------------------------------------------------------------
// Convert a string into buffer (a string).  Return a checksum.
//
// arg: buffer=output   value=data to pack
// ret: checksum, this value must match the one generated by util_destring
int util_mkstring(int fd, char *buffer, char *value)
{
#ifdef _NEW_PACKET_CODE
    char t1[SLICE_SIZE];
    base64Encode(value, t1, strlen(value));
    strcat(buffer, ";");  // It's important to append a SEPARATOR between fields
    strcat(buffer, t1);
    return strlen(value);
#else
    char t1[SLICE_SIZE];

    if (strlen(PersonalKey) == 0)
    {
        strcpy(PersonalKey, getRandDefaultKey(fd));
    }

    util_256to64_shl(t1, value, strlen(value), DEFAULTTABLE, PersonalKey);
    strcat(buffer, ";");  // It's important to append a SEPARATOR between fields
    strcat(buffer, t1);
    return strlen(value);
#endif
}

int StrReplace(char* strRes, char* from, char* to)
{
    char ts[1024 * 5];
    int i, flag = 0;
    char *p, *q;

    for (i = 0; strRes[i]; ++i)
    {
        if (strRes[i] == from[0])
        {
            p = strRes + i;
            q = from;

            while (*q && (*p++ == *q++));

            if (*q == '\0')
            {
                //ts = (char *)malloc(strlen(strRes) + 1);
                strcpy(ts, p);
                strRes[i] = '\0';
                strcat(strRes, to);
                strcat(strRes, ts);
                //free(ts);
                flag = 1;
            }
        }
    }

    return flag;
}


/**将字符转换为ASCII值**/
static int chartoasc(char c)
{
    int i = 0;
    i = c;
    return i;
}

/**将ASCII进行异或运算，产生新的ASCII值**/
static int xor(int i)
{
    int m = 27;
    int result = 0;

    if (59 == i || 100 == i)
    {
        return i;
    }

    result = i ^ m;
    return result;
}

/**将ASCII值转换为字符**/
static char asctochar(int a)
{
    char c;
    c = a;
    return c;
}

/**输入原密码产生新的密码**/
char* encryptStr(const char *org_pass)
{
    char org_password[1024 * 8];
    char new_password[1024 * 8];
    int len = 0;
    int i = 0;
    int asc = 0;
    char ch = 0;
    int x = 0;
    memset(org_password, 0, sizeof(org_password));
    memset(new_password, 0, sizeof(new_password));
    strcpy(org_password, org_pass);
    len = strlen(org_password);

    for (i = 0; i < len; i++)
    {
        ch = org_password[i];
        asc = chartoasc(ch);
        x = xor(asc);
        new_password[i] = asctochar(x);
    }

    return new_password;
}

/**输入加密后的密码返回原密码**/
char* decryptStr(const char *new_pass)
{
    char new_password[1024 * 8];
    char org_password[1024 * 8];
    char ch;
    int a = -1;
    int len = 0;
    int i = 0;
    int x = -1;
    memset(new_password, 0, sizeof(new_password));
    memset(org_password, 0, sizeof(org_password));
    strcpy(new_password, new_pass);
    len = strlen(new_password);

    for (i = 0; i < len; i++)
    {
        ch = new_password[i];
        a = chartoasc(ch);
        x = xor(a);
        org_password[i] = asctochar(x);
    }

    return org_password;
}

BOOL checkStringErr(char *checkstring)
{
    int i, ach;

    for (i = 0, ach = 0; i < strlen(checkstring); i++)
    {
        if ((unsigned char)checkstring[i] == 0xff)
        {
            ach = 1;    // Force no 0xff
            break;
        }

        if ((unsigned char)checkstring[i] == 0x80)
        {
            ach = 1;    // Force no 0x80
            break;
        }

        if ((unsigned char)checkstring[i] == 0x7f)
        {
            ach = 1;    // Force no 0x7f
            break;
        }

        if ((unsigned char)checkstring[i] <= 0x20)
        {
            ach = 1;    // Force greater than 0x20
            break;
        }

        if ((unsigned char)checkstring[i] == 0x27)
        {
            ach = 1;    // Force greater than 0x27
            break;
        }

        if ((unsigned char)checkstring[i] == 0x3b)
        {
            ach = 1;    // Force greater than 0x3b
            break;
        }

        if ((unsigned char)checkstring[i] == 0x2c)
        {
            ach = 1;    // Force greater than 0x2c
            break;
        }

        if ((unsigned char)checkstring[i] == 0x3d)
        {
            ach = 1;    // Force greater than 0x2c
            break;
        }

        if (ach)
        {
            if ((((unsigned char)checkstring[i] >= 0x40) && ((unsigned char)checkstring[i] <= 0x7e)) ||
                    (((unsigned char)checkstring[i] >= 0xa1) && ((unsigned char)checkstring[i] <= 0xfe)))
            {
                ach = 0;
            }
        }
        else
        {
            if (((unsigned char)checkstring[i] >= 0xa1) && ((unsigned char)checkstring[i] <= 0xfe))
            {
                ach = 1;
            }
        }
    }

    if (ach)
    {
        print(" StringDog! ");
        return  TRUE;
    }

    return FALSE;
}

int NextUTF8CharSize(char* src)
{
    int result = 0;
    char ch = *src;

    if ((ch & 0xF0) == 0xE0)
    {
        result = 3;
    }
    else if ((ch & 0x80) == 0x00)
    {
        result = 1;
    }
    else if ((ch & 0xE0) == 0xC0)
    {
        result = 2;
    }
    else if ((ch & 0xF8) == 0xF0)
    {
        result = 4;
    }
    else if ((ch & 0xFC) == 0xF8)
    {
        result = 5;
    }
    else if ((ch & 0xFE) == 0xFC)
    {
        result = 6;
    }

    return result;
}

int CopyOneUTF8Char(char* src, char*dest)
{
    int result = 0;
    char ch = *src;

    if (ch == 0)
    {
        return 0;
    }
    else if ((ch & 0xF0) == 0xE0)
    {
        result = 3;
    }
    else if ((ch & 0x80) == 0x00)
    {
        result = 1;
    }
    else if ((ch & 0xE0) == 0xC0)
    {
        result = 2;
    }
    else if ((ch & 0xF8) == 0xF0)
    {
        result = 4;
    }
    else if ((ch & 0xFC) == 0xF8)
    {
        result = 5;
    }
    else if ((ch & 0xFE) == 0xFC)
    {
        result = 6;
    }

    if (result > 0 && dest != NULL)
    {
        memcpy(dest, src, result);
        dest[result] = '\0';
    }

    return result;
}
