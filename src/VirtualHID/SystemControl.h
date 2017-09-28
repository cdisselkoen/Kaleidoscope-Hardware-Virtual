#pragma once

// Essentially equivalent to the corresponding header file in KeyboardioHID
// with the goal of having an almost identical interface, with different implementation

#include <Arduino.h>
#include "HIDTables.h"

typedef union {
  // Every usable system control key possible
  uint8_t key;
} HID_SystemControlReport_Data_t;


class SystemControl_ {
 public:
  SystemControl_(void);
  void begin(void);
  void end(void);
  void write(uint8_t s);
  void press(uint8_t s);
  void release(void);
  void releaseAll(void);
  void sendReport(void* data, int length);
};

extern SystemControl_ SystemControl;

