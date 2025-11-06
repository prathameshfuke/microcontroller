#include <PIC18F4550.h>

// Define LCD control pins connected to PORTC
#define LCD_EN   PORTCbits.RC1   // Enable pin
#define LCD_RS   PORTCbits.RC0   // Register Select pin
#define LCDPORT  PORTB           // LCD data port (DB0-DB7)

// Simple delay function using nested loops
void LCD_delay(unsigned int time)
{
    unsigned int i, j;
    for(i = 0; i < time; i++)
    {
        for(j = 0; j < time; j++);
    }
}

// Send command byte to LCD
void LCD_cmd(unsigned char command)
{
    LCD_RS = 0;            // RS = 0 to select command register
    LCDPORT = command;     // Place command on data lines
    LCD_EN = 1;            // Generate high-to-low pulse on EN
    LCD_delay(10);         // Short delay for LCD to process
    LCD_EN = 0;
    LCD_delay(10);
}

// Send a data byte (character) to LCD to display
void LCD_write(unsigned char lcddata)
{
    LCD_RS = 1;            // RS = 1 to select data register
    LCDPORT = lcddata;     // Place data (character) on data lines
    LCD_EN = 1;            // Generate enable pulse
    LCD_delay(10);
    LCD_EN = 0;
    LCD_delay(10);
}

// Initialize LCD in 8-bit mode, 2 lines, 5x7 font
void LCD_init()
{
    LCD_cmd(0x38);   // Function set: 8-bit mode, 2 lines, 5x7 dots
    LCD_cmd(0x0C);   // Display ON, Cursor OFF
    LCD_cmd(0x06);   // Entry mode: increment cursor, no display shift
    LCD_cmd(0x01);   // Clear display
    LCD_delay(20);   // Wait for clear command to complete
}

// Display a null-terminated string on LCD
void LCD_write_string(const char *str)
{
    unsigned char i = 0;
    while(str[i] != '\0')
    {
        LCD_write(str[i]);
        i++;
    }
}

void main()
{
    // Configure PORTB and PORTC as output
    TRISB = 0x00;   // PORTB all pins output (LCD data)
    TRISC = 0x00;   // PORTC all pins output (LCD control)
    
    LCD_init();                         // Initialize LCD
    LCD_cmd(0x80);                      // Set cursor to Line 1, Position 0
    LCD_write_string("MMCOE");          // Display string on Line 1
    
    LCD_cmd(0xC0);                      // Set cursor to Line 2, Position 0
    LCD_write_string("Laboratory");     // Display string on Line 2
    
    while(1);  // Infinite loop to keep display
}

// ===== EASY MODIFICATION SECTION =====
// To change displayed text, modify these lines in main():
//   LCD_write_string("Your Text Here");  // Change text between quotes
//
// To change LCD line:
//   LCD_cmd(0x80);  // Line 1, position 0
//   LCD_cmd(0xC0);  // Line 2, position 0
//   LCD_cmd(0x85);  // Line 1, position 5
//   LCD_cmd(0xC5);  // Line 2, position 5
