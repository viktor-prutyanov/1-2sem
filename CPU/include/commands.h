DEF_CMD(END,        0,  cpu->is_end = true,                                                 "end",     0)
                                                                                 
DEF_CMD(PUSH,       1,  CPU_push(cpu->stack, *arg1),                                        "push",    1)
DEF_CMD(POP,        2,  CPU_pop(cpu->stack, arg1),                                          "pop",     1)
                                                                                 
DEF_CMD(DUMP,       3,  CPU_dump(cpu),                                                      "dump",    0)
                                                                                 
DEF_CMD(NOP,        4,  {},                                                                 "nop",     0)

DEF_CMD(ADD,        5,  CPU_add(cpu->stack),                                                "add",     0)
DEF_CMD(SUB,        6,  CPU_sub(cpu->stack),                                                "sub",     0)
DEF_CMD(MUL,        7,  CPU_mul(cpu->stack),                                                "mul",     0)
DEF_CMD(DIV,        8,  CPU_div(cpu->stack),                                                "div",     0)

DEF_CMD(OUT,        9,  CPU_out(cpu),                                                       "out",     0)

DEF_CMD(JMP,        10, CPU_jmp(cpu, jump_to),                                              "jmp",     1)
DEF_CMD(JE,         11, CPU_je (cpu, jump_to),                                              "je",      1)
DEF_CMD(JNE,        12, CPU_jne(cpu, jump_to),                                              "jne",     1)
DEF_CMD(JA,         13, CPU_ja (cpu, jump_to),                                              "ja",      1)
DEF_CMD(JAE,        14, CPU_jae(cpu, jump_to),                                              "jae",     1)
DEF_CMD(JB,         15, CPU_jb (cpu, jump_to),                                              "jb",      1)
DEF_CMD(JBE,        16, CPU_jbe(cpu, jump_to),                                              "jbe",     1)
DEF_CMD(JZ,         17, CPU_jz (cpu, jump_to),                                              "jz",      1)
DEF_CMD(JNZ,        18, CPU_jnz(cpu, jump_to),                                              "jnz",     1)

DEF_CMD(MOV,        19, CPU_mov(arg1, *arg2),                                               "mov",     2) 

DEF_CMD(SWAP,       20, CPU_swap(cpu),                                                      "swap",    0)

DEF_CMD(CALL,       21, CPU_call(cpu, jump_to),                                             "call",    1)
DEF_CMD(RET,        22, CPU_ret(cpu),                                                       "ret",     0)

DEF_CMD(DUP,        23, CPU_dup(cpu),                                                       "dup",     0)

DEF_CMD(IN,         24, CPU_in(cpu),                                                        "in",      0)