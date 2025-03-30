#include "b.h"

// Compiles a single .c file into an .o file
bool compile_file(const char *src_file, const char *obj_file) {
  INFO("Compiling %s to %s", src_file, obj_file);

  char *result = run_command("gcc", "-c", "-o", obj_file, src_file, "-Wall",
                             "-Werror", NULL);
  if (result) {
    free(result);
    return file_exists(obj_file);
  }

  return false;
}

// Links all .o files into an executable file
bool link_files(const char *output, Array *obj_files) {
  if (!obj_files || obj_files->count == 0) {
    ERROR("No object files for linking");
    return false;
  }

  INFO("Linking files into %s", output);

  char *cmd = strdup("gcc -o ");
  cmd = strcat_new(cmd, output);

  for (int i = 0; i < obj_files->count; i++) {
    cmd = strcat_new(cmd, " ");
    cmd = strcat_new(cmd, obj_files->items[i]);
  }

  VERBOSE("Executing command: %s", cmd);
  int result = system(cmd);
  free(cmd);

  return result == 0 && file_exists(output);
}

// Runs the built program
bool run_program(const char *program) {
  INFO("Running program %s", program);

  if (!file_exists(program)) {
    ERROR("Program %s does not exist", program);
    return false;
  }

  if (!is_exec(program)) {
    ERROR("File %s is not executable", program);
    return false;
  }

  char *cmd = strcat_new("./", program);
  int result = system(cmd);
  free(cmd);

  return result == 0;
}

int main(int argc, char **argv) {
  // Directory names
  const char *src_dir = "src";
  const char *obj_dir = "obj";
  const char *bin_dir = "bin";
  const char *output_name = "program";
  bool should_run = false;

  // Check command line arguments
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "--run") == 0 || strcmp(argv[i], "run") == 0) {
      should_run = true;
    } else if (strcmp(argv[i], "--out") == 0 && i + 1 < argc) {
      output_name = argv[++i];
    }
  }

  // Check if directories exist
  if (!dir_exists(src_dir)) {
    FAIL("Source code directory %s does not exist", src_dir);
  }

  // Create directories if they don't exist
  if (!dir_exists(obj_dir)) {
    INFO("Creating directory for object files %s", obj_dir);
    if (!make_dir(obj_dir, 0755)) {
      FAIL("Failed to create directory %s", obj_dir);
    }
  }

  if (!dir_exists(bin_dir)) {
    INFO("Creating directory for executable files %s", bin_dir);
    if (!make_dir(bin_dir, 0755)) {
      FAIL("Failed to create directory %s", bin_dir);
    }
  }

  // Find all .c files in the src directory
  INFO("Searching for source files in %s", src_dir);
  Array *src_files = find_all_files(src_dir, "c");

  if (!src_files || src_files->count == 0) {
    FAIL("No source .c files found in directory %s", src_dir);
  }

  INFO("Found %d source files", src_files->count);

  // Array to store paths to object files
  Array *obj_files = array_new(src_files->count);

  // Compile each file
  for (int i = 0; i < src_files->count; i++) {
    char *src_file = src_files->items[i];
    char *base_name = path_basename(src_file);

    // Replace .c extension with .o
    char *obj_name = strreplace(base_name, ".c", ".o");
    char *obj_file = pathjoin(obj_dir, obj_name);

    if (compile_file(src_file, obj_file)) {
      array_add(obj_files, obj_file);
    } else {
      ERROR("Error compiling %s", src_file);
    }

    free(base_name);
    free(obj_name);
    free(obj_file);
  }

  // Check if all files were successfully compiled
  if (obj_files->count != src_files->count) {
    ERROR("Only %d out of %d files compiled", obj_files->count,
          src_files->count);
  } else {
    INFO("All files successfully compiled");
  }

  // Link object files into an executable file
  char *output_path = pathjoin(bin_dir, output_name);

  if (link_files(output_path, obj_files)) {
    INFO("Program successfully built: %s", output_path);

    // Make the file executable
    change_mode(output_path, 0755);

    // Run the program if the --run flag is specified
    if (should_run) {
      INFO("Running program...");
      run_program(output_path);
    }
  } else {
    ERROR("Error linking program");
  }

  // Free memory
  free(output_path);
  array_free(src_files);
  array_free(obj_files);

  return 0;
}
