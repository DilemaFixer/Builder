# Builder 🛠️

![Build Status](https://img.shields.io/badge/build-passing-brightgreen)
![License](https://img.shields.io/badge/license-MIT-blue)
![Language](https://img.shields.io/badge/language-C-lightgrey)
![Version](https://img.shields.io/badge/version-1.0.0-orange)

A lightweight utility library for C that provides cross-platform file system operations, string utilities, and command execution functions for Unix-like operating systems.

## 📥 Installation

### Unix (Linux/macOS)

```bash
curl -o b.h https://raw.githubusercontent.com/DilemaFixer/Cbuilder/main/b.h
```

## 🧰 API and Usage Examples

### 📝 Logging Utilities

The Builder library provides a comprehensive logging system with color-coded output and different severity levels.

```c
// Logging macros
#define LOG(level, color, fmt, ...)
#define ERROR(fmt, ...)    // Log error messages in red
#define WARN(fmt, ...)     // Log warning messages in yellow
#define INFO(fmt, ...)     // Log informational messages in green
#define DEBUG(fmt, ...)    // Log debug messages in blue
#define VERBOSE(fmt, ...)  // Log verbose messages in cyan
#define FAIL(fmt, ...)     // Log error message and exit program
#define TODO(note)         // Log a TODO note in magenta
```

#### Example:

```c
#include "b.h"

void example_logging() {
    INFO("Starting application");
    DEBUG("Debug information: %d", 42);
    WARN("This is a warning message");
    ERROR("An error occurred: %s", "file not found");
    VERBOSE("Additional details for troubleshooting");
    
    // Fatal error that exits the program
    // FAIL("Critical error: cannot continue");
    
    TODO("Implement error handling");
}
```

### 📁 File System Operations

Functions for working with files and directories.

```c
// File path utilities
char* exec_path();               // Get the path of the current executable
char* exec_dir();                // Get the directory of the current executable
char* pwd();                     // Get the current working directory
bool cd(const char* path);       // Change the current working directory

// File/directory status
bool file_exists(const char* path);      // Check if a file exists
bool dir_exists(const char* path);       // Check if a directory exists
bool is_exec(const char* path);          // Check if a file is executable
off_t file_size(const char* path);       // Get the size of a file
mode_t file_mode(const char* path);      // Get the permissions of a file

// Directory operations
bool make_dir(const char* path, mode_t mode);    // Create a directory
bool remove_dir(const char* path);               // Remove a directory
void list_dir(const char* path);                 // List directory contents

// File operations
bool touch_file(const char* path);                           // Create an empty file
bool touch_with_ext(const char* name, const char* ext);      // Create a file with extension
bool remove_file(const char* path);                          // Delete a file
bool copy_file(const char* src, const char* dst);            // Copy a file
bool move_file(const char* src, const char* dst);            // Move a file
bool rename_file(const char* old_path, const char* new_path);// Rename a file
bool write_to_file(const char* path, const char* content);   // Write string to file
char* read_from_file(const char* path);                      // Read file into string
void cat_file(const char* path);                             // Display file contents

// File permissions
bool change_mode(const char* path, mode_t mode);   // Change file permissions
bool make_symlink(const char* target, const char* link_path); // Create symbolic link

// Temporary files/directories
char* tmp_file();      // Create temporary file
char* tmp_dir();       // Create temporary directory
```

#### Example:

```c
#include "b.h"

void example_filesystem() {
    // Get current directory
    char* current_dir = pwd();
    INFO("Current directory: %s", current_dir);
    
    // Create a new directory
    make_dir("./test_dir", 0755);
    
    // Create a file and write to it
    write_to_file("./test_dir/example.txt", "Hello, Builder!");
    
    // Read file contents
    char* content = read_from_file("./test_dir/example.txt");
    INFO("File content: %s", content);
    
    // Copy file
    copy_file("./test_dir/example.txt", "./test_dir/example_copy.txt");
    
    // Clean up
    free(current_dir);
    free(content);
    remove_file("./test_dir/example.txt");
    remove_file("./test_dir/example_copy.txt");
    remove_dir("./test_dir");
}
```

### 🧵 String Utilities

Functions for string manipulation and path handling.

```c
// String utilities
char* strcat_new(const char* str1, const char* str2);              // Concatenate strings
char* strreplace(const char* str, const char* old_sub, const char* new_sub); // Replace substring
char* pathjoin(const char* path1, const char* path2);              // Join paths
char* path_basename(const char* path);                            // Get filename from path
char* path_dirname(const char* path);                             // Get directory name from path
char* extname(const char* path);                                  // Get file extension
char* joinstr(int count, ...);                                    // Join multiple strings
char* timestamp();                                                // Get current timestamp
char* datestamp();                                                // Get current date
```

#### Example:

```c
#include "b.h"

void example_strings() {
    // Join paths
    char* full_path = pathjoin("/usr/local", "bin");
    INFO("Full path: %s", full_path);
    
    // Get components from path
    char* dir = path_dirname(full_path);
    char* base = path_basename(full_path);
    char* ext = extname("document.txt");
    
    INFO("Directory: %s", dir);
    INFO("Basename: %s", base);
    INFO("Extension: %s", ext);
    
    // String manipulation
    char* str = strcat_new("Hello, ", "Builder!");
    char* replaced = strreplace(str, "Builder", "World");
    
    INFO("Concatenated: %s", str);
    INFO("Replaced: %s", replaced);
    
    // Get current date and time
    char* time = timestamp();
    char* date = datestamp();
    
    INFO("Current time: %s", time);
    INFO("Current date: %s", date);
    
    // Clean up
    free(full_path);
    free(dir);
    free(base);
    free(ext);
    free(str);
    free(replaced);
    free(time);
    free(date);
}
```

### 🔍 Command Execution and External Programs

Functions for executing external commands and working with the results.

```c
// Command execution
char* run_command(const char* cmd, ...);         // Run command and capture output
void RUN(...);                                   // Run command without capturing output
char* find_command(const char* cmd);             // Find path to executable
char* grep(const char* pattern, const char* file); // Search file for pattern
char* find_files(const char* dir, const char* pattern); // Find files by pattern
bool tar_create(const char* archive, const char* files); // Create tar archive
bool tar_extract(const char* archive, const char* dest); // Extract tar archive
bool wget_file(const char* url, const char* output);    // Download file
```

#### Example:

```c
#include "b.h"

void example_commands() {
    // Run a simple command
    RUN("echo", "Hello from Builder!");
    
    // Run a command and capture output
    char* ls_output = run_command("ls", "-la", NULL);
    INFO("Directory listing:\n%s", ls_output);
    
    // Find command in PATH
    char* gcc_path = find_command("gcc");
    if (gcc_path) {
        INFO("GCC found at: %s", gcc_path);
        free(gcc_path);
    }
    
    // Find files
    char* c_files = find_files(".", "*.c");
    INFO("C files in directory:\n%s", c_files);
    
    // Clean up
    free(ls_output);
    free(c_files);
}
```

### 📊 Dynamic Arrays

Functions for working with dynamic string arrays.

```c
// Dynamic array structure and functions
typedef struct {
    char** items;
    int count;
    int capacity;
} Array;

Array* array_new(int initial_capacity);           // Create new dynamic array
void array_add(Array* arr, const char* item);     // Add string to array
void array_free(Array* arr);                      // Free array memory
Array* find_all_files(const char* dir, const char* ext); // Find all files with extension
```

#### Example:

```c
#include "b.h"

void example_arrays() {
    // Create a new array
    Array* fruits = array_new(5);
    
    // Add items
    array_add(fruits, "Apple");
    array_add(fruits, "Banana");
    array_add(fruits, "Orange");
    
    // Use the array
    INFO("Fruits array has %d items:", fruits->count);
    for (int i = 0; i < fruits->count; i++) {
        INFO("  %d: %s", i+1, fruits->items[i]);
    }
    
    // Find all .c files in the current directory
    Array* c_files = find_all_files(".", "c");
    if (c_files) {
        INFO("Found %d .c files:", c_files->count);
        for (int i = 0; i < c_files->count; i++) {
            INFO("  %s", c_files->items[i]);
        }
        array_free(c_files);
    }
    
    // Clean up
    array_free(fruits);
}
```

### ⚙️ Command Line Argument Processing

Functions for handling command-line arguments.

```c
// Command line argument handling
void shift(int* argc, char*** argv);                 // Remove first argument
char* get_arg(int argc, char** argv, int index);     // Get argument at index
bool has_arg(int argc, char** argv, const char* arg); // Check if argument exists
char* get_arg_value(int argc, char** argv, const char* arg); // Get value after argument
```

#### Example:

```c
#include "b.h"

int main(int argc, char** argv) {
    // Check for help flag
    if (has_arg(argc, argv, "--help") || has_arg(argc, argv, "-h")) {
        INFO("Usage: program [options]");
        return 0;
    }
    
    // Get output file
    char* output_file = get_arg_value(argc, argv, "--output");
    if (output_file) {
        INFO("Output file: %s", output_file);
    } else {
        INFO("No output file specified, using default");
    }
    
    // Process remaining arguments
    INFO("Processing %d arguments:", argc);
    for (int i = 0; i < argc; i++) {
        INFO("  Argument %d: %s", i, get_arg(argc, argv, i));
    }
    
    // Remove first argument and continue processing
    shift(&argc, &argv);
    INFO("After shift, first argument is: %s", argv[0]);
    
    return 0;
}
```
