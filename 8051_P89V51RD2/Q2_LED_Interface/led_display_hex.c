/**
 * led_display_hex.c
 * Output a fixed hexadecimal pattern on Port 2 LEDs.
 * Example shown: display 0x3F (common 7-seg pattern) on P2.
 * Target: P89V51RD2 / 8051 family
 * Compiler: Keil C51 (reg52.h)
 *
 * Hardware:
 *   P2.0 - P2.7 --> 330R --> LED anode
 *   LED cathodes --> GND
 */

#include <reg52.h>

/*
 * Use XBYTE[...] to write to external data memory (xdata) address.
 * Adjust LED_ADDR to match the external latch/address where LEDs are
 * mapped on your hardware. Keil C51 supports the XBYTE[] operator for
 * direct external data access (external RAM / memory-mapped I/O).
 */
#define LED_ADDR 0x8000u

void main(void) {
    /* Example: output fixed hex value via memory-mapped I/O */
    /* 0x3F = 0011 1111 (decimal 63) */
    XBYTE[LED_ADDR] = 0x3F;

    /* Keep program running so pattern remains on LEDs */
    while(1);
}
