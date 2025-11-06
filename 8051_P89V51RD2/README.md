# P89V51RD2 (8051) Microcontroller Lab Experiments
## Keil uVision 5 - Complete Guide

---

## 📋 Table of Contents
1. [Device Overview](#device-overview)
2. [Software Setup](#software-setup)
3. [Hardware Requirements](#hardware-requirements)
4. [Experiment List](#experiment-list)
5. [Programming Guide](#programming-guide)
6. [Troubleshooting](#troubleshooting)

---

## 🔧 Device Overview (Simulation Focus)

⚠️ **Lab Note:** These 8051 practicals are demonstrated directly inside Keil µVision without external hardware. Use the built-in Port window, Memory window, and Logic Analyzer to observe LED states, memory transfers, and waveform generation.

### P89V51RD2 Specifications
| Parameter | Specification |
|-----------|---------------|
| **Manufacturer** | NXP (formerly Philips) |
| **CPU Core** | 80C51 compatible |
| **Flash Memory** | 64 KB (ISP/IAP capable) |
| **RAM** | 1024 bytes internal + 768 bytes expandable |
| **I/O Ports** | 4 × 8-bit (Port 0-3) |
| **High-Current Pins** | Port 1: 16 mA (3 pins) |
| **Operating Voltage** | 5V (0-40 MHz) |
| **Clock Modes** | 12-clock or 6-clock (X2 mode) |
| **Timers** | 3 × 16-bit |
| **Interrupts** | 8 sources, 4 priority levels |
| **Special Features** | Dual DPTR, PCA, PWM, SPI, UART |

### Key Advantages over Standard 8051
- ✅ **64 KB on-chip flash** (no external memory needed)
- ✅ **In-System Programming (ISP)** via serial port
- ✅ **Dual DPTR** for faster code execution
- ✅ **Higher port current** capability (16 mA)
- ✅ **X2 mode** for 2× performance

---

## 💻 Software Setup

### 1. Keil uVision 5 Installation
1. Download from: https://www.keil.com/download/product/
2. Install Keil µVision 5 (C51 compiler)
3. Register for evaluation license (2KB code limit for free version)

### 2. FlashMagic Installation (for ISP programming)
1. Download from: http://www.flashmagictool.com/
2. Install FlashMagic
3. Supports P89V51RD2 ISP programming via serial port

### 3. Creating a New Project

#### Step-by-Step Project Creation:

**A. Assembly Language Project:**
```
1. File → New Project
2. Choose project folder and name (e.g., "BlockTransfer")
3. Select Device: 
   - Vendor: NXP (Philips)
   - Device: P89V51RD2
4. Click "OK" when asked to copy STARTUP.A51
5. Right-click "Source Group 1" → Add New Item → Assembly File (.asm)
6. Write code and save
7. Build Project (Hammer icon or Ctrl+B)
```

**B. Embedded C Project:**
```
1. File → New Project
2. Choose project folder and name
3. Select Device: NXP P89V51RD2
4. Copy STARTUP.A51 (click "Yes")
5. Right-click "Source Group 1" → Add New Item → C File (.c)
6. Write code, include <reg51.h>
7. Configure Target Options:
   - Crystal Frequency: 11.0592 MHz
   - Memory Model: Small/Large (based on code size)
   - Optimization: Level 2
   - Enable "Create HEX File" in Output tab
8. Build Project
```

### 4. Target Configuration

**Project Options (Alt+F7):**

**Target Tab:**
- Crystal (MHz): `11.0592` (recommended for UART accuracy)
- Memory Model: 
  - Small: For programs < 2KB
  - Compact: For programs 2-64KB
  - Large: For programs using external memory
- Code Banking: Disabled (unless using > 64KB)

**Output Tab:**
- ☑️ Create HEX File (required for programming)
- Select folder for HEX output

**C51 Tab:**
- Optimization Level: 8 (balance speed/size)
- Define: (leave blank unless needed)

---

## 🔌 Hardware Requirements

### Essential Components

#### For All Experiments:
- P89V51RD2 microcontroller IC (40-pin DIP)
- 11.0592 MHz crystal oscillator
- 2 × 22pF ceramic capacitors (for crystal)
- 10 µF electrolytic capacitor (power supply)
- 0.1 µF ceramic capacitor (bypass)
- 10 kΩ resistor (reset pull-up)
- 8.2 kΩ resistor (EA pin pull-up)
- USB-to-Serial adapter (CP2102/CH340/FT232)
- +5V regulated power supply
- Breadboard and jumper wires

#### Experiment-Specific Components:

**Q1: Block Data Transfer**
- None (internal RAM only)

**Q2: LED Interface**
- 8 × LEDs (5mm, any color)
- 8 × 330Ω resistors

**Q3: DAC Interface**
- DAC0808 IC (8-bit DAC)
- LM741 or similar op-amp
- 10 kΩ potentiometer (VREF adjustment)
- 100 nF capacitor (output smoothing)
- Oscilloscope/CRO

### P89V51RD2 Pin Configuration

```
                    P89V51RD2 (40-pin DIP)
                    ┌────────────────────┐
      (T2) P1.0  ---|1                 40|--- VCC (+5V)
      (T2EX) P1.1 --|2                 39|--- P0.0 (AD0)
      (ECI) P1.2 ---|3                 38|--- P0.1 (AD1)
      (CEX0) P1.3 --|4                 37|--- P0.2 (AD2)
      (CEX1) P1.4 --|5                 36|--- P0.3 (AD3)
      (CEX2) P1.5 --|6                 35|--- P0.4 (AD4)
      (CEX3) P1.6 --|7                 34|--- P0.5 (AD5)
      (CEX4) P1.7 --|8                 33|--- P0.6 (AD6)
      RST ---------|9                 32|--- P0.7 (AD7)
      (RXD) P3.0 --|10                31|--- EA (Pull-up to VCC)
      (TXD) P3.1 --|11                30|--- ALE
      (INT0) P3.2 -|12                29|--- PSEN
      (INT1) P3.3 -|13                28|--- P2.7 (A15)
      (T0) P3.4 ---|14                27|--- P2.6 (A14)
      (T1) P3.5 ---|15                26|--- P2.5 (A13)
      (WR) P3.6 ---|16                25|--- P2.4 (A12)
      (RD) P3.7 ---|17                24|--- P2.3 (A11)
      XTAL2 -------|18                23|--- P2.2 (A10)
      XTAL1 -------|19                22|--- P2.1 (A9)
      GND ---------|20                21|--- P2.0 (A8)
                    └────────────────────┘
```

### Minimal Circuit for Testing

```
Crystal Circuit:
  XTAL1 (Pin 19) --- 22pF --- GND
       |
       +--- 11.0592 MHz Crystal
       |
  XTAL2 (Pin 18) --- 22pF --- GND

Reset Circuit:
  RST (Pin 9) --- 10kΩ --- VCC
       |
       +--- Push button --- GND
       |
       +--- 10µF (Optional: Auto-reset)

Power Supply:
  VCC (Pin 40) --- +5V (regulated)
  GND (Pin 20) --- Ground
  
  Between VCC and GND:
    - 10µF electrolytic capacitor
    - 0.1µF ceramic capacitor (close to IC)

EA Pin:
  EA (Pin 31) --- 8.2kΩ --- VCC (High for internal code execution)
```

---

## 📝 Experiment List

### Q1: Block Data Transfer (Assembly)
**File:** `Q1_Block_Data_Transfer/block_transfer.asm`

**Objective:** Transfer 10 bytes from external RAM (`1000H`) to internal RAM (`20H`) using a minimal loop.

**Key Points:**
- Reads data with `MOVX @DPTR` and writes internally with `MOV @R0`
- Loop controlled by `DJNZ` with 10-byte count
- Perfect for simulation via Keil Memory windows (no hardware needed)

**Build:**
```bash
1. Open Keil uVision 5
2. Create new project → Select P89V51RD2
3. Add block_transfer.asm to Source Group
4. Build (Ctrl+B)
5. Check Build Output for errors
6. HEX file location: Objects folder
```

**Verification (Keil Simulation):**
- Debug → Start/Stop Debug Session (Ctrl+F5)
- View → Memory Windows → Memory 1 → address `0x1000`
- View → Memory Windows → Memory 2 → address `0x20`
- Step through loop (F11) and watch destination bytes update

---

### Q2: LED Interface (Embedded C)

#### Part (i): LED Blinking
**File:** `Q2_LED_Interface/led_blink.c`

**Objective:** Blink 8 LEDs connected to Port 1 with 500ms intervals

**Circuit:**
```
P1.0 --- 330Ω --- LED Anode --- LED Cathode --- GND
P1.1 --- 330Ω --- LED Anode --- LED Cathode --- GND
...
P1.7 --- 330Ω --- LED Anode --- LED Cathode --- GND
```

**Expected Output:**
- All LEDs ON for 500ms
- All LEDs OFF for 500ms
- Repeats continuously

#### Part (ii): LED as Binary Counter
**File:** `Q2_LED_Interface/led_counter.c`

**Objective:** Display binary counting 0-255 on LEDs

**Expected Output:**
- Count 0: All LEDs OFF (00000000)
- Count 1: Only P1.0 ON (00000001)
- Count 255: All LEDs ON (11111111)
- Each count displays for 1 second

**Build:**
```bash
1. Create new project for P89V51RD2
2. Add led_blink.c or led_counter.c
3. Include <reg51.h>
4. Set crystal: 11.0592 MHz
5. Enable HEX file output
6. Build project
```

---

### Q3: DAC Interface (Embedded C)

#### Part (i): Square Wave Generation
**File:** `Q3_DAC_Interface/square_wave.c`

**Objective:** Generate 1 kHz square wave using DAC0808

**Circuit:**
```
P0.0-P0.7 --- DAC0808 (D0-D7)
DAC0808 Pin 4 (Output) --- Oscilloscope
DAC0808 Pin 14 (VREF+) --- +5V
DAC0808 Pin 15 (VREF-) --- GND
```

**Expected Output:**
- Frequency: ~1 kHz
- Amplitude: 0V to 5V
- Duty Cycle: 50%

#### Part (ii): Sawtooth Wave Generation
**File:** `Q3_DAC_Interface/sawtooth_wave.c`

**Objective:** Generate linear ramp waveform

**Expected Output:**
- Frequency: ~1 kHz
- Ramp: Linear 0V to 5V
- Sharp fall from 5V to 0V

**DAC0808 Pin Configuration:**
| Pin | Name | Connection |
|-----|------|------------|
| 1 | NC | No connect |
| 2 | NC | No connect |
| 3 | VCC | +5V |
| 4 | IOUT | Output (to op-amp or scope) |
| 5-12 | D7-D0 | P0.7-P0.0 (digital inputs) |
| 13 | GND | Ground |
| 14 | VREF+ | +5V |
| 15 | VREF- | GND |

---

## 🔥 Programming Guide

### Method 1: Using FlashMagic (ISP via Serial)

**Hardware Connections:**
```
USB-to-Serial Adapter:
  TX  ---  P3.0 (RXD, Pin 10)
  RX  ---  P3.1 (TXD, Pin 11)
  GND ---  GND (Pin 20)
  
Optional: DTR for auto-reset
```

**FlashMagic Configuration:**
1. Open FlashMagic software
2. Set parameters:
   - **Device:** P89V51RD2 (89V51RD2)
   - **COM Port:** Select active port
   - **Baud Rate:** 9600
   - **Oscillator (MHz):** 11.0592
   - **Interface:** None (ISP)
3. Browse HEX file from Keil build
4. Options:
   - ☑️ Erase blocks used by HEX file
   - ☐ Verify after programming (recommended)
   - ☐ Fill unused flash (not needed)
5. Click **"Start"**
6. Reset P89V51RD2 when prompted (or press reset button)
7. Wait for programming completion

**Entering ISP Mode:**
- Power off device
- Connect serial cable
- Press and hold reset button
- Power on device
- Release reset after 1 second
- FlashMagic should detect device

### Method 2: Using Hardware Programmer

If using dedicated programmer (TL866II, etc.):
1. Place P89V51RD2 in ZIF socket
2. Load HEX file
3. Program device
4. Verify
5. Insert programmed IC into circuit

---

## 🐛 Troubleshooting

### Build Errors

**Error: "Cannot open source file 'reg51.h'"**
- **Solution:** Ensure Keil C51 compiler is installed correctly
- Check: Tools → Options → C51 → Include Path

**Error: "Target not created"**
- **Solution:** 
  1. Check syntax errors in code
  2. Ensure device (P89V51RD2) is selected
  3. Rebuild project (Project → Rebuild All Target Files)

**Warning: "L15: MULTIPLE CALL TO SEGMENT"**
- **Solution:** Function called from both main and interrupt
- Use `reentrant` keyword or restructure code

### Programming Errors

**FlashMagic: "Failed to autobaud"**
- **Solution:**
  1. Check serial connections (swap TX/RX if needed)
  2. Verify COM port selection
  3. Check baud rate (try 9600 or 4800)
  4. Ensure P89V51RD2 is in ISP mode (reset properly)
  5. Check crystal frequency setting

**FlashMagic: "Device not responding"**
- **Solution:**
  1. Check power supply (must be stable 5V)
  2. Verify EA pin is pulled HIGH
  3. Confirm crystal oscillation (use scope on XTAL2)
  4. Try manual reset: Ground RST pin briefly

**Programming successful but circuit doesn't work:**
- **Solution:**
  1. Check EA pin: must be HIGH for internal code execution
  2. Verify crystal circuit (22pF caps to ground)
  3. Check power supply stability
  4. Add bypass capacitors (0.1µF) near VCC/GND pins

### Hardware Issues

**LEDs not lighting:**
- Check LED polarity (anode to resistor, cathode to GND)
- Verify Port 1 pins not configured as input
- Test LEDs individually with multimeter
- Check resistor values (330Ω recommended)

**DAC output incorrect:**
- Verify VREF+ = +5V and VREF- = GND
- Check all 8 data lines (P0.0-P0.7) connected properly
- Ensure DAC power supply is stable
- Add 100nF capacitor at output for noise reduction

**Crystal not oscillating:**
- Check crystal orientation (either way works for 2-pin)
- Verify 22pF capacitors to ground on both XTAL1 and XTAL2
- Ensure crystal is not damaged (test with multimeter)
- Try different crystal value (11.0592 MHz or 12 MHz)

### Debugging Tips

**Using Keil Simulator:**
```
1. Debug → Start/Stop Debug Session (Ctrl+F5)
2. View → Periodic Window → Port 1 (to see port state)
3. View → Memory Windows → Memory 1 (to inspect RAM)
4. Use Step (F11) to execute line by line
5. Set breakpoints by clicking on line number
```

**Serial Monitor for UART debugging:**
- Use PuTTY or Tera Term
- Configure: 9600 baud, 8-N-1
- Check TX output from P3.1

---

## 📚 Additional Resources

### Reference Documents:
1. **P89V51RD2 Datasheet:** https://www.nxp.com/docs/en/data-sheet/P89V51RD2.pdf
2. **Keil C51 Compiler Manual:** Installed with Keil (Help → µVision Help)
3. **FlashMagic User Guide:** http://www.flashmagictool.com/

### Crystal Frequency Selection:

| Frequency | Use Case | UART Error |
|-----------|----------|------------|
| 11.0592 MHz | **Recommended** - Perfect UART timing | 0% |
| 12 MHz | Common, but slight UART error | ~0.16% |
| 20 MHz | High-speed applications | Variable |

### Baud Rate Calculation (11.0592 MHz):
```
Timer 1 Mode 2 (8-bit auto-reload)
SMOD = 0: TH1 = 256 - (11.0592MHz / (384 × Baud))
SMOD = 1: TH1 = 256 - (11.0592MHz / (192 × Baud))

For 9600 baud (SMOD=0):
TH1 = 256 - (11.0592MHz / 368640) = 256 - 30 = 0xFD
```

---

## 📞 Support

For lab-related queries:
- Refer to experiment manuals provided by MMCOE
- Consult lab instructor for hardware issues
- Check Keil community forums for software problems

---

**Document Version:** 1.0  
**Last Updated:** November 6, 2025  
**Author:** Microcontroller Lab, MMCOE
