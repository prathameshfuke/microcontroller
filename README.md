# Microcontroller Lab Programs

This repository contains lab programs for 8051 and PIC18F4550 microcontrollers.

## Repository Structure

```
├── 8051_P89V51RD2/           # 8051 Family Programs (Keil uVision)
│   ├── Q1_Block_Data_Transfer/    # Assembly: Block Data Transfer
│   ├── Q2_LED_Interface/          # C: LED Blinking & Patterns
│   └── Q3_DAC_Interface/          # C: DAC Waveform Generation
│
└── PIC18F4550/              # PIC Programs (MPLAB X + XC8)
    ├── Q4_Button_LED_Relay_Buzzer/  # Input/Output Control
    ├── Q5_LCD_16x2_Interface/       # LCD Display
    ├── Q6_Buzzer_Timer_Interrupt/   # Timer Interrupts
    ├── Q7_UART_Serial_Communication/ # UART/Serial Comms
    └── Q8_ADC_LCD_Interface/        # ADC with LCD Display
```

## Development Setup

### 8051 Programs (Q1-Q3)
- IDE: Keil uVision 5
- Device: P89V51RD2 (or compatible 8051)
- Build: See individual project READMEs
- Programming: Use Flash Magic for downloading

### PIC18F4550 Programs (Q4-Q8)
- IDE: MPLAB X
- Compiler: XC8
- Device: PIC18F4550
- Programming: PICkit/PICloader

See individual folders for detailed instructions, wiring diagrams, and code documentation.