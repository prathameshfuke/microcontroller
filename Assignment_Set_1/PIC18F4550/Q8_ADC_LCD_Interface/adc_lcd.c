/******************************************************************************
 * PIC18F4550 ADC with LCD Display
 * 
 * ADC Calculation:
 * Resolution = 10-bit (0-1023)
 * Voltage Range = 0-5V
 * Step Size = 5V/1024 = 0.00488V = 4.88mV per step
 * Digital Value = (Vin × 1023) / 5V
 * 
 * Sample Calculations:
 * Vin (V) | Digital | Hex  | Binary
 * ---------|---------|------|--------
 * 0.00V    |    0    | 0x000| 0000000000
 * 1.00V    |  204    | 0x0CC| 0011001100
 * 2.50V    |  511    | 0x1FF| 0111111111
 * 3.30V    |  675    | 0x2A3| 1010100011
 * 5.00V    | 1023    | 0x3FF| 1111111111
 ******************************************************************************/

#include <xc.h>
#include <pic18f4550.h>

// Configuration Bits
#pragma config FOSC = INTOSCIO_EC
#pragma config WDTE = OFF
#pragma config PWRTE = OFF
#pragma config BOREN = OFF
#pragma config PBADEN = OFF
#pragma config LVP = OFF
#pragma config MCLRE = OFF

// LCD Commands
#define LCD_CLEAR   0x01
#define LCD_HOME    0x02
#define LCD_LINE1   0x80
#define LCD_LINE2   0xC0

// Function prototypes
void init_adc(void);
void init_lcd(void);
void lcd_cmd(unsigned char);
void lcd_data(unsigned char);
void lcd_string(const char *);
unsigned int read_adc(void);

void main(void) {
    unsigned int adc_value;
    float voltage;
    char lcd_buffer[16];
    
    // Initialize ADC and LCD
    init_adc();
    init_lcd();
    
    while(1) {
        // Read ADC and convert to voltage
        adc_value = read_adc();
        voltage = (adc_value * 5.0) / 1023.0;
        
        // Display on LCD
        lcd_cmd(LCD_LINE1);
        sprintf(lcd_buffer, "ADC: %4d", adc_value);
        lcd_string(lcd_buffer);
        
        lcd_cmd(LCD_LINE2);
        sprintf(lcd_buffer, "Volt: %1.2fV", voltage);
        lcd_string(lcd_buffer);
        
        __delay_ms(500);  // Update every 500ms
    }
}

void init_adc(void) {
    ADCON0 = 0x01;    // ADC ON, Channel 0
    ADCON1 = 0x0E;    // AN0 analog, rest digital
    ADCON2 = 0x92;    // Right justify, 4Tad, Fosc/32
}

unsigned int read_adc(void) {
    ADCON0bits.GO = 1;           // Start conversion
    while(ADCON0bits.GO);        // Wait for completion
    return (ADRESH << 8) + ADRESL;
}