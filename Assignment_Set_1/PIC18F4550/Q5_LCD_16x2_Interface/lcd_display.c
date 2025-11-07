/******************************************************************************
 * PIC18F4550 LCD Display
 * 
 * Display "MMCOE" on LCD first line
 * Display "Laboratory" on second line
 * 
 * Hardware:
 * LCD in 4-bit mode:
 * - RS: RA1
 * - EN: RA3
 * - D4-D7: RB4-RB7
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

// LCD Control Pins
#define LCD_RS      LATAbits.LATA1
#define LCD_EN      LATAbits.LATA3
#define LCD_DATA    LATB

void init_lcd(void) {
    // Configure ports
    TRISA &= ~0x0A;  // RA1,RA3 as outputs
    TRISB &= 0x0F;   // RB4-RB7 as outputs
    
    __delay_ms(20);  // Power-up delay
    
    // Initialize LCD in 4-bit mode
    LCD_RS = 0;
    LCD_DATA = 0x30;
    LCD_EN = 1; __delay_ms(1); LCD_EN = 0;
    __delay_ms(5);
    
    LCD_DATA = 0x20;  // 4-bit mode
    LCD_EN = 1; __delay_ms(1); LCD_EN = 0;
    __delay_ms(5);
    
    lcd_cmd(0x28);   // 4-bit, 2 lines
    lcd_cmd(0x0C);   // Display ON, cursor OFF
    lcd_cmd(0x06);   // Auto increment
    lcd_cmd(0x01);   // Clear display
}

void lcd_cmd(unsigned char cmd) {
    LCD_RS = 0;
    LCD_DATA = (LCD_DATA & 0x0F) | (cmd & 0xF0);
    LCD_EN = 1; __delay_ms(1); LCD_EN = 0;
    __delay_ms(1);
    LCD_DATA = (LCD_DATA & 0x0F) | ((cmd << 4) & 0xF0);
    LCD_EN = 1; __delay_ms(1); LCD_EN = 0;
    __delay_ms(2);
}

void lcd_char(unsigned char data) {
    LCD_RS = 1;
    LCD_DATA = (LCD_DATA & 0x0F) | (data & 0xF0);
    LCD_EN = 1; __delay_ms(1); LCD_EN = 0;
    __delay_ms(1);
    LCD_DATA = (LCD_DATA & 0x0F) | ((data << 4) & 0xF0);
    LCD_EN = 1; __delay_ms(1); LCD_EN = 0;
    __delay_ms(2);
}

void lcd_string(const char *str) {
    while(*str) lcd_char(*str++);
}

void main(void) {
    init_lcd();
    
    lcd_cmd(LCD_LINE1);
    lcd_string("MMCOE");
    
    lcd_cmd(LCD_LINE2);
    lcd_string("Laboratory");
    
    while(1);  // Display stays on
}