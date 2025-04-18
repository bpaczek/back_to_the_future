#include <iostream>
#include "logs.h"
#include "explorer.h"
#include "archiver.h"
#include "status.h"
#include "libarchive_wrapper.h"

const std::string DEFAULT_ARCHIVE_NAME = "archive.tar.gz";

#define MAX_PARAM_NUMBERS 2

enum Modes {
    UNDEFINED,
    PACK,
    UNPACK
};

/**
 * @brief Prints the help message for the program.
 * 
 * This function provides usage instructions for the program, including
 * the available modes (archivization and unpacking) and their respective
 * command-line arguments.
 */
void 
print_help(){
    std::cout << "Usage:" << std::endl;
    std::cout << "BTTF for archivization mode" << std::endl;
    std::cout << "BTTF <archive_name> for unpack " << std::endl;
}

/**
 * @brief Handles the packing mode by selecting an item to archive and processing it.
 * 
 * This function demonstrates two possible implementations of using the Archiver class:
 * 1. Using the Archiver with an Explorer instance to manage the archive process.
 * 2. Using the Archiver independently without an Explorer instance.
 * 
 * Note: The dual implementation is provided solely for the purpose of showcasing 
 * the usage of interfaces in C++.
 * 
 * @return Status - Returns the status of the operation, either Success or UserExit.
 */
Status pack_mode(){
    auto libarchive = std::make_unique<LibArchiveWrapper>();
    Explorer explorer;

    std::filesystem::directory_entry entry; 
    auto stat = explorer.SelectItemToArchive(&entry);
    if(stat == UserExit){
        return stat;
    }
        
    debug_print("Selected item", entry.path());;

    /// Possible use of Archiver with Explorer
    Status status = Success;
    auto archive = Archiver(explorer, std::move(libarchive));

    /// Possible use of Archiver without Explorer
    // auto archive = new Archiver(DEFAULT_ARCHIVE_NAME, std::move(libarchive));
    // Status status = archive->ArchiveItem(entry);
    // delete archive;

    return status;
}

/**
 * @brief Unpacks the contents of an archive file.
 * 
 * This function initializes an Archiver object and uses it to extract the 
 * contents of the specified archive file.
 * 
 * @param file_name The name of the archive file to be unpacked.
 * @return Status The result of the extraction operation.
 */
Status unpack_mode(std::string file_name){
    auto libarchive = std::make_unique<LibArchiveWrapper>();
    auto archive = Archiver(std::move(libarchive));
    return archive.Extract(file_name);
}

int
main(int argc, char** argv){
    Modes mode = UNDEFINED;
    Status stat = Success;

    if (argc > MAX_PARAM_NUMBERS) {
        debug_print("Too many arguments");
        stat = TooManyArgs;
    } else if(argc == MAX_PARAM_NUMBERS) {
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
