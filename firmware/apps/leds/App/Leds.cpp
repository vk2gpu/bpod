#include "Leds.hpp"


void Leds::begin() {
    pinMode(10, OUTPUT);
    digitalWrite(10, HIGH);
    pinMode(16, OUTPUT);
    digitalWrite(16, HIGH);
    pinMode(21, OUTPUT);
    digitalWrite(21, HIGH);
    Menu::begin();
}

void Leds::begin(BpodMenu &menu) {
    menu.set_title("LEDs");
    menu.add("IO10 " + std::string(digitalRead(10) ? "ON" : "OFF"), [&menu, this](){ 
        digitalWrite(10, digitalRead(10) ? 0 : 1);
        menu.set(0, "IO10 " + std::string(digitalRead(10) ? "ON" : "OFF"));
    });
    menu.add("IO16 " + std::string(digitalRead(16) ? "ON" : "OFF"), [&menu, this](){ 
        digitalWrite(16, digitalRead(16) ? 0 : 1);
        menu.set(1, "IO16 " + std::string(digitalRead(16) ? "ON" : "OFF"));
    });
    menu.add("IO21 " + std::string(digitalRead(21) ? "ON" : "OFF"), [&menu, this](){ 
        digitalWrite(21, digitalRead(21) ? 0 : 1);
        menu.set(2, "IO21 " + std::string(digitalRead(21) ? "ON" : "OFF"));
    });
}
