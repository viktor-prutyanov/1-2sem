/**
*   Elf class for sl 
*
*   @file Program.h
*
*   @date 03.2015
*
*   @copyright GNU GPL v2.0
*
*   @author Viktor Prutyanov mailto:vitteran@gmail.com
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <stdexcept>

#include <unistd.h>
#include <elf.h>

#define SHSTRTAB_NAME   '.','s','h','s','t','r','t','a','b', 0
#define STRTAB_NAME     '.','s','t','r','t','a','b', 0        
#define SYMTAB_NAME     '.','s','y','m','t','a','b', 0        
#define TEXT_NAME       '.','t','e','x','t', 0                

#define ASM_SRC_NAME    "src.asm"

#define BASE_LBL_NAME   "label00"

#define ESS_LBLS        "_start\0__bss_start\0_edata\0_end"

#define ESS_LBLS_OFF    (sizeof('\0') + sizeof(ASM_SRC_NAME) + sizeof(BASE_LBL_NAME) * jumps_num)

#define BASE_STRTAB_LEN (sizeof('0') + sizeof(ESS_LBLS) + sizeof(ASM_SRC_NAME))

#define BASE_VIRT_ADDR  0x400000

// #define START_LABEL     "_start"
// #define BSS_START_LABEL "__bss_start"
// #define EDATA_LABEL     "_edata"
// #define END_LABEL       "_end"


#define TEXT_OFF        (sizeof(Elf64_Ehdr) + sizeof(Elf64_Phdr) + 8) //0x80

// #define SHSTRTAB_NAME      ".shstrtab"       
// #define STRTAB_NAME        ".strtab"          
// #define SYMTAB_NAME        ".symtab"         
// #define TEXT_NAME          ".text"                 

#define SHSTRTAB_LEN    0x21

#define SHDRS_NUM       5

#define SYMTABS_NUM     8 + jumps_num

#define EXIT_SRC        "\xb8\x3c\x00\x00\x00\xbf\x00\x00\x00\x00\x0f\x05"
#define EXIT_SRC_LEN    12

class Elf
{
public:
    Elf(FILE *objFile);
    ~Elf();

    void Dump(FILE *dumpFile);
    void Link();
    void Store(FILE *elfFile);

private:
    Elf& operator=(const Elf& other);

    size_t text_size;
    size_t jumps_num;
    uint8_t *text;
    size_t *jump_ptrs;
    size_t jump_ptrs_base;

    Elf64_Ehdr ehdr;
    Elf64_Phdr phdr;
    Elf64_Shdr shdrs[SHDRS_NUM];

    uint8_t shstrtab[SHSTRTAB_LEN] = {0, SYMTAB_NAME, STRTAB_NAME, SHSTRTAB_NAME, TEXT_NAME};
    Elf64_Sym *symtabs;
    char *strtab;
    size_t strtab_size;
    size_t start_off;
    size_t end_off;
};

Elf::Elf(FILE *objFile)
   :text_size (0),
    jumps_num (0),
    text (nullptr),
    jump_ptrs (0),
    jump_ptrs_base (0),
    ehdr ({}),
    phdr ({}),
    shdrs ({}),
    symtabs (nullptr),
    strtab (nullptr),
    strtab_size (0),
    start_off (0),
    end_off (0)
{
    if (objFile == nullptr) throw std::invalid_argument("Null pointer as input file.");

    fread(&text_size, 1, sizeof(size_t), objFile);
    fread(&jumps_num, 1, sizeof(size_t), objFile);

    text = new uint8_t[text_size];
    jump_ptrs = new size_t[jumps_num];

    fread(text, text_size, sizeof(uint8_t), objFile);
    fread(jump_ptrs, jumps_num, sizeof(size_t), objFile);

    fread(&jump_ptrs_base, 1, sizeof(size_t), objFile);

    fread(&start_off, 1, sizeof(size_t), objFile);
    fread(&end_off, 1, sizeof(size_t), objFile);

    memcpy(text + end_off, EXIT_SRC, EXIT_SRC_LEN);

    for (size_t i = 0; i < jumps_num; ++i)
    {
        jump_ptrs[i] -= jump_ptrs_base;
    }

    symtabs = new Elf64_Sym[SYMTABS_NUM];

    strtab_size = BASE_STRTAB_LEN + jumps_num * sizeof(BASE_LBL_NAME);
    strtab = new char[strtab_size];
    //TODO: memset
}   

Elf::~Elf()
{
    delete[] text;
    delete[] jump_ptrs;
    delete[] symtabs;
    delete[] strtab;
}

void Elf::Dump(FILE *dumpFile)
{
    if (dumpFile == nullptr) throw std::invalid_argument("Null pointer as dump file.");
    
    fprintf(dumpFile, "Size of .text is %lu bytes.\n", text_size);
    for (size_t i = 0; i < text_size; ++i)
    {
        fprintf(dumpFile, "%.2x ", text[i]);
    }

    fprintf(dumpFile, "\n\nAmount of jumps/calls is %lu.\n", jumps_num);
    for (size_t i = 0; i < jumps_num; ++i)
    {
        fprintf(dumpFile, "%2lu) 0x%.8lx\n", i, jump_ptrs[i]);
    }

    fprintf(dumpFile, "\nstrtab at [%p] (%lu) ($ instead of \\0):\n", strtab, strtab_size);
    for (char *p = strtab; p < strtab + strtab_size; ++p)
    {
        fprintf(dumpFile, "%c", (*p != 0) ? *p : '$');
    }
}

void Elf::Link()
{
    /* ELF header */
    memcpy(&(ehdr.e_ident), ELFMAG, SELFMAG);       // "\x7fELF"
    ehdr.e_ident[EI_CLASS]      = ELFCLASS64;       // 0x02
    ehdr.e_ident[EI_DATA]       = ELFDATA2LSB;      // 0x01
    ehdr.e_ident[EI_VERSION]    = EV_CURRENT;       // 0x01
    ehdr.e_ident[EI_OSABI]      = ELFOSABI_SYSV;    // 0x00
    ehdr.e_ident[EI_ABIVERSION] = 0;
    ehdr.e_ident[EI_PAD]        = 0;
    memset(&(ehdr.e_ident[10]), 0, 8);              // 0x00 * 0x08
    ehdr.e_type      = ET_EXEC;                     // 0x02     
    ehdr.e_machine   = EM_X86_64;                   // 0x3e  
    ehdr.e_version   = EV_CURRENT;                  // 0x01  
    ehdr.e_entry     = BASE_VIRT_ADDR + TEXT_OFF + start_off;   // 0x400080   
    ehdr.e_phoff     = sizeof(Elf64_Ehdr);          // 0x40    
    ehdr.e_shoff     = TEXT_OFF + text_size + SHSTRTAB_LEN + 1; //0xa2 + text_size
    ehdr.e_flags     = 0x00;   
    ehdr.e_ehsize    = sizeof(Elf64_Ehdr);          // 0x40 
    ehdr.e_phentsize = sizeof(Elf64_Phdr);          // 0x38
    ehdr.e_phnum     = 1;    
    ehdr.e_shentsize = sizeof(Elf64_Shdr);          // 0x40
    ehdr.e_shnum     = SHDRS_NUM;   
    ehdr.e_shstrndx  = 2;

    /* Program header */
    phdr.p_type     = PT_LOAD;                      // 0x01
    phdr.p_flags    = PF_X | PF_R;                  // 0x05
    phdr.p_offset   = 0;
    phdr.p_vaddr    = BASE_VIRT_ADDR;               // 0x400000
    phdr.p_paddr    = BASE_VIRT_ADDR;               // 0x400000
    phdr.p_filesz   = TEXT_OFF + text_size;         // text_size + 0x80 
    phdr.p_memsz    = TEXT_OFF + text_size;         // text_size + 0x80
    phdr.p_align    = 0x200000;

    /* 8 null bytes */ 
    /* Section .text */
    /* Section .shstrtab */
    /* 1 null byte */

    /* Section 0: null */
    memset(&shdrs[0], SHN_UNDEF, sizeof(Elf64_Shdr));               // 0x00 * 0x40
    /* Section 1: .text */
    shdrs[1].sh_name      = 0x1b;  
    shdrs[1].sh_type      = SHT_PROGBITS;                           // 1
    shdrs[1].sh_flags     = SHF_ALLOC | SHF_EXECINSTR;              // 0x06
    shdrs[1].sh_addr      = BASE_VIRT_ADDR + TEXT_OFF;              // 0x400080
    shdrs[1].sh_offset    = TEXT_OFF;                               // 0x80
    shdrs[1].sh_size      = text_size;  
    shdrs[1].sh_link      = 0;  
    shdrs[1].sh_info      = 0;  
    shdrs[1].sh_addralign = 16;
    shdrs[1].sh_entsize   = 0; 
    /* Section 2: .shstrtab */
    shdrs[2].sh_name      = 0x11;
    shdrs[2].sh_type      = SHT_STRTAB;                             // 3
    shdrs[2].sh_flags     = 0x00;
    shdrs[2].sh_addr      = 0;
    shdrs[2].sh_offset    = TEXT_OFF + text_size;                   // 0x80 + text_size
    shdrs[2].sh_size      = SHSTRTAB_LEN;                           // 0x21
    shdrs[2].sh_link      = 0;
    shdrs[2].sh_info      = 0;
    shdrs[2].sh_addralign = 1;
    shdrs[2].sh_entsize   = 0;
    /* Section 3: .symtab */
    shdrs[3].sh_name      = 0x01; 
    shdrs[3].sh_type      = SHT_SYMTAB;                             // 2
    shdrs[3].sh_flags     = 0x00;
    shdrs[3].sh_addr      = 0;
    shdrs[3].sh_offset    = ehdr.e_shoff + SHDRS_NUM * sizeof(Elf64_Shdr);
    shdrs[3].sh_size      = (jumps_num + 8) * sizeof(Elf64_Sym);
    shdrs[3].sh_link      = 4;
    shdrs[3].sh_info      = 4 + jumps_num;
    shdrs[3].sh_addralign = 8;
    shdrs[3].sh_entsize   = 0x18;
    /* Section 4: .strtab */
    shdrs[4].sh_name      = 0x09;
    shdrs[4].sh_type      = SHT_STRTAB;                             // 3
    shdrs[4].sh_flags     = 0x00;
    shdrs[4].sh_addr      = 0;
    shdrs[4].sh_offset    = shdrs[3].sh_offset + shdrs[3].sh_size;
    shdrs[4].sh_size      = strtab_size;
    shdrs[4].sh_link      = 0;
    shdrs[4].sh_info      = 0;
    shdrs[4].sh_addralign = 1;
    shdrs[4].sh_entsize   = 0;
    
    /* Section .strtab */
    strtab[0] = '\0';
    memcpy(strtab + 1, ASM_SRC_NAME, sizeof(ASM_SRC_NAME));
    char *cur_str_ptr = strtab + 1 + sizeof(ASM_SRC_NAME);
    for (size_t i = 0; i < jumps_num; ++i)
    {
        memcpy(cur_str_ptr, BASE_LBL_NAME, sizeof(BASE_LBL_NAME));
        cur_str_ptr += sizeof(BASE_LBL_NAME);
        sprintf(cur_str_ptr - sizeof("00"), "%.2d", i);
    }
    memcpy(cur_str_ptr, ESS_LBLS, sizeof(ESS_LBLS));
    
    /* Section .symtab */
    /* 0:             0000000000000000      0 NOTYPE  LOCAL  DEFAULT  UND             */
    symtabs[1].st_name  = STN_UNDEF;  
    symtabs[1].st_info  = ELF64_ST_INFO(STB_LOCAL, STT_NOTYPE);
    symtabs[1].st_other = STV_DEFAULT;
    symtabs[1].st_shndx = SHN_UNDEF;
    symtabs[1].st_value = 0;
    symtabs[1].st_size  = 0;
    /* 1:             0000000000400080      0 SECTION LOCAL  DEFAULT    1             */
    symtabs[1].st_name  = 0;
    symtabs[1].st_info  = ELF64_ST_INFO(STB_LOCAL, STT_SECTION);
    symtabs[1].st_other = STV_DEFAULT;
    symtabs[1].st_shndx = 1;
    symtabs[1].st_value = BASE_VIRT_ADDR + TEXT_OFF;   
    symtabs[1].st_size  = 0;
    /* 2:             0000000000000000      0 FILE    LOCAL  DEFAULT  ABS src.asm     */
    symtabs[2].st_name  = 1;
    symtabs[2].st_info  = ELF64_ST_INFO(STB_LOCAL, STT_FILE);
    symtabs[2].st_other = STV_DEFAULT;
    symtabs[2].st_shndx = SHN_ABS;
    symtabs[2].st_value = 0;   
    symtabs[2].st_size  = 0;
    /* 3 + i:         400080 + jump_ptrs[i] 0 NOTYPE  LOCAL  DEFAULT    1 label<i>    */
    for (size_t i = 0; i < jumps_num; ++i)
    {
        symtabs[3 + i].st_name  = 1 + sizeof(ASM_SRC_NAME) + i * sizeof(BASE_LBL_NAME);
        symtabs[3 + i].st_info  = ELF64_ST_INFO(STB_LOCAL, STT_NOTYPE);
        symtabs[3 + i].st_other = STV_DEFAULT;
        symtabs[3 + i].st_shndx = 1;
        symtabs[3 + i].st_value = BASE_VIRT_ADDR + TEXT_OFF + jump_ptrs[i];   
        symtabs[3 + i].st_size  = 0;
    }
    /* 3 + jumps_num: 0000000000000000      0 FILE    LOCAL  DEFAULT  ABS             */
    symtabs[3 + jumps_num].st_name  = STN_UNDEF;
    symtabs[3 + jumps_num].st_info  = ELF64_ST_INFO(STB_LOCAL, STT_FILE);
    symtabs[3 + jumps_num].st_other = STV_DEFAULT;
    symtabs[3 + jumps_num].st_shndx = SHN_ABS;
    symtabs[3 + jumps_num].st_value = 0;   
    symtabs[3 + jumps_num].st_size  = 0;
    /* 4 + jumps_num: 0000000000400080      0 NOTYPE  GLOBAL DEFAULT    1 _start      */
    symtabs[4 + jumps_num].st_name  = ESS_LBLS_OFF;
    symtabs[4 + jumps_num].st_info  = ELF64_ST_INFO(STB_GLOBAL, STT_NOTYPE);
    symtabs[4 + jumps_num].st_other = STV_DEFAULT;
    symtabs[4 + jumps_num].st_shndx = 1;
    symtabs[4 + jumps_num].st_value = BASE_VIRT_ADDR + TEXT_OFF + start_off;   
    symtabs[4 + jumps_num].st_size  = 0;
    /* 5 + jumps_num: 600000 + phdr.p_memsz 0 NOTYPE  GLOBAL DEFAULT    1 __bss_start */
    symtabs[5 + jumps_num].st_name  = ESS_LBLS_OFF + sizeof("_start");
    symtabs[5 + jumps_num].st_info  = ELF64_ST_INFO(STB_GLOBAL, STT_NOTYPE);
    symtabs[5 + jumps_num].st_other = STV_DEFAULT;
    symtabs[5 + jumps_num].st_shndx = 1;
    symtabs[5 + jumps_num].st_value = 0x600000 + phdr.p_memsz;   
    symtabs[5 + jumps_num].st_size  = 0;
    /* 6 + jumps_num: 600000 + phdr.p_memsz 0 NOTYPE  GLOBAL DEFAULT    1 _edata      */
    symtabs[6 + jumps_num].st_name  = ESS_LBLS_OFF + sizeof("_start\0__bss_start");
    symtabs[6 + jumps_num].st_info  = ELF64_ST_INFO(STB_GLOBAL, STT_NOTYPE);
    symtabs[6 + jumps_num].st_other = STV_DEFAULT;
    symtabs[6 + jumps_num].st_shndx = 1;
    symtabs[6 + jumps_num].st_value = 0x600000 + phdr.p_memsz;   
    symtabs[6 + jumps_num].st_size  = 0;
    /* 7 + jumps_num: 600000 + phdr.p_memsz 0 NOTYPE  GLOBAL DEFAULT    1 _end        */     
    symtabs[7 + jumps_num].st_name  = ESS_LBLS_OFF + sizeof("_start\0__bss_start\0_edata");
    symtabs[7 + jumps_num].st_info  = ELF64_ST_INFO(STB_GLOBAL, STT_NOTYPE);
    symtabs[7 + jumps_num].st_other = STV_DEFAULT;
    symtabs[7 + jumps_num].st_shndx = 1;
    symtabs[7 + jumps_num].st_value = 0x600000 + ((phdr.p_memsz % 8 == 0) ? phdr.p_memsz : (phdr.p_memsz - (phdr.p_memsz % 8) + 8));  
    symtabs[7 + jumps_num].st_size  = 0;
}

void Elf::Store(FILE *elfFile)
{
    if (elfFile == nullptr) throw std::invalid_argument("Null pointer as elf file.");

    uint8_t padding[8] = {0, 0, 0, 0, 0, 0, 0, 0};

    fwrite(&ehdr, sizeof(Elf64_Ehdr), 1, elfFile);
    fwrite(&phdr, sizeof(Elf64_Phdr), 1, elfFile);
    fwrite(padding, sizeof(uint8_t), 8, elfFile);
    fwrite(text, text_size, 1, elfFile);
    fwrite(shstrtab, SHSTRTAB_LEN, 1, elfFile);
    fwrite(padding, sizeof(uint8_t), 1, elfFile);
    fwrite(shdrs, sizeof(Elf64_Shdr), SHDRS_NUM, elfFile);
    fwrite(symtabs, sizeof(Elf64_Sym), SYMTABS_NUM, elfFile);
    fwrite(strtab, sizeof(char), strtab_size, elfFile);
}