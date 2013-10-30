
#ifndef ELFTYPES_H
#define ELFTYPES_H

#ifdef __x86_64__
    typedef  Elf64_Ehdr Elf_Ehdr;

    typedef  Elf64_Half Elf_Half;
    typedef  Elf64_Word Elf_Word;
    typedef  Elf64_Addr Elf_Addr;
    typedef  Elf64_Off Elf_Off;
#else
    typedef  Elf32_Ehdr Elf_Ehdr;

    typedef  Elf32_Half Elf_Half;
    typedef  Elf32_Word Elf_Word;
    typedef  Elf32_Addr Elf_Addr;
    typedef  Elf32_Off Elf_Off;
#endif



#endif

