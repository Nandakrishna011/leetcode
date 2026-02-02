// Learning Objective:
// This tutorial teaches you how to implement a basic C++ system for
// serializing (writing) and deserializing (reading) custom objects
// to and from binary files. You'll learn how to handle primitive data types
// and std::string efficiently and correctly using fstream.
// Binary serialization is crucial for persistent storage of your application's data.

#include <iostream>  // For input/output operations (cout, cerr)
#include <fstream>   // For file stream operations (ifstream, ofstream)
#include <string>    // For std::string
// #include <vector> // Not strictly needed for *this* example, but often useful

// Define a custom object (struct) that we want to serialize and deserialize.
// Using a struct is common for plain data objects.
struct Student {
    int id;          // An integer member
    std::string name; // A string member
    double gpa;      // A floating-point member

    // Default constructor is important for deserialization,
    // as we might create an empty object and then fill its data.
    Student() : id(0), name(""), gpa(0.0) {}

    // Parameterized constructor for easy object creation.
    Student(int id_val, const std::string& name_val, double gpa_val)
        : id(id_val), name(name_val), gpa(gpa_val) {}

    // A helper method to print the student's data.
    void print() const {
        std::cout << "ID: " << id << ", Name: " << name << ", GPA: " << gpa << std::endl;
    }
};

// Function to serialize a Student object to an output stream (e.g., a binary file).
// We pass std::ostream by reference to allow writing to various stream types.
void serializeStudent(std::ostream& os, const Student& student) {
    // Explanation: For primitive types (int, double), their memory layout is
    // generally predictable. We can directly write their raw bytes to the file.
    // reinterpret_cast<const char*> converts the address of the variable
    // to a pointer to a character array, which os.write expects.
    // sizeof() tells us how many bytes to write.
    os.write(reinterpret_cast<const char*>(&student.id), sizeof(student.id));
    os.write(reinterpret_cast<const char*>(&student.gpa), sizeof(student.gpa));

    // Explanation: std::string is more complex. Its internal representation
    // (pointer to character data, length, capacity) is not guaranteed to be
    // consistent across compilers or even runs. Directly writing 'sizeof(string)'
    // would write internal pointers, not the string's actual characters.
    //
    // Therefore, we must manually serialize the string's length first,
    // and then its actual character data.
    size_t nameLength = student.name.length();
    // First, write the length of the string so we know how many characters to read later.
    os.write(reinterpret_cast<const char*>(&nameLength), sizeof(nameLength));
    // Then, write the actual character data of the string.
    // student.name.data() returns a pointer to the string's internal character array
    // (guaranteed to be null-terminated from C++11, but we don't write the null terminator itself).
    os.write(student.name.data(), nameLength);
}

// Function to deserialize a Student object from an input stream (e.g., a binary file).
// We pass std::istream by reference to allow reading from various stream types.
// The Student object is passed by reference so its members can be modified.
void deserializeStudent(std::istream& is, Student& student) {
    // Explanation: This is the reverse process of serialization.
    // For primitive types, we read the raw bytes back into the memory location
    // of the student's members.
    is.read(reinterpret_cast<char*>(&student.id), sizeof(student.id));
    is.read(reinterpret_cast<char*>(&student.gpa), sizeof(student.gpa));

    // Explanation: For std::string, we first read the length that we wrote earlier.
    size_t nameLength;
    is.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));

    // Then, we resize the string to hold the correct number of characters.
    // This pre-allocates memory for the string.
    student.name.resize(nameLength);
    // Finally, we read the actual character data directly into the string's buffer.
    // &student.name[0] gives a pointer to the beginning of the string's internal buffer,
    // which is mutable (writable) in C++11 and later.
    is.read(&student.name[0], nameLength);
}

int main() {
    const std::string filename = "students.bin"; // Name of our binary file

    // 1. Create a Student object to serialize.
    Student originalStudent(101, "Alice Smith", 3.85);
    std::cout << "Original Student: ";
    originalStudent.print();

    // 2. Serialize the student object to a binary file.
    // std::ofstream is used for writing to files.
    // std::ios::binary flag is crucial to ensure data is written as raw bytes,
    // preventing any text-mode conversions (like newline character translation).
    std::ofstream outFile(filename, std::ios::binary);
    if (!outFile.is_open()) {
        std::cerr << "Error: Could not open file for writing: " << filename << std::endl;
        return 1; // Indicate an error
    }

    serializeStudent(outFile, originalStudent);
    outFile.close(); // Always close the file stream when done.
    std::cout << "Student serialized to " << filename << std::endl;

    // 3. Deserialize the student object from the binary file.
    Student deserializedStudent; // Create an empty student object to fill.

    // std::ifstream is used for reading from files.
    // std::ios::binary is again crucial for reading raw bytes.
    std::ifstream inFile(filename, std::ios::binary);
    if (!inFile.is_open()) {
        std::cerr << "Error: Could not open file for reading: " << filename << std::endl;
        return 1; // Indicate an error
    }

    deserializeStudent(inFile, deserializedStudent);
    inFile.close(); // Always close the file stream when done.

    std::cout << "Student deserialized from " << filename << std::endl;
    std::cout << "Deserialized Student: ";
    deserializedStudent.print();

    // 4. Verify that the deserialized object matches the original.
    if (originalStudent.id == deserializedStudent.id &&
        originalStudent.name == deserializedStudent.name &&
        originalStudent.gpa == deserializedStudent.gpa) {
        std::cout << "Verification successful! Original and deserialized data match." << std::endl;
    } else {
        std::cerr << "Verification failed! Data mismatch." << std::endl;
    }

    return 0; // Indicate successful execution
}