#include "stringdb.h"

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
