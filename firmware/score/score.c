#include "score.h"

#include <string.h>

static read_mac_t g_rm = NULL;
static copy_t g_copy = NULL;
static rng_t g_rng = NULL;
static crc_t g_crc = NULL;
static enc_t g_enc = NULL;
static unsigned int g_kx_size = 0;
static unsigned char g_k[16] = { 0 };
static unsigned char g_k2[16] = { 0 };


void set_kx_size(unsigned int size)
{
    g_kx_size = size;
}

void set_k(const char *key)
{
    if ( !g_copy )
    {
        return;
    }
    g_copy(g_k, key, g_kx_size);
}

void set_k2(const char *key)
{
    if ( !g_copy )
    {
        return;
    }
    g_copy(g_k2, key, g_kx_size);
}

void set_rm(read_mac_t fn)
{
    g_rm = fn;
}

void set_copy(copy_t fn)
{
    g_copy = fn;
}

void set_rng(rng_t fn)
{
    g_rng = fn;
}

void set_crc(crc_t fn)
{
    g_crc = fn;
}

void set_enc(enc_t fn)
{
    g_enc = fn;
}

static unsigned int get_id()
{
    if ( NULL == g_rm )
    {
        return 0;
    }
    unsigned char m[6] = { 0 };
    g_rm(m);
    unsigned int devid = 0;
    devid |= (((unsigned int)m[5]) <<  0) & 0x000000ff;
    devid |= (((unsigned int)m[4]) <<  8) & 0x0000ff00;
    devid |= (((unsigned int)m[3]) << 16) & 0x00ff0000;
    devid |= (((unsigned int)m[2]) << 24) & 0xff000000;
    return devid;
}

static unsigned int sl(const char *s)
{
    unsigned int c = 0;
    while ( *s )
    {
        s++;
        c++;
    }
    return c;
}

void get_mk(unsigned char *key, unsigned int size)
{
    if ( 16 != size)
    {
        return;
    }
    if ( !g_copy )
    {
        return;
    }
    g_copy(key, g_k, 16);
    for (unsigned char i = 0; i < 16; i++) {
        key[i] ^= i;
        key[i] ^= 0xaa;
        key[i] ^= g_k2[i];
    }
}

char fb64_char(unsigned char prev, unsigned char value) {
    if (0 != (prev & 0x1)) {
        if (0 == (prev & 0x2)) {
            if (value < 10) {
                return '0' + value;
            } else if (value == 10) {
                return '_';
            } else if (value == 11) {
                return '-';
            } else if (value > 11) {
                return 'N' + (value - 11);
            }
        } else {
            if (0 == (prev & 0x4)) {
                return 'K' + value;
            } else {
                return 'k' + value;
            }
        }
    }
    else {
        if (0 == (prev & 0x4)) {
            return 'A' + value;
        } else {
            return 'a' + value;
        }
    }
}

void fb64_encode(char *buffer, unsigned int buffer_size, const unsigned char *data, unsigned int data_size)
{
    unsigned char temp[21];
    if (buffer_size <= 0)
    {
        return;
    }
    if ((data_size != 16) || (buffer_size < 45))
    {
        buffer[0] = '\0';
        return;
    }
    if ( !g_copy )
    {
        buffer[0] = '\0';
        return;
    }
    g_copy(&(temp[0]), data, 16);
    if ( !g_rng )
    {
        buffer[0] = '\0';
        return;
    }
    g_rng(&(temp[16]), 1);
    if ( !g_crc )
    {
        buffer[0] = '\0';
        return;
    }
    g_crc(&(temp[0]), 17, (unsigned int*)(&(temp[17])));
    unsigned char prev = 0;
    unsigned char value = 0;
    unsigned int j = 0;
    for (unsigned int i = 0; i < 21; i++) {
        value = (temp[i] >> 4) & 0xf;
        buffer[j] = fb64_char(prev, value);
        j++;
        prev = value;
        value = (temp[i] >> 0) & 0xf;
        buffer[j] = fb64_char(prev, value);
        j++;
        prev = value;
    }
    buffer[42] = buffer[prev];
    buffer[43] = '=';
    buffer[44] = '\0';
}

const char *get_surl(char *buf, unsigned int size, const char *url, unsigned char game, unsigned int score)
{
    char *surl = buf;
    if ( (sl(url) + 1) >= (size) )
    {
        return url;
    }
    if ( !g_copy )
    {
        return url;
    }
    g_copy(buf, url, sl(url));
    buf += sl(url);
    size -= sl(url);
    if ( size < 4 )
    {
        return url;
    }
    buf[0] = '?';
    buf[1] = 't';
    buf[2] = '=';
    buf[3] = '\0';
    buf += 3;
    size -= 3;
    if ( !g_rm )
    {
        return url;
    }
    unsigned int devid = get_id();
    unsigned char token[16];
    g_copy(token + 0, &devid, 4);
    g_copy(token + 4, &score, 4);
    g_copy(token + 8, &game, 1);
    if ( !g_rng )
    {
        return url;
    }
    g_rng(&(token[9]), 3);
    g_copy(token + 12, &devid, 4);
    unsigned int crc32val = 0;
    if ( !g_crc )
    {
        return url;
    }
    g_crc(token, 16, &crc32val);
    g_copy(&token[12], &crc32val, 4);
    unsigned char key[16];
    get_mk(key, sizeof(key));
    if ( !g_enc )
    {
        return url;
    }
    g_enc(key, token, sizeof(token));
    if ( size < 48 )
    {
        return url;
    }
    fb64_encode(buf, size, token, sizeof(token));
    return surl;
}
