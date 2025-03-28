# BTTF Project

## Overview
The **BTTF** project is a C++ application designed to handle file archiving and extraction using the `libarchive` library. It includes modular components for managing archives, exploring file systems, and integrating with Google Test for unit testing.

---

## Features
- File archiving and extraction using `libarchive`.
- Modular design with interfaces for flexibility and testability.
- Unit tests implemented with Google Test.

---

## Requirements
- **C++ Compiler**: Requires C++17 support.
- **CMake**: Version 3.10 or higher.
- **Libraries**:
  - `libarchive` (ensure `archive.h` and `libarchive.a` are installed in `/usr/local/include` and `/usr/local/lib`).
  - Google Test (included as a submodule in `thirdparty/googletest`).

---

## Build Instructions
1. Clone the repository:
   ```bash
   git clone <repository-url>
   cd BTTF```

2. Configure the project
   `cmake -S . -B build`

3. Build project
   `cmake --build build`

4. Output Files
- **Executables**: After building, the executable will be located in build directory: '/build'BTTF'
- **Test binaries**: Test binaries will be located in: 'build/tests' 

## Project Structure
 - src/: Source code for the main application.
 - inc/: Header files for the project.
 - tests/: Unit tests for the application.
 - thirdparty/googletest/: Google Test framework.

## Usage Instructions

The **BTTF** tool allows you to archive files or unpack an existing archive. Below are the usage instructions:

### Archivization Mode
1. To create an archive, simply run the tool without any arguments:
`./BTTF`

2. Unpack an archive:
`./BTTF <archive_file_name`



