#define END                 "\xc3"
#define L_END               1

#define OUT                 "\x48\x8b\x44\x24\x08\x55\x48\x89\xe5\x48\xff\xcd\xc6\x45\x00\x0a\x48\x31\xd2\xbb\x0a\x00\x00\x00\xb9\x01\x00\x00\x00\x48\xff\xc1\x48\x31\xd2\x48\xf7\xf3\x48\x83\xc2\x30\x48\xff\xcd\x88\x55\x00\x48\x83\xf8\x00\x75\xe7\xb8\x01\x00\x00\x00\xbf\x01\x00\x00\x00\x48\x89\xee\x48\x89\xca\x0f\x05\x5d"
#define L_OUT               73

#define IN                  "\x55\x48\x89\xe5\x48\x83\xed\x20\xba\x20\x00\x00\x00\x48\x89\xee\xbf\x00\x00\x00\x00\xb8\x00\x00\x00\x00\x0f\x05\x49\x89\xe9\x49\xff\xc9\x48\x89\xe9\x48\x01\xc1\x48\x83\xe9\x02\x41\xba\x01\x00\x00\x00\xbb\x0a\x00\x00\x00\x48\x31\xc0\x4d\x31\xc0\x48\x31\xc0\x8a\x01\x2c\x30\x49\xf7\xe2\x49\x01\xc0\x4c\x89\xd0\x48\xf7\xe3\x49\x89\xc2\x48\xff\xc9\x4c\x39\xc9\x75\xe2\x5d\x4c\x89\xc0"
#define L_IN                95

#define PUSH_BYTE           "\x6a" //+ byte
#define L_PUSH_BYTE         2

#define PUSH_DWORD          "\x68" //+ dword
#define L_PUSH_DWORD        5

#define PUSH_REG            "\x41" //+ from 0x53 to 0x56 : from r11 to r14
#define L_PUSH_REG          2

#define POP_REG             "\x41" //+ from 0x5b to 0x5e : from r11 to r14
#define L_POP_REG           2

#define ADD                 "\x58\x48\x01\x04\x24"
#define L_ADD               5
    
#define SUB                 "\x5b\x58\x48\x29\xd8\x50"
#define L_SUB               6

#define MUL                 "\x58\x5b\x48\xf7\xe3\x50"
#define L_MUL               6

#define DIV                 "\x58\x48\x31\xd2\x5b\x48\xf7\xf3\x50"
#define L_DIV               9

#define NOP                 "\x90"
#define L_NOP               1

#define DUP                 "\x58\x50\x50"
#define L_DUP               3

#define SWAP                "\x58\x5b\x50\x53"
#define L_SWAP              4

#define JMP                 "\xe9" //+ dword (relative address) 
#define L_JMP               5

#define JB                  "\x58\x5b\x48\x39\xd8\x0f\x82" //+ dword (relative address)
#define JAE                 "\x58\x5b\x48\x39\xd8\x0f\x83" //
#define JE                  "\x58\x5b\x48\x39\xd8\x0f\x84" // 
#define JNE                 "\x58\x5b\x48\x39\xd8\x0f\x85" //
#define JBE                 "\x58\x5b\x48\x39\xd8\x0f\x86" //
#define JA                  "\x58\x5b\x48\x39\xd8\x0f\x87" //
#define JZ                  "\x58\x48\x83\xf0\x00\x0f\x84" //
#define JNZ                 "\x58\x48\x83\xf0\x00\x0f\x85" //
#define L_JXX               11 

#define RET                 "\xc3"
#define L_RET               1

#define CALL                "\xe8" //+ dword (relative address)
#define L_CALL              5

#define MOV_REG_REG         "\x4d\x89" //+ byte 0xdN + 0x08 * (M - 10) for rN <- rM (at least, for r11-r14)
#define L_MOV_REG_REG       3

#define MOV_REG_NUM         "\x41" //+ from 0xbb to 0xbc : from r11 to r14 + dword
#define L_MOV_REG_NUM       6

#define MOV_RR_RBASE        '\xda'
#define MOV_RN_RBASE        '\xba'
#define MOV_RR_RSTEP        '\x08'
#define PUSH_R_RBASE        '\x52'
#define  POP_R_RBASE        '\x5a'

#define INSTR_SIZEOF(instr) (sizeof(instr) - 1)

#define BALANCE_POP_RAX     "\x58"
#define BALANCE_PUSH_RAX    "\x50" 