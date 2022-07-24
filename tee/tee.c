#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <error_handler.h>

struct t_file {
    char*   filename;
    int     fd;
};

struct t_files {
    struct t_file*  files_arr;
    int             files_count;
};

typedef struct t_file s_file;
typedef struct t_files s_files;

static e_error open_files(s_files* files, int open_flags, int open_rights) {
    int fd;
    for(int i = 0; i < files->files_count; ++i) {
        fd = open(files->files_arr[i].filename, open_flags, open_rights);
        if(fd == -1) {
            return EE_OPEN_ERROR;
        }
        files->files_arr[i].fd = fd;
    }
    return EE_SUCCESS;
}

static int contain_opt(char* options, char opt) {
    for(int i = 0; i < (int)strlen(options); ++i) {
        if(options[i] == opt) {
            return 0;
        }
    }
    return -1;
}

static e_error add_option(char** options, char new_opt) {
    int old_len = strlen(*options);
    char* new_opts = calloc(sizeof(char), old_len + 2);
    if(new_opts == NULL) {
        return EE_MEMORY_ERROR;
    }
    for(int i = 0; i < old_len; ++i) {
        new_opts[i] = (*options)[i];
    }
    new_opts[old_len] = new_opt;
    free(*options);
    *options = new_opts;
    return EE_SUCCESS;
}

static e_error parse_args(int argc, char** argv, s_files* files, char** options) {
    *options = calloc(sizeof(char), 1);
    files->files_arr = calloc(sizeof(s_file), argc - 1);
    if(options == NULL || files == NULL) {
        return EE_MEMORY_ERROR;
    }
    for(int i = 1; i < argc; ++i) {
        // treat as option
        if(argv[i][0] == '-') {
            check(add_option(options, argv[i][1]));
        } else {
            // treat as filename
            files->files_arr[files->files_count].filename = argv[i];
            ++files->files_count;
        }
    }
    return EE_SUCCESS;
}

static e_error process_input(s_files* files) {
    char* buf = NULL;
    int was_read = 0;
    size_t n;
    while((was_read = getline(&buf, &n, stdin)) != -1) {
        for(int i = 0; i < files->files_count; ++i) {
            if(write(files->files_arr[i].fd, buf, was_read) != was_read) {
                return EE_WRITE_ERROR;
            }
        }
        if(write(STDOUT_FILENO, buf, was_read) != was_read) {
            return EE_WRITE_ERROR;
        }
        free(buf);
        buf = NULL;
    }
    free(buf);
    return EE_SUCCESS;
}

static void free_all(s_files* files, char* options) {
    for(int i = 0; i < files->files_count; ++i) {
        close(files->files_arr[i].fd);
    }
    free(files->files_arr);
    free(options);
}

int main(int argc, char** argv) {
#pragma message("TODO: Add tests")
    if(argc < 2) {
        printf("Usage: %s filename\noptional args:\n-a append to the end of file\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    char* options = NULL;
    int open_flags = 0;
    int open_rights = 0;
    s_files files = {NULL, 0};
    /* creating arrays of files and of options */
    check(parse_args(argc, argv, &files, &options));
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
    check(open_files(&files, open_flags, open_rights));
    /*read input and wite to files and*/
    check(process_input(&files));
    /* close files, free everything */
    free_all(&files, options);
    exit(EXIT_SUCCESS);
}