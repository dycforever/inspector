#include <iostream>
#include <libelf.h>
#include <gelf.h>

#include "common.h"
#include "Header.h"
#include "ProgramHeader.h"
#include "SectionHeader.h"

#include "utility.h"


namespace dyc {

class ELF_TYPE_32 {
public:
    typedef Header<Elf32_Ehdr> HeaderType;
    typedef Elf32_Phdr ProgramHeaderType;
    typedef Elf32_Shdr SectionHeaderType;
};

class ELF_TYPE_64 {
public:
    typedef Header<Elf64_Ehdr> HeaderType;
    typedef Elf64_Phdr ProgramHeaderType;
    typedef Elf64_Shdr SectionHeaderType;
};


template <class ELF_TYPE>
class InspectorTemplate {
public:

    typedef typename ELF_TYPE::HeaderType HeaderType;
    typedef typename ELF_TYPE::ProgramHeaderType ProgramHeaderType;
    typedef typename ELF_TYPE::SectionHeaderType SectionHeaderType;

    int inspect (const std::string& fileName);
    HeaderType* getHeader();
    ProgramHeaderType* getphdr(HeaderType* , int);
    SectionHeaderType* getshdr(HeaderType* , int);

private:
    uint64_t _rawFileSize;
    char* _rawFile;
    Elf* _elf;
    HeaderType* _header;
	std::fstream saveFile;

    std::vector<ProgramHeader<ProgramHeaderType>* > phentrys;
    std::vector<SectionHeader<SectionHeaderType>* > shentrys;

    bool isShstrtbl(SectionHeaderType* shdr, int);
};

template <class HEADER_TYPE>
Header<HEADER_TYPE>* createHeader(HEADER_TYPE* header) {
    return NEW Header<HEADER_TYPE>(header);
}


template <class ELF_TYPE> 
typename InspectorTemplate<ELF_TYPE>::ProgramHeaderType* 
InspectorTemplate<ELF_TYPE>::getphdr(HeaderType* elf_header, int i) {
    Elf_Off phoff = elf_header->getPhoff();
    char* pstart = _rawFile + phoff;
    return ((ProgramHeaderType*)pstart + i);
}

template <class ELF_TYPE> 
typename InspectorTemplate<ELF_TYPE>::SectionHeaderType* 
InspectorTemplate<ELF_TYPE>::getshdr(HeaderType* elf_header, int i) {
    Elf_Off shoff = elf_header->getShoff();
    char* sstart = _rawFile + shoff;
    return ((SectionHeaderType*)sstart + i);
}

template <class ELF_TYPE>
int InspectorTemplate<ELF_TYPE>::inspect(const std::string& fileName) {
    int fd = open(fileName.c_str(), O_RDONLY);
    CHECK_ERROR(-1, fd >= 0, "open %s failed", fileName.c_str());

    elf_version(EV_CURRENT);
    _elf = elf_begin(fd, ELF_C_READ, NULL);
    CHECK_ERROR(-1, _elf != NULL, "elf_begin failed %d %s", elf_errno(), elf_errmsg(elf_errno()));
    Elf_Ehdr* elf_header = NULL;

#ifdef __x86_64__
    elf_header = elf64_getehdr(_elf);
#else
    elf_header = elf32_getehdr(_elf);
#endif
    CHECK_ERROR(-1, elf_header != NULL, "get elf header from %s failed %d %s", fileName.c_str(), elf_errno(), elf_errmsg(elf_errno()));
    _header = createHeader(elf_header);
	saveFile.open("mid.out", std::ios::out | std::ios::trunc | std::ios::binary);
    CHECK_ERROR(-1, !saveFile.fail(), "open file mid.out failed");

	mesg::HeaderMesg* mes = NEW mesg::HeaderMesg();
	mesg::HeaderMesg_ElfHeaderMesg* hmes = mes->mutable_elfheader();
    changeToHeaderMesg(_header, hmes);
//    _header->showInfo();
//    mes->setElfHeaderMesg(hmes);


    int ret = getFileSize(fileName.c_str(), _rawFileSize);
    CHECK_ERROR(-1, ret == 0, "get file size failed");
    _rawFile = NEW char[_rawFileSize];
    CHECK_ERROR(-1, _rawFile != NULL, "new _rawFile failed, size: %llu", _rawFileSize);
    uint64_t uret = read(fd, _rawFile, _rawFileSize);
    CHECK_ERROR(-1, uret == _rawFileSize, "read _rawFile failed, read: %llu != %llu", uret, _rawFileSize);
    
    for (unsigned int i=0; i<_header->getPhnum(); ++i) {
//        ProgramHeaderType* phdr = NEW GElf_Phdr();
//        gelf_getphdr(_elf, i, phdr);
        ProgramHeaderType* phdr = getphdr(_header, i);
        ProgramHeader<ProgramHeaderType>* phentry = NEW ProgramHeader<ProgramHeaderType>(phdr);

		mesg::HeaderMesg_ProgHeaderMesg* pmes = mes->add_progheaders();
		changeToProgMesg(phentry, pmes);

//        std::cout << "\n\n program head " << i << std::endl;
//        phentry->showInfo();
		pmes->set_begin(_header->getPhoff() + i* sizeof(ProgramHeaderType));
		pmes->set_end(pmes->begin() + sizeof(ProgramHeaderType));
        phentrys.push_back(phentry);
    }

    // init section headers
    SectionHeaderType* strtbl = NULL;
    for (unsigned int i=0; i<_header->getShnum(); ++i) {
        SectionHeaderType* shdr = getshdr(_header, i);
        if (isShstrtbl(shdr, i)) {
            strtbl = shdr;
        }
        SectionHeader<SectionHeaderType>* shentry = NEW SectionHeader<SectionHeaderType>(shdr);
        shentrys.push_back(shentry);
    }
    CHECK_ERROR(-1, strtbl!=NULL, "get var string table failed");

    // assign section headers' name
    for (unsigned int i=0; i < shentrys.size(); ++i) {
        SectionHeader<SectionHeaderType>* shentry = shentrys[i];
        shentry->assignName(strtbl, _rawFile);
		mesg::HeaderMesg_SecHeaderMesg* smes = mes->add_secheaders();;
//        shentry->showInfo();
		changeToSecMesg(shentry, smes);
		smes->set_begin(_header->getShoff() + i* sizeof(SectionHeaderType));
		smes->set_end(smes->begin() + sizeof(SectionHeaderType));

    }

	if (!mes->SerializeToOstream(&saveFile)) {
		std::cerr << "Failed to write elf header." << std::endl;
		return -1;
	}

//    Elf_Scn* scn = NULL; 
//    int i = 0;
//    while ((scn = elf_nextscn(_elf, scn)) != NULL) {
//        ++i;
//        GElf_Shdr* shdr = NEW GElf_Shdr;
//        if (gelf_getshdr(scn, shdr) != shdr){
//            FATAL("get shdr failed");
//            break;
//        }
//        printElf32_Shdr((Elf32_Shdr*)(_rawFile+0x130+40));
//        printGElf_Shdr((GElf_Shdr*)shdr);
//        dumpMem("mem.out", (char*)shdr, sizeof(*shdr));
//        break;
//        SectionHeader* shentry = NEW SectionHeader(shdr, _rawFile);
//        std::cout << "\n\n section head " << i << std::endl;
//        shentry->showInfo();
//        shentrys.push_back(shentry);
//    }
//
//    if ((unsigned int)i != _header->getShnum()) {
//        WARNING("i %d != shnum %u", i, _header->getShnum());
//    }

    saveFile.close();
    return 0;
}

template <class ELF_TYPE>
bool InspectorTemplate<ELF_TYPE>::isShstrtbl(SectionHeaderType* shdr, int index) {
    uint64_t type = shdr->sh_type;
    if (type == SHT_STRTAB && shdr->sh_addr == 0 && _header->getShstrndx() == index) {
        std::cout << "index:" << index << " shstrndx:" << _header->getShstrndx() << std::endl;
        return true;
    }
    return false;
}

template <class ELF_TYPE>
typename InspectorTemplate<ELF_TYPE>::HeaderType*
InspectorTemplate<ELF_TYPE>::getHeader() {
    return _header;
}

#ifdef __x86_64__
    typedef InspectorTemplate<ELF_TYPE_64> Inspector;
#else
    typedef InspectorTemplate<ELF_TYPE_32> Inspector;
#endif


}
