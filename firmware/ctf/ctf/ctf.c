#define CTF_NO_CPP
#include "ctf.h"
#undef CTF_NO_CPP

#include <stdio.h>
#include <string.h>

static ctf_data_t *ctf_data_ptr = NULL;

ctf_data_t *ctf_get_data()
{
    return ctf_data_ptr;
}

void ctf_init(
    ctf_data_t *ctf_data,
    tmr_millis_t tmr_millis,
    uart_send_t uart_send,
    i2c_send_t i2c_send,
    spi_send_t spi_send
)
{
    uint32_t timestamp = tmr_millis();
    if ( ctf_data->init )
    {
        if ( (timestamp - ctf_data->timestamp_prev) > 1024 )
        {
            ctf_data->timestamp_prev = timestamp;
            ctf_tick(ctf_data);
        }
        return;
    }
    ctf_data_ptr = ctf_data;
    memset(ctf_data, 0, sizeof(ctf_data));
    ctf_data->init = 1;
    ctf_data->on = 0;
    ctf_data->flag_to_send = 0;
    ctf_data->tmr_millis = tmr_millis;
    ctf_data->uart_send = uart_send;
    ctf_data->i2c_send = i2c_send;
    ctf_data->spi_send = spi_send;
    ctf_on(ctf_data);
    ctf_data->timestamp_prev = timestamp;
    ctf_tick(ctf_data);
}

void ctf_tick(ctf_data_t *ctf_data)
{
    if ( !ctf_data->on )
    {
        return;
    }
    if ( ctf_data->flag_to_send > 3 )
    {
        ctf_data->flag_to_send = 0;
    }
    switch(ctf_data->flag_to_send)
    {
        case 0:
            ctf_data->i2c_send('f', "cybears{i_2_c_y0u}", sizeof("cybears{i_2_c_y0u}") - 1);
            break;
        case 1:
            ctf_data->uart_send("cybears{u_r_a_s3r1al_h4ck3r}", sizeof("cybears{u_r_a_s3r1al_h4ck3r}") - 1);
            break;
        case 2:
            ctf_data->spi_send("cybears{i_spy}", sizeof("cybears{i_spy}") - 1);
            break;
    }
    ctf_data->flag_to_send++;
}

void ctf_on(ctf_data_t *ctf_data)
{
    ctf_data->on = 1;
}

void ctf_off(ctf_data_t *ctf_data)
{
    ctf_data->on = 0;
}
