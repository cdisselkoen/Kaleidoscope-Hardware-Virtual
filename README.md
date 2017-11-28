# Kaleidoscope-Hardware-Virtual

### NOTE: This repository is no longer active; development has moved to [the `keyboardio` org][kbrepo].

This is a plugin for [Kaleidoscope][fw], targeted at **developers** (of Kaleidoscope
or plugins for Kaleidoscope).  It allows you to test Kaleidoscope sketches,
plugins, and changes to the core without physical hardware.  This has several
advantages:
  1. You can test without access to physical hardware.  Maybe you want to work
      at a coffee shop, and don't want to lug along physical hardware.  Or maybe
      your physical hardware is broken or unavailable, and you want to test
      something.  Or maybe you don't have any Kaleidoscope-compatible hardware
      at all (yet), and want to develop for Kaleidoscope anyway. (Woot!)
  2. You can test without disturbing your physical hardware.  If, like me, you
      do development on your Kaleidoscope hardware, it can be irksome to test
      on that same hardware - if you flash a build with unusual settings you're
      not used to, or a build that is less than stable, this can disrupt the
      development process itself.  Testing on virtual hardware allows you to work
      without disturbing your development process.
      Also, not having to flash every time you want to test saves time.
  3. Virtual hardware has very useful debugging features.  The built Kaleidoscope
      image is a normal, full x86 program, which means you can use any debugging
      tools you normally would, such as `gdb`.  You can also insert statements
      to print to console, log things to file, or do anything else - inside your
      sketch or anywhere in the Kaleidoscope core or its plugins.

## Installation

1. Find your plugin directory.  This is wherever you normally put plugins; it should be
`$SKETCHBOOK_DIR/hardware/keyboardio/avr/libraries`, or at least accessible from there
via symlink.  `$SKETCHBOOK_DIR` is your Arduino sketchbook directory, perhaps
`$HOME/Arduino` or `$HOME/Documents/Arduino`.

1. Download Kaleidoscope-Hardware-Virtual into your plugin directory:

   1. __(Option 1 - using Git)__ Clone this Git repo into your plugin directory.
      From the command line,

       `git clone https://github.com/cdisselkoen/Kaleidoscope-Hardware-Virtual`

   1. __(Option 2 - no Git or command-line required)__ Click the green "Clone or download"
      button at the top right of this page, and select "Download ZIP".  Then, unzip the
      folder in your plugin directory.

1. Symlink this plugin's `support/x86` directory as `$SKETCHBOOK_DIR/hardware/keyboardio/x86`.
   E.g.

    `ln -s support/x86 /path/to/sketchbook/hardware/keyboardio/x86`

1. Symlink your plugin directory (`$SKETCHBOOK_DIR/hardware/keyboardio/avr/libraries`) as
   `support/x86/libraries`.  E.g.

    `ln -s /path/to/sketchbook/hardware/keyboardio/avr/libraries support/x86/libraries`

## Usage

Compile your sketch however you normally would, but with the environment variable `BOARD`
set to `"virtual"` (e.g. `BOARD=virtual make`).
Note that `.kaleidoscope-builder.conf` in your current directory may override the value
of `BOARD`, and you may or may not want this.

This will produce an ordinary x86 executable, `output/<sketch_name>/<sketch_name>-latest.elf`,
which you can run just like any other program.  Run this program to test your sketch.

### Input and output

Input in terms of physical keypresses is handled one of two ways - either interactively
(at the command line), or through a script.  To use a script, simply supply its filename
as the argument to the executable; or for interactive mode, use "-i" as the argument.
Run with no arguments (or type '?' or 'help' at the interactive prompt)
for instructions and examples.

Output, in terms of HID reports (packets sent to the host computer, for real hardware),
is printed to the command line (i.e. `stdout`) as it happens, in summarized/human-readable
form.  Raw HID output and serial output (through the `Serial` object) are collected and
redirected to various files in a subdirectory "results" of the current directory.
This feature is intended mostly for purposes of automated testing/validation, but if you
wish to watch the raw or serial output in real time in a separate window during interactive
mode, I recommend `tail -f -n 80 results/whatever.txt`.

Serial input is currently unsupported - sketches requesting it will still build, but will
find nothing is ever transmitted to them on the serial port.

## Limitations

This virtual hardware plugin essentially intercepts function calls from the
Kaleidoscope firmware into:
* the Kaleidoscope hardware plugins (`Kaleidoscope-Hardware-*`),
* the KeyboardioHID plugin, and
* the Arduino core.

As a result, anything in these plugins or in the Arduino core is not simulated/tested.
This means you can test the Kaleidoscope core, most Kaleidoscope plugins, and any
particular Kaleidoscope sketch you want, but not changes to the hardware plugins themselves
or any lower-level Arduino details.

Many major features are currently unsupported (such as LEDs).  In most cases,
"unsupported" means sketches using these features will still build and can still be
tested using virtual hardware, but that functionality is removed.  For instance,
currently, calls to set LEDs simply have no effect on the virtual hardware.

In a few cases, using certain features (or calling certain functions in the Arduino
core) may cause the sketch to fail to build on virtual hardware.  If you encounter
these, open an issue on GitHub and it should be pretty easy to get those features
moved at least to the "unsupported-but-still-builds" state.

Currently, the virtual hardware's key layout resembles the Model 01, in the sense
that it uses the same `KEYMAP()` and `KEYMAP_STACKED()` macros that the Model 01 does,
and expects sketches to specify their keymaps in that format.  It also relies on the
Model 01's key layout for command specification, explained more thoroughly in the
help message.  Modifying or extending the virtual hardware to support other key
layouts shouldn't be very hard, if you have a desire for that.  Pull requests welcome.

If you have feature requests or issues to report, again feel free to open issues on
GitHub!

 [fw]: https://github.com/keyboardio/Kaleidoscope
 [kbrepo]: https://github.com/keyboardio/Kaleidoscope-Hardware-Virtual
