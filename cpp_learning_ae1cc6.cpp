// Learning Objective: This tutorial will introduce you to fundamental file and directory operations in C++ using the <filesystem> library.
// We will cover:
// 1. Checking if a file or directory exists.
// 2. Creating a new directory.
// 3. Writing data to a file.
// 4. Reading data from a file.
// 5. Iterating through files in a directory.
// 6. Renaming a file or directory.
// 7. Removing a file or directory.

#include <iostream>
#include <fstream>      // For file input/output operations (ifstream, ofstream)
#include <filesystem>   // The modern C++ library for filesystem operations
#include <string>       // For using the string data type
#include <vector>       // For storing multiple lines of text

namespace fs = std::filesystem; // Alias for convenience: use 'fs' instead of 'std::filesystem'

int main() {
    // Define some example paths for demonstration
    const fs::path dirPath = "my_demo_directory";
    const fs::path filePath = dirPath / "my_example_file.txt"; // Using the '/' operator to join path components

    // --- 1. Checking if a file or directory exists ---
    std::cout << "--- Checking Existence ---" << std::endl;

    // 'exists()' is a boolean function that returns true if the path points to an existing file or directory.
    if (fs::exists(filePath)) {
        std::cout << filePath << " already exists." << std::endl;
    } else {
        std::cout << filePath << " does not exist yet." << std::endl;
    }

    if (fs::exists(dirPath)) {
        std::cout << dirPath << " already exists." << std::endl;
    } else {
        std::cout << dirPath << " does not exist yet." << std::endl;
    }

    std::cout << std::endl; // Add a blank line for readability

    // --- 2. Creating a new directory ---
    std::cout << "--- Creating Directory ---" << std::endl;

    // 'create_directory()' attempts to create the specified directory.
    // It returns true if the directory was created successfully, false otherwise (e.g., if it already exists).
    if (fs::create_directory(dirPath)) {
        std::cout << "Directory '" << dirPath << "' created successfully." << std::endl;
    } else {
        std::cout << "Directory '" << dirPath << "' already exists or could not be created." << std::endl;
    }

    std::cout << std::endl;

    // --- 3. Writing data to a file ---
    std::cout << "--- Writing to File ---" << std::endl;

    // 'ofstream' is used for output file streams (writing to files).
    // We use it here to write some lines of text to our example file.
    // 'std::ios::app' means append to the file if it exists. Without it, the file would be overwritten.
    std::ofstream outputFile(filePath, std::ios::app); // Open file for appending

    if (outputFile.is_open()) { // Always check if the file was opened successfully
        outputFile << "This is the first line.\n"; // Write a string followed by a newline character
        outputFile << "This is the second line.\n";
        outputFile << "And a third line for good measure!\n";
        outputFile.close(); // It's good practice to close the file when done.
        std::cout << "Data written to '" << filePath << "'." << std::endl;
    } else {
        std::cerr << "Error: Unable to open '" << filePath << "' for writing." << std::endl; // 'cerr' is for error output
    }

    std::cout << std::endl;

    // --- 4. Reading data from a file ---
    std::cout << "--- Reading from File ---" << std::endl;

    // 'ifstream' is used for input file streams (reading from files).
    std::ifstream inputFile(filePath);
    std::string line; // Variable to hold each line read from the file.

    if (inputFile.is_open()) {
        std::cout << "Contents of '" << filePath << "':" << std::endl;
        // 'std::getline()' reads characters from the input stream until a newline character is found.
        while (std::getline(inputFile, line)) {
            std::cout << line << std::endl; // Print the line read from the file.
        }
        inputFile.close(); // Close the file.
    } else {
        std::cerr << "Error: Unable to open '" << filePath << "' for reading." << std::endl;
    }

    std::cout << std::endl;

    // --- 5. Iterating through files in a directory ---
    std::cout << "--- Iterating Directory ---" << std::endl;

    // 'directory_iterator' allows us to loop through the contents of a directory.
    // It yields entries (files and subdirectories) one by one.
    std::cout << "Contents of directory '" << dirPath << "':" << std::endl;
    for (const auto& entry : fs::directory_iterator(dirPath)) {
        // 'entry.path()' gives us the full path of the current item in the directory.
        // 'entry.is_directory()' checks if the item is a directory.
        // 'entry.is_regular_file()' checks if the item is a regular file.
        if (entry.is_regular_file()) {
            std::cout << "  File: " << entry.path().filename() << std::endl; // 'filename()' gets just the name part of the path.
        } else if (entry.is_directory()) {
            std::cout << "  Directory: " << entry.path().filename() << std::endl;
        } else {
            std::cout << "  Other: " << entry.path().filename() << std::endl;
        }
    }

    std::cout << std::endl;

    // --- 6. Renaming a file or directory ---
    std::cout << "--- Renaming File ---" << std::endl;
    const fs::path newFilePath = dirPath / "renamed_example_file.txt";

    // 'rename()' moves or renames a file or directory.
    // It takes the old path and the new path.
    // Be careful: if the new path exists, it might be overwritten depending on the OS.
    if (fs::exists(filePath)) {
        try {
            fs::rename(filePath, newFilePath);
            std::cout << "Renamed '" << filePath << "' to '" << newFilePath << "'." << std::endl;
        } catch (const fs::filesystem_error& e) {
            std::cerr << "Error renaming file: " << e.what() << std::endl;
        }
    } else {
        std::cout << "File '" << filePath << "' does not exist, cannot rename." << std::endl;
    }

    std::cout << std::endl;

    // --- 7. Removing a file or directory ---
    std::cout << "--- Removing File and Directory ---" << std::endl;

    // 'remove()' deletes a file.
    if (fs::exists(newFilePath)) {
        try {
            if (fs::remove(newFilePath)) {
                std::cout << "File '" << newFilePath << "' removed successfully." << std::endl;
            } else {
                std::cout << "File '" << newFilePath << "' could not be removed." << std::endl;
            }
        } catch (const fs::filesystem_error& e) {
            std::cerr << "Error removing file: " << e.what() << std::endl;
        }
    } else {
        std::cout << "File '" << newFilePath << "' does not exist, cannot remove." << std::endl;
    }

    // 'remove_all()' deletes a directory and all its contents.
    // Use with caution!
    if (fs::exists(dirPath)) {
        try {
            uintmax_t n = fs::remove_all(dirPath); // Returns the number of files/directories removed.
            std::cout << "Directory '" << dirPath << "' and its " << n << " contents removed successfully." << std::endl;
        } catch (const fs::filesystem_error& e) {
            std::cerr << "Error removing directory: " << e.what() << std::endl;
        }
    } else {
        std::cout << "Directory '" << dirPath << "' does not exist, cannot remove." << std::endl;
    }

    return 0; // Indicate successful execution.
}
// Example Usage:
// Compile this code with a C++17 compliant compiler (like g++ or clang++):
// g++ your_file_name.cpp -o file_demo -std=c++17 -lstdc++fs
// Then run the executable:
// ./file_demo
// You will see output detailing the operations. A directory named "my_demo_directory" will be created
// and then removed, along with the files it contained.