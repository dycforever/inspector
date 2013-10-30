#ifndef __HEADER_H__
#define __HEADER_H__

#include <iostream>
#include <libelf.h>
#include <gelf.h>

#include "common.h"
#include "ElfTypes.h"

//      typedef struct
//      {
//        unsigned char	e_ident[EI_NIDENT];	/* Magic number and other info */
//        Elf32_Half	e_type;			/* Object file type */
//        Elf32_Half	e_machine;		/* Architecture */
//        Elf32_Word	e_version;		/* Object file version */
//        Elf32_Addr	e_entry;		/* Entry point virtual address */
//        Elf32_Off	e_phoff;		/* Program header table file offset */
//        Elf32_Off	e_shoff;		/* Section header table file offset */
//        Elf32_Word	e_flags;		/* Processor-specific flags */
//        Elf32_Half	e_ehsize;		/* ELF header size in bytes */
//        Elf32_Half	e_phentsize;		/* Program header table entry size */
//        Elf32_Half	e_phnum;		/* Program header table entry count */
//        Elf32_Half	e_shentsize;		/* Section header table entry size */
//        Elf32_Half	e_shnum;		/* Section header table entry count */
//        Elf32_Half	e_shstrndx;		/* Section header string table index */
//      } Elf32_Ehdr;
//      
//
//
//      typedef struct
//      {
//        unsigned char	e_ident[EI_NIDENT];	/* Magic number and other info */
//        Elf64_Half	e_type;			/* Object file type */
//        Elf64_Half	e_machine;		/* Architecture */
//        Elf64_Word	e_version;		/* Object file version */
//        Elf64_Addr	e_entry;		/* Entry point virtual address */
//        Elf64_Off	e_phoff;		/* Program header table file offset */
//        Elf64_Off	e_shoff;		/* Section header table file offset */
//        Elf64_Word	e_flags;		/* Processor-specific flags */
//        Elf64_Half	e_ehsize;		/* ELF header size in bytes */
//        Elf64_Half	e_phentsize;		/* Program header table entry size */
//        Elf64_Half	e_phnum;		/* Program header table entry count */
//        Elf64_Half	e_shentsize;		/* Section header table entry size */
//        Elf64_Half	e_shnum;		/* Section header table entry count */
//        Elf64_Half	e_shstrndx;		/* Section header string table index */
//      } Elf64_Ehdr;
//
//

template <class ELF_EHDR>
class Header {
public:

    Header(ELF_EHDR* header);
    Elf_Half getType() {return _header->e_type;}
    std::string getTypeStr();

    Elf_Half getMachine(){return _header->e_machine;}
    std::string getMachineStr();

    Elf_Word getVersion(){return _header->e_version;}

    Elf_Addr getEntry(){return _header->e_entry;}
    Elf_Off getPhoff(){return _header->e_phoff;}
    Elf_Off getShoff(){return _header->e_shoff;}

    Elf_Word getFlags(){return _header->e_flags;}
    std::string getFlagsStr();

    Elf_Half getEhsize(){return _header->e_ehsize;}
    Elf_Half getPhentsize(){return _header->e_phentsize;}
    Elf_Half getPhnum(){return _header->e_phnum;}	 
    Elf_Half getShentsize(){return _header->e_shentsize;}
    Elf_Half getShnum(){return _header->e_shnum;}	 
    Elf_Half getShstrndx(){return _header->e_shstrndx;} 
    void showInfo();
    void parseMagic();

    int check(int fd);

private:
    ELF_EHDR* _header;
};

template <class ELF_EHDR>
int Header<ELF_EHDR>::check(int fd) {
//    lseek(fd, 0, SEEK_SET);
    ELF_EHDR head;
    read(fd, (char*)&head, sizeof(head));
    return memcmp(_header, (char*)&head, sizeof(head));
}

template <class ELF_EHDR>
Header<ELF_EHDR>::Header(ELF_EHDR* header) {
    _header = header;
}

template <class ELF_EHDR>
std::string Header<ELF_EHDR>::getTypeStr() {
    if (_header->e_type == ET_REL)
        return "[rel/obj file]";
    if (_header->e_type == ET_EXEC)
        return "[exec file]";
    if (_header->e_type == ET_DYN)
        return "[dynamic file]";
    return "unknow file type";
}

template <class ELF_EHDR>
void Header<ELF_EHDR>::parseMagic() {
    unsigned char* bitp = _header->e_ident+4;
    if (*bitp == 1) {
        std::cout << "32 bit  ";
    } else {
        std::cout << "64 bit  ";
    }
    unsigned char* endian = bitp + 1;
    if (*endian == 1) {
        std::cout << "little endian";
    } else if (*endian == 2){
        std::cout << "big endian";
    } else {
        std::cout << "invalid endian";
    }
    std::cout << std::endl;
}

template <class ELF_EHDR>
void Header<ELF_EHDR>::showInfo() {
    parseMagic();
    std::cout << "type: " << getType() << "  " << getTypeStr() << std::endl 
         << "machine: " << getMachine() << "  " << getMachineStr() << std::endl 
         << "version: " << getVersion() << std::endl 
         << "entry: " << std::hex << "0x" << getEntry() << std::endl 
         << "phoff: " << std::dec << getPhoff() << " (" << std::hex << "0x" << getPhoff() << ")" << std::endl 
         << "shoff: " << std::dec << getShoff() << " (" << std::hex << "0x" << getShoff() << ")" << std::endl 
         << "flags: " << getFlags() << std::endl 
         << "ehsize: " << std::dec << getEhsize() << "   while sizeof(_header):" << sizeof(*_header) << std::endl;
}

template <class ELF_EHDR>
std::string Header<ELF_EHDR>::getMachineStr() {
    if (_header->e_machine == EM_386)
        return "[0x86]";
    else 
        return "unknow machine type";
}



#endif //__HEADER_H__
