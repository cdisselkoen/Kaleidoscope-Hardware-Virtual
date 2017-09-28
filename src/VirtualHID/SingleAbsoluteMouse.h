#pragma once

// Essentially equivalent to the corresponding header file in KeyboardioHID
// with the goal of having an almost identical interface, with different implementation

#include <Arduino.h>

class SingleAbsoluteMouse_ {
public:
    SingleAbsoluteMouse_(void);
    void begin(void);
    void move(signed char x, signed char y, signed char wheel);
    void moveTo(uint16_t x, uint16_t y, signed char wheel);
    void click(uint8_t buttons);
    void press(uint8_t buttons);
    void release(uint8_t buttons);

    void sendReport(void* data, int length);
};

extern SingleAbsoluteMouse_ SingleAbsoluteMouse;


