#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "archiver.h"
#include "ILibarchive_wrapper.h"
#include "status.h"

extern "C"{
#include <archive.h>
#include <archive_entry.h>
}

using ::testing::_;
using ::testing::Return;

// Mock class for ILibArchiveWrapper
class MockLibArchiveWrapper : public ILibArchiveWrapper {
    public:
        MOCK_METHOD(struct archive*, archive_read_new, (), (override));
        MOCK_METHOD(int, archive_read_support_filter_all, (struct archive*), (override));
        MOCK_METHOD(int, archive_read_support_format_all, (struct archive*), (override));
        MOCK_METHOD(int, archive_read_open_filename, (struct archive*, const char*, size_t), (override));
        MOCK_METHOD(int, archive_read_next_header, (struct archive*, struct archive_entry**), (override));
        MOCK_METHOD(int, archive_read_data_block, (struct archive*, const void**, size_t*, int64_t*), (override));
        MOCK_METHOD(int, archive_read_close, (struct archive*), (override));
        MOCK_METHOD(int, archive_read_free, (struct archive*), (override));
        MOCK_METHOD(struct archive*, archive_write_new, (), (override));
        MOCK_METHOD(int, archive_write_add_filter_xz, (struct archive*), (override));
        MOCK_METHOD(int, archive_write_set_format_pax_restricted, (struct archive*), (override));
        MOCK_METHOD(int, archive_write_open_filename, (struct archive*, const char*), (override));
        MOCK_METHOD(int, archive_write_header, (struct archive*, struct archive_entry*), (override));
        MOCK_METHOD(int, archive_write_data, (struct archive*, const void*, size_t), (override));
        MOCK_METHOD(int, archive_write_close, (struct archive*), (override));
        MOCK_METHOD(int, archive_write_free, (struct archive*), (override));
        MOCK_METHOD(struct archive_entry*, archive_entry_new, (), (override));
        MOCK_METHOD(void, archive_entry_free, (struct archive_entry*), (override));
        MOCK_METHOD(void, archive_entry_set_pathname, (struct archive_entry*, const char*), (override));
        MOCK_METHOD(void, archive_entry_set_size, (struct archive_entry*, int64_t), (override));
        MOCK_METHOD(void, archive_entry_set_filetype, (struct archive_entry*, unsigned int), (override));
        MOCK_METHOD(void, archive_entry_set_perm, (struct archive_entry*, unsigned int), (override));
        MOCK_METHOD(int, archive_write_disk_set_options, (struct archive*, int), (override));
        MOCK_METHOD(const char*, archive_error_string, (struct archive*), (override));
        MOCK_METHOD(int, archive_write_disk_set_standard_lookup, (struct archive*), (override));
        MOCK_METHOD(struct archive*, archive_write_disk_new, (), (override));
        MOCK_METHOD(int, archive_write_finish_entry, (struct archive*), (override));
        MOCK_METHOD(int64_t, archive_entry_size, (struct archive_entry*), (override));
        MOCK_METHOD(const char*, archive_entry_pathname, (struct archive_entry*), (override));
        MOCK_METHOD(int, archive_write_data_block, (struct archive*, const void*, size_t, int64_t), (override));
    };

// Test case: Extract returns CriticalError when archive_read_new() returns NULL
TEST(ArchiverTest, Extract_ReturnsCriticalError_WhenArchiveReadNewReturnsNull) {
    auto mockLibArchive = std::make_unique<MockLibArchiveWrapper>();
    EXPECT_CALL(*mockLibArchive, archive_read_new()).WillOnce(Return(nullptr));

    Archiver archiver(std::move(mockLibArchive));
    Status status = archiver.Extract("test_archive.tar.gz");

    EXPECT_EQ(status, CriticalError);
}

// Test case: Extract returns CannotOpenFile when archive_read_open_filename() fails
TEST(ArchiverTest, Extract_ReturnsCannotOpenFile_WhenArchiveReadOpenFilenameFails) {
    auto mockLibArchive = std::make_unique<MockLibArchiveWrapper>();
    struct archive* mockArchive;
    EXPECT_CALL(*mockLibArchive, archive_read_new()).WillOnce(Return(mockArchive));
    EXPECT_CALL(*mockLibArchive, archive_read_support_filter_all(mockArchive)).Times(1);
    EXPECT_CALL(*mockLibArchive, archive_read_support_format_all(mockArchive)).Times(1);
    EXPECT_CALL(*mockLibArchive, archive_write_disk_new()).WillOnce(Return(mockArchive));
    EXPECT_CALL(*mockLibArchive, archive_write_disk_set_options(mockArchive, _)).Times(1);
    EXPECT_CALL(*mockLibArchive, archive_write_disk_set_standard_lookup(mockArchive)).Times(1);
    EXPECT_CALL(*mockLibArchive, archive_read_open_filename(mockArchive, _, _)).WillOnce(Return(ARCHIVE_FATAL));

    Archiver archiver(std::move(mockLibArchive));
    Status status = archiver.Extract("test_archive.tar.gz");

    EXPECT_EQ(status, CannotOpenFile);
}
