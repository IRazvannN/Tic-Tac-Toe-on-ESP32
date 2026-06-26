# ESP32-S3 Tic-Tac-Toe Game Engine

A self-contained state-machine based Tic-Tac-Toe game engine compiled for the ESP32-S3 MCU architecture, featuring a custom 3x3 matrix UI layout mapped to a 128x32 SSD1306 OLED screen.

## Hardware Demonstration
Below is an operational demonstration of the matrix updates, win state conditions, and drawing procedures running on the physical module:

[![Watch the demonstration](https://img.youtube.com/vi/Hj2890d6KfE/0.jpg)](https://youtu.be/Hj2890d6KfE)

## Hardware Specifications
- **Microcontroller:** ESP32-S3
- **Display Module:** SSD1306 128x32 OLED Panel
- **I2C Data Line (SDA):** GPIO 47
- **I2C Clock Line (SCL):** GPIO 48
- **Input Matrix Interface:** USB CDC Hardware Serial Protocol

## Architectural Design

### State Validation Matrix
The state machine monitors a 9-element 1D array to validate moves, toggle turns, and execute linear combination matches against pre-compiled multi-dimensional win coordinate sets:

<pre>
 Grid Index Mapping:       Border Coordinate Limits:
    1 | 2 | 3                 col1: X=74, height=32
   -----------                col2: X=86, height=32
    4 | 5 | 6                 row1: Y=10, width=32
   -----------                row2: Y=21, width=32
    7 | 8 | 9              Render Window bounding box: X=64 to X=96
</pre>

On every input frame, the active vector configurations are processed against eight unique index combinations covering horizontal rows, vertical columns, and diagonals.

## Software Dependencies
- Arduino IDE (ESP32 Core Package v2.x or later)
- Adafruit GFX Library
- Adafruit SSD1306 Library

## Installation and Deployment
1. Verify target motherboard line matching for custom pins (GPIO 47/48).
2. Install dependencies via the Library Manager interface in the IDE environment.
3. Upload the project sketch binary directly to the target partition blocks.
4. Open an active serial tracking terminal at 115200 baud. Use number inputs `1`-`9` to command positions across the active board matrix.
