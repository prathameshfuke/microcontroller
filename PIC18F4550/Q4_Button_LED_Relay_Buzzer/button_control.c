/******************************************************************************
 * PIC18F4550 Button, LED, Relay & Buzzer Interface
 * Experiment Q4: Multi-functional I/O Control System
 *
 * Author: Microcontroller Lab
 * Date: November 6, 2025
 * Target Device: PIC18F4550
 * IDE: MPLAB X IDE
 * Compiler: XC8
 * Kit: Microembedded PIC18F4550 Development Kit
 *
 * Description:
 *   This program implements a multi-functional control system with:
 *   - Button 1: Activates relay, buzzer, and left-to-right LED chase
 *   - Button 2: Deactivates relay, buzzer, and right-to-left LED chase
 *   - Default: Slow left-to-right LED chase when no button pressed
 *
 * Hardware Configuration (Microembedded Kit):
 *   Button 1 (Input):  RC0 with internal pull-up, active LOW
 *   Button 2 (Input):  RC1 with internal pull-up, active LOW
 *   LEDs (8-bit):      RB0-RB7 (outputs)
 *   Relay (Output):    RC2 (HIGH = ON, LOW = OFF)
 *   Buzzer (Output):   RC3 (HIGH = ON, LOW = OFF)
 *
 * Crystal Frequency: 8 MHz (Internal Oscillator)
 ******************************************************************************/

#include <xc.h>
#include <pic18f4550.h>

// Configuration Bits
#pragma config FOSC = INTOSCIO_EC   // Internal oscillator, EC on RA6
#pragma config WDTE = OFF           // Watchdog Timer disabled
#pragma config PWRTE = OFF          // Power-up Timer disabled
#pragma config BOREN = OFF          // Brown-out Reset disabled
#pragma config PBADEN = OFF         // PORTB pins as digital I/O
#pragma config LVP = OFF            // Low-Voltage Programming disabled
#pragma config MCLRE = OFF          // MCLR function disabled

// Pin Definitions using LAT registers (recommended for output)
#define Button1     PORTCbits.RC0   // Read input using PORT
#define Button2     PORTCbits.RC1
#define Relay       LATCbits.LATC2  // Write output using LAT
#define Buzzer      LATCbits.LATC3
#define LED_PORT    LATB            // All 8 LEDs on Port B

// LED Pattern Definitions
#define LED_ALL_ON  0xFF
#define LED_ALL_OFF 0x00

/******************************************************************************
 * Function: delay_ms
 * Description: Software delay in milliseconds
 * Parameters: ms - delay duration in milliseconds
 * Returns: None
 * Note: Calibrated for 8 MHz internal oscillator
 ******************************************************************************/
void delay_ms(unsigned int ms) {
    unsigned int i, j;
    for(i = 0; i < ms; i++) {
        for(j = 0; j < 200; j++);  // Calibrated for 8 MHz
    }
}

/******************************************************************************
 * Function: led_chase_left
 * Description: LED chase pattern from left to right (P1.0 to P1.7)
 * Parameters: speed_ms - delay between each LED step
 * Returns: None
 ******************************************************************************/
void led_chase_left(unsigned int speed_ms) {
    unsigned char i;
    for(i = 0; i < 8; i++) {
        LED_PORT = (1 << i);    // Light up LED at position i
        delay_ms(speed_ms);
    }
}

/******************************************************************************
 * Function: led_chase_right
 * Description: LED chase pattern from right to left (P1.7 to P1.0)
 * Parameters: speed_ms - delay between each LED step
 * Returns: None
 ******************************************************************************/
void led_chase_right(unsigned int speed_ms) {
    signed char i;
    for(i = 7; i >= 0; i--) {
        LED_PORT = (1 << i);    // Light up LED at position i
        delay_ms(speed_ms);
    }
}

/******************************************************************************
 * Function: button_debounce
 * Description: Simple button debouncing delay
 * Parameters: None
 * Returns: None
 ******************************************************************************/
void button_debounce(void) {
    delay_ms(20);  // 20ms debounce delay
}

/******************************************************************************
 * Function: system_init
 * Description: Initialize all I/O ports and peripherals
 * Parameters: None
 * Returns: None
 ******************************************************************************/
void system_init(void) {
    // Configure internal oscillator to 8 MHz
    OSCCONbits.IRCF2 = 1;  // 8 MHz internal oscillator
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF0 = 1;
    OSCCONbits.SCS1 = 1;   // Select internal oscillator
    OSCCONbits.SCS0 = 0;
    
    // Configure Port B as output (LEDs)
    TRISB = 0x00;           // All Port B pins as output
    LATB = LED_ALL_OFF;     // Initialize all LEDs OFF
    
    // Configure Port C mixed input/output
    TRISCbits.TRISC0 = 1;   // RC0 (Button 1) as input
    TRISCbits.TRISC1 = 1;   // RC1 (Button 2) as input
    TRISCbits.TRISC2 = 0;   // RC2 (Relay) as output
    TRISCbits.TRISC3 = 0;   // RC3 (Buzzer) as output
    
    // Initialize outputs to OFF state
    Relay = 0;
    Buzzer = 0;
    
    // Enable internal pull-ups on Port C (optional, if not external pull-ups)
    // INTCON2bits.RBPU = 0;  // Enable weak pull-ups (if needed)
}

/******************************************************************************
 * Function: main
 * Description: Main program loop with button polling and control logic
 * Parameters: None
 * Returns: None
 ******************************************************************************/
void main(void) {
    // Initialize system
    system_init();
    
    // Main infinite loop
    while(1) {
        // Check Button 1 (active LOW)
        if(Button1 == 0) {
            button_debounce();      // Wait for debounce
            
            if(Button1 == 0) {      // Confirm button still pressed
                // Button 1 Actions:
                Relay = 1;          // Turn ON relay
                Buzzer = 1;         // Turn ON buzzer
                led_chase_left(100); // Fast LED chase left
            }
        }
        // Check Button 2 (active LOW)
        else if(Button2 == 0) {
            button_debounce();      // Wait for debounce
            
            if(Button2 == 0) {      // Confirm button still pressed
                // Button 2 Actions:
                Relay = 0;          // Turn OFF relay
                Buzzer = 0;         // Turn OFF buzzer
                led_chase_right(100); // Fast LED chase right
            }
        }
        // Default behavior when no button pressed
        else {
            Relay = 0;              // Ensure relay is OFF
            Buzzer = 0;             // Ensure buzzer is OFF
            led_chase_left(200);    // Slow LED chase left
        }
    }
}

/******************************************************************************
 * Build Instructions:
 *   1. Open MPLAB X IDE
 *   2. Create new project: File → New Project
 *   3. Select Device: PIC18F4550
 *   4. Select Compiler: XC8
 *   5. Add this C file to Source Files
 *   6. Build project: Production → Build Main Project
 *   7. Program using PICkit 3/4 or ICD3
 *
 * Expected Behavior:
 *   - Button 1 Pressed:
 *     * Relay clicks ON
 *     * Buzzer sounds continuously
 *     * LEDs chase from left to right (fast)
 *   
 *   - Button 2 Pressed:
 *     * Relay clicks OFF
 *     * Buzzer stops
 *     * LEDs chase from right to left (fast)
 *   
 *   - No Button Pressed:
 *     * Relay OFF
 *     * Buzzer OFF
 *     * LEDs chase from left to right (slow)
 *
 * Troubleshooting:
 *   - Buttons not responding: Check pull-up resistors (10kΩ to VCC)
 *   - LEDs not lighting: Check TRISB configuration and connections
 *   - Relay not clicking: Check transistor driver circuit (BC547 + 1N4007)
 *   - Buzzer not sounding: Check transistor driver and buzzer polarity
 *   - Erratic behavior: Add decoupling capacitors (100nF) near VCC/GND pins
 ******************************************************************************/
