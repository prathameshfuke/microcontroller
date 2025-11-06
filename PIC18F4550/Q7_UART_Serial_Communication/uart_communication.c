/******************************************************************************
 * PIC18F4550 UART Serial Communication with PC
 * Experiment Q7: Bi-directional Serial Data Transfer via UART
 *
 * Author: Microcontroller Lab
 * Date: November 6, 2025
 * Target Device: PIC18F4550
 * IDE: MPLAB X IDE
 * Compiler: XC8
 *
 * Description:
 *   This program establishes bi-directional serial communication between
 *   PIC18F4550 and PC using UART. Sends startup message to PC and echoes
 *   back any received data. Can be used to control LEDs or other peripherals
 *   via serial commands.
 *
 * Hardware Configuration:
 *   UART TX: RC6 --> USB-to-Serial RX (CP2102/CH340/FT232)
 *   UART RX: RC7 --> USB-to-Serial TX
 *   GND: PIC GND --> USB-to-Serial GND
 *
 * Communication Parameters:
 *   Baud Rate: 9600 bps
 *   Data Bits: 8
 *   Stop Bits: 1
 *   Parity: None
 *   Flow Control: None
 *
 * PC Software: Tera Term (recommended), PuTTY, Arduino Serial Monitor, or MPLAB Data Visualizer
 *
 * Crystal Frequency: 8 MHz (Internal Oscillator)
 ******************************************************************************/

#include <xc.h>
#include <pic18f4550.h>
#include <string.h>

// Configuration Bits
#pragma config FOSC = INTOSCIO_EC   // Internal oscillator
#pragma config WDTE = OFF           // Watchdog Timer disabled
#pragma config PWRTE = OFF          // Power-up Timer disabled
#pragma config BOREN = OFF          // Brown-out Reset disabled
#pragma config PBADEN = OFF         // PORTB pins as digital I/O
#pragma config LVP = OFF            // Low-Voltage Programming disabled
#pragma config MCLRE = OFF          // MCLR function disabled

// UART Configuration Constants
#define BAUD_RATE       9600
#define FOSC            8000000L    // 8 MHz internal oscillator
#define SPBRG_VALUE     ((FOSC/(16L*BAUD_RATE))-1)  // Baud rate calculation

// Buffer for received data
#define BUFFER_SIZE     64
volatile char rx_buffer[BUFFER_SIZE];
volatile unsigned char rx_index = 0;
volatile unsigned char data_received = 0;

/******************************************************************************
 * Function: delay_ms
 * Description: Software delay in milliseconds
 * Parameters: ms - delay duration
 * Returns: None
 ******************************************************************************/
void delay_ms(unsigned int ms) {
    unsigned int i, j;
    for(i = 0; i < ms; i++) {
        for(j = 0; j < 200; j++);
    }
}

/******************************************************************************
 * Function: uart_init
 * Description: Initialize UART module for 9600 baud communication
 * Parameters: None
 * Returns: None
 ******************************************************************************/
void uart_init(void) {
    // Configure UART pins (RC6=TX, RC7=RX)
    TRISCbits.TRISC6 = 0;   // TX as output
    TRISCbits.TRISC7 = 1;   // RX as input
    
    // Configure UART Transmit
    TXSTAbits.TXEN = 1;     // Enable transmit
    TXSTAbits.SYNC = 0;     // Asynchronous mode
    TXSTAbits.BRGH = 1;     // High baud rate mode
    
    // Configure UART Receive
    RCSTAbits.SPEN = 1;     // Enable serial port
    RCSTAbits.CREN = 1;     // Enable continuous receive
    
    // Set baud rate (9600 bps)
    SPBRG = SPBRG_VALUE;    // Load baud rate generator
    
    // Configure interrupts for receive
    PIR1bits.RCIF = 0;      // Clear receive interrupt flag
    PIE1bits.RCIE = 1;      // Enable receive interrupt
    INTCONbits.PEIE = 1;    // Enable peripheral interrupts
    INTCONbits.GIE = 1;     // Enable global interrupts
}

/******************************************************************************
 * Function: uart_send_byte
 * Description: Transmit a single byte via UART
 * Parameters: data - byte to transmit
 * Returns: None
 ******************************************************************************/
void uart_send_byte(unsigned char data) {
    while(!TXSTAbits.TRMT); // Wait until transmit shift register is empty
    TXREG = data;           // Load data into transmit register
}

/******************************************************************************
 * Function: uart_send_string
 * Description: Transmit a null-terminated string via UART
 * Parameters: str - pointer to string
 * Returns: None
 ******************************************************************************/
void uart_send_string(const char *str) {
    while(*str) {
        uart_send_byte(*str++);
    }
}

/******************************************************************************
 * Function: uart_send_number
 * Description: Transmit an integer as ASCII string
 * Parameters: num - integer to transmit
 * Returns: None
 ******************************************************************************/
void uart_send_number(unsigned int num) {
    char buffer[10];
    unsigned char i = 0;
    
    // Convert number to string
    if(num == 0) {
        uart_send_byte('0');
        return;
    }
    
    while(num > 0) {
        buffer[i++] = (num % 10) + '0';
        num /= 10;
    }
    
    // Send digits in reverse order
    while(i > 0) {
        uart_send_byte(buffer[--i]);
    }
}

/******************************************************************************
 * Function: process_command
 * Description: Process received serial commands
 * Parameters: cmd - command string
 * Returns: None
 ******************************************************************************/
void process_command(char *cmd) {
    // Example commands
    if(strcmp(cmd, "LED_ON") == 0) {
        LATBbits.LATB0 = 1;
        uart_send_string("LED turned ON\r\n");
    }
    else if(strcmp(cmd, "LED_OFF") == 0) {
        LATBbits.LATB0 = 0;
        uart_send_string("LED turned OFF\r\n");
    }
    else if(strcmp(cmd, "STATUS") == 0) {
        uart_send_string("System: OK\r\n");
        uart_send_string("Device: PIC18F4550\r\n");
    }
    else {
        uart_send_string("Unknown command: ");
        uart_send_string(cmd);
        uart_send_string("\r\n");
    }
}

/******************************************************************************
 * Function: __interrupt() high_priority ISR
 * Description: High-priority interrupt service routine for UART receive
 * Parameters: None
 * Returns: None
 ******************************************************************************/
void __interrupt(high_priority) ISR(void) {
    // Check if UART receive interrupt occurred
    if(PIR1bits.RCIF) {
        char received_byte = RCREG;     // Read received data
        
        // Echo back received character
        uart_send_byte(received_byte);
        
        // Store in buffer if not full
        if(rx_index < (BUFFER_SIZE - 1)) {
            // Check for command terminator (Enter key)
            if(received_byte == '\r' || received_byte == '\n') {
                rx_buffer[rx_index] = '\0';  // Null-terminate string
                data_received = 1;           // Set flag
                rx_index = 0;                // Reset index
            } else {
                rx_buffer[rx_index++] = received_byte;
            }
        } else {
            // Buffer overflow, reset
            rx_index = 0;
        }
        
        // Clear interrupt flag
        PIR1bits.RCIF = 0;
        
        // Check for overrun error
        if(RCSTAbits.OERR) {
            RCSTAbits.CREN = 0;     // Clear overrun by disabling receiver
            RCSTAbits.CREN = 1;     // Re-enable receiver
        }
    }
}

/******************************************************************************
 * Function: system_init
 * Description: Initialize oscillator and I/O ports
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
    
    // Configure Port B for LED (optional)
    TRISBbits.TRISB0 = 0;   // RB0 as output
    LATBbits.LATB0 = 0;     // Initialize LED OFF
}

/******************************************************************************
 * Function: main
 * Description: Main program with serial communication
 * Parameters: None
 * Returns: None
 ******************************************************************************/
void main(void) {
    // Initialize system
    system_init();
    
    // Initialize UART
    uart_init();
    
    // Send startup message
    delay_ms(100);
    uart_send_string("\r\n=============================\r\n");
    uart_send_string("PIC18F4550 UART Ready\r\n");
    uart_send_string("Microcontroller Lab Experiment\r\n");
    uart_send_string("=============================\r\n");
    uart_send_string("Commands:\r\n");
    uart_send_string("  LED_ON  - Turn on LED\r\n");
    uart_send_string("  LED_OFF - Turn off LED\r\n");
    uart_send_string("  STATUS  - Check system status\r\n");
    uart_send_string("=============================\r\n\r\n");
    uart_send_string("Enter command: ");
    
    // Main loop
    while(1) {
        // Check if command received
        if(data_received) {
            data_received = 0;
            
            // Send newline for formatting
            uart_send_string("\r\n");
            
            // Process command
            process_command((char *)rx_buffer);
            
            // Prompt for next command
            uart_send_string("\r\nEnter command: ");
        }
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
 * Testing Instructions:
 *   1. Connect USB-to-Serial adapter:
 *      - Adapter TX --> PIC RC7 (RX)
 *      - Adapter RX --> PIC RC6 (TX)
 *      - Adapter GND --> PIC GND
 *   2. Open terminal software (PuTTY/Tera Term)
 *   3. Configure: 9600 baud, 8-N-1
 *   4. Power on PIC - should see startup message
 *   5. Type commands and press Enter
 *
 * Expected Output:
 *   - Startup message displays on terminal
 *   - Characters typed are echoed back
 *   - Commands execute (LED_ON, LED_OFF, STATUS)
 *   - Responses sent back to PC
 *
 * Troubleshooting:
 *   - No output: Check TX/RX connections (swap if needed)
 *   - Garbage characters: Verify baud rate (9600)
 *   - No echo: Check receive interrupt configuration
 *   - Commands not working: Check strcmp() and string termination
 ******************************************************************************/
