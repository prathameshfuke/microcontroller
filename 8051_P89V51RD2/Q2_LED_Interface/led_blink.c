/******************************************************************************
 * P89V51RD2 LED Blinking Program - Embedded C
 * Experiment Q2 Part (i): LED Blinking on Port 1
 *
 * Author: Microcontroller Lab
 * Date: November 6, 2025
 * Target Device: P89V51RD2 (8051 compatible)
 * IDE: Keil uVision 5
 * Compiler: C51
 *
 * Description:
 *   This program blinks all 8 LEDs connected to Port 1 with 500ms intervals.
 *   LEDs turn ON for 500ms, then OFF for 500ms continuously.
 *
 * Hardware Connection:
 *   Port 1 (P1.0 - P1.7) --> 330Ω Resistor --> LED Anode --> LED Cathode --> GND
 *   P89V51RD2 can supply up to 16mA on Port 1 pins (direct drive capable)
 *
 * Crystal Frequency: 11.0592 MHz
 ******************************************************************************/

#include <reg51.h>      // Standard 8051 register definitions

/******************************************************************************
 * Function: delay_ms
 * Description: Software delay function for approximately 1 millisecond
 * Parameters: ms - delay duration in milliseconds
 * Returns: None
 * Note: Delay calibrated for 11.0592 MHz crystal frequency
 ******************************************************************************/
void delay_ms(unsigned int ms) {
    unsigned int i, j;
    for(i = 0; i < ms; i++) {
        for(j = 0; j < 123; j++);  // Inner loop calibrated for 11.0592 MHz
    }
}

/******************************************************************************
 * Function: main
 * Description: Main program loop - blinks all LEDs on Port 1
 * Parameters: None
 * Returns: None
 ******************************************************************************/
void main() {
    // Configure Port 1 as output (all pins set to 0 initially)
    P1 = 0x00;
    
    // Infinite loop for continuous blinking
    while(1) {
        P1 = 0xFF;          // Turn ON all LEDs (binary: 11111111)
        delay_ms(500);      // Keep LEDs ON for 500 milliseconds
        
        P1 = 0x00;          // Turn OFF all LEDs (binary: 00000000)
        delay_ms(500);      // Keep LEDs OFF for 500 milliseconds
    }
}

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
 *   All 8 LEDs on Port 1 blink synchronously with 500ms ON/OFF intervals
 *
 * Troubleshooting:
 *   - If LEDs don't light: Check polarity (anode to resistor, cathode to GND)
 *   - If LEDs are dim: Reduce resistor value to 220Ω
 *   - If blinking too fast/slow: Adjust delay_ms() inner loop constant
 ******************************************************************************/
