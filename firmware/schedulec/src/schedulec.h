#ifndef _H_SCHEDULE_C_H_
#define _H_SCHEDULE_C_H_

#include <stdint.h>
#include <string.h>
#include <limits.h>
#include <stddef.h>

#include <Arduino.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SCHEDULE_DAY_VALUE_ID           0
#define SCHEDULE_TIME_VALUE_ID          1
#define SCHEDULE_TITLE_VALUE_ID         2
#define SCHEDULE_PRESENTER_VALUE_ID     3
#define SCHEDULE_ABSTRACT_VALUE_ID      4
#define SCHEDULE_BIO_VALUE_ID           5

size_t schedule_get_talk_count(void);
size_t schedule_get_talk_value_size(size_t talk_index, size_t value_id);
size_t schedule_get_talk_value(size_t talk_index, size_t value_id, size_t offset, char *text, size_t size);
size_t schedule_get_talk_text_size(size_t talk_index);
size_t schedule_get_talk_text(size_t talk_index, size_t offset, char *text, size_t size);

#ifdef __cplusplus
}
#endif

#endif
