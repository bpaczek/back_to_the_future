#include <iostream>
#include "logs.h"
#include "explorer.h"
#include "archiver.h"
#include "status.h"

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

Status pack_mode(){
    Explorer explorer;

    std::filesystem::directory_entry entry; 
    auto stat = explorer.SelectItemToArchive(&entry);
    if(stat == UserExit){
        return stat;
    }
        
    debug_print("Selected item", entry.path());

    /// Possible use of Archiver with Explorer
    // Status status = Success;
    // auto archive = Archiver(&explorer);

    /// Possible use of Archiver without Explorer
    auto archive = new Archiver(DEFAULT_ARCHIVE_NAME);
    Status status = archive->ArchiveItem(entry);

    return status;
}

Status unpack_mode(std::string file_name){
    auto archive = Archiver();
    Status stat = archive.Extract(file_name);

    return stat;
}

int
main(int argc, char** argv){
    Modes mode = UNDEFINED;
    Status stat = Success;

    if (argc > 3) {
        debug_print("Too many arguments");
        stat = TooManyArgs;
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
        stat = pack_mode();
        stat == Success ? std::cout << "All files archive sucesfully" << std::endl : std::cout << "Something went wrong. Please verify result" <<  std::endl;
        break;
    case UNPACK:
        unpack_mode(argv[1]);
        stat == Success ? std::cout << "Files restoring finished with success" << std::endl : std::cout << "Something went wrong. Please verify result" <<  std::endl;
        break;
    default:
        print_help();
        break;
    }

    return stat;
}
