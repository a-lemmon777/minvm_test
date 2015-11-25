#
# Simple build file for the vm
#
# You can run all samples using the command:
#
#   find . -name \*.bin -exec vm {} \;
#

PROGRAMS = vm
default: all

all: ${PROGRAMS}

# Default flags disable optimization and enable gdb
CFLAGS = -Wall -Werror -ggdb -O0

clean:
	rm -f ${PROGRAMS}

vm: minvm_test.c minvm_int.c minvm_itr.c minvm_driver.c minvm_defs.h minvm_int.c minvm_opcodes.h
	gcc ${CFLAGS} -o vm minvm_test.c minvm_driver.c minvm_itr.c minvm_int.c
