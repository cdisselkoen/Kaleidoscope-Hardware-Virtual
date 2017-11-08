#pragma once

// Essentially equivalent to the corresponding header file in KeyboardioHID
// with the goal of having an almost identical interface, with different implementation

#include "Arduino.h"
#include "HIDTables.h"
#define HID_FIRST_KEY HID_KEYBOARD_NO_EVENT
#define HID_LAST_KEY HID_KEYPAD_HEXADECIMAL
#define HID_KEYBOARD_FIRST_MODIFIER HID_KEYBOARD_LEFT_CONTROL
#define HID_KEYBOARD_LAST_MODIFIER HID_KEYBOARD_RIGHT_GUI

#define KEY_BYTES 28

typedef union {
  // Modifiers + keymap
  struct {
    uint8_t modifiers;
    uint8_t keys[KEY_BYTES];
  };
  uint8_t allkeys[1 + KEY_BYTES];
} HID_KeyboardReport_Data_t;

class Keyboard_ {
  public:
    Keyboard_(void);
    void begin(void);
    void end(void);
    size_t press(uint8_t k);
    size_t release(uint8_t k);
    void releaseAll(void);
    int sendReport(void);
    uint8_t getLEDs(void);

    boolean isModifierActive(uint8_t k);
    boolean wasModifierActive(uint8_t k);

  protected:
    HID_KeyboardReport_Data_t _keyReport;
    HID_KeyboardReport_Data_t _lastKeyReport;
};

extern Keyboard_ Keyboard;
