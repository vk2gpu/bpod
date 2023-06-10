#include "githash.h"

#include <stdio.h>
#include <stringdb.h>
#include <githash_gen.h>

static const char *g_githash = GITHASH_GEN_DATA;
static char g_githash_hex[9];

const char *githash()
{
    for ( size_t i = 0; i < 4; i++ )
    {
        snprintf(&g_githash_hex[i * 2], 3, STRING(FMT_HEX_BYTE_LOWER), g_githash[i]);
    }
    return g_githash_hex;
}
