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

ORG 0000H           ; Program start address

MOV R2, #10         ; Initialize counter R2 with 10 — number of bytes to transfer
MOV DPTR, #1000H    ; Load DPTR with 1000H — starting address of source data (in external RAM)
MOV R0, #20H        ; Load R0 with 20H — starting address of destination (in internal RAM)

LOOP:
    MOVX A, @DPTR   ; Move byte from external memory (at DPTR) into accumulator A
    MOV @R0, A      ; Store accumulator contents into internal RAM at address pointed by R0
    INC DPTR        ; Increment DPTR to point to next external memory location
    INC R0          ; Increment R0 to point to next internal RAM location
    DJNZ R2, LOOP   ; Decrement R2 and repeat loop until R2 = 0 (10 bytes copied)

SJMP $              ; Infinite loop (halts program execution here)
END                 ; End of program

