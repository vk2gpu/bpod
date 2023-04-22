#include "I2CSniff.hpp"

const static char *NOTES = "" \
    "Limited i2c traffic sniffing tool. Due to small buffers and limited chip speed, " \
    "this is a best-effort to read and decode i2c traffic. Attach bPod to i2c lines running " \
    "between an existing master and slave(s) following the wiring diagram. \n\n" \
    "Example:\n" \
    " S20W+13+FF+s\n" \
    "  S : start bit\n" \
    "  20: addr\n" \
    "  W : write\n" \
    "  + : ack\n" \
    "  13: data\n" \
    "  FF: data\n" \
    "  s : stop bit\n";

#define I2C_PIN_SDA 8
#define I2C_PIN_SCL 9

#define I2C_QUEUE_SIZE              (0x7fff)
#define I2C_QUEUE_SIZE_IN_BYTES     (0x1fff)
#define I2C_QUEUE_SIZE_MASK         (0x7fff)
static volatile uint8_t     i2c_sda_r1 = 0;
static volatile uint8_t     i2c_sda_r2 = 0;
static volatile uint8_t     i2c_scl_r1 = 0;
static volatile uint8_t     i2c_scl_r2 = 0;
static volatile uint8_t     i2c_value = 0;
static volatile uint8_t     i2c_queue[I2C_QUEUE_SIZE_IN_BYTES];
static volatile uint16_t    i2c_queue_wr = 0;
#define                     i2c_queue_wr_byte           (i2c_queue_wr>>2)
#define                     i2c_queue_wr_bit_shift      ((i2c_queue_wr<<1)&0x6)
static volatile uint16_t    i2c_queue_rd = 0;
#define                     i2c_queue_rd_byte           (i2c_queue_rd>>2)
#define                     i2c_queue_rd_bit_shift      ((i2c_queue_rd<<1)&0x6)

static void SA_iRestore(const  uint32_t *__s)
{
    XTOS_RESTORE_INTLEVEL(*__s);
}

#define SA_ATOMIC_RESTORESTATE uint32_t _sa_saved              \
    __attribute__((__cleanup__(SA_iRestore))) = XTOS_DISABLE_ALL_INTERRUPTS

#define NO_INTERRUPTS()                                            \
for ( SA_ATOMIC_RESTORESTATE, _sa_done =  1;                   \
    _sa_done; _sa_done = 0 )

static void IRAM_ATTR i2c_scl_change_interrupt()
{
    NO_INTERRUPTS()
    {
        i2c_value = (0x02 & (digitalRead(I2C_PIN_SDA) << 1)) | (0x01 & digitalRead(I2C_PIN_SCL));
        i2c_queue[i2c_queue_wr_byte] = (i2c_queue[i2c_queue_wr_byte] & ~(0x3 << (i2c_queue_wr_bit_shift))) | (i2c_value << (i2c_queue_wr_bit_shift));
        i2c_queue_wr = (i2c_queue_wr + 1) & I2C_QUEUE_SIZE_MASK;
    }
}

static void IRAM_ATTR i2c_sda_change_interrupt()
{
    NO_INTERRUPTS()
    {
        i2c_value = (0x02 & (digitalRead(I2C_PIN_SDA) << 1)) | (0x01 & digitalRead(I2C_PIN_SCL));
        i2c_queue[i2c_queue_wr_byte] = (i2c_queue[i2c_queue_wr_byte] & ~(0x3 << (i2c_queue_wr_bit_shift))) | (i2c_value << (i2c_queue_wr_bit_shift));
        i2c_queue_wr = (i2c_queue_wr + 1) & I2C_QUEUE_SIZE_MASK;
    }
}

#define I2C_SIGNAL_NONE         (0)
#define I2C_SIGNAL_START_BIT    (1)
#define I2C_SIGNAL_STOP_BIT     (2)
#define I2C_SIGNAL_BIT_0        (3)
#define I2C_SIGNAL_BIT_1        (4)
#define I2C_SIGNAL_IGNORE       (5)
#define I2C_CASE(v,p)           ((v)|((p)<<2))
#define I2C_CASE_SCL_HIGH       (0x5)
#define I2C_CASE_SCL_LOW        (0x0)
#define I2C_CASE_SDA_HIGH       (0xa)
#define I2C_CASE_SDA_LOW        (0x0)
#define I2C_CASE_SDA_FALLING    (0x8)
#define I2C_CASE_SDA_RISING     (0x2)
#define I2C_CASE_SCL_FALLING    (0x4)
#define I2C_CASE_SCL_RISING     (0x1)
static uint8_t i2c_prev_value   = 0;
static uint8_t i2c_active       = 0;
static uint16_t i2c_byte_count  = 0;
static uint8_t i2c_bit_count    = 0;
static uint8_t i2c_bit_reg      = 0x00;
static uint8_t i2c_bit_pos      = 0;
#define i2c_hex(v)              (((v) > 9) ? ('A' + (v) - 0xa) : ('0' + (v)))

static void i2c_reset()
{
    i2c_value = 0;
    i2c_queue_wr = 0;
    i2c_queue_rd = 0;
    i2c_prev_value = 0x3;
    i2c_active = 0;
    i2c_byte_count = 0;
    i2c_bit_count = 0;
    i2c_bit_reg = 0x00;
    i2c_bit_pos = 0;
}

static uint8_t i2c_get_signal()
{
    if ( i2c_queue_rd == i2c_queue_wr )
    {
        return I2C_SIGNAL_NONE;
    }
    uint8_t value = 0x3 & (i2c_queue[i2c_queue_rd_byte] >> i2c_queue_rd_bit_shift);
    uint8_t prev = i2c_prev_value;
    i2c_queue_rd = (i2c_queue_rd + 1) & I2C_QUEUE_SIZE_MASK;
    i2c_prev_value = value;
    switch(I2C_CASE(value, prev))
    {
        case I2C_CASE_SCL_HIGH | I2C_CASE_SDA_FALLING:
            i2c_active = 1;
            return I2C_SIGNAL_START_BIT;

        case I2C_CASE_SCL_HIGH | I2C_CASE_SDA_RISING:
            i2c_active = 0;
            return I2C_SIGNAL_STOP_BIT;

        case I2C_CASE_SCL_RISING | I2C_CASE_SDA_HIGH:
            if ( i2c_active )
            {
                return I2C_SIGNAL_BIT_1;
            }
            return I2C_SIGNAL_IGNORE;

        case I2C_CASE_SCL_RISING | I2C_CASE_SDA_LOW:
            if ( i2c_active )
            {
                return I2C_SIGNAL_BIT_0;
            }
            return I2C_SIGNAL_IGNORE;
    }
    return I2C_SIGNAL_IGNORE;
}

static int i2c_read_byte(uint8_t *v)
{
    uint8_t bit = 0;
    while ( true )
    {
        uint8_t signal = i2c_get_signal();
        while ( I2C_SIGNAL_IGNORE == signal )
        {
            signal = i2c_get_signal();
        }
        switch (signal)
        {
            case I2C_SIGNAL_NONE:
                return 0;

            case I2C_SIGNAL_START_BIT:
                *v = 'S';
                i2c_byte_count = 0;
                i2c_bit_count = 0;
                i2c_bit_reg = 0x00;
                i2c_bit_pos = 6;
                return 1;

            case I2C_SIGNAL_STOP_BIT:
                *v = 's';
                return 1;

            case I2C_SIGNAL_BIT_0:
            case I2C_SIGNAL_BIT_1:
                bit = (signal == I2C_SIGNAL_BIT_1) ? 1 : 0;
                if ( i2c_bit_count == 8 )
                {
                    // + ACK
                    // - NACK
                    *v = bit ? '-' : '+';
                    i2c_byte_count++;
                    i2c_bit_pos = 7;
                    i2c_bit_reg = 0x00;
                    i2c_bit_count = 0;
                    return 1;
                }
                if ( bit )
                {
                    i2c_bit_reg |= 1 << i2c_bit_pos;
                }
                i2c_bit_pos--;
                if ( i2c_byte_count == 0 )
                {
                    // I2C SLAVE ADDRESS BIT
                    if ( i2c_bit_count == 2 )
                    {
                        // first 3 bits of the I2C slave address
                        *v = i2c_hex(i2c_bit_reg >> 4);
                        i2c_bit_reg = 0x00; // reset, don't need these bits
                        i2c_bit_count++;
                        return 1;
                    }
                    if ( i2c_bit_count == 6 )
                    {
                        // next 4 bits of the I2C slave address
                        *v = i2c_hex(i2c_bit_reg);
                        i2c_bit_reg = 0x00; // reset, don't need these bits
                        i2c_bit_count++;
                        return 1;
                    }
                    if ( i2c_bit_count == 7 )
                    {
                        *v = bit ? 'R' : 'W';
                        i2c_bit_count++;
                        return 1;
                    }
                }
                else
                {
                    // DATA BIT
                    if ( i2c_bit_count == 3 )
                    {
                        *v = i2c_hex(i2c_bit_reg >> 4);
                        i2c_bit_reg = 0x00; // reset, don't need these bits
                        i2c_bit_count++;
                        return 1;
                    }
                    if ( i2c_bit_count == 7 )
                    {
                        *v = i2c_hex(i2c_bit_reg);
                        i2c_bit_reg = 0x00; // reset, don't need these bits
                        i2c_bit_count++;
                        return 1;
                    }
                }
                i2c_bit_count++;
                break;
        }
    }
    return 0;
}

void I2CSniffMonitor::begin()
{
    TextView::set_title("Reading");
    TextView::begin();

    Wire.end();
    pinMode(I2C_PIN_SCL, INPUT_PULLUP);   
    pinMode(I2C_PIN_SDA, INPUT_PULLUP);
    i2c_reset();
    attachInterrupt(I2C_PIN_SCL, i2c_scl_change_interrupt, CHANGE);
    attachInterrupt(I2C_PIN_SDA, i2c_sda_change_interrupt, CHANGE);

    pause_ = false;
    last_parse_ = millis();
}

void I2CSniffMonitor::key_event(uint8_t key)
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

void I2CSniffMonitor::loop()
{
    char buf[512]; // TODO: this is too little, need to allocate larger
    size_t size = 0;
    uint8_t v = 0;

    if ( (millis() - last_parse_) > 500 )
    {
        last_parse_ = millis();
        while ( i2c_read_byte(&v) )
        {
            size = 0;
            buf[size] = v;
            size++;
            if ( v == 's' )
            {
                buf[size] = '\n';
                size++;
            }
            while ( (size < (sizeof(buf) - 1) ) && i2c_read_byte(&v) )
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

void I2CSniffMonitor::end()
{
    detachInterrupt(I2C_PIN_SCL);
    detachInterrupt(I2C_PIN_SDA);
    TextView::clear();
    TextView::end();
}

void I2CSniff::begin(BpodMenu &menu) {
    menu.set_title("i2csniff");
    menu.add("Sniff", [this](){
        App::manager_begin(sniff_);
    });
    menu.add("Diagram", [this](){
        diagram_.set_title("Diagram");
        diagram_.add_gnd_label();
        diagram_.add_pin_label(9, "SCK", 0xffff, 0x8170);
        diagram_.add_pin_label(8, "SDA", 0x0000, 0x07e0);
        diagram_.add_wire_gnd();
        diagram_.add_wire(9, "SCK", 0xffff, 0x8170);
        diagram_.add_wire(8, "SDA", 0x0000, 0x07e0);
        App::manager_begin(diagram_);
    });
    menu.add("Notes", [this](){
        notes_.set_title("Notes");
        notes_.set_text(std::string(NOTES));
        App::manager_begin(notes_);
    });
}
