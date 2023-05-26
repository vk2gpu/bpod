#ifndef _H_STRINGDB_H_
#define _H_STRINGDB_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include <stringdb_macro.h>

const char *sdbg(size_t start, size_t end);
size_t sdbs(size_t start, size_t end);
void sdbm(size_t mode);
#define STRING(index) sdbg(SDB_START_##index, SDB_END_##index)
#define STRING_CACHE() sdbm(1)
#define STRING_CLEAR() sdbm(0)
#define STRING_STRLEN(index) sdbs(SDB_START_##index, SDB_END_##index)
#define STRING_MEMCPY(dest, index) memcpy(dest, STRING(index), STRING_STRLEN(index))
#define STRING_STRCPY(dest, index) memcpy(dest, STRING(index), STRING_STRLEN(index) + 1)

#ifdef __cplusplus
}
#endif

#endif
