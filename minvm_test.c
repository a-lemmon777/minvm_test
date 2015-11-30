//
// MinVM Take Home Test
//
// Implement a minimal VM  
//
// See README.md for full instructions
//

#include "minvm_defs.h"
#include <stdio.h> // Can probably take this out eventually

// Implement your VM here
void vm_exec (virtual_machine_t *vm) {
    // Continue running if the halt flag is not set
    while (!(vm->flags & MINVM_HALT)) {
        byte instruction = vm->code[vm->pc];
        byte opcode = 0xF0 & instruction; // The upper 4 bits of the instruction
        switch (opcode) {
            case 0x00: // LOADI
                printf("LOADI\n"); break;
            case 0x10: // INC
                printf("INC\n"); break;
            case 0x20: // DEC
                printf("DEC\n"); break;
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
                printf("ITR\n"); break;
        }
    }
    //printf("%02x", instruction);
    //printf("%02x", opcode);
    //printf("%d", RAM_SIZE);
    //printf("%d", !(0x03 & MINVM_HALT));
    //printf("\n");
    //itr_dump_state(vm);
    UNREF(vm);
}
