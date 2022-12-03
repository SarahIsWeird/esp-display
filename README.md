# ESP Display

ESP Display is a simple project that allows you to speak to a 20x4 I2C LCD display from your computer. This is mostly intended as a demonstration for my presentation on drivers at the Berlin School of Economics and Law.

## Protocol

Communication happens via serial in a binary format. It's not intended to be the perfect format that reduces bandwidth as much as possible, it's just something that I can work with and demonstrate.

The baud rate is 115,200. The first byte determines the command. If the command doesn't include a length byte, it will also determine the length of the rest of the command.

### Host to device

- `00` - (Re-)initialize the display.
- `01 aa` - Sets display flags. Flags can be OR'd together.
  - `01` - Enable backlight
  - `02` - Enable cursor
  - `04` - Enable blink
  - `08` - Enable line breaks (text longer than the current line is placed in the next line instead of being cut off)
  - `10`-`80` - Reserved
- `02` - Clears the display.
- `03 xx yy` - Sets the cursor row (y) and column (x).
- `04 aa ...` - Prints a string of text (`...`) of length `aa` at the current cursor position. Should **not** include an ending NULL byte.

#### Example

```
00 // initialize display
01 03 // turn on backlight and cursor
02 // clear the display
03 00 00 // set the cursor to row 0 col 0
04 03 "hii" // print "hii"
03 00 01 // set the cursor to row 1 col 0
04 04 "hru?" // print "hru?"
```

### Device to host

Anything sent from the device to the host that isn't part of one of these messages can be ignored.
It will most likely be either debugging output or startup logs from the bootloader.

- `00` - The device is ready to receive commands.
- `01` - Last command succeeded
- `02 aa` - Last command had an error with error code `aa`:
  - `01` - Internal device error, most likely unsuccessful communication with the display
  - `02` - Invalid command
  - `03` - Invalid row
  - `04` - Invalid column
- `03 aa aa ...` - Log message with length `aa aa`. Can be ignored if you don't wish to see logs, mostly used for debugging purposes.
