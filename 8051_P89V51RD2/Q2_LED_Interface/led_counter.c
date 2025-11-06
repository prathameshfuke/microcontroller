/******************************************************************************
 * P89V51RD2 LED Counter Program - Embedded C
 * Experiment Q2 Part (ii): LED as Binary Counter Display
 *
 * Author: Microcontroller Lab
 * Date: November 6, 2025
 * Target Device: P89V51RD2 (8051 compatible)
 * IDE: Keil uVision 5
 * Compiler: C51
 *
 * Description:
 *   This program displays binary counting from 0 to 255 on 8 LEDs connected
 *   to Port 1. Each count is displayed for 1 second, then increments.
 *   Counter resets to 0 after reaching 255.
 *
 * Hardware Connection:
 *   P1.0 (LSB) --> 330Ω --> LED0 --> GND
 *   P1.1       --> 330Ω --> LED1 --> GND
 *   P1.2       --> 330Ω --> LED2 --> GND
 *   P1.3       --> 330Ω --> LED3 --> GND
 *   P1.4       --> 330Ω --> LED4 --> GND
 *   P1.5       --> 330Ω --> LED5 --> GND
 *   P1.6       --> 330Ω --> LED6 --> GND
 *   P1.7 (MSB) --> 330Ω --> LED7 --> GND
 *
 * Binary Display Example:
 *   Count = 5  --> LEDs = 00000101 (P1.0 and P1.2 ON)
 *   Count = 255 --> LEDs = 11111111 (All LEDs ON)
 *
 * Crystal Frequency: 11.0592 MHz
 ******************************************************************************/

#include <reg51.h>      // Standard 8051 register definitions

/******************************************************************************
 * Function: delay_ms
 * Description: Software delay function for millisecond delays
 * Parameters: ms - delay duration in milliseconds
 * Returns: None
 * Note: Calibrated for 11.0592 MHz crystal
 ******************************************************************************/
void delay_ms(unsigned int ms) {
    unsigned int i, j;
    for(i = 0; i < ms; i++) {
        for(j = 0; j < 123; j++);  // Calibrated loop for ~1ms
    }
}

/******************************************************************************
 * Function: main
 * Description: Main program - displays binary counter on LEDs
 * Parameters: None
 * Returns: None
 ******************************************************************************/
void main() {
    unsigned char counter = 0;  // 8-bit counter variable (0-255)
    
    // Configure Port 1 as output, initialize to 0
    P1 = 0x00;
    
    // Infinite loop for continuous counting
    while(1) {
        P1 = counter;           // Display counter value on LEDs
        delay_ms(1000);         // Wait 1 second per count
        
        counter++;              // Increment counter
        
        // counter automatically wraps around from 255 to 0
        // due to 8-bit overflow (no explicit reset needed)
    }
}

/******************************************************************************
 * Alternative Version: With Visual Feedback
 ******************************************************************************/
#if 0  // Set to 1 to use this version instead

void main() {
    unsigned char counter = 0;
    
    P1 = 0x00;
    
    while(1) {
        // Display current count
        P1 = counter;
        delay_ms(800);
        
        // Blink all LEDs briefly to show transition
        P1 = 0xFF;
        delay_ms(100);
        P1 = 0x00;
        delay_ms(100);
        
        counter++;
    }
}

#endif

/******************************************************************************
 * Build Instructions:
 *   1. Open Keil uVision 5
 *   2. Create new project and select P89V51RD2 device
 *   3. Add this C file to Source Group 1
 *   4. Set crystal frequency: 11.0592 MHz in Target Options
 *   5. Build project (Ctrl+B) to generate HEX file
 *   6. Program P89V51RD2 using FlashMagic
 *
 * Expected Output:
 *   LEDs display binary counting pattern:
 *   0 (00000000) --> 1 (00000001) --> 2 (00000010) --> ... --> 255 (11111111) --> 0...
 *   Each count displayed for 1 second
 *
 * Verification:
 *   - Count 0: All LEDs OFF
 *   - Count 1: Only P1.0 (rightmost LED) ON
 *   - Count 128: Only P1.7 (leftmost LED) ON
 *   - Count 255: All LEDs ON
 *
 * Educational Value:
 *   - Demonstrates binary number representation
 *   - Shows bit-to-physical LED mapping
 *   - Illustrates 8-bit counter overflow behavior
 ******************************************************************************/
