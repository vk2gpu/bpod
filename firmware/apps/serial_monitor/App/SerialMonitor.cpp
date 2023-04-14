#include "SerialMonitor.hpp"

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
    { SERIAL_5N1, "Serial 5N1" },
    { SERIAL_6N1, "Serial 6N1" },
    { SERIAL_7N1, "Serial 7N1" },
    { SERIAL_8N1, "Serial 8N1" },
    { SERIAL_5N2, "Serial 5N2" },
    { SERIAL_6N2, "Serial 6N2" },
    { SERIAL_7N2, "Serial 7N2" },
    { SERIAL_8N2, "Serial 8N2" },
    { SERIAL_5E1, "Serial 5E1" },
    { SERIAL_6E1, "Serial 6E1" },
    { SERIAL_7E1, "Serial 7E1" },
    { SERIAL_8E1, "Serial 8E1" },
    { SERIAL_5E2, "Serial 5E2" },
    { SERIAL_6E2, "Serial 6E2" },
    { SERIAL_7E2, "Serial 7E2" },
    { SERIAL_8E2, "Serial 8E2" },
    { SERIAL_5O1, "Serial 5O1" },
    { SERIAL_6O1, "Serial 6O1" },
    { SERIAL_7O1, "Serial 7O1" },
    { SERIAL_8O1, "Serial 8O1" },
    { SERIAL_5O2, "Serial 5O2" },
    { SERIAL_6O2, "Serial 6O2" },
    { SERIAL_7O2, "Serial 7O2" },
    { SERIAL_8O2, "Serial 8O2" },
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
    return "Serial XXX";
}

void SerialMonitorBaudRate::begin(BpodMenu &menu)
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

uint32_t SerialMonitorBaudRate::baud()
{
    if ( 0 == baud_ )
    {
        // FUTURE: auto detect rather than default
        baud_ = 115200;
    }
    return baud_;
}

void SerialMonitorConfig::begin(BpodMenu &menu)
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

uint32_t SerialMonitorConfig::config()
{
    if ( 0 == config_ )
    {
        config_ = SERIAL_8N1;
    }
    return config_;
}

void SerialMonitorOutput::begin()
{
    pause_ = false;
    TextView::begin();
}

void SerialMonitorOutput::key_event(uint8_t key)
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

void SerialMonitorOutput::loop()
{
    //int size = Serial1.available();
    char buf[65];
    while ( Serial1.available() > 0 )
    {
        size_t recd = Serial1.read(buf, sizeof(buf) - 1);
        //size -= recd;
        buf[recd] = '\0';
        if ( !pause_ )
        {
            TextView::append_text(std::string(buf, recd), 0x1000);
        }
    }
}

void SerialMonitorOutput::end()
{
    Serial1.end();
    TextView::clear();
    TextView::end();
}

void SerialMonitor::begin(BpodMenu &menu)
{
    menu.set_title("Serial");
    menu.add(std::to_string(baud()), [this](){ App::manager_begin(baud_); });
    menu.add(config_to_string(config()), [this](){ App::manager_begin(config_); });
    menu.add("Begin", [this](){
        Serial1.end();
        Serial1.setRxBufferSize(0x1000);
        Serial1.begin(baud(), config(), 19, 22);
        output_.set_title("Reading");
        App::manager_begin(output_);
    });
}

void SerialMonitor::visible()
{
    // update text on the menu, but keep the scroll position
    size_t backup = Menu::pos();
    Menu::end();
    Menu::begin();
    Menu::set_pos(backup);
}
