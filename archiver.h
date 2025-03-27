#include <archive.h>
#include <archive_entry.h>
#include <filesystem>
#include <string>
#include <fstream>

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
        void Extract(std::string location);
        std::string getCurrentLocation();
        void ArchiveItem(fs::directory_entry location);
    private:
        std::string CurrentLocation;
        std::ifstream ArchiveFile;
        struct archive *a;
        std::string PathOfItemToArchive;
};