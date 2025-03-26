#include <archive.h>
#include <archive_entry.h>
#include <filesystem>
#include <string>
#include <fstream>

namespace fs = std::filesystem;

class Archiver{
    public:
        Archiver(std::string filename);
        ~Archiver();
        //Archiver(std::string location);
        void AddFile(std::string location);
        void AddFile(fs::directory_entry location);
        void AddDirectory(fs::directory_entry location);
        void ArchiveCurrentLocation();
        void ExtractCurrentLocation();
        void setCurrentLocation(std::string location);
        std::string getCurrentLocation();
    private:
        std::string CurrentLocation;
        std::ifstream ArchiveFile;
        struct archive *a;
};