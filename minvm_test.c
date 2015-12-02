//
// MinVM Take Home Test
//
// Implement a minimal VM  
//
// See README.md for full instructions
//
// Author: Aaron Lemmon
//

#include "minvm_defs.h"
#include <stdio.h> // Can probably take this out eventually

const byte registerMasks[] = { REGA, REGB, REGC, REGD };
void loadi (virtual_machine_t *vm, byte *registers[], byte argument);
void inc (byte *registers[], byte argument);
void dec (byte *registers[], byte argument);
void itr (virtual_machine_t *vm, byte argument);

// Implement your VM here
void vm_exec (virtual_machine_t *vm) {
    byte *registers[NUM_REGISTERS]; // Used to loop over the registers, making the code less verbose
    registers[0] = &(vm->a); // Unfortunately, ANSI won't let me inline these
    registers[1] = &(vm->b);
    registers[2] = &(vm->c);
    registers[3] = &(vm->d);
    while (!(vm->flags & MINVM_HALT)) { // Continue running if the halt flag is not set
        byte instruction = vm->code[vm->pc++]; // Increments the program counter past the instruction
        byte opcode = 0xF0 & instruction; // The upper 4 bits of the instruction
        byte argument = 0x0F & instruction; // The lower 4 bits of the instruction
        switch (opcode) {
            case 0x00: // LOADI
                loadi(vm, registers, argument); break;
            case 0x10: // INC
                inc(registers, argument); break;
            case 0x20: // DEC
                dec(registers, argument); break;
            case 0x30: // LOADR
                printf("LOADR\n"); break;
            case 0x40: // ADD
                printf("ADD\n"); break;
            case 0x50: // SUB
                printf("SUB\n"); break;
            case 0x60: // MUL
                printf("MUL\n"); break;
            case 0x70: // DIV
                printf("DIV\n"); break;
            case 0x80: // AND
                printf("AND\n"); break;
            case 0x90: // OR
                printf("OR\n"); break;
            case 0xA0: // XOR
                printf("XOR\n"); break;
            case 0xB0: // ROTR
                printf("ROTR\n"); break;
            case 0xC0: // JMPNEQ
                printf("JMPNEQ\n"); break;
            case 0xD0: // JMPEQ
                printf("JMPEQ\n"); break;
            case 0xE0: // STOR
                printf("STOR\n"); break;
            case 0xF0: // ITR
                itr(vm, argument); break;
        }
    }
}

void loadi (virtual_machine_t *vm, byte *registers[], byte argument) {
    printf("LOADI\n");
    if (argument == 0x00) {
        vm->flags = MINVM_HALT;
        return;
    }
    for (int i = 0; i < NUM_REGISTERS; i++) {
        if (argument & registerMasks[i]) {
            *registers[i] = vm->code[vm->pc++]; // Write the byte to the ith register
        }
    }
}

void inc (byte *registers[], byte argument) {
    printf("INC\n");
    unsigned long temp = 0; // Relevent registers will be copied here
    for (int i = NUM_REGISTERS; i >= 0; i--) { // Start with register D, since it should be the most significant byte
        if (argument & registerMasks[i]) {
            temp = temp << WORD_SIZE; // Make room for the next value (has no effect if temp is still 0)
            temp = temp | *registers[i]; // Put value of the register at the end of temp
        }
    }
    temp++;
    for (int i = 0; i < NUM_REGISTERS; i++) { // Start putting temp back into registers, starting with register A
        if (argument & registerMasks[i]) {
            *registers[i] = (byte)temp; // Stores the least significant byte into the register
            temp = temp >> WORD_SIZE; // Shift values in temp to prepare for next iteration
        }
    }
}

void dec (byte *registers[], byte argument) {
    printf("DEC\n");
    unsigned long temp = 0; // Relevent registers will be copied here
    for (int i = NUM_REGISTERS; i >= 0; i--) { // Start with register D, since it should be the most significant byte
        if (argument & registerMasks[i]) {
            temp = temp << WORD_SIZE; // Make room for the next value (has no effect if temp is still 0)
            temp = temp | *registers[i]; // Put value of the register at the end of temp
        }
    }
    temp--;
    for (int i = 0; i < NUM_REGISTERS; i++) { // Start putting temp back into registers, starting with register A
        if (argument & registerMasks[i]) {
            *registers[i] = (byte)temp; // Stores the least significant byte into the register
            temp = temp >> WORD_SIZE; // Shift values in temp to prepare for next iteration
        }
    }
}

void itr (virtual_machine_t *vm, byte argument) {
    (vm->interrupts[argument])(vm); // Calls the interrupt function specified by the index in argument
}
