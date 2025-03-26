#include <filesystem>
#include <string>
#include <vector>

namespace fs = std::filesystem;

class Explorer{
    public: 
        // constructors

        /// @brief The defulat construtor.
        Explorer();
        /// @brief Explorer Constructor
        /// @param location Location in the file system that shall be set as initial value
        Explorer(std::string location);
        
        std::string getCurrentLocation();
        void GetCurrentLocationContent();
        void PrintCurrentLocationContent();
        void MoveToLocation(std::filesystem::directory_entry location);
        std::filesystem::directory_entry getLocationItembyId(uint32_t id);
        private:
        std::string CurrentLocation;
        std::vector<std::filesystem::directory_entry> LocationContent;
        void setCurrentLocation(std::string location);
};