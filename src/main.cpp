
#include "Inspector.h"

using namespace dyc;

int main (int argc, char** argv) {

    Inspector inspector;
    std::cout << "argc = " << argc << std::endl;
    std::string file;
    if (argc < 2) {
        std::cerr << "usage: ./InspectorTemplate filename" << std::endl;
        file = "a.out";
    }
    else {
         file = argv[1];
    }
    int ret = inspector.inspect(file);
    assert (ret == 0);

    Inspector::HeaderType* header = inspector.getHeader();
    assert(header != NULL);
    header->showInfo();

    int fd = open(file.c_str(), O_RDONLY);
    CHECK_ERROR(-1, fd>0, "open file %s failed", file.c_str());
    ret = header->check(fd);
    
    NOTICE("run success %d", ret);
}
