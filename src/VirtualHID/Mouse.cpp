#include "Mouse.h"
#include <iostream>

Mouse_::Mouse_(void) {}
void Mouse_::begin(void) { releaseAll(); }
void Mouse_::end(void) { releaseAll(); }
void Mouse_::releaseAll(void) {
  _buttons = 0;
  move(0,0,0);
}

void Mouse_::click(uint8_t b) {
  _buttons = b;
  move(0,0,0);
  _buttons = 0;
  move(0,0,0);
}

void Mouse_::move(signed char x, signed char y, signed char wheel) {
  HID_MouseReport_Data_t report;
  report.buttons = _buttons;
  report.xAxis = x;
  report.yAxis = y;
  report.wheel = wheel;
  sendReport(&report, sizeof(report));
}

void Mouse_::buttons(uint8_t b) {
  if (b != _buttons) {
    _buttons = b;
    move(0,0,0);
  }
}

void Mouse_::press(uint8_t b) {
  buttons(_buttons | b);
}

void Mouse_::release(uint8_t b) {
  buttons(_buttons & ~b);
}

bool Mouse_::isPressed(uint8_t b) {
  return ((b & _buttons) > 0);
}

Mouse_ Mouse;
