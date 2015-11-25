
#include <stdio.h>
#include "minvm_defs.h"

void itr_dump_state(virtual_machine_t *state) {
    printf("PC: %u (Flags: 0x%02x): A: %02x B: %02x C: %02x D: %02x\n", 
        state->pc, state->flags, state->a, state->b, state->c, state->d);
}

void itr_print_a(virtual_machine_t *state) {
    printf("%c", state->a);
}

