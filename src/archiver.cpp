#include "archiver.h"
#include "ILibarchive_wrapper.h"
#include "logs.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <cstring> //for memset

#define DATA_BLOCK_SIZE 0x4000
        
/* This class provides multiple constructors, allowing it to be used in different ways depending on changing requirements:
 * - The user can provide their own function to specify items to archive during object execution.
 * - The user can provide a filename and then call the ArchiveItem method to archive a specific item.
 */
class Archiver::Impl {
public:
    Impl(std::unique_ptr<ILibArchiveWrapper> libarchive)
    : libarchive(std::move(libarchive)) {}

    /**
     * @brief Constructs an Archiver object and initializes the archive for writing.
     * 
     * This constructor creates a new archive object, sets up the compression filter
     * to use the XZ algorithm for minimal archive size, and configures the archive
     * format to be PAX restricted. It then attempts to open the specified file for
     * writing the archive. If any operation fails, an appropriate error message is
     * logged, and an exception is thrown.
     * 
     * @param filename The name of the file to be used for the archive.
     * 
     * @throws std::runtime_error If the archive file cannot be opened for writing.
     * 
     * @note The XZ compression algorithm is chosen for its ability to produce
     *       archives with minimal size.
     */
    Impl(std::string filename, std::unique_ptr<ILibArchiveWrapper> libarchive)
        : libarchive(std::move(libarchive)) {
        Archive = this->libarchive->archive_write_new();
        this->libarchive->archive_write_add_filter_xz(Archive);
        this->libarchive->archive_write_set_format_pax_restricted(Archive);

        if (this->libarchive->archive_write_open_filename(Archive, filename.c_str()) != ARCHIVE_OK) {
            throw std::runtime_error("Failed to open archive file");
        }
    }

    /**
     * @brief Constructs an Archiver object and initializes it with a default archive name.
     * 
     * This constructor also archives the location provided by the given IExplorer instance.
     * 
     * @param explorer A pointer to an IExplorer instance used to retrieve the location to be archived.
     */
    Impl(IExplorer* explorer, std::unique_ptr<ILibArchiveWrapper> libarchive) 
        : Impl("default_archive.tar.gz", std::move(libarchive)) {
        ArchiveItem(explorer->GetLocation());
    }

    /**
     * @brief Destructor for the Archiver class.
     * 
     * This destructor ensures proper cleanup of resources used by the Archiver object.
     * It performs the following actions:
     * - Closes and frees the main archive object if it is not null.
     * - Closes the file stream associated with the archive if it is open.
     * - Closes and frees any additional archive file object if it is not null.
     * 
     * Proper cleanup is essential to avoid resource leaks and ensure that all
     * allocated resources are released when the Archiver object is destroyed.
     */
    ~Impl() {
        if (Archive) {
            libarchive->archive_write_close(Archive);
            libarchive->archive_write_free(Archive);
        }

        if (Archive != nullptr) {
            libarchive->archive_write_close(Archive);
            libarchive->archive_write_free(Archive);
        }

        if (FileWithArchive.is_open()) {
            FileWithArchive.close();
        }

        if (ArchiveFile != nullptr) {
            libarchive->archive_write_close(ArchiveFile);
            libarchive->archive_write_free(ArchiveFile);
        }
    }

    /**
     * @brief Archives a specified item (file or directory) at the given location.
     *
     * This function determines whether the provided location corresponds to a 
     * directory or a regular file and processes it accordingly. Unsupported 
     * file types are logged for debugging purposes.
     *
     * @param location A std::filesystem::directory_entry representing the 
     *        file or directory to be archived.
     * 
     * @return Status indicating the result of the archiving operation:
     *         - Success: If the item was successfully archived.
     *         - Other status codes may indicate specific errors or issues.
     */
    Status ArchiveItem(std::filesystem::directory_entry location){
        //Use below line if archive shall contain the relative path.
        CutArchivePath(location.path());
        //Use below line if archive shall contain the absolute path.
        //PathOfItemToArchive = location.path();

        std::cout << "Operation in progress... " << std::endl;
        
        Status status = Success;
        if(fs::is_directory(location)){
            status = AddDirectory(location);
        }
        else if(fs::is_regular_file(location)){
            status = AddFile(location);
        }
        else{
            debug_print("Unsupported file type", location.path());
        }
        std::cout << "Operation finished!" << std::endl;

        return status;
    }

    /**
     * @brief Extracts the contents of an archive file to the specified location.
     *
     * This function reads an archive file, extracts its contents, and writes them
     * to the specified location on disk. It uses libarchive for handling archive
     * files and supports multiple archive formats and compression methods.
     *
     * @param location The file path of the archive to be extracted.
     * @return Status indicating the result of the extraction process:
     *         - Success: Extraction completed successfully.
     *         - CriticalError: Failed to create archive reader or writer.
     *         - CannotOpenFile: Failed to open the specified archive file.
     *         - AccessFileFailed: Failed to read or write archive headers or entries.
     *
     */
    Status Extract(std::string location){
        struct archive_entry *entry;
        int flags = ARCHIVE_EXTRACT_TIME | ARCHIVE_EXTRACT_PERM | ARCHIVE_EXTRACT_FFLAGS;
        int error_code;
        Status status = Success;

        std::cout << "Operation in progress... " << std::endl;

        /* archive reader configuration */
        Archive = libarchive->archive_read_new();
        if(Archive == NULL){
            debug_print("Failed to create archive reader");
            return CriticalError;
        }
        libarchive->archive_read_support_filter_all(Archive);
        libarchive->archive_read_support_format_all(Archive);

        /* configure creating elements on disk */
        ArchiveFile = libarchive->archive_write_disk_new();
        if(ArchiveFile == NULL){
            debug_print("Failed to create archive writer");
            return CriticalError;
        }
        libarchive->archive_write_disk_set_options(ArchiveFile, flags);
        libarchive->archive_write_disk_set_standard_lookup(ArchiveFile);
        
        error_code = libarchive->archive_read_open_filename(Archive, location.c_str(), DATA_BLOCK_SIZE);
        if(error_code != ARCHIVE_OK) {
            debug_print("Failed to open archive file", location);
            debug_print("error code: ", error_code);
            return CannotOpenFile;
        }

        do {
            error_code = libarchive->archive_read_next_header(Archive, &entry);
            if (error_code == ARCHIVE_EOF){
                break;
            }
            if (error_code < ARCHIVE_OK){
                status = AccessFileFailed;
                debug_print("Failed to read archive header", libarchive->archive_error_string(Archive));
                break;
            }

            error_code = libarchive->archive_write_header(ArchiveFile, entry);
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

        std::cout << "Operation finished!" << std::endl;

        /* clean up */
        libarchive->archive_read_close(Archive);
        libarchive->archive_read_free(Archive);
        Archive = NULL;
        libarchive->archive_write_close(ArchiveFile);
        libarchive->archive_write_free(ArchiveFile);
        ArchiveFile = NULL;

        return Success;
    }

private:
    private:
    /* private fields */
    std::unique_ptr<ILibArchiveWrapper> libarchive;
    struct archive* Archive = nullptr;
    struct archive* ArchiveFile = nullptr;
    std::ofstream FileWithArchive;
    std::string PathOfItemToArchive;

    /**
     * @brief Adds a file to the archive.
     *
     * This function creates a new archive entry for the specified file and writes
     * it to the archive. It handles setting the file's properties, such as size,
     * type, and permissions, and writes the file's data to the archive.
     *
     * @param location The full path to the file to be added to the archive.
     * 
     * @return Status indicating the success or failure of the operation.
     *         - Success: The file was successfully added to the archive.
     *         - WriteFailed: Failed to write the archive header or file data.
     *
     * @throws std::filesystem::filesystem_error If there is an error accessing the
     *         file's size.
     */
    Status AddFile(std::string location){
        Status status = Success;
        /* Create new entry to archive */
        struct archive_entry *entry = libarchive->archive_entry_new();
        /* Remove leading part of path and set the pathname in the archive */
        std::string locationInArchive;
        if (location.find(PathOfItemToArchive) == 0) {
            locationInArchive = location.substr(PathOfItemToArchive.length());
        } else {
        try {
            libarchive->archive_entry_set_size(entry, fs::file_size(location));
        } catch (const std::filesystem::filesystem_error& e) {
            debug_print("Failed to get file size for", location, ":", e.what());
            libarchive->archive_entry_set_size(entry, 0); // Set size to 0 as a fallback
        }
            locationInArchive = location; // Fallback to using the full path
        }
        debug_print("Adding file to archive:", locationInArchive);
        libarchive->archive_entry_set_pathname(entry, locationInArchive.c_str());

        libarchive->archive_entry_set_size(entry, fs::file_size(location));
        /* Set archive entry properties */
        libarchive->archive_entry_set_filetype(entry, AE_IFREG);
        /* set permissions */
        libarchive->archive_entry_set_perm(entry, 0644);
        

        if (libarchive->archive_write_header(Archive, entry) != ARCHIVE_OK) {
            debug_print("Failed to write archive header for", location, ":", libarchive->archive_error_string(Archive));
            status = WriteFailed;
        } else {
            status = WriteData(location);
        }

        libarchive->archive_entry_free(entry);

        return status;
    }

    /**
     * @brief Writes data from a specified file location to an archive.
     * 
     * This function reads data from the file located at the given path and writes
     * it to the archive in chunks of size DATA_BLOCK_SIZE. It handles errors
     * during file reading and archive writing, ensuring proper error reporting.
     * 
     * @param location A reference to a string containing the file path to read from.
     * @return Status Returns Success if the operation completes successfully, 
     *         or WriteFailed if an error occurs during file reading or archive writing.
     */
    Status WriteData(std::string &location)
    {
        Status status = Success;
        char buff[DATA_BLOCK_SIZE];
        memset(buff, 0, sizeof(buff));
        std::ifstream file(location, std::ios::binary);

        while (file.is_open())
        {
            file.read(buff, sizeof(buff));
            std::streamsize bytesRead = file.gcount();
            if (bytesRead > 0)
            {
                if (libarchive->archive_write_data(Archive, buff, bytesRead) < ARCHIVE_OK)
                {
                    debug_print("Failed to write data for", location, ":", libarchive->archive_error_string(Archive));
                    status = WriteFailed;
                    break;
                }
            }
            if (file.eof())
            {
                break;
            }
            if (file.fail() && !file.eof())
            {
                debug_print("Error reading file:", location);
                status = WriteFailed;
                break;
            }
        }

        return status;
    }

    /**
     * @brief Adds a file to the archiver from the specified directory entry.
     * 
     * This function takes a directory entry as input and adds the corresponding
     * file to the archiver by extracting its path and delegating to the overloaded
     * AddFile function.
     * 
     * @param location The directory entry representing the file to be added.
     * @return Status indicating the success or failure of the operation.
     */
    Status AddFile(fs::directory_entry location)
    {
        return AddFile(location.path());
    }

    /**
     * @brief Extracts and returns the directory path of the given file location.
     *
     * This function takes a file location as input, identifies the last occurrence
     * of the preferred path separator, and extracts the directory path up to and
     * including that separator. The extracted path is stored in the member variable
     * `PathOfItemToArchive` and returned as the result.
     *
     * @param location The full file path as a string.
     * @return The directory path of the file as a string.
     */
    std::string CutArchivePath(std::string location){
        size_t locationOfItemToArchiveInthePath = location.find_last_of(std::filesystem::path::preferred_separator);
        PathOfItemToArchive = location.substr(0, locationOfItemToArchiveInthePath+1);
        return PathOfItemToArchive;
    }
   
    /**
     * @brief Adds all files from a specified directory and its subdirectories to the archive.
     * 
     * This function processes files in chunks to optimize performance by reducing the number
     * of file system accesses. It skips directories and symbolic links, and only processes
     * regular files. If an error occurs while adding a file, the process continues, but a 
     * warning is logged for verification purposes.
     * 
     * @param location The directory entry representing the root directory to be archived.
     * @return Status Returns `Success` if all files were added successfully, or the first 
     *         encountered error status if any file addition fails.
     * 
     * @warning If an error occurs while adding a file, the process continues, but the user
     *          is advised to verify the archive for completeness.
     */
    Status AddDirectory(fs::directory_entry location){
        Status status = Success;
        //To speed up processing, we will add files in chunks instead of accessing to the file system for each file
        std::vector<fs::directory_entry> filesToArchive;
        const size_t maxNumberFilesInchunk = 100;

        for (const auto& entry : fs::recursive_directory_iterator(location, std::filesystem::directory_options::skip_permission_denied)) {
            if (std::filesystem::is_regular_file(std::filesystem::symlink_status(entry))) {
                if (filesToArchive.size() >= maxNumberFilesInchunk) {
                    for (const auto& filePath : filesToArchive) {
                        status = AddFile(filePath);
                        if(status != Success){
                            debug_print("Failed for file", filePath);
                            debug_print("Due to the significant reason of creating archive, the process will be continue but please verify the archive!");
                        }
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
                Status status_ex = AddFile(filePath);
                if(status_ex != Success && status == Success){
                    status == status_ex;
                }
            }
            filesToArchive.clear();
        }
        return status;
    }


    /**
     * @brief Archives the provided entry by reading its data and writing it to the archive file.
     *
     * This function processes an archive entry by reading its data in blocks and writing
     * those blocks to the destination archive file. It handles errors during both the
     * reading and writing processes and logs debug messages for failures or completion.
     *
     * @param entry Pointer to the archive entry to be processed.
     * @return Status Returns `Success` if the operation completes successfully, or 
     *         `AccessFileFailed` if there is an error finalizing the entry.
     *
     * @note The function assumes that `Archive` and `ArchiveFile` are properly initialized
     *       and valid.
     *
     * Error Handling:
     * - If reading data from the archive fails, the function logs the error and stops processing.
     * - If writing data to the archive file fails, the function logs the error and stops processing.
     * - If finalizing the entry fails, the function logs the error and returns `AccessFileFailed`.
     */
    Status ArchiveEntries(archive_entry* entry){
        Status status = Success;
        int error_code = 0;

        while(archive_entry_size(entry) > 0){
            const void *buff;
            size_t size;
            la_int64_t offset;
            error_code = libarchive->archive_read_data_block(Archive, (const void **)&buff, &size, &offset);
            if (error_code == ARCHIVE_EOF){
                break;
            }
            if (error_code < ARCHIVE_OK){
                debug_print("Failed to read archive data", libarchive->archive_error_string(Archive));
                break;
            }
            error_code = archive_write_data_block(ArchiveFile, buff, size, offset);
            if (error_code < ARCHIVE_OK){
                debug_print("Failed to write archive data", libarchive->archive_error_string(ArchiveFile));
                break;
            }
            debug_print("Finished", archive_entry_pathname(entry));
        }

        error_code = libarchive->archive_write_finish_entry(ArchiveFile);
        if (error_code < ARCHIVE_OK){
            debug_print(libarchive->archive_error_string(ArchiveFile));
            status = AccessFileFailed;
        }

        return status;
    }
};

Archiver::Archiver(std::unique_ptr<ILibArchiveWrapper> libarchive): pImpl(std::make_unique<Impl>(std::move(libarchive))){}

Archiver::Archiver(std::string filename, std::unique_ptr<ILibArchiveWrapper> libarchive) : pImpl(std::make_unique<Impl>(filename, std::move(libarchive))){}

Archiver::Archiver(IExplorer& explorer, std::unique_ptr<ILibArchiveWrapper> libarchive) : pImpl(std::make_unique<Impl>("default_archive.tar.gz", std::move(libarchive))) {
    pImpl->ArchiveItem(explorer.GetLocation());
}

Archiver::~Archiver() = default;

Status Archiver::Extract(std::string location) {
    return pImpl->Extract(location);
}

Status Archiver::ArchiveItem(fs::directory_entry location) {
    return pImpl->ArchiveItem(location);
}
