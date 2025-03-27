#ifndef ILIBARCHIVE_WRAPPER_H
#define ILIBARCHIVE_WRAPPER_H

#include <archive.h>
#include <archive_entry.h>
#include <string>

class ILibArchiveWrapper {
public:
    virtual ~ILibArchiveWrapper() = default;

    virtual struct archive* archive_read_new() = 0;
    virtual int archive_read_support_filter_all(struct archive* a) = 0;
    virtual int archive_read_support_format_all(struct archive* a) = 0;
    virtual int archive_read_open_filename(struct archive* a, const char* filename, size_t block_size) = 0;
    virtual int archive_read_next_header(struct archive* a, struct archive_entry** entry) = 0;
    virtual int archive_read_data_block(struct archive* a, const void** buff, size_t* size, int64_t* offset) = 0;
    virtual int archive_read_free(struct archive* a) = 0;
    virtual struct archive* archive_write_new() = 0;
    virtual int archive_write_add_filter_xz(struct archive* a) = 0;
    virtual int archive_write_set_format_pax_restricted(struct archive* a) = 0;
    virtual int archive_write_open_filename(struct archive* a, const char* filename) = 0;
    virtual int archive_write_header(struct archive* a, struct archive_entry* entry) = 0;
    virtual int archive_write_data(struct archive* a, const void* buff, size_t size) = 0;
    virtual int archive_write_close(struct archive* a) = 0;
    virtual int archive_write_free(struct archive* a) = 0;
    virtual struct archive_entry* archive_entry_new() = 0;
    virtual void archive_entry_free(struct archive_entry* entry) = 0;
    virtual void archive_entry_set_pathname(struct archive_entry* entry, const char* pathname) = 0;
    virtual void archive_entry_set_size(struct archive_entry* entry, int64_t size) = 0;
    virtual void archive_entry_set_filetype(struct archive_entry* entry, unsigned int filetype) = 0;
    virtual void archive_entry_set_perm(struct archive_entry* entry, unsigned int perm) = 0;
    virtual int archive_write_disk_set_options(struct archive* a, int flags) = 0;
    virtual const char* archive_error_string(struct archive* a) = 0;
    virtual int archive_write_disk_set_standard_lookup(struct archive* a) = 0;
    virtual struct archive* archive_write_disk_new() = 0;
    virtual int archive_write_finish_entry(struct archive* a) = 0;
    virtual int archive_read_close(struct archive* a) = 0;
};

#endif