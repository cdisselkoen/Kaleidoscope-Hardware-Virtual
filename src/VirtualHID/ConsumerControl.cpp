#include "ConsumerControl.h"
#include <iostream>
#include "virtual_io.h"

ConsumerControl_::ConsumerControl_(void) {}
void ConsumerControl_::begin(void) { releaseAll(); }
void ConsumerControl_::end(void) { releaseAll(); }
void ConsumerControl_::releaseAll(void) {
  memset(&_report, 0, sizeof(_report));
  sendReport(&_report, sizeof(_report));
}

// write(), press(), and release() are essentially taken directly from KeyboardioHID
void ConsumerControl_::write(uint16_t m) {
  press(m);
  release(m);
}
void ConsumerControl_::press(uint16_t m) {
  // search for a free spot
  for (uint8_t i = 0; i < sizeof(_report) / 2; i++) {
    if (_report.keys[i] == 0x00) {
      _report.keys[i] = m;
      break;
    }
  }
  sendReport(&_report, sizeof(_report));
}
void ConsumerControl_::release(uint16_t m) {
  // search and release the keypress
  for (uint8_t i = 0; i < sizeof(_report) / 2; i++) {
    if (_report.keys[i] == m) {
      _report.keys[i] = 0x00;
      // no break to delete multiple keys
    }
  }
  sendReport(&_report, sizeof(_report));
}

void ConsumerControl_::sendReport(void* data, int length) {
  std::cout << "A virtual ConsumerControl HID report was sent." << std::endl;
  logUSBEvent("ConsumerControl HID report", data, length);
}

ConsumerControl_ ConsumerControl;
