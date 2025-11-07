/******************************************************************************
 * P89V51RD2 LED Hex Display Program
 * Experiment Q2-ii: Display Hex Numbers on LEDs
 *
 * Author: Prathamesh Lab
 * Date: November 7, 2025
 * Target: P89V51RD2 (8051 compatible)
 * Crystal: 11.0592 MHz
 *
 * Description:
 *   Displays hexadecimal numbers (0-F) on Port 2 LEDs
 *   Shows hex counting pattern with visual feedback
 ******************************************************************************/

#include <reg51.h>

// Function prototypes
void delay_ms(unsigned int ms);
void display_hex(unsigned char hex);
void flash_display(void);

// Hex patterns for visual representation
code unsigned char hex_patterns[] = {
    0x3F,  // 0: 0011 1111
    0x06,  // 1: 0000 0110
    0x5B,  // 2: 0101 1011
    0x4F,  // 3: 0100 1111
    0x66,  // 4: 0110 0110
    0x6D,  // 5: 0110 1101
    0x7D,  // 6: 0111 1101
    0x07,  // 7: 0000 0111
    0x7F,  // 8: 0111 1111
    0x6F,  // 9: 0110 1111
    0x77,  // A: 0111 0111
    0x7C,  // B: 0111 1100
    0x39,  // C: 0011 1001
    0x5E,  // D: 0101 1110
    0x79,  // E: 0111 1001
    0x71   // F: 0111 0001
};

/******************************************************************************
 * Main Function
 ******************************************************************************/
void main(void) {
    unsigned char i;
    
    // Configure Port 2 (output by default)
    P2 = 0x00;  // All LEDs off initially
    
    while(1) {
        // Display hex numbers 0-F
        for(i = 0; i < 16; i++) {
            display_hex(i);
            delay_ms(1000);  // Show each digit for 1 second
        }
        
        // Flash display to indicate cycle complete
        flash_display();
    }
}

/******************************************************************************
 * Delay Function: Approximately 1ms @ 11.0592MHz
 ******************************************************************************/
void delay_ms(unsigned int ms) {
    unsigned int i, j;
    for(i = 0; i < ms; i++)
        for(j = 0; j < 123; j++);  // Calibrated for 11.0592 MHz
}

/******************************************************************************
 * Display hex number (0-F) on LEDs
 ******************************************************************************/
void display_hex(unsigned char hex) {
    if(hex < 16) {  // Valid hex digit
        P2 = hex_patterns[hex];
    }
}

/******************************************************************************
 * Flash the display to indicate cycle completion
 ******************************************************************************/
void flash_display() {
    unsigned char i;
    for(i = 0; i < 3; i++) {  // Flash 3 times
        P2 = 0xFF;  // All on
        delay_ms(200);
        P2 = 0x00;  // All off
        delay_ms(200);
    }
    delay_ms(800);  // Pause before next cycle
}

/******************************************************************************
 * Hardware Connections:
 * P2.0 - P2.7: LEDs (through current-limiting resistors)
 *   LED Anode -> 330Ω resistor -> Pn.x
 *   LED Cathode -> GND
 *
 * LED Mapping for Hex Display:
 *   MSB [P2.7 P2.6 P2.5 P2.4 P2.3 P2.2 P2.1 P2.0] LSB
 *
 * Pattern Examples:
 *   0 = 0x3F = 0011 1111 = Bottom 6 LEDs
 *   1 = 0x06 = 0000 0110 = Two middle LEDs
 *   8 = 0x7F = 0111 1111 = All LEDs except MSB
 *   F = 0x71 = 0111 0001 = Top 3 LEDs + bottom LED
 ******************************************************************************/