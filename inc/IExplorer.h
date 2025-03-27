#ifndef IEXPLORER_H
#define IEXPLORER_H

#include <filesystem>

class IExplorer {
public:
    virtual ~IExplorer() = default;
    virtual  std::filesystem::directory_entry GetLocation() = 0;
};

#endif 