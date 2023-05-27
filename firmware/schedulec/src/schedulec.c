#include "schedulec.h"

#include <stringdb.h>

// because we are operating in C (not C++)
#ifdef PROGMEM
#undef PROGMEM
#endif
#define PROGMEM
#ifdef pgm_read_byte
#undef pgm_read_byte
#endif
#define pgm_read_byte(addr)     (*((uint8_t*)(addr)))

// generated
#include <schedule_data.h>

static void read_data(size_t offset, void *data, size_t size)
{
    if ( !data || size == 0)
    {
        return;
    }
    if ( (offset + size) > schedule_size )
    {
        memset(data, 0, size);
        return;
    }
    for ( size_t i = 0; i < size; i++ )
    {
        ((uint8_t*)data)[i] = (uint8_t)pgm_read_byte(&schedule_data[offset + i]);
        ((uint8_t*)data)[i] = ((uint8_t*)data)[i] ^ ((((offset + i) & 0x1f) ^ 0xd5) & 0xff);
    }
}

static size_t read_value(size_t offset)
{
    uint8_t data[2] = {0};
    read_data(offset, data, sizeof(data));

    // packed big-endian >H in python script
    size_t value = 0;
    value |= (((size_t)(data[0])) << 8) & 0xff00;
    value |= (((size_t)(data[1])) << 0) & 0x00ff;
    return value;
}

static size_t read_table_value(size_t table_offset, size_t talk_index, size_t value_id)
{
    size_t offset = 0;

    // into table
    offset += table_offset;

    // into talk (each value is packed >H in python)
    offset += talk_index * (schedule_value_count * sizeof(uint16_t));

    // into value
    offset += value_id * sizeof(uint16_t);

    // read value in table
    return read_value(offset);
}

size_t schedule_get_talk_count()
{
    return schedule_count;
}

size_t schedule_get_talk_value_size(size_t talk_index, size_t value_id)
{
    if ( talk_index >= schedule_count )
    {
        return 0;
    }
    if ( value_id >= schedule_value_count)
    {
        return 0;
    }
    return read_table_value(schedule_size_table_offset, talk_index, value_id);
}

size_t schedule_get_talk_value(size_t talk_index, size_t value_id, size_t offset, char *text, size_t size)
{
    if ( !text || size == 0 )
    {
        return 0;
    }
    if ( talk_index >= schedule_count )
    {
        return 0;
    }
    if ( value_id >= schedule_value_count)
    {
        return 0;
    }

    // cap at value size
    size_t value_size = read_table_value(schedule_size_table_offset, talk_index, value_id);
    if ( offset >= value_size )
    {
        // can't read past the value size
        return 0;
    }
    if ( size >= (value_size - offset) )
    {
        // size is greater than whats left - cap it.
        size = value_size - offset;
    }

    // move the offset into the value
    offset += read_table_value(schedule_offset_table_offset, talk_index, value_id);

    // read data
    read_data(offset, text, size);

    // done
    return size;
}

size_t schedule_get_talk_text_size(size_t talk_index)
{
    size_t size = 0;

    size += schedule_get_talk_value_size(talk_index, SCHEDULE_DAY_VALUE_ID);
    size += STRING_STRLEN(SCHEDULE_DAY_TIME_SEP);
    size += schedule_get_talk_value_size(talk_index, SCHEDULE_TIME_VALUE_ID);
    size += STRING_STRLEN(SCHEDULE_NL_SEP);;
    size += schedule_get_talk_value_size(talk_index, SCHEDULE_TITLE_VALUE_ID);
    size += STRING_STRLEN(SCHEDULE_NL_SEP);;
    size += schedule_get_talk_value_size(talk_index, SCHEDULE_PRESENTER_VALUE_ID);
    size += STRING_STRLEN(SCHEDULE_NLNL_SEP);;
    size += schedule_get_talk_value_size(talk_index, SCHEDULE_ABSTRACT_VALUE_ID);
    if ( schedule_get_talk_value_size(talk_index, SCHEDULE_BIO_VALUE_ID) )
    {
        size += STRING_STRLEN(SCHEDULE_TALK_BIO_SEP);
        size += schedule_get_talk_value_size(talk_index, SCHEDULE_BIO_VALUE_ID);
    }

    return size;
}

size_t schedule_get_talk_text(size_t talk_index, size_t offset, char *text, size_t size)
{
    // TODO: offset is not supported
    if ( 0 != offset )
    {
        return 0;
    }

    // TODO: partial gets not supported
    if ( size < schedule_get_talk_text_size(talk_index) )
    {
        return 0;
    }

    size_t recd = 0;

    recd += schedule_get_talk_value(talk_index, SCHEDULE_DAY_VALUE_ID, 0, text + recd, size - recd);
    STRING_MEMCPY(text + recd, SCHEDULE_DAY_TIME_SEP);
    recd += STRING_STRLEN(SCHEDULE_DAY_TIME_SEP);
    recd += schedule_get_talk_value(talk_index, SCHEDULE_TIME_VALUE_ID, 0, text + recd, size - recd);
    STRING_MEMCPY(text + recd, SCHEDULE_NL_SEP);
    recd += STRING_STRLEN(SCHEDULE_NL_SEP);
    recd += schedule_get_talk_value(talk_index, SCHEDULE_TITLE_VALUE_ID, 0, text + recd, size - recd);
    STRING_MEMCPY(text + recd, SCHEDULE_NL_SEP);
    recd += STRING_STRLEN(SCHEDULE_NL_SEP);
    recd += schedule_get_talk_value(talk_index, SCHEDULE_PRESENTER_VALUE_ID, 0, text + recd, size - recd);
    STRING_MEMCPY(text + recd, SCHEDULE_NLNL_SEP);
    recd += STRING_STRLEN(SCHEDULE_NLNL_SEP);
    recd += schedule_get_talk_value(talk_index, SCHEDULE_ABSTRACT_VALUE_ID, 0, text + recd, size - recd);
    if ( schedule_get_talk_value_size(talk_index, SCHEDULE_BIO_VALUE_ID) )
    {
        STRING_MEMCPY(text + recd, SCHEDULE_TALK_BIO_SEP);
        recd += STRING_STRLEN(SCHEDULE_TALK_BIO_SEP);
        recd += schedule_get_talk_value(talk_index, SCHEDULE_BIO_VALUE_ID, 0, text + recd, size - recd);
    }
    return recd;
}
