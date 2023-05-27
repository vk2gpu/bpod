#include "SPISniff.hpp"

#include <stringdb.h>

#define SPI_PIN_CS      7
#define SPI_PIN_MISO    5
#define SPI_PIN_CLK     9
#define SPI_PIN_MOSI    6

#define SPI_QUEUE_SIZE              (0x3fff)
#define SPI_QUEUE_SIZE_IN_BYTES     (0x1fff)
#define SPI_QUEUE_SIZE_MASK         (0x3fff)
static volatile uint8_t     spi_value = 0;
static volatile uint8_t     spi_queue[SPI_QUEUE_SIZE_IN_BYTES];
static volatile uint16_t    spi_queue_wr = 0;
#define                     spi_queue_wr_byte           (spi_queue_wr>>1)
#define                     spi_queue_wr_bit_shift      ((spi_queue_wr<<2)&0x4)
static volatile uint16_t    spi_queue_rd = 0;
#define                     spi_queue_rd_byte           (spi_queue_rd>>1)
#define                     spi_queue_rd_bit_shift      ((spi_queue_rd<<2)&0x4)
static volatile uint8_t     spi_cs_falling = 0;

static void SA_iRestore(const  uint32_t *__s)
{
    XTOS_RESTORE_INTLEVEL(*__s);
}

#define SA_ATOMIC_RESTORESTATE uint32_t _sa_saved              \
    __attribute__((__cleanup__(SA_iRestore))) = XTOS_DISABLE_ALL_INTERRUPTS

#define NO_INTERRUPTS()                                            \
for ( SA_ATOMIC_RESTORESTATE, _sa_done =  1;                   \
    _sa_done; _sa_done = 0 )

static void IRAM_ATTR spi_cs_falling_interrupt()
{
    NO_INTERRUPTS()
    {
        spi_cs_falling = 1;
    }
}

static void IRAM_ATTR spi_clk_rising_interrupt()
{
    NO_INTERRUPTS()
    {
        spi_value = (0x02 & (digitalRead(SPI_PIN_MOSI) << 1)) | (0x01 & digitalRead(SPI_PIN_MISO)) | (spi_cs_falling << 2);
        spi_cs_falling = 0;
        spi_queue[spi_queue_wr_byte] = (spi_queue[spi_queue_wr_byte] & ~(0xf << (spi_queue_wr_bit_shift))) | (spi_value << (spi_queue_wr_bit_shift));
        spi_queue_wr = (spi_queue_wr + 1) & SPI_QUEUE_SIZE_MASK;
    }
}

#define SPI_SIGNAL_IS_BITS(v)       ((v)<8)
#define SPI_SIGNAL_MOSI_BIT(v)      (((v)>>1)&0x1)
#define SPI_SIGNAL_MISO_BIT(v)      ((v)&0x1)
#define SPI_SIGNAL_IS_CS_FALLING(v) ((v)&0x4)
#define SPI_SIGNAL_NONE             (0xf)
#define SPI_SIGNAL_IGNORE           (0x8)
static uint8_t spi_bit_mosi         = 0x00;
static uint8_t spi_bit_miso         = 0x00;
static uint8_t spi_bit_pos          = 0;
static uint8_t spi_cache_array[6];
static uint8_t spi_cache_count      = 0;
#define spi_hex(v)                  ((((v)&0xf) > 9) ? ('A' + ((v)&0xf) - 0xa) : ('0' + ((v)&0xf)))

static void spi_reset()
{
    spi_value = 0;
    spi_queue_wr = 0;
    spi_queue_rd = 0;
    spi_cs_falling = 0;
    spi_bit_mosi = 0x00;
    spi_bit_miso = 0x00;
    spi_bit_pos = 7;
    spi_cache_count = 0;
}

static uint8_t spi_get_signal()
{
    if ( spi_queue_rd == spi_queue_wr )
    {
        return SPI_SIGNAL_NONE;
    }
    uint8_t value = 0xf & (spi_queue[spi_queue_rd_byte] >> spi_queue_rd_bit_shift);
    spi_queue_rd = (spi_queue_rd + 1) & SPI_QUEUE_SIZE_MASK;
    return value;
}

static int spi_read_byte(uint8_t *v)
{
    while ( true )
    {
        if ( spi_cache_count )
        {
            *v = spi_cache_array[6 - spi_cache_count];
            spi_cache_count--;
            return 1;
        }
        uint8_t signal = spi_get_signal();
        while ( SPI_SIGNAL_IGNORE == signal )
        {
            signal = spi_get_signal();
        }
        if ( SPI_SIGNAL_NONE == signal )
        {
            return 0;
        }
        if ( SPI_SIGNAL_IS_CS_FALLING(signal) )
        {
            spi_bit_mosi = 0x00;
            spi_bit_miso = 0x00;
            spi_bit_pos = 7;
            spi_cache_array[5] = '\n';
            spi_cache_count = 1;
        }
        if ( SPI_SIGNAL_IS_BITS(signal) )
        {
            if ( SPI_SIGNAL_MOSI_BIT(signal) )
            {
                spi_bit_mosi |= 1 << spi_bit_pos;
            }
            if ( SPI_SIGNAL_MISO_BIT(signal) )
            {
                spi_bit_miso |= 1 << spi_bit_pos;
            }
            if ( spi_bit_pos == 0 )
            {
                spi_cache_array[0] = spi_hex(spi_bit_mosi >> 4);
                spi_cache_array[1] = spi_hex(spi_bit_mosi);
                spi_cache_array[2] = ':';
                spi_cache_array[3] = spi_hex(spi_bit_miso >> 4);
                spi_cache_array[4] = spi_hex(spi_bit_miso);
                spi_cache_array[5] = '+';
                spi_cache_count = 6;
                spi_bit_mosi = 0x00; // reset, don't need these bits
                spi_bit_miso = 0x00; // reset, don't need these bits
                spi_bit_pos = 8;
            }
            spi_bit_pos--;
        }
    }
    return 0;
}

void SPISniffMonitor::begin()
{
    TextView::set_title(STRING(STRING_READING));
    TextView::set_dark_mode();
    TextView::begin();

    spi_reset();

    Wire.end();
    pinMode(SPI_PIN_CS, INPUT);
    pinMode(SPI_PIN_MISO, INPUT);
    pinMode(SPI_PIN_CLK, INPUT);
    pinMode(SPI_PIN_MOSI, INPUT);
    attachInterrupt(SPI_PIN_CLK, spi_clk_rising_interrupt, RISING);
    attachInterrupt(SPI_PIN_CS, spi_cs_falling_interrupt, FALLING);

    pause_ = false;
    last_parse_ = millis();
}

void SPISniffMonitor::key_event(uint8_t key)
{
    if ( APP_KEY_PLAY == key )
    {
        pause_ = !pause_;
        if ( pause_ ) {
            TextView::set_title(STRING(STRING_PAUSE));
        }
        else
        {
            TextView::set_title(STRING(STRING_READING));
        }
    }
    TextView::key_event(key);
}

void SPISniffMonitor::loop()
{
    char buf[512]; // TODO: this is too little, need to allocate larger
    size_t size = 0;
    uint8_t v = 0;

    if ( (millis() - last_parse_) > 500 )
    {
        last_parse_ = millis();
        while ( spi_read_byte(&v) )
        {
            size = 0;
            buf[size] = v;
            size++;
            while ( (size < (sizeof(buf) - 1) ) && spi_read_byte(&v) )
            {
                buf[size] = v;
                size++;
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

void SPISniffMonitor::end()
{
    detachInterrupt(SPI_PIN_CLK);
    detachInterrupt(SPI_PIN_CS);
    TextView::clear();
    TextView::end();
}

void SPISniff::begin(BpodMenu &menu) {
    menu.set_title(STRING(STRING_SPISNIFF));
    menu.add(STRING(STRING_SNIFF), [this](){
        App::manager_begin(sniff_);
    });
    menu.add(STRING(STRING_DIAGRAM), [this](){
        diagram_.set_title(STRING(STRING_DIAGRAM));
        diagram_.add_gnd_label();
        diagram_.add_pin_label(SPI_PIN_CS, STRING(STRING_IO) + std::to_string(SPI_PIN_CS), 0x0000, 0xffe0);
        diagram_.add_pin_label(SPI_PIN_MISO, STRING(STRING_IO) + std::to_string(SPI_PIN_MISO), 0xffff, 0x001f);
        diagram_.add_pin_label(SPI_PIN_CLK, STRING(DIAGRAM_SCK), 0xffff, 0x8170);
        diagram_.add_pin_label(SPI_PIN_MOSI, STRING(STRING_IO) + std::to_string(SPI_PIN_MOSI), 0x0000, 0x07e0);
        diagram_.add_wire_gnd();
        diagram_.add_wire(SPI_PIN_CS, STRING(DIAGRAM_CS), 0x0000, 0xffe0);
        diagram_.add_wire(SPI_PIN_MISO, STRING(DIAGRAM_MISO) + 2, 0xffff, 0x001f);
        diagram_.add_wire(SPI_PIN_CLK, STRING(DIAGRAM_SCK), 0xffff, 0x8170);
        diagram_.add_wire(SPI_PIN_MOSI, STRING(DIAGRAM_MOSI) + 2, 0x0000, 0x07e0);
        App::manager_begin(diagram_);
    });
    menu.add(STRING(STRING_NOTES), [this](){
        notes_.set_title(STRING(STRING_NOTES));
        notes_.set_text(std::string(STRING(SPISNIFF_NOTES)));
        App::manager_begin(notes_);
    });
}
