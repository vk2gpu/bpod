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
typedef void(*uart_send_t)(char*,uint32_t);
typedef void(*i2c_send_t)(uint8_t,char*,uint32_t);

typedef struct ctf_data {
    int init;
    int on;
    int flag_to_send;
    tmr_millis_t tmr_millis;
    i2c_send_t i2c_send;
    uart_send_t uart_send;
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
    static void uart_send(char *data, uint32_t size) \
    { \
        Serial1.begin(9600, SERIAL_8N1); \
        delay(100); \
        Serial1.write(data, size); \
        delay(100); \
        Serial1.end(); \
    } \
    static uint8_t ctf_thread_addr; \
    static char ctf_thread_data[128]; \
    static uint32_t ctf_thread_data_size; \
    static pthread_t ctf_thread; \
    static int ctf_thread_init = 0; \
    static void *i2c_send_thread(void *ctx) \
    { \
        i2c_send(ctf_thread_addr, ctf_thread_data, ctf_thread_data_size); \
        memset(ctf_thread_data, 0, sizeof(ctf_thread_data)); \
        return NULL; \
    } \
    static void i2c_send_async(uint8_t addr, char *data, uint32_t size) \
    { \
        ctf_thread_addr = addr; \
        memcpy(ctf_thread_data, data, sizeof(ctf_thread_data)); \
        ctf_thread_data_size = size; \
        if ( ctf_thread_init ) pthread_join(ctf_thread, NULL); \
        ctf_thread_init = 1; \
        pthread_create(&ctf_thread, NULL, i2c_send_thread, NULL); \
    } \
    static void *uart_send_thread(void *ctx) \
    { \
        uart_send(ctf_thread_data, ctf_thread_data_size); \
        memset(ctf_thread_data, 0, sizeof(ctf_thread_data)); \
        return NULL; \
    } \
    static void uart_send_async(char *data, uint32_t size) \
    { \
        memcpy(ctf_thread_data, data, sizeof(ctf_thread_data)); \
        ctf_thread_data_size = size; \
        if ( ctf_thread_init ) pthread_join(ctf_thread, NULL); \
        ctf_thread_init = 1; \
        pthread_create(&ctf_thread, NULL, uart_send_thread, NULL); \
    }

#define CTF_INIT_MACRO ctf_init(&ctf_data, tmr_millis, uart_send_async, i2c_send_async);
#define CTF_ON_MACRO ctf_on(ctf_get_data());
#define CTF_OFF_MACRO ctf_off(ctf_get_data());

#ifdef __cplusplus
extern "C" {
#endif

void ctf_init(
    ctf_data_t *ctf_data,
    tmr_millis_t tmr_millis,
    uart_send_t uart_send,
    i2c_send_t i2c_send
);
void ctf_tick(ctf_data_t *ctf_data);
ctf_data_t *ctf_get_data();
void ctf_on(ctf_data_t *ctf_data);
void ctf_off(ctf_data_t *ctf_data);

#ifdef __cplusplus
}
#endif

#endif
