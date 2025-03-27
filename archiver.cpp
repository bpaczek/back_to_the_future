#include "archiver.h"
#include "logs.h"

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

    this->CurrentLocation = filename;

    try{
        this->ArchiveFile.open(filename, std::ios::in | std::ios::binary);
    
        if(!this->ArchiveFile.is_open()){
             debug_print("Failed to open file", this->CurrentLocation);
        }
    }
    catch(const std::exception& e)
    {
        debug_print("Failed to create an archive!", this->CurrentLocation, ". ", e.what());
    }
}

void Archiver::Extract(std::string location){
    struct archive *a;
    struct archive *ext;
    struct archive_entry *entry;
    int flags = ARCHIVE_EXTRACT_TIME | ARCHIVE_EXTRACT_PERM | ARCHIVE_EXTRACT_FFLAGS;
    int error_code;

    /* archive reader configuration */
    a = archive_read_new();
    archive_read_support_filter_all(a);
    archive_read_support_format_all(a);

    /* configure creating elements on disk */
    ext = archive_write_disk_new();
    archive_write_disk_set_options(ext, flags);
    archive_write_disk_set_standard_lookup(ext);

    if ((error_code = archive_read_open_filename(a, location.c_str(), DATA_BLOCK_SIZE))) {
        debug_print("Failed to open archive file", location);
        throw std::runtime_error("Failed to open archive file");
    }

    do {
        error_code = archive_read_next_header(a, &entry);
        if (error_code == ARCHIVE_EOF){
            break;
        }
        if (error_code < ARCHIVE_OK){
            debug_print("Failed to read archive header", archive_error_string(a));
        }
        if (error_code < ARCHIVE_WARN){
            debug_print("Failed to read archive header", archive_error_string(a));
            break;
        }

        error_code = archive_write_header(ext, entry);
        if (error_code < ARCHIVE_OK){
            debug_print("Failed to write archive header", location);
        }
        else if(archive_entry_size(entry) > 0){
            while(true){
                const void *buff;
                size_t size;
                la_int64_t offset;
                error_code = archive_read_data_block(a, (const void **)&buff, &size, &offset);
                if (error_code == ARCHIVE_EOF){
                    break;
                }
                if (error_code < ARCHIVE_OK){
                    debug_print("Failed to read archive data", archive_error_string(a));
                }
                error_code = archive_write_data_block(ext, buff, size, offset);
                if (error_code < ARCHIVE_OK){
                    debug_print("Failed to write archive data", archive_error_string(a));
                }
            }
            error_code = archive_write_finish_entry(ext);
            if (error_code < ARCHIVE_OK){
                debug_print(archive_error_string(ext));
                break;
            }
            if (error_code < ARCHIVE_WARN){
                debug_print(archive_error_string(ext));
                break;
            }
        }
    } while(true);

    archive_read_close(a);
    archive_read_free(a);
    archive_write_close(ext);
    archive_write_free(ext);
}

void Archiver::AddFile(std::string location){
    /* Create new entry to archive */
    struct archive_entry *entry = archive_entry_new();
    /* Remove leading part of path and set the pathname in the archive */
    auto locationInArchive = location.substr(this->PathOfItemToArchive.length());
    debug_print("Adding file to archive:", locationInArchive);
    archive_entry_set_pathname(entry, locationInArchive.c_str());

    archive_entry_set_size(entry, fs::file_size(location));
    /* Set archive entry properties */
    archive_entry_set_filetype(entry, AE_IFREG); //check AE_IFREG
    /* set permissions */
    archive_entry_set_perm(entry, 0644); //consider to change to 0777
    
    archive_write_header(a, entry);
    char buff[DATA_BLOCK_SIZE];
    std::ifstream file(location, std::ios::binary);
    /* Write data to the archive as long as the stream does not have any error flags set, such as end-of-file, fail, or bad. */
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
    if(this->ArchiveFile.is_open()){
        this->ArchiveFile.close();
    }
}
