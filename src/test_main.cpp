#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "explorer.h"
#include "archiver.h"
#include "logs.h"
#include "status.h"

// Mock Explorer
class MockExplorer : public Explorer {
public:
    MOCK_METHOD(Status, SelectItemToArchive, (std::filesystem::directory_entry*), (override));
};

//Mock Archiver
class MockArchiver : public Archiver {
public:
    MockArchiver(const std::string& archiveName) : Archiver(archiveName) {}
    MOCK_METHOD(Status, ArchiveItem, (const std::filesystem::directory_entry&), (override));
};

// Test suite for pack_mode
TEST(PackModeTest, Success_UserExit) {
    MockExplorer mockExplorer;
    EXPECT_CALL(mockExplorer, SelectItemToArchive(testing::_))
        .WillOnce(testing::Return(UserExit));

    pack_mode();
    Explorer* explorer = &mockExplorer;
    std::filesystem::directory_entry entry;
    auto stat = explorer->SelectItemToArchive(&entry);

    EXPECT_EQ(stat, UserExit);
}

TEST(PackModeTest, SuccessfulArchiving) {
    MockExplorer mockExplorer;
    MockArchiver mockArchiver(DEFAULT_ARCHIVE_NAME);

    std::filesystem::directory_entry mockEntry("/mock/path");
    EXPECT_CALL(mockExplorer, SelectItemToArchive(testing::_))
        .WillOnce(testing::DoAll(testing::SetArgPointee<0>(mockEntry), testing::Return(Success)));
    EXPECT_CALL(mockArchiver, ArchiveItem(testing::_))
        .WillOnce(testing::Return(Success));

    Explorer* explorer = &mockExplorer;
    std::filesystem::directory_entry entry;
    auto stat = explorer->SelectItemToArchive(&entry);
    EXPECT_EQ(stat, Success);

    auto archiveStatus = mockArchiver.ArchiveItem(entry);
    EXPECT_EQ(archiveStatus, Success);
}

TEST(PackModeTest, ArchivingFailure) {
    MockExplorer mockExplorer;
    MockArchiver mockArchiver(DEFAULT_ARCHIVE_NAME);

    std::filesystem::directory_entry mockEntry("/dev/null");
    EXPECT_CALL(mockExplorer, SelectItemToArchive(testing::_))
        .WillOnce(testing::DoAll(testing::SetArgPointee<0>(mockEntry), testing::Return(Success)));
    EXPECT_CALL(mockArchiver, ArchiveItem(testing::_))
        .WillOnce(testing::Return(Failure));

    Explorer* explorer = &mockExplorer;
    std::filesystem::directory_entry entry;
    auto stat = explorer->SelectItemToArchive(&entry);
    EXPECT_EQ(stat, Success);

    auto archiveStatus = mockArchiver.ArchiveItem(entry);
    EXPECT_EQ(archiveStatus, Failure);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}