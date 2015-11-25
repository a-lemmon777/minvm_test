#ifndef _included_minvm_defs_h
#define _included_minvm_defs_h

#include <stdint.h>

// Visual studio is still on C89
#ifndef bool
typedef int bool;
#define true 1
#define false 0
#endif


// Basic types
typedef unsigned char byte;
typedef const char cchar;

// Machine constants
#define RAM_SIZE          256
#define NUM_REGISTERS     4
#define WORD_SIZE         8

// Register masks
#define REGA              0x01
#define REGB              0x02
#define REGC              0x04
#define REGD              0x08

// Setting the halt flag terminates the machine
#define MINVM_HALT        0x01 

// Setting the exception flag indicates an exception occurred
#define MINVM_EXCEPTION   0x02

// Misc defines
#define MESSAGE_SZ        256

//
// Interrupt function type, to be invoked by VM with the ITR instruction 
//
typedef struct virtual_machine_t virtual_machine_t;
typedef void (*interrupt_function_t)(virtual_machine_t *state);

//
// Structure representing virtual machine running state, modify this 
// structure to store the current machine state which will be evaluated after 
// the program runs
// 
struct virtual_machine_t {
    byte                    flags;          // Machine flags
    byte                    pc;             // The program counter
    byte                    a, b, c, d;     // The general registers
    interrupt_function_t   *interrupts;     // Interrupt table, with 16 possible slots
    byte                    *code;          // Pointer to core memory, 256 words
};

// Prevent unreferenced variable warning
#define UNREF(v) (void)v;

// Simple macro to count known array size
#define COUNTOF(a) (sizeof(a) / sizeof(a[0]))

// Default interrupts, other interrupts reserved for extensions
void itr_dump_state (virtual_machine_t *state);      // ITR 0
void itr_print_a (virtual_machine_t *state);         // ITR 1

#endif // _included_minvm_defs_h
