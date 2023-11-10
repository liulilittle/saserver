#include "version.h"
#include "autil.h"
#include "lssproto_serv.h"
#include "common.h"
#include <stdio.h>
#include "buf.h"
#include "md5_encrypt.h"
#include "shop.h"
#ifdef _NEW_SHOP_

typedef struct tagShopData
{
    char shopMd5[64];
    char *shopData;
} tagShopData1;

tagShopData1 shopData[5];

void ShopData_Init()
{
    memset(&shopData, 0, sizeof(tagShopData1) * 5);
}

void WriteShopData(char *data, int id)
{
    char szDigest[17];
    int len = strlen(data) + 1;

    if (shopData[id].shopData)
    {
        freeMemory(shopData[id].shopData);
        shopData[id].shopData = NULL;
    }

    if (*data != 0)
    {
        shopData[id].shopData = (char*)allocateMemory(len);
        memset(shopData[id].shopData, 0, len);
        memcpy(shopData[id].shopData, data, len);
        char *md5 = MD5String(shopData[id].shopData);
        strcpy(shopData[id].shopMd5, md5);
    }
    else
    {
        shopData[id].shopMd5[0] = 0;
    }
}


void lssproto_upshopdata_recv(int fd, char shop[5][1502])
{
    int i = 0;

    for (i; i < 5; i++)
    {
        if (*shopData[i].shopMd5)
        {
            if (strcmp(shopData[i].shopMd5, shop[i]) || !shop[i][0])
            {
                if (shopData[i].shopData)
                {
                    lssproto_UpShopData_send(fd, shopData[i].shopData, shopData[i].shopMd5, i);
                }
            }
        }
    }

    lssproto_ShopOK_send(fd);
}
#endif // _NEW_SHOP_




