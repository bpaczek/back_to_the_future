// #include "libarchive_wrapper.h"

// struct archive* LibArchiveWrapper::archive_read_new(){
//     return ::archive_read_new();
// }

// int LibArchiveWrapper::archive_read_support_filter_all(struct archive* a){
//     return ::archive_read_support_filter_all(a);
// }

// int LibArchiveWrapper::archive_read_support_format_all(struct archive* a){
//     return ::archive_read_support_format_all(a);
// }

// int LibArchiveWrapper::archive_read_open_filename(struct archive* a, const char* filename, size_t block_size){
//     return ::archive_read_open_filename(a, filename, block_size);
// }

// int LibArchiveWrapper::archive_read_next_header(struct archive* a, struct archive_entry** entry){
//     return ::archive_read_next_header(a, entry);
// }

// int LibArchiveWrapper::archive_read_data_block(struct archive* a, const void** buff, size_t* size, int64_t* offset){
//     return ::archive_read_data_block(a, buff, size, offset);
// }

// int LibArchiveWrapper::archive_read_free(struct archive* a){
//     return ::archive_read_free(a);
// }

// struct archive* LibArchiveWrapper::archive_write_new(){
//     return ::archive_write_new();
// }

// int LibArchiveWrapper::archive_write_add_filter_xz(struct archive* a){
//     return ::archive_write_add_filter_xz(a);
// }

// int LibArchiveWrapper::archive_write_set_format_pax_restricted(struct archive* a){
//     return ::archive_write_set_format_pax_restricted(a);
// }

// int LibArchiveWrapper::archive_write_open_filename(struct archive* a, const char* filename){
//     return ::archive_write_open_filename(a, filename);
// }

// int LibArchiveWrapper::archive_write_header(struct archive* a, struct archive_entry* entry){
//     return ::archive_write_header(a, entry);
// }

// int LibArchiveWrapper::archive_write_data(struct archive* a, const void* buff, size_t size){
//     return ::archive_write_data(a, buff, size);
// }

// int LibArchiveWrapper::archive_write_close(struct archive* a){
//     return ::archive_write_close(a);
// }

// int LibArchiveWrapper::archive_write_free(struct archive* a){
//     return ::archive_write_free(a);
// }

// struct archive_entry* LibArchiveWrapper::archive_entry_new(){
//     return ::archive_entry_new();
// }

// void LibArchiveWrapper::archive_entry_free(struct archive_entry* entry){
//     ::archive_entry_free(entry);
// }

// void LibArchiveWrapper::archive_entry_set_pathname(struct archive_entry* entry, const char* pathname){
//     ::archive_entry_set_pathname(entry, pathname);
// }

// void LibArchiveWrapper::archive_entry_set_size(struct archive_entry* entry, int64_t size){
//     ::archive_entry_set_size(entry, size);
// }

// void LibArchiveWrapper::archive_entry_set_filetype(struct archive_entry* entry, unsigned int filetype){
//     ::archive_entry_set_filetype(entry, filetype);
// }

// void LibArchiveWrapper::archive_entry_set_perm(struct archive_entry* entry, unsigned int perm){
//     ::archive_entry_set_perm(entry, perm);
// }