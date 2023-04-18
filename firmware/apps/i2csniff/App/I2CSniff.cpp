#include "I2CSniff.hpp"

#define I2C_PIN_SDA 8
#define I2C_PIN_SCL 9

static void SA_iRestore(const  uint32_t *__s)
{
    XTOS_RESTORE_INTLEVEL(*__s);
}

#define SA_ATOMIC_RESTORESTATE uint32_t _sa_saved              \
    __attribute__((__cleanup__(SA_iRestore))) = XTOS_DISABLE_ALL_INTERRUPTS

#define NO_INTERRUPTS()                                            \
for ( SA_ATOMIC_RESTORESTATE, _sa_done =  1;                   \
    _sa_done; _sa_done = 0 )

#define hex_3bit_in_interrupt(v)  ('0' + (v))
#define hex_4bit_in_interrupt(v)  (((v) > 9) ? ('a' + (v) - 0xa) : ('0' + (v)))

static volatile uint8_t g_queue_data[9600];
static volatile uint16_t g_queue_wp = 0;
static volatile uint16_t g_queue_rp = 0;

static volatile uint8_t g_i2c_scl = 0;
static volatile uint8_t g_i2c_sda = 0;

#define I2C_SCL_LOW     (0 == g_i2c_scl)
#define I2C_SCL_HIGH    (1 == g_i2c_scl)
#define I2C_SCL_FALLING I2C_SCL_LOW
#define I2C_SCL_RISING  I2C_SCL_HIGH
#define I2C_SDA_LOW     (0 == g_i2c_sda)
#define I2C_SDA_HIGH    (1 == g_i2c_sda)
#define I2C_SDA_FALLING I2C_SDA_LOW
#define I2C_SDA_RISING  I2C_SDA_HIGH

static volatile uint8_t g_i2c_state = 0;

#define I2C_STATE_IDLE    (0)
#define I2C_STATE_ACTIVE  (1)

#define I2C_IDLE      (I2C_STATE_IDLE == g_i2c_state)
#define I2C_ACTIVE    (I2C_STATE_ACTIVE == g_i2c_state)

static volatile uint8_t g_i2c_bit_reg = 0x00;
static volatile uint8_t g_i2c_bit_pos = 0;
static volatile uint8_t g_i2c_bit_count = 0;
static volatile uint16_t g_i2c_byte_count = 0;

static void i2c_reset()
{
    g_queue_wp=0;
    g_queue_rp=0;
    g_i2c_scl = 0;
    g_i2c_sda = 0;
    g_i2c_state = I2C_STATE_IDLE;
    g_i2c_bit_pos = 0;
    g_i2c_bit_reg = 0x00;
    g_i2c_bit_count = 0;
    g_i2c_byte_count = 0;
}


#define queue_write_byte_in_interrupt(v) \
{ \
    if ( ( g_queue_wp + 1 ) == sizeof(g_queue_data) ) \
    { \
        if ( 0 != g_queue_rp ) \
        { \
            g_queue_data[g_queue_wp] = v; \
            if ( (g_queue_wp + 1) == sizeof(g_queue_data) ) \
            { \
                g_queue_wp = 0; \
            } \
            else \
            { \
                g_queue_wp++; \
            } \
        } \
    } \
    else \
    { \
        if ( ( g_queue_wp + 1 ) != g_queue_rp ) \
        { \
            g_queue_data[g_queue_wp] = v; \
            if ( (g_queue_wp + 1) == sizeof(g_queue_data) ) \
            { \
                g_queue_wp = 0; \
            } \
            else \
            { \
                g_queue_wp++; \
            } \
        } \
    } \
}

static int queue_read_byte(byte *v)
{
    if ( g_queue_wp == g_queue_rp )
    {
        return 0; // no data    
    }
    *v = g_queue_data[g_queue_rp];
    if ( (g_queue_rp + 1) == sizeof(g_queue_data) )
    {
        g_queue_rp = 0;  // must be atomic
    }
    else
    {
        g_queue_rp++;  // must be atomic
    }
    return 1;
}

static void IRAM_ATTR i2c_scl_change_interrupt()
{
    NO_INTERRUPTS()
    {
        g_i2c_scl =  digitalRead(I2C_PIN_SCL);

        if ( I2C_ACTIVE && I2C_SCL_RISING )
        {
            if ( g_i2c_bit_count == 8 )
            {
                // + ACK
                // - NACK
                queue_write_byte_in_interrupt( g_i2c_sda ? '-' : '+');
                g_i2c_byte_count++;
                g_i2c_bit_pos = 7;
                g_i2c_bit_reg = 0x00;
                g_i2c_bit_count = 0;
            }
            else 
            {
                //queue_write_byte_in_interrupt('0' + g_i2c_sda);
                if ( g_i2c_sda )
                {
                    g_i2c_bit_reg |= 1 << g_i2c_bit_pos;
                }
                g_i2c_bit_pos--;
                if ( g_i2c_byte_count == 0 )
                {
                    // I2C SLAVE ADDRESS BIT
                    if ( g_i2c_bit_count == 2 )
                    {
                        // first 3 bits of the I2C slave address
                        queue_write_byte_in_interrupt(hex_3bit_in_interrupt(g_i2c_bit_reg >> 4));
                        g_i2c_bit_reg = 0x00; // reset, don't need these bits
                    }
                    if ( g_i2c_bit_count == 6 )
                    {
                        // next 4 bits of the I2C slave address
                        queue_write_byte_in_interrupt(hex_4bit_in_interrupt(g_i2c_bit_reg));
                        g_i2c_bit_reg = 0x00; // reset, don't need these bits
                    }
                    if ( g_i2c_bit_count == 7 )
                    {
                        queue_write_byte_in_interrupt( g_i2c_sda ? 'R' : 'W');
                    }
                }
                else
                {
                    // DATA BIT
                    if ( g_i2c_bit_count == 3 )
                    {
                        queue_write_byte_in_interrupt(hex_4bit_in_interrupt(g_i2c_bit_reg >> 4));
                        g_i2c_bit_reg = 0x00; // reset, don't need these bits
                    }
                    if ( g_i2c_bit_count == 7 )
                    {
                        queue_write_byte_in_interrupt(hex_4bit_in_interrupt(g_i2c_bit_reg));
                        g_i2c_bit_reg = 0x00; // reset, don't need these bits
                    }
                }
                g_i2c_bit_count++;
            }
        }
    }
}

static void IRAM_ATTR i2c_sda_change_interrupt()
{
    NO_INTERRUPTS()
    {
        g_i2c_sda =  digitalRead(I2C_PIN_SDA);

        if ( I2C_IDLE )
        {
            // look for start bit
            if ( I2C_SDA_FALLING && I2C_SCL_HIGH )
            {
                // START BIT
                g_i2c_state = I2C_STATE_ACTIVE;
                g_i2c_bit_pos = 6;
                g_i2c_bit_count = 0;
                g_i2c_byte_count = 0;
                g_i2c_bit_reg = 0x00;
                queue_write_byte_in_interrupt('S');
            }
        }
        else if ( I2C_ACTIVE )
        {
            // look for stop bit
            if ( I2C_SDA_RISING && I2C_SCL_HIGH )
            {
                // STOP BIT
                g_i2c_state = I2C_STATE_IDLE;
                queue_write_byte_in_interrupt('s');
            }
        }
    }
}

void I2CSniff::begin()
{
    TextView::set_title("Reading");
    TextView::begin();

    pinMode(I2C_PIN_SCL, INPUT_PULLUP);   
    pinMode(I2C_PIN_SDA, INPUT_PULLUP);
    i2c_reset();
    attachInterrupt(I2C_PIN_SCL, i2c_scl_change_interrupt, CHANGE);
    attachInterrupt(I2C_PIN_SDA, i2c_sda_change_interrupt, CHANGE);

    pause_ = false;
    last_parse_ = millis();
}

void I2CSniff::key_event(uint8_t key)
{
    if ( APP_KEY_PLAY == key )
    {
        pause_ = !pause_;
        if ( pause_ ) {
            TextView::set_title("Paused");
        }
        else
        {
            TextView::set_title("Reading");
        }
    }
    TextView::key_event(key);
}

void I2CSniff::loop()
{
    char buf[512]; // TODO: this is too little, need to allocate larger
    size_t size = 0;
    uint8_t v = 0;

    if ( (millis() - last_parse_) > 500 )
    {
        last_parse_ = millis();
        while ( queue_read_byte(&v) )
        {
            size = 0;
            buf[size] = v;
            size++;
            if ( v == 's' )
            {
                buf[size] = '\n';
                size++;
            }
            while ( (size < (sizeof(buf) - 1) ) && queue_read_byte(&v) )
            {
                buf[size] = v;
                size++;
                if ( v == 's' )
                {
                    buf[size] = '\n';
                    size++;
                }
            }
            if ( size > 0 )
            {
                if ( !pause_ )
                {
                    TextView::append_text(std::string(buf, size), 0x1000);
                }
            }
        }
    }
}

void I2CSniff::end()
{
    detachInterrupt(I2C_PIN_SCL);
    detachInterrupt(I2C_PIN_SDA);
    TextView::clear();
    TextView::end();
}
