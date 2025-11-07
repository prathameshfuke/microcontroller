/******************************************************************************
 * PIC18F4550 UART Communication
 * 
 * Bidirectional serial communication at 9600 baud
 * Echoes received characters back to PC
 * Responds to commands: LED_ON, LED_OFF, STATUS
 * 
 * Hardware:
 * - TX: RC6
 * - RX: RC7
 * - Status LED: RB0
 ******************************************************************************/

#include <xc.h>
#include <pic18f4550.h>
#include <string.h>

// Configuration Bits
#pragma config FOSC = INTOSCIO_EC
#pragma config WDTE = OFF
#pragma config PWRTE = OFF
#pragma config BOREN = OFF
#pragma config PBADEN = OFF
#pragma config LVP = OFF
#pragma config MCLRE = OFF

void init_uart(void) {
    // Configure UART for 9600 baud @ 8MHz
    TRISC |= 0x80;    // RC7/RX as input
    TRISC &= ~0x40;   // RC6/TX as output
    
    TXSTA = 0x24;     // Enable TX, async mode, high speed
    RCSTA = 0x90;     // Enable UART, continuous RX
    BAUDCON = 0x00;   // Default settings
    SPBRG = 12;       // 9600 baud @ 8MHz
}

void uart_send(unsigned char c) {
    while(!TXSTAbits.TRMT);  // Wait for transmit buffer empty
    TXREG = c;
}

void uart_string(const char *str) {
    while(*str) uart_send(*str++);
}

unsigned char uart_receive(void) {
    while(!PIR1bits.RCIF);   // Wait for data
    return RCREG;
}

void main(void) {
    char cmd[10];
    unsigned char i = 0;
    
    init_uart();
    TRISB &= ~0x01;  // RB0 as output (status LED)
    
    uart_string("PIC18F4550 UART Demo\r\n");
    uart_string("Commands: LED_ON, LED_OFF, STATUS\r\n");
    
    while(1) {
        char c = uart_receive();
        uart_send(c);  // Echo back
        
        if(c == '\r' || c == '\n') {  // Command complete
            cmd[i] = '\0';
            if(strcmp(cmd, "LED_ON") == 0) {
                LATBbits.LATB0 = 1;
                uart_string("\r\nLED ON\r\n");
            }
            else if(strcmp(cmd, "LED_OFF") == 0) {
                LATBbits.LATB0 = 0;
                uart_string("\r\nLED OFF\r\n");
            }
            else if(strcmp(cmd, "STATUS") == 0) {
                uart_string("\r\nLED is ");
                uart_string(LATBbits.LATB0 ? "ON\r\n" : "OFF\r\n");
            }
            i = 0;
        }
        else if(i < 9) {  // Store command character
            cmd[i++] = c;
        }
    }
}