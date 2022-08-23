#include <fcntl.h>
#include <errno.h>

int ft_dup(int oldfd) {
    int fd = fcntl(oldfd, F_DUPFD, 0);
    if(fd == -1) {
        errno = EBADF;
    }
    return fd;
}

int ft_dup_2(int oldfd, int newfd) {
    if(oldfd == newfd) {
        if(fcntl(oldfd, F_GETFL) == -1) {
            errno = EBADF;
            return -1;
        }
        return oldfd;
    }
    int fd = fcntl(oldfd, F_DUPFD, newfd);
    if(fd == -1 || fd != newfd) {
        errno = EBADF;
        return -1;
    }
}

/*
    Test section
*/

#include <unistd.h>
#include <stdio.h>
#include <string.h>

int main() {
    int tmp_fd = ft_dup(STDOUT_FILENO);
    if(tmp_fd == -1) {
        printf("%s\n", strerror(errno));
    } else {
        char* check_dup = "check dup\n";
        write(tmp_fd, check_dup, strlen(check_dup));
    }
    
    tmp_fd = ft_dup_2(tmp_fd, 100);
    if(tmp_fd == -1) {
        printf("%s\n", strerror(errno));
    } else {
        char* check_dup = "check dup2\n";
        write(tmp_fd, check_dup, strlen(check_dup));
    }

    tmp_fd = ft_dup(-1);
    if(tmp_fd == -1) {
        printf("%s\n", strerror(errno));
    } else {
        char* check_dup = "check dup\n";
        write(tmp_fd, check_dup, strlen(check_dup));
    }

    tmp_fd = ft_dup_2(-1, -1);
    if(tmp_fd == -1) {
        printf("%s\n", strerror(errno));
    } else {
        char* check_dup = "check dup2\n";
        write(tmp_fd, check_dup, strlen(check_dup));
    }
}