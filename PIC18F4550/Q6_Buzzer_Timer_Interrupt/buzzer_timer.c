/******************************************************************************
 * PIC18F4550 Buzzer Control with Timer1 Interrupt
 * Experiment Q6: Generate Buzzer Sound using Timer-based Interrupts
 *
 * Author: Microcontroller Lab
 * Date: November 6, 2025
 * Target Device: PIC18F4550
 * IDE: MPLAB X IDE
 * Compiler: XC8
 * Kit: Microembedded PIC18F4550 Development Kit
 *
 * Description:
 *   This program generates a buzzing sound by toggling a buzzer pin using
 *   Timer1 interrupt. The buzzer frequency is controlled by Timer1 overflow
 *   rate. Demonstrates ON for 2 seconds, OFF for 2 seconds pattern.
 *
 * Hardware Configuration:
 *   Buzzer Output: RC3 connected via transistor driver (BC547)
 *   - RC3 --> 10kΩ resistor --> Base of BC547 NPN
 *   - Buzzer (+) --> +5V
 *   - Buzzer (-) --> Collector of BC547
 *   - Emitter of BC547 --> GND
 *   - 1N4007 diode across buzzer (cathode to +5V) for back-EMF protection
 *
 * Timer1 Configuration:
 *   Clock Source: Fosc/4 = 8 MHz / 4 = 2 MHz
 *   Prescaler: 1:8
 *   Timer Frequency: 2 MHz / 8 = 250 kHz
 *   Overflow Period: (65536 - preload) / 250 kHz
 *   For ~1 ms interrupt: Preload = 65536 - 250 = 65286 (0xFF06)
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

// Buzzer Pin Definition
#define Buzzer LATCbits.LATC3       // Buzzer on RC3

// Timer1 Preload Values
// For ~1 ms interrupt at 8 MHz with 1:8 prescaler
#define TMR1_HIGH   0xFF            // High byte of preload
#define TMR1_LOW    0x06            // Low byte of preload

// Buzzer control variables
volatile unsigned int interrupt_count = 0;
volatile bit buzzer_enable = 1;     // Control buzzer ON/OFF

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
 * Function: timer1_init
 * Description: Initialize Timer1 for interrupt generation
 * Parameters: None
 * Returns: None
 ******************************************************************************/
void timer1_init(void) {
    // Configure Timer1
    T1CONbits.TMR1CS = 0;   // Clock source: Fosc/4 (internal)
    T1CONbits.T1CKPS1 = 1;  // Prescaler 1:8
    T1CONbits.T1CKPS0 = 1;
    T1CONbits.T1OSCEN = 0;  // Timer1 oscillator disabled
    T1CONbits.RD16 = 1;     // 16-bit read/write mode
    T1CONbits.TMR1ON = 0;   // Timer1 OFF initially
    
    // Load Timer1 preload value
    TMR1H = TMR1_HIGH;
    TMR1L = TMR1_LOW;
    
    // Configure Timer1 Interrupt
    PIR1bits.TMR1IF = 0;    // Clear Timer1 interrupt flag
    PIE1bits.TMR1IE = 1;    // Enable Timer1 interrupt
    INTCONbits.PEIE = 1;    // Enable peripheral interrupts
    INTCONbits.GIE = 1;     // Enable global interrupts
    
    // Start Timer1
    T1CONbits.TMR1ON = 1;
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

/******************************************************************************
 * Alternative Version: Variable Frequency Buzzer
 ******************************************************************************/
#if 0  // Set to 1 to use this version

// Different frequency preload values
#define FREQ_LOW_H      0xFC    // Lower frequency (longer period)
#define FREQ_LOW_L      0x00
#define FREQ_HIGH_H     0xFF    // Higher frequency (shorter period)
#define FREQ_HIGH_L     0x00

void main(void) {
    system_init();
    timer1_init();
    
    while(1) {
        // High frequency tone for 1 second
        buzzer_enable = 1;
        TMR1H = FREQ_HIGH_H;
        TMR1L = FREQ_HIGH_L;
        delay_ms(1000);
        
        // Low frequency tone for 1 second
        TMR1H = FREQ_LOW_H;
        TMR1L = FREQ_LOW_L;
        delay_ms(1000);
        
        // Buzzer OFF for 1 second
        buzzer_enable = 0;
        Buzzer = 0;
        delay_ms(1000);
    }
}

#endif

/******************************************************************************
 * Build Instructions:
 *   1. Open MPLAB X IDE
 *   2. Create new project for PIC18F4550
 *   3. Select XC8 compiler
 *   4. Add this C file to Source Files
 *   5. Build project: Production → Build Main Project
 *   6. Program using PICkit programmer
 *
 * Expected Behavior:
 *   - Buzzer sounds for 2 seconds (continuous tone)
 *   - Buzzer silent for 2 seconds
 *   - Pattern repeats indefinitely
 *
 * Frequency Calculation:
 *   Timer Frequency = Fosc / (4 × Prescaler) = 8 MHz / (4 × 8) = 250 kHz
 *   Interrupt Period = (65536 - Preload) / 250 kHz
 *   Preload = 65286 (0xFF06) → Period = 250 / 250 kHz = 1 ms
 *   Buzzer Toggle Rate = 1 ms → Buzzer Frequency = 500 Hz (square wave)
 *
 * Adjusting Buzzer Frequency:
 *   - Increase preload value → Higher frequency (shorter period)
 *   - Decrease preload value → Lower frequency (longer period)
 *   - For 1 kHz: Toggle every 0.5 ms → Preload = 65536 - 125 = 65411
 *
 * Troubleshooting:
 *   - No sound: Check transistor driver circuit and buzzer polarity
 *   - Very faint sound: Increase transistor base resistor (try 1kΩ)
 *   - Continuous sound when should be OFF: Check buzzer_enable logic
 *   - Frequency wrong: Adjust TMR1 preload values
 *   - Erratic behavior: Add 100nF capacitor across buzzer terminals
 ******************************************************************************/
