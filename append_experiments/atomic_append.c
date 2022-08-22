#include <error_handler.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

static void usage(char* progname) {
    printf("usage: %s filename num-bytes [x]", progname);
}

int main(int argc, char** argv) {
    if(argc != 3 && argc != 4) {
        usage(argv[0]);
    }
    int open_flags = 0;
    mode_t open_mode = 0;
    char* filename = argv[1];
    int num_bytes = atoi(argv[2]);

    open_flags |= O_RDWR;
    open_mode |= S_IRUSR | S_IRGRP | S_IROTH |
                S_IWUSR | S_IWGRP | S_IWOTH;
    if(argc == 3 || (strcmp(argv[3], "x")) != 0) {
        open_flags |= O_APPEND;
    }
    int fd = open(filename, open_flags, open_mode);
    if(fd == -1) {
        check(EE_OPEN_ERROR);
    }
    for(int i = 0; i < num_bytes; ++i) {
        off_t ofs = lseek(fd, 0, SEEK_END);
        if(ofs == -1) {
            check(EE_LSEEK_ERROR);
        }
        int num_written = write(fd, "\0", 1);
        if(num_written != 1) {
            check(EE_WRITE_ERROR);
        }
    }
    exit(EXIT_SUCCESS);
}
