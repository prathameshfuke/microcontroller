/******************************************************************************
 * P89V51RD2 DAC Interface - Square Wave Generation
 * Experiment Q3 Part (i): Generate Square Wave using DAC0808
 *
 * Author: Microcontroller Lab
 * Date: November 6, 2025
 * Target Device: P89V51RD2 (8051 compatible)
 * IDE: Keil uVision 5
 * Compiler: C51
 *
 * Description:
 *   This program generates a square wave by alternating between 0V (0x00)
 *   and 5V (0xFF) output using an external 8-bit DAC (DAC0808) connected
 *   to Port 0. The waveform can be observed on an oscilloscope.
 *
 * Hardware Connection:
 *   P0.0-P0.7 (8-bit data) --> DAC0808 Digital Inputs (D0-D7)
 *   DAC0808 Output Pin --> Oscilloscope/CRO Input
 *   DAC0808 VREF+ --> +5V
 *   DAC0808 VREF- --> GND
 *   Output capacitor: 100nF for smoothing (optional)
 *
 * Wave Parameters:
 *   Frequency: ~1 kHz (adjustable via delay)
 *   Amplitude: 0V to 5V (rail-to-rail)
 *   Duty Cycle: 50% (symmetric square wave)
 *
 * Crystal Frequency: 11.0592 MHz
 ******************************************************************************/

#include <reg51.h>      // Standard 8051 register definitions

// Define Port 0 for DAC interface
#define DAC_PORT P0

/******************************************************************************
 * Function: delay_us
 * Description: Microsecond delay for precise wave timing
 * Parameters: us - delay in microseconds
 * Returns: None
 * Note: Calibrated for 11.0592 MHz crystal
 ******************************************************************************/
void delay_us(unsigned int us) {
    unsigned int i;
    for(i = 0; i < us; i++) {
        _nop_();        // No operation (assembly NOP instruction)
        _nop_();
    }
}

/******************************************************************************
 * Function: delay_ms
 * Description: Millisecond delay function
 * Parameters: ms - delay in milliseconds
 * Returns: None
 ******************************************************************************/
void delay_ms(unsigned int ms) {
    unsigned int i, j;
    for(i = 0; i < ms; i++) {
        for(j = 0; j < 123; j++);
    }
}

/******************************************************************************
 * Function: main
 * Description: Generates square wave on DAC output
 * Parameters: None
 * Returns: None
 ******************************************************************************/
void main() {
    // Initialize DAC port to 0V
    DAC_PORT = 0x00;
    
    // Infinite loop to generate square wave
    while(1) {
        DAC_PORT = 0xFF;        // Output HIGH (5V)
        delay_us(500);          // High for 500 microseconds
        
        DAC_PORT = 0x00;        // Output LOW (0V)
        delay_us(500);          // Low for 500 microseconds
        
        // This creates approximately 1 kHz square wave
        // Period = 500µs + 500µs = 1000µs = 1ms
        // Frequency = 1/0.001s = 1 kHz
    }
}

/******************************************************************************
 * Alternative Version: Variable Frequency Square Wave
 ******************************************************************************/
#if 0  // Set to 1 to use this version

void generate_square_wave(unsigned int frequency_hz) {
    unsigned int half_period_us;
    
    // Calculate half period in microseconds
    // Half period = 1,000,000 / (2 × frequency)
    half_period_us = 500000 / frequency_hz;
    
    while(1) {
        DAC_PORT = 0xFF;
        delay_us(half_period_us);
        DAC_PORT = 0x00;
        delay_us(half_period_us);
    }
}

void main() {
    DAC_PORT = 0x00;
    generate_square_wave(1000);  // Generate 1 kHz square wave
}

#endif

/******************************************************************************
 * Build Instructions:
 *   1. Open Keil uVision 5
 *   2. Create new project and select P89V51RD2 device
 *   3. Add this C file to Source Group 1
 *   4. Set crystal frequency: 11.0592 MHz in Target Options
 *   5. Build project (Ctrl+B) to generate HEX file
 *   6. Program P89V51RD2 using FlashMagic
 *   7. Connect oscilloscope to DAC output
 *
 * Expected Output on Oscilloscope:
 *   - Waveform: Square wave
 *   - Frequency: ~1 kHz (may vary slightly based on delay calibration)
 *   - Peak-to-Peak Voltage: 0V to 5V
 *   - Duty Cycle: 50%
 *
 * DAC0808 Pin Configuration:
 *   Pin 5-12: D0-D7 (Digital inputs from P0.0-P0.7)
 *   Pin 4: Output (Connect to oscilloscope)
 *   Pin 14: VREF+ (+5V reference)
 *   Pin 15: VREF- (GND)
 *   Pin 3: VCC (+5V supply)
 *   Pin 13: GND
 *
 * Troubleshooting:
 *   - No output: Check DAC power supply and connections
 *   - Wrong frequency: Adjust delay_us() calibration
 *   - Noisy signal: Add 100nF capacitor at DAC output
 *   - Unstable output: Ensure proper grounding
 ******************************************************************************/
