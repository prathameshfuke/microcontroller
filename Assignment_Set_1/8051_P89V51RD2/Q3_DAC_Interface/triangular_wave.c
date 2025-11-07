/******************************************************************************
 * P89V51RD2 DAC Interface - Triangular Wave Generation
 * Target: P89V51RD2 (8051 compatible)
 * Crystal: 11.0592 MHz
 *
 * Connections:
 * P1.0-P1.7 -> DAC0808 (D0-D7)
 * DAC0808 output -> CRO input
 *
 * Description:
 * Generates triangular wave by ramping DAC value up and down
 * Frequency determined by delay between steps
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
    bit ramp_up = 1;  // 1 = ramping up, 0 = ramping down

    while(1) {
        P1 = value;  // Output to DAC
        
        if(ramp_up) {
            value++;           // Increment for rising edge
            if(value == 255)  // At peak
                ramp_up = 0;  // Start ramping down
        }
        else {
            value--;          // Decrement for falling edge
            if(value == 0)    // At bottom
                ramp_up = 1;  // Start ramping up
        }
        
        delay_ms(1);  // Control slope (frequency)
    }
}