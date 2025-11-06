#include <xc.h>  // Include device-specific header file for PIC microcontrollers

#define _XTAL_FREQ 48000000  // Define system clock frequency (for delay macros if needed)

/**
 * Function to transmit a single byte via UART
 */
void Txbyte(char data) {
    while(TXSTAbits.TRMT == 0);  // Wait until the transmit shift register is empty
    TXREG = data;                // Load the data byte into the transmit register
}

void main(void) {
    unsigned char i = 0;
    
    // Strings to send via UART
    const char string[] = "\n\r Press any key\n\r";
    const char string1[] = "\n\r UART Tested \n\r";
    
    // -------------- UART Pin Configuration ---------------
    TRISB = 0x00;  // Set PORTB as output (used for displaying received data)
    TRISC = 0x80;  // Set RC7 (RX) as input, RC6 (TX) as output
    
    // -------------- UART Initialization ------------------
    TXSTA = 0x20;  // 8-bit transmission, transmitter enabled, asynchronous mode
    RCSTA = 0x90;  // Serial port enabled, continuous reception, 8-bit reception
    SPBRG = 0x19;  // Baud rate = 9600 (for 48 MHz clock: SPBRG = (Fosc/(64*Baud))-1)
    
    // Send initial message via UART
    for(i = 0; string[i] != '\0'; i++) {
        Txbyte(string[i]);
    }
    
    while(1) {
        // Check if data is received
        if(RCSTAbits.FERR == 1) {
            // Framing error occurred, read and discard RCREG
            i = RCREG;
        }
        else if(PIR1bits.RCIF == 1) {
            // Data received successfully
            i = RCREG;         // Read received data
            PORTB = i;         // Display on PORTB
            Txbyte(i);         // Echo back the received data
            
            // Send confirmation message
            for(i = 0; string1[i] != '\0'; i++) {
                Txbyte(string1[i]);
            }
        }
    }
}

// REMOVED OLD VERBOSE FUNCTIONS
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
