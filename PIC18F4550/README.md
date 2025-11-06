# PIC18F4550 Microembedded Kit Practicals
## MPLAB X IDE + XC8 + PICLoader Workflow

---

## 📋 Table of Contents
1. [Kit Overview](#kit-overview)
2. [Software Setup](#software-setup)
3. [Programming Workflow (PICLoader)](#programming-workflow-picloader)
4. [Experiment Checklist](#experiment-checklist)
5. [Lab Verification Guide](#lab-verification-guide)
6. [Troubleshooting](#troubleshooting)

---

## 🔧 Kit Overview

| Feature | Microembedded PIC18F4550 Trainer |
|---------|-----------------------------------|
| MCU | PIC18F4550 (USB-capable, 8-bit) |
| Clock | 8 MHz internal oscillator (OSCCON = 0x72) |
| Integrated I/O | 8 LEDs (PORTB), Relay (RC2), Buzzer (RC3), Buttons (RC0/RC1) |
| Display | 16x2 LCD (4-bit mode, RS=RA0/RA1, EN=RA2/RA3, D4-D7=RB4-RB7) |
| Connectivity | UART header (RC6/RC7), USB-B for power/debug |
| Programmer | PICLoader standalone utility (HEX upload via USB) |

🔌 **Power:** Plug the supplied USB cable into the kit. PICLoader handles Vdd during programming.

🧰 **No external wiring required** for the provided experiments—everything is on-board. Just hook up jumpers if you want to break signals out to the CRO/logic analyzer.

---

## 🖥️ Software Setup

1. **MPLAB X IDE** (latest) with **XC8 compiler**
   - Used to build the `.hex` files.
2. **PICLoader**
   - Vendor utility to load the HEX into the Microembedded kit.
   - Install the driver package bundled with the kit if Windows does not auto-detect the board.
3. **Tera Term (recommended terminal)**
   - Serial monitor for the UART practical.
   - Configure `9600 baud, 8 data bits, no parity, 1 stop bit`.

Optional helpers:
- MPLAB X IPE (Integrated Programming Environment) if you ever switch to PICkit 3/4.
- Logic analyzer/oscilloscope for waveform verification (DAC or timer outputs if you wire them out).

---

## 🔄 Programming Workflow (PICLoader)

1. **Build HEX in MPLAB X**
   - Project → Clean and Build (Ctrl+F11).
   - HEX file path: `dist/default/production/<project>.hex`.
2. **Launch PICLoader**
   - Select target `PIC18F4550`.
   - Browse to the generated HEX.
3. **Connect the kit via USB**
   - Hold the boot/program button if your kit requires it (most recent boards auto-enter bootloader).
4. **Click `Program`**
   - Wait for status `Programming successful`.
   - Board auto-resets into run mode.
5. **Disconnect and reconnect** only if the COM port changes in Windows Device Manager.

💡 *Tip:* Keep one MPLAB X project per experiment; the supplied source files are ready to drop into `main.c`.

---

## ✅ Experiment Checklist

| Exp | File | What to Observe | Kit Interaction |
|-----|------|-----------------|-----------------|
| **Q4** | `Q4_Button_LED_Relay_Buzzer/button_control.c` | Button1 → relay/buzzer ON + left chase; Button2 → relay/buzzer OFF + right chase; idle → slow left chase | Press on-board tactile switches S1 (RC0) & S2 (RC1) |
| **Q5** | `Q5_LCD_16x2_Interface/lcd_display.c` | LCD line1 = `MMCOE`, line2 = `Laboratory` | LCD auto-initialises; adjust contrast pot if text is faint |
| **Q6** | `Q6_Buzzer_Timer_Interrupt/buzzer_timer.c` | Buzzer toggles (≈500 Hz) for 2 s ON / 2 s OFF | Listen for tone; LED D9 often tied to buzzer transistor (visual cue) |
| **Q7** | `Q7_UART_Serial_Communication/uart_communication.c` | Tera Term shows banner, echoes keystrokes, handles `LED_ON`, `LED_OFF`, `STATUS` | Connect USB-to-UART header to PC (RC6→RX, RC7→TX, GND shared) |
| **Q8** | `Q8_ADC_LCD_Interface/adc_lcd.c` | LCD line1 `Analog: X.XXV`, line2 `Digital: XXXX` updates every 500 ms | Rotate on-board potentiometer linked to AN0 |

🛠️ **Configuration Bits:** Declared at the top of each source file. No extra `config.h` is required—just compile as-is.

---

## 🔍 Lab Verification Guide

### Q4 – Button/LED/Relay/Buzzer
- Default state: relay OFF, buzzer muted, LEDs chase left with slow delay.
- Hold Button1 (RC0, active-low): relay clicks, buzzer sounds, LEDs chase left quickly.
- Hold Button2 (RC1, active-low): relay off, buzzer off, LEDs chase right quickly.
- Debounce handled in software (`delay_ms(20)`).

### Q5 – LCD Display
- Upon reset, the LCD initialises in 4-bit mode.
- If the screen is blank, tweak the on-board contrast knob.
- Strings are hard-coded; modify `lcd_print` calls for custom messages.

### Q6 – Timer1 Buzzer
- Timer1 interrupt fires every 1 ms (`TMR1H=0xFF`, `TMR1L=0x06`).
- ISR toggles RC3 when `buzzer_enable=1`.
- Main loop gates tone: 2 s enable -> 2 s disable.

### Q7 – UART with Tera Term
- Open Tera Term → Serial → pick COM port shown in Device Manager.
- Settings: 9600 / 8 / N / 1, no flow control.
- After reset you should see:
  ```
  PIC18F4550 UART Ready
  Microcontroller Lab Experiment
  ...
  Enter command:
  ```
- Type `LED_ON` + Enter → on-board LED (RB0) lights.
- `LED_OFF` → LED clears.
- `STATUS` → board returns device information.
- Unknown command → `Unknown command: <text>`.

### Q8 – ADC + LCD
- The kit routes a potentiometer to AN0; turning it sweeps 0–5 V.
- Voltage shown with two decimals, digital value is zero-padded.
- Update rate: 500 ms; remove the clear command if you prefer static display.

---

## 🛠️ Troubleshooting

| Symptom | Checks |
|---------|--------|
| PICLoader cannot detect the board | Verify USB cable, hold program button, reinstall bootloader driver |
| Programming succeeds but nothing runs | Ensure PICLoader resets board, confirm config bits, check that MPLAB X build produced the latest HEX |
| Buttons appear inverted | Buttons are active-low; confirm pull-ups (internal weak pull-ups enabled by hardware) |
| LCD shows random glyphs | Re-run `lcd_init()`, increase delays, adjust contrast |
| UART prints garbage | Confirm 9600 baud in Tera Term, cross-check RX/TX jumpers |
| ADC readings jumpy | Add 100 nF capacitor across AN0 and GND, ensure pot is firmly connected |

---

## 📄 File Map

```
PIC18F4550/
├── README.md (this guide)
├── Q4_Button_LED_Relay_Buzzer/
│   └── button_control.c
├── Q5_LCD_16x2_Interface/
│   └── lcd_display.c
├── Q6_Buzzer_Timer_Interrupt/
│   └── buzzer_timer.c
├── Q7_UART_Serial_Communication/
│   └── uart_communication.c
└── Q8_ADC_LCD_Interface/
    └── adc_lcd.c
```

---

**Document version:** 1.0  
**Last updated:** November 6, 2025  
**Author:** Microcontroller Lab, MMCOE  
**Usage:** Prepared specifically for the Microembedded PIC18F4550 kit practicals.
