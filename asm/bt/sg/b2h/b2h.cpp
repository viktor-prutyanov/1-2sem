#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main()
{
    FILE* in_file = fopen("snippet.bin", "rb");
    fseek(in_file, 0, SEEK_END);
    long len = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);
    uint8_t *code = (uint8_t *)calloc(len, sizeof(uint8_t));
    fread((void *)code, sizeof(uint8_t), len, in_file);
    fclose(in_file);

    for (int i = 0; i < len; ++i)
    {
        printf("\\x%.2x", code[i]);
    }

    free(code);
    return 0;
}