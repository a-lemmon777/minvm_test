//
// MinVM Take Home Test:
//
// This is the driver source that will be 
// used to execute your VM. 
// 
// See README.md for full instructions.
//
// Sample byte code is contained in samples/*.bin
//
// Implement your VM code in minvm_test.c
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#include "minvm_defs.h"
#include "minvm_int.h"

extern void vm_exec(virtual_machine_t *vm);

static interrupt_function_t s_interrupts[16] = {
    itr_dump_state,
    itr_print_a
};

int main(int argc, char **argv) {
    int i;
    buffer_t buffer;
    virtual_machine_t vm = { 0, 0, 0, 0, 0, 0, &s_interrupts[0] };

    if (argc < 2) {
        printf("usage: ./vm <filename> [filename]\n");
        return -1;
    }

    for (i = 1; i < argc; ++i) {
        cchar *filename = argv[i];
        if (!mvm_read_buffer_ram(filename, &buffer)) {
            mvm_error("failed to buffer file");
            return -1;
        }

        if (buffer.data_size > RAM_SIZE) {
            mvm_error("%s: invalid RAM: %u", filename, buffer.data_size);
            mvm_free_buffer(&buffer);
            return -1;
        }

        mvm_info("## running: %s, %u bytes", filename, buffer.data_size);
        vm.code = (byte*)buffer.data;

        vm_exec(&vm);
        if (vm.flags & MINVM_HALT) {
            mvm_info("%s PC: 0x%02x, A: 0x%02x, B: 0x%02x, C: 0x%02x, D: 0x%02x",
                ((vm.flags & MINVM_EXCEPTION) ? "EXCEPTION" : "HALT"),
                vm.pc, vm.a, vm.b, vm.c, vm.d);
        }
        if (!mvm_free_buffer(&buffer)) {
            return 1;
        }
    }

    return 0;
}
