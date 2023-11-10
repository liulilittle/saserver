/***********************************************************************

 Copyright 2006-2009 Ma Bingyao
 Copyright 2013 Gao Chunhui, Liu Tao

 These sources is free software. Redistributions of source code must
 retain the above copyright notice. Redistributions in binary form
 must reproduce the above copyright notice. You can redistribute it
 freely. You can use it with any free or commercial software.

 These sources is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY. Without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

 github: https://github.com/liut/pecl-xxtea

 *************************************************************************/

#include "xxtea.h"
#include <memory.h>
#include <stdlib.h>



static char * initBuffer(unsigned int len)
{
    static char *buffer = NULL;
    static unsigned int bufferLen = 0;

    if (buffer == NULL)
    {
        //生成
        buffer = (char *)malloc(len);
        bufferLen = len;
    }
    else if (bufferLen < len)
    {
        free(buffer);
        buffer = (char *)malloc(len);
        bufferLen = len;
    }

    return buffer;
}


static void xxtea_long_encrypt(xxtea_long *v, xxtea_long len, xxtea_long *k)
{
    xxtea_long n = len - 1;
    xxtea_long z = v[n], y = v[0], p, q = 6 + 52 / (n + 1), sum = 0, e;

    if (n < 1)
    {
        return;
    }

    while (0 < q--)
    {
        sum += XXTEA_DELTA;
        e = sum >> 2 & 3;

        for (p = 0; p < n; p++)
        {
            y = v[p + 1];
            z = v[p] += XXTEA_MX;
        }

        y = v[0];
        z = v[n] += XXTEA_MX;
    }
}

static void xxtea_long_decrypt(xxtea_long *v, xxtea_long len, xxtea_long *k)
{
    xxtea_long n = len - 1;
    xxtea_long z = v[n], y = v[0], p, q = 6 + 52 / (n + 1), sum = q * XXTEA_DELTA, e;

    if (n < 1)
    {
        return;
    }

    while (sum != 0)
    {
        e = sum >> 2 & 3;

        for (p = n; p > 0; p--)
        {
            z = v[p - 1];
            y = v[p] -= XXTEA_MX;
        }

        z = v[n];
        y = v[0] -= XXTEA_MX;
        sum -= XXTEA_DELTA;
    }
}

static void fix_key_length(unsigned char *key, xxtea_long key_len, unsigned char *newkey)
{
    memcpy(newkey, key, key_len);
    memset(newkey + key_len, '\0', 16 - key_len);
}

static xxtea_long *xxtea_to_long_array(unsigned char *data, xxtea_long len, int include_length, xxtea_long *ret_len, char *raw)
{
    xxtea_long i, n, *result;
    n = len >> 2;
    n = (((len & 3) == 0) ? n : n + 1);

    if (include_length)
    {
        result = (xxtea_long *)raw;
        result[n] = len;
        *ret_len = n + 1;
    }
    else
    {
        result = (xxtea_long *)raw;
        *ret_len = n;
    }

    memset(result, 0, n << 2);

    for (i = 0; i < len; i++)
    {
        result[i >> 2] |= (xxtea_long)data[i] << ((i & 3) << 3);
    }

    return result;
}

static int xxtea_to_byte_array(xxtea_long *data, xxtea_long len, int include_length, unsigned char *ret, xxtea_long *ret_len)
{
    xxtea_long i, n, m;
    n = len << 2;

    if (include_length)
    {
        m = data[len - 1];

        if ((m < n - 7) || (m > n - 4))
        {
            ret_len = 0;
            return 0;
        }

        n = m;
    }

    if (*ret_len < n)
    {
        *ret_len = n;
        return 0;
    }

    *ret_len = n;

    for (i = 0; i < n; i++)
    {
        ret[i] = (unsigned char)((data[i >> 2] >> ((i & 3) << 3)) & 0xff);
    }

    return 1;
}

static int do_xxtea_encrypt(unsigned char *data, xxtea_long len, unsigned char *key, unsigned char *ret, xxtea_long *ret_len)
{
    char raw1[128];
    int result;
    xxtea_long *v, *k, v_len, k_len;
    int n = (((len & 3) == 0) ? (len >> 2) : (len >> 2) + 1);
    char *buffer = initBuffer((n + 1) << 2);
    v = xxtea_to_long_array(data, len, 1, &v_len, buffer);
    k = xxtea_to_long_array(key, 16, 0, &k_len, raw1);
    xxtea_long_encrypt(v, v_len, k);
    result = xxtea_to_byte_array(v, v_len, 0, ret, ret_len);
    return result;
}

static int do_xxtea_decrypt(unsigned char *data, xxtea_long len, unsigned char *key, unsigned char *ret, xxtea_long *ret_len)
{
    int result;
    char raw1[128];
    xxtea_long *v, *k, v_len, k_len;
    int n = (((len & 3) == 0) ? (len >> 2) : (len >> 2) + 1);
    char *buffer = initBuffer((n + 1) << 2);
    v = xxtea_to_long_array(data, len, 0, &v_len, buffer);
    k = xxtea_to_long_array(key, 16, 0, &k_len, raw1);
    xxtea_long_decrypt(v, v_len, k);
    result = xxtea_to_byte_array(v, v_len, 1, ret, ret_len);
    return result;
}

int xxtea_encrypt(unsigned char *data, xxtea_long data_len, unsigned char *key, xxtea_long key_len, unsigned char *ret, xxtea_long *ret_length)
{
    int result;

    if (key_len < 16)
    {
        unsigned char key2[16];
        fix_key_length(key, key_len, key2);
        result = do_xxtea_encrypt(data, data_len, key2, ret, ret_length);
    }
    else
    {
        result = do_xxtea_encrypt(data, data_len, key, ret, ret_length);
    }

    return result;
}

int xxtea_decrypt(unsigned char *data, xxtea_long data_len, unsigned char *key, xxtea_long key_len, unsigned char *ret, xxtea_long *ret_length)
{
    int result;

    if (key_len < 16)
    {
        unsigned char key2[16];
        fix_key_length(key, key_len, key2);
        result = do_xxtea_decrypt(data, data_len, key2, ret, ret_length);
    }
    else
    {
        result = do_xxtea_decrypt(data, data_len, key, ret, ret_length);
    }

    return result;
}

/* }}} */
