#include "SingleAbsoluteMouse.h"
#include <iostream>
#include "virtual_io.h"

SingleAbsoluteMouse_::SingleAbsoluteMouse_(void) {}

void SingleAbsoluteMouse_::sendReport(void* data, int length) {
  std::cout << "A virtual SingleAbsoluteMouse HID report was sent." << std::endl;
  logUSBEvent("SingleAbsoluteMouse HID report", data, length);
}

// Everything else is stubs for now - no effect
void SingleAbsoluteMouse_::begin(void) {}
void SingleAbsoluteMouse_::move(signed char x, signed char y, signed char wheel) {}
void SingleAbsoluteMouse_::moveTo(uint16_t x, uint16_t y, signed char wheel) {}
void SingleAbsoluteMouse_::click(uint8_t buttons) {}
void SingleAbsoluteMouse_::press(uint8_t buttons) {}
void SingleAbsoluteMouse_::release(uint8_t buttons) {}

SingleAbsoluteMouse_ SingleAbsoluteMouse;
