#include "error_handler.h"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

static void print_error_exit(const char* err)
{
    write(STDERR_FILENO, err, strlen(err));
    exit(EXIT_FAILURE);
}

void check(e_error error_code)
{
    switch(error_code)
    {
        case EE_SUCCESS:
            return;

        case EE_MEMORY_ERROR:
            print_error_exit("Error: memory allocation failure\n");
            break;
    
        case EE_OPEN_ERROR:
            print_error_exit("Error: open call failed\n");
            break;
        
        case EE_READ_ERROR:
            print_error_exit("Error: read call failed\n");
            break;

        case EE_WRITE_ERROR:
            print_error_exit("Error: write call failed\n");
            break;

        case EE_LSEEK_ERROR:
            print_error_exit("Error: lseek call failed\n");
            break;

        default:
            break;
    }
}