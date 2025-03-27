#include <filesystem>
#include <string>
#include <vector>

namespace fs = std::filesystem;

class Explorer{
    public: 
        // constructors
        Explorer();
        Explorer(std::string location);
        
        std::string GetCurrentLocationPath();
        std::filesystem::directory_entry GetCurrentLocation();
        void GetCurrentLocationContent();
        void PrintCurrentLocationContent();
        void MoveToLocation(std::filesystem::directory_entry location);
        std::filesystem::directory_entry GetLocationItembyId(uint32_t id);
        std::filesystem::directory_entry SelectItemToArchive();

    private:
        /* fields */
        std::filesystem::directory_entry CurrentLocation;
        std::vector<std::filesystem::directory_entry> LocationContent;

        /* private functions */
        void SetCurrentLocation(std::filesystem::directory_entry location);        
};