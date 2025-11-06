/******************************************************************************
 * P89V51RD2 DAC Interface - Sawtooth Wave Generation
 * Experiment Q3 Part (ii): Generate Sawtooth Wave using DAC0808
 *
 * Author: Microcontroller Lab
 * Date: November 6, 2025
 * Target Device: P89V51RD2 (8051 compatible)
 * IDE: Keil uVision 5
 * Compiler: C51
 *
 * Description:
 *   This program generates a sawtooth wave by linearly ramping the voltage
 *   from 0V to 5V using an external 8-bit DAC (DAC0808) connected to Port 0.
 *   The waveform can be observed on an oscilloscope.
 *
 * Hardware Connection:
 *   P0.0-P0.7 (8-bit data) --> DAC0808 Digital Inputs (D0-D7)
 *   DAC0808 Output Pin --> Oscilloscope/CRO Input
 *   DAC0808 VREF+ --> +5V
 *   DAC0808 VREF- --> GND
 *   Output capacitor: 100nF for smoothing
 *
 * Wave Parameters:
 *   Frequency: ~1 kHz
 *   Amplitude: 0V to 5V (rail-to-rail)
 *   Type: Ramp-up (Sawtooth)
 *
 * Crystal Frequency: 11.0592 MHz
 ******************************************************************************/

#include <reg51.h>      // Standard 8051 register definitions

// Define Port 0 for DAC interface
#define DAC_PORT P0

/******************************************************************************
 * Function: delay_us
 * Description: Microsecond delay for wave step timing
 * Parameters: us - delay in microseconds
 * Returns: None
 ******************************************************************************/
void delay_us(unsigned int us) {
    unsigned int i;
    for(i = 0; i < us; i++) {
        _nop_();
        _nop_();
    }
}

/******************************************************************************
 * Function: main
 * Description: Generates sawtooth wave on DAC output
 * Parameters: None
 * Returns: None
 ******************************************************************************/
void main() {
    unsigned char sawtooth_value = 0;  // Holds current DAC output value
    
    // Initialize DAC port to 0V
    DAC_PORT = 0x00;
    
    // Infinite loop to generate sawtooth wave
    while(1) {
        // Ramp up from 0x00 (0V) to 0xFF (5V)
        for(sawtooth_value = 0; sawtooth_value < 255; sawtooth_value++) {
            DAC_PORT = sawtooth_value;  // Output current step
            delay_us(4);                // Small delay for ~1 kHz frequency
                                        // 256 steps × 4µs ≈ 1.024ms ≈ 977 Hz
        }
        
        // Output final value 0xFF
        DAC_PORT = 0xFF;
        delay_us(4);
        
        // Quick reset to 0V for next cycle (creates sharp fall)
        DAC_PORT = 0x00;
        delay_us(10);  // Brief pause before next ramp
    }
}

/******************************************************************************
 * Alternative Version: Adjustable Frequency Sawtooth
 ******************************************************************************/
#if 0  // Set to 1 to use this version

void generate_sawtooth(unsigned int frequency_hz) {
    unsigned char i;
    unsigned int step_delay_us;
    
    // Calculate delay per step to achieve desired frequency
    // Period = 1,000,000µs / frequency
    // Step delay = Period / 256 steps
    step_delay_us = (1000000 / frequency_hz) / 256;
    
    while(1) {
        for(i = 0; i < 255; i++) {
            DAC_PORT = i;
            delay_us(step_delay_us);
        }
        DAC_PORT = 0xFF;
        delay_us(step_delay_us);
        DAC_PORT = 0x00;
    }
}

void main() {
    DAC_PORT = 0x00;
    generate_sawtooth(1000);  // Generate 1 kHz sawtooth wave
}

#endif

/******************************************************************************
 * Alternative Version: Triangular Wave
 ******************************************************************************/
#if 0  // Set to 1 to generate triangular wave instead

void main() {
    unsigned char value = 0;
    bit direction = 0;  // 0 = ramp up, 1 = ramp down
    
    DAC_PORT = 0x00;
    
    while(1) {
        DAC_PORT = value;
        delay_us(4);
        
        if(direction == 0) {
            value++;
            if(value == 255) {
                direction = 1;  // Switch to ramp down
            }
        } else {
            value--;
            if(value == 0) {
                direction = 0;  // Switch to ramp up
            }
        }
    }
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
 *   - Waveform: Sawtooth (linear ramp up, sharp fall)
 *   - Frequency: ~1 kHz (adjustable)
 *   - Peak-to-Peak Voltage: 0V to 5V
 *   - Rise Time: ~1ms (256 steps)
 *   - Fall Time: ~10µs (instant reset)
 *
 * Frequency Calculation:
 *   Steps per cycle: 256
 *   Delay per step: 4µs
 *   Period = 256 × 4µs + reset time ≈ 1.024ms + 10µs ≈ 1.034ms
 *   Frequency = 1 / 0.001034s ≈ 967 Hz
 *
 * Troubleshooting:
 *   - Jagged waveform: Reduce step delay for smoother ramp
 *   - Wrong frequency: Adjust delay_us() value
 *   - No sharp fall: Check DAC speed (may need faster DAC)
 *   - Noisy signal: Add capacitor at output for smoothing
 ******************************************************************************/
