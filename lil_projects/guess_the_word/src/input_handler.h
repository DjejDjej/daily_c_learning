#ifndef GUESS_INPUT_HANDLER_h
#define GUESS_INPUT_HANDLER_h
#include <stdbool.h>

#include <string.h>
#define INPUT_BUFF_DEF_SIZE 10

enum status_code{
OK = 0,
INVALID_INPUT,
INPUT_ERROR,
MALLOC_FAILED,
REALLOC_FAILED,
BLANK_INPUT,
};

char *load_user_input(bool *full_sentence,int *status_code) ;

#endif
