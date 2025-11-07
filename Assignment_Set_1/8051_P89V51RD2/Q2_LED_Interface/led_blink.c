/******************************************************************************
 * P89V51RD2 LED Blinking Program
 * Experiment Q2-i: LED Blinking Pattern
 *
 * Author: Prathamesh Lab
 * Date: November 7, 2025
 * Target: P89V51RD2 (8051 compatible)
 * Crystal: 11.0592 MHz
 * 
 * Description:
 *   Demonstrates LED blinking patterns on Port 2:
 *   1. All LEDs blink together
 *   2. Alternating pattern (odd/even)
 *   3. Left-to-right chase
 ******************************************************************************/

#include <reg51.h>

// Function prototypes
void delay_ms(unsigned int ms);
void all_blink(void);
void alternate_blink(void);
void chase_pattern(void);

/******************************************************************************
 * Main Function
 ******************************************************************************/
void main(void) {
    // Port 2 pins configured as output by default
    P2 = 0x00;  // All LEDs off initially
    
    while(1) {
        // Pattern 1: All LEDs blink together (5 times)
        all_blink();
        
        // Pattern 2: Alternate odd/even LEDs (5 times)
        alternate_blink();
        
        // Pattern 3: Chase pattern left to right (2 complete cycles)
        chase_pattern();
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
 * Pattern 1: All LEDs blink together
 ******************************************************************************/
void all_blink(void) {
    unsigned char i;
    for(i = 0; i < 5; i++) {  // 5 cycles
        P2 = 0xFF;    // All LEDs on
        delay_ms(500);
        P2 = 0x00;    // All LEDs off
        delay_ms(500);
    }
}

/******************************************************************************
 * Pattern 2: Alternate between odd and even LEDs
 ******************************************************************************/
void alternate_blink(void) {
    unsigned char i;
    for(i = 0; i < 5; i++) {  // 5 cycles
        P2 = 0x55;    // Odd LEDs (0b01010101)
        delay_ms(500);
        P2 = 0xAA;    // Even LEDs (0b10101010)
        delay_ms(500);
    }
}

/******************************************************************************
 * Pattern 3: LED chase pattern (right to left)
 ******************************************************************************/
void chase_pattern(void) {
    unsigned char i, pattern;
    
    // Two complete cycles
    for(i = 0; i < 2; i++) {
        // Left to right
        pattern = 0x01;  // Start with rightmost LED
        while(pattern != 0) {
            P2 = pattern;
            delay_ms(200);
            pattern = pattern << 1;  // Shift left
        }
    }
}

/******************************************************************************
 * Hardware Connections:
 * P2.0 - P2.7: LEDs (through current-limiting resistors)
 *   LED Anode -> 330Ω resistor -> Pn.x
 *   LED Cathode -> GND
 *
 * Notes:
 * 1. Each port pin can source/sink up to 16mA
 * 2. Use 330Ω resistors for 5V operation
 * 3. Adjust delay_ms() if using different crystal frequency
 ******************************************************************************/