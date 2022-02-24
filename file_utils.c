#include "file_utils.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>      // for stdout redirection
#include <sys/stat.h>

bool is_file_exists(const char* filename)
{
    if (access(filename, F_OK) == 0)
    {
        return true;
    }
    return false;
}

bool is_file_empty(FILE* file)
{
    fseek (file, 0, SEEK_END);  // go to end of file
    size_t size = ftell(file);
    fseek(file, 0, SEEK_SET);   // return to begin of file

    if (size == 0) { return true; }
    return false;
}

void dir_name(char* path, char* dir)
{
    int dir_len = strrchr(path, '/') - path;
    strncpy(dir, path, dir_len);
    dir[dir_len] = '\0';
}

bool redirect_stdout(char* path)
{
    // create directory
    char directory[1024];
    dir_name(path, directory);

    struct stat st = {0};
    if (stat(directory, &st) == -1)
    {
        if (mkdir(directory, 0700) != 0)
        {
            return false;
        }
    }

    int out_file = open(path, O_WRONLY|O_CREAT, 0644);
    if (out_file == -1)
    {
        return false;
    }
    if (dup2(out_file, 1) == -1)
    {
        return false;
    }

    return true;
}

FILE* open_file(char* filename)
{
    if (!is_file_exists(filename))
    {
        return NULL;
    }

    // try to open input file
    FILE* file = fopen(filename, "r");
    if (file == NULL)
    {
        return NULL;
    }

    // check if input file is empty
    if (is_file_empty(file))
    {
        fclose(file);
        return NULL;
    }

    return file;
}
