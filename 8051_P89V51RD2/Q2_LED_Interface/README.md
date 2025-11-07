# Q2 - LED Interface with 8051 (P89V51RD2)

This folder contains simple Embedded C examples to interface LEDs with an 8051-family microcontroller (P89V51RD2).

Files added:
- `led_blink_p2.c`   : Blink single LED on P2.0 (500 ms)
- `led_counter_p2.c` : Display 8-bit binary counter on P2 (P2.0 - P2.7)
- `led_display_hex.c`: Output a fixed hex pattern (0x3F) on P2

Wiring / Interfacing diagram (ASCII)

  +5V
   |
  Vcc (8051)
   |
  GND ---------------------------------+
                                        |
  P2.0 --- 330R ---|>|--- GND (LED0)  |
  P2.1 --- 330R ---|>|--- GND (LED1)  |
  P2.2 --- 330R ---|>|--- GND (LED2)  |
  P2.3 --- 330R ---|>|--- GND (LED3)  |
  P2.4 --- 330R ---|>|--- GND (LED4)  |
  P2.5 --- 330R ---|>|--- GND (LED5)  |
  P2.6 --- 330R ---|>|--- GND (LED6)  |
  P2.7 --- 330R ---|>|--- GND (LED7)  |

Notes:
- Use current-limiting resistors (220Ω-1kΩ, 330Ω typical).
- Adjust LED polarity in code if your LEDs are wired active-low.
- Port choice: examples use Port 2 to avoid conflicts with on-chip peripherals on Port 1.
- Calibrated delays assume a 11.0592 MHz crystal. If using a different crystal, adjust delay constants.

Build / Run (Keil uVision 5):
1. Create a new project and select the P89V51RD2 device.
2. Add any of the `.c` files to Source Group 1.
3. Set the target crystal frequency to 11.0592 MHz in Target Options.
4. Build and flash the generated HEX to your board.

Safety / Troubleshooting:
- If LEDs are dim, verify resistor values and supply voltage (+5V).
- If LEDs don't light, check wiring and LED orientation (anode to port via resistor, cathode to GND).
- If program behaves unexpectedly, confirm target device and compiler options in Keil.

Suggested next steps:
- Add a debounce/button controlled mode to step the counter.
- Implement brightness control using PWM on a single LED (via timer interrupts).
