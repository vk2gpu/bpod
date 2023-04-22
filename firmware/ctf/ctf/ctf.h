#ifndef _H_CTF_H_
#define _H_CTF_H_

#ifndef CTF_NO_CPP
#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#else
#include <stdint.h>
#include <stddef.h>
#endif

typedef uint32_t(*tmr_millis_t)(void);
typedef void(*i2c_send_t)(uint8_t,char*,uint32_t);

typedef struct ctf_data {
    int init;
    int on;
    tmr_millis_t tmr_millis;
    i2c_send_t i2c_send;
    uint32_t timestamp_prev;
} ctf_data_t;

#define CTF_SDA_ON  digitalWrite(8, HIGH)
#define CTF_SDA_OFF digitalWrite(8, LOW)
#define CTF_SCL_ON  digitalWrite(9, HIGH)
#define CTF_SCL_OFF digitalWrite(9, LOW)
#define CTF_DLY     delay(1)

#define CTF_DEFINE_MACRO \
    ctf_data_t ctf_data = { 0 }; \
    static uint32_t tmr_millis() \
    { \
        return (uint32_t)millis(); \
    } \
    static void i2c_send(uint8_t addr, char *data, uint32_t size) \
    { \
        pinMode(8, OUTPUT); \
        pinMode(9, OUTPUT); \
        CTF_SDA_ON; \
        CTF_SCL_ON; \
        CTF_DLY; \
        CTF_SDA_OFF; \
        CTF_DLY; \
        CTF_SCL_OFF; \
        CTF_DLY; \
 \
        uint8_t dat = (addr << 1) | 0; \
        for(uint8_t i = 8; i; i--){ \
            (dat & 0x80) ? CTF_SDA_ON : CTF_SDA_OFF; \
            dat<<=1; \
            CTF_DLY; \
            CTF_SCL_ON; \
            CTF_DLY; \
            CTF_SCL_OFF; \
            CTF_DLY; \
        } \
        CTF_SDA_OFF; \
        CTF_DLY; \
        CTF_SCL_ON; \
        CTF_DLY; \
        CTF_SCL_OFF; \
        CTF_DLY; \
\
        for ( uint32_t i = 0; i < size; i++ ) \
        { \
            uint8_t dat = data[i]; \
            for(uint8_t bit = 8; bit; bit--){ \
                (dat & 0x80) ? CTF_SDA_ON : CTF_SDA_OFF; \
                dat<<=1; \
                CTF_DLY; \
                CTF_SCL_ON; \
                CTF_DLY; \
                CTF_SCL_OFF; \
                CTF_DLY; \
            } \
            CTF_SDA_OFF; \
            CTF_DLY; \
            CTF_SCL_ON; \
            CTF_DLY; \
            CTF_SCL_OFF; \
            CTF_DLY; \
        } \
 \
        CTF_SDA_OFF; \
        CTF_DLY; \
        CTF_SCL_ON; \
        CTF_DLY; \
        CTF_SDA_ON; \
        CTF_DLY; \
    } \
    static uint8_t i2c_addr; \
    static char i2c_data[128]; \
    static uint32_t i2c_size; \
    static pthread_t i2c_thread; \
    static int i2c_thread_init = 0; \
    static void *i2c_send_thread(void *ctx) \
    { \
        i2c_send(i2c_addr, i2c_data, i2c_size); \
        memset(i2c_data, 0, sizeof(i2c_data)); \
        return NULL; \
    } \
    static void i2c_send_async(uint8_t addr, char *data, uint32_t size) \
    { \
        i2c_addr = addr; \
        memcpy(i2c_data, data, sizeof(i2c_data)); \
        i2c_size = size; \
        if ( i2c_thread_init ) pthread_join(i2c_thread, NULL); \
        i2c_thread_init = 1; \
        pthread_create(&i2c_thread, NULL, i2c_send_thread, NULL); \
    }

#define CTF_INIT_MACRO ctf_init(&ctf_data, tmr_millis, i2c_send_async);
#define CTF_ON_MACRO ctf_on(ctf_get_data());
#define CTF_OFF_MACRO ctf_off(ctf_get_data());

#ifdef __cplusplus
extern "C" {
#endif

void ctf_init(ctf_data_t *ctf_data, tmr_millis_t tmr_millis, i2c_send_t i2c_send);
void ctf_tick(ctf_data_t *ctf_data);
ctf_data_t *ctf_get_data();
void ctf_on(ctf_data_t *ctf_data);
void ctf_off(ctf_data_t *ctf_data);

#ifdef __cplusplus
}
#endif

#endif
