#ifndef ARCHIVER_H
#define ARCHIVER_H

#include <archive.h>
#include <archive_entry.h>
#include <filesystem>
#include <string>
#include <fstream>
#include "status.h"

namespace fs = std::filesystem;

class Archiver{
    public:
        Archiver();
        Archiver(std::string filename);
        ~Archiver();
        void AddFile(std::string location);
        void AddFile(fs::directory_entry location);
        void AddDirectory(fs::directory_entry location);
        void ArchiveCurrentLocation();
        Status Extract(std::string location);
        std::string getCurrentLocation();
        void ArchiveItem(fs::directory_entry location);
    private:
        std::string CurrentLocation;
        std::ifstream FileWithArchive;
        struct archive *a;
        std::string PathOfItemToArchive;

        archive* ArchiveReader;
        archive* ArchiveFile;

    /* proteced methods */
    protected:
        Status ArchiveEntries(archive_entry* entry);
};

#endif