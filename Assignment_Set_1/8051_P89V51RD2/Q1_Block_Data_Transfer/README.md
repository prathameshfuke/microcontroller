# Block Data Transfer (8051 Assembly)
## Setup & Execution

### Hardware Requirements
- P89V51RD2 development board or compatible 8051 board
- Keil µVision 5 IDE
- Flash Magic programmer (for hardware implementation)

### Memory Map
```
Internal RAM:
30H-39H: Source block (Method 1)
40H-49H: Destination block (Method 1)
20H-29H: Destination block (Method 2)

External RAM:
1000H-1009H: Source block (Method 2)
```

### Program Flow
1. **Method 1**: Internal to Internal RAM Transfer
   - Uses register indirect addressing with R0, R1
   - Transfers 10 bytes from 30H to 40H

2. **Method 2**: External to Internal RAM Transfer
   - Uses DPTR for external RAM access
   - Transfers 10 bytes from 1000H to 20H

### Building in Keil
```
1. Create new project
   - Project → New µVision Project
   - Select device: Philips P89V51RD2

2. Add source file
   - File → New
   - Save as block_transfer.asm
   - Add to project

3. Build Settings
   - Project → Options for Target
   - Set XTAL frequency (11.0592 MHz typical)
   - Enable HEX file generation
```

### Verification
Using Keil Debugger:
1. Build project (F7)
2. Start Debug Session (Ctrl+F5)
3. Open Memory Window (View → Memory)
4. Check memory ranges:
   - 30H-39H: Source data
   - 40H-49H: Destination (Method 1)
   - 20H-29H: Destination (Method 2)

### Sample Data Pattern
```
Source Data: 11H,22H,33H,44H,55H,66H,77H,88H,99H,AAH
Expected in destination after transfer:
Addr | Val  Addr | Val  Addr | Val  Addr | Val  Addr | Val
40H  | 11H  41H  | 22H  42H  | 33H  43H  | 44H  44H  | 55H
45H  | 66H  46H  | 77H  47H  | 88H  48H  | 99H  49H  | AAH
```

### Common Issues & Solutions
1. **Data not transferring**: Check memory pointers (R0, R1, DPTR)
2. **Wrong values**: Verify source data initialization
3. **External RAM issues**: Confirm MOVX instruction timing