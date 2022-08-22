#ifndef ERROR_HANDLER
# define ERROR_HANDLER

/*To add new error add new enum value + add new description in check function*/

enum e_error {
    EE_SUCCESS,
    EE_MEMORY_ERROR,
    EE_OPEN_ERROR,
    EE_READ_ERROR,
    EE_WRITE_ERROR,
    EE_LSEEK_ERROR
};

typedef enum e_error e_error;

void check(e_error error_code);

#endif