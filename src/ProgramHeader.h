#ifndef __PROGRAMHEADER_H__
#define __PROGRAMHEADER_H__

#include <iostream>
#include <libelf.h>
#include <gelf.h>


/* Program segment header.  */
//    typedef struct
//    {
//      Elf32_Word	p_type;			/* Segment type */
//      Elf32_Off	p_offset;		/* Segment file offset */
//      Elf32_Addr	p_vaddr;		/* Segment virtual address */
//      Elf32_Addr	p_paddr;		/* Segment physical address */
//      Elf32_Word	p_filesz;		/* Segment size in file */
//      Elf32_Word	p_memsz;		/* Segment size in memory */
//      Elf32_Word	p_flags;		/* Segment flags */
//      Elf32_Word	p_align;		/* Segment alignment */
//    } Elf32_Phdr;
//
// typedef struct
// {
//   Elf64_Word	p_type;			/* Segment type */
//   Elf64_Word	p_flags;		/* Segment flags */
//   Elf64_Off	p_offset;		/* Segment file offset */
//   Elf64_Addr	p_vaddr;		/* Segment virtual address */
//   Elf64_Addr	p_paddr;		/* Segment physical address */
//   Elf64_Xword	p_filesz;		/* Segment size in file */
//   Elf64_Xword	p_memsz;		/* Segment size in memory */
//   Elf64_Xword	p_align;		/* Segment alignment */
// } Elf64_Phdr;

/* Special value for e_phnum.  This indicates that the real number of
   program headers is too large to fit into e_phnum.  Instead the real
   value is in the field sh_info of section 0.  */

#define PN_XNUM		0xffff

/* Legal values for p_type (segment type).  */

#define	PT_NULL		0		/* Program header table entry unused */
#define PT_LOAD		1		/* Loadable program segment */
#define PT_DYNAMIC	2		/* Dynamic linking information */
#define PT_INTERP	3		/* Program interpreter */
#define PT_NOTE		4		/* Auxiliary information */
#define PT_SHLIB	5		/* Reserved */
#define PT_PHDR		6		/* Entry for header table itself */
#define PT_TLS		7		/* Thread-local storage segment */
#define	PT_NUM		8		/* Number of defined types */
#define PT_LOOS		0x60000000	/* Start of OS-specific */
#define PT_GNU_EH_FRAME	0x6474e550	/* GCC .eh_frame_hdr segment */
#define PT_GNU_STACK	0x6474e551	/* Indicates stack executability */
#define PT_GNU_RELRO	0x6474e552	/* Read-only after relocation */
#define PT_PAX_FLAGS	0x65041580	/* Indicates PaX flag markings */
#define PT_LOSUNW	0x6ffffffa
#define PT_SUNWBSS	0x6ffffffa	/* Sun Specific segment */
#define PT_SUNWSTACK	0x6ffffffb	/* Stack segment */
#define PT_HISUNW	0x6fffffff
#define PT_HIOS		0x6fffffff	/* End of OS-specific */
#define PT_LOPROC	0x70000000	/* Start of processor-specific */
#define PT_HIPROC	0x7fffffff	/* End of processor-specific */

/* Legal values for p_flags (segment flags).  */

#define PF_X		(1 << 0)	/* Segment is executable */
#define PF_W		(1 << 1)	/* Segment is writable */
#define PF_R		(1 << 2)	/* Segment is readable */
#define PF_PAGEEXEC	(1 << 4)	/* Enable  PAGEEXEC */
#define PF_NOPAGEEXEC	(1 << 5)	/* Disable PAGEEXEC */
#define PF_SEGMEXEC	(1 << 6)	/* Enable  SEGMEXEC */
#define PF_NOSEGMEXEC	(1 << 7)	/* Disable SEGMEXEC */
#define PF_MPROTECT	(1 << 8)	/* Enable  MPROTECT */
#define PF_NOMPROTECT	(1 << 9)	/* Disable MPROTECT */
#define PF_RANDEXEC	(1 << 10)	/* Enable  RANDEXEC */
#define PF_NORANDEXEC	(1 << 11)	/* Disable RANDEXEC */
#define PF_EMUTRAMP	(1 << 12)	/* Enable  EMUTRAMP */
#define PF_NOEMUTRAMP	(1 << 13)	/* Disable EMUTRAMP */
#define PF_RANDMMAP	(1 << 14)	/* Enable  RANDMMAP */
#define PF_NORANDMMAP	(1 << 15)	/* Disable RANDMMAP */
#define PF_MASKOS	0x0ff00000	/* OS-specific */
#define PF_MASKPROC	0xf0000000	/* Processor-specific */



template <class ELF_PHDR>
class ProgramHeader {
public:
    ProgramHeader(ELF_PHDR* phentry):_phentry(phentry){}
    uint64_t getType() { return _phentry->p_type; }
    std::string getTypeStr();
    uint64_t getOffset(){ return _phentry->p_offset; }
    uint64_t getVaddr(){ return _phentry->p_vaddr; }
    uint64_t getPaddr(){ return _phentry->p_paddr; }
    uint64_t getFilesz(){ return _phentry->p_filesz; }
    uint64_t getMemsz(){ return _phentry->p_memsz; }
    uint64_t getFlags(){ return _phentry->p_flags; }
    std::string getFlagsStr();
    uint64_t getAlign(){ return _phentry->p_align; }
    void showInfo();
private:
    ELF_PHDR* _phentry;
};

template <class ELF_PHDR>
void ProgramHeader<ELF_PHDR>::showInfo() {
    std::cout << "type: " << getType() << "  type: " << getTypeStr() << std::endl 
         << "vaddr: " << std::hex << "0x" << getVaddr() << std::endl 
         << "paddr: " << std::hex << "0x" << getPaddr() << std::endl 
         << "filesz: " << std::dec << getFilesz() << std::endl 
         << "memsz: " << std::dec << getMemsz() << std::endl 
         << "flags: " << getFlags() << "  " << getFlagsStr() << std::endl 
         << "align: " << getAlign() << std::endl;
}

template <class ELF_PHDR>
std::string ProgramHeader<ELF_PHDR>::getFlagsStr() {
    return "";
}

template <class ELF_PHDR>
std::string ProgramHeader<ELF_PHDR>::getTypeStr() {
    std::string s;
#define C(V) case PT_##V: s = #V; break
        switch (getType()) {
            C(NULL);        C(LOAD);        C(DYNAMIC);
            C(INTERP);      C(NOTE);        C(SHLIB);
            C(PHDR);        C(TLS);         
            C(SUNWBSS);     C(SUNWSTACK);   
            default:
            s = "unknown";
            break;
        }
#undef  C
        return s;
}


#endif //__PROGRAMHEADER_H__
