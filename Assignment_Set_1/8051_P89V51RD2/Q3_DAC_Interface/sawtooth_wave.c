/******************************************************************************
 * P89V51RD2 DAC Interface - Sawtooth Wave Generation
 * Target: P89V51RD2 (8051 compatible)
 * Crystal: 11.0592 MHz
 *
 * Connections:
 * P1.0-P1.7 -> DAC0808 (D0-D7)
 * DAC0808 output -> CRO input
 ******************************************************************************/

#include <reg51.h>

void delay_ms(unsigned int ms) {
    unsigned int i, j;
    for(i = 0; i < ms; i++) {
        for(j = 0; j < 123; j++);  // For 11.0592 MHz
    }
}

void main(void) {
    unsigned char value = 0;

    while(1) {
        P1 = value;     // Output to DAC
        value++;        // Increment for ramp
        delay_ms(1);    // 1ms between steps
    }
}