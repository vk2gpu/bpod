#include "stringdb.h"

#include <tinyaes.h>
#include <stringdb_data.h>

static char cache_buf[STRINGDB_MAX_STRING_LEN + 1];
static size_t cache_mode = 0;
static size_t cache_pos = 0;

size_t sdbs(size_t start, size_t end)
{
    if ( ( start > STRINGDB_DATA_SIZE ) || ( end > STRINGDB_DATA_SIZE ) )
    {
        return 0;
    }
    if ( end < start )
    {
        return 0;
    }
    return end - start;
}

void sdbm(size_t mode)
{
    cache_mode = mode;
    if ( 1 == cache_mode )
    {
        cache_pos = 0;
    }
    if ( 0 == cache_mode )
    {
        cache_pos = 0;
    }
}

static void sdec(uint8_t *data, uint32_t offset, uint32_t size)
{
    struct AES_ctx aes_ctx_tmp;
    uint8_t iv[16];
    uint8_t key[16];
    uint8_t mask[16];

    memset(key, 0, sizeof(key));
    memcpy(&key[0], &offset, 4);
    memcpy(&key[4], &size, 4);
    memcpy(iv, key, 16);
    memcpy(mask, key, 16);

    memset(&aes_ctx_tmp, 0, sizeof(aes_ctx_tmp));
    AES_init_ctx_iv(&aes_ctx_tmp, key, iv);
    AES_CBC_encrypt_buffer(&aes_ctx_tmp, mask, sizeof(mask));

    for ( size_t i = 0; i < size; i++ )
    {
        data[i] = data[i] ^ mask[i & 0xf];
        mask[i & 0xf] = (mask[i & 0xf] + i) & 0xff;
    }
}

const char *sdbg(size_t start, size_t end)
{
    size_t size = sdbs(start, end);
    char *buf = NULL;
    if (0 == size )
    {
        return "";
    }
    if ( size > (sizeof(cache_buf) - 1 - cache_pos) )
    {
        size = (sizeof(cache_buf) - 1 - cache_pos);
    }
    buf = &cache_buf[cache_pos];
    memcpy(buf, STRINGDB_DATA + start, size);
    sdec((uint8_t*)buf, (uint32_t)start, (uint32_t)(end - start));
    buf[size] = '\0';
    if ( 1 == cache_mode )
    {
        cache_pos += (size + 1);
        if ( cache_pos > (sizeof(cache_buf) - 1) )
        {
            cache_pos = sizeof(cache_buf) - 1;
        }
    }
    return buf;
}
