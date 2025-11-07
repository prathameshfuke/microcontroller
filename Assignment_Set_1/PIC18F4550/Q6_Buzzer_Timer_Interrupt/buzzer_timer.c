/******************************************************************************
 * PIC18F4550 Timer Interrupt Buzzer Control
 * 
 * Uses Timer0 interrupt to generate buzzer tone
 * Buzzer ON for 2 seconds, OFF for 2 seconds
 * 
 * Hardware:
 * - Buzzer: RC3
 * - Timer0 generates interrupt every 1ms
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

// Timer period = 1ms
#define TMR0_LOAD_VAL (65536 - 2000)

unsigned int ms_count = 0;
unsigned char buzzer_enable = 0;

void __interrupt() isr(void) {
    if(INTCONbits.TMR0IF) {
        TMR0H = (TMR0_LOAD_VAL >> 8);
        TMR0L = TMR0_LOAD_VAL & 0xFF;
        
        if(buzzer_enable)
            LATCbits.LATC3 = !LATCbits.LATC3;  // Toggle buzzer
            
        ms_count++;
        if(ms_count >= 2000) {  // 2 seconds
            ms_count = 0;
            buzzer_enable = !buzzer_enable;  // Toggle enable
        }
        
        INTCONbits.TMR0IF = 0;  // Clear flag
    }
}

void init_timer0(void) {
    T0CON = 0x88;    // 16-bit mode, prescaler 1:1
    TMR0H = (TMR0_LOAD_VAL >> 8);
    TMR0L = TMR0_LOAD_VAL & 0xFF;
    
    INTCONbits.TMR0IE = 1;  // Enable Timer0 interrupt
    INTCONbits.GIE = 1;     // Enable global interrupts
}

void main(void) {
    TRISC &= ~0x08;  // RC3 as output (buzzer)
    LATCbits.LATC3 = 0;  // Buzzer off initially
    
    init_timer0();
    
    while(1);  // Everything handled in interrupt
}