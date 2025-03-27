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

void Explorer::PrintHelp(){
    std::cout << "Select item to archive: " << std::endl;
    std::cout << "\t[number] -(file) archive file  " << std::endl;
    std::cout << "\t[number] -(directory) go to the directory " << std::endl;
    std::cout << "\tA - Archive current directory " << std::endl;
    std::cout << "\tX - Exit " << std::endl;
}

Status Explorer::SelectItemToArchive(std::filesystem::directory_entry* location){
    Status status = Success;
    auto selectedLocation = GetCurrentLocation();
    uint32_t id = 0;
    std::string x;

    do{
        PrintHelp();
        this->GetCurrentLocationContent();
        std::cout << this->GetCurrentLocationPath() << ":" << std::endl;
        this->PrintCurrentLocationContent();

        if (!(std::cin >> x)) {
            std::cin.clear(); // Clear the error flag
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard invalid input
            debug_print("Invalid input. Please try again.");
            continue;
        }

        if(x == "A"){
            break;
        }
        else if(x == "X") {
            debug_print("User requested exit.");   
            status = UserExit;
            break;
        }
        else {
            try{
                int id_temp = std::stoi(x);
                if(id_temp < 0 || static_cast<size_t>(id_temp) >= this->LocationContent.size()) {
                    debug_print("Invalid selection");
                    continue;
                }
                id = static_cast<uint32_t>(id_temp);
            } catch (const std::out_of_range& e) {
                debug_print("Invalid input: number out of range");
                continue;
            } catch (const std::invalid_argument& e) {
                debug_print("Invalid input: not a number");
                continue;
            } catch (const std::exception& e) {
                debug_print("Invalid input: ", e.what());
                continue;
            }

            selectedLocation = GetLocationItembyId(id);
            if(selectedLocation.is_directory()){
                MoveToLocation(selectedLocation);
            }
            else{
                debug_print("Selected item is a file: ", selectedLocation.path());
                break;
            }
        }
    } while(true);

    SetCurrentLocation(selectedLocation);

    if(location != nullptr) location = &this->CurrentLocation;

    return status;
}