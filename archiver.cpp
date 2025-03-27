#include "archiver.h"
#include "logs.h"
#include <memory.h>
#include <vector>

#define DATA_BLOCK_SIZE 0x4000

Archiver::Archiver(){};
Archiver::Archiver(std::string filename){
    a = archive_write_new();
    archive_write_add_filter_xz(a); //based on the observation this algorithm produce an archive with minimal size.
    archive_write_set_format_pax_restricted(a);
    if (archive_write_open_filename(a, filename.c_str()) != ARCHIVE_OK) {
        debug_print("Failed to open archive file", filename);
        throw std::runtime_error("Failed to open archive file");
    }

    CurrentLocation = filename;

    try{
        FileWithArchive.open(filename, std::ios::in | std::ios::binary);
    
        if(!FileWithArchive.is_open()){
             debug_print("Failed to open file", this->CurrentLocation);
        }
    }
    catch(const std::exception& e)
    {
        debug_print("Failed to create an archive!", this->CurrentLocation, ". ", e.what());
    }
}

Status Archiver::ArchiveEntries(archive_entry* entry){
    Status status = Success;
    int error_code = 0;

    while(archive_entry_size(entry) > 0){
        const void *buff;
        size_t size;
        la_int64_t offset;
        error_code = archive_read_data_block(ArchiveReader, (const void **)&buff, &size, &offset);
        if (error_code == ARCHIVE_EOF){
            break;
        }
        if (error_code < ARCHIVE_OK){
            debug_print("Failed to read archive data", archive_error_string(ArchiveReader));
            break;
        }
        error_code = archive_write_data_block(ArchiveFile, buff, size, offset);
        if (error_code < ARCHIVE_OK){
            debug_print("Failed to write archive data", archive_error_string(ArchiveFile));
            break;
        }
        debug_print("Error finishing entry for", archive_entry_pathname(entry), ":", archive_error_string(ArchiveFile));
    }

    error_code = archive_write_finish_entry(ArchiveFile);
    if (error_code < ARCHIVE_OK){
        debug_print(archive_error_string(ArchiveFile));
        status = AccessFileFailed;
    }

    return status;
}
Status Archiver::Extract(std::string location){
    struct archive_entry *entry;
    int flags = ARCHIVE_EXTRACT_TIME | ARCHIVE_EXTRACT_PERM | ARCHIVE_EXTRACT_FFLAGS;
    int error_code;
    Status status = Success;

    /* archive reader configuration */
    ArchiveReader = archive_read_new();
    archive_read_support_filter_all(ArchiveReader);
    archive_read_support_format_all(ArchiveReader);

    /* configure creating elements on disk */
    ArchiveReader = archive_write_disk_new();
    archive_write_disk_set_options(ArchiveReader, flags);
    archive_write_disk_set_standard_lookup(ArchiveReader);
    
    error_code = archive_read_open_filename(ArchiveReader, location.c_str(), DATA_BLOCK_SIZE);
    if(error_code != ARCHIVE_OK) {
        debug_print("Failed to open archive file", location);
        return CannotOpenFile;
    }

    do {
        error_code = archive_read_next_header(ArchiveReader, &entry);
        if (error_code == ARCHIVE_EOF){
            break;
        }
        if (error_code < ARCHIVE_OK){
            status = AccessFileFailed;
            debug_print("Failed to read archive header", archive_error_string(ArchiveReader));
            break;
        }

        error_code = archive_write_header(ArchiveReader, entry);
        if (error_code < ARCHIVE_OK){
            debug_print("Failed to write archive header", location);
            status = AccessFileFailed;
            break;
        }

        status = ArchiveEntries(entry);
        if(status != Success){
            debug_print("ArchiveEntries finished with status", status);
        }
    } while(true);

    /* clean up */
    archive_read_close(ArchiveReader);
    archive_read_free(ArchiveReader);
    archive_write_close(ArchiveFile);
    archive_write_free(ArchiveFile);

    return Success;
}

void Archiver::AddFile(std::string location){
    /* Create new entry to archive */
    struct archive_entry *entry = archive_entry_new();
    /* Remove leading part of path and set the pathname in the archive */
    std::string locationInArchive;
    if (location.find(this->PathOfItemToArchive) == 0) {
        locationInArchive = location.substr(this->PathOfItemToArchive.length());
    } else {
    try {
        archive_entry_set_size(entry, fs::file_size(location));
    } catch (const std::filesystem::filesystem_error& e) {
        debug_print("Failed to get file size for", location, ":", e.what());
        archive_entry_set_size(entry, 0); // Set size to 0 as a fallback
    }
        locationInArchive = location; // Fallback to using the full path
    }
    debug_print("Adding file to archive:", locationInArchive);
    archive_entry_set_pathname(entry, locationInArchive.c_str());

    archive_entry_set_size(entry, fs::file_size(location));
    /* Set archive entry properties */
    archive_entry_set_filetype(entry, AE_IFREG); //check AE_IFREG
    /* set permissions */
    archive_entry_set_perm(entry, 0644); //consider to change to 0777
    
    bool success = true;

    if (archive_write_header(a, entry) != ARCHIVE_OK) {
        debug_print("Failed to write archive header for", location, ":", archive_error_string(a));
        success = false;
    } else {
        char buff[DATA_BLOCK_SIZE];
        memset(buff, 0, sizeof(buff));
	    std::ifstream file(location, std::ios::binary);

        while (file.is_open()) {
            file.read(buff, sizeof(buff));
            std::streamsize bytesRead = file.gcount();
            if (bytesRead > 0) {
                if (archive_write_data(a, buff, bytesRead) < ARCHIVE_OK) {
                    debug_print("Failed to write data for", location, ":", archive_error_string(a));
                    success = false;
                    break;
                }
            }
            if (file.eof()) {
                break;
            }
            if (file.fail() && !file.eof()) {
                debug_print("Error reading file:", location);
                success = false;
                break;
            }
        }
    }

    archive_entry_free(entry);
}

void Archiver::AddFile(fs::directory_entry location){
    this->AddFile(location.path());
}

void Archiver::AddDirectory(fs::directory_entry location){
    //To speed up processing, we will add files in chunks instead of accessing to the file system for each file
    std::vector<fs::directory_entry> filesToArchive;
    const size_t maxNumberFilesInchunk = 100;

    for (const auto& entry : fs::recursive_directory_iterator(location, std::filesystem::directory_options::skip_permission_denied)) {
        if (std::filesystem::is_regular_file(std::filesystem::symlink_status(entry))) {
            if (filesToArchive.size() >= maxNumberFilesInchunk) {
                for (const auto& filePath : filesToArchive) {
                    this->AddFile(filePath);
                }
                filesToArchive.clear();
            }
            else {
                filesToArchive.push_back(entry);
            }
        }
    }

    //If there are any files left in the vector, process them and clear vector
    if(filesToArchive.size() > 0){
        debug_print("Processing remaining files in the buffer", filesToArchive.size());
        for (const auto& filePath : filesToArchive) {
            this->AddFile(filePath);
        }
        filesToArchive.clear();
    }

    // Process any remaining files in the batch
    for (const auto& filePath : filesToArchive) {
        this->AddFile(filePath);
    }
}

void Archiver::ArchiveItem(std::filesystem::directory_entry location){
    std::string path = location.path();
    size_t locationOfItemToArchiveInthePath = path.find_last_of(std::filesystem::path::preferred_separator);
    this->PathOfItemToArchive = path.substr(0, locationOfItemToArchiveInthePath+1);

    if(fs::is_directory(location)){
        this->AddDirectory(location);
    }
    else if(fs::is_regular_file(location)){
        this->AddFile(location);
    }
    else{
        debug_print("Unsupported file type", location.path());
    }
}

Archiver::~Archiver(){
    if(a != nullptr){
        archive_write_close(a);
        archive_write_free(a);
    }

    if(FileWithArchive.is_open()){
        FileWithArchive.close();
    }

    if(ArchiveFile != nullptr)
    {
        archive_write_close(ArchiveFile);
        archive_write_free(ArchiveFile);
    }
}
