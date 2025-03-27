#include "explorer.h"
#include "logs.h"
#include <iostream>
#include <limits>
#include <vector>

namespace fs = std::filesystem;

/**
 * @class Explorer::Impl
 * @brief Implementation class for the Explorer functionality, providing methods to navigate and interact with the filesystem.
 * 
 * This class encapsulates the logic for managing the current directory, listing its contents,
 * and allowing navigation and selection of items within the filesystem.
 */
class Explorer::Impl {
public:
    /**
     * @brief Represents the current location in the filesystem as a directory entry.
     */
    fs::directory_entry CurrentLocation;

    /**
     * @brief The contents of the current location in the filesystem.
     */
    std::vector<fs::directory_entry> LocationContent;

    /**
     * @brief Default constructor. Initializes the current location to the current working directory.
     */
    Impl() {
        CurrentLocation = fs::directory_entry(fs::current_path());
        debug_print("Explorer created. Current directory is:", CurrentLocation.path());
    }

    /**
     * @brief Constructor that initializes the current location to a specified directory.
     * @param location The path to the initial directory.
     */
    Impl(std::string location) : CurrentLocation(location) {}

    /**
     * @brief Allows the user to select an item to archive or navigate the filesystem.
     * @param location Pointer to store the selected location, if any.
     * @return Status indicating the result of the operation (e.g., success or user exit).
     */
    Status SelectItemToArchive(std::filesystem::directory_entry* location){
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
    
        if(location != nullptr) *location = this->CurrentLocation;
    
        return status;
    }

    std::filesystem::directory_entry GetLocation() {
        return this->CurrentLocation;
    } 

    private:
    /**
     * @brief Sets the current location in the filesystem.
     * @param location The new current location.
     */
    void SetCurrentLocation(std::filesystem::directory_entry location){
        CurrentLocation = location;
    }

    /**
     * @brief Retrieves the path of the current location as a string.
     * @return The path of the current location.
     */
    std::string GetCurrentLocationPath(){
            return this->CurrentLocation.path();;
    }

    /**
     * @brief Populates the LocationContent vector with the contents of the current location.
     */
    void GetCurrentLocationContent(){
        this->LocationContent.clear();
        for(const auto item : fs::directory_iterator(this->CurrentLocation)){
                this->LocationContent.push_back(item);
        }
    }

    /**
     * @brief Prints the contents of the current location to the console.
     */
    void PrintCurrentLocationContent(){
        uint32_t i = 0;
        for(const auto item : this->LocationContent){
            std::cout << i++ << " " << item << std::endl;
        }
    }

    /**
     * @brief Moves to a specified location in the filesystem.
     * @param location The new location to move to.
     */
    void MoveToLocation(std::filesystem::directory_entry location){
        debug_print("Moving to location", location.path());
        this->SetCurrentLocation(location);
        this->GetCurrentLocationContent();
        debug_print("Found items: ", this->LocationContent.size());
    }

    /**
     * @brief Retrieves an item from the current location's contents by its ID.
     * @param id The ID of the item to retrieve.
     * @return The directory entry corresponding to the specified ID.
     */
    std::filesystem::directory_entry GetLocationItembyId(uint32_t id){
        auto location = this->LocationContent.at(id);
        debug_print("Selected item: ", location.path());
        return location;
    }

    /**
     * @brief Retrieves the current location as a directory entry.
     * @return The current location.
     */
    std::filesystem::directory_entry GetCurrentLocation(){
        return this->CurrentLocation;
    }

    /**
     * @brief Prints a help message to the console, describing available commands for ths module.
     */
    void PrintHelp(){
        std::cout << "Select item to archive: " << std::endl;
        std::cout << "\t[number] -(file) archive file  " << std::endl;
        std::cout << "\t[number] -(directory) go to the directory " << std::endl;
        std::cout << "\tA - Archive current directory " << std::endl;
        std::cout << "\tX - Exit " << std::endl;
    }
};

Explorer::Explorer() : pImpl(std::make_unique<Impl>()) {}

Explorer::Explorer(std::string location) : pImpl(std::make_unique<Impl>(location)) {}

Explorer::~Explorer() = default;

Status Explorer::SelectItemToArchive(std::filesystem::directory_entry* location) {
    return pImpl->SelectItemToArchive(location);
}







