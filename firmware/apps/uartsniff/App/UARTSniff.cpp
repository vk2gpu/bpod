#include "UARTSniff.hpp"

const static char *NOTES = "" \
    "To sniff UART messages, use the wiring diagram to " \
    "join the bPod to the other device. Once wired, select " \
    "baud rate (how fast the other device is transmitting) and "\
    "config (data bits, parity bit and stop bit).\n" \
    "Example 8N1:\n" \
    " - 8 data bits\n" \
    " - No parity bit\n" \
    " - 1 stop bit\n" \
    "You can then select Sniff to start seeing messages. You can " \
    "press play to stop and resume the capture.";

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
    return "XXX";
}

void UARTSniffBaudRate::begin(BpodMenu &menu)
{
    menu.set_title("Baud");
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

uint32_t UARTSniffBaudRate::baud()
{
    if ( 0 == baud_ )
    {
        // FUTURE: auto detect rather than default
        baud_ = 115200;
    }
    return baud_;
}

void UARTSniffConfig::begin(BpodMenu &menu)
{
    menu.set_title("Config");
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

uint32_t UARTSniffConfig::config()
{
    if ( 0 == config_ )
    {
        config_ = SERIAL_8N1;
    }
    return config_;
}

void UARTSniffOutput::begin()
{
    pause_ = false;
    TextView::begin();
}

void UARTSniffOutput::key_event(uint8_t key)
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

void UARTSniffOutput::loop()
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

void UARTSniffOutput::end()
{
    Serial1.end();
    TextView::clear();
    TextView::end();
}

void UARTSniff::begin(BpodMenu &menu)
{
    menu.set_title("uartsniff");
    menu.add(std::to_string(baud()), [this](){ App::manager_begin(baud_); });
    menu.add(config_to_string(config()), [this](){ App::manager_begin(config_); });
    menu.add("Sniff", [this](){
        Serial1.end();
        Serial1.setRxBufferSize(0x1000);
        Serial1.begin(baud(), config());
        output_.set_title("Reading");
        App::manager_begin(output_);
    });
    menu.add("Diagram", [this](){
        diagram_.set_title("Diagram");
        diagram_.add_gnd_label();
        diagram_.add_pin_label(17, "TX1", 0x0000, 0x07e0);  // GREEN / BLACK
        diagram_.add_pin_label(18, "RX1", 0xffff, 0x001f);  // BLUE / BLACK
        diagram_.add_wire(17, "RX1", 0xffff, 0x001f);
        diagram_.add_wire(18, "TX1", 0x0000, 0x07e0);
        diagram_.add_wire_gnd();
        App::manager_begin(diagram_);
    });
    menu.add("Notes", [this](){
        notes_.set_title("Notes");
        notes_.set_text(std::string(NOTES));
        App::manager_begin(notes_);
    });
}

void UARTSniff::visible()
{
    // update text on the menu, but keep the scroll position
    Menu::set(0, std::to_string(baud()));
    Menu::set(1, config_to_string(config()));

    // don't need to keep the notes
    notes_.clear();
    diagram_.clear();
}
