#include <filesystem>
#include <string>
#include "logs.h"

#include "explorer.h"
#include <stdint.h>

namespace fs = std::filesystem;

Explorer::Explorer(){
    this->CurrentLocation = fs::directory_entry(fs::current_path());
    debug_print("Explorer created. Current directory is:", this->CurrentLocation.path());
};

Explorer::Explorer(std::string location):CurrentLocation(location){};

void Explorer::SetCurrentLocation(std::filesystem::directory_entry location){
    this->CurrentLocation = location;
}
  
std::string Explorer::GetCurrentLocationPath(){
        return this->CurrentLocation.path();;
}

void Explorer::GetCurrentLocationContent(){
    this->LocationContent.clear();
    for(const auto item : fs::directory_iterator(this->CurrentLocation)){
            this->LocationContent.push_back(item);
    }
}

void Explorer::PrintCurrentLocationContent(){
    uint32_t i = 0;
    for(const auto item : this->LocationContent){
        std::cout << i++ << " " << item << std::endl;
    }
}

void Explorer::MoveToLocation(std::filesystem::directory_entry location){
    debug_print("Moving to location", location.path());
    this->SetCurrentLocation(location);
    this->GetCurrentLocationContent();
    debug_print("Found items: ", this->LocationContent.size());
}

std::filesystem::directory_entry Explorer::GetLocationItembyId(uint32_t id){
    auto location = this->LocationContent.at(id);
    debug_print("Selected item: ", location.path());
    return location;
}

std::filesystem::directory_entry Explorer::GetCurrentLocation(){
    return this->CurrentLocation;
}

std::filesystem::directory_entry Explorer::SelectItemToArchive(){
    auto location = GetCurrentLocation();

    try{
        std::string x = std::string();
        std::cout << "Select item to archive: " << std::endl;
        std::cout << "\t[number] -(file) archive file  " << std::endl;
        std::cout << "\t[number] -(directory) go to the directory " << std::endl;
        std::cout << "\tA - Archive current directory " << std::endl;
        std::cout << "\tX - Exit " << std::endl;
        
        do{
            this->GetCurrentLocationContent();
            std::cout << this->GetCurrentLocationPath() << ":" << std::endl;
            this->PrintCurrentLocationContent();
            std::cin >> x;

            if(x == "A" ){
                break;
            }else if(x == "X"){
                exit(0);
            }else{
                uint32_t id = std::stoi(x);
                if(id >= this->LocationContent.size()){
                    debug_print("Invalid selection");
                    continue;
                }

                location = GetLocationItembyId(id);
                if(location.is_directory() == true){
                    MoveToLocation(location);
                }
                else{
                    debug_print("Selected item is a file: ", location.path());
                    break;
                }
            }
        } while(true);
    }
    catch(const std::exception& e){
        debug_print("Failed to select item to archive");
    }

    return location;
}