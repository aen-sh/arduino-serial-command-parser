# Arduino Serial Command Parser

A serial command parser for Arduino with non-blocking command execution.
Commands are sent as plain text over Serial and parsed on-device without
using `delay()`, so the board stays responsive while a command (like
blinking an LED) is still running.

## Hardware

- Any Arduino board (Uno, Nano, etc.)
- USB cable
- No additional components required — uses the built-in LED (`LED_BUILTIN`)

## Commands

| Command      | Description                                  |
|--------------|-----------------------------------------------|
| `LED ON`     | Turn the LED on, cancels any ongoing blink    |
| `LED OFF`    | Turn the LED off, cancels any ongoing blink   |
| `BLINK <n>`  | Blink the LED `n` times (non-blocking)        |
| `STATUS`     | Print current LED state (ON/OFF)              |
| `HELP`       | List available commands                       |

### Examples

LED ON
LED OFF
BLINK 5
STATUS


## How it works

- Incoming Serial bytes are read one at a time into a fixed-size buffer
  (64 bytes) until a newline is received.
- The buffer is bounds-checked to prevent overflow; an oversized line
  resets the buffer and reports an error instead of corrupting memory.
- The line is split into a command and an optional argument by locating
  the first space and inserting a null terminator in place.
- `BLINK` does not block execution with `delay()`. Instead, it sets a
  `BLINKING` state and toggles the LED based on elapsed time (`millis()`),
  so new commands can still be received and processed while blinking
  is in progress.

## Build

This project uses [PlatformIO](https://platformio.org/).

1. Open the project folder in VS Code with the PlatformIO extension installed.
2. Connect the Arduino board via USB.
3. Click **Upload** (or `Ctrl+Alt+U`).
4. Open the Serial Monitor (`Ctrl+Alt+S`) at 9600 baud.
5. Type a command and press Enter.

## Known limitations

- Only ASCII input is handled correctly; multi-byte characters (e.g.
  Cyrillic in UTF-8) may print unexpected byte fragments if a buffer
  overflow occurs mid-character. This does not affect ASCII commands.
- Sending a new `BLINK` command while one is already in progress
  restarts the blink counter without warning (no queueing or rejection).

## Possible next steps

- EEPROM-based configuration (e.g. persisting blink interval)
- Splitting parsing and state machine logic into separate modules
- Unit tests for the command parser