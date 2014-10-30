DEF_CMD(END,    0,  {},                                     "end",      0)

DEF_CMD(PUSH,   1,  CPU_push(stack, val),                   "push",     1)
DEF_CMD(POP,    2,  CPU_pop(stack, &success),               "pop",      0)

DEF_CMD(DUMP,   3,  {},                                     "dump",     0)

DEF_CMD(NOP,    4,  {},                                     "nop",      0)

DEF_CMD(ADD,    5,  CPU_add(stack),                         "add",      0)
DEF_CMD(SUB,    6,  CPU_sub(stack),                         "sub",      0)
DEF_CMD(MUL,    7,  CPU_mul(stack),                         "mul",      0)
DEF_CMD(DIV,    8,  CPU_div(stack),                         "div",      0)

DEF_CMD(OUT,    9,  CPU_out(stack),                         "out",      0)

DEF_CMD(JMP,    10, CPU_jmp(stack, val, &ip, length),       "jmp",      1)
DEF_CMD(JE,     11, CPU_je(stack, val, &ip, length),        "je",       1)
DEF_CMD(JNE,    12, CPU_jne(stack, val, &ip, length),       "jne",      1)
DEF_CMD(JA,     13, CPU_ja(stack, val, &ip, length),        "ja",       1)
DEF_CMD(JAE,    14, CPU_jae(stack, val, &ip, length),       "jae",      1)
DEF_CMD(JB,     15, CPU_jb(stack, val, &ip, length),        "jb",       1)
DEF_CMD(JBE,    16, CPU_jbe(stack, val, &ip, length),       "jbe",      1)
