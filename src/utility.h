
#ifndef __UTILITY_H__
#define __UTILITY_H__

#include "messages/mesg.pb.h"

namespace dyc {

template <class Header>
mesg::HeaderMesg_ElfHeaderMesg* changeToHeaderMesg (Header* header, mesg::HeaderMesg_ElfHeaderMesg* mesg) {

	mesg->set_type(header->getType());
	mesg->set_machine(header->getMachine());
	mesg->set_version(header->getVersion());
	mesg->set_entry(header->getEntry());
	mesg->set_phoff(header->getPhoff());
	mesg->set_shoff(header->getShoff());

	mesg->set_flags(header->getFlags());

	mesg->set_ehsize(header->getEhsize());
	mesg->set_phentsize(header->getPhentsize());
	mesg->set_phnum(header->getPhnum());
	mesg->set_shentsize(header->getShentsize());
	mesg->set_shnum(header->getShnum());
	mesg->set_shstrndx(header->getShstrndx());

	return mesg;
}

template <class SecHeader>
mesg::HeaderMesg_SecHeaderMesg* changeToSecMesg (SecHeader* header, mesg::HeaderMesg_SecHeaderMesg* mesg) {

	mesg->set_name(header->getNameStr());
	mesg->set_type(header->getType());
	mesg->set_flags(header->getFlags());
	mesg->set_addr(header->getAddr());
	mesg->set_offset(header->getOffset());
	mesg->set_size(header->getSize());

	mesg->set_link(header->getLink());
	mesg->set_info(header->getInfo());
	mesg->set_addralign(header->getAddralign());
	mesg->set_entsize(header->getEntsize());

	return mesg;
}

template <class ProgramHeader>
mesg::HeaderMesg_ProgHeaderMesg* changeToProgMesg (ProgramHeader* header, mesg::HeaderMesg_ProgHeaderMesg* mesg) {

	mesg->set_type(header->getType());
	mesg->set_offset(header->getOffset());
	mesg->set_vaddr(header->getVaddr());
	mesg->set_paddr(header->getPaddr());
	mesg->set_filesz(header->getFilesz());
	mesg->set_memsz(header->getMemsz());
	mesg->set_flags(header->getFlags());
	mesg->set_align(header->getAlign());

	return mesg;
}

void dumpfile(std::string in, std::string out, size_t off, size_t size);
void dumpMem(std::string out, char* buf, size_t size);

void printGElf_Shdr (GElf_Shdr* shdr);
void printElf32_Shdr (Elf32_Shdr* shdr);



}

#endif