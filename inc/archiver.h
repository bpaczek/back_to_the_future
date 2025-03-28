#ifndef ARCHIVER_H
#define ARCHIVER_H

#include <string>
#include <filesystem>
#include "status.h"
#include "IExplorer.h"
#include "ILibarchive_wrapper.h"

namespace fs = std::filesystem;

class Archiver {
public:
    Archiver(std::unique_ptr<ILibArchiveWrapper> libarchive);
    Archiver(std::string filename, std::unique_ptr<ILibArchiveWrapper> libarchive);
    Archiver(IExplorer& explorer, std::unique_ptr<ILibArchiveWrapper> libarchive);

    ~Archiver();

    Status Extract(std::string location);
    Status ArchiveItem(fs::directory_entry location);

private:
    class Impl; 
    std::unique_ptr<Impl> pImpl;
};

#endif // ARCHIVER_H