#ifndef ARCHIVER_H
#define ARCHIVER_H

#include <string>
#include <filesystem>
#include "status.h"
#include "IExplorer.h"

namespace fs = std::filesystem;

class Archiver {
public:
    // Konstruktor domyślny
    Archiver();

    // Konstruktor z nazwą pliku
    Archiver(std::string filename);

    // Konstruktor z obiektem IExplorer
    Archiver(IExplorer* explorer);

    // Destruktor
    ~Archiver();

    // Metody publiczne
    Status Extract(std::string location);
    Status ArchiveItem(fs::directory_entry location);

private:
    class Impl; // Forward declaration klasy implementacyjnej
    std::unique_ptr<Impl> pImpl; // Wskaźnik do implementacji
};

#endif // ARCHIVER_H