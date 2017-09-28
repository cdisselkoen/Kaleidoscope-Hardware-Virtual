#include "Arduino.h"

// TODO: better time emulation
// this is pretty hacky, but hopefully helps most code behave sanely
unsigned long millis(void) {
  static unsigned long time = 0;
  return time++;
}
unsigned long micros(void) {
  return millis()*1000;
}
