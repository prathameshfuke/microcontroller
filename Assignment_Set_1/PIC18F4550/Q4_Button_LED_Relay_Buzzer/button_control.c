/******************************************************************************
 * PIC18F4550 Button Control with LED Chase, Relay and Buzzer
 * 
 * Button 1: Turn ON relay & buzzer, LEDs chase left
 * Button 2: Turn OFF relay & buzzer, LEDs chase right
 * 
 * Hardware:
 * - LED Array: PORTB (RB0-RB7)
 * - Button 1: RC0 (active low)
 * - Button 2: RC1 (active low)
 * - Relay: RC2
 * - Buzzer: RC3
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

// Pin definitions
#define BTN1        PORTCbits.RC0
#define BTN2        PORTCbits.RC1
#define RELAY       LATCbits.LATC2
#define BUZZER      LATCbits.LATC3
#define LED_PORT    LATB

void main(void) {
    // Configure ports
    TRISB = 0x00;    // PORTB as output (LEDs)
    TRISC = 0x03;    // RC0,RC1 input (buttons), RC2,RC3 output
    
    unsigned char led_pattern = 0x01;
    
    while(1) {
        if(!BTN1) {  // Button 1 pressed (active low)
            RELAY = 1;
            BUZZER = 1;
            // Chase left
            LED_PORT = led_pattern;
            led_pattern = (led_pattern << 1) | (led_pattern >> 7);
            __delay_ms(100);  // Fast chase
        }
        else if(!BTN2) {  // Button 2 pressed
            RELAY = 0;
            BUZZER = 0;
            // Chase right
            LED_PORT = led_pattern;
            led_pattern = (led_pattern >> 1) | (led_pattern << 7);
            __delay_ms(100);  // Fast chase
        }
        else {  // No button pressed
            RELAY = 0;
            BUZZER = 0;
            // Slow left chase
            LED_PORT = led_pattern;
            led_pattern = (led_pattern << 1) | (led_pattern >> 7);
            __delay_ms(500);  // Slow chase
        }
    }
}