#include <iostream>
#include "logs.h"
#include "explorer.h"
#include "archiver.h"

#define ARCHIVE_NAME_LOCATION 1
const std::string DEFAULT_ARCHIVE_NAME = "archive.tar.gz";

void 
print_help(){
    std::cout << "Usage:" << std::endl;
    std::cout << "BTTF for archivization mode" << std::endl;
    std::cout << "BTTF <archive_name> for unpack " << std::endl;
}

enum Modes {
    PACK,
    UNPACK,
    UNDEFINED,
};

int pack_mode(){
    auto explorer = new Explorer();
    auto entry = explorer->SelectItemToArchive();
    
    debug_print("Selected item", entry.path());

    auto archive = new Archiver(DEFAULT_ARCHIVE_NAME);
    archive->ArchiveItem(entry);
    
    delete archive;
    delete explorer;
    return 0;
}

int unpack_mode(std::string file_name){
    auto archive = new Archiver();
    archive->Extract(file_name);
    delete archive;

    return 0;
}

int
main(int argc, char** argv){
    Modes mode = UNDEFINED;

    if (argc > 3) {
        debug_print("Too many arguments");
    } else if(argc == 2) {
        mode = UNPACK;
        debug_print("Unpack mode");
    } else {
        mode = PACK;
        debug_print("Pack mode");
    }

    switch (mode)
    {
    case PACK:
        pack_mode();
        break;
    case UNPACK:
        unpack_mode(argv[1]);
        break;
    default:
        print_help();
        break;
    }

    return 0;
}
