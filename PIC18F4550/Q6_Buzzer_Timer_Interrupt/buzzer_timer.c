#include <pic18f4550.h>

// Define Buzzer as RA5 output pin
#define Buzzer LATAbits.LATA5

// Global count variable to count timer overflows
unsigned int count = 0;

// Interrupt Service Routine (ISR) for handling interrupts
void __interrupt() Timer1_ISR(void)
{
    // Check if Timer1 overflow interrupt flag is set
    if(TMR1IF == 1)
    {
        // Reload Timer1 registers with initial count values
        // This preload value determines timer overflow interval (~12 ms delay)
        TMR1L = 0x1B;
        TMR1H = 0xD1;
        
        count++; // Increment overflow count
        
        // Toggle buzzer output every 1000 overflows (~12 ms * 1000 = ~12 seconds)
        if(count >= 1000)
        {
            Buzzer = ~Buzzer; // Toggle buzzer pin
            count = 0;        // Reset overflow count
        }
        
        // Clear Timer1 interrupt flag to allow next interrupt
        TMR1IF = 0;
    }
}

void main()
{
    TRISAbits.TRISA5 = 0;  // Configure RA5 as output pin for buzzer
    // Buzzer = 0;         // (Commented out) Initialize buzzer pin to LOW

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
    
    // Configure Port C
    TRISCbits.TRISC3 = 0;   // RC3 (Buzzer) as output
    Buzzer = 0;             // Initialize buzzer OFF
}

/******************************************************************************
 * Function: __interrupt() high_priority ISR
 * Description: High-priority interrupt service routine for Timer1
 * Parameters: None
 * Returns: None
 ******************************************************************************/
void __interrupt(high_priority) ISR(void) {
    // Check if Timer1 interrupt occurred
    if(PIR1bits.TMR1IF) {
        PIR1bits.TMR1IF = 0;        // Clear interrupt flag
        
        // Reload Timer1 for next interrupt
        TMR1H = TMR1_HIGH;
        TMR1L = TMR1_LOW;
        
        // Toggle buzzer if enabled
        if(buzzer_enable) {
            Buzzer = ~Buzzer;       // Toggle buzzer pin
        } else {
            Buzzer = 0;             // Keep buzzer OFF
        }
        
        interrupt_count++;          // Increment interrupt counter
    }
}

/******************************************************************************
 * Function: main
 * Description: Main program with buzzer ON/OFF pattern
 * Parameters: None
 * Returns: None
 ******************************************************************************/
void main(void) {
    // Initialize system
    system_init();
    
    // Initialize Timer1 and interrupts
    timer1_init();
    
    // Main loop: Buzzer ON for 2 sec, OFF for 2 sec
    while(1) {
        // Buzzer ON for 2 seconds
        buzzer_enable = 1;      // Enable buzzer toggling in ISR
        delay_ms(2000);         // Wait 2 seconds
        
        // Buzzer OFF for 2 seconds
        buzzer_enable = 0;      // Disable buzzer toggling
        Buzzer = 0;             // Ensure buzzer is OFF
        delay_ms(2000);         // Wait 2 seconds
    }
}

// OUTPUT:
// Buzzer ON for approximately 1 second
// Buzzer OFF for approximately 1 second
// This ON/OFF cycle repeats continuously, creating a 1 Hz square wave with a 50% duty cycle
