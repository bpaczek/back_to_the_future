#include "archiver.h"
#include "logs.h"

/// @brief 
/// @param filename - archive file name
Archiver::Archiver(std::string filename){
    //a = archive_write_new();
    a = archive_write_disk_new();
    archive_write_add_filter_gzip(a);
    archive_write_set_format_pax_restricted(a);
    archive_write_open_filename(a, filename.c_str()); //check if file exists

    this->CurrentLocation = filename;

    try{
        this->ArchiveFile.open(this->CurrentLocation, std::ios::in | std::ios::binary);
    
        if(!this->ArchiveFile.is_open()){
            debug_print("Failed to open file", this->CurrentLocation);
        }
    }
    catch(const std::exception& e)
    {
        debug_print("Failed to create an archive!", this->CurrentLocation);
    }
}

void Archiver::AddFile(std::string location){
    struct archive_entry *entry = archive_entry_new();
    archive_entry_set_pathname(entry, location.c_str());
    archive_entry_set_size(entry, fs::file_size(location));
    archive_entry_set_filetype(entry, AE_IFREG); //check AE_IFREG
    archive_entry_set_perm(entry, 0644); //consider to change to 0777
    archive_write_header(a, entry);
    char buff[8192];
    std::ifstream file(location, std::ios::binary);
    while (file.good()) {
        file.read(buff, sizeof(buff));
        archive_write_data(a, buff, file.gcount());
    }
    archive_entry_free(entry);
}

void Archiver::AddFile(fs::directory_entry location){
    this->AddFile(location.path());
}

void Archiver::AddDirectory(fs::directory_entry location){
    for (const auto& entry : fs::recursive_directory_iterator(location)) {
        if (fs::is_regular_file(entry)) {
            this->AddFile(entry.path());
        }
    }
}

Archiver::~Archiver(){
    archive_write_close(a);
    archive_write_free(a);
    this->ArchiveFile.close();
}
