#ifndef EXPLORER_H
#define EXPLORER_H

#include <filesystem>
#include "IExplorer.h"
#include <memory>
#include "status.h"

namespace fs = std::filesystem;

class Explorer : public IExplorer {
public:
    Explorer();
    Explorer(std::string location);
    ~Explorer();

    Status SelectItemToArchive(std::filesystem::directory_entry* location = nullptr);
    std::filesystem::directory_entry GetLocation();

private:
    class Impl; // Forward declaration of implementation class
    std::unique_ptr<Impl> pImpl; // Pointer to implementation
};

#endif