#ifndef ARCHIVER_H
#define ARCHIVER_H

#include <archive.h>
#include <archive_entry.h>
#include <filesystem>
#include <string>
#include <fstream>
#include "status.h"
#include "IExplorer.h"

namespace fs = std::filesystem;

class Archiver{
    public:
        Archiver();
        Archiver(std::string filename);
        Archiver(IExplorer* explorer);
        ~Archiver();

        Status Extract(std::string location);
        Status ArchiveItem(fs::directory_entry location);
    private:
        archive* Archive = NULL;
        archive* ArchiveFile = NULL;

        std::ifstream FileWithArchive;
        std::string PathOfItemToArchive;

        Status AddFile(std::string location);
        Status AddFile(fs::directory_entry location);
        Status AddDirectory(fs::directory_entry location);
        void ArchiveCurrentLocation();
        std::string CutArchivePath(std::string path);
        Status ArchiveEntries(archive_entry* entry);

        Status WriteData(std::string &location);
};

#endif