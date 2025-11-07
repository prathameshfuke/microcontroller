;******************************************************************************
; P89V51RD2 Block Data Transfer - Assembly Language
; Experiment Q1: Transfer data block between memory locations
;
; Author: Prathamesh Lab
; Date: November 7, 2025
; Target Device: P89V51RD2 (8051 compatible)
; IDE: Keil uVision 5
; Build: Generate HEX file and use Flash Magic for downloading
;
; Description:
;   Transfers a block of 10 bytes from one memory location to another.
;   Shows both internal RAM and external RAM transfer methods.
;   Includes full verification steps in comments.
;
; Memory Maps (Two Examples):
;
; 1. Internal RAM to Internal RAM:
;    Source:      30H to 39H (10 bytes)
;    Destination: 40H to 49H (10 bytes)
;
; 2. External RAM to Internal RAM:
;    Source:      1000H to 1009H (external, 10 bytes)
;    Destination: 20H to 29H (internal, 10 bytes)
;******************************************************************************

        ORG     0000H           ; Reset vector
        LJMP    MAIN            ; Jump to main program

;******************************************************************************
; Data initialization (sample data for testing)
;******************************************************************************
        ORG     0030H           ; Source data in internal RAM
        DB      11H,22H,33H,44H,55H,66H,77H,88H,99H,0AAH  ; Test pattern

;******************************************************************************
; Main Program
;******************************************************************************
        ORG     0100H           ; Main program start
MAIN:
        ; Method 1: Internal RAM to Internal RAM Transfer
        MOV     R0, #30H        ; Source pointer
        MOV     R1, #40H        ; Destination pointer
        MOV     R2, #0AH        ; Counter (10 bytes)

LOOP1:  
        MOV     A, @R0          ; Get byte from source
        MOV     @R1, A          ; Store at destination
        INC     R0              ; Next source
        INC     R1              ; Next destination
        DJNZ    R2, LOOP1       ; Loop until done

        ; Method 2: External to Internal RAM Transfer
        MOV     DPTR, #1000H    ; External RAM source
        MOV     R0, #20H        ; Internal RAM destination
        MOV     R2, #0AH        ; Counter (10 bytes)

LOOP2:  
        MOVX    A, @DPTR        ; Read from external RAM
        MOV     @R0, A          ; Write to internal RAM
        INC     DPTR            ; Next external address
        INC     R0              ; Next internal address
        DJNZ    R2, LOOP2       ; Loop until done

DONE:   SJMP    DONE            ; Infinite loop when done

;******************************************************************************
; Verification Steps in Keil µVision:
;
; 1. Build and start debug session (Alt+F7, then F5)
; 2. Open Memory Windows (Alt+6 or View → Memory)
; 3. Check these memory ranges:
;    - 30H-39H: Source data (internal)
;    - 40H-49H: Copied data (internal to internal)
;    - 20H-29H: Copied data (external to internal)
;
; Expected Results (Method 1):
; Address | Value   Address | Value
; 40H     | 11H    45H     | 66H
; 41H     | 22H    46H     | 77H
; 42H     | 33H    47H     | 88H
; 43H     | 44H    48H     | 99H
; 44H     | 55H    49H     | AAH
;
; Note: For physical hardware, use Flash Magic to program and
; an oscilloscope/logic analyzer to verify timing.
;******************************************************************************

        END                     ; End of program