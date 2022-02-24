#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <stdio.h>
#include <stdbool.h>

bool is_file_exists(const char* filename);
bool is_file_empty(FILE* file);
void dir_name(char* path, char* dirname);
bool create_dir(char* dirname);
bool redirect_stdout(char* path);
FILE* open_file(char* filename);

#endif  // FILE_UTILS_H
