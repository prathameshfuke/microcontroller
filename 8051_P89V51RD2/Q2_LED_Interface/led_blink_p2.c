/**
 * led_blink_p2.c
 * Blink a single LED connected to P2.0 (8051 family, reg52 style)
 * Target: P89V51RD2 / AT89C51 / 8051 compatible
 * Compiler: Keil C51 (reg52.h)
 *
 * Connection:
 *   P2.0 --> 330R --> LED anode
 *   LED cathode --> GND
 *
 */

#include <reg52.h>

sbit LED = P2^0;  // LED connected to Pin P2.0

void delay_ms(unsigned int ms) {
    unsigned int i, j;
    for(i = 0; i < ms; i++) {
        for(j = 0; j < 123; j++);  // calibrated for ~1ms @ 11.0592MHz
    }
}

void main(void) {
    // Configure P2 bits as outputs by writing 0/1 as needed.
    // For standard 8051 quasi-bidirectional ports, writing 0 forces low.
    LED = 1; // LED off (adjust if your LED is active-low)

    while(1) {
        LED = 0;    // Turn LED ON (adjust polarity if wiring is active-high)
        delay_ms(500);
        LED = 1;    // Turn LED OFF
        delay_ms(500);
    }
}
