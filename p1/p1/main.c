#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int compare (const void *a, const void *b)
{
    float res = fabs(*((float *)a)) - fabs(*((float *)b));
    if (res > 0)
    {
        return 1;
    }
    else if (res < 0)
    {
        return -1;
    }
    else
    {
        if (*((float *)a) < 0)
        {
            return 1;
        }
        else if (*((float *)b) < 0)
        {
            return -1;
        }
        else
        {
            return 0;
        }
    }
}

int main ()
{
    float nums[2000] = {};
    char str[100] = {};
    char c = 0;
    int i = 0, nums_amount = 0;

    while (c != '\n')
    {
        c = getchar();
        if (c == ' ' || c == '\n')
        {
            i = 0;
            nums[nums_amount] = atof (str);
            nums_amount++;
        }
        else
        {
            str[i] = c;
            i++;
        }
    }

    for (int i = 0; i < nums_amount; i++)
    {
        printf ("%f ", nums[i]);
    }
    
    printf ("\n");

    qsort (nums, nums_amount, sizeof (float), compare);

    for (int i = 0; i < nums_amount; i++)
    {
        printf ("%f ", nums[i]);
    }
    
    printf ("\n");

    system ("pause");
}