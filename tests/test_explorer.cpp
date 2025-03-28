#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "explorer.h"
#include "status.h"
#include <filesystem>
#include <sstream>
#include <fstream>

// Mock std::cin for testing user input
class MockCin {
public:
    MockCin(const std::string& input) {
        std::cin.rdbuf(inputStream.rdbuf());
        inputStream.str(input);
    }

    ~MockCin() {
        std::cin.rdbuf(originalBuffer);
    }

private:
    std::streambuf* originalBuffer = std::cin.rdbuf();
    std::stringstream inputStream;
};

// Test case: User exits the selection process
TEST(ExplorerTest, SelectItemToArchive_UserExit) {
    Explorer explorer;
    MockCin mockInput("X\n"); // Simulate user input "X" to exit

    std::filesystem::directory_entry selectedLocation;
    Status status = explorer.SelectItemToArchive(&selectedLocation);

    EXPECT_EQ(status, UserExit);
}

// Test case: User selects a valid file
TEST(ExplorerTest, SelectItemToArchive_ValidFileSelection) {
    // Create a temporary directory and file for testing
    std::filesystem::path tempDir = std::filesystem::temp_directory_path() / "test_explorer";
    std::filesystem::create_directory(tempDir);
    std::filesystem::path tempFile = tempDir / "test_file.txt";
    std::ofstream(tempFile) << "Test content";

    Explorer explorer(tempDir.string());
    MockCin mockInput("0\n"); // Simulate user input "0" to select the first item

    std::filesystem::directory_entry selectedLocation;
    Status status = explorer.SelectItemToArchive(&selectedLocation);

    EXPECT_EQ(status, Success);
    EXPECT_EQ(selectedLocation.path(), tempFile);

    // Cleanup
    std::filesystem::remove(tempFile);
    std::filesystem::remove(tempDir);
}
