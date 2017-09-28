/* -*- mode: c++ -*-
 * Kaleidoscope-Hardware-Virtual -- Test and debug Kaleidoscope sketches, plugins and core
 * Copyright (C) 2017  Craig Disselkoen
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <Kaleidoscope.h>
#include "Kaleidoscope-Hardware-Virtual.h"
#include <string>
#include <iostream>
#include <sstream>

Virtual::Virtual(void) {
}

void Virtual::setup(void) {
  for(byte row = 0; row < ROWS; row++) {
    for(byte col = 0; col < COLS; col++) {
      keystates[row][col] = NOT_PRESSED;
      keystates_prev[row][col] = NOT_PRESSED;
      mask[row][col] = false;
    }
  }
}

static void printHelp(void) {
  std::cout << "--- Virtual Keyboard Help ---" << std::endl;
  std::cout << "\n1. BASICS\n" << std::endl;
  std::cout << "At the prompt, specify what actions you want to take on the virtual keys for this scan cycle." << std::endl;
  std::cout << "To 'tap' a virtual key, just enter its name." << std::endl;
  std::cout << "To 'tap' multiple keys in one cycle, enter each of their names separated by a space." << std::endl;
  std::cout << "Keys are identified by their \"physical\" names, regardless of what the keymap in the current" << std::endl;
  std::cout << "  Kaleidoscope sketch may or may not be doing.  The physical name of a key is the (unshifted)" << std::endl;
  std::cout << "  text printed on the key on the standard QWERTY Model 01.  As an exception, we distinguish" << std::endl;
  std::cout << "  physical keys with the same text (ctrl, shift, and fn) with 'l' or 'r' indicating the hand." << std::endl;
  std::cout << "Here is a list of all the valid key names:" << std::endl;
  std::cout << "  prog 1 2 3 4 5 led any 6 7 8 9 0 num ` q w e r t y u i o p = pgup a s d f g tab enter h j k l ; '" << std::endl;
  std::cout << "  pgdn z x c v b esc fly n m , . / - lctrl bksp cmd lshift lfn rshift alt space rctrl rfn" << std::endl;
  std::cout << "\nExamples: ('>' is the prompt)" << std::endl;
  std::cout << "  > t             | tap the physical T key" << std::endl;
  std::cout << "  > esc           | tap the physcial esc key" << std::endl;
  std::cout << "  > lshift e      | tap the lshift and e keys simultaneously" << std::endl;
  std::cout << "  > p q lfn fly   | tap the p, q, lfn, and fly keys simultaneously" << std::endl;
  std::cout << "\n2. ADVANCED\n" << std::endl;
  std::cout << "In addition to key names, there are various commands available.  Key names are always in all" << std::endl;
  std::cout << "  lowercase (defined as symbols that appear in the unshifted positions on the standard QWERTY" << std::endl;
  std::cout << "  Model 01); uppercase/shifted symbols denote commands." << std::endl;
  std::cout << "Commands can be inserted anywhere in the input line, and affect the handling of keys following." << std::endl;
  std::cout << "The default command, which we used above, is 'tap' (where the key is 'down' for just this cycle)." << std::endl;
  std::cout << "'tap' can also be explicitly specified by 'T', as in \"> T b\" to 'tap' the physical B key." << std::endl;
  //std::cout << "Whitespace between the command and its key is optional, so the above could also be expressed as \"> Tb\"." << std::endl;
  std::cout << "You can hold virtual keys down using the 'D' (down) command.  The key will remain held until you" << std::endl;
  std::cout << "  say otherwise. While keys are held, the prompt changes from '>' to '+>'." << std::endl;
  std::cout << "You can release a previously held virtual key using the 'U' command." << std::endl;
  std::cout << "Commands affect all following keys within the line unless overridden. So, \"> D lshift u\" holds" << std::endl;
  std::cout << "  both lshift and u. To hold lshift and tap u, either enter \"> D lshift T u\", or \"> u D lshift\"." << std::endl;
  std::cout << "An exception to the above rule is the command 'C', which releases all currently held keys." << std::endl;
  std::cout << "The command '_' will process a scan cycle with no input (but held keys will remain held)." << std::endl;
  std::cout << "One final command, 'Q', will quit the program." << std::endl;
  std::cout << "\nAdvanced sequence example:" << std::endl;
  std::cout << "  > h             | tap the physical H key" << std::endl;
  std::cout << "  > D lshift      | hold the physical lshift key down" << std::endl;
  std::cout << "  +> c tab        | tap both c and tab (with lshift held)" << std::endl;
  std::cout << "  +> D alt        | hold alt (in addition to lshift)" << std::endl;
  std::cout << "  +> U lshift T e | Release lshift, and tap e in the same cycle" << std::endl;
  std::cout << "  +> _            | Do nothing for a scan cycle (but keep alt held)" << std::endl;
  std::cout << "  +> C            | Release all held keys (in this case, just alt)" << std::endl;
  std::cout << "  > enter D `     | Tap the physical enter key, and hold the ` key" << std::endl;
  std::cout << "  +> fly          | Tap the fly key (with ` key held)" << std::endl;
  std::cout << "  +> Q            | Quit the program" << std::endl;
  std::cout << std::endl;
}

typedef enum {
  M_TAP,
  M_DOWN,
  M_UP,
} Mode;

bool Virtual::anythingHeld() {
  for(byte row = 0; row < ROWS; row++) {
    for(byte col = 0; col < COLS; col++) {
      if(keystates[row][col] == PRESSED) return true;
    }
  }
  return false;
}

void Virtual::readMatrix() {
  std::cout << "Enter a command for this scan cycle, or ? or 'help' for help." << std::endl;
  if(anythingHeld()) std::cout << "+> ";
  else std::cout << "> ";
  std::string line;
  std::getline(std::cin, line);
  std::stringstream sline;
  sline << line;
  Mode mode = M_TAP;
  while(true) {
    std::string token;
    std::getline(sline, token, ' ');
    if(token == "") break;  // end of line
    else if(token == "_") break;  // do nothing this line
    else if(token == "?" || token == "help") {
      printHelp();
    } else if(token == "Q") {
      exit(0);
    } else if(token == "T") {
      mode = M_TAP;
    } else if(token == "D") {
      mode = M_DOWN;
    } else if(token == "U") {
      mode = M_UP;
    } else if(token == "C") {
      for(byte row = 0; row < ROWS; row++) {
        for(byte col = 0; col < COLS; col++) {
          keystates[row][col] = NOT_PRESSED;
        }
      }
    } else {
      rc key = getRCfromPhysicalKey(token);
      if(key.row < ROWS && key.col < COLS) {
        keystates[key.row][key.col] =
          (mode == M_DOWN) ? PRESSED :
          (mode == M_UP) ? NOT_PRESSED :
          TAP;
      } else {
        std::cout << "Unrecognized command: " << token << std::endl;
      }
    }
  }
}

void Virtual::actOnMatrixScan() {
  for (byte row = 0; row < ROWS; row++) {
    for (byte col = 0; col < COLS; col++) {
      uint8_t keyState = 0;
      switch(keystates_prev[row][col]) {
        case PRESSED:
          keyState |= WAS_PRESSED;
          break;
        case TAP:
          std::cerr << "Error: assertion failed, keystates_prev should not be TAP" << std::endl;
          break;
        case NOT_PRESSED:
        default:
          /* do nothing */
          break;
      }
      switch(keystates[row][col]) {
        case PRESSED:
        case TAP:
          keyState |= IS_PRESSED;
          break;
        case NOT_PRESSED:
        default:
          /* do nothing */
          break;
      }
      handleKeyswitchEvent(Key_NoKey, row, col, keyState);
      keystates_prev[row][col] = keystates[row][col];
      if(keystates[row][col] == TAP) {
        keyState = WAS_PRESSED & ~IS_PRESSED;
        handleKeyswitchEvent(Key_NoKey, row, col, keyState);
        keystates[row][col] = NOT_PRESSED;
        keystates_prev[row][col] = NOT_PRESSED;
      }
    }
  }
}

Virtual::rc Virtual::getRCfromPhysicalKey(std::string keyname) {
  if(keyname == "prog") return {0,0};
  else if(keyname == "1") return {0,1};
  else if(keyname == "2") return {0,2};
  else if(keyname == "3") return {0,3};
  else if(keyname == "4") return {0,4};
  else if(keyname == "5") return {0,5};
  else if(keyname == "led") return {0,6};
  else if(keyname == "any") return {0,9};
  else if(keyname == "6") return {0,10};
  else if(keyname == "7") return {0,11};
  else if(keyname == "8") return {0,12};
  else if(keyname == "9") return {0,13};
  else if(keyname == "0") return {0,14};
  else if(keyname == "num") return {0,15};
  else if(keyname == "`") return {1,0};
  else if(keyname == "q") return {1,1};
  else if(keyname == "w") return {1,2};
  else if(keyname == "e") return {1,3};
  else if(keyname == "r") return {1,4};
  else if(keyname == "t") return {1,5};
  else if(keyname == "tab") return {1,6};
  else if(keyname == "enter") return {1,9};
  else if(keyname == "y") return {1,10};
  else if(keyname == "u") return {1,11};
  else if(keyname == "i") return {1,12};
  else if(keyname == "o") return {1,13};
  else if(keyname == "p") return {1,14};
  else if(keyname == "=") return {1,15};
  else if(keyname == "pgup") return {2,0};
  else if(keyname == "a") return {2,1};
  else if(keyname == "s") return {2,2};
  else if(keyname == "d") return {2,3};
  else if(keyname == "f") return {2,4};
  else if(keyname == "g") return {2,5};
  else if(keyname == "h") return {2,10};
  else if(keyname == "j") return {2,11};
  else if(keyname == "k") return {2,12};
  else if(keyname == "l") return {2,13};
  else if(keyname == ";") return {2,14};
  else if(keyname == "'") return {2,15};
  else if(keyname == "pgdn") return {3,0};
  else if(keyname == "z") return {3,1};
  else if(keyname == "x") return {3,2};
  else if(keyname == "c") return {3,3};
  else if(keyname == "v") return {3,4};
  else if(keyname == "b") return {3,5};
  else if(keyname == "esc") return {2,6};  // yes, row 2
  else if(keyname == "fly") return {2,9};  // yes, row 2
  else if(keyname == "n") return {3,10};
  else if(keyname == "m") return {3,11};
  else if(keyname == ",") return {3,12};
  else if(keyname == ".") return {3,13};
  else if(keyname == "/") return {3,14};
  else if(keyname == "-") return {3,15};
  else if(keyname == "lctrl") return {0,7};
  else if(keyname == "bksp") return {1,7};
  else if(keyname == "cmd") return {2,7};
  else if(keyname == "lshift") return {3,7};
  else if(keyname == "rshift") return {3,8};
  else if(keyname == "alt") return {2,8};
  else if(keyname == "space") return {1,8};
  else if(keyname == "rctrl") return {0,8};
  else if(keyname == "lfn") return {3,6};
  else if(keyname == "rfn") return {3,9};
  return {255,255};
}

void Virtual::maskKey(byte row, byte col) {
  if (row >= ROWS || col >= COLS)
    return;
  mask[row][col] = true;
}

void Virtual::unMaskKey(byte row, byte col) {
  if (row >= ROWS || col >= COLS)
    return;
  mask[row][col] = false;
}

bool Virtual::isKeyMasked(byte row, byte col) {
  if (row >= ROWS || col >= COLS)
    return false;
  return mask[row][col];
}

void Virtual::maskHeldKeys(void) {
  for(byte row = 0; row < ROWS; row++) {
    for(byte col = 0; col < COLS; col++) {
      mask[row][col] = (keystates[row][col] == PRESSED);
    }
  }
}

HARDWARE_IMPLEMENTATION KeyboardHardware;
