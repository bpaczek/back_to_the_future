#include <filesystem>
#include <string>
#include "logs.h"

#include "explorer.h"
#include <stdint.h>

namespace fs = std::filesystem;

Explorer::Explorer(){
    this->CurrentLocation = fs::current_path();
};

Explorer::Explorer(std::string location):CurrentLocation(location){};

void Explorer::setCurrentLocation(std::string location){
    this->CurrentLocation = location;
}
  
std::string Explorer::getCurrentLocation(){
        return this->CurrentLocation;
}

void Explorer::GetCurrentLocationContent(){
    for(const auto item : fs::directory_iterator(this->CurrentLocation)){
            debug_print("Found item", item);
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
    this->setCurrentLocation(location.path());
    this->LocationContent.clear();
    this->GetCurrentLocationContent();
}

std::filesystem::directory_entry Explorer::getLocationItembyId(uint32_t id){
    return this->LocationContent.at(id);
}
