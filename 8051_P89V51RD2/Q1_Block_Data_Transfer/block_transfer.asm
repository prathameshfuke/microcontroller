;******************************************************************************
; P89V51RD2 Block Data Transfer - Assembly Language
; Experiment Q1: Transfer 10 bytes from external RAM to internal RAM
;
; Author: Microcontroller Lab
; Date: November 6, 2025
; Target Device: P89V51RD2 (8051 compatible)
; IDE: Keil uVision 5
; Testing: Use Keil Debugger to verify memory transfer
;
; Description:
;   Simple block data transfer using MOVX for external memory read
;   and MOV for internal memory write. Transfers 10 bytes from
;   external RAM address 1000H to internal RAM address 20H.
;
; Memory Map:
;   Source: External RAM 1000H-1009H (10 bytes)
;   Destination: Internal RAM 20H-29H (10 bytes)
;
; Verification in Keil:
;   1. Build project (Ctrl+B)
;   2. Start Debug (Ctrl+F5)
;   3. View → Memory Windows → Memory 1
;   4. Enter address 0x1000 to view source
;   5. View → Memory Windows → Memory 2
;   6. Enter address 0x20 to view destination
;   7. Step through program (F11) and verify transfer
;******************************************************************************

ORG 0000H

MOV R2, #10         ; Counter = 10 bytes
MOV DPTR, #1000H    ; Source address (external RAM)
MOV R0, #20H        ; Destination address (internal RAM)

LOOP:
    MOVX A, @DPTR   ; Read from source
    MOV @R0, A      ; Write to destination
    INC DPTR        ; Next source
    INC R0          ; Next destination
    DJNZ R2, LOOP   ; Decrement counter, loop

SJMP $              ; End
END
