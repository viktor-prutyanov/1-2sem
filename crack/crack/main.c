#include <stdio.h>
#include <stdlib.h>
#include <io.h>

int main()
{
    FILE *file1 = fopen("PASSWD.COM", "rb");
    FILE *file2 = fopen("CRACKD.COM", "wb");
    if (file1 == nullptr)
    {
        printf ("Error opening PASSWD.COM file.\n");
        system ("pause");
        return EXIT_FAILURE;
    }
    if (file1 == nullptr)
    {
        printf ("Error opening CRACKD.COM file.\n");
        system ("pause");
        return EXIT_FAILURE;
    }
    long len = filelength(fileno(file1));
    printf ("Length is %d\n", len);
    char *data = (char *)calloc(len, sizeof(char));
    printf ("PASSWD.COM succeed. %d bytes has read.\n", fread((void *)data, sizeof(char), len, file1));

    char password[] = "hello";
    for (int i = 0x40; i < 0x45; i++)
    {
        if (password[i - 0x40] != data[i])
        {
            printf ("Crack attempt failed, maybe program was already cracked.");
            return EXIT_FAILURE;
        }
    }

    data[0x2a] = 0xf2;
    printf ("CRACKD.COM succeed. %d bytes has written.\n", fwrite((void *)data, sizeof(char), len, file2));

    fcloseall();
    printf ("Success.\n");
    system ("pause");
    return EXIT_SUCCESS;
}