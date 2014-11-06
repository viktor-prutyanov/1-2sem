/**
*   Assemler
*
*   @file stack.h
*
*   @date 10.2014
*
*   @copyright GNU GPL v2.0
*
*   @author Viktor Prutyanov mailto:vitteran@gmail.com 
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdint.h>

#define HEADER "VPVM102"

#define label_length 8
#define reg_length 2
#define command_length 4
#define NO_CMD INT16_MIN

#define IS_JUMP(num) ((10 <= num) && (num <= 18) || (num == 21))

typedef struct label_t
{
    char name[label_length + 1];
    int16_t address;
};

/**
    @brief Function that gets length of file

    @param file is pointer to file

    @return length of file in bytes 
*/
unsigned long int file_length(FILE *file);

size_t write_values_table(FILE *out_file, size_t values_count, double *values);

size_t write_header(FILE *out_file, size_t values_count, size_t commands_count, char *header);

bool first_pass(FILE *in_file, size_t *values_count, size_t *labels_count, size_t *commands_count);

bool second_pass(FILE *in_file, size_t values_count, size_t labels_count, size_t commands_count, double *values, label_t *labels);

size_t write_command(FILE *out_file, uint16_t command_number, int16_t arg1, int16_t arg2, int16_t arg3);

bool third_pass(FILE *in_file, FILE *out_file, size_t values_count, size_t labels_count, size_t commands_count, double *values, label_t *labels);

int16_t find_value(size_t values_count, double *values, double value);

int16_t find_label(size_t labels_count, label_t *labels, char* label_name);

bool read_reg(FILE *in_file, char *reg);

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf ("Usage:\n\tasm <input file> <output file>\n");
        #ifdef _DEBUG
            system ("pause");
        #endif
        return 0;
    }
    
    FILE *in_file  = fopen(argv[1], "r");
    FILE *out_file = fopen(argv[2], "wb");
    if (in_file == nullptr || out_file == nullptr) 
    {
        printf ("One or more incorrect files.\n");

        fcloseall();
        in_file = nullptr;
        out_file = nullptr;

        #ifdef _DEBUG
            system ("pause");
        #endif
        return 0;
    }

    printf ("Welcome to %s assmebler.\n", HEADER);
    printf ("\nInput is %s, output is %s\n\n", argv[1], argv[2]);

    char *str;
    char command[command_length] = {0};
    double value = 0;
    bool success = false;
    char label[label_length] = {0};
    char reg[reg_length] = {0};
    size_t labels_count = 0;
    size_t values_count = 0;
    size_t commands_count = 0;

    first_pass (in_file, &values_count, &labels_count, &commands_count);

    printf ("\nValues count: %d\n", values_count);
    printf ("Labels count: %d\n", labels_count);
    printf ("Commands count: %d\n", commands_count);

    double *values = (double *) calloc(values_count, sizeof(*values));
    label_t *labels = (label_t *) calloc(labels_count, sizeof(*labels));

    second_pass(in_file, values_count,labels_count, commands_count, values, labels);

    printf ("\nValues table:\n");
    for (int i = 0; i < values_count; i++)
    {
        printf ("\t[%d] %lf\n", i, values[i]);
    }

    printf ("\nLabels table:\n");
    for (int i = 0; i < labels_count; i++)
    {
        printf ("\t[%d] %s -> %d\n", i, labels[i].name, labels[i].address); 
    }

    write_header(out_file, values_count, commands_count, HEADER);
    printf ("\nHeader {%s %d %d} is wrotten.\n\n", HEADER, values_count, commands_count);
    write_values_table(out_file, values_count, values);
    third_pass(in_file, out_file, values_count, labels_count, commands_count, values, labels);

    printf ("\nSize of %s is %d bytes.\n", argv[2], file_length(out_file));
    fcloseall();
    in_file = nullptr;
    out_file = nullptr;

    #ifdef _DEBUG
        system ("pause");
    #endif
    return 0;
}

bool first_pass(FILE *in_file, size_t *values_count, size_t *labels_count, size_t *commands_count)
{
    if ((in_file == nullptr) || (values_count == nullptr) || (labels_count == nullptr) || (commands_count == nullptr)) return false;

    fseek(in_file, 0,  SEEK_SET);

    bool success = false;
    double value = 0;
    char label[label_length + 1] = {0};
    char reg[reg_length + 1] = {0};
    char command[command_length + 1] = {0};

    while(!feof (in_file))
    {
        success = fscanf (in_file, "%lf", &value);
        if (success)
        {
            printf ("value   |  %lf\n", value);
            (*values_count)++;
        }
        else
        {
            success = fscanf (in_file, ":%s", &label);
            if (success)
            {
                printf ("lbl_to  |  %s\n", label);
                (*labels_count)++;
            }
            else
            {
                success = fscanf (in_file, "_%s", &label);
                if (success)
                {
                    printf ("lbl_from|  %s\n", label);
                }
                else
                {
                    success = read_reg (in_file, reg);
                    if (success)
                    {
                        printf ("register|  %s\n", reg);
                    }
                    else
                    {
                        success = fscanf (in_file, "%s", &command);
                        if (success)
                        {
                            printf ("command |  %s\n", command);
                            (*commands_count)++;
                        }
                        else
                        {
                            printf ("Syntax error.\n");
                            break;
                            return false;
                        }
                    }
                }
            }
        }
    }

    fseek(in_file, 0,  SEEK_SET);
    return true;
}

bool second_pass(FILE *in_file, size_t values_count, size_t labels_count, size_t commands_count, double *values, label_t *labels)
{
    if ((in_file == nullptr) || (values == nullptr) || (labels == nullptr)) return false;

    fseek(in_file, 0,  SEEK_SET);

    bool success = false;
    double value = 0;
    size_t recorded_values_count = 0;
    size_t recorded_labels_count = 0;
    size_t recorded_commands_count = 0;
    char label[label_length + 1] = {0};
    char reg[reg_length + 1] = {0};
    char command[command_length + 1] = {0};

    while(!feof (in_file))
    {
        success = fscanf (in_file, "%lf", &(values[recorded_values_count]));
        if (success)
        {
            recorded_values_count++;
        }
        else
        {
            success = fscanf (in_file, ":%s", &(labels[recorded_labels_count].name));
            if (success)
            {
                labels[recorded_labels_count].address = recorded_commands_count;
                recorded_labels_count++;
            }
            else
            {
                success = fscanf (in_file, "_%s", &label);
                if (success)
                {
                    //printf ("lbl_from|  %s\n", label);
                }
                else
                {
                    success = read_reg (in_file, reg);
                    if (success)
                    {
                        //printf ("register|  %s\n", reg);
                    }
                    else
                    {
                        success = fscanf (in_file, "%s", &command);
                        if (success)
                        {
                            //printf ("command |  %s\n", command);
                            recorded_commands_count++;
                        }
                        else
                        {
                            //printf ("Syntax error.\n");
                            break;
                            return false;
                        }
                    }
                }
            }
        }
    }

    fseek(in_file, 0,  SEEK_SET);
    return ((recorded_labels_count == labels_count) && (recorded_values_count == values_count));
}

bool third_pass(FILE *in_file, FILE *out_file, size_t values_count, size_t labels_count, size_t commands_count, double *values, label_t *labels)
{
    if ((in_file == nullptr) || (values == nullptr) || (labels == nullptr)) return false;

    fseek(in_file, 0,  SEEK_SET);

    bool success = false;
    double value = 0;
    size_t recorded_values_count = 0;
    size_t recorded_labels_count = 0;
    size_t recorded_commands_count = 0;
    char label[label_length + 1] = {0};
    char reg[reg_length + 1] = {0};
    char command[command_length + 1] = {0};
    int16_t arg1 = NO_CMD, arg2 = NO_CMD, arg3 = NO_CMD;

    while(!feof (in_file))
    {
        success = fscanf (in_file, "%lf", &value);
        if (success)
        {
            printf ("v\n");
        }
        else
        {
            success = fscanf (in_file, ":%s", &label);
            if (success)
            {
                ;
            }
            else
            {
                success = false;
                if (success)
                {
                    ;
                }
                else
                {
                    success = read_reg (in_file, reg);
                    if (success)
                    {
                        printf ("r\n");
                    }
                    else
                    {
                        success = fscanf (in_file, "%4s", &command);
                        if (success)
                        {    
                            if (false)
                            {

                            }
                            #define DEF_CMD(cmd, num, code, name, args)                                         \
                            else if (strcmp (name, command) == 0)                                               \
                            {                                                                                   \
                                printf ("%5s (#%3d) ", name, num);                                              \
                                switch (args)                                                                   \
                                {                                                                               \
                                    case 0:                                                                     \
                                        write_command (out_file, num, NO_CMD, NO_CMD, NO_CMD);                  \
                                        printf ("%5d\n", num);                                                  \
                                        break;                                                                  \
                                    case 1:                                                                     \
                                        if (IS_JUMP(num))                                                       \
                                        {                                                                       \
                                            success = fscanf (in_file, " _%s", &label);                         \
                                            arg1 = find_label (labels_count, labels, label);                    \
                                        }                                                                       \
                                        else                                                                    \
                                        {                                                                       \
                                            success = fscanf (in_file, "%lf", &value);                          \
                                            if (success)                                                        \
                                            {                                                                   \
                                                arg1 = find_value (values_count, values, value);                \
                                            }                                                                   \
                                            else                                                                \
                                            {                                                                   \
                                                success = read_reg (in_file, reg);                              \
                                                if (success)                                                    \
                                                {                                                               \
                                                    arg1 = -(reg[0] - 96);                                      \
                                                }                                                               \
                                                else                                                            \
                                                {                                                               \
                                                    return false;                                               \
                                                }                                                               \
                                            }                                                                   \
                                        }                                                                       \
                                        if (arg1 == NO_CMD) return false;                                       \
                                        write_command (out_file, num, arg1, NO_CMD, NO_CMD);                    \
                                        printf ("%5d %5d\n", num, arg1);                                        \
                                        break;                                                                  \
                                    case 2:                                                                     \
                                        /*printf("!%d", args);*/                                                \
                                        success = fscanf (in_file, "%lf", &value);                              \
                                        if (success)                                                            \
                                        {                                                                       \
                                            arg1 = find_value (values_count, values, value);                    \
                                            success = fscanf (in_file, "%lf", &value);                          \
                                            if (success)                                                        \
                                            {                                                                   \
                                                arg2 = find_value (values_count, values, value);                \
                                            }                                                                   \
                                            else                                                                \
                                            {                                                                   \
                                                success = read_reg (in_file, reg);                              \
                                                if (success)                                                    \
                                                {                                                               \
                                                    arg2 = -(reg[0] - 96);                                      \
                                                }                                                               \
                                                else                                                            \
                                                {                                                               \
                                                    return false;                                               \
                                                }                                                               \
                                            }                                                                   \
                                        }                                                                       \
                                        else                                                                    \
                                        {                                                                       \
                                            success = read_reg (in_file, reg);                                  \
                                            if (success)                                                        \
                                            {                                                                   \
                                                arg1 = -(reg[0] - 96);                                          \
                                                success = fscanf (in_file, "%lf", &value);                      \
                                                if (success)                                                    \
                                                {                                                               \
                                                    arg2 = find_value (values_count, values, value);            \
                                                }                                                               \
                                                else                                                            \
                                                {                                                               \
                                                    success = read_reg (in_file, reg);                          \
                                                    if (success)                                                \
                                                    {                                                           \
                                                        arg2 = -(reg[0] - 96);                                  \
                                                    }                                                           \
                                                    else                                                        \
                                                    {                                                           \
                                                        return false;                                           \
                                                    }                                                           \
                                                }                                                               \
                                            }                                                                   \
                                            else                                                                \
                                            {                                                                   \
                                                success = fscanf (in_file, "%lf", &value);                      \
                                                if (success)                                                    \
                                                {                                                               \
                                                    arg2 = find_value (values_count, values, value);            \
                                                }                                                               \
                                                else                                                            \
                                                {                                                               \
                                                    success = read_reg (in_file, reg);                          \
                                                    if (success)                                                \
                                                    {                                                           \
                                                        arg2 = -(reg[0] - 96);                                  \
                                                    }                                                           \
                                                    else                                                        \
                                                    {                                                           \
                                                        return false;                                           \
                                                    }                                                           \
                                                }                                                               \
                                            }                                                                   \
                                        }                                                                       \
                                        if ((arg1 == NO_CMD) || (arg2 == NO_CMD)) return false;                 \
                                        write_command (out_file, num, arg1, arg2, NO_CMD);                      \
                                        printf ("%5d %5d %5d\n", num, arg1, arg2);                              \
                                    default:                                                                    \
                                        /*printf ("!%d", args);*/                                               \
                                        break;                                                                  \
                                }                                                                               \
                            }                                                                                   
                            #include "..\include\commands.h"
                            #undef DEF_CMD
                        }
                        else
                        {
                            printf ("Invalid or unsupported command.\n");
                            return false;
                        }
                    }
                }
            }
        }
    }

    fseek(in_file, 0,  SEEK_SET);
    return true;
}

size_t write_header(FILE *out_file, size_t values_count, size_t commands_count, char *header)
{
    if ((out_file == nullptr) || (header == nullptr)) return -1;
    return fwrite ((void *)header, sizeof (char), 8, out_file) + 
        fwrite ((void *)(&values_count), sizeof(size_t), 1, out_file) +
        fwrite ((void *)(&commands_count), sizeof(size_t), 1, out_file);
}

size_t write_values_table(FILE *out_file, size_t values_count, double *values)
{
    if ((out_file == nullptr) || (values == nullptr)) return -1;
    return fwrite ((void *)values, sizeof(double), values_count, out_file);
}

size_t write_command(FILE *out_file, uint16_t command_number, int16_t arg1, int16_t arg2, int16_t arg3)
{
    if (out_file == nullptr) return -1;
    return fwrite ((void *)(&command_number), sizeof (uint16_t), 1, out_file) + 
        fwrite ((void *)(&arg1), sizeof (int16_t), 1, out_file) +
        fwrite ((void *)(&arg2), sizeof (int16_t), 1, out_file) + 
        fwrite ((void *)(&arg3), sizeof (int16_t), 1, out_file);
}

int16_t find_value(size_t values_count, double *values, double value)
{
    if (values == nullptr) return -1;
    int16_t res = NO_CMD;
    for (uint16_t i = 0; i < values_count; i++)                             
    {                                                                  
        if (values[i] == value)                                        
        {                                                              
            res = i;                                                  
            break;                                                     
        }                                                              
    }
    return res;
}

int16_t find_label(size_t labels_count, label_t *labels, char *label_name)
{
    if ((labels == nullptr) || (label_name == nullptr)) return -1;
    int16_t res = NO_CMD;
    for (int16_t i = 0; i < labels_count; i++)                             
    {                                                                  
        if (strcmp (labels[i].name, label_name) == 0)                                        
        {                                                              
            res = labels[i].address;                                                  
            break;                                                     
        }                                                              
    }
    return res;
}

unsigned long int file_length(FILE *file)
{
    fseek (file, 0, SEEK_END);
    unsigned long int length = ftell (file);
    fseek (file, 0, SEEK_SET);
    return length;
}

bool read_reg(FILE *in_file, char *reg)
{
    bool success = false; 
    success = fscanf (in_file, "%1[a-d]", &(reg[0]));
    if (!success) return false;
    success = fscanf (in_file, "%1[x]", &(reg[1]));
    if (!success)
    {
        ungetc(reg[0], in_file);
        return false;
    }
    return true;
}