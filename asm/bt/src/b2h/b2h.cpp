/**
*   Binary file to hex view converter.
*
*   @file b2h.cpp
*
*   @date 04.2015
*
*   @copyright GNU GPL v2.0
*
*   @author Viktor Prutyanov mailto:vitteran@gmail.com
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char *argv[])
{
    FILE* in_file = fopen("snippet.bin", "rb");
    fseek(in_file, 0, SEEK_END);
    long len = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);
    uint8_t *code = (uint8_t *)calloc(len, sizeof(uint8_t));
    fread((void *)code, sizeof(uint8_t), len, in_file);
    fclose(in_file);

    if (argc == 2 && argv[1][1] == 'x')
    {
        for (int i = 0; i < len; ++i)
        {
            printf("%.2x", code[i]);
        }
    }
    else
    {
        for (int i = 0; i < len; ++i)
        {
            printf("\\x%.2x", code[i]);
        }
    }

    free(code);
    return 0;
}