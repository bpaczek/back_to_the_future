#ifndef LIBARCHIVE_WRAPPER_H
#define LIBARCHIVE_WRAPPER_H

#include "ILibarchive_wrapper.h"

class LibArchiveWrapper : public ILibArchiveWrapper {
public:
    struct archive* archive_read_new() override {
        return ::archive_read_new();
    }

    int archive_read_support_filter_all(struct archive* a) override {
        return ::archive_read_support_filter_all(a);
    }

    int archive_read_support_format_all(struct archive* a) override {
        return ::archive_read_support_format_all(a);
    }

    int archive_read_open_filename(struct archive* a, const char* filename, size_t block_size) override {
        return ::archive_read_open_filename(a, filename, block_size);
    }

    int archive_read_next_header(struct archive* a, struct archive_entry** entry) override {
        return ::archive_read_next_header(a, entry);
    }

    int archive_read_data_block(struct archive* a, const void** buff, size_t* size, int64_t* offset) override {
        return ::archive_read_data_block(a, buff, size, offset);
    }

    int archive_read_free(struct archive* a) override {
        return ::archive_read_free(a);
    }

    struct archive* archive_write_new() override {
        return ::archive_write_new();
    }

    int archive_write_add_filter_xz(struct archive* a) override {
        return ::archive_write_add_filter_xz(a);
    }

    int archive_write_set_format_pax_restricted(struct archive* a) override {
        return ::archive_write_set_format_pax_restricted(a);
    }

    int archive_write_open_filename(struct archive* a, const char* filename) override {
        return ::archive_write_open_filename(a, filename);
    }

    int archive_write_header(struct archive* a, struct archive_entry* entry) override {
        return ::archive_write_header(a, entry);
    }

    int archive_write_data(struct archive* a, const void* buff, size_t size) override {
        return ::archive_write_data(a, buff, size);
    }

    int archive_write_close(struct archive* a) override {
        return ::archive_write_close(a);
    }

    int archive_write_free(struct archive* a) override {
        return ::archive_write_free(a);
    }

    struct archive_entry* archive_entry_new() override {
        return ::archive_entry_new();
    }

    void archive_entry_free(struct archive_entry* entry) override {
        ::archive_entry_free(entry);
    }

    void archive_entry_set_pathname(struct archive_entry* entry, const char* pathname) override {
        ::archive_entry_set_pathname(entry, pathname);
    }

    void archive_entry_set_size(struct archive_entry* entry, int64_t size) override {
        ::archive_entry_set_size(entry, size);
    }

    void archive_entry_set_filetype(struct archive_entry* entry, unsigned int filetype) override {
        ::archive_entry_set_filetype(entry, filetype);
    }

    void archive_entry_set_perm(struct archive_entry* entry, unsigned int perm) override {
        ::archive_entry_set_perm(entry, perm);
    }

    int archive_write_disk_set_options(struct archive* a, int flags) {
        return ::archive_write_disk_set_options(a, flags);
    }
    
    const char* archive_error_string(struct archive* a) {
        return ::archive_error_string(a);
    }

    int archive_write_disk_set_standard_lookup(struct archive* a) {
        return ::archive_write_disk_set_standard_lookup(a);
    }

    struct archive* archive_write_disk_new() {
        return ::archive_write_disk_new();
    }

    int archive_write_finish_entry(struct archive* a) {
        return ::archive_write_finish_entry(a);
    }

    int archive_read_close(struct archive* a) {
        return ::archive_read_close(a);
    }

    int64_t archive_entry_size(struct archive_entry* entry) {
        return ::archive_entry_size(entry);
    }
    
    const char* archive_entry_pathname(struct archive_entry* entry) {
        return ::archive_entry_pathname(entry);
    }
    
    int archive_write_data_block(struct archive* a, const void* buff, size_t size, int64_t offset) {
        return ::archive_write_data_block(a, buff, size, offset);
    }
};

#endif
