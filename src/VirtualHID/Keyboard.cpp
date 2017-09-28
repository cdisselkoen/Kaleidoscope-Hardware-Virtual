#include "Keyboard.h"
#include <iostream>

Keyboard_::Keyboard_(void) {}
void Keyboard_::begin(void) {
  releaseAll();
}
void Keyboard_::end(void) {
  releaseAll();
}

// press(), release(), releaseAll(), isModifierActive(), and wasModifierActive() are all
// taken directly from KeyboardioHID's versions
size_t Keyboard_::press(uint8_t k) {
  // If the key is in the range of 'printable' keys
  if (k <= HID_LAST_KEY) {
    uint8_t bit = 1 << (uint8_t(k) % 8);
    _keyReport.keys[k / 8] |= bit;
    return 1;
  }

  // It's a modifier key
  else if (k >= HID_KEYBOARD_FIRST_MODIFIER && k <= HID_KEYBOARD_LAST_MODIFIER) {
    // Convert key into bitfield (0 - 7)
    k = k - HID_KEYBOARD_FIRST_MODIFIER;
    _keyReport.modifiers |= (1 << k);
    return 1;
  }

  // No empty/pressed key was found
  return 0;
}
size_t Keyboard_::release(uint8_t k) {
  // If we're releasing a printable key
  if (k <= HID_LAST_KEY) {
    uint8_t bit = 1 << (k % 8);
    _keyReport.keys[k / 8] &= ~bit;
    return 1;
  }

  // It's a modifier key
  else if (k >= HID_KEYBOARD_FIRST_MODIFIER && k <= HID_KEYBOARD_LAST_MODIFIER) {
    // Convert key into bitfield (0 - 7)
    k = k - HID_KEYBOARD_FIRST_MODIFIER;
    _keyReport.modifiers &= ~(1 << k);
    return 1;
  }

  // No empty/pressed key was found
  return 0;
}
void Keyboard_::releaseAll(void) {
  memset(&_keyReport.allkeys, 0x00, sizeof(_keyReport.allkeys));
}
boolean Keyboard_::isModifierActive(uint8_t k) {
  if (k >= HID_KEYBOARD_FIRST_MODIFIER && k <= HID_KEYBOARD_LAST_MODIFIER) {
    k = k - HID_KEYBOARD_FIRST_MODIFIER;
    return !!(_keyReport.modifiers & (1 << k));
  }
  return false;
}
boolean Keyboard_::wasModifierActive(uint8_t k) {
  if(k >= HID_KEYBOARD_FIRST_MODIFIER && k <= HID_KEYBOARD_LAST_MODIFIER) {
    k = k - HID_KEYBOARD_FIRST_MODIFIER;
    return !!(_lastKeyReport.modifiers & (1 << k));
  }
  return false;
}

// For each bit set in 'bitfield', output the corresponding string to cout
#define FOREACHBIT(bitfield, str0, str1, str2, str3, str4, str5, str6, str7) \
  if((bitfield) & 1<<0) std::cout << str0; \
  if((bitfield) & 1<<1) std::cout << str1; \
  if((bitfield) & 1<<2) std::cout << str2; \
  if((bitfield) & 1<<3) std::cout << str3; \
  if((bitfield) & 1<<4) std::cout << str4; \
  if((bitfield) & 1<<5) std::cout << str5; \
  if((bitfield) & 1<<6) std::cout << str6; \
  if((bitfield) & 1<<7) std::cout << str7;

int Keyboard_::sendReport(void) {
  // Following KeyboardioHID, we only send report if it differs from previous report.
  if(!memcmp(_lastKeyReport.allkeys, _keyReport.allkeys, sizeof(_keyReport))) return -1;

  std::cout << "Sent virtual HID report. Pressed keys: ";
  bool anything = false;
  if(_keyReport.modifiers) anything = true;
  else for(int i = 0; i < KEY_BYTES; i++) if(_keyReport.keys[i]) { anything = true; break; }
  if(!anything) {
    std::cout << "none";
  } else {
    FOREACHBIT(_keyReport.modifiers, "lctrl ", "lshift ", "lalt ", "lgui ",
                                     "rctrl ", "rshift ", "ralt ", "rgui ")
    FOREACHBIT(_keyReport.keys[0], "NO_EVENT ", "ERROR_ROLLOVER ", "POST_FAIL ", "ERROR_UNDEFINED ",
                                   "a ", "b ", "c ", "d ")
    FOREACHBIT(_keyReport.keys[1], "e ", "f ", "g ", "h ", "i ", "j ", "k ", "l ")
    FOREACHBIT(_keyReport.keys[2], "m ", "n ", "o ", "p ", "q ", "r ", "s ", "t ")
    FOREACHBIT(_keyReport.keys[3], "u ", "v ", "w ", "x ", "y ", "z ", "1/! ", "2/@ ")
    FOREACHBIT(_keyReport.keys[4], "3/# ", "4/$ ", "5/% ", "6/^ ", "7/& ", "8/* ", "9/( ", "0/) ")
    FOREACHBIT(_keyReport.keys[5], "enter ", "esc ", "del/bksp ", "tab ",
                                   "space ", "-/_ ", "=/+ ", "[/{ ")
    FOREACHBIT(_keyReport.keys[6], "]/} ", "\\/| ", "#/~ ", ";/: ", "'/\" ", "`/~ ", ",/< ", "./> ")
    FOREACHBIT(_keyReport.keys[7], "//? ", "capslock ", "F1 ", "F2 ", "F3 ", "F4 ", "F5 ", "F6 ")
    FOREACHBIT(_keyReport.keys[8], "F7 ", "F8 ", "F9 ", "F10 ", "F11 ", "F12 ", "prtscr ", "scrolllock ")
    FOREACHBIT(_keyReport.keys[9], "pause ", "ins ", "home ", "pgup ", "del ", "end ", "pgdn ", "r_arrow ")
    FOREACHBIT(_keyReport.keys[10], "l_arrow ", "d_arrow ", "u_arrow ", "numlock ",
                                    "num/ ", "num* ", "num- ", "num+ ")
    FOREACHBIT(_keyReport.keys[11], "numenter ", "num1 ", "num2 ", "num3 ", "num4 ", "num5 ", "num6 ", "num7 ")
    FOREACHBIT(_keyReport.keys[12], "num8 ", "num9 ", "num0 ", "num. ", "\\/| ", "app ", "power ", "num= ")
    FOREACHBIT(_keyReport.keys[13], "F13 ", "F14 ", "F15 ", "F16 ", "F17 ", "F18 ", "F19 ", "F20 ")
    FOREACHBIT(_keyReport.keys[14], "F21 ", "F22 ", "F23 ", "F24 ", "exec ", "help ", "menu ", "sel ")
    FOREACHBIT(_keyReport.keys[15], "stop ", "again ", "undo ", "cut ", "copy ", "paste ", "find ", "mute ")
    FOREACHBIT(_keyReport.keys[16], "volup ", "voldn ", "capslock_l ", "numlock_l ",
                                    "scrolllock_l ", "num, ", "num= ", "(other) ")
    for(int i = 17; i < KEY_BYTES; i++) {
      // might undercount the number of "other" keys pressed, but whatever
      // If any of these keys are frequently used, we should just put them in by name anyway
      if(_keyReport.keys[i]) std::cout << "(other) ";
    }
  }
  std::cout << std::endl;
  memcpy(_lastKeyReport.allkeys, _keyReport.allkeys, sizeof(_keyReport));
  return 0;  // actually not sure what the return value should be here; seems that
             // existing code doesn't check it
}

Keyboard_ Keyboard;
