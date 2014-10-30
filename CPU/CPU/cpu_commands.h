#include "..\Stack\stack.h"

bool CPU_push (Stack_t *stack, double value);
double CPU_pop (Stack_t *stack, bool *success);

bool CPU_mul (Stack_t *stack);
bool CPU_div (Stack_t *stack);
bool CPU_add (Stack_t *stack);
bool CPU_sub (Stack_t *stack);

bool CPU_out (Stack_t *stack);

bool CPU_jmp(Stack_t *stack, int value, int *ip, int length);
bool CPU_je(Stack_t *stack, int value, int *ip, int length);
bool CPU_jne(Stack_t *stack, int value, int *ip, int length);
bool CPU_ja(Stack_t *stack, int value, int *ip, int length);
bool CPU_jae(Stack_t *stack, int value, int *ip, int length);
bool CPU_jb(Stack_t *stack, int value, int *ip, int length);
bool CPU_jbe(Stack_t *stack, int value, int *ip, int length);