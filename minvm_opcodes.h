
#ifndef OPCODE
#error "including code must define OPCODE"
#endif

OPCODE(LOADI       , 0x00, "*"  , 1)    // N immediate values based on register mask, (default halt)
OPCODE(INC         , 0x10, "D"  , 1)    // Destination mask
OPCODE(DEC         , 0x20, "D"  , 1)    // Destination mask
OPCODE(LOADR       , 0x30, "DL" , 2)    // Destination mask, address offset register mask
OPCODE(ADD         , 0x40, "DV" , 2)    // Destination mask, 2 operand Value mask
OPCODE(SUB         , 0x50, "DV" , 2)    // Destination mask, 2 operand Value mask
OPCODE(MUL         , 0x60, "DV" , 2)    // Destination mask, 2 operand Value mask
OPCODE(DIV         , 0x70, "DV" , 2)    // Destination mask, 2 operand Value mask
OPCODE(AND         , 0x80, "DV" , 2)    // Destination mask, 2 operand Value mask
OPCODE(OR          , 0x90, "DV" , 2)    // Destination mask, 2 operand Value mask
OPCODE(XOR         , 0xA0, "DV" , 2)    // Destination mask, 2 operand Value mask
OPCODE(ROTR        , 0xB0, "R"  , 1)    // Register selection for rotation
OPCODE(JMPNEQ      , 0xC0, "RI" , 2)    // Register comparand, Index jump destination
OPCODE(JMPEQ       , 0xD0, "RI" , 2)    // Register comparand, Index jump destination
OPCODE(STOR        , 0xE0, "RI" , 2)    // Register source mask, Index destination
OPCODE(ITR         , 0xF0, "i"  , 1)    // Index of routine in low bits of instruction 
OPCODE(JMPI        , 0xD0, "I"  , 2)    // Simulated opcode, JMPEQ (unconditional jump)
OPCODE(NOOP        , 0x00, ""   , 1)    // Simulated opcode, Increment with no arguments
