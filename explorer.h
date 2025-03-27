#ifndef EXPLORER_H
#define EXPLORER_H

#include <filesystem>
#include <string>
#include <vector>
#include "status.h"

namespace fs = std::filesystem;

class Explorer{
    public: 
    Explorer();
    Explorer(std::string location);
    
    std::string GetCurrentLocationPath();
    std::filesystem::directory_entry GetCurrentLocation();
    void GetCurrentLocationContent();
    void PrintCurrentLocationContent();
    void MoveToLocation(std::filesystem::directory_entry location);
    std::filesystem::directory_entry GetLocationItembyId(uint32_t id);
    Status SelectItemToArchive(std::filesystem::directory_entry* location = nullptr);

    private:
    std::filesystem::directory_entry CurrentLocation;
    std::vector<std::filesystem::directory_entry> LocationContent;
    static void PrintHelp();
    void SetCurrentLocation(std::filesystem::directory_entry location);        
};

#endif