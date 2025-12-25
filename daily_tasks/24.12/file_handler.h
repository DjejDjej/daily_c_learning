#ifndef FILE_HANDLER_H 
#include <stdio.h>   // FILE
#include <stdbool.h> // bool
#define FILE_HANDLER_H 

enum status_codes{
FH_OK = 0,
FH_MALLOC_FAILED = 1,
FH_EOF = 2,
FH_ERR = 3,
FH_FILE_OPEN_ERR = 4,
FH_CLOSED = 5,


};


struct file_handle {
  FILE *file;
};

struct file_handle *fh_open(const char *path, const char *mode, int *status);
int fh_read_char(struct file_handle *fh, int *out);
int fh_close(struct file_handle **fh);

#endif
