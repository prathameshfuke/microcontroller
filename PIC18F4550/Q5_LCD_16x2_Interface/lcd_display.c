/******************************************************************************
 * PIC18F4550 LCD 16x2 Interface - 4-Bit Mode
 * Experiment Q5: Display Text on Character LCD
 *
 * Author: Microcontroller Lab
 * Date: November 6, 2025
 * Target Device: PIC18F4550
 * IDE: MPLAB X IDE
 * Compiler: XC8
 * Kit: Microembedded PIC18F4550 Development Kit (16x2 LCD integrated)
 *
 * Description:
 *   This program interfaces with a 16x2 character LCD in 4-bit mode to
 *   display text. Shows "MMCOE" on line 1 and "Laboratory" on line 2.
 *   Uses efficient 4-bit communication to save I/O pins.
 *
 * Hardware Configuration (4-bit Mode):
 *   LCD RS (Register Select):  RA0
 *   LCD RW (Read/Write):        GND (write-only mode)
 *   LCD EN (Enable):            RA2
 *   LCD D4-D7 (Data lines):     RB4-RB7
 *   LCD VSS:                    GND
 *   LCD VDD:                    +5V
 *   LCD VEE (Contrast):         10kΩ POT between VDD and GND
 *   LCD Backlight (A/K):        +5V/GND (with 100Ω resistor)
 *
 * Crystal Frequency: 8 MHz (Internal Oscillator)
 ******************************************************************************/

#include <xc.h>
#include <pic18f4550.h>

// Configuration Bits
#pragma config FOSC = INTOSCIO_EC   // Internal oscillator
#pragma config WDTE = OFF           // Watchdog Timer disabled
#pragma config PWRTE = OFF          // Power-up Timer disabled
#pragma config BOREN = OFF          // Brown-out Reset disabled
#pragma config PBADEN = OFF         // PORTB pins as digital I/O
#pragma config LVP = OFF            // Low-Voltage Programming disabled
#pragma config MCLRE = OFF          // MCLR function disabled

// LCD Pin Definitions
#define LCD_RS      LATAbits.LATA0  // Register Select (0=Command, 1=Data)
#define LCD_EN      LATAbits.LATA2  // Enable pulse (High-to-Low triggers)
#define LCD_DATA    LATB            // Data lines connected to RB4-RB7

// LCD Commands
#define LCD_CLEAR           0x01    // Clear display
#define LCD_HOME            0x02    // Return home
#define LCD_ENTRY_MODE      0x06    // Entry mode: increment cursor, no shift
#define LCD_DISPLAY_ON      0x0C    // Display ON, Cursor OFF, Blink OFF
#define LCD_DISPLAY_OFF     0x08    // Display OFF
#define LCD_CURSOR_ON       0x0E    // Display ON, Cursor ON, Blink OFF
#define LCD_FUNCTION_SET    0x28    // 4-bit mode, 2 lines, 5x7 font
#define LCD_LINE1           0x80    // DDRAM address for line 1, column 0
#define LCD_LINE2           0xC0    // DDRAM address for line 2, column 0

/******************************************************************************
 * Function: delay_ms
 * Description: Software delay in milliseconds
 * Parameters: ms - delay duration
 * Returns: None
 ******************************************************************************/
void delay_ms(unsigned int ms) {
    unsigned int i, j;
    for(i = 0; i < ms; i++) {
        for(j = 0; j < 200; j++);  // Calibrated for 8 MHz
    }
}

/******************************************************************************
 * Function: lcd_send_nibble
 * Description: Send 4-bit nibble to LCD data lines (upper 4 bits of Port B)
 * Parameters: nibble - 4-bit data to send (lower 4 bits used)
 * Returns: None
 ******************************************************************************/
void lcd_send_nibble(unsigned char nibble) {
    // Clear upper 4 bits of Port B, set nibble data
    LCD_DATA = (LCD_DATA & 0x0F) | (nibble << 4);
    
    // Generate Enable pulse (High-to-Low transition)
    LCD_EN = 1;
    delay_ms(1);        // Enable pulse width
    LCD_EN = 0;
    delay_ms(1);        // Time for LCD to process
}

/******************************************************************************
 * Function: lcd_send_cmd
 * Description: Send command to LCD (RS=0)
 * Parameters: cmd - 8-bit command byte
 * Returns: None
 ******************************************************************************/
void lcd_send_cmd(unsigned char cmd) {
    LCD_RS = 0;                     // Select command register
    lcd_send_nibble(cmd >> 4);      // Send upper nibble
    lcd_send_nibble(cmd & 0x0F);    // Send lower nibble
    delay_ms(2);                    // Command execution time
}

/******************************************************************************
 * Function: lcd_send_data
 * Description: Send data (character) to LCD (RS=1)
 * Parameters: data - 8-bit ASCII character
 * Returns: None
 ******************************************************************************/
void lcd_send_data(unsigned char data) {
    LCD_RS = 1;                     // Select data register
    lcd_send_nibble(data >> 4);     // Send upper nibble
    lcd_send_nibble(data & 0x0F);   // Send lower nibble
    delay_ms(2);                    // Data write time
}

/******************************************************************************
 * Function: lcd_init
 * Description: Initialize LCD in 4-bit mode with proper initialization sequence
 * Parameters: None
 * Returns: None
 ******************************************************************************/
void lcd_init(void) {
    // Configure Port A for RS and EN
    TRISA = 0x00;           // Port A as output
    LATA = 0x00;            // Initialize to LOW
    
    // Configure Port B upper nibble for data (RB4-RB7 as output)
    TRISB = 0x0F;           // RB4-RB7 output, RB0-RB3 input (optional)
    LATB = 0x00;            // Initialize to LOW
    
    // Wait for LCD power-on initialization (>15ms required)
    delay_ms(20);
    
    // LCD Initialization Sequence for 4-bit mode
    lcd_send_nibble(0x3);   // Function set: 8-bit mode (initial)
    delay_ms(5);
    lcd_send_nibble(0x3);   // Repeat
    delay_ms(1);
    lcd_send_nibble(0x3);   // Repeat
    delay_ms(1);
    lcd_send_nibble(0x2);   // Switch to 4-bit mode
    delay_ms(1);
    
    // Now in 4-bit mode, send full commands
    lcd_send_cmd(LCD_FUNCTION_SET);     // 4-bit, 2 lines, 5x7 font
    lcd_send_cmd(LCD_DISPLAY_OFF);      // Display OFF
    lcd_send_cmd(LCD_CLEAR);            // Clear display
    delay_ms(2);
    lcd_send_cmd(LCD_ENTRY_MODE);       // Entry mode: increment, no shift
    lcd_send_cmd(LCD_DISPLAY_ON);       // Display ON, cursor OFF
}

/******************************************************************************
 * Function: lcd_print
 * Description: Print a string to LCD at current cursor position
 * Parameters: str - pointer to null-terminated string
 * Returns: None
 ******************************************************************************/
void lcd_print(const char *str) {
    while(*str) {
        lcd_send_data(*str++);  // Send each character
    }
}

/******************************************************************************
 * Function: lcd_goto
 * Description: Move cursor to specific line and column
 * Parameters: row - line number (1 or 2)
 *             col - column number (0 to 15)
 * Returns: None
 ******************************************************************************/
void lcd_goto(unsigned char row, unsigned char col) {
    unsigned char address;
    
    if(row == 1) {
        address = LCD_LINE1 + col;  // Line 1: 0x80 to 0x8F
    } else {
        address = LCD_LINE2 + col;  // Line 2: 0xC0 to 0xCF
    }
    
    lcd_send_cmd(address);
}

/******************************************************************************
 * Function: system_init
 * Description: Initialize oscillator
 * Parameters: None
 * Returns: None
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
 * Description: Main program - display text on LCD
 * Parameters: None
 * Returns: None
 ******************************************************************************/
void main(void) {
    // Initialize system
    system_init();
    
    // Initialize LCD
    lcd_init();
    
    // Display text on Line 1
    lcd_goto(1, 0);             // Go to Line 1, Column 0
    lcd_print("MMCOE");         // Display "MMCOE"
    
    // Display text on Line 2
    lcd_goto(2, 0);             // Go to Line 2, Column 0
    lcd_print("Laboratory");    // Display "Laboratory"
    
    // Main loop (keep display static)
    while(1) {
        // Display remains static
        // Can add dynamic updates here if needed
    }
}

/******************************************************************************
 * Build Instructions:
 *   1. Open MPLAB X IDE
 *   2. Create new project for PIC18F4550
 *   3. Select XC8 compiler
 *   4. Add this C file to Source Files
 *   5. Build project: Production → Build Main Project
 *   6. Program using PICkit programmer
 *
 * Expected Output on LCD:
 *   Line 1: MMCOE
 *   Line 2: Laboratory
 *
 * LCD Contrast Adjustment:
 *   - Rotate potentiometer on VEE pin
 *   - If characters not visible: adjust contrast
 *   - Optimal: characters clearly visible, not too dark
 *
 * Troubleshooting:
 *   - No display: Check power supply (VDD=+5V, VSS=GND)
 *   - Garbled text: Verify data line connections (RB4-RB7 to D4-D7)
 *   - Blank screen with backlight: Adjust contrast potentiometer
 *   - Incorrect characters: Check RS and EN connections
 *   - Initialization fails: Increase delay in lcd_init()
 ******************************************************************************/
