/******************************************************************************
 * PIC18F4550 Internal ADC with LCD Display
 * Experiment Q8: Analog-to-Digital Conversion with 16x2 LCD Output
 *
 * Author: Microcontroller Lab
 * Date: November 6, 2025
 * Target Device: PIC18F4550
 * IDE: MPLAB X IDE
 * Compiler: XC8
 * Kit: Microembedded PIC18F4550 Development Kit
 *
 * Description:
 *   This program reads analog voltage (0-5V) from AN0 (RA0) using internal
 *   10-bit ADC and displays both analog voltage and digital value on 16x2 LCD.
 *   Updates every 500ms. Uses potentiometer to vary input voltage.
 *
 * Hardware Configuration:
 *   ADC Input:          AN0 (RA0) - Connect 10kΩ potentiometer
 *   LCD RS:             RA1
 *   LCD EN:             RA3
 *   LCD D4-D7:          RB4-RB7
 *   Potentiometer:      One end to GND, other to +5V, wiper to RA0
 *
 * ADC Configuration:
 *   Resolution: 10-bit (0-1023)
 *   Reference: VSS to VDD (0V to 5V)
 *   Clock: Fosc/64
 *   Channel: AN0 (RA0)
 *
 * Display Format:
 *   Line 1: "Analog: X.XXV"
 *   Line 2: "Digital: XXXX"
 *
 * Crystal Frequency: 8 MHz (Internal Oscillator)
 ******************************************************************************/

#include <xc.h>
#include <pic18f4550.h>
#include <stdio.h>
#include <stdlib.h>

// Configuration Bits
#pragma config FOSC = INTOSCIO_EC   // Internal oscillator
#pragma config WDTE = OFF           // Watchdog Timer disabled
#pragma config PWRTE = OFF          // Power-up Timer disabled
#pragma config BOREN = OFF          // Brown-out Reset disabled
#pragma config PBADEN = OFF         // PORTB pins as digital I/O
#pragma config LVP = OFF            // Low-Voltage Programming disabled
#pragma config MCLRE = OFF          // MCLR function disabled

// LCD Pin Definitions
#define LCD_RS      LATAbits.LATA1  // Register Select
#define LCD_EN      LATAbits.LATA3  // Enable
#define LCD_DATA    LATB            // Data on RB4-RB7

// LCD Commands
#define LCD_CLEAR       0x01
#define LCD_HOME        0x02
#define LCD_LINE1       0x80
#define LCD_LINE2       0xC0

/******************************************************************************
 * Function: delay_ms
 * Description: Software delay in milliseconds
 ******************************************************************************/
void delay_ms(unsigned int ms) {
    unsigned int i, j;
    for(i = 0; i < ms; i++) {
        for(j = 0; j < 200; j++);
    }
}

/******************************************************************************
 * LCD Functions (4-bit mode)
 ******************************************************************************/
void lcd_send_nibble(unsigned char nibble) {
    LCD_DATA = (LCD_DATA & 0x0F) | (nibble << 4);
    LCD_EN = 1;
    delay_ms(1);
    LCD_EN = 0;
    delay_ms(1);
}

void lcd_send_cmd(unsigned char cmd) {
    LCD_RS = 0;
    lcd_send_nibble(cmd >> 4);
    lcd_send_nibble(cmd & 0x0F);
    delay_ms(2);
}

void lcd_send_data(unsigned char data) {
    LCD_RS = 1;
    lcd_send_nibble(data >> 4);
    lcd_send_nibble(data & 0x0F);
    delay_ms(2);
}

void lcd_init(void) {
    // Configure Port A for RS and EN
    TRISA = 0x01;           // RA0 as input (ADC), RA1 & RA3 as output (LCD)
    LATA = 0x00;
    
    // Configure Port B upper nibble for data
    TRISB = 0x0F;
    LATB = 0x00;
    
    // Wait for LCD power-on
    delay_ms(20);
    
    // Initialization sequence
    lcd_send_nibble(0x3);
    delay_ms(5);
    lcd_send_nibble(0x3);
    delay_ms(1);
    lcd_send_nibble(0x3);
    delay_ms(1);
    lcd_send_nibble(0x2);   // 4-bit mode
    delay_ms(1);
    
    lcd_send_cmd(0x28);     // 4-bit, 2 lines, 5x7 font
    lcd_send_cmd(0x08);     // Display OFF
    lcd_send_cmd(0x01);     // Clear display
    delay_ms(2);
    lcd_send_cmd(0x06);     // Entry mode
    lcd_send_cmd(0x0C);     // Display ON, cursor OFF
}

void lcd_print(const char *str) {
    while(*str) {
        lcd_send_data(*str++);
    }
}

void lcd_goto(unsigned char row, unsigned char col) {
    unsigned char address;
    if(row == 1) {
        address = LCD_LINE1 + col;
    } else {
        address = LCD_LINE2 + col;
    }
    lcd_send_cmd(address);
}

/******************************************************************************
 * Function: adc_init
 * Description: Initialize ADC module
 ******************************************************************************/
void adc_init(void) {
    // Configure RA0 as analog input
    TRISAbits.TRISA0 = 1;   // RA0 as input
    
    // Configure ADC
    ADCON0bits.CHS = 0b0000;    // Select AN0 channel
    ADCON0bits.ADON = 1;        // Enable ADC module
    
    // ADCON1: Configure analog pins and voltage reference
    // PCFG = 1110: AN0 as analog, rest digital; VREF+ = VDD, VREF- = VSS
    ADCON1 = 0x0E;
    
    // ADCON2: Configure ADC timing and result format
    // ADFM = 1 (right justified), ACQT = 110 (16 TAD), ADCS = 110 (Fosc/64)
    ADCON2bits.ADFM = 1;        // Right justified result
    ADCON2bits.ACQT2 = 1;       // Acquisition time = 16 TAD
    ADCON2bits.ACQT1 = 1;
    ADCON2bits.ACQT0 = 0;
    ADCON2bits.ADCS2 = 1;       // Conversion clock = Fosc/64
    ADCON2bits.ADCS1 = 1;
    ADCON2bits.ADCS0 = 0;
}

/******************************************************************************
 * Function: adc_read
 * Description: Read 10-bit ADC value
 * Returns: 10-bit ADC result (0-1023)
 ******************************************************************************/
unsigned int adc_read(void) {
    ADCON0bits.GO = 1;          // Start conversion
    while(ADCON0bits.GO);       // Wait for completion
    return ((ADRESH << 8) | ADRESL);  // Return 10-bit result
}

/******************************************************************************
 * Function: float_to_string
 * Description: Convert float to string with 2 decimal places
 * Parameters: value - float value, buffer - output string buffer
 ******************************************************************************/
void float_to_string(float value, char *buffer) {
    int integer_part = (int)value;
    int decimal_part = (int)((value - integer_part) * 100);
    
    if(decimal_part < 0) decimal_part = -decimal_part;
    
    sprintf(buffer, "%d.%02d", integer_part, decimal_part);
}

/******************************************************************************
 * Function: display_adc
 * Description: Read ADC, calculate voltage, and display on LCD
 ******************************************************************************/
void display_adc(void) {
    unsigned int adc_value;
    float voltage;
    char buffer[16];
    
    // Read ADC value
    adc_value = adc_read();
    
    // Calculate voltage (10-bit ADC, 0-1023 maps to 0-5V)
    voltage = (float)adc_value * (5.0 / 1023.0);
    
    // Display on LCD Line 1: "Analog: X.XXV"
    lcd_goto(1, 0);
    lcd_print("Analog: ");
    float_to_string(voltage, buffer);
    lcd_print(buffer);
    lcd_print("V  ");  // Extra spaces to clear previous longer values
    
    // Display on LCD Line 2: "Digital: XXXX"
    lcd_goto(2, 0);
    lcd_print("Digital: ");
    sprintf(buffer, "%4d", adc_value);
    lcd_print(buffer);
    lcd_print("    ");
}

/******************************************************************************
 * Function: system_init
 * Description: Initialize oscillator
 ******************************************************************************/
void system_init(void) {
    // Configure internal oscillator to 8 MHz
    OSCCONbits.IRCF2 = 1;
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF0 = 1;
    OSCCONbits.SCS1 = 1;
    OSCCONbits.SCS0 = 0;
}

/******************************************************************************
 * Function: main
 * Description: Main program
 ******************************************************************************/
void main(void) {
    // Initialize system
    system_init();
    
    // Initialize LCD
    lcd_init();
    
    // Initialize ADC
    adc_init();
    
    // Display title briefly
    lcd_goto(1, 0);
    lcd_print(" ADC  Reading  ");
    lcd_goto(2, 0);
    lcd_print("  PIC18F4550   ");
    delay_ms(2000);
    
    // Clear display
    lcd_send_cmd(LCD_CLEAR);
    
    // Main loop - continuous ADC reading and display
    while(1) {
        display_adc();      // Read ADC and update display
        delay_ms(500);      // Update every 500ms
    }
}

/******************************************************************************
 * Example ADC Readings:
 *
 *   Potentiometer Position    |  Voltage  |  ADC Value
 *   -------------------------------------------------------
 *   Fully CCW (GND)           |   0.00V   |      0
 *   25% rotation              |   1.25V   |    256
 *   50% rotation (middle)     |   2.50V   |    512
 *   75% rotation              |   3.75V   |    768
 *   Fully CW (+5V)            |   5.00V   |   1023
 *
 ******************************************************************************/

/******************************************************************************
 * Build Instructions:
 *   1. Open MPLAB X IDE
 *   2. Create new project for PIC18F4550
 *   3. Select XC8 compiler
 *   4. Add this C file to Source Files
 *   5. Build project: Production → Build Main Project
 *   6. Program using PICkit programmer
 *
 * Hardware Setup:
 *   1. Connect 10kΩ potentiometer:
 *      - One end to GND
 *      - Other end to +5V
 *      - Wiper (middle) to RA0 (AN0)
 *   2. Connect LCD as per Q5 configuration
 *   3. Optional: Add 100nF capacitor at RA0 for noise filtering
 *
 * Testing:
 *   1. Power on the circuit
 *   2. Rotate potentiometer slowly
 *   3. Observe LCD displaying changing voltage and digital value
 *   4. Verify values match expected conversion
 *
 * Expected Output:
 *   LCD Line 1: "Analog: 2.50V" (updates with pot rotation)
 *   LCD Line 2: "Digital: 512"  (updates with pot rotation)
 *
 * Troubleshooting:
 *   - Constant 0V reading: Check RA0 connection and ADCON1 configuration
 *   - Constant 5V reading: Check potentiometer connections
 *   - Erratic readings: Add capacitor (100nF) from RA0 to GND
 *   - No LCD display: Check LCD connections (refer to Q5)
 *   - Wrong voltage calculation: Verify VREF+ = VDD = 5V
 ******************************************************************************/
