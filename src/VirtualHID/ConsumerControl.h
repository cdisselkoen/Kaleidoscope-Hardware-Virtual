#pragma once

// Essentially equivalent to the corresponding header file in KeyboardioHID
// with the goal of having an almost identical interface, with different implementation

#include "Arduino.h"
#include "HIDTables.h"

#define KEY_BYTES 28

typedef union {
  uint16_t keys[4];
  struct {
    uint16_t key1;
    uint16_t key2;
    uint16_t key3;
    uint16_t key4;
  };
} HID_ConsumerControlReport_Data_t;

class ConsumerControl_ {
  public:
    ConsumerControl_(void);
    void begin(void);
    void end(void);
    void write(uint16_t m);
    void press(uint16_t m);
    void release(uint16_t m);
    void releaseAll(void);
    void sendReport(void* data, int length);

  protected:
    HID_ConsumerControlReport_Data_t _report;
};

extern ConsumerControl_ ConsumerControl;
