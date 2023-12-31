#include "UARTTerm.hpp"

#include <stringdb.h>

static uint32_t baud_rate_list[] = {
    300,
    2400,
    4800,
    9600,
    19200,
    38400,
    57600,
    115200,
    230400,
};

struct config_entry {
    uint32_t value;
    const char *name;
};

static struct config_entry config_list[] = {
    { SERIAL_5N1, "5N1" },
    { SERIAL_6N1, "6N1" },
    { SERIAL_7N1, "7N1" },
    { SERIAL_8N1, "8N1" },
    { SERIAL_5N2, "5N2" },
    { SERIAL_6N2, "6N2" },
    { SERIAL_7N2, "7N2" },
    { SERIAL_8N2, "8N2" },
    { SERIAL_5E1, "5E1" },
    { SERIAL_6E1, "6E1" },
    { SERIAL_7E1, "7E1" },
    { SERIAL_8E1, "8E1" },
    { SERIAL_5E2, "5E2" },
    { SERIAL_6E2, "6E2" },
    { SERIAL_7E2, "7E2" },
    { SERIAL_8E2, "8E2" },
    { SERIAL_5O1, "5O1" },
    { SERIAL_6O1, "6O1" },
    { SERIAL_7O1, "7O1" },
    { SERIAL_8O1, "8O1" },
    { SERIAL_5O2, "5O2" },
    { SERIAL_6O2, "6O2" },
    { SERIAL_7O2, "7O2" },
    { SERIAL_8O2, "8O2" },
};

static const char *config_to_string(uint32_t config)
{
    for ( size_t i = 0; i < sizeof(config_list) / sizeof(config_list[0]); i++ )
    {
        if ( config_list[i].value == config )
        {
            return config_list[i].name;
        }
    }
    return STRING(STRING_UART_CONFIG_UNKNOWN);
}

void UARTTermBaudRate::begin(BpodMenu &menu)
{
    menu.set_title(STRING(STRING_BAUD));
    for ( size_t i = 0; i < sizeof(baud_rate_list) / sizeof(baud_rate_list[0]); i++ )
    {
        uint32_t b = baud_rate_list[i];
        menu.add(std::to_string(b), [this, b](){ baud_ = b; App::manager_end(); });
        if ( b == baud() )
        {
            menu.move_to(menu.size() - 1);
        }
    }
}

uint32_t UARTTermBaudRate::baud()
{
    if ( 0 == baud_ )
    {
        // FUTURE: auto detect rather than default
        baud_ = 115200;
    }
    return baud_;
}

void UARTTermConfig::begin(BpodMenu &menu)
{
    menu.set_title(STRING(STRING_CONFIG));
    for ( size_t i = 0; i < sizeof(config_list) / sizeof(config_list[0]); i++ )
    {
        uint32_t c = config_list[i].value;
        menu.add(config_list[i].name, [this, c](){ config_ = c; App::manager_end(); });
        if ( c == config() )
        {
            menu.move_to(menu.size() - 1);
        }
    }
}

uint32_t UARTTermConfig::config()
{
    if ( 0 == config_ )
    {
        config_ = SERIAL_8N1;
    }
    return config_;
}

void UARTTermOutput::begin()
{
    pause_ = false;
    TextView::set_dark_mode();
    TextView::begin();
}

void UARTTermOutput::key_event(uint8_t key)
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

void UARTTermOutput::keyboard_event(int key)
{
    char c = key;
    if ( key == 0x11 || key == 0x18 )
    {
        App::manager_end();
        return;
    }
    Serial1.write(&c, sizeof(c));
}

void UARTTermOutput::loop()
{
    char buf[65];
    while ( Serial1.available() > 0 )
    {
        size_t recd = Serial1.read(buf, sizeof(buf) - 1);
        buf[recd] = '\0';
        if ( !pause_ )
        {
            TextView::append_text(std::string(buf, recd), 0x1000);
        }
    }
}

void UARTTermOutput::end()
{
    Serial1.end();
    TextView::clear();
    TextView::end();
}

void UARTTerm::begin(BpodMenu &menu)
{
    menu.set_title(STRING(STRING_UARTTERM));
    menu.add(std::to_string(baud()), [this](){ App::manager_begin(baud_); });
    STRING_CACHE();
    menu.add(config_to_string(config()), [this](){ App::manager_begin(config_); });
    STRING_CLEAR();
    menu.add(STRING(STRING_TERMINAL), [this](){
        Serial1.end();
        Serial1.setRxBufferSize(0x1000);
        Serial1.begin(baud(), config());
        output_.set_title(STRING(STRING_READING));
        App::manager_begin(output_);
    });
    menu.add(STRING(STRING_DIAGRAM), [this](){
        diagram_.set_title(STRING(STRING_DIAGRAM));
        diagram_.add_gnd_label();
        char tx[4];
        char rx[4];
        STRING_STRCPY(tx, STRING_TX1);
        STRING_STRCPY(rx, STRING_RX1);
        diagram_.add_pin_label(17, tx, 0x0000, 0x07e0);  // GREEN / BLACK
        diagram_.add_pin_label(18, rx, 0xffff, 0x001f);  // BLUE / BLACK
        tx[2] = '\0';
        rx[2] = '\0';
        diagram_.add_wire(17, rx, 0xffff, 0x001f);
        diagram_.add_wire(18, tx, 0x0000, 0x07e0);
        diagram_.add_wire_gnd();
        App::manager_begin(diagram_);
    });
    menu.add(STRING(STRING_NOTES), [this](){
        notes_.set_title(STRING(STRING_NOTES));
        notes_.set_text(std::string(STRING(UARTTERM_NOTES)));
        App::manager_begin(notes_);
    });
}

void UARTTerm::visible()
{
    // update text on the menu, but keep the scroll position
    Menu::set(0, std::to_string(baud()));
    STRING_CACHE();
    Menu::set(1, config_to_string(config()));
    STRING_CLEAR();

    // don't need to keep the notes and diagram
    notes_.clear();
    diagram_.clear();

    Menu::visible();
}
