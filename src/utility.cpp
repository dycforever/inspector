
#include "utility.h"

namespace dyc {

void dumpMem(string out, char* buf, size_t size) {
    FILE* fpout = fopen(out.c_str(), "w");
    fwrite(buf, 1, size, fpout);
    fclose(fpout);
}

void dumpfile(string in, string out, size_t off, size_t size) {
    FILE* fpin = fopen(in.c_str(), "r");
    FILE* fpout = fopen(out.c_str(), "w");
    assert(fpin != NULL && fpout != NULL);
    fseek(fpin, off, SEEK_SET);
    char* buf = NEW char[size];
    fread(buf, 1, size, fpin);
    fwrite(buf, 1, size, fpout);
    DELETES(buf);
    fclose(fpin);
    fclose(fpout);
}

void printGElf_Shdr (GElf_Shdr* shdr) {
    cout << endl << "output gelf info : " << endl;
    cout << "type: " << shdr->sh_type << endl 
    << "flags: " << shdr->sh_flags << endl 
    << "addr: " << hex << "0x" << shdr->sh_addr << endl 
    << "offset: " << hex << "0x" << shdr->sh_offset << endl 
    << "size: " << dec << shdr->sh_size << endl 
    << "link: " << dec << shdr->sh_link << endl 
    << "info: " << dec << shdr->sh_info << endl 
    << "align: " << shdr->sh_addralign << endl
    << "entsize: " << shdr->sh_entsize << endl;
}

void printElf32_Shdr (Elf32_Shdr* shdr) {
    cout << endl << "output elf32 info : " << endl;
    cout << "type: " << shdr->sh_type << endl 
    << "flags: " << shdr->sh_flags << endl 
    << "addr: " << hex << "0x" << shdr->sh_addr << endl 
    << "offset: " << hex << "0x" << shdr->sh_offset << endl 
    << "size: " << dec << shdr->sh_size << endl 
    << "link: " << dec << shdr->sh_link << endl 
    << "info: " << dec << shdr->sh_info << endl 
    << "align: " << shdr->sh_addralign << endl
    << "entsize: " << shdr->sh_entsize << endl;
}

}

