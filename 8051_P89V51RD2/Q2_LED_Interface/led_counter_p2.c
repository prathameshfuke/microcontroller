/**
 * led_counter_p2.c
 * Display an 8-bit binary counter on Port 2 LEDs (P2.0 - P2.7)
 * Target: P89V51RD2 / 8051 family
 * Compiler: Keil C51 (reg52.h)
 *
 * Hardware:
 *   P2.0 - P2.7 --> 330R --> LED anode
 *   LED cathodes --> GND
 */

#include <reg52.h>

void delay_ms(unsigned int ms) {
    unsigned int i, j;
    for(i = 0; i < ms; i++) {
        for(j = 0; j < 123; j++);  // calibrated loop for ~1ms @ 11.0592MHz
    }
}

void main(void) {
    unsigned char count = 0;

    // Initialize Port 2 as output
    P2 = 0x00;

    while(1) {
        P2 = count;   // Output the binary count on P2 pins (LEDs)
        delay_ms(500); // adjust speed as preferred
        count++;      // 8-bit overflow wraps automatically
    }
}
