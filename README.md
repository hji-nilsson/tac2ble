# tac2ble
Project for adding BLE support to a C64 joystick based on the Adafruit Feather 32u4 Bluefruit LE.
Still work in progress so full functionality not implemented.

## Functionality
Whats works now? It reads the input ports and convert that to a key command string to send over BLE.

## Mapping
Up:    Keyboard up arrow
Down:  Keyboard down arrow
Left:  Keyboard left arrow
Right: Keyboard right arrow
Fire:  Keyboard enter

Final mapping is still to be decided but this is what is used for debug purposes.

## How to build
At the moment I would recommend using the Arduino IDE to build and flash but I plan to add support for the standard GNU toolchain.
