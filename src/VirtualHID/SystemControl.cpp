#include "SystemControl.h"
#include <iostream>

SystemControl_::SystemControl_(void) {}
void SystemControl_::begin(void) { releaseAll(); }
void SystemControl_::end(void) { releaseAll(); }
void SystemControl_::releaseAll(void) {
  uint8_t _report = 0x00;
  sendReport(&_report, sizeof(_report));
}

void SystemControl_::write(uint8_t s) {
  press(s);
  release();
}

void SystemControl_::release(void) { releaseAll(); }

void SystemControl_::press(uint8_t s) {
  sendReport(&s, sizeof(s));
}

void SystemControl_::sendReport(void* data, int length) {
  // TODO: More informative here
  std::cout << "A virtual SystemControl HID report with value " << *(uint8_t*)data << " was sent." << std::endl;
}

SystemControl_ SystemControl;
