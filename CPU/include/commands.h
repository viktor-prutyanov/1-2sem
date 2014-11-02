DEF_CMD(END,        0,  cpu->is_end = true,                                           "end",     0)
                                                                                 
DEF_CMD(PUSH,       1,  CPU_push(cpu->stack, val),                    "push",    1)
DEF_CMD(POP,        2,  {},                                           "pop",     0)
                                                                                 
DEF_CMD(DUMP,       3,  CPU_dump(cpu),                         "dump",    0)
                                                                                 
DEF_CMD(NOP,        4,  {},                                           "nop",     0)
                                                                                 
DEF_CMD(ADD,        5,  CPU_add(cpu->stack),                               "add",     0)
DEF_CMD(SUB,        6,  CPU_sub(cpu->stack),                               "sub",     0)
DEF_CMD(MUL,        7,  CPU_mul(cpu->stack),                               "mul",     0)
DEF_CMD(DIV,        8,  CPU_div(cpu->stack),                               "div",     0)
                                                                                 
DEF_CMD(OUT,        9,  CPU_out(cpu->stack),                               "out",     0)

DEF_CMD(JMP,        10, CPU_jmp(cpu->stack, val, &(cpu->ip), cpu->buffer_size),        "jmp",     1)
DEF_CMD(JE,         11, CPU_je (cpu->stack, val, &(cpu->ip), cpu->buffer_size),        "je",      1)
DEF_CMD(JNE,        12, CPU_jne(cpu->stack, val, &(cpu->ip), cpu->buffer_size),        "jne",     1)
DEF_CMD(JA,         13, CPU_ja (cpu->stack, val, &(cpu->ip), cpu->buffer_size),        "ja",      1)
DEF_CMD(JAE,        14, CPU_jae(cpu->stack, val, &(cpu->ip), cpu->buffer_size),        "jae",     1)
DEF_CMD(JB,         15, CPU_jb (cpu->stack, val, &(cpu->ip), cpu->buffer_size),        "jb",      1)
DEF_CMD(JBE,        16, CPU_jbe(cpu->stack, val, &(cpu->ip), cpu->buffer_size),        "jbe",     1)

DEF_CMD(PUSH_AX,    17,  CPU_push(cpu->stack, cpu->ax),                   "push ax",  0)
DEF_CMD(POP_AX,     18,  cpu->ax = CPU_pop(cpu->stack, &success),         "pop ax" ,  0)