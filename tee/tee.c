#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#define MEMORY_ALLOC_ERR -1

struct t_file {
    char*   filename;
    int     fd;
};

typedef struct t_file s_file;

static int open_files(s_file* files, int open_flags, int open_rights, int files_count) {
    int fd;
    for(int i = 0; i < files_count; ++i) {
        fd = open(files[i].filename, open_flags, open_rights);
        if(fd == -1) {
            return -1;
        }
        files[i].fd = fd;
    }
    return 0;
}

static int contain_opt(char* options, char opt) {
    for(int i = 0; i < (int)strlen(options); ++i) {
        if(options[i] == opt) {
            return 0;
        }
    }
    return -1;
}

static int add_option(char** options, char new_opt) {
    int old_len = strlen(*options);
    char* new_opts = calloc(sizeof(char), old_len + 2);
    if(new_opts == NULL) {
        return MEMORY_ALLOC_ERR;
    }
    for(int i = 0; i < old_len; ++i) {
        new_opts[i] = (*options)[i];
    }
    new_opts[old_len] = new_opt;
    free(*options);
    *options = new_opts;
    return 0;
}

static int parse_args(int argc, char** argv, s_file** files, char** options) {
    *options = calloc(sizeof(char), 1);
    *files = calloc(sizeof(s_file), argc - 1);
    int file_counter = 0;
    if(options == NULL || files == NULL) {
        return MEMORY_ALLOC_ERR;
    }
    for(int i = 1; i < argc; ++i) {
        // treat as option
        if(argv[i][0] == '-') {
            if(add_option(options, argv[i][1]) == -1) {
                return MEMORY_ALLOC_ERR;
            }
        } else {
            // treat as filename
            (*files)[file_counter].filename = argv[i];
            ++file_counter;
        }
    }
    return file_counter;
}

static int process_input(s_file* files, int files_count) {
    char* buf = NULL;
    int was_read = 0;
    size_t n;
    while((was_read = getline(&buf, &n, stdin)) != -1) {
        for(int i = 0; i < files_count; ++i) {
            if(write(files[i].fd, buf, was_read) != was_read) {
                return -1;
            }
        }
        if(write(STDOUT_FILENO, buf, was_read) != was_read) {
            return -1;
        }
        free(buf);
        buf = NULL;
    }
    if(buf != NULL) {
        free(buf);
    }
    return 0;
}

static void free_all(s_file* files, char* options, int files_count) {
    for(int i = 0; i < files_count; ++i) {
        close(files[i].fd);
    }
    free(files);
    free(options);
}

int main(int argc, char** argv) {
#pragma message("TODO: Add common error-handling mechanism")
#pragma message("TODO: Add makefile")
#pragma message("TODO: Add tests")
#pragma message("TODO: Add file array wrapper")
    if(argc < 2) {
        printf("Usage: %s filename\noptional args:\n-a append to the end of file\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    char* options = NULL;
    s_file* files = NULL;
    int open_flags = 0;
    int open_rights = 0;
    int files_count = 0;
    /* creating arrays of files and of options */
    files_count = parse_args(argc, argv, &files, &options);
    if(files_count <= 0) {
        exit(EXIT_FAILURE);
    }
    /* default flags for any file */
    open_flags = O_CREAT | O_WRONLY;
    /* check if we have to truncate or append strigs to files */
    if(contain_opt(options, 'a') == 0) {
        open_flags |= O_APPEND;
    } else {
        open_flags |= O_TRUNC;
    }
    /* file rights (-rw-rw-rw) */
    open_rights = S_IRUSR | S_IWUSR |
                S_IRGRP | S_IWGRP |
                S_IROTH | S_IWOTH;
    if(open_files(files, open_flags, open_rights, files_count) != 0) {
        printf("Files open error\n");
        exit(EXIT_FAILURE);
    }
    /*read input and wite to files and*/
    process_input(files, files_count);
    /* close files, free everything */
    free_all(files, options, files_count);
    exit(EXIT_SUCCESS);
}